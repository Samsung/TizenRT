/**
  ******************************************************************************
  * @file    amebad2_ledc.c
  * @author
  * @version V1.0.0
  * @date    2020-10-23
  * @brief   This file contains all the functions prototypes for the LEDC firmware
  *             library, including the following functionalities of the Universal Asynchronous
  *             Transmitter peripheral:
  *           - Initialization
  *           - LEDC Transmit Control (disable/enable)
  *           - Output Timings Control
  *           - Output RGB mode control
  *           - Interrupts management
  *           - FIFO Threshold setting interface
  *
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#include "ameba_soc.h"

/**
  * @brief  Fills each LEDC_InitStruct member with its default value.
  * @param  LEDC_InitStruct: pointer to an LEDC_InitTypeDef structure which will be initialized.
  * @retval None
  */
void LEDC_StructInit(LEDC_InitTypeDef *LEDC_InitStruct)
{
	LEDC_InitStruct->data_length = LEDC_DEFAULT_LED_NUM;
	LEDC_InitStruct->led_count = LEDC_DEFAULT_LED_NUM;
	LEDC_InitStruct->ledc_trans_mode = LEDC_DMA_MODE;
	LEDC_InitStruct->ledc_fifo_level = 0xF;
	LEDC_InitStruct->ledc_polarity = LEDC_IDLE_POLARITY_LOW;
	LEDC_InitStruct->output_RGB_mode = LEDC_OUTPUT_GRB; //input mode GRB
	LEDC_InitStruct->reset_ns = 0x3A97; //375000ns
	LEDC_InitStruct->t0h_ns = 0xd;
	LEDC_InitStruct->t0l_ns = 0x27;
	LEDC_InitStruct->t1h_ns = 0x27;
	LEDC_InitStruct->t1l_ns = 0xd;
	LEDC_InitStruct->wait_data_time_ns = 0x3A97; //375000ns
	LEDC_InitStruct->wait_time0_en = ENABLE;
	LEDC_InitStruct->wait_time1_en = ENABLE;
	LEDC_InitStruct->wait_time0_ns = 0xEF; //6us
	LEDC_InitStruct->wait_time1_ns = 0x61A7; //625000ns
}

/**
  * @brief  Initializes the LEDC peripheral according to the specified
  *   parameters in the LEDC_InitStruct
  * @param  LEDCx: selected LEDC peripheral.
  * @param  LEDC_InitStruct: pointer to a LEDC_InitTypeDef structure that
  *   contains the configuration information for the specified LEDC peripheral
  * @retval None
  */
void LEDC_Init(LEDC_TypeDef *LEDCx, LEDC_InitTypeDef *LEDC_InitStruct)
{
	LEDC_SetReset_val(LEDCx, LEDC_InitStruct->reset_ns);
	LEDC_SetT0H_val(LEDCx, LEDC_InitStruct->t0h_ns);
	LEDC_SetT0L_val(LEDCx, LEDC_InitStruct->t0l_ns);
	LEDC_SetT1H_val(LEDCx, LEDC_InitStruct->t1h_ns);
	LEDC_SetT1L_val(LEDCx, LEDC_InitStruct->t1l_ns);
	LEDC_SetWaitDataTime_val(LEDCx, LEDC_InitStruct->wait_data_time_ns);
	LEDC_SetWaitTime0_val(LEDCx, LEDC_InitStruct->wait_time0_ns, LEDC_InitStruct->wait_time0_en);
	LEDC_SetWaitTime1_val(LEDCx, LEDC_InitStruct->wait_time1_ns, LEDC_InitStruct->wait_time1_en);

	LEDC_SetPolarity(LEDCx, LEDC_InitStruct->ledc_polarity);
	LEDC_SetOutputMode(LEDCx, LEDC_InitStruct->output_RGB_mode);

	LEDC_SetTransferMode(LEDCx, LEDC_InitStruct->ledc_trans_mode);
	LEDC_SetFIFOLevel(LEDCx, LEDC_InitStruct->ledc_fifo_level);
	LEDC_SetLEDNum(LEDCx, LEDC_InitStruct->led_count);
	LEDC_SetTotalLength(LEDCx, LEDC_InitStruct->data_length);

	LEDC_INTConfig(LEDCx, LEDC_INT_EXT_EN, ENABLE);
}

/**
  * @brief  Enable or disable LEDC peripheral.
  * @param  LEDCx: selected LEDC peripheral.
  * @param  NewState: new state of the operation mode.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LEDC_Cmd(LEDC_TypeDef *LEDCx, u8 NewState)
{
	if (NewState == DISABLE) {
		LEDCx->LEDC_CTRL_REG &= ~LEDC_BIT_EN;
	} else {
		LEDC_SoftReset(LEDCx);
		DelayUs(1);
		LEDCx->LEDC_CTRL_REG |= LEDC_BIT_EN;
	}
}

/**
  * @brief  ENABLE/DISABLE LEDC's interrupt bits.
  * @param  LEDCx: selected LEDC peripheral.
  * @param  LEDC_IT: specifies the LEDCx interrupt sources to be enabled or disabled.
  *   This parameter can be one or combinations of the following values:
  *            @arg BIT_LEDC_FIFO_OVERFLOW_INT_EN: LEDC FIFO Overflow Interrupt
  *            @arg BIT_LEDC_WAITDATA_TIMEOUT_INT_EN: LEDC Wait Data Timeout Interrupt
  *            @arg BIT_LEDC_CPUREQ_FIFO_INT_EN: LEDC CPU Requset Interrupt
  *            @arg BIT_LEDC_TRANS_FINISH_INT_EN: LEDC Transfer Finishi Interrupt
  * @param  NewState: specifies the state of the interrupt.
  *   This parameter can be one of the following values:
  *            @arg ENABLE
  *            @arg DISABLE
  * @retval None
  */
void LEDC_INTConfig(LEDC_TypeDef *LEDCx, u32 LEDC_IT, u32 NewState)
{
	u32 reg_val = 0;

	assert_param(IS_LEDC_INTERRUPT(LEDC_IT));

	if (NewState == ENABLE) {
		reg_val = LEDC_BIT_GLOBAL_INT_EN | LEDC_IT;
		LEDCx->LEDC_LED_INTERRUPT_CTRL_REG |= reg_val;
	} else {
		LEDCx->LEDC_LED_INTERRUPT_CTRL_REG &= ~LEDC_IT;
	}
}

/**
  * @brief  Clear the LEDC's interrupt bits.
  * @param  LEDCx: selected LEDC peripheral.
  * @param  LEDC_IT: specifies the LEDCx interrupt sources to be enabled or disabled.
  *   This parameter can be one or combinations of the following values:
  *            @arg BIT_LEDC_FIFO_OVERFLOW_INT_EN: LEDC FIFO Overflow Interrupt
  *            @arg BIT_LEDC_WAITDATA_TIMEOUT_INT_EN: LEDC Wait Data Timeout Interrupt
  *            @arg BIT_LEDC_CPUREQ_FIFO_INT_EN: LEDC CPU Requset Interrupt
  *            @arg BIT_LEDC_TRANS_FINISH_INT_EN: LEDC Transfer Finishi Interrupt
  * @retval None
  */
void LEDC_ClearINT(LEDC_TypeDef *LEDCx, u32 LEDC_IT)
{
	u32 reg_val;

	assert_param(IS_LEDC_INTERRUPT(LEDC_IT));

	reg_val = LEDCx->LEDC_LED_INT_STS_REG;
	reg_val |= LEDC_IT;
	LEDCx->LEDC_LED_INT_STS_REG = reg_val;
}

/**
  * @brief  Get LEDC's interrupt status.
  * @param  LEDCx: selected LEDC peripheral.
  * @retval LEDC interrupt status.
  *   each bit of the interrupt status shows as follows:
  *            - bit 17 : FIFO empty status
  *            - bit 16 : FIFO full status
  *            - bit 3 : FIFO overflow interrupt happens when FIFO overflow
  *            - bit 2 : Wait data timeout interrupt happens when FIFO is empty after LEDC_WAIT_DATA_TIME
  *            - bit 1 : CPU Request interrupt happens when FIFO left less than FIFO threshold
  *            - bit 0 : Transfer done interrupt happens when transfer complete.
  */
u32 LEDC_GetINT(LEDC_TypeDef *LEDCx)
{
	return LEDCx->LEDC_LED_INT_STS_REG;
}

/**
  * @brief  Set LEDC transfer mode
  * @param  LEDCx: selected LEDC peripheral.
  * @param  mode:This parameter can be one of the following values:
  *            @arg LEDC_CPU_MODE
  *            @arg LEDC_DMA_MODE
  * @retval None
  */
void LEDC_SetTransferMode(LEDC_TypeDef *LEDCx, u32 mode)
{
	assert_param(IS_LEDC_TRANS_MODE(mode));

	if (mode) {
		LEDCx->LEDC_DMA_CTRL_REG |= LEDC_BIT_DMA_EN;
		LEDCx->LEDC_LED_INTERRUPT_CTRL_REG &= ~LEDC_BIT_FIFO_CPUREQ_INT_EN;
	} else {
		LEDCx->LEDC_DMA_CTRL_REG &= ~LEDC_BIT_DMA_EN;
		LEDCx->LEDC_LED_INTERRUPT_CTRL_REG |= LEDC_BIT_GLOBAL_INT_EN | LEDC_BIT_FIFO_CPUREQ_INT_EN;
	}
}

/**
  * @brief  Get LEDC transfer mode
  * @param  LEDCx: selected LEDC peripheral.
  * @retval LEDC transfer mode.
  *   the result can be one of the folling values:
  *            - LEDC_DMA_MODE
  *            - LEDC_CPU_MODE
  */
u32 LEDC_GetTransferMode(LEDC_TypeDef *LEDCx)
{
	if (LEDCx->LEDC_DMA_CTRL_REG & LEDC_BIT_DMA_EN) {
		return LEDC_DMA_MODE;
	} else {
		return LEDC_CPU_MODE;
	}
}

/**
  * @brief  Set LEDC's FIFO Level
  * @param  LEDCx: selected LEDC peripheral.
  * @param  FifoLevel: LEDCx FIFO level. Value range: 0 to 31.
  *   When the number of transmit FIFO entries is greater or equal to this value,
  *         DMA request or BIT_LEDC_CPUREQ_FIFO_INT_EN happens.
  *         This parameter is recommaned to 7 or 15.
  *
  * @retval None
  */
void LEDC_SetFIFOLevel(LEDC_TypeDef *LEDCx, u8 FifoLevel)
{
	u32 reg_val;

	reg_val = LEDCx->LEDC_DMA_CTRL_REG;
	reg_val &= ~LEDC_MASK_FIFO_TRIG_LEVEL;
	reg_val |= LEDC_FIFO_TRIG_LEVEL(FifoLevel);
	LEDCx->LEDC_DMA_CTRL_REG = reg_val;
}

/**
  * @brief  Get LEDC FIFO Level
  * @param  LEDCx: selected LEDC peripheral.
  * @retval LEDC FIFO level
  */
u32 LEDC_GetFIFOLevel(LEDC_TypeDef *LEDCx)
{
	return (LEDC_GET_FIFO_TRIG_LEVEL(LEDCx->LEDC_DMA_CTRL_REG) + 1);
}

/**
  * @brief  Init and Enable LEDC TX GDMA.
  * @param  LEDCx: selected LEDC peripheral.
  * @param  GDMA_InitStruct: pointer to a GDMA_InitTypeDef structure that contains
  *   the configuration information for the GDMA peripheral.
  * @param  CallbackData: GDMA callback data.
  * @param  CallbackFunc: GDMA callback function.
  * @param  pTxData: Tx Buffer.
  * @param  Length: Tx Count.
  * @retval   TRUE/FLASE
  */
BOOL LEDC_TXGDMA_Init(
	LEDC_TypeDef *LEDCx,
	PGDMA_InitTypeDef GDMA_InitStruct,
	void *CallbackData,
	IRQ_FUN CallbackFunc,
	u32 *pTxData,
	u32 Length
)
{
	u8 GdmaChnl;
	u8 LedcFIFOThr;

	assert_param(GDMA_InitStruct != NULL);

	// clear and invalidate D_Cache of tx RAM address
	DCache_CleanInvalidate((u32)pTxData, Length);

	// Allocate a GDMA channel for LEDC tx
	GdmaChnl = GDMA_ChnlAlloc(0, (IRQ_FUN)CallbackFunc, (u32)CallbackData, INT_PRI_MIDDLE);
	if (GdmaChnl == 0xFF) { /*	No Available DMA channel */
		return _FALSE;
	}

	LedcFIFOThr = LEDC_GetFIFOLevel(LEDCx);

	// alloc and Init GDMA
	_memset((void *)GDMA_InitStruct, 0, sizeof(GDMA_InitTypeDef));

	GDMA_InitStruct->GDMA_Index = 0;
	GDMA_InitStruct->GDMA_ChNum = GdmaChnl;
	GDMA_InitStruct->GDMA_DIR = TTFCMemToPeri_PerCtrl;
	GDMA_InitStruct->GDMA_DstHandshakeInterface = GDMA_HANDSHAKE_INTERFACE_LEDC_TX;
	GDMA_InitStruct->GDMA_IsrType = (BlockType | TransferType | ErrType);

	GDMA_InitStruct->GDMA_SrcAddr = (u32)(pTxData);
	GDMA_InitStruct->GDMA_DstAddr = (u32) & (LEDCx->LEDC_DATA_REG);
	GDMA_InitStruct->GDMA_DstInc = NoChange;
	GDMA_InitStruct->GDMA_SrcInc = IncType;
	GDMA_InitStruct->GDMA_DstMsize = MsizeSixteen;
	GDMA_InitStruct->GDMA_DstDataWidth = TrWidthFourBytes;
	GDMA_InitStruct->GDMA_SrcMsize = MsizeSixteen;
	GDMA_InitStruct->GDMA_SrcDataWidth = TrWidthFourBytes;

	if (LedcFIFOThr == 0x08) {
		GDMA_InitStruct->GDMA_DstMsize = MsizeEight;
		GDMA_InitStruct->GDMA_DstDataWidth = TrWidthFourBytes;
		GDMA_InitStruct->GDMA_SrcMsize = MsizeEight;
		GDMA_InitStruct->GDMA_SrcDataWidth = TrWidthFourBytes;
	}

	GDMA_Init(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, GDMA_InitStruct);
	GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, ENABLE);

	return _TRUE;
}

/**
  * @brief  Set LEDC soft reset, Force LEDC Enters IDLE State
  * @param  LEDCx: selected LEDC peripheral.
  * @retval None
  *
  * @note LEDC soft reset will clear to 0 after set 1; after LEDC soft reset, LEDC enters idle state, which
  *   brings all interrupt status cleared, FIFO read write pointer cleared, finished data count is 0 and LEDC_EN bit is
  *   cleared to 0.
  */
void LEDC_SoftReset(LEDC_TypeDef *LEDCx)
{
	LEDCx->LEDC_CTRL_REG |= LEDC_BIT_SOFT_RESET;
}

/**
  * @brief  Set LEDC Total Data Length
  * @param  LEDCx: selected LEDC peripheral.
  * @param  len: LEDCx data len. Value range: 0 to 8023.
  *   When the number of transmit FIFO entries is greater or equal to this value, MA request or
  *   BIT_LEDC_CPUREQ_FIFO_INT_EN happens. This parameter is recommaned to 7 or 15.
  * @retval None
  */
void LEDC_SetTotalLength(LEDC_TypeDef *LEDCx, u32 TotalData)
{
	u32 reg_val;

	assert_param(IS_LEDC_DATA_LENGTH(TotalData));

	reg_val = LEDCx->LEDC_CTRL_REG;
	reg_val &= ~LEDC_MASK_TOTAL_DATA_LENGTH;
	reg_val |= LEDC_TOTAL_DATA_LENGTH(TotalData - 1);
	LEDCx->LEDC_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC LED Number
  * @param  LEDCx: selected LEDC peripheral.
  * @param  Num: LEDCx LED number. Value range: 0 to 1023.
  * @retval None
  */
void LEDC_SetLEDNum(LEDC_TypeDef *LEDCx, u32 Num)
{
	u32 reg_val;

	assert_param(IS_LEDC_LED_NUM(Num));

	reg_val = LEDCx->LEDC_LED_RESET_TIMING_CTRL_REG;
	reg_val &= ~LEDC_MASK_LED_NUM;
	reg_val |= LEDC_LED_NUM(Num - 1);
	LEDCx->LEDC_LED_RESET_TIMING_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC Idle Output Level
  * @param  LEDCx: selected LEDC peripheral.
  * @param  Pol: LEDCx Output level.
  *   This parameter can be one of the following values:
  *            @arg LEDC_IDLE_POLARITY_HIGH: LEDC Idle output High Level
  *            @arg LEDC_IDLE_POLARITY_LOW: LEDC Idle output Low Level
  * @retval None
  */
void LEDC_SetPolarity(LEDC_TypeDef *LEDCx, u32 Pol)
{
	u32 reg_val;

	reg_val = LEDCx->LEDC_CTRL_REG;
	if (Pol) {
		reg_val |= LEDC_BIT_LED_POLARITY;
	} else {
		reg_val &= ~LEDC_BIT_LED_POLARITY;
	}

	LEDCx->LEDC_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC Output RGB Mode
  * @param  LEDCx: selected LEDC peripheral.
  * @param  mode: LEDCx Output mode with input GBR mode order.
  *   This parameter can be one of the following values:
  *            @arg LEDC_OUTPUT_GRB: LEDC Output in order of GRB
  *            @arg LEDC_OUTPUT_GBR: LEDC Output in order of GBR
  *            @arg LEDC_OUTPUT_RGB: LEDC Output in order of RGB
  *            @arg LEDC_OUTPUT_BGR: LEDC Output in order of BGR
  *            @arg LEDC_OUTPUT_RBG: LEDC Output in order of RBG
  *            @arg LEDC_OUTPUT_BRG: LEDC Output in order of BRG
  * @retval None
  */
void LEDC_SetOutputMode(LEDC_TypeDef *LEDCx, u32 mode)
{
	u32 reg_val;

	assert_param(IS_LEDC_OUTPUT_MODE(mode));

	reg_val = LEDCx->LEDC_CTRL_REG;
	reg_val &= ~LEDC_MASK_LED_RGB_MODE;
	reg_val |= LEDC_LED_RGB_MODE(mode);
	LEDCx->LEDC_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC Input data with MSB or LSB order
  * @param  LEDCx: selected LEDC peripheral.
  *   This parameter can be one of the following values:
  *            @arg LEDC_INPUT_LSB: LEDC input data in order of GRB
  *            @arg LEDC_INPUT_MSB: LEDC input data in order of GBR
  * @retval None
  */
void LEDC_SetOutputOrder(LEDC_TypeDef *LEDCx, u8 order)
{
	u32 reg_val;

	reg_val = LEDCx->LEDC_CTRL_REG;
	reg_val &= ~LEDC_OUTPUT_ORDER_MASK;
	reg_val |= LEDC_OUTPUT_ORDER(order);
	LEDCx->LEDC_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC Input MSB or LSB order
  * @param  LEDCx: selected LEDC peripheral.
  * @param  order: LEDCx input order with BYTE2:BYTE1:BYTE0.
  *   Value Range is 0 to 7. 1b`0 is LSB; 1b`1 is MSB.
  * @retval None
  */
void LEDC_SetInputMode(LEDC_TypeDef *LEDCx, u8 order)
{
	u32 reg_val;

	assert_param(IS_LEDC_INPUT_ORDER(order));

	reg_val = LEDCx->LEDC_CTRL_REG;
	if (order) {
		reg_val |= LEDC_BIT_LED_MSB_TOP;
	} else {
		reg_val &= ~LEDC_BIT_LED_MSB_TOP;
	}

	LEDCx->LEDC_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC Send LED refresh sinal
  * @param  LEDCx: selected LEDC peripheral.
  * @retval None
  */
void LEDC_LEDReset(LEDC_TypeDef *LEDCx)
{
	LEDCx->LEDC_CTRL_REG |= LEDC_BIT_RESET_LED_EN;
}

/**
  * @brief  Write Data to LEDC FIFO
  * @param  LEDCx: selected LEDC peripheral.
  * @param  data: LEDCx input data, only the lower 24bits valid.
  * @retval None
  */
void LEDC_WriteData(LEDC_TypeDef *LEDCx, u32 data)
{
	LEDCx->LEDC_DATA_REG = LEDC_DATA(data);
}

/**
  * @brief  Send Specific Data to LEDC FIFO
  * @param  LEDCx: selected LEDC peripheral.
  * @param  data: LEDCx input source data, only the lower 24bits valid.
  * @param  Len: data length to send.
  * @retval None
  */
u32 LEDC_SendData(LEDC_TypeDef *LEDCx, void *data, u32 Len)
{
	u32 fifo_full;
	u32 tx_max;
	u32 data_len = Len;

	assert_param(IS_LEDC_DATA_LENGTH(Len));

	fifo_full = LEDC_GetINT(LEDCx)&LEDC_BIT_FIFO_FULL;
	tx_max = LEDC_FIFO_DEPTH - LEDC_GetFIFOLevel(LEDCx);

	if (!fifo_full) {
		while (tx_max--) {
			if (data != NULL) {
				LEDC_WriteData(LEDCx, *(u32 *)(data));
				data = (void *)(((u32 *)data) + 1);
			} else {
				break;
			}
			data_len --;

			if (data_len == 0) {
				break;
			}
		}
	}
	return (Len - data_len);
}

/**
  * @brief  Set LEDC Reset Time Value
  * @param  LEDCx: selected LEDC peripheral.
  * @param  RstVal: LEDC reset time Value. Value Range is 1 to 0x3FFF
  * @retval None
  */
void LEDC_SetReset_val(LEDC_TypeDef *LEDCx, u32 RstVal)
{
	u32 reg_val;

	reg_val = LEDCx->LEDC_LED_RESET_TIMING_CTRL_REG;
	reg_val &= ~LEDC_MASK_RESET_TIME;
	reg_val |= LEDC_RESET_TIME(RstVal);
	LEDCx->LEDC_LED_RESET_TIMING_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC Reset Time Value
  * @param  LEDCx: selected LEDC peripheral.
  * @param  T1hVal: LEDC logic 1 high level time. Value Range is 0 to 0x7F.
  * @retval None
  */
void LEDC_SetT1H_val(LEDC_TypeDef *LEDCx, u32 T1hVal)
{
	u32 reg_val;

	reg_val = LEDCx->LEDC_LED_T01_TIMING_CTRL_REG;
	reg_val &= ~LEDC_MASK_T1H_CNT;
	reg_val |= LEDC_T1H_CNT(T1hVal);
	LEDCx->LEDC_LED_T01_TIMING_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC Reset Time Value
  * @param  LEDCx: selected LEDC peripheral.
  * @param  T1lVal: LEDC logic 1 low level time. Value Range is 0 to 0x7F.
  * @retval None
  */
void LEDC_SetT1L_val(LEDC_TypeDef *LEDCx, u32 T1lVal)
{
	u32 reg_val;

	reg_val = LEDCx->LEDC_LED_T01_TIMING_CTRL_REG;
	reg_val &= ~LEDC_MASK_T1L_CNT;
	reg_val |= LEDC_T1L_CNT(T1lVal);
	LEDCx->LEDC_LED_T01_TIMING_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC Reset Time Value
  * @param  LEDCx: selected LEDC peripheral.
  * @param  T0hVal: LEDC logic 0 high level time. Value Range is 0 to 0x7F.
  * @retval None
  */
void LEDC_SetT0H_val(LEDC_TypeDef *LEDCx, u32 T0hVal)
{
	u32 reg_val;

	reg_val = LEDCx->LEDC_LED_T01_TIMING_CTRL_REG;
	reg_val &= ~LEDC_MASK_T0H_CNT;
	reg_val |= LEDC_T0H_CNT(T0hVal);
	LEDCx->LEDC_LED_T01_TIMING_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC Reset Time Value
  * @param  LEDCx: selected LEDC peripheral.
  * @param  T0lVal: LEDC logic 0 low level time. Value Range is 0 to 0x7F.
  * @retval None
  */
void LEDC_SetT0L_val(LEDC_TypeDef *LEDCx, u32 T0lVal)
{
	u32 reg_val;

	reg_val = LEDCx->LEDC_LED_T01_TIMING_CTRL_REG;
	reg_val &= ~LEDC_MASK_T0L_CNT;
	reg_val |= LEDC_T0L_CNT(T0lVal);
	LEDCx->LEDC_LED_T01_TIMING_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC Wait data Time Value
  * @param  LEDCx: selected LEDC peripheral.
  * @param  WaitDataVal: LEDC wait data. Value Range is 0 to 0x7FFF.
  * @retval None
  */
void LEDC_SetWaitDataTime_val(LEDC_TypeDef *LEDCx, u32 WaitDataVal)
{
	u32 reg_val;

	reg_val = LEDCx->LEDC_DATA_FINISH_CNT_REG;
	reg_val &= ~LEDC_MASK_LED_WAIT_DATA_TIME;
	reg_val |= LEDC_LED_WAIT_DATA_TIME(WaitDataVal);
	LEDCx->LEDC_DATA_FINISH_CNT_REG = reg_val;
}

/**
  * @brief  Set LEDC Wait between Two Packages Time0 Value
  * @param  LEDCx: selected LEDC peripheral.
  * @param  WaitTime0: LEDC wait time0. Value Range is 0 to 0x1FF.
  * @param  NewState: enable or disable wait time between tow packages.
  *   This parameter can be one of the following values:
  *            @arg ENABLE
  *            @arg DISABLE
  * @retval None
  */
void LEDC_SetWaitTime0_val(LEDC_TypeDef *LEDCx, u32 WaitTime0, u8 NewState)
{
	u32 reg_val = 0;

	if (NewState == ENABLE) {
		reg_val = LEDC_BIT_WAIT_TIME0_EN | LEDC_TOTAL_WAIT_TIME0(WaitTime0);
	} else {
		reg_val |= ~LEDC_BIT_WAIT_TIME0_EN;
	}

	LEDCx->LEDC_WAIT_TIME0_CTRL_REG = reg_val;
}

/**
  * @brief  Set LEDC Wait between Two Frames Time1 Value
  * @param  LEDCx: selected LEDC peripheral.
  * @param  WaitTime1: LEDC wait time1. Value Range is 0 to 0x7FFFFFFF.
  * @param  NewState: enable or disable wait time between tow frames.
  *   This parameter can be one of the following values:
  *            @arg ENABLE
  *            @arg DISABLE
  * @retval None
  */
void LEDC_SetWaitTime1_val(LEDC_TypeDef *LEDCx, u32 WaitTime1, u8 NewState)
{
	u32 reg_val = 0;

	if (NewState == ENABLE) {
		reg_val = LEDC_BIT_WAIT_TIME1_EN | LEDC_TOTAL_WAIT_TIME1(WaitTime1);
	} else {
		reg_val |= ~LEDC_BIT_WAIT_TIME1_EN;
	}

	LEDCx->LEDC_WAIT_TIME1_CTRL_REG = reg_val;
}

