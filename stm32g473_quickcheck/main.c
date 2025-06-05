/*

*/
#include <assert.h>

#include "arm_cm4.h"
#include "stm32g4xx.h"

#include "clock.h"
#include "fifo.h"
#include "gpio2.h"
#include "nvic.h"
#include "tprintf.h"

// Pinout
enum {

	USART3_TX_PIN = PB10, 

};

/* clang-format off */
static const struct gpio_config_t {
	enum GPIO_Pin  pins;
	enum GPIO_Conf mode;
} pin_cfgs[] = {
	{PAAll, GPIO_ANALOG},
	{PBAll, GPIO_ANALOG},
	{USART3_TX_PIN, 	GPIO_AF7_USART1 | GPIO_HIGH},
	{0, 0},	// sentinel
};

// Interrupt priorities (pm0214 section 4.4.5 and 4.3.7)
// prio[7:6] : 4 groups,  prio[5:4] : 4 subgroups
enum { IRQ_PRIORITY_GROUPING_2_2 = 5 }; 
#define PRIO(grp, sub) (((grp)<<6)|((sub)<<4))
static const struct {
    enum IRQn_Type irq;
    uint8_t        prio;
} irqprios[] = {
    { SysTick_IRQn,  	PRIO(0,0) },
	{ USART3_IRQn,		PRIO(1,3) },
    { TIM4_IRQn, 		PRIO(3,1) },	// 1Hz periodic status reports
    { None_IRQn, 0xff}, // sentinel
};
#undef PRIO


/* clang-format on */

// USART3
static uint8_t constx_buf[1024];
static struct Fifo constx = FIFO_INITIALIZER(constx_buf);
static uint32_t constx_ovfl = 0;

// print to fifo, for use by fctprintf
static void _fputc(char ch, void *serial) {
	struct Fifo *s = serial;
	if (fifo_full(s)) {
		fifo_reset(s);
		for (const char *p = "!OVFL!"; *p != 0; ++p) {
			fifo_put_head(s, *p);
		}
		++constx_ovfl;
	}
	fifo_put_head(s, ch);
	USART3.CR1 |= USART_CR1_TXEIE;
	return;
}

// print to console, usart3 tx (serial), used by tprintf
// \r\n transformation on plain printf to console:
// \r ignored, \n clears to end of line
void _putchar(char ch) {
	if (ch == '\r') {
		return;
	}
	if (ch == '\n') {
		_fputc('\e', &constx);
		_fputc('[', &constx);
		_fputc('K', &constx);
	}
	_fputc(ch, &constx);
}

// these are the only non-trivial defines, to adapt tprintf
// because making vararg adapters is more of a hassle
#define printf tprintf
// #define fprintf(s, fmt, ...) fctprintf(_fputc, s, fmt, __VA_ARGS__)

void USART3_Handler(void) { 
	if (!fifo_empty(&constx)) {
		if ((USART3.ISR & USART_ISR_TXE) != 0) {
			USART3.TDR = fifo_get_tail(&constx);
		}
	} else {
		USART3.CR1 &= ~USART_CR1_TXEIE;
	}
	return;
}

// mcu registers defined in  stm32g473.ld
extern uint32_t UNIQUE_DEVICE_ID[3];  		// Section 48.1

void main(void) {
	uint8_t rf = (RCC.CSR >> 24) & 0xfc;  // reset flags
	RCC.CSR |= RCC_CSR_RMVF;			  // Set RMVF bit to clear the reset flags

	// Enable all peripherals we'll be using.
	RCC.AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN;// | RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIODEN;
	RCC.APB1ENR1 |= RCC_APB1ENR1_USART3EN;

	// set up IRQ priorities.
	NVIC_SetPriorityGrouping(IRQ_PRIORITY_GROUPING_2_2);
	for (int i = 0; irqprios[i].irq != None_IRQn; i++) {
		NVIC_SetPriority(irqprios[i].irq, irqprios[i].prio);
	}

	// Configure all I/Os.
	for (const struct gpio_config_t *p = pin_cfgs; p->pins; ++p) {
		gpioConfig(p->pins, p->mode);
	}

	gpioLock(PAAll);
	gpioLock(PBAll);
	// gpioLock(PCAll);
	// gpioLock(PDAll);

	USART3.CR1 = 0;
	USART3.CR2 = 0;
	USART3.CR3 = 0;
	USART3.BRR = usart_brr(115200);
	USART3.CR1 |= USART_CR1_UE | USART_CR1_TE;
	NVIC_EnableIRQ(USART3_IRQn);

#ifdef __REVISION__
	printf("SWREV:%07x\n", __REVISION__);
#endif
	printf("CPUID:%08lx\n", SCB.CPUID);
	printf("IDCODE:%08lx\n", DBGMCU.IDCODE);
	printf("DEVID:%08lx:%08lx:%08lx\n", UNIQUE_DEVICE_ID[2], UNIQUE_DEVICE_ID[1], UNIQUE_DEVICE_ID[0]);
	printf("RESET:%02x%s%s%s%s%s%s\n", rf, rf & 0x80 ? " LPWR" : "", rf & 0x40 ? " WWDG" : "", rf & 0x20 ? " IWDG" : "",
		   rf & 0x10 ? " SFT" : "", rf & 0x08 ? " POR" : "", rf & 0x04 ? " PIN" : "");


	// Set up independent watchdog
	IWDG.KR	 = 0x5555;	// enable watchdog config
	IWDG.PR	 = 3;		// prescaler 32768 Hz/32 -> 1024 Hz
	IWDG.RLR = 2048;	// 2s timeout
	IWDG.KR	 = 0xCCCC;	// start watchdog countdown

	uint64_t now = cycleCount();
	printf("%lld mainloop start\n", now / C_US);

	for (;;) {
//		IWDG.KR = 0xAAAA;  // pet watchdog
		__WFI();
	}
}
