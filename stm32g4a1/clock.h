#pragma once

enum { CLOCKSPEED_HZ = 144000000, C_US = CLOCKSPEED_HZ / 1000000 };

extern volatile uint64_t clockticks;
static uint64_t cycleCount(void) { return clockticks - stk_val_get_current(&STK); }

// delay up to 1'000'000 usec
void delay(uint32_t usec);