/** mbed Microcontroller Library
  ******************************************************************************
  * @file    spi_api.c
  * @author
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for SPI.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#include "objects.h"
#include "spi_api.h"
#include "spi_ex_api.h"
#include "PinNames.h"

#define USE_DMA_WRITE_MASTER_READ	1

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @defgroup MBED_SPI
 *  @brief    MBED_SPI driver modules.
 *  @{
 */

/** @defgroup MBED_SPI_Exported_Types MBED_SPI Exported Types
  * @{
  */
typedef struct {
	GDMA_InitTypeDef SSITxGdmaInitStruct;
	GDMA_InitTypeDef SSIRxGdmaInitStruct;
	IRQn_Type   IrqNum;
	SPI_TypeDef *spi_dev;

	void (*RxCompCallback)(void *Para);
	void *RxCompCbPara;
	void (*TxCompCallback)(void *Para);
	void *TxCompCbPara;
	void (*TxIdleCallback)(void *Para);
	void *TxIdleCbPara;

	void *RxData;
	void *TxData;
	u32  RxLength;
	u32  TxLength;

	u32   Index;
	u32   Role;

	/* mbed var */
	u32 dma_en;
} HAL_SSI_ADAPTOR, *PHAL_SSI_ADAPTOR;

HAL_SSI_ADAPTOR ssi_adapter_g[2];

/**
  * @}
  */

/** @defgroup MBED_SPI_Exported_Functions MBED_SPI Exported Functions
  * @{
  */
void spi_flush_rx_fifo(spi_t *obj);

static void spi_tx_done_callback(VOID *spi_obj)
{
	spi_t *obj = (spi_t *)spi_obj;
	spi_irq_handler handler;

	if (obj->state & SPI_STATE_TX_BUSY) {
		obj->state &= ~SPI_STATE_TX_BUSY;
		if (obj->irq_handler) {
			handler = (spi_irq_handler)obj->irq_handler;
			handler(obj->irq_id, SpiTxIrq);
		}
	}
}

static void spi_rx_done_callback(VOID *spi_obj)
{
	spi_t *obj = (spi_t *)spi_obj;
	spi_irq_handler handler;

	obj->state &= ~SPI_STATE_RX_BUSY;
	if (obj->irq_handler) {
		handler = (spi_irq_handler)obj->irq_handler;
		handler(obj->irq_id, SpiRxIrq);
	}
}

// Bus Idle: Real TX done, TX FIFO empty and bus shift all data out already
void spi_bus_tx_done_callback(VOID *spi_obj)
{
	spi_t *obj = (spi_t *)spi_obj;
	spi_irq_handler handler;

	if (obj->bus_tx_done_handler) {
		handler = (spi_irq_handler)obj->bus_tx_done_handler;
		handler(obj->bus_tx_done_irq_id, SpiTxIrq);
	}
}

/**
  * @brief  Register SPI bus Tx done interrupt handler if needed.
  * @param  obj: SPI object defined in application software.
  * @param  handler: Interrupt bus Tx done callback function.
  * @param  id: Interrupt bus Tx done callback parameter.
  * @retval none
  */
void spi_bus_tx_done_irq_hook(spi_t *obj, spi_irq_handler handler, uint32_t id)
{
	obj->bus_tx_done_handler = (u32)handler;
	obj->bus_tx_done_irq_id = (u32)id;
}

static u32 ssi_interrupt(void *Adaptor)
{
	PHAL_SSI_ADAPTOR ssi_adapter = (PHAL_SSI_ADAPTOR) Adaptor;
	u32 InterruptStatus = SSI_GetIsr(ssi_adapter->spi_dev);
	u32 DataFrameSize = SSI_GetDataFrameSize(ssi_adapter->spi_dev);

	SSI_SetIsrClean(ssi_adapter->spi_dev, InterruptStatus);

	if (InterruptStatus & (SPI_BIT_TXOIS | SPI_BIT_RXUIS | SPI_BIT_RXOIS | SPI_BIT_MSTIS_FAEIS)) {
		DBG_PRINTF(MODULE_SPI, LEVEL_INFO, "[INT] Tx/Rx Warning %x \n", InterruptStatus);
	}

	if ((InterruptStatus & SPI_BIT_RXFIS)) {
		u32 TransLen = 0;

		TransLen = SSI_ReceiveData(ssi_adapter->spi_dev, ssi_adapter->RxData, ssi_adapter->RxLength);
		ssi_adapter->RxLength -= TransLen;
		if (DataFrameSize > 8) {
			// 16~9 bits mode
			ssi_adapter->RxData = (void *)(((u16 *)ssi_adapter->RxData) + TransLen);
		} else {
			// 8~4 bits mode
			ssi_adapter->RxData = (void *)(((u8 *)ssi_adapter->RxData) + TransLen);
		}

		if (ssi_adapter->RxLength == 0) {
			SSI_INTConfig(ssi_adapter->spi_dev, (SPI_BIT_RXFIM | SPI_BIT_RXOIM | SPI_BIT_RXUIM), DISABLE);
			if (ssi_adapter->RxCompCallback != NULL) {
				ssi_adapter->RxCompCallback(ssi_adapter->RxCompCbPara);
			}
		}
	}

	if (InterruptStatus & SPI_BIT_TXEIS) {
		u32 TransLen = 0;
		volatile u32 bus_busy;
		u32 i;

		/* all data complete */
		if (ssi_adapter->TxLength == 0) {
			SSI_INTConfig(ssi_adapter->spi_dev, (SPI_BIT_TXOIM | SPI_BIT_TXEIM), DISABLE);
			for (i = 0; i < 1000000; i++) {
				bus_busy = SSI_Busy(ssi_adapter->spi_dev);
				if (!bus_busy) {
					break;  // break the for loop
				}
			}

			// If it's not a dummy TX for master read SPI, then call the TX_done callback
			if (ssi_adapter->TxData != NULL) {
				if (ssi_adapter->TxIdleCallback != NULL) {
					ssi_adapter->TxIdleCallback(ssi_adapter->TxIdleCbPara);
				}
			}

			return 0;
		}

		TransLen = SSI_SendData(ssi_adapter->spi_dev, ssi_adapter->TxData,
								ssi_adapter->TxLength, ssi_adapter->Role);

		ssi_adapter->TxLength -= TransLen;
		if (ssi_adapter->TxData != NULL) {
			if (DataFrameSize > 8) {
				// 16~9 bits mode
				ssi_adapter->TxData = (void *)(((u16 *)ssi_adapter->TxData) + TransLen);
			} else {
				// 8~4 bits mode
				ssi_adapter->TxData = (void *)(((u8 *)ssi_adapter->TxData) + TransLen);
			}
		}
		/* all data write into fifo */
		if (ssi_adapter->TxLength == 0) {
			SSI_INTConfig(ssi_adapter->spi_dev, (SPI_BIT_TXOIM), DISABLE);
			// If it's not a dummy TX for master read SPI, then call the TX_done callback
			if (ssi_adapter->TxData != NULL) {
				if (ssi_adapter->TxCompCallback != NULL) {
					ssi_adapter->TxCompCallback(ssi_adapter->TxCompCbPara);
				}
			}
		}
	}

	return 0;
}

static u32 ssi_int_read(void *Adapter, void *RxData, u32 Length)
{
	PHAL_SSI_ADAPTOR ssi_adapter = (PHAL_SSI_ADAPTOR) Adapter;
	u32 DataFrameSize = SSI_GetDataFrameSize(ssi_adapter->spi_dev);

	assert_param(Length != 0);

	/*  As a Slave mode, if the peer(Master) side is power off, the BUSY flag is always on */
	if (SSI_Busy(ssi_adapter->spi_dev)) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "ssi_int_read: SSI is busy\n");
		return _FALSE;
	}

	if (DataFrameSize > 8) {
		/*  16~9 bits mode */
		ssi_adapter->RxLength = Length >> 1; // 2 bytes(16 bit) every transfer
	} else {
		/*  8~4 bits mode */
		ssi_adapter->RxLength = Length; // 1 byte(8 bit) every transfer
	}

	ssi_adapter->RxData = RxData;

	SSI_INTConfig(ssi_adapter->spi_dev, (SPI_BIT_RXFIM | SPI_BIT_RXOIM | SPI_BIT_RXUIM), ENABLE);

	return _TRUE;
}


static u32 ssi_int_write(void *Adapter, u8 *pTxData, u32 Length)
{
	PHAL_SSI_ADAPTOR ssi_adapter = (PHAL_SSI_ADAPTOR) Adapter;
	u32 DataFrameSize = SSI_GetDataFrameSize(ssi_adapter->spi_dev);

	assert_param(Length != 0);

	if (DataFrameSize > 8) {
		/*  16~9 bits mode */
		ssi_adapter->TxLength = Length >> 1; // 2 bytes(16 bit) every transfer
	} else {
		/*  8~4 bits mode */
		ssi_adapter->TxLength = Length; // 1 byte(8 bit) every transfer
	}

	ssi_adapter->TxData = (void *)pTxData;
	SSI_INTConfig(ssi_adapter->spi_dev, (SPI_BIT_TXOIM | SPI_BIT_TXEIM), ENABLE);

	return _TRUE;
}

/* GDMA IRQ Handler */
static u32 ssi_dma_tx_irq(void *Data)
{
	PHAL_SSI_ADAPTOR ssi_adapter = (PHAL_SSI_ADAPTOR) Data;
	PGDMA_InitTypeDef GDMA_InitStruct;

	GDMA_InitStruct = &ssi_adapter->SSITxGdmaInitStruct;

	/* Clear Pending ISR */
	GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
	GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);

	/*  Call user TX complete callback */
	if (NULL != ssi_adapter->TxCompCallback) {
		ssi_adapter->TxCompCallback(ssi_adapter->TxCompCbPara);
	}

	SSI_SetDmaEnable(ssi_adapter->spi_dev, DISABLE, SPI_BIT_TDMAE);

	GDMA_ChnlFree(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

	ssi_adapter->dma_en &= ~SPI_DMA_TX_EN;

	return 0;
}

static u32 ssi_dma_rx_irq(void *Data)
{
	PHAL_SSI_ADAPTOR ssi_adapter = (PHAL_SSI_ADAPTOR) Data;
	u32 Length = ssi_adapter->RxLength;
	u32 *pRxData = ssi_adapter->RxData;
	PGDMA_InitTypeDef GDMA_InitStruct;

	GDMA_InitStruct = &ssi_adapter->SSIRxGdmaInitStruct;

	/* Clear Pending ISR */
	GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
	GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);

	DCache_Invalidate((u32) pRxData, Length);

	/* Set SSI DMA Disable */
	SSI_SetDmaEnable(ssi_adapter->spi_dev, DISABLE, SPI_BIT_RDMAE);

	/*  Call user RX complete callback */
	if (NULL != ssi_adapter->RxCompCallback) {
		ssi_adapter->RxCompCallback(ssi_adapter->RxCompCbPara);
	}

	GDMA_ChnlFree(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

	ssi_adapter->dma_en &= ~SPI_DMA_RX_EN;

	return 0;
}

static u32 ssi_dma_send(void *Adapter, u8 *pTxData, u32 Length)
{
	PHAL_SSI_ADAPTOR ssi_adapter = (PHAL_SSI_ADAPTOR) Adapter;
	u32 ret = _TRUE;

	assert_param(Length != 0);
	assert_param(pTxData != NULL);

	if (ssi_adapter->dma_en & SPI_DMA_TX_EN) {
		return _FALSE;
	}

	ssi_adapter->dma_en |= SPI_DMA_TX_EN;
	ssi_adapter->TxLength = Length;
	ssi_adapter->TxData = (void *)pTxData;

	SSI_SetDmaEnable(ssi_adapter->spi_dev, ENABLE, SPI_BIT_TDMAE);

	ret = SSI_TXGDMA_Init(ssi_adapter->Index, &ssi_adapter->SSITxGdmaInitStruct,
						  ssi_adapter, (IRQ_FUN) ssi_dma_tx_irq,
						  pTxData, Length);

	//NVIC_SetPriority(GDMA_GetIrqNum(0, ssi_adapter->SSITxGdmaInitStruct.GDMA_ChNum), 10);

	return ret;
}

static u32 ssi_dma_recv(void *Adapter, u8  *pRxData, u32 Length)
{
	PHAL_SSI_ADAPTOR ssi_adapter = (PHAL_SSI_ADAPTOR) Adapter;
	u32 ret = _TRUE;

	assert_param(Length != 0);
	assert_param(pRxData != NULL);

	if (ssi_adapter->dma_en & SPI_DMA_RX_EN) {
		return _FALSE;
	}

	ssi_adapter->dma_en |= SPI_DMA_RX_EN;
	ssi_adapter->RxLength = Length;
	ssi_adapter->RxData = (void *)pRxData;

	ret = SSI_RXGDMA_Init(ssi_adapter->Index, &ssi_adapter->SSIRxGdmaInitStruct,
						  ssi_adapter, (IRQ_FUN) ssi_dma_rx_irq,
						  pRxData, Length);

	//NVIC_SetPriority(GDMA_GetIrqNum(0, ssi_adapter->SSIRxGdmaInitStruct.GDMA_ChNum), 11);

	/* Set SSI DMA Enable */
	SSI_SetDmaEnable(ssi_adapter->spi_dev, ENABLE, SPI_BIT_RDMAE);

	return ret;
}

static u32 spi_stop_recv(spi_t *obj)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	PGDMA_InitTypeDef GDMA_InitStruct = &ssi_adapter->SSIRxGdmaInitStruct;
	u32 DMAStopAddr = 0;
	u32 ReceivedCnt;
	u8 DmaMode = 0;

	SSI_INTConfig(ssi_adapter->spi_dev, (SPI_BIT_RXFIM | SPI_BIT_RXOIM | SPI_BIT_RXUIM), DISABLE);

	if (ssi_adapter->dma_en & SPI_DMA_RX_EN) {
		DmaMode = 1;
		/* Set SSI DMA Disable */
		SSI_SetDmaEnable(ssi_adapter->spi_dev, DISABLE, SPI_BIT_RDMAE);

		/* Clear Pending ISR */
		GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
		GDMA_ChCleanAutoReload(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, CLEAN_RELOAD_SRC_DST);
		GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);
		DMAStopAddr = GDMA_GetDstAddr(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
		GDMA_ChnlFree(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

		ReceivedCnt = DMAStopAddr - (u32)(ssi_adapter->RxData);
		ssi_adapter->RxLength -= ReceivedCnt;
		ssi_adapter->RxData = (u8 *)(ssi_adapter->RxData) + ReceivedCnt;

		ssi_adapter->dma_en &= ~SPI_DMA_RX_EN;
	}

	if (ssi_adapter->RxLength > 0) {
		ReceivedCnt = SSI_ReceiveData(ssi_adapter->spi_dev, ssi_adapter->RxData, ssi_adapter->RxLength);
		if (DmaMode && (SSI_GetDataFrameSize(ssi_adapter->spi_dev) > 8)) {
			ReceivedCnt = ReceivedCnt * 2;
		}
		ssi_adapter->RxLength -= ReceivedCnt;
	}

	if (ssi_adapter->RxLength == 0) {
		spi_flush_rx_fifo(obj);
	}

	obj->state &= ~ SPI_STATE_RX_BUSY;

	return HAL_OK;
}

/**
  * @brief  Initialize the SPI device, including clock, function, interrupt and SPI registers.
  * @param  obj: SPI object defined in application software.
  * @param  mosi: MOSI PinName according to pinmux spec.
  * @param  miso: MISO PinName according to pinmux spec.
  * @param  sclk: SCLK PinName according to pinmux spec.
  * @param  ssel: CS PinName according to pinmux spec.
  * @retval none
  * @attention Remember to set obj->spi_index to MBED_SPI0 or MBED_SPI1 before calling spi_init.
  */
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	SSI_InitTypeDef SSI_InitStruct;

	if ((obj->spi_idx != MBED_SPI0) && (obj->spi_idx != MBED_SPI1)) {
		DBG_PRINTF(MODULE_SPI, LEVEL_ERROR, "spi_init: you should set spi_idx MBED_SPI0 or MBED_SPI1");
		assert_param(0);
	}

	ssi_adapter->Index = spi_idx;
	ssi_adapter->spi_dev = SPI_DEV_TABLE[ssi_adapter->Index].SPIx;
	obj->sclk = (u8)sclk;

	if (spi_idx == 1) {
		RCC_PeriphClockCmd(APBPeriph_SPI1, APBPeriph_SPI1_CLOCK, ENABLE);
		Pinmux_Config(mosi, PINMUX_FUNCTION_SPI);
		Pinmux_Config(miso, PINMUX_FUNCTION_SPI);
		Pinmux_Config(sclk, PINMUX_FUNCTION_SPI);
		Pinmux_Config(ssel, PINMUX_FUNCTION_SPI);
	} else {
		RCC_PeriphClockCmd(APBPeriph_SPI0, APBPeriph_SPI0_CLOCK, ENABLE);
		Pinmux_Config(mosi, PINMUX_FUNCTION_SPI);
		Pinmux_Config(miso, PINMUX_FUNCTION_SPI);
		Pinmux_Config(sclk, PINMUX_FUNCTION_SPI);
		Pinmux_Config(ssel, PINMUX_FUNCTION_SPI);
	}

	SSI_StructInit(&SSI_InitStruct);
	SSI_Init(ssi_adapter->spi_dev, &SSI_InitStruct);

	ssi_adapter->IrqNum = SPI_DEV_TABLE[ssi_adapter->Index].IrqNum;
	InterruptRegister((IRQ_FUN)ssi_interrupt, ssi_adapter->IrqNum, (u32)ssi_adapter, 5);
	InterruptEn(ssi_adapter->IrqNum, 5);

	ssi_adapter->TxCompCallback = spi_tx_done_callback;
	ssi_adapter->TxCompCbPara = (void *)obj;
	ssi_adapter->RxCompCallback = spi_rx_done_callback;
	ssi_adapter->RxCompCbPara = (void *)obj;
	ssi_adapter->TxIdleCallback = spi_bus_tx_done_callback;
	ssi_adapter->TxIdleCbPara = (void *)obj;
	obj->state = SPI_STATE_READY;

#ifdef CONFIG_GDMA_EN
	ssi_adapter->dma_en = 0;
#endif
}

/**
  * @brief  Deinitialize and Disable the SPI device.
  * @param  obj: SPI object defined in application software.
  * @retval none
  */
void spi_free(spi_t *obj)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];

	InterruptDis(ssi_adapter->IrqNum);
	InterruptUnRegister(ssi_adapter->IrqNum);

	SSI_INTConfig(ssi_adapter->spi_dev, (SPI_BIT_RXFIM | SPI_BIT_RXOIM | SPI_BIT_RXUIM), DISABLE);

	if (ssi_adapter->dma_en & SPI_DMA_RX_EN) {
		PGDMA_InitTypeDef GDMA_InitStruct = &ssi_adapter->SSIRxGdmaInitStruct;

		/* Set SSI DMA Disable */
		SSI_SetDmaEnable(ssi_adapter->spi_dev, DISABLE, SPI_BIT_RDMAE);

		/* Clear Pending ISR */
		GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
		GDMA_ChCleanAutoReload(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, CLEAN_RELOAD_SRC_DST);
		GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);
		GDMA_ChnlFree(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
	}

	if (ssi_adapter->dma_en & SPI_DMA_TX_EN) {
		PGDMA_InitTypeDef GDMA_InitStruct = &ssi_adapter->SSITxGdmaInitStruct;

		/* Set SSI DMA Disable */
		SSI_SetDmaEnable(ssi_adapter->spi_dev, DISABLE, SPI_BIT_TDMAE);

		/* Clear Pending ISR */
		GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
		GDMA_ChCleanAutoReload(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, CLEAN_RELOAD_SRC_DST);
		GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, DISABLE);
		GDMA_ChnlFree(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
	}

	obj->state = 0;

	SSI_Cmd(ssi_adapter->spi_dev, DISABLE);
}

/**
  * @brief  Set SPI format, including DFS, Phase, Polarity and Role.
  * @param  obj: SPI object defined in application software.
  * @param  bits: Data frame size, which can be 4~16.
  * @param  mode: This parameter can be one of the following values:
  *		@arg 0: [Polarity,Phase]=[0,0].
  *		@arg 1: [Polarity,Phase]=[0,1].
  *		@arg 2: [Polarity,Phase]=[1,0].
  *		@arg 3: [Polarity,Phase]=[1,1].
  * @param  slave: This parameter can be one of the following values:
  *		@arg 0: Configure SPI as the master.
  *		@arg 1: Configure SPI as the slave.
  * @retval none
  */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	u32   SclkPhase;
	u32   SclkPolarity;
	u32 DataFrameSize = (bits - 1);
	SSI_InitTypeDef SSI_InitStruct;

	/*
	* mode | POL PHA
	* -----+--------
	*   0  |  0   0
	*   1  |  0   1
	*   2  |  1   0
	*   3  |  1   1
	*
	* SCPOL_INACTIVE_IS_LOW  = 0,
	* SCPOL_INACTIVE_IS_HIGH = 1
	*
	* SCPH_TOGGLES_IN_MIDDLE = 0,
	* SCPH_TOGGLES_AT_START  = 1
	*/
	switch (mode) {
	case 0:
		SclkPolarity = SCPOL_INACTIVE_IS_LOW;
		SclkPhase    = SCPH_TOGGLES_IN_MIDDLE;
		break;
	case 1:
		SclkPolarity = SCPOL_INACTIVE_IS_LOW;
		SclkPhase    = SCPH_TOGGLES_AT_START;
		break;
	case 2:
		SclkPolarity = SCPOL_INACTIVE_IS_HIGH;
		SclkPhase    = SCPH_TOGGLES_IN_MIDDLE;
		break;
	case 3:
		SclkPolarity = SCPOL_INACTIVE_IS_HIGH;
		SclkPhase    = SCPH_TOGGLES_AT_START;
		break;
	default:  // same as 3
		SclkPolarity = SCPOL_INACTIVE_IS_HIGH;
		SclkPhase    = SCPH_TOGGLES_AT_START;
		break;
	}

	// Re-init after setting role
	SSI_StructInit(&SSI_InitStruct);
	if (slave == 1) {
		SSI_SetRole(ssi_adapter->spi_dev, SSI_SLAVE);
		ssi_adapter->Role = SSI_SLAVE;
		SSI_InitStruct.SPI_Role = SSI_SLAVE;
	} else {
		SSI_SetRole(ssi_adapter->spi_dev, SSI_MASTER);
		ssi_adapter->Role = SSI_MASTER;
		SSI_InitStruct.SPI_Role = SSI_MASTER;
	}
	SSI_Init(ssi_adapter->spi_dev, &SSI_InitStruct);

	SSI_SetSclkPhase(ssi_adapter->spi_dev, SclkPhase);
	SSI_SetSclkPolarity(ssi_adapter->spi_dev, SclkPolarity);
	SSI_SetDataFrameSize(ssi_adapter->spi_dev, DataFrameSize);

	if (slave == 1) {
		if (SclkPolarity == SCPOL_INACTIVE_IS_LOW) {
			PAD_PullCtrl((u32)obj->sclk, GPIO_PuPd_DOWN);
		} else {
			PAD_PullCtrl((u32)obj->sclk, GPIO_PuPd_UP);
		}
	}
}

/**
  * @brief  Set SPI baudrate.
  * @param  obj: SPI master object defined in application software.
  * @param  hz: Baudrate for SPI bus in units of Hz.
  * @retval none
  * @attention Baudrate to be set should be less than or equal to half of the SPI IpClk.
  * @note IpClk for SPI0 and SPI1 is 100MHz.
  */
void spi_frequency(spi_t *obj, int hz)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	u32 IpClk;
	u32 ClockDivider;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];

	IpClk = 100000000;

	/*Adjust SCKDV-Parameter to an even number */
	ClockDivider = IpClk / hz + 1;
	if ((IpClk % hz) > (u32)(hz / 2)) {
		ClockDivider++;
	}
	if (ClockDivider >= 0xFFFF) {
		/*  devider is 16 bits */
		ClockDivider = 0xFFFE;
	}
	ClockDivider &= 0xFFFE;     // bit 0 always is 0

	SSI_SetBaudDiv(ssi_adapter->spi_dev, ClockDivider);
}

void spi_slave_select(spi_t *obj, ChipSelect slaveindex)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];

	if (ssi_adapter->Role == SSI_MASTER) {
		SSI_SetSlaveEnable(ssi_adapter->spi_dev, slaveindex);
	} else {
		assert_param(0);
	}
}

static inline void ssi_write(spi_t *obj, int value)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];

	while (!SSI_Writeable(ssi_adapter->spi_dev));
	SSI_WriteData(ssi_adapter->spi_dev, value);
}

static inline int ssi_read(spi_t *obj)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];

	while (!SSI_Readable(ssi_adapter->spi_dev));
	return (int)SSI_ReadData(ssi_adapter->spi_dev);
}

/**
  * @brief  Master SPI sends one frame.
  * @param  obj: SPI master object defined in application software.
  * @param  value: Data to be transmitted.
  * @return Data received from slave SPI.
  */
int spi_master_write(spi_t *obj, int value)
{
	ssi_write(obj, value);
	return ssi_read(obj);
}

/**
  * @brief  Get readable state and busy state of slave SPI.
  * @param  obj: SPI slave object defined in application software.
  * @return Readable and busy state of slave SPI.
  */
int spi_slave_receive(spi_t *obj)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	int Readable;
	int Busy;

	Readable = SSI_Readable(ssi_adapter->spi_dev);
	Busy     = (int)SSI_Busy(ssi_adapter->spi_dev);
	return ((Readable && !Busy) ? 1 : 0);
}

/**
  * @brief  Slave SPI receives one frame.
  * @param  obj: SPI slave object defined in application software.
  * @return Data received from master SPI.
  */
int spi_slave_read(spi_t *obj)
{
	return ssi_read(obj);
}

/**
  * @brief  Slave SPI sends one frame.
  * @param  obj: SPI slave object defined in application software.
  * @param  value: Data to be transmitted.
  * @retval none
  */
void spi_slave_write(spi_t *obj, int value)
{
	ssi_write(obj, value);
}

/**
  * @brief  Get SPI busy state.
  * @param  obj: SPI object defined in application software.
  * @return Current busy state.
  */
int spi_busy(spi_t *obj)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];

	return (int)SSI_Busy(ssi_adapter->spi_dev);
}

/**
  * @brief  SPI device to flush Rx FIFO.
  * @param  obj: SPI object defined in application software.
  * @retval  none
  */
void spi_flush_rx_fifo(spi_t *obj)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	u32 rx_fifo_level;
	u32 i;

	while (SSI_Readable(ssi_adapter->spi_dev)) {
		rx_fifo_level = SSI_GetRxCount(ssi_adapter->spi_dev);
		for (i = 0; i < rx_fifo_level; i++) {
			(void)SSI_ReadData(ssi_adapter->spi_dev);
		}
	}
}

/**
  * @brief  Slave SPI receives data of target length under interrupt mode.
  * @param  obj: SPI slave object defined in application software.
  * @param  rx_buffer: Buffer to save data read from SPI FIFO.
  * @param  length: Number of data bytes to be read.
  * @return Stream init status.
  * @retval HAL_OK: Success.
  * @retval HAL_BUSY: Error.
  */
int32_t spi_slave_read_stream(spi_t *obj, char *rx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	int32_t ret;

	if (obj->state & SPI_STATE_RX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spis int rx: state(0x%x) is not ready\r\n",
				   obj->state);
		return HAL_BUSY;
	}

	//DBG_PRINTF(MODULE_SPI, LEVEL_INFO, "rx_buffer addr: %X, length: %d\n", rx_buffer, length);
	obj->state |= SPI_STATE_RX_BUSY;
	if ((ret = ssi_int_read(ssi_adapter, rx_buffer, length)) != _TRUE) {
		obj->state &= ~SPI_STATE_RX_BUSY;
	}

	return (ret == _TRUE) ? HAL_OK : HAL_BUSY;
}

/**
  * @brief  Slave SPI sends data of target length under interrupt mode.
  * @param  obj: SPI slave object defined in application software.
  * @param  tx_buffer: Buffer to be written to Tx FIFO.
  * @param  length: Number of data bytes to be sent.
  * @return Stream init status.
  * @retval HAL_OK: Success.
  * @retval HAL_BUSY: Error.
  */
int32_t spi_slave_write_stream(spi_t *obj, char *tx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	int32_t ret;

	if (obj->state & SPI_STATE_TX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spis int tx: state(0x%x) is not ready\r\n",
				   obj->state);
		return HAL_BUSY;
	}

	obj->state |= SPI_STATE_TX_BUSY;
	if ((ret = ssi_int_write(ssi_adapter, (u8 *) tx_buffer, length)) != _TRUE) {
		obj->state &= ~SPI_STATE_TX_BUSY;
	}
	return (ret == _TRUE) ? HAL_OK : HAL_BUSY;
}

/**
  * @brief  Master SPI receives data of target length under interrupt mode.
  * @param  obj: SPI master object defined in application software.
  * @param  rx_buffer: Buffer to save data read from SPI FIFO.
  * @param  length: Number of data bytes to be read.
  * @return Stream init status.
  * @retval HAL_OK: Success.
  * @retval HAL_BUSY: Error.
  */
int32_t spi_master_read_stream(spi_t *obj, char *rx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];

	int32_t ret;

	if (obj->state & SPI_STATE_RX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spim int rx: state(0x%x) is not ready\r\n",
				   obj->state);
		return HAL_BUSY;
	}

	// wait bus idle
	while (SSI_Busy(ssi_adapter->spi_dev));

	obj->state |= SPI_STATE_RX_BUSY;
	if ((ret = ssi_int_read(ssi_adapter, rx_buffer, length)) == _TRUE) {
		/* as Master mode, it need to push data to TX FIFO to generate clock out
		then the slave can transmit data out */
		// send some dummy data out
		if ((ret = ssi_int_write(ssi_adapter, NULL, length)) != _TRUE) {
			obj->state &= ~SPI_STATE_RX_BUSY;
		}
	} else {
		obj->state &= ~SPI_STATE_RX_BUSY;
	}

	return (ret == _TRUE) ? HAL_OK : HAL_BUSY;
}

/**
  * @brief  Master SPI sends data of target length under interrupt mode.
  * @param  obj: SPI master object defined in application software.
  * @param  tx_buffer: Buffer to be written to Tx FIFO.
  * @param  length: Number of data bytes to be sent.
  * @return Stream init status.
  * @retval HAL_OK: Success.
  * @retval HAL_BUSY: Error.
  */
int32_t spi_master_write_stream(spi_t *obj, char *tx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	int32_t ret;

	if (obj->state & SPI_STATE_TX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spim int tx: state(0x%x) is not ready\r\n",
				   obj->state);
		return HAL_BUSY;
	}

	obj->state |= SPI_STATE_TX_BUSY;
	/* as Master mode, sending data will receive data at sametime, so we need to
	drop those received dummy data */
	if ((ret = ssi_int_write(ssi_adapter, (u8 *) tx_buffer, length)) != _TRUE) {
		obj->state &= ~SPI_STATE_TX_BUSY;
	}
	return (ret == _TRUE) ? HAL_OK : HAL_BUSY;
}

/**
  * @brief  Master SPI sends and receives data of target length under interrupt mode.
  * @param  obj: SPI master object defined in application software.
  * @param  tx_buffer: Buffer to be written to Tx FIFO.
  * @param  rx_buffer: Buffer to save data read from SPI FIFO.
  * @param  length: Number of data bytes to be sent and receive.
  * @return Stream init status.
  * @retval HAL_OK: Success.
  * @retval HAL_BUSY: Error.
  */
int32_t spi_master_write_read_stream(spi_t *obj, char *tx_buffer,
									 char *rx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	int32_t ret;

	if (obj->state & (SPI_STATE_RX_BUSY | SPI_STATE_TX_BUSY)) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spim int trx: state(0x%x) is not ready\r\n",
				   obj->state);
		return HAL_BUSY;
	}

	// wait bus idle
	while (SSI_Busy(ssi_adapter->spi_dev));

	obj->state |= SPI_STATE_RX_BUSY;
	/* as Master mode, sending data will receive data at sametime */
	if ((ret = ssi_int_read(ssi_adapter, rx_buffer, length)) == _TRUE) {
		obj->state |= SPI_STATE_TX_BUSY;
		if ((ret = ssi_int_write(ssi_adapter, (u8 *) tx_buffer, length)) != _TRUE) {
			obj->state &= ~(SPI_STATE_RX_BUSY | SPI_STATE_TX_BUSY);
			// Disable RX IRQ
			SSI_INTConfig(ssi_adapter->spi_dev, (SPI_BIT_RXFIM | SPI_BIT_RXOIM | SPI_BIT_RXUIM), DISABLE);
		}
	} else {
		obj->state &= ~(SPI_STATE_RX_BUSY);
	}

	return (ret == _TRUE) ? HAL_OK : HAL_BUSY;
}

/**
  * @brief  Register SPI interrupt handler if needed.
  * @param  obj: SPI object defined in application software.
  * @param  handler: SPI Interrupt callback function.
  * @param  id: SPI Interrupt callback parameter.
  * @retval none
  */
void spi_irq_hook(spi_t *obj, spi_irq_handler handler, uint32_t id)
{
	obj->irq_handler = (u32)handler;
	obj->irq_id = (u32)id;
}

/**
  * @brief  Slave SPI receives data of target length under DMA mode.
  * @param  obj: SPI slave object defined in application software.
  * @param  rx_buffer: Buffer to save data read from SPI FIFO.
  * @param  length: Number of data bytes to be read.
  * @return Stream init status.
  * @retval HAL_OK: Success.
  * @retval HAL_BUSY: Error.
  */
int32_t spi_slave_read_stream_dma(spi_t *obj, char *rx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	int32_t ret;

	if (obj->state & SPI_STATE_RX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spim dma rx: state(0x%x) is not ready\r\n",
				   obj->state);
		return HAL_BUSY;
	}

	obj->state |= SPI_STATE_RX_BUSY;
	ret = ssi_dma_recv(ssi_adapter, (u8 *) rx_buffer, length);
	if (ret != _TRUE) {
		obj->state &= ~SPI_STATE_RX_BUSY;
	}
	return (ret == _TRUE) ? HAL_OK : HAL_BUSY;
}

/**
  * @brief  Slave SPI sends data of target length under DMA mode.
  * @param  obj: SPI slave object defined in application software.
  * @param  tx_buffer: Buffer to be written to Tx FIFO.
  * @param  length: Number of data bytes to be send.
  * @return Stream init status.
  * @retval HAL_OK: Success.
  * @retval HAL_BUSY: Error.
  */
int32_t spi_slave_write_stream_dma(spi_t *obj, char *tx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	int32_t ret;

	if (obj->state & SPI_STATE_TX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spis dma tx: state(0x%x) is not ready\r\n",
				   obj->state);
		return HAL_BUSY;
	}

	obj->state |= SPI_STATE_TX_BUSY;
	ret = ssi_dma_send(ssi_adapter, (u8 *) tx_buffer, length);
	if (ret != _TRUE) {
		obj->state &= ~SPI_STATE_TX_BUSY;
	}
	return (ret == _TRUE) ? HAL_OK : HAL_BUSY;
}

/**
  * @brief  Master SPI receives data of target length under DMA mode.
  * @param  obj: SPI master object defined in application software.
  * @param  rx_buffer: Buffer to save data read from SPI FIFO.
  * @param  length: Number of data bytes to be read.
  * @return Stream init status.
  * @retval HAL_OK: Success.
  * @retval HAL_BUSY: Error.
  * @note DMA or Interrupt mode can be used to TX dummy data
  */
int32_t spi_master_read_stream_dma(spi_t *obj, char *rx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	int32_t ret;

	if (obj->state & SPI_STATE_RX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spim dma rx: state(0x%x) is not ready\r\n",
				   obj->state);
		return HAL_BUSY;
	}

	obj->state |= SPI_STATE_RX_BUSY;
	ret = ssi_dma_recv(ssi_adapter, (u8 *) rx_buffer, length);
	if (ret != _TRUE) {
		obj->state &= ~SPI_STATE_RX_BUSY;
	}

	// for master mode, we need to send data to generate clock out
#ifdef USE_DMA_WRITE_MASTER_READ
	// TX DMA is on already, so use DMA to TX data
	// Make the GDMA to use the rx_buffer too

	ret = ssi_dma_send(ssi_adapter, (u8 *) rx_buffer, length);
	if (ret != _TRUE) {
		obj->state &= ~SPI_STATE_RX_BUSY;
	}
#else
	// TX DMA isn't enabled, so we just use Interrupt mode to TX dummy data
	if ((ret = ssi_int_write(ssi_adapter, NULL, length)) != _TRUE) {
		obj->state &= ~SPI_STATE_RX_BUSY;
	}
#endif
	return (ret == _TRUE) ? HAL_OK : HAL_BUSY;
}

/**
  * @brief  Master SPI sends data of target length under DMA mode.
  * @param  obj: SPI master object defined in application software.
  * @param  tx_buffer: Buffer to be written to Tx FIFO.
  * @param  length: Number of data bytes to be sent.
  * @return Stream init status.
  * @retval HAL_OK: Success.
  * @retval HAL_BUSY: Error.
  */
int32_t spi_master_write_stream_dma(spi_t *obj, char *tx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	int32_t ret;

	if (obj->state & SPI_STATE_TX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spim dma tx: state(0x%x) is not ready\r\n",
				   obj->state);
		return HAL_BUSY;
	}

	obj->state |= SPI_STATE_TX_BUSY;
	ret = ssi_dma_send(ssi_adapter, (u8 *) tx_buffer, length);
	if (ret != _TRUE) {
		obj->state &= ~SPI_STATE_TX_BUSY;
	}

	return (ret == _TRUE) ? HAL_OK : HAL_BUSY;
}

/**
  * @brief  Master SPI sends and receives data of target length under DMA mode.
  * @param  obj: SPI master object defined in application software.
  * @param  tx_buffer: Buffer to be written to Tx FIFO.
  * @param  rx_buffer: Buffer to save data read from SPI FIFO.
  * @param  length: Number of data bytes to be sent and received.
  * @return Stream init status.
  * @retval HAL_OK: Success.
  * @retval HAL_BUSY: Error.
  */
int32_t spi_master_write_read_stream_dma(spi_t *obj, char *tx_buffer,
		char *rx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	int32_t ret;

	if (obj->state & (SPI_STATE_RX_BUSY | SPI_STATE_TX_BUSY)) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spi_master_write_and_read_stream: state(0x%x) is not ready\r\n", obj->state);
		return HAL_BUSY;
	}

	obj->state |= SPI_STATE_RX_BUSY;
	ret = ssi_dma_recv(ssi_adapter, (u8 *) rx_buffer, length);
	if (ret != _TRUE) {
		obj->state &= ~SPI_STATE_RX_BUSY;
		return HAL_BUSY;
	}

	obj->state |= SPI_STATE_TX_BUSY;
	ret = ssi_dma_send(ssi_adapter, (u8 *) tx_buffer, length);
	if (ret != _TRUE) {
		obj->state &= ~SPI_STATE_TX_BUSY;
		return HAL_BUSY;
	}

	return (ret == _TRUE) ? HAL_OK : HAL_BUSY;
}

/**
  * @brief  Slave SPI receives data of target length under DMA mode before timeout.
  * @param  obj: SPI slave object defined in application software.
  * @param  rx_buffer: Buffer to save data read from SPI FIFO.
  * @param  length: Number of data bytes to be read.
  * @param  timeout_ms: Timeout waiting time in units of ms.
  * @return Number of already read bytes.
  */
int32_t spi_slave_read_stream_dma_timeout(spi_t *obj, char *rx_buffer, uint32_t length, uint32_t timeout_ms)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	int ret, timeout = 0;
	uint32_t StartCount = 0;

	assert_param(timeout_ms > 0);

	if (obj->state & SPI_STATE_RX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spi_slave_read_stream_dma: state(0x%x) is not ready\r\n", obj->state);
		return -HAL_BUSY;
	}

	obj->state |= SPI_STATE_RX_BUSY;
	ret = ssi_dma_recv(ssi_adapter, (u8 *) rx_buffer, length);
	if (ret != _TRUE) {
		obj->state &= ~SPI_STATE_RX_BUSY;
		return -HAL_BUSY;
	}

	StartCount = SYSTIMER_TickGet();

	while (1) {
		/* complete */
		if ((obj->state & SPI_STATE_RX_BUSY) == 0) {
			break;
		}

		/* time out */
		if (SYSTIMER_GetPassTime(StartCount) > timeout_ms) {
			ret = spi_stop_recv(obj);
			obj->state &= ~ SPI_STATE_RX_BUSY;
			timeout = 1;

			DBG_PRINTF(MODULE_SPI, LEVEL_INFO, "Slave is timeout\n");
			break;
		}
	}

	//if (SSI_GetDataFrameSize(SPIx) > 8){
	//	ssi_adapter->RxLength <<= 1;
	//}

	if (timeout) {
		return (length - ssi_adapter->RxLength);
	} else {
		return length;
	}
}

/**
  * @brief  Slave SPI receives data of target length under DMA mode and stops if SPI bus is idle.
  * @param  obj: SPI slave object defined in application software.
  * @param  rx_buffer: Buffer to save data read from SPI FIFO.
  * @param  length: Number of data bytes to be read.
  * @return Number of already read bytes.
  */
int32_t spi_slave_read_stream_dma_terminate(spi_t *obj, char *rx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	SPI_TypeDef *SPIx = SPI_DEV_TABLE[spi_idx].SPIx;
	int ret;

	if (obj->state & SPI_STATE_RX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spi_slave_read_stream_dma: state(0x%x) is not ready\r\n", obj->state);
		return -HAL_BUSY;
	}

	obj->state |= SPI_STATE_RX_BUSY;
	ret = ssi_dma_recv(ssi_adapter, (u8 *) rx_buffer, length);
	if (ret != _TRUE) {
		obj->state &= ~SPI_STATE_RX_BUSY;
		return -HAL_BUSY;
	}

	while (obj->state & SPI_STATE_RX_BUSY) {
		if (SSI_Busy(SPIx) == 0) {
			ret = spi_stop_recv(obj);
			goto EndOfDMACS;
		}
	}

EndOfDMACS:
	if ((obj->state & SPI_STATE_RX_BUSY) != 0) {
		obj->state &= ~ SPI_STATE_RX_BUSY;
	}

	//if (SSI_GetDataFrameSize(SPIx) > 8) {
	//	ssi_adapter->RxLength <<= 1;
	//}

	return (length - ssi_adapter->RxLength);
}

/**
  * @brief  Slave device to flush Tx FIFO.
  * @param  obj: SPI slave object defined in application software.
  * @note  All data will be discarded both in Tx FIFO  and Rx FIFO.
  */
void spi_slave_flush_fifo(spi_t *obj)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	SPI_TypeDef *SPIx = SPI_DEV_TABLE[spi_idx].SPIx;

	SSI_Cmd(SPIx, DISABLE);
	SSI_Cmd(SPIx, ENABLE);

	obj->state &= ~SPI_STATE_TX_BUSY;
}

/**
  * @brief  Slave SPI receives data of target length under interrupt mode monitored before timeout.
  * @param  obj: SPI slave object defined in application software.
  * @param  rx_buffer: Buffer to save data read from SPI FIFO.
  * @param  length: Number of data bytes to be read.
  * @param  timeout_ms: Timeout waiting time in units of ms.
  * @return Number of already read bytes.
  */
int32_t spi_slave_read_stream_timeout(spi_t *obj, char *rx_buffer, uint32_t length, uint32_t timeout_ms)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	SPI_TypeDef *SPIx = SPI_DEV_TABLE[spi_idx].SPIx;
	uint32_t timeout = 0;
	uint32_t StartCount = 0;

	assert_param(timeout_ms > 0);

	if (obj->state & SPI_STATE_RX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spi_slave_read_stream: state(0x%x) is not ready\r\n", obj->state);
		return -HAL_BUSY;
	}

	obj->state |= SPI_STATE_RX_BUSY;
	if (ssi_int_read(ssi_adapter, rx_buffer, length) != _TRUE) {
		obj->state &= ~SPI_STATE_RX_BUSY;
		return -HAL_BUSY;
	}


	StartCount = SYSTIMER_TickGet();
	while (1) {
		/* complete */
		if ((obj->state & SPI_STATE_RX_BUSY) == 0) {
			break;
		}

		/* time out */
		if (SYSTIMER_GetPassTime(StartCount) > timeout_ms) {
			spi_stop_recv(obj);
			obj->state &= ~ SPI_STATE_RX_BUSY;
			timeout = 1;

			DBG_PRINTF(MODULE_SPI, LEVEL_INFO, "Slave is timeout\n");
			break;
		}
	}

	if (SSI_GetDataFrameSize(SPIx) > 8) {
		ssi_adapter->RxLength <<= 1;
	}

	if (timeout) {
		return (length - ssi_adapter->RxLength);
	} else {
		return length;
	}
}

/**
  * @brief  Slave SPI receives data of target length under interrupt mode and stops if SPI bus is idle.
  * @param  obj: SPI slave object defined in application software.
  * @param  rx_buffer: Buffer to save data read from SPI FIFO.
  * @param  length: Number of data bytes to be read.
  * @return Number of data bytes already read if success or -HAL_BUSY if error happens.
  */
int32_t spi_slave_read_stream_terminate(spi_t *obj, char *rx_buffer, uint32_t length)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;
	PHAL_SSI_ADAPTOR ssi_adapter = &ssi_adapter_g[spi_idx];
	SPI_TypeDef *SPIx = SPI_DEV_TABLE[spi_idx].SPIx;

	if (obj->state & SPI_STATE_RX_BUSY) {
		DBG_PRINTF(MODULE_SPI, LEVEL_WARN, "spi_slave_read_stream_dma: state(0x%x) is not ready\r\n", obj->state);
		return -HAL_BUSY;
	}

	obj->state |= SPI_STATE_RX_BUSY;
	if (ssi_int_read(ssi_adapter, rx_buffer, length) != _TRUE) {
		obj->state &= ~SPI_STATE_RX_BUSY;
		return -HAL_BUSY;
	}


	while (obj->state & SPI_STATE_RX_BUSY) {
		if (SSI_Busy(SPIx) == 0) {
			spi_stop_recv(obj);
			goto EndOfCS;
		}
	}

EndOfCS:
	if ((obj->state & SPI_STATE_RX_BUSY) != 0) {
		obj->state &= ~ SPI_STATE_RX_BUSY;
	}

	if (SSI_GetDataFrameSize(SPIx) > 8) {
		ssi_adapter->RxLength <<= 1;
	}

	return (length - ssi_adapter->RxLength);
}

/**
  * @brief  Enable SPI device clock.
  * @param  obj: SPI object defined in application software.
  * @retval none
  */
void spi_enable(spi_t *obj)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;

	if (spi_idx == 1) {
		RCC_PeriphClockCmd(APBPeriph_SPI1, APBPeriph_SPI1_CLOCK, ENABLE);
	} else {
		RCC_PeriphClockCmd(APBPeriph_SPI0, APBPeriph_SPI0_CLOCK, ENABLE);
	}
}

/**
  * @brief  Disable SPI device clock.
  * @param  obj: SPI object defined in application software.
  * @retval none
  */
void spi_disable(spi_t *obj)
{
	uint8_t  spi_idx = obj->spi_idx & 0x0F;

	if (spi_idx == 1) {
		RCC_PeriphClockCmd(APBPeriph_SPI1, APBPeriph_SPI1_CLOCK, DISABLE);
	} else {
		RCC_PeriphClockCmd(APBPeriph_SPI0, APBPeriph_SPI0_CLOCK, DISABLE);
	}
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
