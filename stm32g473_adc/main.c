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
    { ADC1_2_IRQn, 		PRIO(3,1) },
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
extern uint16_t TS_CAL1, TS_CAL2, VREFINT;



static volatile int	adc_chan = 0;

void ADC1_2_Handler(void) {
	uint64_t now = cycleCount();
	uint16_t isr = ADC1.ISR;

	uint64_t us = now/C_US;

	if (isr & ADC_ISR_EOC) {
		printf("%lld.%06lld ch%d: %4ld\n", us/1000000, us%1000000, adc_chan, ADC1.DR);
		++adc_chan;
	}

	if (isr & ADC_ISR_EOS) {
		ADC1.ISR |= ADC_ISR_EOS;
		adc_chan = 0;
		printf("\e[H");
	}

	assert((isr & ADC_ISR_OVR) == 0);
	assert(adc_chan < 3);

	IWDG.KR = 0xAAAA;  // pet watchdog
}


void main(void) {
	uint8_t rf = (RCC.CSR >> 24) & 0xfc;  // reset flags
	RCC.CSR |= RCC_CSR_RMVF;			  // Set RMVF bit to clear the reset flags

	// Enable all peripherals we'll be using.
	RCC.AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN;
	RCC.APB1ENR1 |= RCC_APB1ENR1_USART3EN | RCC_APB1ENR1_TIM6EN;
    RCC.AHB2ENR  |= RCC_AHB2ENR_ADC12EN;

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

    printf("ADCCAL: TEMP %u %u VREF %u\n", TS_CAL1, TS_CAL2, VREFINT);

	 // ADC
	adc_common_ccr_set_ckmode(&ADC12_Common, 3);  // HCLK/4, 42 MHz

	ADC1.CR &= ~ADC_CR_DEEPPWD;      // end deep power down
	ADC1.CR |= ADC_CR_ADVREGEN;      // enable voltage regulator
	delay(25);                       // DS11453 table 66
	ADC1.CR |= ADC_CR_ADCAL;         // start calibration
	while (ADC1.CR & ADC_CR_ADCAL) {
			__NOP();
	}

	ADC1.ISR |= ADC_ISR_ADRDY;                  // clear ready flag
	ADC1.CR |= ADC_CR_ADEN;                     // enable
	while ((ADC1.ISR & ADC_ISR_ADRDY) != 0) {  // wait until ready
		__NOP();
	}

	printf("%lld ADC Ready\n", cycleCount() / C_US);

	ADC12_Common.CCR |= ADC_COMMON_CCR_VSENSESEL | ADC_COMMON_CCR_VREFEN | ADC_COMMON_CCR_VBATSEL;

	// configure to sample channels 16,17,18
	adc_sqr1_set_l(&ADC1, 3 - 1);   // 3 samples
	adc_sqr1_set_sq1(&ADC1, 16);     // channel 16, V_TS
	adc_sqr1_set_sq2(&ADC1, 17);     // channel 17 Vbat/3
	adc_sqr1_set_sq3(&ADC1, 18);     // channel 18 Vrefint
	adc_smpr2_set_smp16(&ADC1, 7);   // 640.5/(42Mhz) = 15.25 μs
	adc_smpr2_set_smp17(&ADC1, 7);   // 
	adc_smpr2_set_smp18(&ADC1, 7);   // 
	adc_cfgr_set_exten(&ADC1, 1);    // trigger on rising edge
	adc_cfgr_set_extsel(&ADC1, 13);  // ext 13 is TIM6 TRGO (see below), for some reason TIM4 doesnt work. 

	ADC1.CFGR |= ADC_CFGR_AUTDLY;  // without this, more than 3 samples trigger an overflow error (this was hard to find)
	ADC1.IER = ADC_IER_EOCIE;      // irq at end of conversion. the last one will also have EOS bit set, no need for extra IRQ
	ADC1.CR |= ADC_CR_ADSTART;     // start as soon as the triggers come in from TIM6
	NVIC_EnableIRQ(ADC1_2_IRQn);

	// set up TIM6 for a 10Hz hearbeat for triggering the ADC
	TIM6.PSC = (CLOCKSPEED_HZ / 10000) - 1;
	TIM6.ARR = 2000 - 1;             // 10KHz/2000 = 5Hz
	tim6_cr2_set_mms(&TIM6, 2);      // TRGO = update event


	// Set up independent watchdog
	IWDG.KR	 = 0x5555;	// enable watchdog config
	IWDG.PR	 = 3;		// prescaler 32768 Hz/32 -> 1024 Hz
	IWDG.RLR = 2048;	// 2s timeout
	IWDG.KR	 = 0xCCCC;	// start watchdog countdown

	uint64_t now = cycleCount();
	printf("%lld mainloop start\n", now / C_US);

	TIM6.CR1 |= TIM6_CR1_CEN;

	for (;;) {
		__WFI();
	}
}
