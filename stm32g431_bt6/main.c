#include "stm32g431.h"

#include "clock.h"
#include "gpio2.h"

enum {
	   LED0_PIN      = PA5,
};


/* clang-format off */
static struct gpio_config_t {
    enum GPIO_Pin  pins;
    enum GPIO_Conf mode;
} pin_cfgs[] = {
    {LED0_PIN, GPIO_OUTPUT},
    {0, 0}, // sentinel
};
/* clang-format on */

static inline void led0_on(void) { digitalLo(LED0_PIN); }
static inline void led0_off(void) { digitalHi(LED0_PIN); }
static inline void led0_toggle(void) { digitalToggle(LED0_PIN); }

void main(void) {	
	//uint8_t rf = (RCC.CSR >> 24) & 0xfc;
    RCC.CSR |= RCC_CSR_RMVF; // Set RMVF bit to clear the reset flags

	RCC.AHB2ENR |= RCC_AHB2ENR_GPIOAEN;


	for (const struct gpio_config_t* p = pin_cfgs; p->pins; ++p) {
		gpioConfig(p->pins, p->mode);
    }


    for(;;) {
    	led0_toggle();
    	delay(500*1000);
    }


}