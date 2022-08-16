/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/stm32h745/stm32h745_usart.c
 *
 *   Copyright (C) 2009, 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>

#include <arch/board/board.h>

#include "up_internal.h"
#include "up_arch.h"

#include "chip.h"
#include <stm32h7xx_hal.h>
#include <stm32h7xx_ll_rcc.h>
#include <stm32h7xx_ll_bus.h>
#include <stm32h7xx_ll_gpio.h>
#include <stm32h7xx_ll_usart.h>
#include <system_stm32h745.h>

#if defined(CONFIG_USART2_SERIAL_CONSOLE)
  #define CONSOLE_UART                  USART2
  #define CONSOLE_PORT                  GPIOA
  #define CONSOLE_TX_PIN                LL_GPIO_PIN_2
  #define CONSOLE_RX_PIN                LL_GPIO_PIN_3
  #define CONSOLE_AF                    LL_GPIO_AF_7
  #define CONSOLE_UART_CLOCK_ENABLE()   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
  #define CONSOLE_PORT_CLOCK_ENABLE()   LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA)
  #define CONSOLE_CLOCK_SOURCE()        LL_RCC_SetUSARTClockSource(LL_RCC_USART234578_CLKSOURCE_PCLK1)
  #define CONSOLE_BAUDRATE		CONFIG_USART2_BAUD
#elif defined(CONFIG_USART3_SERIAL_CONSOLE)
  #define CONSOLE_UART                  USART3
  #define CONSOLE_PORT                  GPIOB
  #define CONSOLE_TX_PIN                LL_GPIO_PIN_10
  #define CONSOLE_RX_PIN                LL_GPIO_PIN_11
  #define CONSOLE_AF                    LL_GPIO_AF_7
  #define CONSOLE_UART_CLOCK_ENABLE()   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)
  #define CONSOLE_PORT_CLOCK_ENABLE()   LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB)
  #define CONSOLE_CLOCK_SOURCE()        LL_RCC_SetUSARTClockSource(LL_RCC_USART234578_CLKSOURCE_PCLK1)
  #define CONSOLE_BAUDRATE              CONFIG_USART3_BAUD
#elif defined(CONFIG_USART6_SERIAL_CONSOLE)
  #define CONSOLE_UART                  USART6
  #define CONSOLE_PORT                  GPIOG
  #define CONSOLE_TX_PIN                LL_GPIO_PIN_9
  #define CONSOLE_RX_PIN                LL_GPIO_PIN_14
  #define CONSOLE_AF                    LL_GPIO_AF_7
  #define CONSOLE_UART_CLOCK_ENABLE()   LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART6)
  #define CONSOLE_PORT_CLOCK_ENABLE()   LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOG)
  #define CONSOLE_CLOCK_SOURCE()        LL_RCC_SetUSARTClockSource(LL_RCC_USART16_CLKSOURCE_PCLK2)
  #define CONSOLE_BAUDRATE              CONFIG_USART6_BAUD
#endif

void up_earlyserialinit(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  CONSOLE_CLOCK_SOURCE();

  /* Peripheral clock enable */
  CONSOLE_UART_CLOCK_ENABLE();
  CONSOLE_PORT_CLOCK_ENABLE();
  /**CONSOLE_UART GPIO Configuration
  CONSOLE_TX_PIN   ------> TX
  CONSOLE_RX_PIN   ------> RX
  */
  GPIO_InitStruct.Pin = CONSOLE_TX_PIN|CONSOLE_RX_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = CONSOLE_AF;
  LL_GPIO_Init(CONSOLE_PORT, &GPIO_InitStruct);

  USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate = CONSOLE_BAUDRATE;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(CONSOLE_UART, &USART_InitStruct);
  LL_USART_SetTXFIFOThreshold(CONSOLE_UART, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_SetRXFIFOThreshold(CONSOLE_UART, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_DisableFIFO(CONSOLE_UART);
  LL_USART_ConfigAsyncMode(CONSOLE_UART);

  LL_USART_Enable(CONSOLE_UART);

  /* Polling CONSOLE UART initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(CONSOLE_UART))) || (!(LL_USART_IsActiveFlag_REACK(CONSOLE_UART))))
  {
  }  
}

/****************************************************************************
 * Name: up_lowputc
 *
 * Description:
 *   Output one byte on the serial console
 *
 * Input Parameters:
 *   ch - chatacter to output
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void up_lowputc(char ch)
{
  LL_USART_DisableIT_TC(CONSOLE_UART);
  while(!LL_USART_IsActiveFlag_TXE(CONSOLE_UART)){};
  LL_USART_TransmitData8(CONSOLE_UART, (uint8_t)ch);
  while(!LL_USART_IsActiveFlag_TC(CONSOLE_UART)){};
}


/****************************************************************************
 * Name: up_putc
 *
 * Description:
 *   Output one byte on the serial console
 *
 * Input Parameters:
 *   ch - chatacter to output
 *
 * Returned Value:
 *  sent character
 *
 ****************************************************************************/
int up_putc(int ch)
{
    if (ch == '\n')
    {
        up_lowputc('\r');
    }

    up_lowputc((char)ch);
    
    return ch;
}


/****************************************************************************
 * Name: up_getc
 *
 * Description:
 *   Get one character from the UART
 *
 ****************************************************************************/
uint8_t up_getc(void)
{
  uint8_t ch = 0x0;
  if(LL_USART_IsActiveFlag_ORE(CONSOLE_UART))
  {
    LL_USART_ClearFlag_ORE(CONSOLE_UART);
  }

  while(!LL_USART_IsActiveFlag_RXNE(CONSOLE_UART)){};

  if(ch == LL_USART_ReceiveData8(CONSOLE_UART))
  {
    return ch;
  }
  else
  {
    return -1;
  }
}




