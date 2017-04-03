/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/s5j/s5j_serial.h
 *
 *   Copyright (C) 2009-2010, 2014 Gregory Nutt. All rights reserved.
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

#ifndef __ARCH_ARM_SRC_S5J_S5J_SERIAL_H
#define __ARCH_ARM_SRC_S5J_S5J_SERIAL_H

#include <arch/chip/chip_types.h>

/* Register Map */
#define UART_LCON       0x00
#define UART_CON        0x04
#define UART_FCON       0x08
#define UART_MCON       0x0C
#define UART_TRSTAT     0x10
#define UART_ERSTAT     0x14
#define UART_FSTAT      0x18
#define UART_MSTAT      0x1C
#define UART_TXH        0x20
#define UART_RXH        0x24
#define UART_BRDIV      0x28
#define UART_FRACVAL    0x2C
#define UART_INTP       0x30
#define UART_INTSP      0x34
#define UART_INTM       0x38

#define UART_INTP_MASK		0x0F
#define UART_FSTAT_RX_MASK	0x1FF
#define UART_FSTAT_TX_MASK	(0x1FF << 16)
#define UART_RX_MASK		0xFF

/* Enumultion & Structure */
typedef enum {
	WL_5BIT = 0,		/**< 5-bit */
	WL_6BIT,		/**< 6-bit */
	WL_7BIT,		/**< 7-bit */
	WL_8BIT,		/**< 8-bit */
} UART_WORD_LENGTH;

typedef enum {
	SB_1 = 0,		/**< One stop bit per frame */
	SB_2,			/**< Two stop bits per frame */
} UART_STOP_BIT;

typedef enum {
	PM_NO = 0,		/**< No Parity */
	PM_ODD = 4,		/**< Odd Parity */
	PM_EVEN,		/**< Even Parity */
	PM_1,			/**< Parity forced, checked as 1 */
	PM_0,			/**< Parity forced, checked as 0 */
} UART_PARITY_MODE;

typedef enum {
	BL_1 = 0,		/**<  Burst Length 1 (Single) */
	BL_4,			/**<  Burst Length 4 */
	BL_8,			/**<  Burst Length 8 */
	BL_16,			/**<  Burst Length 16 */
	BL_32,			/**<  Burst Length 32 */
	BL_64,			/**<  Burst Length 64 */
	BL_128,			/**<  Burst Length 128 */
	BL_256,			/**<  Burst Length 256 */
} UART_BURST_LENGTH;

typedef enum {
	TL_1 = 0,		/**<  Trigger Level 1 - UART0(TX:  0B,RX: 32B),UART1(TX: 0B,RX: 8B),UART2/3(TX: 0B,RX: 2B) */
	TL_2,			/**<  Trigger Level 2 - UART0(TX: 32B,RX: 64B),UART1(TX: 8B,RX:16B),UART2/3(TX: 2B,RX: 4B) */
	TL_3,			/**<  Trigger Level 3 - UART0(TX: 64B,RX: 96B),UART1(TX:16B,RX:24B),UART2/3(TX: 4B,RX: 6B) */
	TL_4,			/**<  Trigger Level 4 - UART0(TX: 96B,RX:128B),UART1(TX:24B,RX:32B),UART2/3(TX: 6B,RX: 8B) */
	TL_5,			/**<  Trigger Level 5 - UART0(TX:128B,RX:160B),UART1(TX:32B,RX:40B),UART2/3(TX: 8B,RX:10B) */
	TL_6,			/**<  Trigger Level 6 - UART0(TX:160B,RX:192B),UART1(TX:40B,RX:48B),UART2/3(TX:10B,RX:12B) */
	TL_7,			/**<  Trigger Level 7 - UART0(TX:192B,RX:224B),UART1(TX:48B,RX:56B),UART2/3(TX:12B,RX:14B) */
	TL_8,			/**<  Trigger Level 8 - UART0(TX:224B,RX:256B),UART1(TX:56B,RX:64B),UART2/3(TX:14B,RX:16B) */
} UART_TRIGGER_LEVEL;

typedef enum {
	RTL_1 = 0,		/**<  RTS Trigger Level 1 - UART0(RX:255B),UART1(RX:63B),UART2(RX:15B) */
	RTL_2,			/**<  RTS Trigger Level 2 - UART0(RX:224B),UART1(RX:56B),UART2(RX:14B) */
	RTL_3,			/**<  RTS Trigger Level 3 - UART0(RX:192B),UART1(RX:48B),UART2(RX:12B) */
	RTL_4,			/**<  RTS Trigger Level 4 - UART0(RX:160B),UART1(RX:40B),UART2(RX:10B) */
	RTL_5,			/**<  RTS Trigger Level 5 - UART0(RX:128B),UART1(RX:32B),UART2(RX: 8B) */
	RTL_6,			/**<  RTS Trigger Level 6 - UART0(RX: 96B),UART1(RX:24B),UART2(RX: 6B) */
	RTL_7,			/**<  RTS Trigger Level 7 - UART0(RX: 64B),UART1(RX:16B),UART2(RX: 4B) */
	RTL_8,			/**<  RTS Trigger Level 8 - UART0(RX: 32B),UART1(RX: 8B),UART2(RX: 2B) */
} UART_RTS_TRIGGER_LEVEL;

typedef enum {
	DISABLE_MODE = 0,		/**<  Disabled */
	POLL_MODE,			/**<  Polling mode */
	INT_MODE,			/**<  Interrupt mode */
	DMA_MODE,			/**<  DMA mode */
} UART_MODE;

typedef enum {
	RX_BUF_FILL = (1 << 0),		/**<  Rx buffer register fill      */
	TX_BUF_EMPTY = (1 << 1),	/**<  Tx buffer register empty      */
	TX_EMPTY = (1 << 2),		/**<  Tx (transmitter buffer & shift register) empty      */
} UART_TXRX_STATUS;

typedef enum {
	ERR_OVERRUN = (1 << 0),		/**<  Overrun Error     */
	ERR_PARITY = (1 << 1),		/**<  Parity Error     */
	ERR_FRAME = (1 << 2),		/**<  Frame Error      */
	ERR_BREAK = (1 << 3),		/**<  Break Signal Detection      */
} UART_ERROR_STATUS;

typedef enum {
	CTS_ACTIVATE = (1 << 0),	/**<  CTS Activate     */
	CTS_CHANGE = (1 << 4),		/**<  Delta CTS     */
} UART_MODEM_STATUS;

typedef enum {
	NONE_INT = 0,
	RX_INT = (1 << 0),		/**<  Receive Interrupt     */
	ERROR_INT = (1 << 1),		/**<  Error Interrupt     */
	TX_INT = (1 << 2),		/**<  Transmit Interrupt     */
	MODEM_INT = (1 << 3),		/**<  Modem Interrupt     */
	ALL_INT = 0xF,			/**<  All Interrupts: Receive, Transmit, Error, Modem     */
} UART_INTERRUPT;

typedef enum {
	DMA_FSM_IDLE,
	DMA_FSM_BURST_REQ,
	DMA_FSM_BURST_ACK,
	DMA_FSM_BURST_NEXT,
	DMA_FSM_SINGLE_REQ,
	DMA_FSM_SINGLE_ACK,
	DMA_FSM_SINGLE_NEXT,
	DMA_FSM_LAST_BURST_REQ,
	DMA_FSM_LAST_BURST_ACK,
	DMA_FSM_LAST_SINGLE_REQ,
} UART_DMA_FSM;

typedef enum {
	UARTDBG,
	UART0,
	UART1,
	UART2,
	UART3,
	UART_MAX_CHANNEL,
} UART_CHANNEL;

typedef enum {
	UART_CLEAN_DCACHE,
	UART_INVALIDATE_DCACHE,
} UART_DCACHE_CTRL;

typedef struct {
	uint32_t uBase;
	uint32_t nBaudrate;
	uint32_t nClock;
} UART_CONTEXT;

typedef enum uart_baudrate {
	BAUD_9600 = 9600,
	BAUD_14400 = 14400,
	BAUD_38400 = 38400,
	BAUD_57600 = 57600,
	BAUD_115200 = 115200,
	BAUD_230400 = 230400,
} UART_BAUDRATE;


#endif	/* __ARCH_ARM_SRC_S5J_S5J_SERIAL_H */
