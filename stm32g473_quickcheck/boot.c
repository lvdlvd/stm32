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

	// prepare the flash
	FLASH.ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

	// Prepare the Cortex system timer (BEFORE calling delay()!)
	stk_load_set_reload(STK_LOAD_RELOAD);  // maximum value
	stk_val_set_current(STK_LOAD_RELOAD);
	STK.CTRL |= STK_CTRL_CLKSOURCE | STK_CTRL_TICKINT | STK_CTRL_ENABLE;

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
