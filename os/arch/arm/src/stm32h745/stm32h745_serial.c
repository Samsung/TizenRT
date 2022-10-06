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
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/stm32h745/stm32h745_serial.c
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

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/serial/serial.h>

#include <arch/serial.h>
#include <arch/board/board.h>
#include <errno.h>

#include "up_internal.h"
#include "up_arch.h"

#ifdef CONFIG_SERIAL_TERMIOS
#include <termios.h>
#endif

#include "chip.h"
#include <stm32h7xx_hal.h>
#include <stm32h7xx_ll_rcc.h>
#include <stm32h7xx_ll_bus.h>
#include <stm32h7xx_ll_gpio.h>
#include <stm32h7xx_ll_usart.h>
#include <system_stm32h745.h>

#if defined(CONFIG_USART2_SERIAL_CONSOLE)

  #define CONSOLE_DEV             g_uart2             /* USART2 is console */
#ifdef CONFIG_USART3_ISUART
  #define TTYS0_DEV               g_uart3             /* USART3 is ttyS0 */
#endif
#ifdef CONFIG_USART6_ISUART
  #define TTYS1_DEV               g_uart6             /* USART6 is ttyS1 */
#endif

#elif defined(CONFIG_USART3_SERIAL_CONSOLE)

  #define CONSOLE_DEV             g_uart3             /* USART3 is console */
#ifdef CONFIG_USART2_ISUART
  #define TTYS0_DEV               g_uart2             /* USART2 is ttyS0 */
#endif
#ifdef CONFIG_USART6_ISUART
  #define TTYS1_DEV               g_uart6             /* USART6 is ttyS1 */
#endif

#elif defined(CONFIG_USART6_SERIAL_CONSOLE)

  #define CONSOLE_DEV             g_uart6             /* USART6 is console */
#ifdef CONFIG_USART2_ISUART
  #define TTYS0_DEV               g_uart2             /* USART2 is ttyS0 */
#endif
#ifdef CONFIG_USART3_ISUART
  #define TTYS1_DEV               g_uart3             /* USART3 is ttyS1 */
#endif

#endif //CONFIG_USART2_SERIAL_CONSOLE



struct stm32h745_up_dev_s
{
  USART_TypeDef *USART;
  uint32_t parity;       /* 0=none, 1=odd, 2=even */
  uint32_t bits;         /* Number of bits (7 or 8) */
  uint32_t stopbit;      /* LL_USART_STOPBITS_1 or LL_USART_STOPBITS_2 */
  uint32_t baud;        /* Configured baud */
  uint32_t irq;
  uint32_t tx;
  uint32_t rx;
  uint32_t rts;
  uint32_t cts;
  uint32_t FlowControl;
  bool txint_enable;
  bool rxint_enable;
#ifdef CONFIG_SERIAL_IFLOWCONTROL
  uint32_t iflow:1;      /* input flow control (RTS) enabled */
#endif
#ifdef CONFIG_SERIAL_OFLOWCONTROL
  uint32_t oflow:1;      /* output flow control (CTS) enabled */
#endif
  uint8_t tx_level;
};

static int  stm32h745_up_setup(struct uart_dev_s *dev);
static void stm32h745_up_shutdown(struct uart_dev_s *dev);
static int  stm32h745_up_attach(struct uart_dev_s *dev);
static void stm32h745_up_detach(struct uart_dev_s *dev);
static int  up_interrupt(int irq, void *context, FAR void *arg);
static int  stm32h745_up_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg);
static int  stm32h745_up_receive(struct uart_dev_s *dev, unsigned int *status);
static void stm32h745_up_rxint(struct uart_dev_s *dev, bool enable);
static bool stm32h745_up_rxavailable(struct uart_dev_s *dev);
static void stm32h745_up_send(struct uart_dev_s *dev, int ch);
static void stm32h745_up_txint(struct uart_dev_s *dev, bool enable);
static bool stm32h745_up_txready(struct uart_dev_s *dev);
static bool stm32h745_up_txempty(struct uart_dev_s *dev);

static const struct uart_ops_s g_uart_ops = 
{
  .setup         = stm32h745_up_setup,
  .shutdown      = stm32h745_up_shutdown,
  .attach        = stm32h745_up_attach,
  .detach        = stm32h745_up_detach,
  .ioctl         = stm32h745_up_ioctl,
  .receive       = stm32h745_up_receive,
  .rxint         = stm32h745_up_rxint,
  .rxavailable   = stm32h745_up_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
  .rxflowcontrol = NULL,
#endif
  .send          = stm32h745_up_send,
  .txint         = stm32h745_up_txint,
  .txready       = stm32h745_up_txready,
  .txempty       = stm32h745_up_txempty,
};

/****************************************************************************
 * USART 2 configuration
 ****************************************************************************/
#ifdef CONFIG_USART2_ISUART
static char g_uart2_rxbuffer[CONFIG_USART2_RXBUFSIZE];
static char g_uart2_txbuffer[CONFIG_USART2_TXBUFSIZE];

static struct stm32h745_up_dev_s g_uart2_priv = 
{
  .USART = USART2,
#if (CONFIG_USART2_PARITY == 1)
  .parity = LL_USART_PARITY_ODD,
#elif (CONFIG_USART2_PARITY == 2)
  .parity = LL_USART_PARITY_EVEN,
#else
  .parity = LL_USART_PARITY_NONE,
#endif

#if (CONFIG_USART2_BITS == 7)
  .bits = LL_USART_DATAWIDTH_7B,
#elif (CONFIG_USART2_BITS == 8)
  .bits = LL_USART_DATAWIDTH_8B,
#else
  #  error "Unsupported UART Bits!!!"
#endif

#if (CONFIG_USART2_2STOP)
  .stopbit = LL_USART_STOPBITS_2,
#else
  .stopbit = LL_USART_STOPBITS_1,
#endif
  .baud = CONFIG_USART2_BAUD,
  .irq  = STM32H745_IRQ_USART2,
  .tx   = 0,
  .rx   = 0,
  .rts  = 0,
  .cts  = 0,
  .FlowControl  = UART_HWCONTROL_NONE,
  .txint_enable = false,
  .rxint_enable = false,
};

static uart_dev_t g_uart2 = 
{
  .isconsole = false,
  .recv = 
  {
    .size   = CONFIG_USART2_RXBUFSIZE,
    .buffer = g_uart2_rxbuffer,
  },
  .xmit = 
  {
    .size   = CONFIG_USART2_TXBUFSIZE,
    .buffer = g_uart2_txbuffer,
  },
  .ops  = &g_uart_ops,
  .priv = &g_uart2_priv,
};
#endif

/****************************************************************************
 * USART 3 configuration
 ****************************************************************************/
#ifdef CONFIG_USART3_ISUART
static char g_uart3_rxbuffer[CONFIG_USART3_RXBUFSIZE];
static char g_uart3_txbuffer[CONFIG_USART3_TXBUFSIZE];

static struct stm32h745_up_dev_s g_uart3_priv = 
{
  .USART = USART3,
#if (CONFIG_USART3_PARITY == 1)
  .parity = LL_USART_PARITY_ODD,
#elif (CONFIG_USART3_PARITY == 2)
  .parity = LL_USART_PARITY_EVEN,
#else
  .parity = LL_USART_PARITY_NONE,
#endif

#if (CONFIG_USART3_BITS == 7)
  .bits = LL_USART_DATAWIDTH_7B,
#elif (CONFIG_USART3_BITS == 8)
  .bits = LL_USART_DATAWIDTH_8B,
#else
  #  error "Unsupported UART Bits!!!"
#endif



#if (CONFIG_USART3_2STOP)
  .stopbit = LL_USART_STOPBITS_2,
#else
  .stopbit = LL_USART_STOPBITS_1,
#endif
  .baud = CONFIG_USART3_BAUD,
  .irq  = STM32H745_IRQ_USART3,
  .tx   = 0,
  .rx   = 0,
  .rts  = 0,
  .cts  = 0,
  .FlowControl  = UART_HWCONTROL_NONE,
  .txint_enable = false,
  .rxint_enable = false,
};

static uart_dev_t g_uart3 = 
{
  .isconsole = true,
  .recv = 
  {
    .size   = CONFIG_USART3_RXBUFSIZE,
    .buffer = g_uart3_rxbuffer,
  },
  .xmit = 
  {
    .size   = CONFIG_USART3_TXBUFSIZE,
    .buffer = g_uart3_txbuffer,
  },
  .ops  = &g_uart_ops,
  .priv = &g_uart3_priv,
};
#endif

/****************************************************************************
 * USART 6 configuration
 ****************************************************************************/
#ifdef CONFIG_USART6_ISUART
static char g_uart6_rxbuffer[CONFIG_USART6_RXBUFSIZE];
static char g_uart6_txbuffer[CONFIG_USART6_TXBUFSIZE];

static struct stm32h745_up_dev_s g_uart6_priv = 
{
  .USART = USART6,
#if (CONFIG_USART6_PARITY == 1)
  .parity = LL_USART_PARITY_ODD,
#elif (CONFIG_USART6_PARITY == 2)
  .parity = LL_USART_PARITY_EVEN,
#else
  .parity = LL_USART_PARITY_NONE,
#endif

#if (CONFIG_USART6_BITS == 7)
  .bits = LL_USART_DATAWIDTH_7B,
#elif (CONFIG_USART6_BITS == 8)
  .bits = LL_USART_DATAWIDTH_8B,
#else
  #  error "Unsupported UART Bits!!!"
#endif

#if (CONFIG_USART6_2STOP)
  .stopbit = LL_USART_STOPBITS_2,
#else
  .stopbit = LL_USART_STOPBITS_1,
#endif
  .baud = CONFIG_USART6_BAUD,
  .irq  = STM32H745_IRQ_USART6,
  .tx   = 0,
  .rx   = 0,
  .rts  = 0,
  .cts  = 0,
  .FlowControl  = UART_HWCONTROL_NONE,
  .txint_enable = false,
  .rxint_enable = false,
};

static uart_dev_t g_uart6 = 
{
  .isconsole = false,
  .recv = 
  {
    .size   = CONFIG_USART6_RXBUFSIZE,
    .buffer = g_uart6_rxbuffer,
  },
  .xmit = 
  {
    .size   = CONFIG_USART6_TXBUFSIZE,
    .buffer = g_uart6_txbuffer,
  },
  .ops  = &g_uart_ops,
  .priv = &g_uart6_priv,
};
#endif

/****************************************************************************
 * Name: up_serialinit
 *
 * Description:
 *   Register serial console and serial ports.  This assumes
 *   that up_earlyserialinit was called previously.
 *
 ****************************************************************************/
void up_serialinit(void)
{

#ifdef CONSOLE_DEV
  CONSOLE_DEV.isconsole = true;
  stm32h745_up_setup(&CONSOLE_DEV);

  uart_register("/dev/console", &CONSOLE_DEV);
#endif

#ifdef TTYS0_DEV
  uart_register("/dev/ttyS0", &TTYS0_DEV);
#endif
#ifdef TTYS1_DEV
  uart_register("/dev/ttyS1", &TTYS1_DEV);
#endif
}

/****************************************************************************
 * Private Functions - stm32h745_up_setup
 ****************************************************************************/
static int  stm32h745_up_setup(struct uart_dev_s *dev)
{
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  LL_USART_InitTypeDef     USART_InitStruct = {0};
  LL_GPIO_InitTypeDef      GPIO_InitStruct = {0};

  if(priv->USART == USART3)
  {
    LL_RCC_SetUSARTClockSource(LL_RCC_USART234578_CLKSOURCE_PCLK1);

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
    /**USART3 GPIO Configuration
    PB10   ------> USART3_TX
    PB11   ------> USART3_RX
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_11;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
  else if(priv->USART == USART2)
  {
    LL_RCC_SetUSARTClockSource(LL_RCC_USART234578_CLKSOURCE_PCLK1);

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
    /**USART2 GPIO Configuration
    PA2   ------> USART2_TX
    PA3   ------> USART2_RX
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
  else if(priv->USART == USART6)
  {
    LL_RCC_SetUSARTClockSource(LL_RCC_USART16_CLKSOURCE_PCLK2);

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART6);
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOG);
    /**USART6 GPIO Configuration
    PG9   ------> USART6_RX
    PG14   ------> USART6_TX
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_9|LL_GPIO_PIN_14;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOG, &GPIO_InitStruct);
  }

  USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate = priv->baud;
  if(priv->parity != LL_USART_PARITY_NONE)
  {
    if(priv->bits == LL_USART_DATAWIDTH_7B)
    {
      USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    }
    else
    {
      USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_9B;
    }
  }
  else
  {
    USART_InitStruct.DataWidth = priv->bits;
  }
  USART_InitStruct.StopBits = priv->stopbit;
  USART_InitStruct.Parity = priv->parity;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;

  LL_USART_DeInit(priv->USART);
  LL_USART_Disable(priv->USART);

  LL_USART_Init(priv->USART, &USART_InitStruct);
  LL_USART_SetTXFIFOThreshold(priv->USART, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_SetRXFIFOThreshold(priv->USART, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_DisableFIFO(priv->USART);
  LL_USART_ConfigAsyncMode(priv->USART);

  LL_USART_Enable(priv->USART);

  while((!(LL_USART_IsActiveFlag_TEACK(priv->USART))) || (!(LL_USART_IsActiveFlag_REACK(priv->USART))))
  {
  }  

  return OK;
}

/****************************************************************************
 * Private Functions - stm32h745_up_shutdown
 ****************************************************************************/
static void stm32h745_up_shutdown(struct uart_dev_s *dev)
{
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  stm32h745_up_detach(dev);

  if(priv->USART == USART3)
  {
    LL_USART_DeInit(USART3);
    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART3);
  }
  else if(priv->USART == USART2)
  {
    LL_USART_DeInit(USART2);
    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART2);
  }
  else if(priv->USART == USART6)
  {
    LL_USART_DeInit(USART6);
    LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_USART6);
  }
}

/****************************************************************************
 * Private Functions - stm32h745_up_attach
 ****************************************************************************/
static int stm32h745_up_attach(struct uart_dev_s *dev)
{
  int ret;
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  ret = irq_attach(priv->irq, up_interrupt, dev);
  if (ret == OK)
  {
    (void)up_enable_irq(priv->irq);
    /*
    * Removed to use default priority
    (void)up_prioritize_irq(priv->irq, NVIC_SYSH_PRIORITY_MIN);
    */
  }
  return ret;
}

/****************************************************************************
 * Private Functions - stm32h745_up_detach
 ****************************************************************************/
static void stm32h745_up_detach(struct uart_dev_s *dev)
{
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  up_disable_irq(priv->irq);
  irq_detach(priv->irq);
}

/****************************************************************************
 * Private Functions - stm32h745_up_interrupt
 ****************************************************************************/
static int  up_interrupt(int irq, void *context, FAR void *arg)
{
  bool handled;
  int  passes;

  FAR uart_dev_t *dev = (FAR uart_dev_t *)arg;
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  handled = true;

  for (passes = 0; passes < 256 && handled; passes++)
  {
    handled = false;

    if(LL_USART_IsActiveFlag_TXE_TXFNF(priv->USART) && 
      LL_USART_IsEnabledIT_TXE_TXFNF(priv->USART))
    {
      uart_xmitchars(dev);
      handled = true;
    }

    if(LL_USART_IsActiveFlag_RXNE_RXFNE(priv->USART) && 
      LL_USART_IsEnabledIT_RXNE_RXFNE(priv->USART))
    {
      uart_recvchars(dev);
      handled = true;
    }

    if(LL_USART_IsActiveFlag_TC(priv->USART) &&
      LL_USART_IsEnabledIT_TC(priv->USART))
    {
      LL_USART_ClearFlag_TC(priv->USART);
      LL_USART_DisableIT_TC(priv->USART);
    }

    if(LL_USART_IsActiveFlag_PE(priv->USART))
    {
      LL_USART_ClearFlag_PE(priv->USART);
    }

    if(LL_USART_IsActiveFlag_FE(priv->USART))
    {
      LL_USART_ClearFlag_FE(priv->USART);
    }

    if(LL_USART_IsActiveFlag_NE(priv->USART))
    {
      LL_USART_ClearFlag_NE(priv->USART);
    }

    if(LL_USART_IsActiveFlag_ORE(priv->USART))
    {
      LL_USART_ClearFlag_ORE(priv->USART);
    }
    
    if(LL_USART_IsActiveFlag_UDR(priv->USART))
    {
      LL_USART_ClearFlag_UDR(priv->USART);
    }
  }

  return OK;
}

/****************************************************************************
 * Private Functions - stm32h745_up_ioctl
 ****************************************************************************/
static int  stm32h745_up_ioctl(FAR struct uart_dev_s *dev, int cmd, unsigned long arg)
{
#if defined(CONFIG_SERIAL_TERMIOS)
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;
  int ret = OK;
  struct termios *termiosp = (struct termios *)arg;

  DEBUGASSERT(priv);
  switch (cmd)
  {
  case TCGETS:
    if (!termiosp)
    {
      return -EINVAL;
    }

    cfsetispeed(termiosp, priv->baud);

    termiosp->c_cflag = 0;

    if(priv->parity != LL_USART_PARITY_NONE)
    {
      termiosp->c_cflag |= PARENB;

      if (priv->parity == LL_USART_PARITY_ODD)
      {
        termiosp->c_cflag |= PARODD;
      }
    }

    if (priv->stopbit == LL_USART_STOPBITS_2)
    {
      termiosp->c_cflag |= CSTOPB;
    }

    if(priv->bits == LL_USART_DATAWIDTH_8B)
    {
      termiosp->c_cflag |= CS8;
    }
    else if(priv->bits == LL_USART_DATAWIDTH_7B)
    {
      termiosp->c_cflag |= CS7;
    }
    else
    {
      return -EINVAL;
    }

    break;

  case TCSETS:
    if (!termiosp)
    {
      return -EINVAL;
    }

    //Bits
    if((termiosp->c_cflag&CS8) == CS8)
    {
      priv->bits = LL_USART_DATAWIDTH_8B;
    }
    else if((termiosp->c_cflag&CS7) == CS7)
    {
      priv->bits = LL_USART_DATAWIDTH_7B;
    }
    else
    {
      return -EINVAL; 
    }

    //Stop bits
    if((termiosp->c_cflag&CSTOPB) == CSTOPB)
    {
      priv->stopbit = LL_USART_STOPBITS_2;
    }
    else
    {
      priv->stopbit = LL_USART_STOPBITS_1; 
    }

    //Parity bits
    priv->parity = LL_USART_PARITY_NONE;
    if (termiosp->c_cflag & PARENB)
    {
      if (termiosp->c_cflag & PARODD)
      {
        priv->parity = LL_USART_PARITY_ODD;
      }
      else
      {
        priv->parity = LL_USART_PARITY_EVEN;
      }
    }

    //Baud rate
    priv->baud = cfgetispeed(termiosp);

    stm32h745_up_shutdown(dev);
    stm32h745_up_setup(dev);
    stm32h745_up_attach(dev);
    stm32h745_up_txint(dev, priv->txint_enable);
    stm32h745_up_rxint(dev, priv->rxint_enable);
    break;

  default:
    ret = -ENOTTY;
    break;
  }
#else
  int ret = -ENOTTY;
#endif
  return ret;
}


/****************************************************************************
 * Private Functions - stm32h745_up_receive
 ****************************************************************************/
static int  stm32h745_up_receive(struct uart_dev_s *dev, unsigned int *status)
{
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  return (int)LL_USART_ReceiveData8(priv->USART);
}

/****************************************************************************
 * Private Functions - stm32h745_up_rxint
 ****************************************************************************/
//LL_USART_EnableIT_RXNE_RXFNE(priv->USART);
//LL_USART_EnableIT_TC(priv->USART);
//LL_USART_EnableIT_TXE_TXFNF(priv->USART);
//LL_USART_EnableIT_PE(priv->USART);
//LL_USART_EnableIT_CM(priv->USART);
//LL_USART_EnableIT_RTO(priv->USART);
//LL_USART_EnableIT_EOB(priv->USART);
//LL_USART_EnableIT_TXFE(priv->USART);
//LL_USART_EnableIT_RXFF(priv->USART);
//LL_USART_EnableIT_LBD(priv->USART);
//LL_USART_EnableIT_ERROR(priv->USART);
//LL_USART_EnableIT_CTS(priv->USART);
//LL_USART_EnableIT_WKUP(priv->USART);
//LL_USART_EnableIT_TXFT(priv->USART);
//LL_USART_EnableIT_TCBGT(priv->USART);
//LL_USART_EnableIT_RXFT(priv->USART);

//LL_USART_DisableIT_IDLE(priv->USART);
//LL_USART_DisableIT_RXNE_RXFNE(priv->USART);
//LL_USART_DisableIT_TC(priv->USART);
//LL_USART_DisableIT_TXE_TXFNF(priv->USART);
//LL_USART_DisableIT_PE(priv->USART);
//LL_USART_DisableIT_CM(priv->USART);
//LL_USART_DisableIT_RTO(priv->USART);
//LL_USART_DisableIT_EOB(priv->USART);
//LL_USART_DisableIT_TXFE(priv->USART);
//LL_USART_DisableIT_RXFF(priv->USART);
//LL_USART_DisableIT_LBD(priv->USART);
//LL_USART_DisableIT_ERROR(priv->USART);
//LL_USART_DisableIT_CTS(priv->USART);
//LL_USART_DisableIT_WKUP(priv->USART);
//LL_USART_DisableIT_TXFT(priv->USART);
//LL_USART_DisableIT_TCBGT(priv->USART);
//LL_USART_DisableIT_RXFT(priv->USART);

static void stm32h745_up_rxint(struct uart_dev_s *dev, bool enable)
{
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  irqstate_t flags;
  flags = irqsave();

  priv->rxint_enable = enable;

  if (enable)
  {
    LL_USART_EnableIT_RXNE_RXFNE(priv->USART);
    LL_USART_EnableIT_ERROR(priv->USART);
  }
  else
  {
    LL_USART_DisableIT_RXNE_RXFNE(priv->USART);
    LL_USART_DisableIT_ERROR(priv->USART);
  }

  irqrestore(flags);
}

/****************************************************************************
 * Private Functions - stm32h745_up_rxavailable
 ****************************************************************************/
static bool stm32h745_up_rxavailable(struct uart_dev_s *dev)
{
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  return (bool)LL_USART_IsActiveFlag_RXNE_RXFNE(priv->USART);
}

/****************************************************************************
 * Private Functions - stm32h745_up_send
 ****************************************************************************/
static void stm32h745_up_send(struct uart_dev_s *dev, int ch)
{
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  LL_USART_TransmitData8(priv->USART, (uint8_t)ch);
}

/****************************************************************************
 * Private Functions - stm32h745_up_txint
 ****************************************************************************/
static void stm32h745_up_txint(struct uart_dev_s *dev, bool enable)
{
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  irqstate_t flags;
  flags = irqsave();

  priv->txint_enable = enable;

  if (enable)
  {
    LL_USART_EnableIT_TXE_TXFNF(priv->USART);
    LL_USART_EnableIT_ERROR(priv->USART);
  }
  else
  {
    LL_USART_DisableIT_TXE_TXFNF(priv->USART);
    LL_USART_DisableIT_ERROR(priv->USART);
  }

  irqrestore(flags);
}

/****************************************************************************
 * Private Functions - stm32h745_up_txready
 ****************************************************************************/
static bool stm32h745_up_txready(struct uart_dev_s *dev)
{
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  return LL_USART_IsActiveFlag_TXE_TXFNF(priv->USART);
}


/****************************************************************************
 * Private Functions - stm32h745_up_txempty
 ****************************************************************************/
static bool stm32h745_up_txempty(struct uart_dev_s *dev)
{
  struct stm32h745_up_dev_s *priv = (struct stm32h745_up_dev_s *)dev->priv;

  return LL_USART_IsActiveFlag_TXE_TXFNF(priv->USART);
}







