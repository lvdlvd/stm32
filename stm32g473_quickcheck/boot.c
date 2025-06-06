#include "arm_cm4.h"
#include "stm32g4xx.h"

#include <assert.h>
#include "clock.h"

extern void main(void) __attribute__((noreturn));	// in main.c
extern void (*const vector_table[])(void);		// in vector.c
extern uint32_t _etext, _sidata, _eidata, _sdata, _edata, _sbss, _ebss;	// provided by linker script

enum {
	HSE_FREQ_MHZ	= 8,
	HSE_RDY_TIMEOUT = 5000,
};

// The system starts operation here after a reset through the interrupt vector table.
// The stack pointer will have been set, and we are running on the HSI clock, but other than
// that we can make no assumptions.
// This function initializes the data and bss segments, sets up the clock tree and initializes
// the SYSTICK builtin peripheral to provide cycleCount() as a global time reference, before
// transferring control to main()
void Reset_Handler(void) {

	// Copy initialized data segment to RAM (see stm32XXXX.ld)
	uint32_t *src = &_sidata;
	uint32_t *dst = &_sdata;

	while (dst < &_edata) {
		*dst++ = *src++;
	}

	// clear BSS segment (see stm32XXXX.ld)
	for (dst = &_sbss; dst < &_ebss; dst++) {
	    *dst = 0;
	}

	// Vector Table Relocation to from flash or ram1 mapped to 00000000
	// PM0214 sec 4.4.4
	// NOTE, without this vector_table may not be referenced, and will only be included
	// if the linker is invoked with flag --undefined=vector_table or
	// EXTERN(vector_table) in the linker script.
	SCB.VTOR = (uintptr_t)&vector_table;  // provided in vectors.c	

	SCB.CCR |= SCB_CCR_DIV_0_TRP;  // division by zero causes trap
	// enable usage/bus/mem fault separate handlers.
	SCB.SHCSR |= SCB_SHCSR_USGFAULTENA | SCB_SHCSR_BUSFAULTENA | SCB_SHCSR_MEMFAULTENA;

	fpu_cpacr_cpacr_set_cp(0xf);  // CP10/CP11 Full Access

	rcc_cfgr_set_mcosel(0);	 // no mco signal out
	rcc_cfgr_set_mcopre(0);

	// Reset the RCC clock configuration to the default reset state
	RCC.CR |= RCC_CR_HSION;
	rcc_cfgr_set_mcosel(0);
	rcc_cfgr_set_mcopre(0);
	rcc_cfgr_set_ppre1(0);
	rcc_cfgr_set_ppre2(0);
	rcc_cfgr_set_hpre(0);
	RCC.CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON | RCC_CR_HSEBYP);
	rcc_cfgr_set_sws(1);  // HSI16 as clock source

	// Disable all interrupts and clear pending bits
	RCC.CIER = 0;
	RCC.CICR = RCC.CIFR;

	// set system clock to PLL 168 MHz

	RCC.CR |= RCC_CR_HSEON;
	for (int i = 0; i < HSE_RDY_TIMEOUT; i++) {
		if (RCC.CR & RCC_CR_HSERDY) {
			break;
		}
	}

	// Power control
	RCC.APB1ENR1 |= RCC_APB1ENR1_PWREN;
	pwr_cr1_set_vos(1);			 // Voltage Scaling range 1 (fast)
	PWR.CR5 &= ~PWR_CR5_R1MODE;	 // bit off = boost mode, only when sysclock > 150MHz

	// prepare periphperal clocks: page 283: set to 84MHz first
	rcc_cfgr_set_hpre(8);  // AHB HCLK = SYSCLK/2 =  84MHz

	// After a write operation to these bits and before decreasing the voltage range,
	// this register must be read to be sure that the new value has been taken into account.
	while (rcc_cfgr_get_hpre() != 8) {
		// __NOP();
	}

	rcc_cfgr_set_ppre1(0);	// APB1 PCLK = AHB HCLK
	rcc_cfgr_set_ppre2(0);	// APB2 PCLK = AHB HCLK

	// Configure the main PLL
	if (RCC.CR & RCC_CR_HSERDY) {
		rcc_pllcfgr_set_pllsrc(3);					   // select 3:HSE source (multiple of 8MHz)
		rcc_pllcfgr_set_pllm((HSE_FREQ_MHZ / 8) - 1);  // 0..15       : vco_in = HSE / (1+m)  2..16MHz        24/3 = 8MHz
	} else {
		// HSE clock failed to become ready
		rcc_pllcfgr_set_pllsrc(2);	// select 2:HSI source (16MHz)
		rcc_pllcfgr_set_pllm(1);	// 0..15       : vco_in = HSI / (1+m)  2..16MHz        16/2 = 8MHz
	}

	rcc_pllcfgr_set_plln(42);  // 8...127     : vco_out = vco_in * n = 96...344MHz    8 * 42 =  336 MHz
	rcc_pllcfgr_set_pllr(0);   // 0,1,2,3 -> p=2,4,6,8  : sysclk = vco_out / p <= 170MHz  8 * 42 / 2 = 168 MHz
	rcc_pllcfgr_set_pllq(2);   // 0,1,2,3 -> p=2,4,6,8  :   8 * 42 / 6 =  56MHz

	RCC.PLLCFGR |= RCC_PLLCFGR_PLLREN | RCC_PLLCFGR_PLLQEN;
	RCC.CR |= RCC_CR_PLLON;

	// set the FDCAN clock to PLLQ at 56MHz, see 7.4.26, p.328
	rcc_ccipr_set_fdcansel(1);	// 0:HSE,  1:PLLQ  2:PLCLK

	// prepare the flash
	FLASH.ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;
	flash_acr_set_latency(4);  // 4 wait states (5 cycles) cpf p.150 table 19

	while (flash_acr_get_latency() != 4) {
		// __NOP();
	}

	// Wait till PLL is ready
	while ((RCC.CR & RCC_CR_PLLRDY) == 0) {
		// __NOP();
	}

	// if HSE is the pll source, enable clock security system to generate NMI when HSE fails
	// if (RCC.CR & RCC_CR_HSERDY) { RCC.CR |= RCC_CR_CSSON; }

	// Select PLL as system clock source
	rcc_cfgr_set_sw(3);
	while (rcc_cfgr_get_sws() != 3) {
		// __NOP();
	}

	// Prepare the Cortex system timer (BEFORE calling delay()!)
	stk_load_set_reload(STK_LOAD_RELOAD);  // maximum value
	stk_val_set_current(STK_LOAD_RELOAD);
	STK.CTRL |= STK_CTRL_CLKSOURCE | STK_CTRL_TICKINT | STK_CTRL_ENABLE;

	// update periphperal clock (APB{1,2} = AHB = 168MHz)
	delay(1);			   // actually this runs still at 84MHz, but 2us is fine too
	rcc_cfgr_set_hpre(0);  // AHB HCLK = SYSCLK =  168MHz

	// USB Type-C and Power Delivery Dead Battery disable. After exiting reset, the USB Type-C dead batterybehavior is enabled,
	// which may have a pull-down effect on CC1 and CC2 pins. It is recommended to disable it in all cases, either to stop this
	// pull-down or to hand over control to the UCPD1 (which should therefore be initialized before doing the disable).
	PWR.CR3 |= PWR_CR3_UCPD1_DBDIS;

	main();

	assert(0);
}

volatile uint64_t clockticks = STK_LOAD_RELOAD + 1;	 // clocktics starts at 1 tick ahead.

// When this irq has a higher priority than all userspace, we may consider update to be atomic.
// clockticks holds the counts at the _end_ of the next systick,
// so cyclecount can just subtract current VAL (which counts down)
void SysTick_Handler(void) { clockticks += stk_load_get_reload() + 1; }
