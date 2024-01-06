#pragma once

#include "stdarg.h"
#include "stddef.h"

#include "printf.h"
#include "stm32g4xx.h"

// usart_init() initializes USART{1,2,3,4,5}.
//
// The bytes will transmitted be as 8 data bits, no Parity bit, 1 stop bit (8N1) at the specified baud rate.
//
// Before calling usart_init, make sure to set up the GPIO pins: TX to AF_PP/10MHz. RX to IN FLOATING or Pull-up.
// and to enable the USART in the RCC register:	RCC->APBxENR |= RCC_APBxENR_USARTyEN;
static inline void usart_init(struct USART_Type *usart, int baud) {
	usart->CR1 = 0; // USART_CR1_M0|USART_CR1_PCE; // we set Even parity, for compatibility with stm system bootloader.;
	usart->CR2 = 0;
	usart->CR3 = 0;
	usart->BRR = (CLOCKSPEED_HZ + baud/2) / baud;
	usart->CR1 |= USART_CR1_UE | USART_CR1_TE;
}

// Create IRQ Handlers for the usarts you use like so:
// 	void USARTx_IRQ_Handler(void) { usart_irq_andler(&USARTx, txbufx); }
static inline void usart_irq_handler(struct USART_Type *usart, struct Ringbuffer *rb) {
	if (!ringbuffer_empty(rb)) {
		if ((usart->ISR & USART_ISR_TXE) != 0) {
			usart->TDR = ringbuffer_get_tail(rb);
		}
	} else {
		usart->CR1 &= ~USART_CR1_TXEIE;
	}
	return;
}

// create a puts for use with cbprintf() like so:
// 	size_t ux_puts(const char *buf, size_t len) { return usart_puts(&USARTx, tx_bufx, buf, len); }
static inline size_t usart_puts(struct USART_Type *usart, struct Ringbuffer *rb, const char *buf, size_t len) {
	size_t r = ringbuffer_puts(rb, buf, len);
	// on overflow zap the buffer and leave a marker for the user that data was lost
	if (r < len) {
		ringbuffer_clear(rb);
		ringbuffer_puts(rb, "!OVFL!", 6);
	}
	usart->CR1 |= USART_CR1_TXEIE;	// enable transmissions
	return r;
}

static inline void usart_wait(struct USART_Type *usart) {
	while (usart->CR1 & USART_CR1_TXEIE)
		;
}





