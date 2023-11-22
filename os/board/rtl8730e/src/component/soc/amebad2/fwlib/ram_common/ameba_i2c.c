/**
  ******************************************************************************
  * @file    rtl8721d_i2c.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the functions prototypes for the I2C firmware
  *          library, including the following functionalities of theIntel-Integrated
  *             Circuit (I2C) peripheral:
  *           - Initialization
  *           - I2C Speed Setting
  *           - I2C Slave Address Updating
  *           - Receive/Send Data Interface
  *           - I2C Peripheral Control (disable/enable)
  *           - I2C SleepMode Control
  *           - Output pin Configuration
  *           - Interrupts and flags management
  *
  *  @verbatim
  *
  *			===================================================================
  *								   How to use this driver
  *			===================================================================
  *			1. Enable peripheral clock using the follwoing functions(it is enabled by default)
  *				RCC_PeriphClockCmd(APBPeriph_I2Cx, APBPeriph_I2Cx_CLOCK, ENABLE); for I2C0/I2C1/I2C2
  *
  *			2. Configure the I2C pinmux.
  *		   		Pinmux_Config(Pin_Num, PINMUX_FUNCTION_I2C)
  *
  *			3. Call I2C_Init(I2Cx, I2C_InitStruct) to configure the i2c with the corresponding configuration
  *
  *			4. Enable the NVIC and the corresponding interrupt using the function
  *			   I2C_INTConfig() and register the i2c irq handler if you need to use interrupt mode.
  *
  *			5. Enable i2c module using I2C_Cmd(I2Cx, NewState)
  *
  * @Note: All other functions can be used separately to modify, if needed,
  *         a specific feature of the I2C.
  *
  *	@endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

static const char *TAG = "I2C";

const I2C_DevTable I2C_DEV_TABLE[3] = {
#ifdef ARM_CORE_CM4
	{I2C0_DEV, I2C0_IRQ},
	{I2C1_DEV, I2C1_IRQ},
	{I2C2_DEV, I2C2_IRQ},

#elif defined (ARM_CORE_CM0)
	{I2C0_DEV, I2C0_IRQ},
	{I2C1_DEV, I2C1_IRQ},
	{I2C2_DEV, I2C2_IRQ},

#elif defined (ARM_CORE_CA32)
	{I2C0_DEV, I2C0_IRQ},
	{I2C1_DEV, I2C1_IRQ},
	{I2C2_DEV, I2C2_IRQ},
#endif
};

u32 I2C_SLAVEWRITE_PATCH;

/*below parameters are used for I2C speed fine-tune*/
u32 IC_SS_SCL_HCNT_TRIM = 0;
u32 IC_SS_SCL_LCNT_TRIM = 0;
u32 IC_FS_SCL_HCNT_TRIM = 0;
u32 IC_FS_SCL_LCNT_TRIM = 0;
u32 IC_HS_SCL_HCNT_TRIM = 0;
u32 IC_HS_SCL_LCNT_TRIM = 0;

/**
  * @brief  Fills each I2C_InitStruct member with its default value.
  * @param  I2C_InitStruct: pointer to an I2C_InitTypeDef structure which will be initialized.
  * @retval None
  */
void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct)
{
	/* Load HAL initial data structure default value */
	I2C_InitStruct->I2CMaster     = I2C_MASTER_MODE;
	I2C_InitStruct->I2CAddrMod    = I2C_ADDR_7BIT;
	I2C_InitStruct->I2CSpdMod     = I2C_SS_MODE;
	I2C_InitStruct->I2CClk        = 100;
	I2C_InitStruct->I2CIPClk      = 10000000;
	I2C_InitStruct->I2CAckAddr    = 0x11;
	I2C_InitStruct->I2CSdaHd      = 2;
	I2C_InitStruct->I2CSlvSetup   = 0x3;
	I2C_InitStruct->I2CRXTL       = 0x00;
	I2C_InitStruct->I2CTXTL       = 0x10;
	I2C_InitStruct->I2CMstReSTR   = DISABLE;
	I2C_InitStruct->I2CMstGC      = DISABLE;
	I2C_InitStruct->I2CMstStartB  = DISABLE;
	I2C_InitStruct->I2CSlvNoAck   = DISABLE;
	I2C_InitStruct->I2CFilter     = 0x101;
	I2C_InitStruct->I2CAckAddr1   = 0x12;
}

/**
  * @brief  Initializes the I2Cx peripheral according to the specified
  *			parameters in the I2C_InitStruct.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  I2C_InitStruct: pointer to a I2C_InitTypeDef structure that contains
  * 		the configuration information for the specified I2C peripheral.
  * @retval None
  */
void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct)
{
	u8  Specical;

	/* Check the parameters */
	assert_param(IS_I2C_ADDR_MODE(I2C_InitStruct->I2CAddrMod));
	assert_param(IS_I2C_SPEED_MODE(I2C_InitStruct->I2CSpdMod));

	/* Disable the IC first */
	I2Cx->IC_ENABLE &= ~I2C_BIT_ENABLE;

	/* Master case*/
	if (I2C_InitStruct->I2CMaster) {
		/*RESTART MUST be set in these condition in Master mode.
		But it might be NOT compatible in old slaves.*/
		if ((I2C_InitStruct->I2CAddrMod == I2C_ADDR_10BIT) || (I2C_InitStruct->I2CSpdMod == I2C_HS_MODE)
			|| (I2C_InitStruct->I2CMstStartB != 0)) {
			I2C_InitStruct->I2CMstReSTR = ENABLE;
		}

		I2Cx->IC_CON = (I2C_BIT_IC_SLAVE_DISABLE_1 |
						I2C_BIT_IC_SLAVE_DISABLE_0 |
						(I2C_InitStruct->I2CMstReSTR << 5) |
						(I2C_InitStruct->I2CSpdMod << 1) |
						(I2C_InitStruct->I2CMaster));

		/* To set target addr.*/
		Specical = 0;
		if ((I2C_InitStruct->I2CMstGC != 0) || (I2C_InitStruct->I2CMstStartB != 0)) {
			Specical = 1;
		}

		I2Cx->IC_TAR = ((I2C_InitStruct->I2CAddrMod << 12) |
						(Specical << 11) |
						(I2C_InitStruct->I2CMstStartB << 10) |
						(I2C_InitStruct->I2CAckAddr & I2C_MASK_IC_TAR));

		/* To Set I2C clock*/
		I2C_SetSpeed(I2Cx, I2C_InitStruct->I2CSpdMod, I2C_InitStruct->I2CClk, I2C_InitStruct->I2CIPClk);
	}    /*if (Master)*/
	else {
		I2Cx->IC_CON = ((I2C_InitStruct->I2CMaster << 7) |
						(I2C_InitStruct->I2CMaster << 6) |
						(I2C_InitStruct->I2CAddrMod << 3) |
						(I2C_InitStruct->I2CSpdMod << 1) |
						(I2C_InitStruct->I2CMaster));

		/* To set slave0 addr. */
		I2Cx->IC_SAR = (I2C_InitStruct->I2CAckAddr & I2C_MASK_IC_SAR);
		/* To set slave1 addr. */
		I2Cx->IC_SAR2 = (I2C_InitStruct->I2CAckAddr1 & I2C_MASK_IC_SAR2);
		/* To set slave no ack */
		I2Cx->IC_SLV_DATA_NACK_ONLY = I2C_InitStruct->I2CSlvNoAck;
		/* Set ack general call. */
		I2Cx->IC_ACK_GENERAL_CALL = (I2C_InitStruct->I2CSlvAckGC & I2C_BIT_ACK_GEN_CALL);
		/* to set SDA setup time */
		I2Cx->IC_SDA_SETUP = (I2C_InitStruct->I2CSlvSetup & I2C_MASK_IC_SDA_SETUP);
	}
	/* To set SDA hold time */
	I2Cx->IC_SDA_HOLD = (I2C_InitStruct->I2CSdaHd & I2C_MASK_IC_SDA_HOLD);

	/* To set TX_Empty Level */
	I2Cx->IC_TX_TL = I2C_InitStruct->I2CTXTL;

	/* To set RX_Full Level */
	I2Cx->IC_RX_TL = I2C_InitStruct->I2CRXTL;

	/* To set IC_FILTER */
	I2Cx->IC_FILTER = I2C_InitStruct->I2CFilter;

	/*I2C Clear all interrupts first*/
	I2C_ClearAllINT(I2Cx);

	/*I2C Disable all interrupts first*/
	I2C_INTConfig(I2Cx, 0xFFFFFFFF, DISABLE);
}

/**
  * @brief  Master sets I2C Speed Mode.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  SpdMd: I2C Speed Mode.
  *   This parameter can be one of the following values:
  *     @arg I2C_SS_MODE:
  *     @arg I2C_FS_MODE:
  *     @arg I2C_HS_MODE:(not support for I2C0)
  * @param  I2Clk: I2C Bus Clock, unit is KHz.
  *   This parameter can be one of the following values:
  *     @arg 50:
  *     @arg 100:
  *     @arg 400:
  *     @arg 1000:
  *     @arg 3000: or others
  * @param  IPClk: I2C IP Clock, unit is Hz.
  * @retval None
  */
void I2C_SetSpeed(I2C_TypeDef *I2Cx, u32 SpdMd, u32 I2Clk, u32 I2CIPClk)
{
	u32 ICHLcnt;
	u32 ICHtime;
	u32 ICLtime;
	u32 IPClkM = I2CIPClk / 1000000;

	switch (SpdMd) {
	case I2C_SS_MODE: {
		ICHtime = ((1000000 / I2Clk) * I2C_SS_MIN_SCL_HTIME) / (I2C_SS_MIN_SCL_HTIME + I2C_SS_MIN_SCL_LTIME);
		ICLtime = ((1000000 / I2Clk) * I2C_SS_MIN_SCL_LTIME) / (I2C_SS_MIN_SCL_HTIME + I2C_SS_MIN_SCL_LTIME);

		ICHLcnt = (ICHtime * IPClkM) / 1000;
		if (ICHLcnt > IC_SS_SCL_HCNT_TRIM) { /*this part is according to the fine-tune result*/
			ICHLcnt -= IC_SS_SCL_HCNT_TRIM;
		}
		I2Cx->IC_SS_SCL_HCNT = ICHLcnt + 1;
		ICHLcnt = (ICLtime * IPClkM) / 1000;
		if (ICHLcnt > IC_SS_SCL_LCNT_TRIM) { /*this part is according to the fine-tune result*/
			ICHLcnt -= IC_SS_SCL_LCNT_TRIM;
		}
		I2Cx->IC_SS_SCL_LCNT = ICHLcnt;

		break;
	}

	case I2C_FS_MODE: {
		ICHtime = ((1000000 / I2Clk) * I2C_FS_MIN_SCL_HTIME) / (I2C_FS_MIN_SCL_HTIME + I2C_FS_MIN_SCL_LTIME);
		ICLtime = ((1000000 / I2Clk) * I2C_FS_MIN_SCL_LTIME) / (I2C_FS_MIN_SCL_HTIME + I2C_FS_MIN_SCL_LTIME);

		ICHLcnt = (ICHtime * IPClkM) / 1000;
		if (ICHLcnt > IC_FS_SCL_HCNT_TRIM) { /*this part is according to the fine-tune result*/
			ICHLcnt -= IC_FS_SCL_HCNT_TRIM;
		}
		I2Cx->IC_FS_SCL_HCNT = ICHLcnt + 1;

		ICHLcnt = (ICLtime * IPClkM) / 1000;
		if (ICHLcnt > IC_FS_SCL_LCNT_TRIM) { /*this part is according to the fine-tune result*/
			ICHLcnt -= IC_FS_SCL_LCNT_TRIM;
		}
		I2Cx->IC_FS_SCL_LCNT = ICHLcnt;

		break;
	}

	case I2C_HS_MODE: {
		/*set Fast mode count for Master code*/
		ICHtime = ((1000000 / 400) * I2C_FS_MIN_SCL_HTIME) / (I2C_FS_MIN_SCL_HTIME + I2C_FS_MIN_SCL_LTIME);
		ICLtime = ((1000000 / 400) * I2C_FS_MIN_SCL_LTIME) / (I2C_FS_MIN_SCL_HTIME + I2C_FS_MIN_SCL_LTIME);

		ICHLcnt = (ICHtime * IPClkM) / 1000;
		if (ICHLcnt > IC_FS_SCL_HCNT_TRIM) { /*this part is according to the fine-tune result*/
			ICHLcnt -= IC_FS_SCL_HCNT_TRIM;
		}
		I2Cx->IC_FS_SCL_HCNT = ICHLcnt + 1;

		ICHLcnt = (ICLtime * IPClkM) / 1000;
		if (ICHLcnt > IC_FS_SCL_LCNT_TRIM) { /*this part is according to the fine-tune result*/
			ICHLcnt -= IC_FS_SCL_LCNT_TRIM;
		}
		I2Cx->IC_FS_SCL_LCNT = ICHLcnt;

		ICHtime = ((1000000 / I2Clk) * I2C_HS_MIN_SCL_HTIME_100) / (I2C_HS_MIN_SCL_HTIME_100 + I2C_HS_MIN_SCL_LTIME_100);
		ICLtime = ((1000000 / I2Clk) * I2C_HS_MIN_SCL_LTIME_100) / (I2C_HS_MIN_SCL_HTIME_100 + I2C_HS_MIN_SCL_LTIME_100);

		ICHLcnt = (ICHtime * IPClkM) / 1000;
		if (ICHLcnt > IC_HS_SCL_HCNT_TRIM) { /*this part is according to the fine-tune result*/
			ICHLcnt -= IC_HS_SCL_HCNT_TRIM;
		}
		I2Cx->IC_HS_SCL_HCNT = ICHLcnt + 1;

		ICHLcnt = (ICLtime * IPClkM) / 1000;
		if (ICHLcnt > IC_HS_SCL_LCNT_TRIM) { /*this part is according to the fine-tune result*/
			ICHLcnt -= IC_HS_SCL_LCNT_TRIM;
		}
		I2Cx->IC_HS_SCL_LCNT = ICHLcnt;

		break;
	}

	default:
		break;
	}
}

/**
  * @brief  Master transmits the address byte to select the slave device.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  Address: specifies the slave address which will be transmitted
  * @retval None.
  */
void I2C_SetSlaveAddress(I2C_TypeDef *I2Cx, u16 Address)
{
	u32 tar = I2Cx->IC_TAR & ~(I2C_MASK_IC_TAR);

	/*set target address to generate start signal*/
	I2Cx->IC_TAR = (Address & I2C_MASK_IC_TAR) | tar;
}

/**
  * @brief  Master transmits the address format byte to select the slave device.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  NewAddrFormat: where I2Cx can be I2C_ADDR_7BIT or I2C_ADDR_10BIT.
  * @retval None.
  */
void I2C_SetSlaveAddressFormat(I2C_TypeDef *I2Cx, u32 NewAddrFormat)
{
	u32 TempVal = I2Cx->IC_TAR;
	u32 TempVal2 = I2Cx->IC_CON;

	if (NewAddrFormat == I2C_ADDR_7BIT) {
		TempVal &= ~I2C_BIT_IC_10BITADDR_MASTER;
	} else if (NewAddrFormat == I2C_ADDR_10BIT) {
		TempVal |= I2C_BIT_IC_10BITADDR_MASTER;
		TempVal2 |= I2C_BIT_IC_RESTATRT_EN;
	}

	I2Cx->IC_TAR = TempVal;
	I2Cx->IC_CON = TempVal2;
}

/**
  * @brief  Checks whether the specified I2C flag is set or not.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  I2C_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg I2C_BIT_SLV_ACTIVITY:
  *     @arg I2C_BIT_MST_ACTIVITY:
  *     @arg I2C_BIT_RFF:
  *     @arg I2C_BIT_RFNE:
  *     @arg I2C_BIT_TFE:
  *     @arg I2C_BIT_TFNF:
  *     @arg I2C_BIT_ACTIVITY:
  * @retval The new state of I2C_FLAG:
  *          - 1: the specified I2C flag is set
  *          - 0: the specified I2C flag is not set
  */
u8 I2C_CheckFlagState(I2C_TypeDef *I2Cx, u32 I2C_FLAG)
{
	u8 bit_status = 0;

	if ((I2Cx->IC_STATUS & I2C_FLAG) != 0) {
		/* I2C_FLAG is set */
		bit_status = 1;
	}

	/* Return the I2C_FLAG status */
	return  bit_status;
}

/**
  * @brief  ENABLE/DISABLE  the I2C's interrupt bits..
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  I2C_IT: specifies the I2Cx interrupt sources to be enabled or disabled.
  *          This parameter can be one or combinations of the following values:
  *            @arg I2C_BIT_M_LP_WAKE_2: I2C slave1 Address Match Interrupt
  *            @arg I2C_BIT_M_LP_WAKE_1: I2C slave0 Address Match Interrupt
  *            @arg I2C_BIT_M_GEN_CALL: General Call Interrupt
  *            @arg I2C_BIT_M_START_DET: Start or Restart Condition Interrupt
  *            @arg I2C_BIT_M_STOP_DET: Stop Condition Interrupt
  *            @arg I2C_BIT_M_ACTIVITY: I2C Activity Interrupt
  *            @arg I2C_BIT_M_RX_DONE: Slave Transmitter RX Done Interrupt
  *            @arg I2C_BIT_M_TX_ABRT: Transmit Abort Interrupt
  *            @arg I2C_BIT_M_RD_REQ: Read Request Interrupt
  *            @arg I2C_BIT_M_TX_EMPTY: Transmit FIFO Empty Interrupt
  *            @arg I2C_BIT_M_TX_OVER: Transmit FIFO Over Interrupt
  *            @arg I2C_BIT_M_RX_FULL: Receive FIFO Full Interrupt
  *            @arg I2C_BIT_M_RX_OVER: Receive FIFO Over Interrupt
  *            @arg I2C_BIT_M_RX_UNDER: Receive FIFO Under Interrupt
  * @param  NewState: specifies the state of the interrupt.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_INTConfig(I2C_TypeDef *I2Cx, u32 I2C_IT, u32 NewState)
{
	u32 TempVal = I2Cx->IC_INTR_MASK;

	if (NewState == ENABLE) {
		TempVal |= I2C_IT;
	} else {
		TempVal &= ~I2C_IT;
	}

	I2Cx->IC_INTR_MASK = TempVal;
}

/**
  * @brief  Clears the specified I2C interrupt pending bit.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  INTrBit: specifies the interrupt to be cleared.
  *          This parameter can be one of the following values:
  *            @arg I2C_BIT_R_LP_WAKE_2: I2C slave1 Address Match Interrupt
  *            @arg I2C_BIT_R_LP_WAKE_1: I2C slave0 Address Match Interrupt
  *            @arg I2C_BIT_R_GEN_CALL: General Call Interrupt
  *            @arg I2C_BIT_R_START_DET: Start or Restart Condition Interrupt
  *            @arg I2C_BIT_R_STOP_DET: Stop Condition Interrupt
  *            @arg I2C_BIT_R_ACTIVITY: I2C Activity Interrupt
  *            @arg I2C_BIT_R_RX_DONE: Slave Transmitter RX Done Interrupt
  *            @arg I2C_BIT_R_TX_ABRT: Transmit Abort Interrupt
  *            @arg I2C_BIT_R_RD_REQ: Read Request Interrupt
  *            @arg I2C_BIT_R_TX_EMPTY: Transmit FIFO Empty Interrupt
  *            @arg I2C_BIT_R_TX_OVER: Transmit FIFO Over Interrupt
  *            @arg I2C_BIT_R_RX_FULL: Receive FIFO Full Interrupt
  *            @arg I2C_BIT_R_RX_OVER: Receive FIFO Over Interrupt
  *            @arg I2C_BIT_R_RX_UNDER: Receive FIFO Under Interrupt
  * @note   I2C_BIT_R_TX_EMPTY is automatically cleared by hardware when the buffer
  *         level goes above the I2CTXTL threshold.
  *         I2C_BIT_R_RX_FULL is automatically cleared by hardware when the buffer
  *         level goes below the I2CRXTL threshold.
  * @retval None
  */
u32 I2C_ClearINT(I2C_TypeDef *I2Cx, u32 INTrBit)
{
	u32 ret;

	switch (INTrBit) {
	case I2C_BIT_R_LP_WAKE_2:
		ret = I2Cx->IC_CLR_ADDR_MATCH;
		break;
	case I2C_BIT_R_LP_WAKE_1:
		ret = I2Cx->IC_CLR_ADDR_MATCH;
		break;
	case I2C_BIT_R_GEN_CALL:
		ret = I2Cx->IC_CLR_GEN_CALL;
		break;
	case I2C_BIT_R_START_DET:
		ret = I2Cx->IC_CLR_START_DET;
		break;
	case I2C_BIT_R_STOP_DET:
		ret = I2Cx->IC_CLR_STOP_DET;
		break;
	case I2C_BIT_R_ACTIVITY:
		ret = I2Cx->IC_CLR_ACTIVITY;
		break;
	case I2C_BIT_R_RX_DONE:
		ret = I2Cx->IC_CLR_RX_DONE;
		break;
	case I2C_BIT_R_TX_ABRT:
		ret = I2Cx->IC_CLR_TX_ABRT;
		break;
	case I2C_BIT_R_RD_REQ:
		ret = I2Cx->IC_CLR_RD_REQ;
		break;
	case I2C_BIT_R_TX_OVER:
		ret = I2Cx->IC_CLR_TX_OVER;
		break;
	case I2C_BIT_R_RX_OVER:
		ret = I2Cx->IC_CLR_RX_OVER;
		break;
	case I2C_BIT_R_RX_UNDER:
		ret = I2Cx->IC_CLR_RX_UNDER;
		break;
	case I2C_BIT_R_TX_EMPTY:
	case I2C_BIT_R_RX_FULL:
	default:
		ret = 0;
		break;
	}
	return ret;
}

/**
  * @brief  Clears all of the I2C interrupt pending bit.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @retval None
  */
u32 I2C_ClearAllINT(I2C_TypeDef *I2Cx)
{
	return I2Cx->IC_CLR_INTR;
}

/**
  * @brief  Get I2C Raw Interrupt Status.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @retval raw interrupt status
  */
u32 I2C_GetRawINT(I2C_TypeDef *I2Cx)
{
	return I2Cx->IC_RAW_INTR_STAT;
}

/**
  * @brief  Get I2C interrupt status.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @retval interrupt status
  */
u32 I2C_GetINT(I2C_TypeDef *I2Cx)
{
	return I2Cx->IC_INTR_STAT;
}

/**
  * @brief  Master sends single byte through the I2Cx peripheral according to the set of the upper layer.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  pBuf: point to the data that to be write.
  * @param  I2CCmd: specifies whether a read from FIFO or a write to FIFO is performed.
  * @param  I2CStop: specifies whether a STOP is issued after the byte is sent or received.
  * @param  I2CReSTR: specifies whether a RESTART is issued after the byte is sent or received.
  * @retval None
*/
void I2C_MasterSendNullData(I2C_TypeDef *I2Cx, u8 *pBuf, u8  I2CCmd, u8  I2CStop, u8  I2CReSTR)
{
	I2Cx->IC_DATA_CMD = *(pBuf) |
						(1 << 11) |
						(I2CReSTR << 10) |
						(I2CCmd << 8) |
						(I2CStop << 9);
}

/**
  * @brief  Master sends single byte through the I2Cx peripheral according to the set of the upper layer.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  pBuf: point to the data that to be write.
  * @param  I2CCmd: specifies whether a read from FIFO or a write to FIFO is performed.
  * @param  I2CStop: specifies whether a STOP is issued after the byte is sent or received.
  * @param  I2CReSTR: specifies whether a RESTART is issued after the byte is sent or received.
  * @retval None
*/
void I2C_MasterSend(I2C_TypeDef *I2Cx, u8 *pBuf, u8  I2CCmd, u8  I2CStop, u8  I2CReSTR)
{
	I2Cx->IC_DATA_CMD = *(pBuf) |
						(I2CReSTR << 10) |
						(I2CCmd << 8) |
						(I2CStop << 9);
}

/**
  * @brief  Slave sends single byte through the I2Cx peripheral after receiving read request of Master.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  Data: data to be transmitted.
  * @param  StopState: specifies whether a STOP is issued after the byte is sent.
  * @retval None
  */
void I2C_SlaveSend(I2C_TypeDef *I2Cx, u8 Data)
{
	I2Cx->IC_DATA_CMD = Data;
}

/**
  * @brief  Returns the most recent received data by the I2Cx peripheral.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @retval The value of the received data.
  */
u8 I2C_ReceiveData(I2C_TypeDef *I2Cx)
{
	/* Return the data in the DR register */
	return (u8)I2Cx->IC_DATA_CMD;
}

/**
  * @brief  Send data with special length in master mode through the I2Cx peripheral.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  pBuf: point to the data to be transmitted.
  * @param  len: the length of data that to be transmitted.
  * @retval None
  */
void I2C_MasterWrite(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len)
{
	u32 cnt = 0;

	/* Write in the DR register the data to be sent */
	for (cnt = 0; cnt < len; cnt++) {
		while ((I2C_CheckFlagState(I2Cx, I2C_BIT_TFNF)) == 0);

		if (cnt >= len - 1) {
			/*generate stop signal*/
			I2Cx->IC_DATA_CMD = (*pBuf++) | (1 << 9);
		} else {
			I2Cx->IC_DATA_CMD = (*pBuf++);
		}
	}

	while ((I2C_CheckFlagState(I2Cx, I2C_BIT_TFE)) == 0);
}

/**
  * @brief  Send data with special length in master mode through the I2Cx peripheral.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  pBuf: point to the data to be transmitted.
  * @param  len: the length of data that to be transmitted.
  * @retval: The length of data that have been sent to tx fifo
  */
u32 I2C_MasterWriteBrk(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len)
{
	u32 cnt = 0;

	/* Write in the DR register the data to be sent */
	for (cnt = 0; cnt < len; cnt++) {

		while ((I2C_CheckFlagState(I2Cx, I2C_BIT_TFNF)) == 0);

		if (cnt >= len - 1) {
			/*generate stop signal*/
			I2Cx->IC_DATA_CMD = (*pBuf++) | (1 << 9);
		} else {
			I2Cx->IC_DATA_CMD = (*pBuf++);
		}

		while ((I2C_CheckFlagState(I2Cx, I2C_BIT_TFE)) == 0) {
			if (I2C_GetRawINT(I2Cx) & I2C_BIT_TX_ABRT) {
				I2C_ClearAllINT(I2Cx);
				return cnt;
			}
		}
	}

	return cnt;
}

/**
  * @brief  Read data with special length in master mode through the I2Cx peripheral under DW IP.
  * @note   Under DW IP, master must send two times read cmd:
  *			flow is:
  * 		step 1: master request first data entry
  * 		step 2: slave send first data entry
  * 		step 3: master send seconed read cmd to ack first data and request second data
  * 		step 4: slave send second data
  * 		step 5: master rx full interrupt receive fisrt data and ack second data and request third data.
  * 		loop step 4 and step 5.
  * 		so last slave data have no ack, this is permitted by the spec.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  pBuf: point to the buffer to hold the received data.
  * @param  len: the length of data that to be received.
  * @retval None
  */
void I2C_MasterReadDW(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len)
{
	u32 cnt = 0;

	/* read in the DR register the data to be received */
	for (cnt = 0; cnt < len; cnt++) {
		if (cnt >= len - 1) {
			/* generate stop singal */
			I2Cx->IC_DATA_CMD = 0x0003 << 8;
		} else {
			I2Cx->IC_DATA_CMD = 0x0001 << 8;
		}

		/* read data */
		if (cnt > 0) {
			/* wait for I2C_FLAG_RFNE flag */
			while ((I2C_CheckFlagState(I2Cx, I2C_BIT_RFNE)) == 0);
			*pBuf++ = (u8)I2Cx->IC_DATA_CMD;
		}
	}

	/* recv last data and NACK */
	while ((I2C_CheckFlagState(I2Cx, I2C_BIT_RFNE)) == 0);
	*pBuf++ = (u8)I2Cx->IC_DATA_CMD;
}

/**
  * @brief  Read data with special length in master mode through the I2Cx peripheral under in-house IP.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  pBuf: point to the buffer to hold the received data.
  * @param  len: the length of data that to be received.
  * @retval The length of data that have received from rx fifo.
  */
u32 I2C_MasterRead(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len)
{
	u32 cnt = 0;

	/* read in the DR register the data to be received */
	for (cnt = 0; cnt < len; cnt++) {
		if (cnt >= len - 1) {
			/* generate stop singal */
			I2Cx->IC_DATA_CMD = 0x0003 << 8;
		} else {
			I2Cx->IC_DATA_CMD = 0x0001 << 8;
		}

		/* wait for I2C_FLAG_RFNE flag */
		while ((I2C_CheckFlagState(I2Cx, I2C_BIT_RFNE)) == 0) {
			if (I2C_GetRawINT(I2Cx) & I2C_BIT_TX_ABRT) {
				I2C_ClearAllINT(I2Cx);
				return cnt;
			}
		}
		*pBuf++ = (u8)I2Cx->IC_DATA_CMD;
	}

	return cnt;
}

/**
  * @brief  Send data with special length in slave mode through the I2Cx peripheral.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  pBuf: point to the data to be transmitted.
  * @param  len: the length of data that to be transmitted.
  * @retval None
  */
u32 I2C_SlaveWrite(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len)
{
	u32 cnt = 0;

	if ((I2Cx->IC_RAW_INTR_STAT & I2C_BIT_RX_DONE)) {
		I2C_ClearINT(I2Cx, I2C_BIT_R_RX_DONE);
	}

	for (cnt = 0; cnt < len; cnt++) {

		while (((I2Cx->IC_RAW_INTR_STAT & I2C_BIT_RD_REQ) == 0) && ((I2Cx->IC_RAW_INTR_STAT & I2C_BIT_RX_DONE) == 0));

		I2C_ClearINT(I2Cx, I2C_BIT_R_RD_REQ);

		/* Check I2C TX FIFO status */

		while (((I2C_CheckFlagState(I2Cx, I2C_BIT_TFNF))  == 0) && ((I2Cx->IC_RAW_INTR_STAT & I2C_BIT_RX_DONE) == 0));
		if (((I2Cx->IC_RAW_INTR_STAT & I2C_BIT_RX_DONE) != 0)) {
			return I2C_EARLY_RX_DONE;
		};

		I2Cx->IC_DATA_CMD = (*pBuf++);
	}
	while (((I2C_CheckFlagState(I2Cx, I2C_BIT_TFE)) == 0));
	I2C_ClearAllINT(I2Cx);
	return 0;
}


/**
  * @brief  Read data with special length in slave mode through the I2Cx peripheral.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  pBuf: point to the buffer to hold the received data.
  * @param  len: the length of data that to be received.
  * @retval None
  */
void I2C_SlaveRead(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len)
{
	u32 cnt = 0;

	for (cnt = 0; cnt < len; cnt++) {
		/* Check I2C RX FIFO status */
		while ((I2C_CheckFlagState(I2Cx, (I2C_BIT_RFNE | I2C_BIT_RFF))) == 0);

		*pBuf++ = (u8)I2Cx->IC_DATA_CMD;
	}
}
/**
  * @brief  Sends data and read data in master mode through the I2Cx peripheral.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  pWriteBuf: Byte to be transmitted.
  * @param  Writelen: Byte number to be transmitted.
  * @param  pReadBuf: Byte to be received.
  * @param  Readlen: Byte number to be received.
  * @retval None
  */
void I2C_MasterRepeatRead(I2C_TypeDef *I2Cx, u8 *pWriteBuf, u32 Writelen, u8 *pReadBuf, u32 Readlen)
{

	u32 cnt = 0;

	/* write in the DR register the data to be sent */
	for (cnt = 0; cnt < Writelen; cnt++) {
		while ((I2C_CheckFlagState(I2Cx, I2C_BIT_TFNF)) == 0);

		if (cnt >= Writelen - 1) {
			/*generate restart signal*/
			I2Cx->IC_DATA_CMD = (*pWriteBuf++) | (1 << 10);
		} else {
			I2Cx->IC_DATA_CMD = (*pWriteBuf++);
		}
	}

	/*Wait I2C TX FIFO not full*/
	while ((I2C_CheckFlagState(I2Cx, I2C_BIT_TFNF)) == 0);

	I2C_MasterRead(I2Cx, pReadBuf, Readlen);
}

/**
  * @brief  Enables or disables the specified I2C peripheral, this is one bit register.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  NewState: new state of the I2Cx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_Cmd(I2C_TypeDef *I2Cx, u8 NewState)
{
	if (NewState != DISABLE) {
		/* Enable the selected I2C peripheral */
		I2Cx->IC_ENABLE |= I2C_BIT_ENABLE;
	} else {
		/* Disable the selected I2C peripheral */
		I2Cx->IC_ENABLE &= ~I2C_BIT_ENABLE;
	}
}

/**
  * @brief  Read data with special length in master mode through the I2Cx peripheral under in-house IP.
  * @param  I2Cx: where I2Cx can be I2C0_DEV .
  * @param  pBuf: point to the buffer to hold the received data.
  * @param  len: the length of data that to be received.
  * @param  timeout_ms: specifies timeout time, unit is ms.
  * @retval the length of data read.
  */
u32 I2C_MasterRead_TimeOut(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len, u32 times)
{
	u32 cnt = 0;
	u32 InTimeoutCount = 0;

	/* read in the DR register the data to be received */
	for (cnt = 0; cnt < len; cnt++) {
		InTimeoutCount = times * 500;

		if (cnt >= len - 1) {
			/* generate stop singal */
			I2Cx->IC_DATA_CMD = I2C_BIT_CMD_RW | I2C_BIT_CMD_STOP;
		} else {
			I2Cx->IC_DATA_CMD = I2C_BIT_CMD_RW;
		}

		/* wait for I2C_FLAG_RFNE flag */
		while ((I2C_CheckFlagState(I2Cx, I2C_BIT_RFNE)) == 0) {
			if (I2C_GetRawINT(I2Cx) & I2C_BIT_TX_ABRT) {
				RTK_LOGE(TAG, "TX_ABRT\n");
				I2C_ClearAllINT(I2Cx);
				return cnt;
			}

			DelayUs(2);

			if (InTimeoutCount == 0) {
				RTK_LOGW(TAG, "MasterRead_TimeOut\n");
				return cnt;
			}
			InTimeoutCount--;
		}

		*pBuf++ = (u8)I2Cx->IC_DATA_CMD;
	}

	return cnt;
}

/**
  * @brief  Write data with special length in master mode through the I2Cx peripheral under in-house IP.
  * @param  I2Cx: where I2Cx can be I2C0_DEV.
  * @param  pBuf: point to the data to be transmitted.
  * @param  len: the length of data that to be received.
  * @param  timeout_ms: specifies timeout time, unit is ms.
  * @retval the length of data send.
  */
u32 I2C_MasterWrite_TimeOut(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len, u32 times)
{
	u32 cnt = 0;
	u32 InTimeoutCount = 0;

	/* Write in the DR register the data to be sent */
	for (cnt = 0; cnt < len; cnt++) {
		InTimeoutCount = times * 500;

		while ((I2C_CheckFlagState(I2Cx, I2C_BIT_TFNF)) == 0);

		if (cnt >= len - 1) {
			/*generate stop signal*/
			I2Cx->IC_DATA_CMD = (*pBuf++) | I2C_BIT_CMD_STOP;
		} else {
			I2Cx->IC_DATA_CMD = (*pBuf++);
		}

		while ((I2C_CheckFlagState(I2Cx, I2C_BIT_TFE)) == 0) {
			if (I2C_GetRawINT(I2Cx) & I2C_BIT_TX_ABRT) {
				RTK_LOGE(TAG, "TX_ABRT\n");
				I2C_ClearAllINT(I2Cx);
				return cnt;
			}

			DelayUs(2);

			if (InTimeoutCount == 0) {
				RTK_LOGW(TAG, "MasterWrite_TimeOut\n");
				return cnt;
			}
			InTimeoutCount--;
		}
	}

	return cnt;
}

/**
  * @brief  Master sends single byte through the I2Cx peripheral to detect slave device.
  * @param  obj: i2c object defined in application software.
  * @param  address: the address of slave that to be detected.
  * @param  timeout_ms: specifies timeout time, unit is ms.
  * @retval Slave ack condition:
  *          - 0: Slave available
  *          - -1: Slave not available
  */
s32 I2C_MasterSendNullData_TimeOut(I2C_TypeDef *I2Cx, int address, u32 timeout_ms)
{
	u8 I2CTemp = (u8)(address << 1);
	I2C_MasterSendNullData(I2Cx, &I2CTemp, 0, 1, 0);

	DelayMs(timeout_ms);

	if (I2C_GetRawINT(I2Cx) & I2C_BIT_TX_ABRT) {
		I2C_ClearAllINT(I2Cx);

		/* Wait for i2c enter trap state from trap_stop state*/
		DelayUs(100);
		I2C_Cmd(I2Cx, DISABLE);
		I2C_Cmd(I2Cx, ENABLE);

		return -1;
	}
	return 0;
}

/**
  * @brief  I2C TX/RX intrerrupt handler.
  * @param  I2C_SemStruct: structure containing the function of acquiring and releasing semaphores.
  * @retval None
  * @note This function has been defined as weak in the SDK, and users can redefine it according to their needs.
  */
__weak u32 I2C_ISRHandle(I2C_IntModeCtrl *I2C_SemStruct)
{
	I2C_TypeDef *I2Cx = I2C_SemStruct->I2Cx;
	u32 intr_status = I2C_GetINT(I2Cx);

	assert_param(I2C_SemStruct->I2CWaitSem != NULL);
	assert_param(I2C_SemStruct->I2CSendSem != NULL);

	/* I2C TX Abort | Empty Intr */
	if (intr_status & (I2C_BIT_R_TX_ABRT | I2C_BIT_R_TX_EMPTY)) {
		I2C_ClearINT(I2Cx, intr_status & (I2C_BIT_R_TX_ABRT | I2C_BIT_R_TX_EMPTY));
		I2C_INTConfig(I2Cx, (I2C_BIT_R_TX_ABRT | I2C_BIT_R_TX_EMPTY), DISABLE);

		I2C_SemStruct->I2CSendSem(TRUE);
	}

	/* I2C RX Over | RX FULL Intr */
	if (intr_status & (I2C_BIT_R_TX_ABRT | I2C_BIT_R_RX_OVER | I2C_BIT_RX_FULL)) {
		I2C_ClearINT(I2Cx, intr_status & (I2C_BIT_R_TX_ABRT | I2C_BIT_R_RX_OVER | I2C_BIT_RX_FULL));
		I2C_INTConfig(I2Cx, (I2C_BIT_R_TX_ABRT | I2C_BIT_R_RX_OVER | I2C_BIT_RX_FULL), DISABLE);

		I2C_SemStruct->I2CSendSem(FALSE);
	}

	return 0;
}

/**
  * @brief  Master sends data in interrupt mode.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  I2C_SemStruct: structure containing the function of acquiring and releasing semaphores.
  * @param  pBuf: point to the data to be transmitted.
  * @param  len: the length of data that to be transmitted.
  * @retval remaining to be transferred count.
  */
u32 I2C_MasterWriteInt(I2C_TypeDef *I2Cx, I2C_IntModeCtrl *I2C_SemStruct, u8 *pBuf, u32 len)
{
	u32 cnt = 0;

	assert_param(I2C_SemStruct->I2CWaitSem != NULL);

	/* Write in the DR register the data to be sent */
	for (cnt = len; cnt > 0;) {
		if (I2Cx->IC_TXFLR < I2C_TRX_BUFFER_DEPTH) {
			if (cnt <= 1) {
				/*generate stop signal*/
				I2Cx->IC_DATA_CMD = (*pBuf++) | I2C_BIT_CMD_STOP;
			} else {
				I2Cx->IC_DATA_CMD = (*pBuf++);
			}
			cnt--;
		} else {
			I2C_INTConfig(I2Cx, (I2C_BIT_R_TX_ABRT | I2C_BIT_R_TX_EMPTY), ENABLE);
			I2C_SemStruct->I2CWaitSem(TRUE);
		}

		if (I2C_GetRawINT(I2Cx) & I2C_BIT_TX_ABRT) {
			I2C_ClearAllINT(I2Cx);
			return (len - cnt);
		}
	}
	return (len - cnt);
}

/**
  * @brief  Master receives data in interrupt mode.
  * @param  I2Cx: where I2Cx can be I2C0_DEV, I2C1_DEV and I2C2_DEV.
  * @param  I2C_SemStruct: structure containing the function of acquiring and releasing semaphores.
  * @param  pBuf:  point to the buffer to hold the received data.
  * @param  len: the length of data that to be received.
  * @retval received count.
  */
u32 I2C_MasterReadInt(I2C_TypeDef *I2Cx, I2C_IntModeCtrl *I2C_SemStruct, u8 *pBuf, u32 len)
{
	u32 left_cnt = len;
	u32 trigger_cnt;
	u32 rcvd = 0;

	assert_param(I2C_SemStruct->I2CWaitSem != NULL);

	while (left_cnt > 0) {
		/* First Tx I2C_TRX_BUFFER_DEPTH triggers, and I2C_TRX_BUFFER_DEPTH will be received */
		for (trigger_cnt = 1; trigger_cnt <= I2C_TRX_BUFFER_DEPTH; trigger_cnt++) {
			if (left_cnt == 1) {
				I2Cx->IC_DATA_CMD = I2C_BIT_CMD_RW | I2C_BIT_CMD_STOP;
				left_cnt--;
				break;
			} else {
				I2Cx->IC_DATA_CMD = I2C_BIT_CMD_RW;
				left_cnt--;
			}
		}

		/* change rx full thresh based on trigger number */
		if (trigger_cnt < I2C_TRX_BUFFER_DEPTH) {
			I2Cx->IC_RX_TL = trigger_cnt - 1;
		} else {
			I2Cx->IC_RX_TL = I2C_TRX_BUFFER_DEPTH - 1;
		}

		/* wait Semaphore */
		I2C_INTConfig(I2Cx, (I2C_BIT_R_TX_ABRT | I2C_BIT_M_RX_FULL | I2C_BIT_M_RX_OVER), ENABLE);
		I2C_SemStruct->I2CWaitSem(FALSE);

		/* read IC_RXFLR to empty */
		while (I2Cx->IC_RXFLR) {// Read rx fifo until it's empty
			*pBuf++ = (u8)I2Cx->IC_DATA_CMD;
			rcvd++;
		};

		if (I2C_GetRawINT(I2Cx) & I2C_BIT_TX_ABRT) {
			I2C_ClearAllINT(I2Cx);
			return rcvd;
		}
	}
	return rcvd;
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
