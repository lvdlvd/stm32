#pragma once
// This firmware uses the Cortex M4 builtin 'Systick' as a timer that counts in clock cycles.
#include <stdint.h>

#include "stm32g4xx.h"

enum { CLOCKSPEED_HZ = 168000000ULL, C_US = CLOCKSPEED_HZ / 1000000 };

// For PCLK1 = 168 Mhz: 84MHz/(1<<0..7)
enum spi_clock_div_t {
	SPI_84MHz = 0,
	SPI_42MHz = 1,
	SPI_21MHz = 2,
	SPI_10500KHz = 3,
	SPI_5250KHz = 4,
	SPI_2625KHz = 5,
	SPI_1312KHz = 6,
	SPI_656KHz = 7,
};


static inline uint32_t usart_brr(uint32_t baud) { return (CLOCKSPEED_HZ + baud / 2) / baud; }

// in boot.c, updated by SysTick irq handler
extern volatile uint64_t clockticks;

// return the current time as measured in cpu cycles since initCycleCount was called.
static inline uint64_t cycleCount(void) { return clockticks - stk_val_get_current(); }

// spinlock using the systick
// usec = 10...1000000 (.01 ms ..  1s)
static inline void delay(uint32_t usec) {
	uint64_t then = cycleCount() + C_US * usec;
	while (cycleCount() < then) {
		;
	}
}
