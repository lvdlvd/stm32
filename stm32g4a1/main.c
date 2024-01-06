#include "cortex_m4.h"
#include "stm32g4xx.h"

#include "clock.h"
#include "gpio2.h"
#include "nvic.h"
#include "printf.h"
#include "usart.h"
#include "usb.h"

enum {
//	LED0_PIN	  = PA5,  // 64 pin nucleo  //PB8 on 32 pin mini nucleo
	LED0_PIN = PC3|PC6,
	USART2_TX_PIN = PA2,
};

/* clang-format off */
static struct gpio_config_t {
    enum GPIO_Pin  pins;
    enum GPIO_Conf mode;
} pin_cfgs[] = {
    {LED0_PIN, GPIO_OUTPUT},
    {USART2_TX_PIN, GPIO_AF7_USART1|GPIO_HIGH},
    {0, 0}, // sentinel
};

enum { IRQ_PRIORITY_GROUPING_2_2 = 5 }; // prio[7:6] : 4 groups,  prio[5:4] : 4 subgroups
#define PRIO(group, subgroup) ((group<<6) | (subgroup<<4))
struct {
    enum IRQn_Type irq;
    uint8_t        prio;
} irqprios[] = {
    {SysTick_IRQn, PRIO(0, 0)},
    {USART2_IRQn,  PRIO(1, 0)},
    {USB_LP_IRQn,  PRIO(2, 0)},
    {TIM3_IRQn,    PRIO(3, 0)},
    {None_IRQn, 0xff},
};
#undef PRIO
/* clang-format on */

static inline void led0_off(void) { digitalLo(LED0_PIN); }
static inline void led0_on(void) { digitalHi(LED0_PIN); }
static inline void led0_toggle(void) { digitalToggle(LED0_PIN); }

extern uint32_t UNIQUE_DEVICE_ID[3];  // Section 48.1

static struct Ringbuffer usart2tx;
void					 USART2_Handler(void) { usart_irq_handler(&USART2, &usart2tx); }
static size_t			 u2puts(const char *buf, size_t len) { return usart_puts(&USART2, &usart2tx, buf, len); }
static size_t            usb_puts(const char *buf, size_t len) { return usb_send(buf, len); }

void TIM3_Handler(void) {
	if ((TIM3.SR & TIM1_SR_UIF) == 0) {
		return;
	}
	TIM3.SR &= ~TIM1_SR_UIF;
	static int i = 0;
	cbprintf(u2puts, "%lld: bingo %d\n", cycleCount(), i);
	cbprintf(usb_puts, "%lld: bingo %d\n", cycleCount(), i++);
	led0_toggle();
}

void USB_LP_Handler(void) {
        uint64_t now = cycleCount();
        led0_toggle();
        static int i = 0;
        cbprintf(u2puts, "%lld IRQ %i: %s\n", now / C_US, i++, usb_state_str(usb_state()));
        uint8_t buf[64];
        size_t  len = usb_recv(buf, sizeof buf);
        if (len > 0) {
                cbprintf(u2puts, "received %i: %*s\n", len, len, buf);
        }
}

static const char* clksrcstr[] = { "RSV", "HSI", "HSE", "PLL" };
static const char* pllsrcstr[] = { "OFF", "NONE", "HSI", "HSE" };

void main(void) __attribute__((noreturn));
void main(void) {
	uint8_t rf = (RCC.CSR >> 24) & 0xfc;
	RCC.CSR |= RCC_CSR_RMVF;  // Set RMVF bit to clear the reset flags

	NVIC_SetPriorityGrouping(IRQ_PRIORITY_GROUPING_2_2);
	for (int i = 0; irqprios[i].irq != None_IRQn; i++) {
		NVIC_SetPriority(irqprios[i].irq, irqprios[i].prio);
	}

	RCC.APB1ENR1 |= RCC_APB1ENR1_USART2EN | RCC_APB1ENR1_TIM3EN | RCC_APB1ENR1_USBEN;
	RCC.APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	RCC.AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN;

	for (const struct gpio_config_t *p = pin_cfgs; p->pins; ++p) {
		gpioConfig(p->pins, p->mode);
	}

    ringbuffer_clear(&usart2tx);
	usart_init(&USART2, 115200);
	NVIC_EnableIRQ(USART2_IRQn);


	cbprintf(u2puts, "SWREV:%s\n", __REVISION__);
	cbprintf(u2puts, "CPUID:%08lx\n", SCB.CPUID);
	/// DEVID:20333631:5034500d:004f001d is aski encoded lot/wafer/x/y (see manual)
	cbprintf(u2puts, "DEVID:%08lx:%08lx:%08lx\n", UNIQUE_DEVICE_ID[2], UNIQUE_DEVICE_ID[1], UNIQUE_DEVICE_ID[0]);
	cbprintf(u2puts, "RESET:%02x%s%s%s%s%s%s\n", rf, rf & 0x80 ? " LPWR" : "", rf & 0x40 ? " WWDG" : "", rf & 0x20 ? " IWDG" : "", rf & 0x10 ? " SFT" : "", rf & 0x08 ? " POR" : "", rf & 0x04 ? " PIN" : "");
	cbprintf(u2puts, "CLOCKSRC:%s\n", clksrcstr[rcc_cfgr_get_sws(&RCC)]);
	if (rcc_cfgr_get_sws(&RCC) == 3) {
		cbprintf(u2puts, "PLLSRC:%s\n", pllsrcstr[rcc_pllcfgr_get_pllsrc(&RCC)]);
	}
	usart_wait(&USART2);

	// enable 1Hz TIM3
	TIM3.DIER |= TIM1_DIER_UIE;
	TIM3.PSC = (CLOCKSPEED_HZ / 10000) - 1;  // 10KHz
	TIM3.ARR = 10000 - 1;  // 10KHz/10000  = 1Hz
	TIM3.CR1 |= TIM1_CR1_CEN;
	NVIC_EnableIRQ(TIM3_IRQn);

#if 1
    // use HSI48 + clock recovery system for the USB clock
	RCC.APB1ENR1 |= RCC_APB1ENR1_CRSEN;
	RCC.CRRCR |= RCC_CRRCR_HSI48ON;
	while((RCC.CRRCR & RCC_CRRCR_HSI48RDY) == 0)
		__NOP();
	crs_cfgr_set_syncsrc(&CRS, 2);  // USB SOF frames
	crs_cfgr_set_syncdiv(&CRS, 0);  // undivided
	crs_cfgr_set_felim(&CRS, 0x22);   // default
	crs_cfgr_set_reload(&CRS, (48000000/1000)-1);   // 48MHz/1KHz
	crs_cr_set_trim(&CRS, 0x40);
	CRS.CR |= CRS_CR_AUTOTRIMEN | CRS_CR_CEN;
	rcc_ccipr_set_clk48sel(&RCC, 0); // 7.4.26, p.328:  0: HSI48, 2:PLLQ other reserved
#endif

	usb_init();
	NVIC_EnableIRQ(USB_LP_IRQn);

	for (;;) {
		__WFI();  // wait for interrupt to change the state of any of the subsystems
	}
}
