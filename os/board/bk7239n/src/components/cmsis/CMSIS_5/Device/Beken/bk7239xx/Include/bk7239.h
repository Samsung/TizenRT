/*
 * Copyright (c) 2009-2018 Arm Limited.
 * Copyright (c) 2018-2020 ArmChina.
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "armstar.h"

/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* -------------------  Processor Exceptions Numbers  ----------------------------- */
  NonMaskableInt_IRQn           = -14,     /*  2 Non Maskable Interrupt */
  HardFault_IRQn                = -13,     /*  3 HardFault Interrupt */
  MemoryManagement_IRQn         = -12,     /*  4 Memory Management Interrupt */
  BusFault_IRQn                 = -11,     /*  5 Bus Fault Interrupt */
  UsageFault_IRQn               = -10,     /*  6 Usage Fault Interrupt */
  SecureFault_IRQn              =  -9,     /*  7 Secure Fault Interrupt */
  SVCall_IRQn                   =  -5,     /* 11 SV Call Interrupt */
  DebugMonitor_IRQn             =  -4,     /* 12 Debug Monitor Interrupt */
  PendSV_IRQn                   =  -2,     /* 14 Pend SV Interrupt */
  SysTick_IRQn                  =  -1,     /* 15 System Tick Interrupt */

/* -------------------  BK7239 Specific Interrupt Numbers  ----------------------------- */
  DMA0_NSEC_IRQn                =   0,
  ENCP_SEC_IRQn                 =   1,
  ENCP_NSEC_IRQn                =   2,
  TIMER0_IRQn                   =   3,
  UART0_IRQn                    =   4,
  PWM0_IRQn                     =   5,
  I2C0_IRQn                     =   6,
  SPI0_IRQn                     =   7,
  SARADC_IRQn                   =   8,
  IRDA_IRQn                     =   9,
  SDIO_IRQn                     =   10,
  GDMA_IRQn                     =   11,
  LA_IRQn                       =   12,
  TIMER1_IRQn                   =   13,
  I2C1_IRQn                     =   14,
  UART1_IRQn                    =   15,
  UART2_IRQn                    =   16,
  SPI1_IRQn                     =   17,
  CAN_IRQn                      =   18,
  USB_IRQn                      =   19,
  QSPI0_IRQn                    =   20,
  CKMN_IRQn                     =   21,
  SBC_IRQn                      =   22,
  AUDIO_IRQn                    =   23,
  I2S0_IRQn                     =   24,
  JPEG_ENC_IRQn                 =   25,
  JPEG_DEC_IRQn                 =   26,
  DISPLAY_IRQn                  =   27,
  DMA2D_IRQn                    =   28,
  PHY_MBP_IRQn                  =   29,
  PHY_RIU_IRQn                  =   30,
  MAC_INT_TX_RX_TIMER_IRQn      =   31,
  MAC_INT_TX_RX_MISC_IRQn       =   32,
  MAC_INT_RX_TRIGGER_IRQn       =   33,
  MAC_INT_TX_TRIGGER_IRQn       =   34,
  MAC_INT_PORT_TRIGGER_IRQn     =   35,
  MAC_INT_GEN_IRQn              =   36,
  HSU_IRQn                      =   37,
  INT_MAC_WAKEUP_IRQn           =   38,
  DM_IRQn                       =   39,
  BLE_IRQn                      =   40,
  BT_IRQn                       =   41,
  QSPI1_IRQn                    =   42,
  PWM1_IRQn                     =   43,
  I2S1_IRQn                     =   44,
  I2S2_IRQn                     =   45,
  H264_IRQn                     =   46,
  SDMADC_IRQn                   =   47,
  ETHERNET_IRQn                 =   48,
  SCAL0_IRQn                    =   49,
  BMC64_IRQn                    =   50,
  DPLL_UNLOCK_IRQn              =   51,
  TOUCH_IRQn                    =   52,
  USB_PLUG_IRQn                 =   53,
  RTC_IRQn                      =   54,
  GPIO_IRQn                     =   55,
  DMA1_SEC_IRQn                 =   56,
  DMA1_NSEC_IRQn                =   57,
  YUV_BUF_IRQn                  =   58,
  ROTT_IRQn                     =   59,

  
  InterruptMAX_IRQn
  /* Interrupts 64 .. 480 are left out */
} IRQn_Type;
// eof

