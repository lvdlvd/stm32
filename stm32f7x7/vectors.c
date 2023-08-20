#include "cortex_cm7.h"
#include "stm32f7x7.h"

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
void PVD_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TAMP_STAMP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RTC_WKUP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FLASH_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RCC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream6_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ADC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN1_TX_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN1_RX0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN1_RX1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN1_SCE_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI9_5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_BRK_TIM9_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_UP_TIM10_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_TRG_COM_TIM11_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
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
void OTG_FS_WKUP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_BRK_TIM12_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_UP_TIM13_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_TRG_COM_TIM14_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_CC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FMC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SDMMC1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UART4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UART5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM6_DAC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ETH_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ETH_WKUP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN2_TX_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN2_RX0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN2_RX1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN2_SCE_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_FS_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream6_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART6_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C3_EV_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C3_ER_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_HS_EP1_OUT_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_HS_EP1_IN_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_HS_WKUP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_HS_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DCMI_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CRYP_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void HASH_RNG_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FPU_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UART7_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UART8_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI4_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI5_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI6_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SAI1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void LCD_TFT_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void LCD_TFT_ER_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2D_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SAI2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void QuadSPI_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void LP_Timer1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C4_EV_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C4_ER_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPDIFRX_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DSIHOST_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DFSDM1_FLT0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DFSDM1_FLT1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DFSDM1_FLT2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DFSDM1_FLT3_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SDMMC2_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN3_TX_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN3_RX0_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN3_RX1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN3_SCE_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void JPEG_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void MDIOS_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));

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
    [16+PVD_IRQn] = PVD_Handler,  // 1 PVD through EXTI line detection
    [16+TAMP_STAMP_IRQn] = TAMP_STAMP_Handler,  // 2 Tamper and TimeStamp interrupts through the EXTI line
    [16+RTC_WKUP_IRQn] = RTC_WKUP_Handler,  // 3 RTC Tamper or TimeStamp /CSS on LSE through EXTI line 19 interrupts
    [16+FLASH_IRQn] = FLASH_Handler,  // 4 Flash global interrupt
    [16+RCC_IRQn] = RCC_Handler,  // 5 RCC global interrupt
    [16+EXTI0_IRQn] = EXTI0_Handler,  // 6 EXTI Line0 interrupt
    [16+EXTI1_IRQn] = EXTI1_Handler,  // 7 EXTI Line1 interrupt
    [16+EXTI2_IRQn] = EXTI2_Handler,  // 8 EXTI Line2 interrupt
    [16+EXTI3_IRQn] = EXTI3_Handler,  // 9 EXTI Line3 interrupt
    [16+EXTI4_IRQn] = EXTI4_Handler,  // 10 EXTI Line4 interrupt
    [16+DMA1_Stream0_IRQn] = DMA1_Stream0_Handler,  // 11 DMA1 Stream0 global interrupt
    [16+DMA1_Stream1_IRQn] = DMA1_Stream1_Handler,  // 12 DMA1 Stream1 global interrupt
    [16+DMA1_Stream2_IRQn] = DMA1_Stream2_Handler,  // 13 DMA1 Stream2 global interrupt
    [16+DMA1_Stream3_IRQn] = DMA1_Stream3_Handler,  // 14 DMA1 Stream3 global interrupt
    [16+DMA1_Stream4_IRQn] = DMA1_Stream4_Handler,  // 15 DMA1 Stream4 global interrupt
    [16+DMA1_Stream5_IRQn] = DMA1_Stream5_Handler,  // 16 DMA1 Stream5 global interrupt
    [16+DMA1_Stream6_IRQn] = DMA1_Stream6_Handler,  // 17 DMA1 Stream6 global interrupt
    [16+ADC_IRQn] = ADC_Handler,  // 18 ADC1 global interrupt
    [16+CAN1_TX_IRQn] = CAN1_TX_Handler,  // 19 CAN1 TX interrupts
    [16+CAN1_RX0_IRQn] = CAN1_RX0_Handler,  // 20 CAN1 RX0 interrupts
    [16+CAN1_RX1_IRQn] = CAN1_RX1_Handler,  // 21 CAN1 RX1 interrupts
    [16+CAN1_SCE_IRQn] = CAN1_SCE_Handler,  // 22 CAN1 SCE interrupt
    [16+EXTI9_5_IRQn] = EXTI9_5_Handler,  // 23 EXTI Line[9:5] interrupts
    [16+TIM1_BRK_TIM9_IRQn] = TIM1_BRK_TIM9_Handler,  // 24 TIM1 Break interrupt and TIM9 global interrupt
    [16+TIM1_UP_TIM10_IRQn] = TIM1_UP_TIM10_Handler,  // 25 TIM1 Update interrupt and TIM10
    [16+TIM1_TRG_COM_TIM11_IRQn] = TIM1_TRG_COM_TIM11_Handler,  // 26 TIM1 Trigger and Commutation interrupts and TIM11 global interrupt
    [16+TIM1_CC_IRQn] = TIM1_CC_Handler,  // 27 TIM1 Capture Compare interrupt
    [16+TIM2_IRQn] = TIM2_Handler,  // 28 TIM2 global interrupt
    [16+TIM3_IRQn] = TIM3_Handler,  // 29 TIM3 global interrupt
    [16+TIM4_IRQn] = TIM4_Handler,  // 30 TIM4 global interrupt
    [16+I2C1_EV_IRQn] = I2C1_EV_Handler,  // 31 I2C1 event interrupt
    [16+I2C1_ER_IRQn] = I2C1_ER_Handler,  // 32 I2C1 error interrupt
    [16+I2C2_EV_IRQn] = I2C2_EV_Handler,  // 33 I2C2 event interrupt
    [16+I2C2_ER_IRQn] = I2C2_ER_Handler,  // 34 I2C2 error interrupt
    [16+SPI1_IRQn] = SPI1_Handler,  // 35 SPI1 global interrupt
    [16+SPI2_IRQn] = SPI2_Handler,  // 36 SPI2 global interrupt
    [16+USART1_IRQn] = USART1_Handler,  // 37 USART1 global interrupt
    [16+USART2_IRQn] = USART2_Handler,  // 38 USART2 global interrupt
    [16+USART3_IRQn] = USART3_Handler,  // 39 USART3 global interrupt
    [16+EXTI15_10_IRQn] = EXTI15_10_Handler,  // 40 EXTI Line[15:10] interrupts
    [16+RTC_ALARM_IRQn] = RTC_ALARM_Handler,  // 41 RTC alarms through EXTI line 18 interrupts
    [16+OTG_FS_WKUP_IRQn] = OTG_FS_WKUP_Handler,  // 42 USB On-The-Go FS Wakeup through EXTI line interrupt
    [16+TIM8_BRK_TIM12_IRQn] = TIM8_BRK_TIM12_Handler,  // 43 TIM8 Break interrupt and TIM12 global interrupt
    [16+TIM8_UP_TIM13_IRQn] = TIM8_UP_TIM13_Handler,  // 44 TIM8 Update interrupt and TIM13 global interrupt
    [16+TIM8_TRG_COM_TIM14_IRQn] = TIM8_TRG_COM_TIM14_Handler,  // 45 TIM8 Trigger and Commutation interrupts and TIM14 global interrupt
    [16+TIM8_CC_IRQn] = TIM8_CC_Handler,  // 46 TIM8 Capture Compare interrupt
    [16+DMA1_Stream7_IRQn] = DMA1_Stream7_Handler,  // 47 DMA1 Stream7 global interrupt
    [16+FMC_IRQn] = FMC_Handler,  // 48 FMC global interrupt
    [16+SDMMC1_IRQn] = SDMMC1_Handler,  // 49 SDMMC1 global interrupt
    [16+TIM5_IRQn] = TIM5_Handler,  // 50 TIM5 global interrupt
    [16+SPI3_IRQn] = SPI3_Handler,  // 51 SPI3 global interrupt
    [16+UART4_IRQn] = UART4_Handler,  // 52 UART4 global interrupt
    [16+UART5_IRQn] = UART5_Handler,  // 53 UART5 global interrupt
    [16+TIM6_DAC_IRQn] = TIM6_DAC_Handler,  // 54 TIM6 global interrupt, DAC1 and DAC2 underrun error interrupt
    [16+TIM7_IRQn] = TIM7_Handler,  // 55 TIM7 global interrupt
    [16+DMA2_Stream0_IRQn] = DMA2_Stream0_Handler,  // 56 DMA2 Stream0 global interrupt
    [16+DMA2_Stream1_IRQn] = DMA2_Stream1_Handler,  // 57 DMA2 Stream1 global interrupt
    [16+DMA2_Stream2_IRQn] = DMA2_Stream2_Handler,  // 58 DMA2 Stream2 global interrupt
    [16+DMA2_Stream3_IRQn] = DMA2_Stream3_Handler,  // 59 DMA2 Stream3 global interrupt
    [16+DMA2_Stream4_IRQn] = DMA2_Stream4_Handler,  // 60 DMA2 Stream4 global interrupt
    [16+ETH_IRQn] = ETH_Handler,  // 61 Ethernet global interrupt
    [16+ETH_WKUP_IRQn] = ETH_WKUP_Handler,  // 62 Ethernet Wakeup through EXTI line
    [16+CAN2_TX_IRQn] = CAN2_TX_Handler,  // 63 CAN2 TX interrupts
    [16+CAN2_RX0_IRQn] = CAN2_RX0_Handler,  // 64 CAN2 RX0 interrupts
    [16+CAN2_RX1_IRQn] = CAN2_RX1_Handler,  // 65 CAN2 RX1 interrupts
    [16+CAN2_SCE_IRQn] = CAN2_SCE_Handler,  // 66 CAN2 SCE interrupt
    [16+OTG_FS_IRQn] = OTG_FS_Handler,  // 67 USB On The Go FS global interrupt
    [16+DMA2_Stream5_IRQn] = DMA2_Stream5_Handler,  // 68 DMA2 Stream5 global interrupt
    [16+DMA2_Stream6_IRQn] = DMA2_Stream6_Handler,  // 69 DMA2 Stream6 global interrupt
    [16+DMA2_Stream7_IRQn] = DMA2_Stream7_Handler,  // 70 DMA2 Stream7 global interrupt
    [16+USART6_IRQn] = USART6_Handler,  // 71 USART6 global interrupt
    [16+I2C3_EV_IRQn] = I2C3_EV_Handler,  // 72 I2C3 event interrupt
    [16+I2C3_ER_IRQn] = I2C3_ER_Handler,  // 73 I2C3 error interrupt
    [16+OTG_HS_EP1_OUT_IRQn] = OTG_HS_EP1_OUT_Handler,  // 74 USB On The Go HS End Point 1 Out global interrupt
    [16+OTG_HS_EP1_IN_IRQn] = OTG_HS_EP1_IN_Handler,  // 75 USB On The Go HS End Point 1 In global interrupt
    [16+OTG_HS_WKUP_IRQn] = OTG_HS_WKUP_Handler,  // 76 USB On The Go HS Wakeup through EXTI interrupt
    [16+OTG_HS_IRQn] = OTG_HS_Handler,  // 77 USB On The Go HS global interrupt
    [16+DCMI_IRQn] = DCMI_Handler,  // 78 DCMI global interrupt
    [16+CRYP_IRQn] = CRYP_Handler,  // 79 CRYP crypto global interrupt
    [16+HASH_RNG_IRQn] = HASH_RNG_Handler,  // 80 Hash and Rng global interrupt
    [16+FPU_IRQn] = FPU_Handler,  // 81 Floating point unit interrupt
    [16+UART7_IRQn] = UART7_Handler,  // 82 UART7 global interrupt
    [16+UART8_IRQn] = UART8_Handler,  // 83 UART 8 global interrupt
    [16+SPI4_IRQn] = SPI4_Handler,  // 84 SPI 4 global interrupt
    [16+SPI5_IRQn] = SPI5_Handler,  // 85 SPI 5 global interrupt
    [16+SPI6_IRQn] = SPI6_Handler,  // 86 SPI 6 global interrupt
    [16+SAI1_IRQn] = SAI1_Handler,  // 87 SAI1 global interrupt
    [16+LCD_TFT_IRQn] = LCD_TFT_Handler,  // 88 LTDC global interrupt
    [16+LCD_TFT_ER_IRQn] = LCD_TFT_ER_Handler,  // 89 LCD-TFT global Error interrupt
    [16+DMA2D_IRQn] = DMA2D_Handler,  // 90 DMA2D global interrupt
    [16+SAI2_IRQn] = SAI2_Handler,  // 91 SAI2 global interrupt
    [16+QuadSPI_IRQn] = QuadSPI_Handler,  // 92 QuadSPI global interrupt
    [16+LP_Timer1_IRQn] = LP_Timer1_Handler,  // 93 LP Timer1 global interrupt
    [16+I2C4_EV_IRQn] = I2C4_EV_Handler,  // 95 I2C4 event interrupt
    [16+I2C4_ER_IRQn] = I2C4_ER_Handler,  // 96 I2C4 Error interrupt
    [16+SPDIFRX_IRQn] = SPDIFRX_Handler,  // 97 SPDIFRX global interrupt
    [16+DSIHOST_IRQn] = DSIHOST_Handler,  // 98 DSI host global interrupt
    [16+DFSDM1_FLT0_IRQn] = DFSDM1_FLT0_Handler,  // 99 DFSDM1 Filter 0 global interrupt
    [16+DFSDM1_FLT1_IRQn] = DFSDM1_FLT1_Handler,  // 100 DFSDM1 Filter 1 global interrupt
    [16+DFSDM1_FLT2_IRQn] = DFSDM1_FLT2_Handler,  // 101 DFSDM1 Filter 2 global interrupt
    [16+DFSDM1_FLT3_IRQn] = DFSDM1_FLT3_Handler,  // 102 DFSDM1 Filter 3 global interrupt
    [16+SDMMC2_IRQn] = SDMMC2_Handler,  // 103 SDMMC2 global interrupt
    [16+CAN3_TX_IRQn] = CAN3_TX_Handler,  // 104 CAN3 TX interrupt
    [16+CAN3_RX0_IRQn] = CAN3_RX0_Handler,  // 105 CAN3 RX0 interrupt
    [16+CAN3_RX1_IRQn] = CAN3_RX1_Handler,  // 106 CAN3 RX1 interrupt
    [16+CAN3_SCE_IRQn] = CAN3_SCE_Handler,  // 107 CAN3 SCE interrupt
    [16+JPEG_IRQn] = JPEG_Handler,  // 108 JPEG global interrupt
    [16+MDIOS_IRQn] = MDIOS_Handler,  // 109 MDIO slave global interrupt
};
