#pragma once

#include "stm32f7x7.h"

// sets the priority grouping field using the required unlock sequence.
// val 0...7  indicates the position of the binary point that splits the PRI_n fields in the Interrupt Priority Registers into separate group priority and subpriority
// 0: xxxx xxx.y  128 priorities, 2 subpriorites
// ...
// 7: yyyy yyyy   0 priorities, 256 subpriorities
static inline void nvic_set_priority_grouping(uint32_t val) {
	uint32_t r = SCB.AIRCR;
	r &= ~(SCB_AIRCR_VECTKEYSTAT | SCB_AIRCR_PRIGROUP);
	r |= (0x5FAUL << 16) | ((val & 0x7) << 8);
	SCB.AIRCR = r;
}

static inline void nvic_set_priority(enum IRQn_Type IRQn, uint8_t pri) {
	if (IRQn < 0) {
		((uint8_t*)(&SCB.SHPR1))[IRQn + 12] = pri;
	} else {
		((uint8_t*)(&NVIC.IPR0))[IRQn] = pri;
	}
}

// only works for the peripheral interrupt numbers, which have (int)IRQn >= 0
static inline void nvic_enable_irq (enum IRQn_Type IRQn) { (&NVIC.ISER0)[IRQn >> 5] = 1U << (IRQn & 0x1F); }  // how did this ever work with irqn == -1?
static inline void nvic_disable_irq(enum IRQn_Type IRQn) { (&NVIC.ICER0)[IRQn >> 5] = 1U << (IRQn & 0x1F); }

