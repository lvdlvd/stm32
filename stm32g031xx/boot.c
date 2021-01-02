#include "cortex_m0.h"
#include "stm32g031.h"

extern void main(void);                            // in main.c
extern void (*vector_table[])(void);               // in vector.c
extern char _sidata, _sdata, _edata, _sbss, _ebss; // provided by linker script

void Reset_Handler(void) {
	char* src = &_sidata;
	char* dst = &_sdata;

	while (dst < &_edata)
		*dst++ = *src++;

	for (dst = &_sbss; dst < &_ebss; dst++)
		*dst = 0;

	SCB.VTOR = (uintptr_t)&vector_table; /* Vector Table Relocation in Internal FLASH. */

	/* Reset the RCC clock configuration to the default reset state(for debug purpose) */
	RCC.CR |= RCC_CR_HSION;
	rcc_cfgr_set_mcosel(&RCC, 0);
	rcc_cfgr_set_mcopre(&RCC, 0);
	rcc_cfgr_set_ppre(&RCC, 0);
	rcc_cfgr_set_hpre(&RCC, 0);
	RCC.CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON | RCC_CR_HSEBYP);
	rcc_cr_set_hsidiv(&RCC, 0);
	rcc_cfgr_set_sws(&RCC, 0);

	/* Disable all interrupts and clear pending bits  */
	RCC.CIER = 0;
	RCC.CICR = RCC.CIFR;

	stk_rvr_set_reload(&STK, STK_RVR_RELOAD);
	STK.CSR |= STK_CSR_CLKSOURCE|STK_CSR_TICKINT|STK_CSR_ENABLE;

	main();

	for (;;)
		__NOP(); // hang
}
