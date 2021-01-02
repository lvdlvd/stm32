#include "cortex_m0.h"
#include "stm32g031.h"

#include "clock.h"

// with SysTick_Config((1U << 24) - 1), tick at 4.2915 HZ,
// but everything also works for different configs.

// clocktics starts at 1 tick ahead.  TODO set when enabling the systick interval
static volatile uint64_t clockticks = (1U << 24); // rolls over after 2^64/72MHz = 8118.81294 years

// this irq has a higher priority than all userspace, so we consider update to be atomic
// clockticks holds the counts at the _end_ of the next systick,
// so cyclecount can just subtract current VAL (which counts down)
void SysTick_Handler(void) { clockticks += stk_rvr_get_reload(&STK) + 1; }

// VAL counts down from LOAD in 24 bits
uint64_t cycleCount(void) { return clockticks - stk_cvr_get_current(&STK); }

// delay up to 1'000'000 usec
void delay(uint32_t usec) {
	uint64_t now = cycleCount();
	// then = now + usec * clockspeed_hz / (usec/sec)
	uint64_t then = CLOCKSPEED_HZ;
	then *= usec;
	then /= 1000000;
	then += now;
	while (cycleCount() < then)
		__NOP(); // wait
}
