#include "cortex_m4.h"
#include "stm32g4xx.h"

#include "clock.h"

extern void main(void) __attribute__((noreturn));	// in main.c
extern void (*const vector_table[])(void);			// in vector.c
extern char _sidata, _sdata, _edata, _sbss, _ebss;	// provided by linker script

enum { 
	HSE_FREQ_MHZ 	= 8,     // nucleo board: 24MHz, canard: 8MHz
	HSE_RDY_TIMEOUT = 5000, 
};

void Reset_Handler(void) {
	char *src = &_sidata;
	char *dst = &_sdata;

	while (dst < &_edata) {
		*dst++ = *src++;
	}

	for (dst = &_sbss; dst < &_ebss; dst++) {
		*dst = 0;
	}

	SCB.VTOR = (uintptr_t)&vector_table; /* Vector Table Relocation in Internal FLASH. */

	SCB.CCR |= SCB_CCR_DIV_0_TRP;  // division by zero causes trap
	// enable usage/bus/mem fault separate handlers.
	SCB.SHCSR |= SCB_SHCSR_USGFAULTENA | SCB_SHCSR_BUSFAULTENA | SCB_SHCSR_MEMFAULTENA;

	fpu_cpacr_cpacr_set_cp(&FPU_CPACR, 0xf); /* CP10/CP11 Full Access */

	/* Disable all interrupts and clear pending bits  */
	RCC.CIER = 0;
	RCC.CICR = RCC.CIFR;

	RCC.CR |= RCC_CR_HSEON;
	for (int i = 0; i < HSE_RDY_TIMEOUT; i++) {
		if (RCC.CR & RCC_CR_HSERDY) {
			break;
		}
	}

	RCC.APB1ENR1 |= RCC_APB1ENR1_PWREN;
	pwr_cr1_set_vos(&PWR, 1);	 // Voltage Scaling range 1 (fast)
	PWR.CR5 &= ~PWR_CR5_R1MODE;	 // bit off = boost mode.

	// USB Type-C and Power Delivery Dead Battery disable. After exiting reset, the USB Type-C “dead battery” behavior is enabled,
	// which may have a pull-down effect on CC1 and CC2 pins. It is recommended to disable it in all cases, either to stop this
	// pull-down or to hand over control to the UCPD1 (which should therefore be initialized before doing the disable).
	PWR.CR3 |= PWR_CR3_UCPD1_DBDIS;

	// Configure the main PLL
	if ((RCC.CR & RCC_CR_HSERDY) == 0) {
		// HSE clock failed to become ready
		rcc_pllcfgr_set_pllsrc(&RCC, 2);  // select 2:HSI source (16MHz)
		rcc_pllcfgr_set_pllm(&RCC, 1);	  // 0..15       : vco_in = HSI / (1+m)  2..16MHz   16/2 = 8MHz
	} else {
		rcc_pllcfgr_set_pllsrc(&RCC, 3);  // select 3:HSE source (8 or 24MHz)
		rcc_pllcfgr_set_pllm(&RCC, (HSE_FREQ_MHZ/8)-1);	  // 0..15       : vco_in = HSE / (1+m)  2..16MHz    24/3 = 8MHz
	}

	rcc_pllcfgr_set_plln(&RCC, 36);	 // 8...127     : vco_out = vco_in * n = 96...344MHz    8 * 36 = 288MHz
	rcc_pllcfgr_set_pllr(&RCC, 0);	 // 0,1,2,3 -> p=2,4,6,8  : sysclk = vco_out / p <= 170MHz  8 * 36 / 2 = 144MHz
//	rcc_pllcfgr_set_pllq(&RCC, 2);   // 0,1,2,3 -> p=2,4,6,8  :   8 * 36 / 6 =  48MHz

//	RCC.PLLCFGR |= RCC_PLLCFGR_PLLREN | RCC_PLLCFGR_PLLQEN;
	RCC.PLLCFGR |= RCC_PLLCFGR_PLLREN;
	RCC.CR |= RCC_CR_PLLON;

//	rcc_ccipr_set_clk48sel(&RCC, 2);  // 7.4.26, p.328:  0: HSI48, 2:PLLQ other reserved

	FLASH.ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;
	flash_acr_set_latency(&FLASH, 4);  // 4 wait states (5 cycles) cpf p.150 table 19

	while (flash_acr_get_latency(&FLASH) != 4) {
		__NOP();
	}

	// Wait till PLL is ready
	while ((RCC.CR & RCC_CR_PLLRDY) == 0) {
		__NOP();
	}

	// prepare periphperal clocks: page 283: set to 72MHz first
	rcc_cfgr_set_hpre(&RCC, 8);	 // AHB HCLK = SYSCLK/2 =  72MHz
	// After a write operation to these bits and before decreasing the voltage range,
	// this register must be read to be sure that the new value has been taken into account.
	while (rcc_cfgr_get_hpre(&RCC) != 8) {
		__NOP();
	}

	rcc_cfgr_set_ppre1(&RCC, 0);  // APB1 PCLK = AHB HCLK
	rcc_cfgr_set_ppre2(&RCC, 0);  // APB2 PCLK = AHB HCLK


	rcc_cfgr_set_sw(&RCC, 3);  // Select PLL as system clock source

	// Wait till PLL is used as system clock source
	while (rcc_cfgr_get_sws(&RCC) != 3) {
		__NOP();
	}

	stk_load_set_reload(&STK, STK_LOAD_RELOAD);
	stk_val_set_current(&STK, STK_LOAD_RELOAD);
	STK.CTRL |= STK_CTRL_CLKSOURCE | STK_CTRL_TICKINT | STK_CTRL_ENABLE;

	delay(1);

	rcc_cfgr_set_hpre(&RCC, 0);	 // AHB HCLK = SYSCLK =  144MHz

	main();
}

volatile uint64_t clockticks = STK_LOAD_RELOAD + 1;

void SysTick_Handler(void) { clockticks += STK_LOAD_RELOAD + 1; }

void delay(uint32_t usec) {
	uint64_t now = cycleCount();
	// then = now + usec * clockspeed_hz / (usec/sec)
	uint64_t then = CLOCKSPEED_HZ;
	then *= usec;
	then /= 1000000;
	then += now;
	while (cycleCount() < then) {
		__NOP();  // wait
	}
}
