#include "usart.h"
extern inline void usart_wait(struct USART_Type* usart);

void usart_init(struct USART_Type* usart, int baud) {
	usart->CR1 = 0;
	usart->CR2 = 0;
	usart->CR3 = 0;

    uint32_t clk = 144000000;
 
	usart->BRR = clk / baud;
	usart->CR1 |= USART_CR1_UE | USART_CR1_TE;
}

void usart_irq_handler(struct USART_Type* usart, struct Ringbuffer* rb) {
	if (!ringbuffer_empty(rb)) {
		if ((usart->ISR & USART_ISR_TXE) != 0) {
			usart->TDR = ringbuffer_get_tail(rb);   
		}
	} else {
		usart->CR1 &= ~USART_CR1_TXEIE;
	}
	return;
}

size_t usart_puts(struct USART_Type* usart, struct Ringbuffer* rb, const char *buf, size_t len) {
    size_t r = ringbuffer_puts(rb, buf, len);
    // on overflow zap the buffer and leave a marker for the user that data was lost
    if (r < len) {
        ringbuffer_clear(rb);
        ringbuffer_puts(rb, "!OVFL!", 6);
    }
	usart->CR1 |= USART_CR1_TXEIE; // enable transmissions
    return r;
}
