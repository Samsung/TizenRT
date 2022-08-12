/****************************************************************************************************
 * arch/arm/include/stm32l4/stm32l4x3xx_irq.h
 *
 *   Copyright (C) 2015 Sebastien Lorquet. All rights reserved.
 *   Authors: Sebastien Lorquet <sebastien@lorquet.fr>
 *            Juha Niskanen <juha.niskanen@haltian.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************************/

/* This file should never be included directed but, rather, only indirectly through arch/irq.h */

#ifndef __ARCH_ARM_INCLUDE_STM32H745_STM32H745XX_IRQ_H
#define __ARCH_ARM_INCLUDE_STM32H745_STM32H745XX_IRQ_H

/****************************************************************************************************
 * Included Files
 ****************************************************************************************************/

#include <tinyara/config.h>

/****************************************************************************************************
 * Pre-processor Definitions
 ****************************************************************************************************/

/* IRQ numbers.  The IRQ number corresponds vector number and hence map directly to bits in the
 * NVIC.  This does, however, waste several words of memory in the IRQ to handle mapping tables.
 *
 * Processor Exceptions (vectors 0-15).  These common definitions can be found in the file
 * nuttx/arch/arm/include/stm32l4/irq.h which includes this file
 *
 * External interrupts (vectors >= 16)
 *
 * These interrupts vectors was implemented based on RM0394 Table 45 and should work for
 * STM32H745xx
 *
 */
#define STM32H745_IRQ_WWDG                   (STM32H745_IRQ_FIRST + 0)      /*!< Window WatchDog Interrupt ( wwdg1_it, wwdg2_it)                   */
#define STM32H745_IRQ_PVD_AVD                (STM32H745_IRQ_FIRST + 1)      /*!< PVD/AVD through EXTI Line detection Interrupt                     */
#define STM32H745_IRQ_TAMP_STAMP             (STM32H745_IRQ_FIRST + 2)      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
#define STM32H745_IRQ_RTC_WKUP               (STM32H745_IRQ_FIRST + 3)      /*!< RTC Wakeup interrupt through the EXTI line                        */
#define STM32H745_IRQ_FLASH                  (STM32H745_IRQ_FIRST + 4)      /*!< FLASH global Interrupt                                            */
#define STM32H745_IRQ_RCC                    (STM32H745_IRQ_FIRST + 5)      /*!< RCC global Interrupt                                              */
#define STM32H745_IRQ_EXTI0                  (STM32H745_IRQ_FIRST + 6)      /*!< EXTI Line0 Interrupt                                              */
#define STM32H745_IRQ_EXTI1                  (STM32H745_IRQ_FIRST + 7)      /*!< EXTI Line1 Interrupt                                              */
#define STM32H745_IRQ_EXTI2                  (STM32H745_IRQ_FIRST + 8)      /*!< EXTI Line2 Interrupt                                              */
#define STM32H745_IRQ_EXTI3                  (STM32H745_IRQ_FIRST + 9)      /*!< EXTI Line3 Interrupt                                              */
#define STM32H745_IRQ_EXTI4                  (STM32H745_IRQ_FIRST + 10)     /*!< EXTI Line4 Interrupt                                              */
#define STM32H745_IRQ_DMA1_Stream0           (STM32H745_IRQ_FIRST + 11)     /*!< DMA1 Stream 0 global Interrupt                                    */
#define STM32H745_IRQ_DMA1_Stream1           (STM32H745_IRQ_FIRST + 12)     /*!< DMA1 Stream 1 global Interrupt                                    */
#define STM32H745_IRQ_DMA1_Stream2           (STM32H745_IRQ_FIRST + 13)     /*!< DMA1 Stream 2 global Interrupt                                    */
#define STM32H745_IRQ_DMA1_Stream3           (STM32H745_IRQ_FIRST + 14)     /*!< DMA1 Stream 3 global Interrupt                                    */
#define STM32H745_IRQ_DMA1_Stream4           (STM32H745_IRQ_FIRST + 15)     /*!< DMA1 Stream 4 global Interrupt                                    */
#define STM32H745_IRQ_DMA1_Stream5           (STM32H745_IRQ_FIRST + 16)     /*!< DMA1 Stream 5 global Interrupt                                    */
#define STM32H745_IRQ_DMA1_Stream6           (STM32H745_IRQ_FIRST + 17)     /*!< DMA1 Stream 6 global Interrupt                                    */
#define STM32H745_IRQ_ADC                    (STM32H745_IRQ_FIRST + 18)     /*!< ADC1 and  ADC2 global Interrupts                                  */
#define STM32H745_IRQ_FDCAN1_IT0             (STM32H745_IRQ_FIRST + 19)     /*!< FDCAN1 Interrupt line 0                                           */
#define STM32H745_IRQ_FDCAN2_IT0             (STM32H745_IRQ_FIRST + 20)     /*!< FDCAN2 Interrupt line 0                                           */
#define STM32H745_IRQ_FDCAN1_IT1             (STM32H745_IRQ_FIRST + 21)     /*!< FDCAN1 Interrupt line 1                                           */
#define STM32H745_IRQ_FDCAN2_IT1             (STM32H745_IRQ_FIRST + 22)     /*!< FDCAN2 Interrupt line 1                                           */
#define STM32H745_IRQ_EXTI9_5                (STM32H745_IRQ_FIRST + 23)     /*!< External Line[9:5] Interrupts                                     */
#define STM32H745_IRQ_TIM1_BRK               (STM32H745_IRQ_FIRST + 24)     /*!< TIM1 Break Interrupt                                              */
#define STM32H745_IRQ_TIM1_UP                (STM32H745_IRQ_FIRST + 25)     /*!< TIM1 Update Interrupt                                             */
#define STM32H745_IRQ_TIM1_TRG_COM           (STM32H745_IRQ_FIRST + 26)     /*!< TIM1 Trigger and Commutation Interrupt                            */
#define STM32H745_IRQ_TIM1_CC                (STM32H745_IRQ_FIRST + 27)     /*!< TIM1 Capture Compare Interrupt                                    */
#define STM32H745_IRQ_TIM2                   (STM32H745_IRQ_FIRST + 28)     /*!< TIM2 global Interrupt                                             */
#define STM32H745_IRQ_TIM3                   (STM32H745_IRQ_FIRST + 29)     /*!< TIM3 global Interrupt                                             */
#define STM32H745_IRQ_TIM4                   (STM32H745_IRQ_FIRST + 30)     /*!< TIM4 global Interrupt                                             */
#define STM32H745_IRQ_I2C1_EV                (STM32H745_IRQ_FIRST + 31)     /*!< I2C1 Event Interrupt                                              */
#define STM32H745_IRQ_I2C1_ER                (STM32H745_IRQ_FIRST + 32)     /*!< I2C1 Error Interrupt                                              */
#define STM32H745_IRQ_I2C2_EV                (STM32H745_IRQ_FIRST + 33)     /*!< I2C2 Event Interrupt                                              */
#define STM32H745_IRQ_I2C2_ER                (STM32H745_IRQ_FIRST + 34)     /*!< I2C2 Error Interrupt                                              */
#define STM32H745_IRQ_SPI1                   (STM32H745_IRQ_FIRST + 35)     /*!< SPI1 global Interrupt                                             */
#define STM32H745_IRQ_SPI2                   (STM32H745_IRQ_FIRST + 36)     /*!< SPI2 global Interrupt                                             */
#define STM32H745_IRQ_USART1                 (STM32H745_IRQ_FIRST + 37)     /*!< USART1 global Interrupt                                           */
#define STM32H745_IRQ_USART2                 (STM32H745_IRQ_FIRST + 38)     /*!< USART2 global Interrupt                                           */
#define STM32H745_IRQ_USART3                 (STM32H745_IRQ_FIRST + 39)     /*!< USART3 global Interrupt                                           */
#define STM32H745_IRQ_EXTI15_10              (STM32H745_IRQ_FIRST + 40)     /*!< External Line[15:10] Interrupts                                   */
#define STM32H745_IRQ_RTC_Alarm              (STM32H745_IRQ_FIRST + 41)     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
/* Reserved */
#define STM32H745_IRQ_TIM8_BRK_TIM12         (STM32H745_IRQ_FIRST + 43)     /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
#define STM32H745_IRQ_TIM8_UP_TIM13          (STM32H745_IRQ_FIRST + 44)     /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
#define STM32H745_IRQ_TIM8_TRG_COM_TIM14     (STM32H745_IRQ_FIRST + 45)     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
#define STM32H745_IRQ_TIM8_CC                (STM32H745_IRQ_FIRST + 46)     /*!< TIM8 Capture Compare Interrupt                                    */
#define STM32H745_IRQ_DMA1_Stream7           (STM32H745_IRQ_FIRST + 47)     /*!< DMA1 Stream7 Interrupt                                            */
#define STM32H745_IRQ_FMC                    (STM32H745_IRQ_FIRST + 48)     /*!< FMC global Interrupt                                              */
#define STM32H745_IRQ_SDMMC1                 (STM32H745_IRQ_FIRST + 49)     /*!< SDMMC1 global Interrupt                                           */
#define STM32H745_IRQ_TIM5                   (STM32H745_IRQ_FIRST + 50)     /*!< TIM5 global Interrupt                                             */
#define STM32H745_IRQ_SPI3                   (STM32H745_IRQ_FIRST + 51)     /*!< SPI3 global Interrupt                                             */
#define STM32H745_IRQ_UART4                  (STM32H745_IRQ_FIRST + 52)     /*!< UART4 global Interrupt                                            */
#define STM32H745_IRQ_UART5                  (STM32H745_IRQ_FIRST + 53)     /*!< UART5 global Interrupt                                            */
#define STM32H745_IRQ_TIM6_DAC               (STM32H745_IRQ_FIRST + 54)     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
#define STM32H745_IRQ_TIM7                   (STM32H745_IRQ_FIRST + 55)     /*!< TIM7 global interrupt                                             */
#define STM32H745_IRQ_DMA2_Stream0           (STM32H745_IRQ_FIRST + 56)     /*!<   DMA2 Stream 0 global Interrupt                                  */
#define STM32H745_IRQ_DMA2_Stream1           (STM32H745_IRQ_FIRST + 57)     /*!<   DMA2 Stream 1 global Interrupt                                  */
#define STM32H745_IRQ_DMA2_Stream2           (STM32H745_IRQ_FIRST + 58)     /*!<   DMA2 Stream 2 global Interrupt                                  */
#define STM32H745_IRQ_DMA2_Stream3           (STM32H745_IRQ_FIRST + 59)     /*!<   DMA2 Stream 3 global Interrupt                                  */
#define STM32H745_IRQ_DMA2_Stream4           (STM32H745_IRQ_FIRST + 60)     /*!<   DMA2 Stream 4 global Interrupt                                  */
#define STM32H745_IRQ_ETH                    (STM32H745_IRQ_FIRST + 61)     /*!< Ethernet global Interrupt                                         */
#define STM32H745_IRQ_ETH_WKUP               (STM32H745_IRQ_FIRST + 62)     /*!< Ethernet Wakeup through EXTI line Interrupt                       */
#define STM32H745_IRQ_FDCAN_CAL              (STM32H745_IRQ_FIRST + 63)     /*!< FDCAN Calibration unit Interrupt                                  */
#define STM32H745_IRQ_CM7_SEV                (STM32H745_IRQ_FIRST + 64)     /*!< CM7 Send event interrupt for CM4                                  */
#define STM32H745_IRQ_CM4_SEV                (STM32H745_IRQ_FIRST + 65)     /*!< CM4 Send event interrupt for CM7                                  */
/* Reserved */
/* Reserved */
#define STM32H745_IRQ_DMA2_Stream5           (STM32H745_IRQ_FIRST + 68)     /*!< DMA2 Stream 5 global interrupt                                    */
#define STM32H745_IRQ_DMA2_Stream6           (STM32H745_IRQ_FIRST + 69)     /*!< DMA2 Stream 6 global interrupt                                    */
#define STM32H745_IRQ_DMA2_Stream7           (STM32H745_IRQ_FIRST + 70)     /*!< DMA2 Stream 7 global interrupt                                    */
#define STM32H745_IRQ_USART6                 (STM32H745_IRQ_FIRST + 71)     /*!< USART6 global interrupt                                           */
#define STM32H745_IRQ_I2C3_EV                (STM32H745_IRQ_FIRST + 72)     /*!< I2C3 event interrupt                                              */
#define STM32H745_IRQ_I2C3_ER                (STM32H745_IRQ_FIRST + 73)     /*!< I2C3 error interrupt                                              */
#define STM32H745_IRQ_OTG_HS_EP1_OUT         (STM32H745_IRQ_FIRST + 74)     /*!< USB OTG HS End Point 1 Out global interrupt                       */
#define STM32H745_IRQ_OTG_HS_EP1_IN          (STM32H745_IRQ_FIRST + 75)     /*!< USB OTG HS End Point 1 In global interrupt                        */
#define STM32H745_IRQ_OTG_HS_WKUP            (STM32H745_IRQ_FIRST + 76)     /*!< USB OTG HS Wakeup through EXTI interrupt                          */
#define STM32H745_IRQ_OTG_HS                 (STM32H745_IRQ_FIRST + 77)     /*!< USB OTG HS global interrupt                                       */
#define STM32H745_IRQ_DCMI                   (STM32H745_IRQ_FIRST + 78)     /*!< DCMI global interrupt                                             */
/* Reserved */
#define STM32H745_IRQ_RNG                    (STM32H745_IRQ_FIRST + 80)     /*!< RNG global interrupt                                              */
#define STM32H745_IRQ_FPU                    (STM32H745_IRQ_FIRST + 81)     /*!< FPU global interrupt                                              */
#define STM32H745_IRQ_UART7                  (STM32H745_IRQ_FIRST + 82)     /*!< UART7 global interrupt                                            */
#define STM32H745_IRQ_UART8                  (STM32H745_IRQ_FIRST + 83)     /*!< UART8 global interrupt                                            */
#define STM32H745_IRQ_SPI4                   (STM32H745_IRQ_FIRST + 84)     /*!< SPI4 global Interrupt                                             */
#define STM32H745_IRQ_SPI5                   (STM32H745_IRQ_FIRST + 85)     /*!< SPI5 global Interrupt                                             */
#define STM32H745_IRQ_SPI6                   (STM32H745_IRQ_FIRST + 86)     /*!< SPI6 global Interrupt                                             */
#define STM32H745_IRQ_SAI1                   (STM32H745_IRQ_FIRST + 87)     /*!< SAI1 global Interrupt                                             */
#define STM32H745_IRQ_LTDC                   (STM32H745_IRQ_FIRST + 88)     /*!< LTDC global Interrupt                                             */
#define STM32H745_IRQ_LTDC_ER                (STM32H745_IRQ_FIRST + 89)     /*!< LTDC Error global Interrupt                                       */
#define STM32H745_IRQ_DMA2D                  (STM32H745_IRQ_FIRST + 90)     /*!< DMA2D global Interrupt                                            */
#define STM32H745_IRQ_SAI2                   (STM32H745_IRQ_FIRST + 91)     /*!< SAI2 global Interrupt                                             */
#define STM32H745_IRQ_QUADSPI                (STM32H745_IRQ_FIRST + 92)     /*!< Quad SPI global interrupt                                         */
#define STM32H745_IRQ_LPTIM1                 (STM32H745_IRQ_FIRST + 93)     /*!< LP TIM1 interrupt                                                 */
#define STM32H745_IRQ_CEC                    (STM32H745_IRQ_FIRST + 94)     /*!< HDMI-CEC global Interrupt                                         */
#define STM32H745_IRQ_I2C4_EV                (STM32H745_IRQ_FIRST + 95)     /*!< I2C4 Event Interrupt                                              */
#define STM32H745_IRQ_I2C4_ER                (STM32H745_IRQ_FIRST + 96)     /*!< I2C4 Error Interrupt                                              */
#define STM32H745_IRQ_SPDIF_RX               (STM32H745_IRQ_FIRST + 97)     /*!< SPDIF-RX global Interrupt                                         */
#define STM32H745_IRQ_OTG_FS_EP1_OUT         (STM32H745_IRQ_FIRST + 98)     /*!< USB OTG HS2 global interrupt                                      */
#define STM32H745_IRQ_OTG_FS_EP1_IN          (STM32H745_IRQ_FIRST + 99)     /*!< USB OTG HS2 End Point 1 Out global interrupt                      */
#define STM32H745_IRQ_OTG_FS_WKUP            (STM32H745_IRQ_FIRST + 100)    /*!< USB OTG HS2 End Point 1 In global interrupt                       */
#define STM32H745_IRQ_OTG_FS                 (STM32H745_IRQ_FIRST + 101)    /*!< USB OTG HS2 Wakeup through EXTI interrupt                         */
#define STM32H745_IRQ_DMAMUX1_OVR            (STM32H745_IRQ_FIRST + 102)    /*!<DMAMUX1 Overrun interrupt                                          */
#define STM32H745_IRQ_HRTIM1_Master          (STM32H745_IRQ_FIRST + 103)    /*!< HRTIM Master Timer global Interrupts                              */
#define STM32H745_IRQ_HRTIM1_TIMA            (STM32H745_IRQ_FIRST + 104)    /*!< HRTIM Timer A global Interrupt                                    */
#define STM32H745_IRQ_HRTIM1_TIMB            (STM32H745_IRQ_FIRST + 105)    /*!< HRTIM Timer B global Interrupt                                    */
#define STM32H745_IRQ_HRTIM1_TIMC            (STM32H745_IRQ_FIRST + 106)    /*!< HRTIM Timer C global Interrupt                                    */
#define STM32H745_IRQ_HRTIM1_TIMD            (STM32H745_IRQ_FIRST + 107)    /*!< HRTIM Timer D global Interrupt                                    */
#define STM32H745_IRQ_HRTIM1_TIME            (STM32H745_IRQ_FIRST + 108)    /*!< HRTIM Timer E global Interrupt                                    */
#define STM32H745_IRQ_HRTIM1_FLT             (STM32H745_IRQ_FIRST + 109)    /*!< HRTIM Fault global Interrupt                                      */
#define STM32H745_IRQ_DFSDM1_FLT0            (STM32H745_IRQ_FIRST + 110)    /*!<DFSDM Filter1 Interrupt                                            */
#define STM32H745_IRQ_DFSDM1_FLT1            (STM32H745_IRQ_FIRST + 111)    /*!<DFSDM Filter2 Interrupt                                            */
#define STM32H745_IRQ_DFSDM1_FLT2            (STM32H745_IRQ_FIRST + 112)    /*!<DFSDM Filter3 Interrupt                                            */
#define STM32H745_IRQ_DFSDM1_FLT3            (STM32H745_IRQ_FIRST + 113)    /*!<DFSDM Filter4 Interrupt                                            */
#define STM32H745_IRQ_SAI3                   (STM32H745_IRQ_FIRST + 114)    /*!< SAI3 global Interrupt                                             */
#define STM32H745_IRQ_SWPMI1                 (STM32H745_IRQ_FIRST + 115)    /*!< Serial Wire Interface 1 global interrupt                          */
#define STM32H745_IRQ_TIM15                  (STM32H745_IRQ_FIRST + 116)    /*!< TIM15 global Interrupt                                            */
#define STM32H745_IRQ_TIM16                  (STM32H745_IRQ_FIRST + 117)    /*!< TIM16 global Interrupt                                            */
#define STM32H745_IRQ_TIM17                  (STM32H745_IRQ_FIRST + 118)    /*!< TIM17 global Interrupt                                            */
#define STM32H745_IRQ_MDIOS_WKUP             (STM32H745_IRQ_FIRST + 119)    /*!< MDIOS Wakeup  Interrupt                                           */
#define STM32H745_IRQ_MDIOS                  (STM32H745_IRQ_FIRST + 120)    /*!< MDIOS global Interrupt                                            */
#define STM32H745_IRQ_JPEG                   (STM32H745_IRQ_FIRST + 121)    /*!< JPEG global Interrupt                                             */
#define STM32H745_IRQ_MDMA                   (STM32H745_IRQ_FIRST + 122)    /*!< MDMA global Interrupt                                             */
/* Reserved */
#define STM32H745_IRQ_SDMMC2                 (STM32H745_IRQ_FIRST + 124)    /*!< SDMMC2 global Interrupt                                           */
#define STM32H745_IRQ_HSEM1                  (STM32H745_IRQ_FIRST + 125)    /*!< HSEM1 global Interrupt                                            */
#define STM32H745_IRQ_HSEM2                  (STM32H745_IRQ_FIRST + 126)    /*!< HSEM2 global Interrupt                                            */
#define STM32H745_IRQ_ADC3                   (STM32H745_IRQ_FIRST + 127)    /*!< ADC3 global Interrupt                                             */
#define STM32H745_IRQ_DMAMUX2_OVR            (STM32H745_IRQ_FIRST + 128)    /*!<DMAMUX2 Overrun interrupt                                          */
#define STM32H745_IRQ_BDMA_Channel0          (STM32H745_IRQ_FIRST + 129)    /*!< BDMA Channel 0 global Interrupt                                   */
#define STM32H745_IRQ_BDMA_Channel1          (STM32H745_IRQ_FIRST + 130)    /*!< BDMA Channel 1 global Interrupt                                   */
#define STM32H745_IRQ_BDMA_Channel2          (STM32H745_IRQ_FIRST + 131)    /*!< BDMA Channel 2 global Interrupt                                   */
#define STM32H745_IRQ_BDMA_Channel3          (STM32H745_IRQ_FIRST + 132)    /*!< BDMA Channel 3 global Interrupt                                   */
#define STM32H745_IRQ_BDMA_Channel4          (STM32H745_IRQ_FIRST + 133)    /*!< BDMA Channel 4 global Interrupt                                   */
#define STM32H745_IRQ_BDMA_Channel5          (STM32H745_IRQ_FIRST + 134)    /*!< BDMA Channel 5 global Interrupt                                   */
#define STM32H745_IRQ_BDMA_Channel6          (STM32H745_IRQ_FIRST + 135)    /*!< BDMA Channel 6 global Interrupt                                   */
#define STM32H745_IRQ_BDMA_Channel7          (STM32H745_IRQ_FIRST + 136)    /*!< BDMA Channel 7 global Interrupt                                   */
#define STM32H745_IRQ_COMP                   (STM32H745_IRQ_FIRST + 137)    /*!< COMP global Interrupt                                             */
#define STM32H745_IRQ_LPTIM2                 (STM32H745_IRQ_FIRST + 138)    /*!< LP TIM2 global interrupt                                          */
#define STM32H745_IRQ_LPTIM3                 (STM32H745_IRQ_FIRST + 139)    /*!< LP TIM3 global interrupt                                          */
#define STM32H745_IRQ_LPTIM4                 (STM32H745_IRQ_FIRST + 140)    /*!< LP TIM4 global interrupt                                          */
#define STM32H745_IRQ_LPTIM5                 (STM32H745_IRQ_FIRST + 141)    /*!< LP TIM5 global interrupt                                          */
#define STM32H745_IRQ_LPUART1                (STM32H745_IRQ_FIRST + 142)    /*!< LP UART1 interrupt                                                */
#define STM32H745_IRQ_WWDG_RST               (STM32H745_IRQ_FIRST + 143)    /*!<Window Watchdog reset interrupt (exti_d2_wwdg_it, exti_d1_wwdg_it) */
#define STM32H745_IRQ_CRS                    (STM32H745_IRQ_FIRST + 144)    /*!< Clock Recovery Global Interrupt                                   */
#define STM32H745_IRQ_ECC                    (STM32H745_IRQ_FIRST + 145)    /*!< ECC diagnostic Global Interrupt                                   */
#define STM32H745_IRQ_SAI4                   (STM32H745_IRQ_FIRST + 146)    /*!< SAI4 global interrupt                                             */
/* Reserved */
#define STM32H745_IRQ_HOLD_CORE              (STM32H745_IRQ_FIRST + 148)    /*!< Hold core interrupt                                               */
#define STM32H745_IRQ_WAKEUP_PIN             (STM32H745_IRQ_FIRST + 149)    /*!< Interrupt for all 6 wake-up pins                                  */

#if defined(CONFIG_STM32H745_STM32H745ZI)
#  define STM32H745_IRQ_NEXTINTS  150
#else
#  error "Unsupported STM32H745 chip"
#endif

/* (EXTI interrupts do not use IRQ numbers) */

#define NR_IRQS                 (STM32H745_IRQ_FIRST + STM32H745_IRQ_NEXTINTS)

/****************************************************************************************************
 * Public Types
 ****************************************************************************************************/

/****************************************************************************************************
 * Public Data
 ****************************************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************************************
 * Public Functions
 ****************************************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif /* __ARCH_ARM_INCLUDE_STM32H745_STM32H745XX_IRQ_H */
