extern void _estack(void);              // fake definition, will be filled in by linker script.

void default_IRQ_Handler(void) {
    for(;;);
}


// Core fault handlers
void Reset_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void NonMaskableInt_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void MemoryManagement_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_8_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_9_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_10_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SVCall_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DebugMonitor_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Reserved_13_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));

//  Device specific Interrupts
void WWDG_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void PVD_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RTC_TAMP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FLASH_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RCC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI0_1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI2_3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI4_15_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA_Channel1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA_Channel2_3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA_Channel4_5_6_7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ADC_COMP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_BRK_UP_TRG_COM_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_CC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM14_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM16_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM17_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART3_USART4_LPUART1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CEC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));

__attribute__((section(".isr_vector"))) void (*vector_table[])(void) = {
    _estack,
    Reset_Handler,
    NonMaskableInt_Handler,
    Reserved_3_Handler,
    MemoryManagement_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    Reserved_7_Handler,
    Reserved_8_Handler,
    Reserved_9_Handler,
    Reserved_10_Handler,
    SVCall_Handler,
    DebugMonitor_Handler,
    Reserved_13_Handler,
    PendSV_Handler,
    SysTick_Handler,
    [0+16] = WWDG_Handler,
    [1+16] = PVD_Handler,
    [2+16] = RTC_TAMP_Handler,
    [3+16] = FLASH_Handler,
    [4+16] = RCC_Handler,
    [5+16] = EXTI0_1_Handler,
    [6+16] = EXTI2_3_Handler,
    [7+16] = EXTI4_15_Handler,
    [9+16] = DMA_Channel1_Handler,
    [10+16] = DMA_Channel2_3_Handler,
    [11+16] = DMA_Channel4_5_6_7_Handler,
    [12+16] = ADC_COMP_Handler,
    [13+16] = TIM1_BRK_UP_TRG_COM_Handler,
    [14+16] = TIM1_CC_Handler,
    [15+16] = TIM2_Handler,
    [16+16] = TIM3_Handler,
    [19+16] = TIM14_Handler,
    [21+16] = TIM16_Handler,
    [22+16] = TIM17_Handler,
    [23+16] = I2C1_Handler,
    [24+16] = I2C2_Handler,
    [25+16] = SPI1_Handler,
    [26+16] = SPI2_Handler,
    [27+16] = USART1_Handler,
    [28+16] = USART2_Handler,
    [29+16] = USART3_USART4_LPUART1_Handler,
    [30+16] = CEC_Handler,
};
