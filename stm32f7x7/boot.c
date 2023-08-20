#include "cortex_cm7.h"
#include "stm32f7x7.h"

extern void main(void) __attribute__ ((noreturn)); // in main.c
extern void (*vector_table[])(void);               // in vector.c
extern char _sidata, _sdata, _edata, _sbss, _ebss; // provided by linker script

static inline void systemInit(void) {
	fpu_cpacr_cpacr_set_cp(&FPU_CPACR, 0xf); /* CP10/CP11 Full Access */

	/* Reset the RCC clock configuration to the default reset state(for debug purpose) */
	RCC.CR |= RCC_CR_HSION;
	RCC.CFGR = 0;
	RCC.CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON | RCC_CR_HSEBYP);

	/* Disable all interrupts and clear pending bits  */
	RCC.CIR = (RCC.CIR & 0xff) << 16;
}

enum {
	HSE_RDY_TIMEOUT = 5000,
};

static int setSysClockTo144MHz(void) {

	// nucleo board uses 8MHz MCO of the st-link as clock source
	RCC.CR |= RCC_CR_HSEON | RCC_CR_HSEBYP; 

	for (int i = 0; i < HSE_RDY_TIMEOUT; i++)
		if (RCC.CR & RCC_CR_HSERDY)
			break;

	rcc_cfgr_set_hpre(&RCC, 0);  // AHB HCLK = SYSCLK =  144MHz
	rcc_cfgr_set_ppre1(&RCC, 5); // APB1 PCLK = AHB HCLK / 4  = 36 not to exceed 45MHz
	rcc_cfgr_set_ppre2(&RCC, 4); // APB2 PCLK = AHB HCLK / 2  = 72 not to exceed 90MHz

	// Configure the main PLL 
	if (RCC.CR & RCC_CR_HSERDY) {
		RCC.PLLCFGR |= RCC_PLLCFGR_PLLSRC;
		rcc_pllcfgr_set_pllm(&RCC, 4);     // 2..63       : vco_in = HSE / m 1..2 MHz    8/4 = 2MHz
	}  else {
		// HSE clock failed to become ready
		rcc_pllcfgr_set_pllm(&RCC, 8);     // 2..63       : vco_in = HSI / m  1..2MHz   16/8 = 2MHz
	}

	rcc_pllcfgr_set_plln(&RCC, 144);	// 50...423   : vco_out = vco_in * n =  100..432 MHz   (288 Mhz) 
	rcc_pllcfgr_set_pllp(&RCC, 0); 	// 0,1,2,3 -> r=2,4,6,8  : sysclk = vco_out / r <= 192MHz  (144 Mhz)
	rcc_pllcfgr_set_pllq(&RCC, 6); 	// q=2..15    : usb clock = vco_out / q  = 48MHz
	rcc_pllcfgr_set_pllr(&RCC, 6); 	// r=2..7     : dsi clock = vco_out / r  ??
	RCC.CR |= RCC_CR_PLLON;

	// prepare the flash	
	FLASH.ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ARTEN;
	// Voltage range 2.7 V - 3.6 V: 4 wait states (5 cycles) cf p.91 table 7
	flash_acr_set_latency(&FLASH, 4);

	while(flash_acr_get_latency(&FLASH) != 4)
		__NOP();

	// Wait till PLL is ready
	while ((RCC.CR & RCC_CR_PLLRDY) == 0)
		__NOP();

	rcc_cfgr_set_sw(&RCC, 2); // Select PLL as system clock source

	// Wait till PLL is used as system clock source
	while (rcc_cfgr_get_sws(&RCC) != 2)
		__NOP();

	return 1;
}

void Reset_Handler(void) {
	char* src = &_sidata;
	char* dst = &_sdata;

	while (dst < &_edata)
		*dst++ = *src++;

	for (dst = &_sbss; dst < &_ebss; dst++)
		*dst = 0;

	SCB.VTOR = (uintptr_t)&vector_table; /* Vector Table Relocation in Internal FLASH. */

	SCB.CCR |= SCB_CCR_DIV_0_TRP; // division by zero causes trap
	// enable usage/bus/mem fault separate handlers.
	SCB.SHCSR |= SCB_SHCSR_USGFAULTENA|SCB_SHCSR_BUSFAULTENA|SCB_SHCSR_MEMFAULTENA;

	systemInit();

	while (!setSysClockTo144MHz())
	 	__NOP();

	stk_rvr_set_reload(&STK, STK_RVR_RELOAD);
	stk_cvr_set_current(&STK, STK_RVR_RELOAD);
	STK.CSR |= STK_CSR_CLKSOURCE | STK_CSR_TICKINT | STK_CSR_ENABLE;

	main();
}
