/**
  ******************************************************************************
  * @file    rtl8721dhp_audio.c
  * @author
  * @version V1.0.0
  * @date    2017-12-13
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Audio codec peripheral:
  *           - SPORT Initialization
  *           - SPORT parameters management
  *           - Data transfers configuration
  *           - GDMA configuration
  *
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#include "ameba_soc.h"

SP_RegTypeDef SP_RegFlag[2] = {{0, 0, 0}, {0, 0, 0}};

const AUDIO_DevTable AUDIO_DEV_TABLE[2] = {
	{
		AUDIO_SPORT0_DEV, GDMA_HANDSHAKE_INTERFACE_SPORT0F0_TX, GDMA_HANDSHAKE_INTERFACE_SPORT0F0_RX,
		GDMA_HANDSHAKE_INTERFACE_SPORT0F1_TX, GDMA_HANDSHAKE_INTERFACE_SPORT0F1_RX
	},	 /*audio sport0 */
	{
		AUDIO_SPORT1_DEV, GDMA_HANDSHAKE_INTERFACE_SPORT1F0_TX, GDMA_HANDSHAKE_INTERFACE_SPORT1F0_RX,
		GDMA_HANDSHAKE_INTERFACE_SPORT1F1_TX, GDMA_HANDSHAKE_INTERFACE_SPORT1F1_RX
	},	 /*audio sport1 */
};

/**
  * @brief  Fills each SP_StructInit member with its default value.
  * @param  SP_StructInit: pointer to an SP_StructInit structure which will be
  *         initialized.
  * @retval None
  */
void AUDIO_SP_StructInit(SP_InitTypeDef *SP_InitStruct)
{
	SP_InitStruct->SP_SelDataFormat = SP_DF_I2S;
	SP_InitStruct->SP_SelWordLen = SP_TXWL_16;
	SP_InitStruct->SP_SelChLen = SP_TXCL_32;
	SP_InitStruct->SP_SelCh = SP_TX_CH_LR;
	SP_InitStruct->SP_SR = SP_48K;
	SP_InitStruct->SP_SelTDM = SP_TX_NOTDM;
	SP_InitStruct->SP_SelFIFO = SP_TX_FIFO2;
	SP_InitStruct->SP_SelI2SMonoStereo = SP_CH_STEREO;
	SP_InitStruct->SP_SetMultiIO = SP_TX_MULTIIO_DIS;
	SP_InitStruct->SP_SelClk = CKSL_I2S_XTAL40M;
	SP_InitStruct->SP_Bclk = (32 * ((SP_InitStruct->SP_SelTDM) + 1) * 2 * (SP_InitStruct->SP_SR));
	SP_InitStruct->SP_Fix_Bclk = DISABLE;
	SP_InitStruct->SP_Sel_FixBclk = FIX_BCLK_20M;
}


/**
  * @brief  register sport if this sport is used.
  * @param  index: 0 ~ 1.
  * @retval value: _TRUE/_FALSE
*/
BOOL AUDIO_SP_Register(u32 index, u32 direction, SP_InitTypeDef *SP_InitStruct)
{

	if (direction == SP_DIR_TX) {

		if (SP_RegFlag[index].SP_DIR_TX_flag) {
			DBG_8195A("SPORT TX has been registered!\n");
			return 0;
		} else {
			if (SP_RegFlag[index].SP_DIR_RX_flag) {
				if (SP_RegFlag[index].SP_Bclk == SP_InitStruct->SP_Bclk) {
					SP_RegFlag[index].SP_DIR_TX_flag = ENABLE;
					return 1;

				} else {
					if (SP_InitStruct->SP_Fix_Bclk) {
						return 1;
					} else {
						DBG_8195A("SPORT RX Bclk can't be changed!\n");
						return 0;
					}
				}
			} else {
				SP_RegFlag[index].SP_DIR_TX_flag = ENABLE;
				SP_RegFlag[index].SP_Bclk = SP_InitStruct->SP_Bclk;
				return 1;
			}
		}
	} else {

		if (SP_RegFlag[index].SP_DIR_RX_flag) {
			DBG_8195A("SPORT RX has been registered!\n");
			return 0;
		} else {
			if (SP_RegFlag[index].SP_DIR_TX_flag) {
				if (SP_RegFlag[index].SP_Bclk == SP_InitStruct->SP_Bclk) {
					SP_RegFlag[index].SP_DIR_RX_flag = ENABLE;
					return 1;

				} else {
					if (SP_InitStruct->SP_Fix_Bclk) {
						return 1;
					} else {
						DBG_8195A("SPORT TX Bclk can't be changed!\n");
						return 0;
					}
				}
			} else {
				SP_RegFlag[index].SP_DIR_RX_flag = ENABLE;
				SP_RegFlag[index].SP_Bclk = SP_InitStruct->SP_Bclk;
				return 1;
			}
		}
	}
}

/**
* @brief Unregister sport.
* @param index: 0 ~ 3.
* @retval value: _TRUE/_FALSE
*/
void AUDIO_SP_Unregister(u32 index, u32 direction)
{
	assert_param(IS_SP_SET_DIR(direction));

	if (direction == SP_DIR_TX) {
		SP_RegFlag[index].SP_DIR_TX_flag = DISABLE;
	} else {
		SP_RegFlag[index].SP_DIR_RX_flag = DISABLE;
	}

}


/**
  * @brief  Reset SPORT module.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @retval None
  */
void AUDIO_SP_Reset(AUDIO_SPORT_TypeDef *SPORTx)
{
	SPORTx->SP_CTRL0 |= SP_BIT_RESET;
	SPORTx->SP_CTRL0 &= ~ SP_BIT_RESET;
}

u32 AUDIO_SP_GetTXChnLen(AUDIO_SPORT_TypeDef *SPORTx)
{
	u32 chn_len;
	u32 len_tx = ((SPORTx->SP_FORMAT) & SP_MASK_CH_LEN_SEL_TX) >> 24;

	switch (len_tx) {
	case 0:
		chn_len = 16;
		break;
	case 1:
		chn_len = 20;
		break;
	case 2:
		chn_len = 24;
		break;
	default:
		chn_len = 32;
		break;
	}

	return chn_len;
}

u32 AUDIO_SP_GetRXChnLen(AUDIO_SPORT_TypeDef *SPORTx)
{
	u32 chn_len;
	u32 len_rx = ((SPORTx->SP_FORMAT) & SP_MASK_CH_LEN_SEL_RX) >> 28;

	switch (len_rx) {
	case 0:
		chn_len = 16;
		break;
	case 1:
		chn_len = 20;
		break;
	case 2:
		chn_len = 24;
		break;
	default:
		chn_len = 32;
		break;
	}

	return chn_len;
}


/**
  * @brief  Set the AUDIO SPORT BLCK divider factor.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @param  mi: the denominator of clock divider.
  * @param  mi: the nominator of clock divider.
  * @note  This api is used only when clock source is 40MHz.
  * @note1  BLCK = 40MHz*(ni/mi).
  * @note2  SP_CLK_MI_NI_UPDATE bit is used to update mi and ni to get the new clock rate.
  *               This bit will be reset automatically when update is done.
  * @retval None
  */
void AUDIO_SP_SetTXClkDiv(u32 index, u32 clock, u32 sr, u32 tdm, u32 chn_len)
{
	u32 Tmp;
	u32 MI = 0;
	u32 NI = 0;
	u32 Bclk_div;

	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	switch (clock) {
	default:
		if ((!(sr % 4)) && ((sr * 1000) % 11025)) {  //48k系列
			MI = 1250;
			NI = (chn_len / 4) * (tdm + 1) * (sr / 4);
		} else {
			if (chn_len == 16 || chn_len == 32) {
				MI = 50000;
				NI = (441 * (chn_len / 4) * (tdm + 1) * (sr / 11025)) / 4;
			} else if (chn_len == 20) {
				MI = 40000;
				NI = (441 * (chn_len / 4) * (tdm + 1) * (sr / 11025)) / 5;
			} else {
				MI = 60000;
				NI = (441 * 3 * (chn_len / 4) * (tdm + 1) * (sr / 11025)) / 10;
			}
		}
		break;

	case CKSL_I2S_CPUPLL:
		//TBD
		break;
	case CKSL_I2S_DSPPLL:
		//TBD
		break;

	}

	switch (chn_len) {
	case SP_CL_16:
		Bclk_div = 32;
		break;
	case SP_CL_20:
		Bclk_div = 40;
		break;
	case SP_CL_24:
		Bclk_div = 48;
		break;
	default:
		Bclk_div = 64;
		break;

	}

	assert_param(NI <= 0xFFFF);
	assert_param(MI <= 0xFFFF);

	Tmp = SPORTx->SP_SR_TX_BCLK ;
	Tmp &= ~(SP_MASK_TX_MI | SP_MASK_TX_NI | SP_BIT_TX_MI_NI_UPDATE);
	Tmp |= (SP_TX_MI(MI) | SP_TX_NI(NI) | SP_BIT_TX_MI_NI_UPDATE);
	SPORTx->SP_SR_TX_BCLK  = Tmp;

	if (tdm == SP_TX_NOTDM) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_TX_BCLK_DIV_RATIO);
		Tmp |= SP_TX_BCLK_DIV_RATIO(Bclk_div - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else if (tdm == SP_TX_TDM4) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_TX_BCLK_DIV_RATIO);
		Tmp |= SP_TX_BCLK_DIV_RATIO(Bclk_div * 2 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else if (tdm == SP_TX_TDM6) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_TX_BCLK_DIV_RATIO);
		Tmp |= SP_TX_BCLK_DIV_RATIO(Bclk_div * 3 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_TX_BCLK_DIV_RATIO);
		Tmp |= SP_TX_BCLK_DIV_RATIO(Bclk_div * 4 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	}

}

void AUDIO_SP_SetRXClkDiv(u32 index, u32 clock, u32 sr, u32 tdm, u32 chn_len)
{
	u32 Tmp;
	u32 NI = 0;
	u32 MI = 0;
	u32 Bclk_div;

	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	switch (clock) {
	default:
		if ((!(sr % 4)) && ((sr * 1000) % 11025)) {  //48k系列
			MI = 1250;
			NI = (chn_len / 4) * (tdm + 1) * (sr / 4);
		} else {
			if (chn_len == 16 || chn_len == 32) {
				MI = 50000;
				NI = (441 * (chn_len / 4) * (tdm + 1) * (sr / 11025)) / 4;
			} else if (chn_len == 20) {
				MI = 40000;
				NI = (441 * (chn_len / 4) * (tdm + 1) * (sr / 11025)) / 5;
			} else {
				MI = 60000;
				NI = (441 * 3 * (chn_len / 4) * (tdm + 1) * (sr / 11025)) / 10;
			}
		}
		break;

	case CKSL_I2S_CPUPLL:
		//TBD
		break;
	case CKSL_I2S_DSPPLL:
		//TBD
		break;

	}

	assert_param(MI <= 0xFFFF);
	assert_param(NI <= 0xFFFF);

	Tmp = SPORTx->SP_RX_BCLK ;
	Tmp &= ~(SP_MASK_RX_MI | SP_MASK_RX_NI | SP_BIT_RX_MI_NI_UPDATE);
	Tmp |= (SP_RX_MI(MI) | SP_RX_NI(NI) | SP_BIT_RX_MI_NI_UPDATE);
	SPORTx->SP_RX_BCLK  = Tmp;

	switch (chn_len) {
	case SP_CL_16:
		Bclk_div = 32;
		break;
	case SP_CL_20:
		Bclk_div = 40;
		break;
	case SP_CL_24:
		Bclk_div = 48;
		break;
	default:
		Bclk_div = 64;
		break;
	}

	if (tdm == SP_RX_NOTDM) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_RX_BCLK_DIV_RATIO);
		Tmp |= SP_RX_BCLK_DIV_RATIO(Bclk_div - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else if (tdm == SP_RX_TDM4) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_RX_BCLK_DIV_RATIO);
		Tmp |= SP_RX_BCLK_DIV_RATIO(Bclk_div * 2 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else if (tdm == SP_TX_TDM6) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_RX_BCLK_DIV_RATIO);
		Tmp |= SP_RX_BCLK_DIV_RATIO(Bclk_div * 3 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_RX_BCLK_DIV_RATIO);
		Tmp |= SP_RX_BCLK_DIV_RATIO(Bclk_div * 4 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	}
}


/**
  * @brief  Initializes the AUDIO SPORT registers according to the specified parameters
  *         in SP_InitStruct. *FIFO_0 and FIFO_1 have the same configuration.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @param  SP_InitStruct: pointer to an SP_InitTypeDef structure that contains
  *         the configuration information for the specified AUDIO SPORT peripheral
  * @note   AUDIO SPORT has two clock sources, one is 98.304MHz, the other is 45.1584MHz.
  *         BIT_CTRL_CTLX_I2S_CLK_SRC can used to select the clock source.
  * @retval None
  */
void AUDIO_SP_Init(u32 index, u32 direction, SP_InitTypeDef *SP_InitStruct)
{
	u32 chn_len;
	assert_param(IS_SP_SET_DIR(direction));

	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	/* Set SP_REG_MUX to 0XFFFFFFFF */
	if (SPORTx != AUDIO_SPORT0_DEV) {
		SPORTx->SP_REG_MUX |= SP_MASK_REG_MUX;
	}

	/* Enable MCLK */
	SPORTx->SP_CTRL1 |= SP_BIT_ENABLE_MCLK;

	if (direction == SP_DIR_TX) {

		/* Check the parameters*/
		assert_param(IS_SP_DATA_FMT(SP_InitStruct->SP_SelDataFormat));
		assert_param(IS_SP_TX_WL(SP_InitStruct->SP_SelWordLen));
		assert_param(IS_SP_TXCH_LEN(SP_InitStruct->SP_SelChLen));
		assert_param(IS_SP_SEL_TX_CH(SP_InitStruct->SP_SelCh));
		assert_param(IS_SP_SEL_TX_TDM(SP_InitStruct->SP_SelTDM));
		assert_param(IS_SP_SEL_TX_FIFO(SP_InitStruct->SP_SelFIFO));
		assert_param(IS_SP_CHN_NUM(SP_InitStruct->SP_SelI2SMonoStereo));
		assert_param(IS_SP_SET_TX_MULTIIO(SP_InitStruct->SP_SetMultiIO));

		switch (SP_InitStruct->SP_SelChLen) {
		case 0:
			chn_len = 16;
			break;
		case 1:
			chn_len = 20;
			break;
		case 2:
			chn_len = 24;
			break;
		default:
			chn_len = 32;
			break;
		}

		SP_InitStruct->SP_Bclk = (chn_len * ((SP_InitStruct->SP_SelTDM) + 1) * 2 * (SP_InitStruct->SP_SR));
		assert_param(AUDIO_SP_Register(index, direction, SP_InitStruct));

		/* Configure parameters: disable TX, AUDIO SPORT mode */
		AUDIO_SP_TXStart(SPORTx, DISABLE);

		/* Configure FIFO0 parameters: data format, word length, channel number, etc */
		SPORTx->SP_FORMAT &= ~(SP_BIT_TRX_SAME_LENGTH);
		SPORTx->SP_CTRL0 &= ~(SP_MASK_DATA_LEN_SEL_TX_0 | SP_MASK_DATA_FORMAT_SEL_TX | SP_MASK_SEL_I2S_TX_CH);
		SPORTx->SP_CTRL0 |= ((SP_DATA_LEN_SEL_TX_0(SP_InitStruct->SP_SelWordLen)) | (SP_DATA_FORMAT_SEL_TX(SP_InitStruct->SP_SelDataFormat))
							 | (SP_SEL_I2S_TX_CH(SP_InitStruct->SP_SelCh)));

		if (SP_InitStruct->SP_SelI2SMonoStereo == SP_CH_STEREO) {
			SPORTx->SP_CTRL0 &= ~ SP_BIT_EN_I2S_MONO_TX_0;
			SPORTx->SP_DIRECT_CTRL1 &= ~ SP_BIT_EN_I2S_MONO_TX_1;
		} else {
			SPORTx->SP_CTRL0 |= (SP_BIT_EN_I2S_MONO_TX_0);
			SPORTx->SP_DIRECT_CTRL1 |= (SP_BIT_EN_I2S_MONO_TX_1);
		}

		/* Configure parameters:Channel Length, data format*/
		SPORTx->SP_FORMAT &= ~(SP_MASK_CH_LEN_SEL_TX | SP_BIT_TRX_SAME_CH_LEN | SP_BIT_TRX_SAME_CH);
		SPORTx->SP_FORMAT |= (SP_CH_LEN_SEL_TX(SP_InitStruct->SP_SelChLen)) ;

		/* Configure FIFO1 parameters*/
		SPORTx->SP_DIRECT_CTRL1 &= ~(SP_MASK_DATA_LEN_SEL_TX_1);
		SPORTx->SP_DIRECT_CTRL1 |= (SP_DATA_LEN_SEL_TX_1(SP_InitStruct->SP_SelWordLen));

		/* Configure parameters: TX TDM mode */
		SPORTx->SP_CTRL0 &= ~SP_MASK_TDM_MODE_SEL_TX;
		SPORTx->SP_CTRL0 |= SP_TDM_MODE_SEL_TX(SP_InitStruct->SP_SelTDM);
		SPORTx->SP_CTRL1 &= ~SP_BIT_MULTI_IO_EN_TX;

		AUDIO_SP_SetTXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_InitStruct->SP_SelTDM, chn_len);

		/* Configure TX FIFO Channel */
		if (SP_InitStruct->SP_SelFIFO == SP_TX_FIFO2) {
			SPORTx->SP_CTRL1 |= SP_BIT_TX_FIFO_0_REG_0_EN;
		} else if (SP_InitStruct->SP_SelFIFO == SP_TX_FIFO4) {
			SPORTx->SP_CTRL1 |= SP_BIT_TX_FIFO_0_REG_0_EN | SP_BIT_TX_FIFO_0_REG_1_EN;
		} else if (SP_InitStruct->SP_SelFIFO == SP_TX_FIFO6) {
			SPORTx->SP_CTRL1 |= (SP_BIT_TX_FIFO_0_REG_0_EN | SP_BIT_TX_FIFO_0_REG_1_EN | SP_BIT_TX_FIFO_1_REG_0_EN);
		} else if (SP_InitStruct->SP_SelFIFO == SP_TX_FIFO8) {
			SPORTx->SP_CTRL1 |= (SP_BIT_TX_FIFO_0_REG_0_EN | SP_BIT_TX_FIFO_0_REG_1_EN | SP_BIT_TX_FIFO_1_REG_0_EN | SP_BIT_TX_FIFO_1_REG_1_EN);
		}

		/* Configure SPORT parameters: TX MULTI_IO Mode*/
		if (SP_InitStruct->SP_SetMultiIO == SP_TX_MULTIIO_EN) {
			SPORTx->SP_CTRL1 |= SP_BIT_MULTI_IO_EN_TX;
			AUDIO_SP_SetTXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_TX_NOTDM, chn_len);

		} else {
			SPORTx->SP_CTRL1 &= ~(SP_BIT_MULTI_IO_EN_TX);
		}

		/*set DMA burstsize: 8*/
		SPORTx->SP_TX_LRCLK &= ~(SP_MASK_TXDMA_BURSTSIZE);
		SPORTx->SP_TX_LRCLK |= SP_TXDMA_BURSTSIZE(8);

		if (SP_InitStruct->SP_Fix_Bclk) {
			SPORTx->SP_FORMAT |= SP_BIT_FIXED_BCLK;
			SPORTx->SP_FORMAT |= SP_BIT_FIXED_BCLK_SEL;
			SPORTx->SP_FORMAT &= ~SP_BIT_TRX_SAME_FS;
		}


	} else {

		assert_param(IS_SP_DATA_FMT(SP_InitStruct->SP_SelDataFormat));
		assert_param(IS_SP_RX_WL(SP_InitStruct->SP_SelWordLen));
		assert_param(IS_SP_RXCH_LEN(SP_InitStruct->SP_SelChLen));
		assert_param(IS_SP_SEL_RX_CH(SP_InitStruct->SP_SelCh));
		assert_param(IS_SP_SEL_RX_TDM(SP_InitStruct->SP_SelTDM));
		assert_param(IS_SP_SEL_RX_FIFO(SP_InitStruct->SP_SelFIFO));
		assert_param(IS_SP_CHN_NUM(SP_InitStruct->SP_SelI2SMonoStereo));
		assert_param(IS_SP_SET_RX_MULTIIO(SP_InitStruct->SP_SetMultiIO));

		switch (SP_InitStruct->SP_SelChLen) {
		case 0:
			chn_len = 16;
			break;
		case 1:
			chn_len = 20;
			break;
		case 2:
			chn_len = 24;
			break;
		default:
			chn_len = 32;
			break;
		}

		SP_InitStruct->SP_Bclk = (chn_len * ((SP_InitStruct->SP_SelTDM) + 1) * 2 * (SP_InitStruct->SP_SR));
		assert_param(AUDIO_SP_Register(index, direction, SP_InitStruct));

		/* Configure parameters: disable RX, AUDIO SPORT mode */
		AUDIO_SP_RXStart(SPORTx, DISABLE);

		/* Configure FIFO0 parameters: data format, word length, channel number, etc */
		SPORTx->SP_CTRL0 &= ~(SP_MASK_SEL_I2S_RX_CH | SP_MASK_TDM_MODE_SEL_RX);
		SPORTx->SP_CTRL0 |= (SP_SEL_I2S_RX_CH(SP_InitStruct->SP_SelCh));

		if (SP_InitStruct->SP_SelI2SMonoStereo == SP_CH_STEREO) {
			SPORTx->SP_FORMAT &= ~ SP_BIT_EN_I2S_MONO_RX_0;
			SPORTx->SP_DIRECT_CTRL1 &= ~ SP_BIT_EN_I2S_MONO_RX_1;
		} else {
			SPORTx->SP_FORMAT |= (SP_BIT_EN_I2S_MONO_RX_0);
			SPORTx->SP_DIRECT_CTRL1 |= (SP_BIT_EN_I2S_MONO_RX_1);
		}

		/* Configure parameters:Channel Length, data format*/
		SPORTx->SP_FORMAT &= ~(SP_MASK_CH_LEN_SEL_RX | SP_MASK_DATA_FORMAT_SEL_RX | SP_MASK_DATA_LEN_SEL_RX_0 |
							   SP_BIT_TRX_SAME_CH_LEN | SP_BIT_TRX_SAME_CH | SP_BIT_TRX_SAME_LENGTH);
		SPORTx->SP_FORMAT |= ((SP_CH_LEN_SEL_RX(SP_InitStruct->SP_SelChLen)) | (SP_DATA_FORMAT_SEL_RX(SP_InitStruct->SP_SelDataFormat))
							  | (SP_DATA_LEN_SEL_RX_0(SP_InitStruct->SP_SelWordLen)));

		/* Configure FIFO1 parameters*/
		SPORTx->SP_DIRECT_CTRL1 &= ~(SP_MASK_DATA_LEN_SEL_RX_1);
		SPORTx->SP_DIRECT_CTRL1 |= (SP_DATA_LEN_SEL_RX_1(SP_InitStruct->SP_SelWordLen));

		/* Configure parameters: RX TDM mode */
		SPORTx->SP_CTRL0 |= SP_TDM_MODE_SEL_RX(SP_InitStruct->SP_SelTDM);
		SPORTx->SP_CTRL1 &= ~SP_BIT_MULTI_IO_EN_RX;

		if (SP_InitStruct->SP_Fix_Bclk) {
			SPORTx->SP_FORMAT |= SP_BIT_FIXED_BCLK;
			SPORTx->SP_FORMAT |= SP_BIT_FIXED_BCLK_SEL;
			SPORTx->SP_FORMAT &= ~SP_BIT_TRX_SAME_FS;
			AUDIO_SP_SetRXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_InitStruct->SP_SelTDM, chn_len);
		} else {
			AUDIO_SP_SetTXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_InitStruct->SP_SelTDM, chn_len);
			AUDIO_SP_SetRXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_InitStruct->SP_SelTDM, chn_len);
		}


		/* Configure RX FIFO Channel */
		if (SP_InitStruct->SP_SelFIFO == SP_RX_FIFO2) {
			SPORTx->SP_CTRL1 |= SP_BIT_RX_FIFO_0_REG_0_EN;
		} else if (SP_InitStruct->SP_SelFIFO == SP_RX_FIFO4) {
			SPORTx->SP_CTRL1 |= SP_BIT_RX_FIFO_0_REG_0_EN | SP_BIT_RX_FIFO_0_REG_1_EN;
		} else if (SP_InitStruct->SP_SelFIFO == SP_RX_FIFO6) {
			SPORTx->SP_CTRL1 |= (SP_BIT_RX_FIFO_0_REG_0_EN | SP_BIT_RX_FIFO_0_REG_1_EN | SP_BIT_RX_FIFO_1_REG_0_EN);
		} else if (SP_InitStruct->SP_SelFIFO == SP_RX_FIFO8) {
			SPORTx->SP_CTRL1 |= (SP_BIT_RX_FIFO_0_REG_0_EN | SP_BIT_RX_FIFO_0_REG_1_EN | SP_BIT_RX_FIFO_1_REG_0_EN | SP_BIT_RX_FIFO_1_REG_1_EN);
		}

		/* Configure SPORT parameters:RX MULTI_IO Mode*/
		if (SP_InitStruct->SP_SetMultiIO == SP_RX_MULTIIO_EN) {

			if (SP_InitStruct->SP_Fix_Bclk) {
				SPORTx->SP_FORMAT |= SP_BIT_FIXED_BCLK;
				SPORTx->SP_FORMAT |= SP_BIT_FIXED_BCLK_SEL;
				SPORTx->SP_FORMAT &= ~SP_BIT_TRX_SAME_FS;
				AUDIO_SP_SetRXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_RX_NOTDM, chn_len);
			} else {
				AUDIO_SP_SetTXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_RX_NOTDM, chn_len);
				AUDIO_SP_SetRXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_RX_NOTDM, chn_len);
			}

			SPORTx->SP_CTRL1 |= SP_BIT_MULTI_IO_EN_RX;
		} else {
			SPORTx->SP_CTRL1 &= ~(SP_BIT_MULTI_IO_EN_RX);
		}

		/*set DMA burstsize: 8*/
		SPORTx->SP_TX_LRCLK &= ~(SP_MASK_RXDMA_BUSRTSIZE);
		SPORTx->SP_TX_LRCLK |= SP_RXDMA_BUSRTSIZE(8);

	}

}

/**
  * @brief  Start or stop SPORT TX.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @param  NewState: new state of the SPORT TX.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void AUDIO_SP_TXStart(AUDIO_SPORT_TypeDef *SPORTx, u32 NewState)
{
	if (NewState == ENABLE) {
		SPORTx->SP_CTRL0 &= ~ SP_BIT_TX_DISABLE;
		SPORTx->SP_CTRL0 |= SP_BIT_START_TX;
	} else {
		SPORTx->SP_CTRL0 |= SP_BIT_TX_DISABLE;
		SPORTx->SP_CTRL0 &= ~ SP_BIT_START_TX;
	}
}

/**
  * @brief  Start or stop SPORT RX.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @param  NewState: new state of the SPORT RX.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void AUDIO_SP_RXStart(AUDIO_SPORT_TypeDef *SPORTx, u32 NewState)
{
	if (NewState == ENABLE) {
		SPORTx->SP_CTRL0 &= ~ SP_BIT_RX_DISABLE;
		SPORTx->SP_CTRL0 |= SP_BIT_START_RX;
	} else {
		SPORTx->SP_CTRL0 |= SP_BIT_RX_DISABLE;
		SPORTx->SP_CTRL0 &= ~ SP_BIT_START_RX;
	}
}

/**
  * @brief  SPORT and GDMA handshake on or off.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @param  NewState: new state of the handshake of GDMA-SPORT.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void AUDIO_SP_DmaCmd(AUDIO_SPORT_TypeDef *SPORTx, u32 NewState)
{
	if (NewState == ENABLE) {
		SPORTx->SP_CTRL0 &= ~ SP_BIT_DSP_CTL_MODE;
	} else {
		SPORTx->SP_CTRL0 |= SP_BIT_DSP_CTL_MODE;
	}
}

/**
  * @brief  Set SPORT and GDMA handshake on or off.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @retval None
  */
void AUDIO_SP_SetSelfLPBK(AUDIO_SPORT_TypeDef *SPORTx)
{
	SPORTx->SP_CTRL0 |= SP_BIT_LOOPBACK;
}

/**
  * @brief  Set the AUDIO SPORT word length.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @param  SP_TX_WordLen: the value of word length.
  *          This parameter can be one of the following values:
  *            @arg SP_TXWL_16: sample bit is 16 bit
  *            @arg SP_TXWL_20: sample bit is 20 bit
  *            @arg SP_TXWL_24: sample bit is 24 bit
  *            @arg SP_TXWL_32: sample bit is 32 bit
  * @retval None
  */

void AUDIO_SP_SetTXWordLen(AUDIO_SPORT_TypeDef *SPORTx, u32 SP_TX_WordLen)
{
	assert_param(IS_SP_TX_WL(SP_TX_WordLen));

	SPORTx->SP_CTRL0 &= ~(SP_MASK_DATA_LEN_SEL_TX_0);
	SPORTx->SP_CTRL0 |= SP_DATA_LEN_SEL_TX_0(SP_TX_WordLen);

	SPORTx->SP_DIRECT_CTRL1 &= ~(SP_MASK_DATA_LEN_SEL_TX_1);
	SPORTx->SP_DIRECT_CTRL1 |=  SP_DATA_LEN_SEL_TX_1(SP_TX_WordLen);
}

/**
  * @brief  Set the AUDIO SPORT word length.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @param  SP_RX_WordLen: the value of word length.
  *          This parameter can be one of the following values:
  *            @arg SP_RXWL_16: sample bit is 16 bit
  *            @arg SP_RXWL_20: sample bit is 20 bit
  *            @arg SP_RXWL_24: sample bit is 24 bit
  *            @arg SP_RXWL_32: sample bit is 32 bit
  * @retval None
  */

void AUDIO_SP_SetRXWordLen(AUDIO_SPORT_TypeDef *SPORTx, u32 SP_RX_WordLen)
{
	assert_param(IS_SP_RX_WL(SP_RX_WordLen));

	SPORTx->SP_FORMAT &= ~(SP_MASK_DATA_LEN_SEL_RX_0);
	SPORTx->SP_FORMAT |= SP_DATA_LEN_SEL_RX_0(SP_RX_WordLen);

	SPORTx->SP_DIRECT_CTRL1 &= ~(SP_MASK_DATA_LEN_SEL_RX_1);
	SPORTx->SP_DIRECT_CTRL1 |= SP_DATA_LEN_SEL_RX_1(SP_RX_WordLen);
}

/**
  * @brief Get AUDIO SPORT word length.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @retval the value of word length.
  *            @arg 0: sample bit is 16 bit
  *            @arg 1: sample bit is 20 bit
  *            @arg 2: sample bit is 24 bit
  *            @arg 3: sample bit is 32 bit
  */
u32 AUDIO_SP_GetTXWordLen(AUDIO_SPORT_TypeDef *SPORTx)
{
	u32 len_TX = ((SPORTx->SP_CTRL0) & SP_MASK_DATA_LEN_SEL_TX_0) >> 12;
	return len_TX;
}

/**
  * @brief Get AUDIO SPORT word length.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @retval the value of word length.
  *            @arg 0: sample bit is 16 bit
  *            @arg 1: sample bit is 20 bit
  *            @arg 2: sample bit is 24 bit
  *            @arg 3: sample bit is 32 bit
  */
u32 AUDIO_SP_GetRXWordLen(AUDIO_SPORT_TypeDef *SPORTx)
{
	u32 len_RX = ((SPORTx->SP_FORMAT) & SP_MASK_DATA_LEN_SEL_RX_0) >> 12;
	return len_RX;
}


/**
  * @brief  Set the AUDIO SPORT channel number.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @param  SP_MonoStereo: mono or stereo.
  *          This parameter can be one of the following values:
  *            @arg SP_CH_STEREO: stereo channel, channel number is 0
  *            @arg SP_CH_MONO: mono channel, channel number is 1
  * @retval None
  */

/*仅配置TX， RX默认配置Stereo,同时配置tRX_same_ch为0*/
void AUDIO_SP_SetMonoStereo(AUDIO_SPORT_TypeDef *SPORTx, u32 SP_MonoStereo)
{
	assert_param(IS_SP_CHN_NUM(SP_MonoStereo));

	if (SP_MonoStereo == SP_CH_STEREO) {
		SPORTx->SP_CTRL0 &= ~(SP_BIT_EN_I2S_MONO_TX_0);
	} else {
		SPORTx->SP_CTRL0 &= ~SP_BIT_TRX_SAME_CH;
		SPORTx->SP_CTRL0 |= SP_BIT_EN_I2S_MONO_TX_0;
	}
}

/**
  * @brief  SPORT0 and SPORT1 can be set to master or slave mode.
  * @param  SPORTx: pointer to the base addr of AUDIO SPORT peripheral.
  * @param  SP_MasterSlave: master or slave.
  *          This parameter can be one of the following values:
  *            @arg MASTER: SPORT master mode
  *            @arg SLAVE: SPORT slave mode
  * @retval None
  */
void AUDIO_SP_SetMasterSlave(AUDIO_SPORT_TypeDef *SPORTx, u32 SP_MasterSlave)
{
	assert_param(IS_SP_SEL_MODE(SP_MasterSlave));

	if (SP_MasterSlave == MASTER) {
		SPORTx->SP_CTRL0 &= ~(SP_BIT_SLAVE_CLK_SEL | SP_BIT_SLAVE_DATA_SEL);
	} else {
		SPORTx->SP_CTRL0 |= SP_BIT_SLAVE_CLK_SEL | SP_BIT_SLAVE_DATA_SEL;
	}

}

/**
  * @brief  Set the AUDIO SPORT pinmux function mux: dout or din.
  * @param  i2s_sel: select i2s.
  * @param  pinfunc_sel: select pinmux function: dout or din.
  *          This parameter can be one of the following values:
  *            @arg DOUT0_FUNC
  *            @arg DOUT1_FUNC
  *            @arg DOUT2_FUNC
  *            @arg DOUT3_FUNC
  *            @arg DIN0_FUNC
  *            @arg DIN1_FUNC
  *            @arg DIN2_FUNC
  *            @arg DIN3_FUNC
  * @retval None
  */

void AUDIO_SP_SetPinMux(u32 i2s_sel, u32 pinfunc_sel)
{
	u32 tmp;

	assert_param(IS_SP_SET_I2S_PINFUNC(i2s_sel));
	assert_param(IS_SP_SET_I2S_PINFUNC(pinfunc_sel));

	tmp = HAL_READ32(PINMUX_REG_BASE, REG_I2S_CTRL);

	if (i2s_sel == I2S0) {
		switch (pinfunc_sel) {
		case DOUT3_FUNC:
			tmp |= PAD_BIT_SP0_DIO0_MUXSEL;
			break;
		case DOUT2_FUNC:
			tmp |= PAD_BIT_SP0_DIO1_MUXSEL;
			break;
		case DOUT1_FUNC:
			tmp |= PAD_BIT_SP0_DIO2_MUXSEL;
			break;
		case DOUT0_FUNC:
			tmp |= PAD_BIT_SP0_DIO3_MUXSEL;
			break;
		case DIN0_FUNC:
			tmp &= ~PAD_BIT_SP0_DIO0_MUXSEL;
			break;
		case DIN1_FUNC:
			tmp &= ~PAD_BIT_SP0_DIO1_MUXSEL;
			break;
		case DIN2_FUNC:
			tmp &= ~PAD_BIT_SP0_DIO2_MUXSEL;
			break;
		case DIN3_FUNC:
			tmp &= ~PAD_BIT_SP0_DIO3_MUXSEL;
			break;
		default:
			break;
		}
	} else {
		switch (pinfunc_sel) {
		case DOUT3_FUNC:
			tmp |= PAD_BIT_SP1_DIO0_MUXSEL;
			break;
		case DOUT2_FUNC:
			tmp |= PAD_BIT_SP1_DIO1_MUXSEL;
			break;
		case DOUT1_FUNC:
			tmp |= PAD_BIT_SP1_DIO2_MUXSEL;
			break;
		case DOUT0_FUNC:
			tmp |= PAD_BIT_SP1_DIO3_MUXSEL;
			break;
		case DIN0_FUNC:
			tmp &= ~PAD_BIT_SP1_DIO0_MUXSEL;
			break;
		case DIN1_FUNC:
			tmp &= ~PAD_BIT_SP1_DIO1_MUXSEL;
			break;
		case DIN2_FUNC:
			tmp &= ~PAD_BIT_SP1_DIO2_MUXSEL;
			break;
		case DIN3_FUNC:
			tmp &= ~PAD_BIT_SP1_DIO3_MUXSEL;
			break;
		default:
			break;
		}
	}

	HAL_WRITE32(PINMUX_REG_BASE, REG_I2S_CTRL, tmp);
}

/**
  * @brief    Initialize GDMA peripheral single block mode for sending data.
  * @param  Index: 0.
  * @param  SelGDMA: GDMA Int or Ext.
  * @param  GDMA_InitStruct: pointer to a GDMA_InitTypeDef structure that contains
  *         the configuration information for the GDMA peripheral.
  * @param  CallbackData: GDMA callback data.
  * @param  CallbackFunc: GDMA callback function.
  * @param  pTXData: TX Buffer.
  * @param  Length: TX Count.
  * @retval   TRUE/FLASE
  */
BOOL AUDIO_SP_TXGDMA_Init(
	u32 Index,
	u32 SelGDMA,
	GDMA_InitTypeDef *GDMA_InitStruct,
	void *CallbackData,
	IRQ_FUN CallbackFunc,
	u8 *pTXData,
	u32 Length
)
{
	u8 GdmaChnl;

	assert_param(GDMA_InitStruct != NULL);
	DCache_CleanInvalidate((u32)pTXData, Length);
	/*obtain a DMA channel and register DMA interrupt handler*/
	GdmaChnl = GDMA_ChnlAlloc(0, CallbackFunc, (u32)CallbackData, INT_PRI4);
	if (GdmaChnl == 0xFF) {
		// No Available DMA channel
		return _FALSE;
	}

	_memset((void *)GDMA_InitStruct, 0, sizeof(GDMA_InitTypeDef));

	/*set GDMA initial structure member value*/
	GDMA_InitStruct->MuliBlockCunt = 0;
	GDMA_InitStruct->GDMA_ReloadSrc = 0;
	//GDMA_InitStruct->GDMA_ReloadSrc = 1;
	GDMA_InitStruct->MaxMuliBlock = 1;
	GDMA_InitStruct->GDMA_DIR = TTFCMemToPeri;
	if (SelGDMA == GDMA_INT) {
		GDMA_InitStruct->GDMA_DstHandshakeInterface = AUDIO_DEV_TABLE[Index].Tx_HandshakeInterface;
		GDMA_InitStruct->GDMA_DstAddr = (u32)&AUDIO_DEV_TABLE[Index].SPORTx->SP_TX_FIFO_0_WR_ADDR;
	} else {
		GDMA_InitStruct->GDMA_DstHandshakeInterface = AUDIO_DEV_TABLE[Index].Tx_HandshakeInterface1;
		GDMA_InitStruct->GDMA_DstAddr = (u32)&AUDIO_DEV_TABLE[Index].SPORTx->SP_TX_FIFO_1_WR_ADDR;
	}
	GDMA_InitStruct->GDMA_Index = 0;
	GDMA_InitStruct->GDMA_ChNum = GdmaChnl;
	GDMA_InitStruct->GDMA_IsrType = (BlockType | TransferType | ErrType);
	GDMA_InitStruct->GDMA_DstMsize  = MsizeFour;
	GDMA_InitStruct->GDMA_DstDataWidth = TrWidthOneByte;
	GDMA_InitStruct->GDMA_DstInc = NoChange;
	GDMA_InitStruct->GDMA_SrcInc = IncType;

	/*	Cofigure GDMA transfer */
	/*	24bits or 16bits mode */
	if (((Length & 0x03) == 0) && (((u32)(pTXData) & 0x03) == 0)) {
		/*	4-bytes aligned, move 4 bytes each transfer */
		GDMA_InitStruct->GDMA_SrcMsize	 = MsizeFour;
		GDMA_InitStruct->GDMA_SrcDataWidth = TrWidthFourBytes;
		GDMA_InitStruct->GDMA_BlockSize = Length >> 2;
	} else if (((Length & 0x01) == 0) && (((u32)(pTXData) & 0x01) == 0)) {
		/*	2-bytes aligned, move 2 bytes each transfer */
		GDMA_InitStruct->GDMA_SrcMsize	 = MsizeEight;
		GDMA_InitStruct->GDMA_SrcDataWidth = TrWidthTwoBytes;
		GDMA_InitStruct->GDMA_BlockSize = Length >> 1;
	} else {
		DBG_8195A("AUDIO_SP_TXGDMA_Init: Aligment Err: pTXData=0x%x,  Length=%d\n", pTXData, Length);
		return _FALSE;
	}
	GDMA_InitStruct->GDMA_DstMsize = MsizeFour;
	GDMA_InitStruct->GDMA_DstDataWidth = TrWidthFourBytes;

	/*configure GDMA source address */
	GDMA_InitStruct->GDMA_SrcAddr = (u32)pTXData;


	/*	Enable GDMA for TX */
	GDMA_Init(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, GDMA_InitStruct);
	GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, ENABLE);
	return _TRUE;
}

/**
  * @brief    Initialize GDMA peripheral single block mode for receiving data.
  * @param  Index: 0.
  * @param  GDMA_InitStruct: pointer to a GDMA_InitTypeDef structure that contains
  *         the configuration information for the GDMA peripheral.
  * @param  CallbackData: GDMA callback data.
  * @param  CallbackFunc: GDMA callback function.
  * @param  pRXData: RX Buffer.
  * @param  Length: RX Count.
  * @retval   TRUE/FLASE
  */
BOOL AUDIO_SP_RXGDMA_Init(
	u32 Index,
	u32 SelGDMA,
	GDMA_InitTypeDef *GDMA_InitStruct,
	void *CallbackData,
	IRQ_FUN CallbackFunc,
	u8 *pRXData,
	u32 Length
)
{
	u8 GdmaChnl;
	DCache_CleanInvalidate((u32)pRXData, Length);
	assert_param(GDMA_InitStruct != NULL);
	/*obtain a DMA channel and register DMA interrupt handler*/
	GdmaChnl = GDMA_ChnlAlloc(0, CallbackFunc, (u32)CallbackData, INT_PRI4);
	if (GdmaChnl == 0xFF) {
		// No Available DMA channel
		return _FALSE;
	}

	_memset((void *)GDMA_InitStruct, 0, sizeof(GDMA_InitTypeDef));


	/*set GDMA initial structure member value*/
	GDMA_InitStruct->MuliBlockCunt = 0;
	GDMA_InitStruct->GDMA_ReloadSrc = 0;
	//GDMA_InitStruct->GDMA_ReloadDst = 1; //only for test
	GDMA_InitStruct->MaxMuliBlock = 1;
	GDMA_InitStruct->GDMA_DIR = TTFCPeriToMem;

	if (SelGDMA == GDMA_INT) {
		GDMA_InitStruct->GDMA_SrcHandshakeInterface = AUDIO_DEV_TABLE[Index].Rx_HandshakeInterface;
		GDMA_InitStruct->GDMA_SrcAddr = (u32)&AUDIO_DEV_TABLE[Index].SPORTx->SP_RX_FIFO_0_RD_ADDR;
	} else {
		GDMA_InitStruct->GDMA_SrcHandshakeInterface = AUDIO_DEV_TABLE[Index].Rx_HandshakeInterface1;
		GDMA_InitStruct->GDMA_SrcAddr = (u32)&AUDIO_DEV_TABLE[Index].SPORTx->SP_RX_FIFO_1_RD_ADDR;
	}
	GDMA_InitStruct->GDMA_Index = 0;
	GDMA_InitStruct->GDMA_ChNum = GdmaChnl;
	GDMA_InitStruct->GDMA_IsrType = (BlockType | TransferType | ErrType);
	GDMA_InitStruct->GDMA_DstInc = IncType;
	GDMA_InitStruct->GDMA_SrcInc = NoChange;

	/*  Cofigure GDMA transfer */
	GDMA_InitStruct->GDMA_SrcDataWidth = TrWidthFourBytes;
	GDMA_InitStruct->GDMA_SrcMsize = MsizeEight;

	GDMA_InitStruct->GDMA_DstMsize = MsizeEight;
	GDMA_InitStruct->GDMA_DstDataWidth = TrWidthFourBytes;
	GDMA_InitStruct->GDMA_BlockSize = Length >> 2;

	/*configure GDMA destination address */
	GDMA_InitStruct->GDMA_DstAddr = (u32)pRXData;

	/*  Enable GDMA for RX */
	GDMA_Init(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, GDMA_InitStruct);

	GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, ENABLE);
	//DBG_8195A("RXGDMA_End\n");

	return _TRUE;
}

/**
  * @brief  Audio GDMA TX restart in isr
  * @param  GDMA_Index: GDMA index used in this routine.
  * @param  GDMA_ChNum: GDMA channel number used in this routine.
  * @param  TX_addr: Address of data to be sent.
  * @param  TX_length: Length of data to be sent.
  * @retval TRUE
  */
BOOL AUDIO_SP_TXGDMA_Restart(
	u8 GDMA_Index,
	u8 GDMA_ChNum,
	u32 TX_addr,
	u32 TX_length
)
{
	GDMA_SetSrcAddr(GDMA_Index, GDMA_ChNum, TX_addr);
	GDMA_SetBlkSize(GDMA_Index, GDMA_ChNum, TX_length >> 2);
	DCache_CleanInvalidate(TX_addr, TX_length);
	GDMA_Cmd(GDMA_Index, GDMA_ChNum, ENABLE);

	return _TRUE;
}

/**
  * @brief  Audio GDMA RX restart in isr
  * @param  GDMA_Index: GDMA index used in this routine.
  * @param  GDMA_ChNum: GDMA channel number used in this routine.
  * @param  TX_addr: Address of data to be received.
  * @param  TX_length: Length of data to be received.
  * @retval TRUE
  */
BOOL AUDIO_SP_RXGDMA_Restart(
	u8 GDMA_Index,
	u8 GDMA_ChNum,
	u32 RX_addr,
	u32 RX_length
)
{
	GDMA_SetDstAddr(GDMA_Index, GDMA_ChNum, RX_addr);
	GDMA_SetBlkSize(GDMA_Index, GDMA_ChNum, RX_length >> 2);
	DCache_CleanInvalidate(RX_addr, RX_length);
	GDMA_Cmd(GDMA_Index, GDMA_ChNum, ENABLE);

	return _TRUE;
}

/**
  * @brief    Initialize GDMA peripheral linklist mode for sending data.
  * @param  Index: 0.
  * @param  SelGDMA: GDMA Int or Ext.
  * @param  GDMA_InitStruct: pointer to a GDMA_InitTypeDef structure that contains
  *         the configuration information for the GDMA peripheral.
  * @param  CallbackData: GDMA callback data.
  * @param  CallbackFunc: GDMA callback function.
  * @param  Length: BlockSize.
  * @param  MaxLLP: LLP mode max number.
  * @param  Lli: This parameter stores the address pointing to the next Linked List Item in block chaining.
  * @retval   TRUE/FLASE
  */

BOOL AUDIO_SP_LLPTXGDMA_Init(
	u32 Index,
	u32 SelGDMA,
	GDMA_InitTypeDef *GDMA_InitStruct,
	void *CallbackData,
	IRQ_FUN CallbackFunc,
	u32 Length,
	u32 MaxLLP,
	struct GDMA_CH_LLI *Lli
)
{
	u32 i;
	u8 GdmaChnl;

	assert_param(GDMA_InitStruct != NULL);
	_memset((void *)GDMA_InitStruct, 0, sizeof(GDMA_InitTypeDef));

	u32 pTxData = Lli[0].LliEle.Sarx;

	/*obtain a DMA channel and register DMA interrupt handler*/
	GdmaChnl = GDMA_ChnlAlloc(0, CallbackFunc, (u32)CallbackData, 12);
	if (GdmaChnl == 0xFF) {
		// No Available DMA channel
		return _FALSE;
	}

	/*set GDMA initial structure member value*/
	GDMA_InitStruct->MuliBlockCunt = 0;
	GDMA_InitStruct->GDMA_ReloadSrc = 0;
	GDMA_InitStruct->GDMA_ReloadDst = 0;
	GDMA_InitStruct->GDMA_Index = 0;
	GDMA_InitStruct->GDMA_ChNum = GdmaChnl;
	GDMA_InitStruct->MaxMuliBlock = MaxLLP;
	GDMA_InitStruct->GDMA_DIR = TTFCMemToPeri;
	GDMA_InitStruct->GDMA_IsrType = (BlockType | TransferType | ErrType);
	GDMA_InitStruct->GDMA_DstMsize = MsizeFour;
	GDMA_InitStruct->GDMA_DstDataWidth = TrWidthFourBytes;
	GDMA_InitStruct->GDMA_DstInc = NoChange;
	GDMA_InitStruct->GDMA_SrcInc = IncType;
	GDMA_InitStruct->GDMA_LlpSrcEn = 1;
	GDMA_InitStruct->GDMA_LlpDstEn = 1;

	if (SelGDMA == GDMA_INT) {
		GDMA_InitStruct->GDMA_DstHandshakeInterface = AUDIO_DEV_TABLE[Index].Tx_HandshakeInterface;
		for (i = 0; i < MaxLLP; i++) {
			Lli[i].LliEle.Darx = (u32)&AUDIO_DEV_TABLE[Index].SPORTx->SP_TX_FIFO_0_WR_ADDR;
		}

	} else {
		GDMA_InitStruct->GDMA_DstHandshakeInterface = AUDIO_DEV_TABLE[Index].Tx_HandshakeInterface1;
		for (i = 0; i < MaxLLP; i++) {
			Lli[i].LliEle.Darx = (u32)&AUDIO_DEV_TABLE[Index].SPORTx->SP_TX_FIFO_1_WR_ADDR;
		}
	}

	/*  Cofigure GDMA transfer */
	/*  24bits or 16bits mode */
	if (((Length & 0x03) == 0) && (((u32)(pTxData) & 0x03) == 0)) {
		/*  4-bytes aligned, move 4 bytes each transfer */
		GDMA_InitStruct->GDMA_SrcMsize   = MsizeFour;
		GDMA_InitStruct->GDMA_SrcDataWidth = TrWidthFourBytes;
		GDMA_InitStruct->GDMA_BlockSize = Length >> 2;
	} else if (((Length & 0x01) == 0) && (((u32)(pTxData) & 0x01) == 0)) {
		/*  2-bytes aligned, move 2 bytes each transfer */
		GDMA_InitStruct->GDMA_SrcMsize   = MsizeEight;
		GDMA_InitStruct->GDMA_SrcDataWidth = TrWidthTwoBytes;
		GDMA_InitStruct->GDMA_BlockSize = Length >> 1;
	} else {
		DBG_8195A("AUDIO_SP_TXGDMA_Init: Aligment Err: pTxData=0x%x,  Length=%d\n", pTxData, Length);
		return _FALSE;
	}


	for (i = 0; i < MaxLLP; i++) {
		Lli[i].BlockSize = GDMA_InitStruct->GDMA_BlockSize;
	}

	/*configure GDMA source address */
	GDMA_InitStruct->GDMA_SrcAddr = pTxData;

	/*  Enable GDMA for TX */
	GDMA_Init(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, GDMA_InitStruct);
	GDMA_SetLLP(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, GDMA_InitStruct->MaxMuliBlock, Lli, 1);
	DCache_CleanInvalidate((u32)Lli, MaxLLP * sizeof(*Lli));
	GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, ENABLE);

	return _TRUE;
}

/**
  * @brief  Initialize GDMA peripheral linklist mode for receiving data.
  * @param  Index: 0.
  * @param  SelGDMA: GDMA Int or Ext.
  * @param  GDMA_InitStruct: pointer to a GDMA_InitTypeDef structure that contains
  *         the configuration information for the GDMA peripheral.
  * @param  CallbackData: GDMA callback data.
  * @param  CallbackFunc: GDMA callback function.
  * @param  Length: BlockSize.
  * @param  MaxLLP: LLP mode max number.
  * @param  Lli: This parameter stores the address pointing to the next Linked List Item in block chaining.
  * @retval   TRUE/FLASE
  */
BOOL AUDIO_SP_LLPRXGDMA_Init(
	u32 Index,
	u32 SelGDMA,
	GDMA_InitTypeDef *GDMA_InitStruct,
	void *CallbackData,
	IRQ_FUN CallbackFunc,
	u32 Length,
	u32 MaxLLP,
	struct GDMA_CH_LLI *Lli
)
{
	u8 GdmaChnl;
	u32 i;

	assert_param(GDMA_InitStruct != NULL);
	_memset((void *)GDMA_InitStruct, 0, sizeof(GDMA_InitTypeDef));

	/*obtain a DMA channel and register DMA interrupt handler*/
	GdmaChnl = GDMA_ChnlAlloc(0, CallbackFunc, (u32)CallbackData, 12);
	if (GdmaChnl == 0xFF) {
		// No Available DMA channel
		return _FALSE;
	}

	/*set GDMA initial structure member value*/
	GDMA_InitStruct->MuliBlockCunt = 0;
	GDMA_InitStruct->GDMA_ReloadSrc = 0;
	GDMA_InitStruct->GDMA_ReloadDst = 0;
	GDMA_InitStruct->MaxMuliBlock = MaxLLP;
	GDMA_InitStruct->GDMA_DIR = TTFCPeriToMem;
	GDMA_InitStruct->GDMA_Index = 0;
	GDMA_InitStruct->GDMA_ChNum = GdmaChnl;
	GDMA_InitStruct->GDMA_IsrType = (BlockType | TransferType | ErrType);
	GDMA_InitStruct->GDMA_DstInc = IncType;
	GDMA_InitStruct->GDMA_SrcInc = NoChange;
	GDMA_InitStruct->GDMA_LlpSrcEn = 1;
	GDMA_InitStruct->GDMA_LlpDstEn = 1;

	if (SelGDMA == GDMA_INT) {
		GDMA_InitStruct->GDMA_SrcHandshakeInterface = AUDIO_DEV_TABLE[Index].Rx_HandshakeInterface;
		for (i = 0; i < MaxLLP; i++) {
			Lli[i].LliEle.Sarx = (u32)&AUDIO_DEV_TABLE[Index].SPORTx->SP_RX_FIFO_0_RD_ADDR;
		}
	} else {
		GDMA_InitStruct->GDMA_SrcHandshakeInterface = AUDIO_DEV_TABLE[Index].Rx_HandshakeInterface1;
		for (i = 0; i < MaxLLP; i++) {
			Lli[i].LliEle.Sarx = (u32)&AUDIO_DEV_TABLE[Index].SPORTx->SP_RX_FIFO_1_RD_ADDR;
		}
	}

	/*  Cofigure GDMA transfer */
	GDMA_InitStruct->GDMA_SrcDataWidth = TrWidthFourBytes;
	GDMA_InitStruct->GDMA_SrcMsize = MsizeEight;

	GDMA_InitStruct->GDMA_DstMsize = MsizeEight;
	GDMA_InitStruct->GDMA_DstDataWidth = TrWidthFourBytes;
	GDMA_InitStruct->GDMA_BlockSize = Length >> 2;

	/*check GDMA block size*/
	for (i = 0; i < MaxLLP; i++) {
		Lli[i].BlockSize = GDMA_InitStruct->GDMA_BlockSize;
	}

	/*configure GDMA destination address */
	GDMA_InitStruct->GDMA_DstAddr = Lli[0].LliEle.Darx;

	/*  Enable GDMA for RX */
	GDMA_Init(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, GDMA_InitStruct);
	GDMA_SetLLP(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, GDMA_InitStruct->MaxMuliBlock, Lli, 1);
	DCache_CleanInvalidate((u32)Lli, MaxLLP * sizeof(*Lli));
	GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, ENABLE);

	return _TRUE;
}

/**
  * @brief  Enable or disable SPORT TX counter interrupt.
  * @param  index: select SPORT.
  * @param  state: enable or disable.
  */
void AUDIO_SP_SetTXCounter(u32 index, u32 state)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	if (state == ENABLE) {
		SPORTx->SP_RX_LRCLK |= SP_BIT_EN_TX_SPORT_INTERRUPT;
	} else {
		SPORTx->SP_RX_LRCLK &= ~SP_BIT_EN_TX_SPORT_INTERRUPT;
	}
}

/**
  * @brief  Set SPORT TX counter comp_val, When counter equal comp_val,SPORT will send interrupt to DSP.
  * @param  index: select SPORT.
  * @param  comp_val.
  */
void AUDIO_SP_SetTXCounterCompVal(u32 index, u32 comp_val)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	SPORTx->SP_RX_LRCLK &= ~SP_MASK_TX_SPORT_COMPARE_VAL;
	SPORTx->SP_RX_LRCLK |= SP_TX_SPORT_COMPARE_VAL(comp_val);
}


/**
  * @brief  Clear SPORT TX counter irq.
  * @param  index: select SPORT index.
  */
void AUDIO_SP_ClearTXCounterIrq(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	SPORTx->SP_RX_LRCLK |= SP_BIT_CLR_TX_SPORT_RDY;
}


/**
  * @brief  Get SPORT TX counter.
  * @param  index: select SPORT.
  */
u32 AUDIO_SP_GetTXCounterVal(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	SPORTx->SP_RX_LRCLK |= SP_BIT_EN_FS_PHASE_LATCH;
	u32 tx_counter = ((SPORTx->SP_DSP_COUNTER) & SP_MASK_TX_SPORT_COUNTER) >> 5;
	return tx_counter;
}


/**
  * @brief  Enable or disable SPORT RX counter interrupt.
  * @param  index: select SPORT.
  * @param  state: enable or disable.
  */
void AUDIO_SP_SetRXCounter(u32 index, u32 state)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	if (state == ENABLE) {
		SPORTx->SP_RX_COUNTER1 |= SP_BIT_EN_RX_SPORT_INTERRUPT;
	} else {
		SPORTx->SP_RX_COUNTER1 &= ~SP_BIT_EN_RX_SPORT_INTERRUPT;
	}
}


/**
  * @brief  Set SPORT RX counter comp_val, When counter equal comp_val,SPORT will send interrupt to DSP.
  * @param  index: select SPORT.
  * @param  comp_val.
  */
void AUDIO_SP_SetRXCounterCompVal(u32 index, u32 comp_val)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	SPORTx->SP_RX_COUNTER1 &= ~SP_MASK_RX_SPORT_COMPARE_VAL;
	SPORTx->SP_RX_COUNTER1 |= SP_RX_SPORT_COMPARE_VAL(comp_val);
}


/**
  * @brief  Clear SPORT RX counter irq.
  * @param  index: select SPORT index.
  */
void AUDIO_SP_ClearRXCounterIrq(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	SPORTx->SP_RX_COUNTER1 |= SP_BIT_CLR_RX_SPORT_RDY;
}


/**
  * @brief  Get SPORT RX counter.
  * @param  index: select SPORT.
  */
u32 AUDIO_SP_GetRXCounterVal(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	SPORTx->SP_RX_LRCLK |= SP_BIT_EN_FS_PHASE_LATCH;
	u32 rx_counter = ((SPORTx->SP_RX_COUNTER2) & SP_MASK_RX_SPORT_COUNTER) >> 5;
	return rx_counter;
}


void AUDIO_SP_Deinit(u32 index, u32 direction)
{
	assert_param(IS_SP_SET_DIR(direction));

	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	if (direction == SP_DIR_TX) {
		AUDIO_SP_Unregister(index, direction);
		AUDIO_SP_TXStart(SPORTx, DISABLE);
	} else {
		AUDIO_SP_Unregister(index, direction);
		AUDIO_SP_RXStart(SPORTx, DISABLE);
	}

}


/******************* (C) COPYRIGHT 2017 Realtek Semiconductor *****END OF FILE****/
