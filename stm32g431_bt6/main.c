#include "stm32g4xx.h"

#include "clock.h"
#include "gpio2.h"
#include "nvic.h"
#include "printf.h"
#include "usart.h"

enum {
	LED0_PIN      = PA5, // 64 pin nucleo  //PB8 on 32 pin mini nucleo
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
/* clang-format on */

static inline void led0_off(void) { digitalLo(LED0_PIN); }
static inline void led0_on(void) { digitalHi(LED0_PIN); }
static inline void led0_toggle(void) { digitalToggle(LED0_PIN); }

extern uint32_t UNIQUE_DEVICE_ID[3]; // Section 48.1

static struct Ringbuffer usart2tx;
void                     USART2_Handler(void) { usart_irq_handler(&USART2, &usart2tx); }
static size_t            u2puts(const char* buf, size_t len) { return usart_puts(&USART2, &usart2tx, buf, len); }

void main(void) {
	uint8_t rf = (RCC.CSR >> 24) & 0xfc;
	RCC.CSR |= RCC_CSR_RMVF; // Set RMVF bit to clear the reset flags

	RCC.AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC.APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	for (const struct gpio_config_t* p = pin_cfgs; p->pins; ++p) {
		gpioConfig(p->pins, p->mode);
	}

	usart_init(&USART2, 921600);
	NVIC_EnableIRQ(USART2_IRQn);

	cbprintf(u2puts, "SWREV:%s\n", __REVISION__);
	cbprintf(u2puts, "CPUID:%08lx\n", SCB.CPUID);
	cbprintf(u2puts, "DEVID:%08lx:%08lx:%08lx\n", UNIQUE_DEVICE_ID[2], UNIQUE_DEVICE_ID[1], UNIQUE_DEVICE_ID[0]);
	cbprintf(u2puts, "RESET:%02x%s%s%s%s%s%s\n", rf, rf & 0x80 ? " LPWR" : "", rf & 0x40 ? " WWDG" : "", rf & 0x20 ? " IWDG" : "",
	         rf & 0x10 ? " SFT" : "", rf & 0x08 ? " POR" : "", rf & 0x04 ? " PIN" : "");
	usart_wait(&USART2);

	for (;;) {
		led0_toggle();
		delay(250 * 1000);
	}
}