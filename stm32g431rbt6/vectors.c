#include "cortex_m4.h"
#include "stm32g431.h"

extern void _estack(void);              // fake definition, will be filled in by linker script.

void default_IRQ_Handler(void) {
    for(;;)
        __NOP();
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
void PVD_PVM_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RTC_TAMP_CSS_LSE_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RTC_WKUP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FLASH_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RCC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_CH1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_CH2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_CH3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_CH4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_CH5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_CH6_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ADC1_2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USB_HP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USB_LP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void fdcan1_intr1_it_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void fdcan1_intr0_it_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI9_5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_BRK_TIM15_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_UP_TIM16_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_TRG_COM_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_CC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C1_EV_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C1_ER_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C2_EV_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C2_ER_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI15_10_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RTC_ALARM_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USBWakeUP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_BRK_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_UP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_TRG_COM_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_CC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void LPTIM1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UART4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM6_DACUNDER_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UCPD1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void COMP1_2_3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void COMP4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CRS_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SAI_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FPU_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void AES_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RNG_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void LPUART_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C3_EV_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C3_ER_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMAMUX_OVR_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH6_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Cordic_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FMAC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));

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
    [16+WWDG_IRQn] = WWDG_Handler,  // 0 Window Watchdog interrupt
    [16+PVD_PVM_IRQn] = PVD_PVM_Handler,  // 1 PVD through EXTI line detection
    [16+RTC_TAMP_CSS_LSE_IRQn] = RTC_TAMP_CSS_LSE_Handler,  // 2 RTC_TAMP_CSS_LSE
    [16+RTC_WKUP_IRQn] = RTC_WKUP_Handler,  // 3 RTC Wakeup timer
    [16+FLASH_IRQn] = FLASH_Handler,  // 4 FLASH
    [16+RCC_IRQn] = RCC_Handler,  // 5 RCC
    [16+EXTI0_IRQn] = EXTI0_Handler,  // 6 EXTI Line0 interrupt
    [16+EXTI1_IRQn] = EXTI1_Handler,  // 7 EXTI Line1 interrupt
    [16+EXTI2_IRQn] = EXTI2_Handler,  // 8 EXTI Line2 interrupt
    [16+EXTI3_IRQn] = EXTI3_Handler,  // 9 EXTI Line3 interrupt
    [16+EXTI4_IRQn] = EXTI4_Handler,  // 10 EXTI Line4 interrupt
    [16+DMA1_CH1_IRQn] = DMA1_CH1_Handler,  // 11 DMA1 channel 1 interrupt
    [16+DMA1_CH2_IRQn] = DMA1_CH2_Handler,  // 12 DMA1 channel 2 interrupt
    [16+DMA1_CH3_IRQn] = DMA1_CH3_Handler,  // 13 DMA1 channel 3 interrupt
    [16+DMA1_CH4_IRQn] = DMA1_CH4_Handler,  // 14 DMA1 channel 4 interrupt
    [16+DMA1_CH5_IRQn] = DMA1_CH5_Handler,  // 15 DMA1 channel 5 interrupt
    [16+DMA1_CH6_IRQn] = DMA1_CH6_Handler,  // 16 DMA1 channel 6 interrupt
    [16+ADC1_2_IRQn] = ADC1_2_Handler,  // 18 ADC1 and ADC2 global interrupt
    [16+USB_HP_IRQn] = USB_HP_Handler,  // 19 USB_HP
    [16+USB_LP_IRQn] = USB_LP_Handler,  // 20 USB_LP
    [16+fdcan1_intr1_it_IRQn] = fdcan1_intr1_it_Handler,  // 21 fdcan1_intr1_it
    [16+fdcan1_intr0_it_IRQn] = fdcan1_intr0_it_Handler,  // 22 fdcan1_intr0_it
    [16+EXTI9_5_IRQn] = EXTI9_5_Handler,  // 23 EXTI9_5
    [16+TIM1_BRK_TIM15_IRQn] = TIM1_BRK_TIM15_Handler,  // 24 TIM1_BRK_TIM15
    [16+TIM1_UP_TIM16_IRQn] = TIM1_UP_TIM16_Handler,  // 25 TIM1_UP_TIM16
    [16+TIM1_TRG_COM_IRQn] = TIM1_TRG_COM_Handler,  // 26 TIM1_TRG_COM/
    [16+TIM1_CC_IRQn] = TIM1_CC_Handler,  // 27 TIM1 capture compare interrupt
    [16+TIM2_IRQn] = TIM2_Handler,  // 28 TIM2
    [16+TIM3_IRQn] = TIM3_Handler,  // 29 TIM3
    [16+TIM4_IRQn] = TIM4_Handler,  // 30 TIM4
    [16+I2C1_EV_IRQn] = I2C1_EV_Handler,  // 31 I2C1_EV
    [16+I2C1_ER_IRQn] = I2C1_ER_Handler,  // 32 I2C1_ER
    [16+I2C2_EV_IRQn] = I2C2_EV_Handler,  // 33 I2C2_EV
    [16+I2C2_ER_IRQn] = I2C2_ER_Handler,  // 34 I2C2_ER
    [16+SPI1_IRQn] = SPI1_Handler,  // 35 SPI1
    [16+SPI2_IRQn] = SPI2_Handler,  // 36 SPI2
    [16+USART1_IRQn] = USART1_Handler,  // 37 USART1
    [16+USART2_IRQn] = USART2_Handler,  // 38 USART2
    [16+USART3_IRQn] = USART3_Handler,  // 39 USART3
    [16+EXTI15_10_IRQn] = EXTI15_10_Handler,  // 40 EXTI15_10
    [16+RTC_ALARM_IRQn] = RTC_ALARM_Handler,  // 41 RTC_ALARM
    [16+USBWakeUP_IRQn] = USBWakeUP_Handler,  // 42 USBWakeUP
    [16+TIM8_BRK_IRQn] = TIM8_BRK_Handler,  // 43 TIM8_BRK
    [16+TIM8_UP_IRQn] = TIM8_UP_Handler,  // 44 TIM8_UP
    [16+TIM8_TRG_COM_IRQn] = TIM8_TRG_COM_Handler,  // 45 TIM8_TRG_COM
    [16+TIM8_CC_IRQn] = TIM8_CC_Handler,  // 46 TIM8_CC
    [16+LPTIM1_IRQn] = LPTIM1_Handler,  // 49 LPTIM1
    [16+SPI3_IRQn] = SPI3_Handler,  // 51 SPI3
    [16+UART4_IRQn] = UART4_Handler,  // 52 UART4
    [16+TIM6_DACUNDER_IRQn] = TIM6_DACUNDER_Handler,  // 54 TIM6_DACUNDER
    [16+TIM7_IRQn] = TIM7_Handler,  // 55 TIM7
    [16+DMA2_CH1_IRQn] = DMA2_CH1_Handler,  // 56 DMA2_CH1
    [16+DMA2_CH2_IRQn] = DMA2_CH2_Handler,  // 57 DMA2_CH2
    [16+DMA2_CH3_IRQn] = DMA2_CH3_Handler,  // 58 DMA2_CH3
    [16+DMA2_CH4_IRQn] = DMA2_CH4_Handler,  // 59 DMA2_CH4
    [16+DMA2_CH5_IRQn] = DMA2_CH5_Handler,  // 60 DMA2_CH5
    [16+UCPD1_IRQn] = UCPD1_Handler,  // 63 UCPD1
    [16+COMP1_2_3_IRQn] = COMP1_2_3_Handler,  // 64 COMP1_2_3
    [16+COMP4_IRQn] = COMP4_Handler,  // 65 COMP4_5_6
    [16+CRS_IRQn] = CRS_Handler,  // 75 CRS
    [16+SAI_IRQn] = SAI_Handler,  // 76 SAI
    [16+FPU_IRQn] = FPU_Handler,  // 81 Floating point unit interrupt
    [16+AES_IRQn] = AES_Handler,  // 85 AES
    [16+RNG_IRQn] = RNG_Handler,  // 90 RNG
    [16+LPUART_IRQn] = LPUART_Handler,  // 91 LPUART
    [16+I2C3_EV_IRQn] = I2C3_EV_Handler,  // 92 I2C3_EV
    [16+I2C3_ER_IRQn] = I2C3_ER_Handler,  // 93 I2C3_ER
    [16+DMAMUX_OVR_IRQn] = DMAMUX_OVR_Handler,  // 94 DMAMUX_OVR
    [16+DMA2_CH6_IRQn] = DMA2_CH6_Handler,  // 97 DMA2_CH6
    [16+Cordic_IRQn] = Cordic_Handler,  // 100 Cordic
    [16+FMAC_IRQn] = FMAC_Handler,  // 101 FMAC
};
