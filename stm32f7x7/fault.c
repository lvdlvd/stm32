#include "cortex_cm7.h"
#include "stm32f7x7.h"
#include <stdarg.h>
#include <stddef.h>

// TODO: we assume USART2 is up and running and clocks are sane
#if 0
static inline void setSysclockToHSI16(void) {
	RCC.CR |= RCC_CR_HSION;
	while ((RCC.CR & RCC_CR_HSIRDY) == 0)
		__NOP();

	rcc_cfgr_set_sws(&RCC, 1);
	while (rcc_cfgr_get_sws(&RCC) != 1)
		__NOP();

	RCC.CFGR &= ~(RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1 | RCC_CFGR_HPRE);

	USART2.CR1 = 0;
	USART2.CR2 = 0;
	USART2.CR3 = 0;

	USART2.BRR = 139; // 115107 Bd
	USART2.CR1 |= USART_CR1_UE | USART_CR1_TE;
}
#endif

static void putc_(char c) {
	while ((USART2.ISR & USART1_ISR_TXE) == 0)
		__NOP();
	USART2.TDR = c;
}

// dbg printf can ony handle %s and %x
static void dbg_printf(const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);
	char        c;
	const char* s;
	uint32_t    x;
	while ((c = *fmt++) != 0) {
		if (c != '%') {
			putc_(c);
			continue;
		}
		if ((c = *fmt++) == 0) {
			break;
		}
		switch (c) {
		case 's':
			s = va_arg(va, const char*);
			while (*s) {
				putc_(*s++);
			}
			break;
		case 'x':
			x = va_arg(va, uint32_t);
			for (int i = 0; i < 8; ++i) {
				putc_("0123456789abcdef"[(x >> 4 * (7 - i)) & 0xf]);
			}
			break;
		default:
			putc_('%');
			putc_(c);
		}
	}
	va_end(va);
}

static const char* const regname[] = {"r0", "r1", "r2", "r3", "r12", "lr", "pc", "psr"};
static void        dumpstack(uint32_t* stack) {
    for (int i = 0; i < 8; ++i) {
        dbg_printf(" %s = 0x%x\n", regname[i], stack[i]);
    }
}

static void printflags(uint32_t val, const char* const* flgs, const char* sep) {
	int n = 0;
	while (*flgs && val) {
		if ((val & 1) && **flgs) {
			dbg_printf("%s%s", (n++ ? sep : ""), *flgs);
		}
		val >>= 1;
		++flgs;
	}
}

static const char* const cfsrflags[] = {
    "Instruction access violation",
    "Data access violation",
    "",
    "Memmgr fault on unstacking for a return from exception",
    "Memmgr fault on stacking for exception entry",
    "Memmgr fault on floating-point lazy state preservation",
    "",
    "Memmgr Fault Address Register valid",
    "Instruction bus error",
    "Precise data bus error",
    "Imprecise data bus error",
    "Bus fault on unstacking for a return from exception",
    "Bus fault on stacking for exception entry",
    "Bus fault on floating-point lazy state preservation",
    "",
    "Bus Fault Address Register valid",
    "Undefined instruction",
    "Invalid state",
    "Invalid PC load",
    "No coprocessor",
    "",
    "",
    "",
    "",
    "Unaligned access",
    "Divide by zero",
    NULL,
};

static void handlefault(uint32_t stack, const char* lbl) __attribute__ ((noreturn));

static void handlefault(uint32_t stack, const char* lbl) {
	// setSysclockToHSI16();
	uint32_t cfsr = SCB.CFSR_UFSR_BFSR_MMFSR;
	dbg_printf("%s FAULT CFSR: %x\n", lbl, cfsr);
	dumpstack((uint32_t*)stack); 
	if (cfsr & SCB_CFSR_UFSR_BFSR_MMFSR_MMARVALID) {
		dbg_printf("mem fault address = 0x%x\n", SCB.MMFAR);
	}
	if (cfsr & SCB_CFSR_UFSR_BFSR_MMFSR_BFARVALID) {
		dbg_printf("bus fault address = 0x%x\n", SCB.BFAR);
	}
	if (SCB.HFSR & SCB_HFSR_FORCED) {
		dbg_printf("forced hard fault\n");
	}
	if (SCB.HFSR & SCB_HFSR_VECTTBL) {
		dbg_printf("vector table hard fault\n");
	}
	printflags(cfsr, cfsrflags, "\n");
	dbg_printf("\n------HALT\n");
	for (;;)
		__NOP();
}

void HardFault_Handler(void) __attribute__ ((naked));
void MemoryManagement_Handler(void) __attribute__ ((naked));
void BusFault_Handler(void) __attribute__ ((naked));
void UsageFault_Handler(void) __attribute__ ((naked));

void HardFault_Handler(void) { handlefault(__get_MSP(), "HARD"); }
void MemoryManagement_Handler(void) { handlefault(__get_MSP(), "MEM"); }
void BusFault_Handler(void) { handlefault(__get_MSP(), "BUS"); }
void UsageFault_Handler(void) { handlefault(__get_MSP(), "USAGE"); }


// called from vector.c default_IRQ_Handler
void unhandled_interrupt(uint32_t irq) {
	// setSysclockToHSI16();

	dbg_printf("Unhandled IRQ %x\n----HALT\n", irq);
	__BKPT(1);
	for (;;)
		__NOP();

}
