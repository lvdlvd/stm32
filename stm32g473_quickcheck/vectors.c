// Generated irq vector table for device STM32G4xx version 2.0
// Generated by genstruct, DO NOT EDIT.

#include "arm_cm4.h"
#include "stm32g4xx.h"

extern void _estack(void);              // fake definition, will be filled in by linker script.
extern void unhandled_interrupt(uint32_t irq);  // in fault.c

void default_IRQ_Handler(void) { unhandled_interrupt(__get_IPSR()); }

// Core fault handlers
void Reset_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void NonMaskableInt_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
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
void DMA1_CH7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ADC1_2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USB_HP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USB_LP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FDCAN1_IT0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FDCAN1_IT1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI9_5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_BRK_TIM15_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_UP_TIM16_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_TRG_COM_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_CC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
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
void ADC3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FMC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void LPTIM1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM6_DACUNDER_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ADC4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ADC5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void COMP1_2_3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void COMP4_5_6_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void COMP7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CRS_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM20_BRK_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM20_UP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM20_TRG_COM_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM20_CC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FPU_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FDCAN2_IT0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FDCAN2_IT1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FDCAN3_IT0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FDCAN3_IT1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RNG_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void LPUART_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMAMUX_OVR_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_CH8_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH6_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_CH8_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void Cordic_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FMAC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));

__attribute__((section(".isr_vector"))) void (* const vector_table[])(void) = {
    _estack,
    Reset_Handler,
    NonMaskableInt_Handler,
    HardFault_Handler,          // only on cortex M4 and up
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
    [16+PVD_PVM_IRQn] = PVD_PVM_Handler,  // 1 PVD through EXTI line detection
    [16+RTC_TAMP_CSS_LSE_IRQn] = RTC_TAMP_CSS_LSE_Handler,  // 2 RTC_TAMP_CSS_LSE
    [16+RTC_WKUP_IRQn] = RTC_WKUP_Handler,  // 3 RTC Wakeup timer
    [16+FLASH_IRQn] = FLASH_Handler,  // 4 FLASH
    [16+RCC_IRQn] = RCC_Handler,  // 5 RCC global interrupt
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
    [16+DMA1_CH7_IRQn] = DMA1_CH7_Handler,  // 17 DMA1 channel 7 interrupt
    [16+ADC1_2_IRQn] = ADC1_2_Handler,  // 18 ADC1 and ADC2 global interrupt
    [16+USB_HP_IRQn] = USB_HP_Handler,  // 19 USB_HP
    [16+USB_LP_IRQn] = USB_LP_Handler,  // 20 USB_LP
    [16+FDCAN1_IT0_IRQn] = FDCAN1_IT0_Handler,  // 21 FDCAN1 interrupt 0
    [16+FDCAN1_IT1_IRQn] = FDCAN1_IT1_Handler,  // 22 FDCAN1 interrupt 1
    [16+EXTI9_5_IRQn] = EXTI9_5_Handler,  // 23 EXTI9_5
    [16+TIM1_BRK_TIM15_IRQn] = TIM1_BRK_TIM15_Handler,  // 24 TIM1_BRK_TIM15
    [16+TIM1_UP_TIM16_IRQn] = TIM1_UP_TIM16_Handler,  // 25 TIM1_UP_TIM16
    [16+TIM1_TRG_COM_IRQn] = TIM1_TRG_COM_Handler,  // 26 TIM1_TRG_COM/
    [16+TIM1_CC_IRQn] = TIM1_CC_Handler,  // 27 TIM1 capture compare interrupt
    [16+TIM2_IRQn] = TIM2_Handler,  // 28 TIM2
    [16+TIM3_IRQn] = TIM3_Handler,  // 29 TIM3
    [16+TIM4_IRQn] = TIM4_Handler,  // 30 TIM4
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
    [16+ADC3_IRQn] = ADC3_Handler,  // 47 ADC3
    [16+FMC_IRQn] = FMC_Handler,  // 48 FMC
    [16+LPTIM1_IRQn] = LPTIM1_Handler,  // 49 LPTIM1
    [16+TIM5_IRQn] = TIM5_Handler,  // 50 TIM5
    [16+SPI3_IRQn] = SPI3_Handler,  // 51 SPI3
    [16+TIM6_DACUNDER_IRQn] = TIM6_DACUNDER_Handler,  // 54 TIM6_DACUNDER
    [16+TIM7_IRQn] = TIM7_Handler,  // 55 TIM7
    [16+DMA2_CH1_IRQn] = DMA2_CH1_Handler,  // 56 DMA2_CH1
    [16+DMA2_CH2_IRQn] = DMA2_CH2_Handler,  // 57 DMA2_CH2
    [16+DMA2_CH3_IRQn] = DMA2_CH3_Handler,  // 58 DMA2_CH3
    [16+DMA2_CH4_IRQn] = DMA2_CH4_Handler,  // 59 DMA2_CH4
    [16+DMA2_CH5_IRQn] = DMA2_CH5_Handler,  // 60 DMA2_CH5
    [16+ADC4_IRQn] = ADC4_Handler,  // 61 ADC4
    [16+ADC5_IRQn] = ADC5_Handler,  // 62 ADC5
    [16+COMP1_2_3_IRQn] = COMP1_2_3_Handler,  // 64 COMP1_2_3
    [16+COMP4_5_6_IRQn] = COMP4_5_6_Handler,  // 65 COMP4_5_6
    [16+COMP7_IRQn] = COMP7_Handler,  // 66 COMP7
    [16+CRS_IRQn] = CRS_Handler,  // 75 CRS
    [16+TIM20_BRK_IRQn] = TIM20_BRK_Handler,  // 77 TIM20_BRK
    [16+TIM20_UP_IRQn] = TIM20_UP_Handler,  // 78 TIM20_UP
    [16+TIM20_TRG_COM_IRQn] = TIM20_TRG_COM_Handler,  // 79 TIM20_TRG_COM
    [16+TIM20_CC_IRQn] = TIM20_CC_Handler,  // 80 TIM20_CC
    [16+FPU_IRQn] = FPU_Handler,  // 81 Floating point unit interrupt
    [16+SPI4_IRQn] = SPI4_Handler,  // 84 SPI4
    [16+FDCAN2_IT0_IRQn] = FDCAN2_IT0_Handler,  // 86 FDCAN2 interrupt 0
    [16+FDCAN2_IT1_IRQn] = FDCAN2_IT1_Handler,  // 87 FDCAN2 interrupt 1
    [16+FDCAN3_IT0_IRQn] = FDCAN3_IT0_Handler,  // 88 FDCAN3 interrupt 0
    [16+FDCAN3_IT1_IRQn] = FDCAN3_IT1_Handler,  // 89 FDCAN3 interrupt 1
    [16+RNG_IRQn] = RNG_Handler,  // 90 RNG
    [16+LPUART_IRQn] = LPUART_Handler,  // 91 LPUART
    [16+DMAMUX_OVR_IRQn] = DMAMUX_OVR_Handler,  // 94 DMAMUX_OVR
    [16+DMA1_CH8_IRQn] = DMA1_CH8_Handler,  // 96 DMA1_CH8
    [16+DMA2_CH6_IRQn] = DMA2_CH6_Handler,  // 97 DMA2_CH6
    [16+DMA2_CH7_IRQn] = DMA2_CH7_Handler,  // 98 DMA2_CH7
    [16+DMA2_CH8_IRQn] = DMA2_CH8_Handler,  // 99 DMA2_CH8
    [16+Cordic_IRQn] = Cordic_Handler,  // 100 Cordic
    [16+FMAC_IRQn] = FMAC_Handler,  // 101 FMAC
};
