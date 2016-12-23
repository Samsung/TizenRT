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
 * arch/arm/src/s5j/s5j_spidrv.h
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
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

#ifndef __ARCH_ARM_SRC_S5J_S5J_SPIDRV_H
#define __ARCH_ARM_SRC_S5J_S5J_SPIDRV_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <arch/chip/chip_types.h>
#include <arch/chip/irq.h>
#include <chip.h>
#include <chip/s5j_gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define SPI_CLOCK_TEST    0
#define SPI_TRANS_SIZE    8
#define SPI_BUFFER_SIZE   32
#define SPI_NO_ERROR      0
#define SPI_ERROR         (-1)

#define TEMP_TEST

typedef enum {
	SPI_LOOPBACK_MASTER_CH0_CH1 = 1,
	SPI_LOOPBACK_SLAVE_CH0_CH1,
	SPI_LOOPBACK_MASTER_CH2_CH3,
	SPI_LOOPBACK_SLAVE_CH2_CH3,
} eSPI_IOCTL_CMD;

typedef struct _SPI_SFR {
	u32 CH_CFG;				/*  0x00 spi configuration register */
	u32 CLK_CFG;			/*  0x04 deleted */
	u32 MODE_CFG;			/*  0x08 spi fifo control register */
	u32 CS_REG;				/*  0x0C slave selection signal */
	u32 SPI_INT_EN;			/*  0x10 spi interrupt enable register */
	u32 SPI_STATUS;			/*  0x14 spi status register */
	u32 SPI_TX_DATA;		/*  0x18 spi tx data register */
	u32 SPI_RX_DATA;		/*  0x1C spi rx data register */
	u32 PACKET_CNT_REG;		/*  0x20 count how many data master gets. */
	u32 PENDING_CLR_REG;	/*  0x24 pending clear register */
	u32 SWAP_CFG;			/*  0x28 swap config register */
	u32 FB_CLK_SEL;			/*  0x2C feedback clock config register */
} SPI_SFR;

typedef enum {
	SPI_PORT0 = 0,
	SPI_PORT1,
	SPI_PORT2,
	SPI_PORT3,
	SPI_PORT_MAX,
} SPI_PORT;

typedef enum {
	SPI_MASTER = 0,
	SPI_SLAVE,
} SPI_OpMode;

typedef enum {
	SPI_ACTIVE_HIGH = 0,
	SPI_ACTIVE_LOW
} SPI_CPOL;

typedef enum {
	SPI_FORMAT_A = 0,
	SPI_FORMAT_B
} SPI_CPHA;

typedef enum {
	SPI_PCLK = 0,
	SPI_EXT_CLK
} SPI_clock_selection;

typedef enum {
	SPI_DMA_SINGLE = 0,
	SPI_DMA_4_BURST,
} SPI_DMA_type;

typedef enum {
	SPI_INT_TRAILING    = (1 << 6),
	SPI_INT_RX_OVERRUN  = (1 << 5),
	SPI_INT_RX_UNDERRUN = (1 << 4),
	SPI_INT_TX_OVERRUN  = (1 << 3),
	SPI_INT_TX_UNDERRUN = (1 << 2),
	SPI_INT_RX_FIFORDY  = (1 << 1),
	SPI_INT_TX_FIFORDY  = (1 << 0),
	SPI_INT_ALL         = 0x7F,
} SPI_IntrModeBit_et;

typedef enum {
	SPI_CLR_PND_TX_UNDERRUN = (1 << 4),
	SPI_CLR_PND_TX_OVERRUN  = (1 << 3),
	SPI_CLR_PND_RX_UNDERRUN = (1 << 2),
	SPI_CLR_PND_RX_OVERRUN  = (1 << 1),
	SPI_CLR_PND_TRAILING    = (1 << 0),
	SPI_CLR_PND_ALL         = 0x1F,
} SPI_IntClrModeBit_et;

typedef enum {
	SPI_STATUS_TX_DONE       = (1 << 25),
	SPI_STATUS_TRAILING_BYTE = (1 << 24),
	SPI_STATUS_RX_OVERRUN    = (1 << 5),
	SPI_STATUS_RX_UNDERRUN   = (1 << 4),
	SPI_STATUS_TX_OVERRUN    = (1 << 3),
	SPI_STATUS_TX_UNDERRUN   = (1 << 2),
	SPI_STATUS_RX_FIFORDY    = (1 << 1),
	SPI_STATUS_TX_FIFORDY    = (1 << 0),
} SPI_StatusBit_et;

typedef enum {
	SPI_BYTE  = 0,
	SPI_HWORD = 1,
	SPI_WORD  = 2,
} SPI_transfer_data_type;

typedef enum {
	SPI_PHASE_DELAY0   = 0x00,
	SPI_PHASE_DELAY90  = 0x01,
	SPI_PHASE_DELAY180 = 0x02,
	SPI_PHASE_DELAY270 = 0x03,
	SPI_PHASE_DELAY    = 0x04,

} SPI_feedbackClock;

typedef enum {
	SPI_INTERRUPT_MODE = 0x0,
	SPI_DMA_MODE       = 0x1,
	SPI_POLLING_MODE   = 0x4,
} SPI_transfer_mode;

typedef enum {
	SPI_CLKSRC_XXTI    = 0,
	SPI_CLKSRC_BUS_PLL = 1,
} SPI_CLKSRC_et;

typedef enum {
	SPI_NSSOUT_ACTIVE    = 0,
	SPI_NSSOUT_INACTIVE  = 1,
	SPI_NSSOUT_USER_MODE = 2,
} SPI_NSSOUT_et;

typedef enum {
	SPI_NO_SWAP    = 0,
	SPI_BIT_SWAP   = (1 << 1),	/*  Bit swap. */
	SPI_BYTE_SWAP  = (1 << 2),	/*  Byte swap. */
	SPI_HWORD_SWAP = (1 << 3),	/*  Half Word swap. */
} SPI_SwapMode;

typedef struct {
	u32 m_StartMemAddr;
	u32 m_TotalSize;
	u32 m_CurMemAddr;		/*  I'll use the Non Cacheable Memory Area as Ring-Buffer. */
} SPI_NCBufCtrlInfo_st;

typedef struct {
	u8 *m_pBufPtr;			/*  Start of buffer pointer */
	u8 *m_pCurPtr;			/*  Current buffer pointer */
	u32 m_BufSize;			/*  Buffer size */
	u32 m_ReqDataSize;		/*  Requested data size */
	u32 m_RemainDataSize;	/*  not treated, Remain data size */
} SPI_BufAllocInfo_st;

typedef void (*SPI_IntHandler_t)(void);
typedef void (*SPI_Callback_t)(u32 Value2);

typedef struct {
	SPI_PORT m_SPIPortIdx;	/*  SPI0, SPI1, SPI2 */
	SPI_SFR *m_SPISFRBase;	/*  SFR Base Register */

	/* CH_CFG */
	bool m_EnHighSpeed;		/*  Enable High Speed in Salve Tx */
	SPI_OpMode m_OpMode;	/*  Master or Slave Mode */
	SPI_CPOL m_CPOL;		/*  CPOL */
	SPI_CPHA m_CPHA;		/*  CPHA */
	bool m_EnRxChannel;		/*  Enable Tx Channel */
	bool m_EnTxChannel;		/*  Enalbe Rx Channel */

	bool m_EnClk;			/*  Clock Enable/Disable */
	bool m_ClkDivPre;		/*  1~256 */
	SPI_CLKSRC_et m_ClkSrc;	/*  XTI, USBXTI */
	u32 m_ClkDiv;			/*  1 ~ 16 */
	u32 m_OpClock;			/*  Opeation clock */
	u32 m_TargetSPICLK;		/*  Target Clock */

	/* MODE_CFG */
	SPI_transfer_data_type m_ChWidth;	/*  BYTE, HWORD, WORD */
	u32 m_TraillingCnt;					/*  Trailing Bytes threshold value in Rx FIFO */
	SPI_transfer_data_type m_BusWidth;	/*  BYTE, HWORD, WORD */
	u32 m_RxReadyLevel;					/*  Rx FIFO trigger level */
	u32 m_TxReadyLevel;					/*  Tx FIFO trigger level */
	bool m_Self_loopbackMode;			/* self loopback test mode */
	SPI_transfer_mode m_RxTransferMode;	/*  Interrupt mode, DMA mode */
	SPI_transfer_mode m_TxTransferMode;	/*  Interrupt mode, DMA mode */
	u32 m_MaxFIFOSize;					/*  Port0 : 256 bytes, Port1/2 : 64 bytes */
	u32 m_FifoLevelGrid;				/*  Port0 : 4 bytes, Port1/2 : 1 byte */

	u32 m_TxTransferSize;
	u32 m_RxTransferSize;
	SPI_DMA_type m_DMAType;	/*  Single or 4Burst */

	/* CS_REG */
	u32 m_NCSTimeCnt;		/*  NCS Time Count */
	bool m_EnAutoCS;		/*  Manual or Auto Chip Selection */
	SPI_NSSOUT_et m_NSSOut;	/*  Active or Inactive in Manual mode Chip Selection */

	/* INT_EN */
	bool m_EnTrailingInt;		/*  Enable Trailing Interrupt */
	bool m_EnRxOverrunInt;		/*  Enable Rx Overrun Interrupt */
	bool m_EnRxUnderrunInt;		/*  Enable Rx Underrun Interrupt */
	bool m_EnTxOverrunInt;		/*  Enable Tx Overrun Interrupt */
	bool m_EnTxUnderrunInt;		/*  Enable Tx Underrun Interrupt */
	bool m_EnRxFifoReadyInt;	/*  Enable Rx FIFO Ready Interrupt */
	bool m_EnTxFifoReadyInt;	/*  Enable Tx FIFO Ready Interrupt */

	/* PACKET_CNT_REG */
	bool m_EnPacketCnt;
	u32 m_PacketCntValue;

	/* SWAP_CFG */
	SPI_SwapMode m_EnRxSwap;
	SPI_SwapMode m_EnTxSwap;

	/* FB_CLK_SEL */
	SPI_feedbackClock m_FBClkSel;	/*  Select Feedback Clock */

	/*  Drive Strength */
	u32 m_DrvStrength;				/*  Drive Strength */
	bool m_EnSWReset;				/*  Reset in InitializeREG() */

	bool m_EnPrintInISR;			/*  interrupt massage */
	u32 m_SPIIntNum;				/*  store interrupt Number */
	SPI_IntHandler_t m_fnISR_SPI;	/* SPI interrupt handler */

	SPI_BufAllocInfo_st m_RxBufNode;
	SPI_BufAllocInfo_st m_TxBufNode;

	/*  ISR */
	SPI_Callback_t m_RxDMACompleteCallback;
	u32 m_RxDMACompleteCallback_Value;

	SPI_Callback_t m_TxDMACompleteCallback;
	u32 m_TxDMACompleteCallback_Value;

	SPI_Callback_t m_TrailingINT_Callback;
	u32 m_TrailingINT_Value;

	SPI_Callback_t m_RxOverrunINT_Callback;
	u32 m_RxOverrunINT_Value;

	SPI_Callback_t m_RxUnderrunINT_Callback;
	u32 m_RxUnderrunINT_Value;

	SPI_Callback_t m_TxOverrunINT_Callback;
	u32 m_TxOverrunINT_Value;

	SPI_Callback_t m_TxUnderrunINT_Callback;
	u32 m_TxUnderrunINT_Value;

	SPI_Callback_t m_RxFIFORdyINT_Callback;
	u32 m_RxFIFORdyINT_Value;

	SPI_Callback_t m_TxFIFORdyINT_Callback;
	u32 m_TxFIFORdyINT_Value;

	SPI_Callback_t m_RxIntCompleteCallback;
	u32 m_RxIntCompleteCallback_Value;

	SPI_Callback_t m_TxIntCompleteCallback;
	u32 m_TxIntCompleteCallback_Value;
} SPI_PortInfo_st;

typedef struct {
	bool m_AutoTest;
	bool m_TargetPort_A;
	bool m_TargetPort_B;
	bool m_TargetPort_C;
	bool m_TargetPort_D;
	SPI_PortInfo_st m_SPIPortDefaultInfo[SPI_PORT_MAX];
	bool m_NonBufferClear;
} SPI_CtrlInfo_st;

/****************************************************************************
 * Public Functions Prototypes
 ****************************************************************************/

void spi_init(SPI_CtrlInfo_st *pSPICtrlInfo);
void spi_deinit(SPI_CtrlInfo_st *pSPICtrlInfo);

void spi_wr_tx_data08(SPI_PortInfo_st *pPortInfo, u8 uTxData);
void spi_wr_tx_data16(SPI_PortInfo_st *pPortInfo, u16 uTxData);
void spi_wr_tx_data32(SPI_PortInfo_st *pPortInfo, u32 uTxData);
u8 spi_rd_rx_data08(SPI_PortInfo_st *pPortInfo);
u16 spi_rd_rx_data16(SPI_PortInfo_st *pPortInfo);
u32 spi_rd_rx_data32(SPI_PortInfo_st *pPortInfo);

void spi_set_isr_func(SPI_PortInfo_st *pPortInfo, u32 uIntrMode, void (*fnIsrFunc)(u32), u32 uIsrVar);

/****************************************************************************
 * Private Functions Prototypes
 ****************************************************************************/
static void spi_initialize(SPI_PortInfo_st *pPortInfo);
static void spi_chip_select(SPI_PortInfo_st *pPortInfo, SPI_NSSOUT_et NSSOut);
static void spi_enable_channel(SPI_PortInfo_st *pPortInfo);
static u8 spi_is_tx_done(SPI_PortInfo_st *pPortInfo);
static void spi_sw_reset(SPI_PortInfo_st *pPortInfo);
static void spi_enable_packet_count(SPI_PortInfo_st *pPortInfo, bool En, u32 CntVal);
static u8 spi_is_tx_fifo_rdy(SPI_PortInfo_st *pPortInfo);
static u16 spi_is_tx_fifo_lvl(SPI_PortInfo_st *pPortInfo);
static u16 spi_is_rx_fifo_lvl(SPI_PortInfo_st *pPortInfo);

/****************************************************************************
 * Private Functions Prototypes; Used for only Test
 ****************************************************************************/

static void spi_create_ncbuf_ctrl(u32 StartAddr, u32 TotalSize);
static u8 *spi_get_ncbuf(SPI_BufAllocInfo_st *pBufAllocInfo, u32 BufSize);
static int spi_init_ncbuf_zero(SPI_BufAllocInfo_st *pBufAllocInfo);
static void spi_display_ncbuf_hex(SPI_BufAllocInfo_st *pBufAllocInfo, SPI_transfer_data_type EnDisplay);
static int spi_init_ncbuf_fix(SPI_BufAllocInfo_st *pBufAllocInfo, u32 BufSize);
static int spi_init_buf_zero(void *pBuf, u32 BufSize);
static s32 spi_polling_master_tx(SPI_PortInfo_st *pPortInfo);
static s32 spi_polling_slave_rx(SPI_PortInfo_st *pPortInfo);
static s32 spi_polling_master_rx(SPI_PortInfo_st *pPortInfo);
static s32 spi_polling_slave_tx(SPI_PortInfo_st *pPortInfo);
static s32 spi_polling_master_txrx(SPI_PortInfo_st *pPortInfo);
static int spi_compare_data(u8 *data1, u8 *data2, u32 bytes);
static s32 spi_check_loopback_master_and_channel(SPI_PortInfo_st *pPortAInfo, SPI_PortInfo_st *pPortBInfo, SPI_PortInfo_st **pMasterPort, SPI_PortInfo_st **pSlavePort, SPI_PortInfo_st **pTxPort, SPI_PortInfo_st **pRxPort);
static s32 spi_loopback_polling_masterTxRx_slaveTxRx_test(SPI_PortInfo_st *pMaster, SPI_PortInfo_st *pSlave);
static s32 spi_loopback_polling_masterTx_slaveRx_test(SPI_PortInfo_st *pMaster, SPI_PortInfo_st *pSlave);
static s32 spi_loopback_polling_masterRx_slaveTx_test(SPI_PortInfo_st *pMaster, SPI_PortInfo_st *pSlave);
static s32 spi_loopback_portA_portB_polling_test(SPI_PortInfo_st *pPortAInfo, SPI_PortInfo_st *pPortBInfo);
static s32 spi_loopback_polling_test(SPI_CtrlInfo_st *pCtrlInfo);
static s32 spi_loopback_test(void);

#ifdef __cplusplus
}
#endif
#endif							/* __ARCH_ARM_SRC_S5J_S5J_SPIDRV_H */
