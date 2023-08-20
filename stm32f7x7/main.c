#include "cortex_cm7.h"
#include "stm32f7x7.h"

#include "clock.h"
#include "gpio2.h"
#include "nvic.h"
#include "usart.h"

enum {
    // 144 pin nucleo pins
    LEDGREEN_PIN  = PB0, 
    LEDBLUE_PIN   = PB7, 
    LEDRED_PIN    = PB14, 
    BUTTON_PIN    = PC13,
	USART3_TX_PIN = PD8,
    USART3_RX_PIN = PD9,
};

/* clang-format off */
static struct gpio_config_t {
    enum GPIO_Pin  pins;
    enum GPIO_Conf mode;
} pin_cfgs[] = {
    {LEDGREEN_PIN|LEDBLUE_PIN|LEDRED_PIN, GPIO_OUTPUT},
    {USART3_TX_PIN, GPIO_AF7|GPIO_HIGH},
    {0, 0}, // sentinel
};

enum { IRQ_PRIORITY_GROUPING = 4 }; // prio[7:4] : 16 groups,  prio[3:0] : 16 subgroups
struct {
    enum IRQn_Type irq;
    uint8_t        group, sub;
} irqprios[] = {
    {SysTick_IRQn, 0, 0},
    {USART3_IRQn,  1, 0},
    {None_IRQn, 0xff, 0xff},
};
/* clang-format on */


extern uint32_t UNIQUE_DEVICE_ID[3]; // Section 48.1

static struct Ringbuffer usart3tx;
void                     USART3_Handler(void) { usart_irq_handler(&USART3, &usart3tx); }
 size_t            u3puts(const char* buf, size_t len) { return usart_puts(&USART3, &usart3tx, buf, len); }

void main(void) __attribute__ ((noreturn));
void main(void) {
	uint8_t rf = (RCC.CSR >> 24) & 0xfc;
    (void)rf;
	RCC.CSR |= RCC_CSR_RMVF; // Set RMVF bit to clear the reset flags

    nvic_set_priority_grouping(IRQ_PRIORITY_GROUPING);
    for (int i = 0; irqprios[i].irq != None_IRQn; i++) {
        nvic_set_priority(irqprios[i].irq,  (irqprios[i].group << 4) | irqprios[i].sub);
    }


	RCC.APB1ENR |= RCC_APB1ENR_USART3EN;
	RCC.AHB1ENR |= RCC_AHB1ENR_GPIOBEN |RCC_AHB1ENR_GPIOCEN |RCC_AHB1ENR_GPIODEN;

	for (const struct gpio_config_t* p = pin_cfgs; p->pins; ++p) {
		gpioConfig(p->pins, p->mode);
	}

	usart_init(&USART3, 115200);
	nvic_enable_irq(USART3_IRQn);

	cbprintf(u3puts, "SWREV:%s\n", __REVISION__);
	cbprintf(u3puts, "CPUID:%08lx\n", SCB.CPUID);
	/// DEVID:20333631:5034500d:004f001d is aski encoded lot/wafer/x/y (see manual)
	cbprintf(u3puts, "DEVID:%08lx:%08lx:%08lx\n", UNIQUE_DEVICE_ID[2], UNIQUE_DEVICE_ID[1], UNIQUE_DEVICE_ID[0]);
	cbprintf(u3puts, "RESET:%02x%s%s%s%s%s%s\n", rf, rf & 0x80 ? " LPWR" : "", rf & 0x40 ? " WWDG" : "", rf & 0x20 ? " IWDG" : "",
	         rf & 0x10 ? " SFT" : "", rf & 0x08 ? " POR" : "", rf & 0x04 ? " PIN" : "");
	usart_wait(&USART3);

    digitalHi(LEDGREEN_PIN|LEDBLUE_PIN|LEDRED_PIN);


    for(;;) {
//        digitalToggle(LEDGREEN_PIN);
        delay(100*1000);
        digitalToggle(LEDBLUE_PIN);
        delay(100*1000);
        digitalToggle(LEDRED_PIN);
        delay(100*1000);

        if (digitalIn(BUTTON_PIN))
            digitalToggle(LEDGREEN_PIN);
    }


 	for (;;)
        __WFI(); // wait for interrupt to change the state of any of the subsystems
}