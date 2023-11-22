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

static const char *TAG = "SPORT";
SP_RegTypeDef SP_RegFlag[4] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

const AUDIO_DevTable AUDIO_DEV_TABLE[4] = {
	{
		AUDIO_SPORT0_DEV, GDMA_HANDSHAKE_INTERFACE_SPORT0F0_TX, GDMA_HANDSHAKE_INTERFACE_SPORT0F0_RX,
		GDMA_HANDSHAKE_INTERFACE_SPORT0F1_TX, GDMA_HANDSHAKE_INTERFACE_SPORT0F1_RX
	},	 /*audio sport0 */
	{
		AUDIO_SPORT1_DEV, GDMA_HANDSHAKE_INTERFACE_SPORT1F0_TX, GDMA_HANDSHAKE_INTERFACE_SPORT1F0_RX,
		GDMA_HANDSHAKE_INTERFACE_SPORT1F1_TX, GDMA_HANDSHAKE_INTERFACE_SPORT1F1_RX
	},	 /*audio sport1 */
	{
		AUDIO_SPORT2_DEV, GDMA_HANDSHAKE_INTERFACE_SPORT2F0_TX, GDMA_HANDSHAKE_INTERFACE_SPORT2F0_RX,
		GDMA_HANDSHAKE_INTERFACE_SPORT2F1_TX, GDMA_HANDSHAKE_INTERFACE_SPORT2F1_RX
	},	 /*audio sport2 */
	{
		AUDIO_SPORT3_DEV, GDMA_HANDSHAKE_INTERFACE_SPORT3F0_TX, GDMA_HANDSHAKE_INTERFACE_SPORT3F0_RX,
		GDMA_HANDSHAKE_INTERFACE_SPORT3F1_TX, GDMA_HANDSHAKE_INTERFACE_SPORT3F1_RX
	},	 /*audio sport3 */
};

/**
  * @brief  Fills each SP_StructInit member with its default value.
  * @param  SP_StructInit: pointer to an SP_StructInit structure which will be initialized.
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
	SP_InitStruct->SP_SelClk = I2S_XTAL40M;
	SP_InitStruct->SP_Bclk = (32 * ((SP_InitStruct->SP_SelTDM) + 1) * 2 * (SP_InitStruct->SP_SR));
	SP_InitStruct->SP_Fix_Bclk = DISABLE;

}


/**
  * @brief  Register sport.
  * @param  index: 0 ~ 3.
  * @param  direction: SP_DIR_TX or SP_DIR_RX.
  * @param  SP_StructInit: pointer to an SP_StructInit structure.
  * @retval None
*/
BOOL AUDIO_SP_Register(u32 index, u32 direction, SP_InitTypeDef *SP_InitStruct)
{
	assert_param(IS_SP_SET_DIR(direction));

	if (direction == SP_DIR_TX) {

		if (SP_RegFlag[index].SP_DIR_TX_flag) {
			//RTK_LOGW(TAG, "SPORT TX has been registered!\n");
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
						//RTK_LOGW(TAG, "SPORT RX Bclk can't be changed!\n");
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
			//RTK_LOGW(TAG, "SPORT RX has been registered!\n");
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
						//RTK_LOGW(TAG, "SPORT TX Bclk can't be changed!\n");
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
* @param  direction: SP_DIR_TX or SP_DIR_RX.
* @retval None
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
  * @param  index: select SPORT.
  * @retval None
  */
void AUDIO_SP_Reset(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	SPORTx->SP_CTRL0 |= SP_BIT_RESET;
	SPORTx->SP_CTRL0 &= ~ SP_BIT_RESET;
}


/**
  * @brief  Get sport tx channel length.
  * @param  index: select SPORT.
  * @retval None
*/
u32 AUDIO_SP_GetTXChnLen(u32 index)
{
	u32 chn_len;
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
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

/**
  * @brief  Get sport rx channel length.
  * @param  index: select SPORT.
  * @retval None
*/
u32 AUDIO_SP_GetRXChnLen(u32 index)
{
	u32 chn_len;
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
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
  * @brief  Set AUDIO SPORT MCLK enable or disable.
  * @param  index: sport index.
  * @param  NewState: enable or disable.
  * @retval None
  */
void AUDIO_SP_SetMclk(u32 index, u32 NewState)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	if (NewState == ENABLE) {
		SPORTx->SP_CTRL1 |= SP_BIT_ENABLE_MCLK;
	} else {
		SPORTx->SP_CTRL1 &= ~SP_BIT_ENABLE_MCLK;
	}
}


/**
  * @brief  Set AUDIO SPORT MCLK divider factor.
  * @param  mck_div: sport index.
  * @param  div: MCLK output.
  *          This parameter can be one of the following values:
  *            @arg 0: MCLK output=dsp_src_clk/4
  *            @arg 1: MCLK output=dsp_src_clk/2
  *            @arg 2: MCLK output=dsp_src_clk
  * @retval None
  */
void AUDIO_SP_SetMclkDiv(u32 index, u32 mck_div)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	SPORTx->SP_CTRL0 &= ~SP_MASK_MCLK_SEL;
	SPORTx->SP_CTRL0 |= SP_MCLK_SEL(mck_div);
}

/**
  * @brief  Set AUDIO SPORT fixed BLCK mode.
  * @param  index: sport index.
  * @param  NewState: enable or disable fix bclk mode.
  * @retval None
  */
void AUDIO_SP_SetFixBclk(u32 index, SP_InitTypeDef *SP_InitStruct)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	if (SP_InitStruct->SP_Fix_Bclk == ENABLE) {
		SPORTx->SP_FORMAT |= SP_BIT_FIXED_BCLK;
	} else {
		SPORTx->SP_FORMAT &= ~SP_BIT_FIXED_BCLK;
	}
}

/**
  * @brief  Select AUDIO SPORT BCLK divider factor.
  * @param  index: sport index.
  * @param  bclk_sel: fixed bclk value.
  *          This parameter can be one of the following values:
  *            @arg DSP_SRC_CLK_DIV4: dsp_src_clk/4
  *            @arg DSP_SRC_CLK_DIV2: dsp_src_clk/2
  * @retval None
  */
void AUDIO_SP_SelFixBclk(u32 index, u32 bclk_sel)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	if (bclk_sel == DSP_SRC_BCLK_DIV2) {
		SPORTx->SP_FORMAT |= SP_BIT_FIXED_BCLK_SEL;
	} else {
		SPORTx->SP_FORMAT &= ~SP_BIT_FIXED_BCLK_SEL;
	}
}

/**
  * @brief  Set AUDIO SPORT TX BLCK divider factor.
  * @param	index: sport index.
  * @param  clock: sport clock source.
  * @param  sr: sport tx sample rate.
  * @param  tdm: sport tx tdm mode.
  * @param  chn_len: sport tx channel length.
  * @retval None
  * @retval None
  */
void AUDIO_SP_SetTXClkDiv(u32 index, u32 clock, u32 sr, u32 tdm, u32 chn_len)
{
	u32 Tmp;
	u32 MI = 0;
	u32 NI = 0;
	u32 i = 1;
	u32 bclk;
	u32 bclk_div;
	u32 channel_count;

	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	if (!(clock % 40000000)) {
		if (!(sr % 4000)) {
			MI = 1250;
			NI = (chn_len / 4) * (tdm + 1) * (sr / 4000);
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
	} else {
		channel_count = (tdm + 1) * 2;
		bclk = channel_count * chn_len * sr;
		NI = 1;

		if (clock * NI % bclk == 0) {
			MI = clock * NI / bclk;
		} else {
			for (i = 1; i < 0x7FFF; i++) {
				if (clock * i % bclk == 0) {
					NI = i;
					MI = clock * NI / bclk;
					break;
				}
			}
		}
	}

	switch (chn_len) {
	case SP_CL_16:
		bclk_div = 32;
		break;
	case SP_CL_20:
		bclk_div = 40;
		break;
	case SP_CL_24:
		bclk_div = 48;
		break;
	default:
		bclk_div = 64;
		break;

	}


	Tmp = SPORTx->SP_SR_TX_BCLK ;
	Tmp &= ~(SP_MASK_TX_MI | SP_MASK_TX_NI | SP_BIT_TX_MI_NI_UPDATE);
	Tmp |= (SP_TX_MI(MI) | SP_TX_NI(NI) | SP_BIT_TX_MI_NI_UPDATE);
	SPORTx->SP_SR_TX_BCLK  = Tmp;

	if (tdm == SP_TX_NOTDM) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_TX_BCLK_DIV_RATIO);
		Tmp |= SP_TX_BCLK_DIV_RATIO(bclk_div - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else if (tdm == SP_TX_TDM4) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_TX_BCLK_DIV_RATIO);
		Tmp |= SP_TX_BCLK_DIV_RATIO(bclk_div * 2 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else if (tdm == SP_TX_TDM6) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_TX_BCLK_DIV_RATIO);
		Tmp |= SP_TX_BCLK_DIV_RATIO(bclk_div * 3 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_TX_BCLK_DIV_RATIO);
		Tmp |= SP_TX_BCLK_DIV_RATIO(bclk_div * 4 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	}

}

/**
  * @brief  Set AUDIO SPORT RX BLCK divider factor.
  * @param  index: sport index.
  * @param  clock: sport clock source.
  * @param  sr: sport rx sample rate.
  * @param  tdm: sport rx tdm mode.
  * @param  chn_len: sport rx channel length.
  * @retval None
  */
void AUDIO_SP_SetRXClkDiv(u32 index, u32 clock, u32 sr, u32 tdm, u32 chn_len)
{
	u32 Tmp;
	u32 NI = 0;
	u32 MI = 0;
	u32 bclk_div;
	u32 i = 1;
	u32 bclk;
	u32 channel_count;

	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	if (!(clock % 40000000)) {
		if (!(sr % 4000)) {
			MI = 1250;
			NI = (chn_len / 4) * (tdm + 1) * (sr / 4000);
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
	} else {
		channel_count = (tdm + 1) * 2;
		bclk = channel_count * chn_len * sr;
		NI = 1;

		if (clock * NI % bclk == 0) {
			MI = clock * NI / bclk;
		} else {
			for (i = 1; i < 0x7FFF; i++) {
				if (clock * i % bclk == 0) {
					NI = i;
					MI = clock * NI / bclk;
					break;
				}
			}
		}
	}

	Tmp = SPORTx->SP_RX_BCLK ;
	Tmp &= ~(SP_MASK_RX_MI | SP_MASK_RX_NI | SP_BIT_RX_MI_NI_UPDATE);
	Tmp |= (SP_RX_MI(MI) | SP_RX_NI(NI) | SP_BIT_RX_MI_NI_UPDATE);
	SPORTx->SP_RX_BCLK  = Tmp;

	switch (chn_len) {
	case SP_CL_16:
		bclk_div = 32;
		break;
	case SP_CL_20:
		bclk_div = 40;
		break;
	case SP_CL_24:
		bclk_div = 48;
		break;
	default:
		bclk_div = 64;
		break;
	}

	if (tdm == SP_RX_NOTDM) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_RX_BCLK_DIV_RATIO);
		Tmp |= SP_RX_BCLK_DIV_RATIO(bclk_div - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else if (tdm == SP_RX_TDM4) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_RX_BCLK_DIV_RATIO);
		Tmp |= SP_RX_BCLK_DIV_RATIO(bclk_div * 2 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else if (tdm == SP_TX_TDM6) {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_RX_BCLK_DIV_RATIO);
		Tmp |= SP_RX_BCLK_DIV_RATIO(bclk_div * 3 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	} else {
		Tmp = SPORTx->SP_TX_LRCLK ;
		Tmp &= ~(SP_MASK_RX_BCLK_DIV_RATIO);
		Tmp |= SP_RX_BCLK_DIV_RATIO(bclk_div * 4 - 1);
		SPORTx->SP_TX_LRCLK  = Tmp;
	}
}

/**
* @brief Select AUDIO SPORT tx channel source.
* @param  index: select SPORT.
* @param  tx_chn: select tx channel,channel 0-7 corresponding to DOUT0-3.
*		   This parameter can be one of the following values:
*			 @arg TXCHN0
*			 @arg TXCHN1
*			 @arg TXCHN2
*			 @arg TXCHN3
*			 @arg TXCHN4
*			 @arg TXCHN5
*			 @arg TXCHN6
*			 @arg TXCHN7
* @param  fifo_chn: select fifo channel
*		   This parameter can be one of the following values:
*			 @arg TX_FIFO0_REG0_L
*			 @arg TX_FIFO0_REG0_R
*			 @arg TX_FIFO0_REG1_L
*			 @arg TX_FIFO0_REG1_R
*			 @arg TX_FIFO1_REG0_L
*			 @arg TX_FIFO1_REG0_R
*			 @arg TX_FIFO1_REG1_L
*			 @arg TX_FIFO1_REG1_R
*/
void AUDIO_SP_TXCHNSrcSel(u32 index, u32 tx_chn, u32 fifo_chn)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	switch (tx_chn) {
	case TXCHN0:
		SPORTx->SP_DIRECT_CTRL0 &= ~SP_MASK_TX_CH0_DATA_SEL;
		SPORTx->SP_DIRECT_CTRL0 |= SP_TX_CH0_DATA_SEL(fifo_chn);
		break;
	case TXCHN1:
		SPORTx->SP_DIRECT_CTRL0 &= ~SP_MASK_TX_CH1_DATA_SEL;
		SPORTx->SP_DIRECT_CTRL0 |= SP_TX_CH1_DATA_SEL(fifo_chn);
		break;
	case TXCHN2:
		SPORTx->SP_DIRECT_CTRL0 &= ~SP_MASK_TX_CH2_DATA_SEL;
		SPORTx->SP_DIRECT_CTRL0 |= SP_TX_CH2_DATA_SEL(fifo_chn);
		break;
	case TXCHN3:
		SPORTx->SP_DIRECT_CTRL0 &= ~SP_MASK_TX_CH3_DATA_SEL;
		SPORTx->SP_DIRECT_CTRL0 |= SP_TX_CH3_DATA_SEL(fifo_chn);
		break;
	case TXCHN4:
		SPORTx->SP_DIRECT_CTRL0 &= ~SP_MASK_TX_CH4_DATA_SEL;
		SPORTx->SP_DIRECT_CTRL0 |= SP_TX_CH4_DATA_SEL(fifo_chn);
		break;
	case TXCHN5:
		SPORTx->SP_DIRECT_CTRL0 &= ~SP_MASK_TX_CH5_DATA_SEL;
		SPORTx->SP_DIRECT_CTRL0 |= SP_TX_CH5_DATA_SEL(fifo_chn);
		break;
	case TXCHN6:
		SPORTx->SP_DIRECT_CTRL0 &= ~SP_MASK_TX_CH6_DATA_SEL;
		SPORTx->SP_DIRECT_CTRL0 |= SP_TX_CH6_DATA_SEL(fifo_chn);
		break;
	case TXCHN7:
		SPORTx->SP_DIRECT_CTRL0 &= ~SP_MASK_TX_CH7_DATA_SEL;
		SPORTx->SP_DIRECT_CTRL0 |= SP_TX_CH7_DATA_SEL(fifo_chn);
		break;
	default:
		break;
	}

}

/**
  * @brief Select AUDIO SPORT rx fifo source.
  * @param	index: select SPORT.
  * @param	fifo_chn: select fifo channel
  * 		 This parameter can be one of the following values:
  * 		   @arg RX_FIFO0_REG0_L
  * 		   @arg RX_FIFO0_REG0_R
  * 		   @arg RX_FIFO0_REG1_L
  * 		   @arg RX_FIFO0_REG1_R
  * 		   @arg RX_FIFO1_REG0_L
  * 		   @arg RX_FIFO1_REG0_R
  * 		   @arg RX_FIFO1_REG1_L
  * 		   @arg RX_FIFO1_REG1_R
  * @param	rx_chn: select rx channel,channel 0-7 corresponding to DIN0-3.
  * 		 This parameter can be one of the following values:
  * 		   @arg RXCHN0
  * 		   @arg RXCHN1
  * 		   @arg RXCHN2
  * 		   @arg RXCHN3
  * 		   @arg RXCHN4
  * 		   @arg RXCHN5
  * 		   @arg RXCHN6
  * 		   @arg RXCHN7
  */
void AUDIO_SP_RXFIFOSrcSel(u32 index, u32 fifo_chn, u32 rx_chn)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	switch (fifo_chn) {
	case RX_FIFO0_REG0_L:
		SPORTx->SP_DIRECT_CTRL3 &= ~SP_MASK_RX_FIFO_0_REG_0_L_SEL;
		SPORTx->SP_DIRECT_CTRL3 |= SP_RX_FIFO_0_REG_0_L_SEL(rx_chn);
		break;
	case RX_FIFO0_REG0_R:
		SPORTx->SP_DIRECT_CTRL3 &= ~SP_MASK_RX_FIFO_0_REG_0_R_SEL;
		SPORTx->SP_DIRECT_CTRL3 |= SP_RX_FIFO_0_REG_0_R_SEL(rx_chn);
		break;
	case RX_FIFO0_REG1_L:
		SPORTx->SP_DIRECT_CTRL3 &= ~SP_MASK_RX_FIFO_0_REG_1_L_SEL;
		SPORTx->SP_DIRECT_CTRL3 |= SP_RX_FIFO_0_REG_1_L_SEL(rx_chn);
		break;
	case RX_FIFO0_REG1_R:
		SPORTx->SP_DIRECT_CTRL3 &= ~SP_MASK_RX_FIFO_0_REG_1_R_SEL;
		SPORTx->SP_DIRECT_CTRL3 |= SP_RX_FIFO_0_REG_1_R_SEL(rx_chn);
		break;
	case RX_FIFO1_REG0_L:
		SPORTx->SP_DIRECT_CTRL4 &= ~SP_MASK_RX_FIFO_1_REG_0_L_SEL;
		SPORTx->SP_DIRECT_CTRL4 |= SP_RX_FIFO_1_REG_0_L_SEL(rx_chn);
		break;
	case RX_FIFO1_REG0_R:
		SPORTx->SP_DIRECT_CTRL4 &= ~SP_MASK_RX_FIFO_1_REG_0_R_SEL;
		SPORTx->SP_DIRECT_CTRL4 |= SP_RX_FIFO_1_REG_0_R_SEL(rx_chn);
		break;
	case RX_FIFO1_REG1_L:
		SPORTx->SP_DIRECT_CTRL4 &= ~SP_MASK_RX_FIFO_1_REG_1_L_SEL;
		SPORTx->SP_DIRECT_CTRL4 |= SP_RX_FIFO_1_REG_1_L_SEL(rx_chn);
		break;
	case RX_FIFO1_REG1_R:
		SPORTx->SP_DIRECT_CTRL4 &= ~SP_MASK_RX_FIFO_1_REG_1_R_SEL;
		SPORTx->SP_DIRECT_CTRL4 |= SP_RX_FIFO_1_REG_1_R_SEL(rx_chn);
		break;
	default:
		break;
	}

}

/**
  * @brief  Set AUDIO SPORT TX FIFO enable or disable.
  * @param  index: sport index.
  * @param  fifo_num: tx fifo number.
  * @param  NewState: enable or disable.
  * @retval None
  */
void AUDIO_SP_TXSetFifo(u32 index, u32 fifo_num, u32 NewState)
{
	assert_param(IS_SP_SEL_TX_FIFO(fifo_num));

	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	if (NewState == ENABLE) {
		if (fifo_num == SP_TX_FIFO2) {
			SPORTx->SP_CTRL1 |= SP_BIT_TX_FIFO_0_REG_0_EN;
		} else if (fifo_num == SP_TX_FIFO4) {
			SPORTx->SP_CTRL1 |= SP_BIT_TX_FIFO_0_REG_0_EN | SP_BIT_TX_FIFO_0_REG_1_EN;
		} else if (fifo_num == SP_TX_FIFO6) {
			SPORTx->SP_CTRL1 |= (SP_BIT_TX_FIFO_0_REG_0_EN | SP_BIT_TX_FIFO_0_REG_1_EN | SP_BIT_TX_FIFO_1_REG_0_EN);
		} else {
			SPORTx->SP_CTRL1 |= (SP_BIT_TX_FIFO_0_REG_0_EN | SP_BIT_TX_FIFO_0_REG_1_EN | SP_BIT_TX_FIFO_1_REG_0_EN | SP_BIT_TX_FIFO_1_REG_1_EN);
		}
	} else {
		if (fifo_num == SP_TX_FIFO2) {
			SPORTx->SP_CTRL1 &= ~(SP_BIT_TX_FIFO_0_REG_0_EN);
		} else if (fifo_num == SP_TX_FIFO4) {
			SPORTx->SP_CTRL1 &= ~(SP_BIT_TX_FIFO_0_REG_0_EN | SP_BIT_TX_FIFO_0_REG_1_EN);
		} else if (fifo_num == SP_TX_FIFO6) {
			SPORTx->SP_CTRL1 &= ~(SP_BIT_TX_FIFO_0_REG_0_EN | SP_BIT_TX_FIFO_0_REG_1_EN | SP_BIT_TX_FIFO_1_REG_0_EN);
		} else {
			SPORTx->SP_CTRL1 &= ~(SP_BIT_TX_FIFO_0_REG_0_EN | SP_BIT_TX_FIFO_0_REG_1_EN | SP_BIT_TX_FIFO_1_REG_0_EN | SP_BIT_TX_FIFO_1_REG_1_EN);
		}
	}
}

/**
  * @brief  Set AUDIO SPORT RX FIFO enable or disable.
  * @param  index: sport index.
  * @param  fifo_num: rx fifo number.
  * @param  NewState: enable or disable.
  * @retval None
  */
void AUDIO_SP_RXSetFifo(u32 index, u32 fifo_num, u32 NewState)
{
	assert_param(IS_SP_SEL_RX_FIFO(fifo_num));

	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	if (NewState == ENABLE) {
		if (fifo_num == SP_RX_FIFO2) {
			SPORTx->SP_CTRL1 |= SP_BIT_RX_FIFO_0_REG_0_EN;
		} else if (fifo_num == SP_RX_FIFO4) {
			SPORTx->SP_CTRL1 |= SP_BIT_RX_FIFO_0_REG_0_EN | SP_BIT_RX_FIFO_0_REG_1_EN;
		} else if (fifo_num == SP_RX_FIFO6) {
			SPORTx->SP_CTRL1 |= (SP_BIT_RX_FIFO_0_REG_0_EN | SP_BIT_RX_FIFO_0_REG_1_EN | SP_BIT_RX_FIFO_1_REG_0_EN);
		} else {
			SPORTx->SP_CTRL1 |= (SP_BIT_RX_FIFO_0_REG_0_EN | SP_BIT_RX_FIFO_0_REG_1_EN | SP_BIT_RX_FIFO_1_REG_0_EN | SP_BIT_RX_FIFO_1_REG_1_EN);
		}

	} else {
		if (fifo_num == SP_RX_FIFO2) {
			SPORTx->SP_CTRL1 &= ~(SP_BIT_RX_FIFO_0_REG_0_EN);
		} else if (fifo_num == SP_RX_FIFO4) {
			SPORTx->SP_CTRL1 &= ~(SP_BIT_RX_FIFO_0_REG_0_EN | SP_BIT_RX_FIFO_0_REG_1_EN);
		} else if (fifo_num == SP_RX_FIFO6) {
			SPORTx->SP_CTRL1 &= ~(SP_BIT_RX_FIFO_0_REG_0_EN | SP_BIT_RX_FIFO_0_REG_1_EN | SP_BIT_RX_FIFO_1_REG_0_EN);
		} else {
			SPORTx->SP_CTRL1 &= ~(SP_BIT_RX_FIFO_0_REG_0_EN | SP_BIT_RX_FIFO_0_REG_1_EN | SP_BIT_RX_FIFO_1_REG_0_EN | SP_BIT_RX_FIFO_1_REG_1_EN);
		}
	}
}

/**
  * @brief  Initializes the AUDIO SPORT registers according to the specified parameters in SP_InitStruct.
  * @param  index: 0 ~ 3.
  * @param  direction: SP_DIR_TX or SP_DIR_RX.
  * @param  SP_StructInit: pointer to an SP_StructInit structure which will be initialized.
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
		AUDIO_SP_TXStart(index, DISABLE);

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
		AUDIO_SP_TXSetFifo(index, SP_InitStruct->SP_SelFIFO, ENABLE);

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
		AUDIO_SP_RXStart(index, DISABLE);

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
		AUDIO_SP_SetTXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_InitStruct->SP_SelTDM, chn_len);
		AUDIO_SP_SetRXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_InitStruct->SP_SelTDM, chn_len);

		/* Configure RX FIFO Channel */
		AUDIO_SP_RXSetFifo(index, SP_InitStruct->SP_SelFIFO, ENABLE);

		/* Configure SPORT parameters:RX MULTI_IO Mode*/
		if (SP_InitStruct->SP_SetMultiIO == SP_RX_MULTIIO_EN) {
			AUDIO_SP_SetTXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_RX_NOTDM, chn_len);
			AUDIO_SP_SetRXClkDiv(index, SP_InitStruct->SP_SelClk, SP_InitStruct->SP_SR, SP_RX_NOTDM, chn_len);
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
  * @param  index: select SPORT.
  * @param  NewState: new state of the SPORT TX.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void AUDIO_SP_TXStart(u32 index, u32 NewState)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

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
  * @param  index: select SPORT.
  * @param  NewState: new state of the SPORT RX.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void AUDIO_SP_RXStart(u32 index, u32 NewState)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

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
  * @param  index: select SPORT.
  * @param  NewState: new state of the handshake of GDMA-SPORT.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void AUDIO_SP_DmaCmd(u32 index, u32 NewState)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	if (NewState == ENABLE) {
		SPORTx->SP_CTRL0 &= ~ SP_BIT_DSP_CTL_MODE;
	} else {
		SPORTx->SP_CTRL0 |= SP_BIT_DSP_CTL_MODE;
	}
}

/**
  * @brief  Set SPORT self loopback mode.
  * @param  index: select SPORT.
  * @retval None
  */
void AUDIO_SP_SetSelfLPBK(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	SPORTx->SP_CTRL0 |= SP_BIT_LOOPBACK;
}

/**
  * @brief  Set the AUDIO SPORT word length.
  * @param  index: select SPORT.
  * @param  SP_TX_WordLen: the value of word length.
  *          This parameter can be one of the following values:
  *            @arg SP_TXWL_16: sample bit is 16 bit
  *            @arg SP_TXWL_20: sample bit is 20 bit
  *            @arg SP_TXWL_24: sample bit is 24 bit
  *            @arg SP_TXWL_32: sample bit is 32 bit
  * @retval None
  */

void AUDIO_SP_SetTXWordLen(u32 index, u32 SP_TX_WordLen)
{
	assert_param(IS_SP_TX_WL(SP_TX_WordLen));
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	SPORTx->SP_CTRL0 &= ~(SP_MASK_DATA_LEN_SEL_TX_0);
	SPORTx->SP_CTRL0 |= SP_DATA_LEN_SEL_TX_0(SP_TX_WordLen);

	SPORTx->SP_DIRECT_CTRL1 &= ~(SP_MASK_DATA_LEN_SEL_TX_1);
	SPORTx->SP_DIRECT_CTRL1 |=  SP_DATA_LEN_SEL_TX_1(SP_TX_WordLen);
}

/**
  * @brief  Set the AUDIO SPORT word length.
  * @param  index: select SPORT.
  * @param  SP_RX_WordLen: the value of word length.
  *          This parameter can be one of the following values:
  *            @arg SP_RXWL_16: sample bit is 16 bit
  *            @arg SP_RXWL_20: sample bit is 20 bit
  *            @arg SP_RXWL_24: sample bit is 24 bit
  *            @arg SP_RXWL_32: sample bit is 32 bit
  * @retval None
  */

void AUDIO_SP_SetRXWordLen(u32 index, u32 SP_RX_WordLen)
{
	assert_param(IS_SP_RX_WL(SP_RX_WordLen));
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	SPORTx->SP_FORMAT &= ~(SP_MASK_DATA_LEN_SEL_RX_0);
	SPORTx->SP_FORMAT |= SP_DATA_LEN_SEL_RX_0(SP_RX_WordLen);

	SPORTx->SP_DIRECT_CTRL1 &= ~(SP_MASK_DATA_LEN_SEL_RX_1);
	SPORTx->SP_DIRECT_CTRL1 |=  SP_DATA_LEN_SEL_RX_1(SP_RX_WordLen);
}

/**
  * @brief Get AUDIO SPORT word length.
  * @param  index: select SPORT.
  * @retval the value of word length.
  *            @arg 0: sample bit is 16 bit
  *            @arg 1: sample bit is 20 bit
  *            @arg 2: sample bit is 24 bit
  *            @arg 3: sample bit is 32 bit
  */
u32 AUDIO_SP_GetTXWordLen(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	u32 len_TX = ((SPORTx->SP_CTRL0) & SP_MASK_DATA_LEN_SEL_TX_0) >> 12;
	return len_TX;
}

/**
  * @brief Get AUDIO SPORT word length.
  * @param  index: select SPORT.
  * @retval the value of word length.
  *            @arg 0: sample bit is 16 bit
  *            @arg 1: sample bit is 20 bit
  *            @arg 2: sample bit is 24 bit
  *            @arg 3: sample bit is 32 bit
  */
u32 AUDIO_SP_GetRXWordLen(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	u32 len_RX = ((SPORTx->SP_FORMAT) & SP_MASK_DATA_LEN_SEL_RX_0) >> 12;
	return len_RX;
}


/**
  * @brief  Set the AUDIO SPORT channel number.
  * @param  index: select SPORT.
  * @param  SP_MonoStereo: mono or stereo.
  *          This parameter can be one of the following values:
  *            @arg SP_CH_STEREO: stereo channel, channel number is 0
  *            @arg SP_CH_MONO: mono channel, channel number is 1
  * @retval None
  */
void AUDIO_SP_SetMonoStereo(u32 index, u32 SP_MonoStereo)
{
	assert_param(IS_SP_CHN_NUM(SP_MonoStereo));
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	if (SP_MonoStereo == SP_CH_STEREO) {
		SPORTx->SP_CTRL0 &= ~(SP_BIT_EN_I2S_MONO_TX_0);
	} else {
		SPORTx->SP_CTRL0 &= ~SP_BIT_TRX_SAME_CH;
		SPORTx->SP_CTRL0 |= SP_BIT_EN_I2S_MONO_TX_0;
	}
}

/**
  * @brief  SPORT2 and SPORT3 can be set to master or slave mode.
  * @param  index: select SPORT.
  * @param  SP_MasterSlave: master or slave.
  *          This parameter can be one of the following values:
  *            @arg MASTER: SPORT master mode
  *            @arg SLAVE: SPORT slave mode
  * @retval None
  */
void AUDIO_SP_SetMasterSlave(u32 index, u32 SP_MasterSlave)
{
	assert_param(IS_SP_SEL_MODE(SP_MasterSlave));
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;

	if (SPORTx == AUDIO_SPORT2_DEV || SPORTx == AUDIO_SPORT3_DEV) {
		if (SP_MasterSlave == MASTER) {
			SPORTx->SP_CTRL0 &= ~(SP_BIT_SLAVE_CLK_SEL | SP_BIT_SLAVE_DATA_SEL);
		} else {
			SPORTx->SP_CTRL0 |= SP_BIT_SLAVE_CLK_SEL | SP_BIT_SLAVE_DATA_SEL;
		}
	} else {
		//RTK_LOGE(TAG, "Not support: Only SPORT2&3 supprot!\n");
		return;
	}
}

/**
  * @brief  Initialize GDMA peripheral single block mode for sending data.
  * @param  Index: the index of sport.
  * @param  SelGDMA: GDMA_INT or GDMA_EXT.
  * @param  GDMA_InitStruct: pointer to a GDMA_InitTypeDef structure that contains
  *         the configuration information for the GDMA peripheral.
  * @param  CallbackData: GDMA callback data.
  * @param  CallbackFunc: GDMA callback function.
  * @param  pTXData: TX Buffer.
  * @param  Length: TX length.
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
	GdmaChnl = GDMA_ChnlAlloc(0, CallbackFunc, (u32)CallbackData, INT_PRI_MIDDLE);
	if (GdmaChnl == 0xFF) {
		// No Available DMA channel
		return _FALSE;
	}

	_memset((void *)GDMA_InitStruct, 0, sizeof(GDMA_InitTypeDef));

	/*set GDMA initial structure member value*/
	GDMA_InitStruct->MuliBlockCunt = 0;
	GDMA_InitStruct->GDMA_ReloadSrc = 0;
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
		//RTK_LOGE(TAG, "AUDIO_SP_TXGDMA_Init: Aligment Err: pTXData=%p,  Length=%lu\n", pTXData, Length);
		return _FALSE;
	}
	GDMA_InitStruct->GDMA_DstMsize = MsizeFour;
	GDMA_InitStruct->GDMA_DstDataWidth = TrWidthFourBytes;

	/*configure GDMA source address */
	GDMA_InitStruct->GDMA_SrcAddr = (u32)pTXData;


	/*	Enable GDMA for TX */
	GDMA_Init(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, GDMA_InitStruct);
	GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, ENABLE);
	//RTK_LOGD(TAG, "TXGDMA_End\n");
	return _TRUE;
}

/**
  * @brief  Initialize GDMA peripheral single block mode for receiving data.
  * @param  Index: index of sport.
  * @param  SelGDMA: GDMA_INT or GDMA_EXT.
  * @param  GDMA_InitStruct: pointer to a GDMA_InitTypeDef structure that contains
  *         the configuration information for the GDMA peripheral.
  * @param  CallbackData: GDMA callback data.
  * @param  CallbackFunc: GDMA callback function.
  * @param  pRXData: RX Buffer.
  * @param  Length: RX length.
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
	GdmaChnl = GDMA_ChnlAlloc(0, CallbackFunc, (u32)CallbackData, INT_PRI_MIDDLE);
	if (GdmaChnl == 0xFF) {
		// No Available DMA channel
		return _FALSE;
	}

	_memset((void *)GDMA_InitStruct, 0, sizeof(GDMA_InitTypeDef));


	/*set GDMA initial structure member value*/
	GDMA_InitStruct->MuliBlockCunt = 0;
	GDMA_InitStruct->GDMA_ReloadSrc = 0;
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
	//RTK_LOGD(TAG, "RXGDMA_End\n");

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
  * @brief  Initialize GDMA peripheral linklist mode for sending data.
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
	GdmaChnl = GDMA_ChnlAlloc(0, CallbackFunc, (u32)CallbackData, INT_PRI_MIDDLE);
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
		//RTK_LOGE(TAG, "AUDIO_SP_TXGDMA_Init: Aligment Err: pTxData=0x%lX,  Length=%lu\n", pTxData, Length);
		return _FALSE;
	}

	for (i = 0; i < MaxLLP; i++) {
		Lli[i].BlockSize = GDMA_InitStruct->GDMA_BlockSize;
	}

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
	GdmaChnl = GDMA_ChnlAlloc(0, CallbackFunc, (u32)CallbackData, INT_PRI_MIDDLE);
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
  * @param  comp_val, it should be comp_val={32~8191}.
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
  * @brief  Set SPORT phase latch.
  * @param  index: select SPORT.
  */
void AUDIO_SP_SetPhaseLatch(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	SPORTx->SP_RX_LRCLK |= SP_BIT_EN_FS_PHASE_LATCH;
}

/**
  * @brief  Set SPORT tx counter value.
  * @param  index: select SPORT.
  */
u32 AUDIO_SP_GetTXCounterVal(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	u32 tx_counter = ((SPORTx->SP_DSP_COUNTER) & SP_MASK_TX_SPORT_COUNTER) >> 5;
	return tx_counter;
}

/**
  * @brief  Get SPORT tx phase value.
  * @param  index: select SPORT.
  */
u32 AUDIO_SP_GetTXPhaseVal(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	u32 tx_phase = (SPORTx->SP_DSP_COUNTER) & SP_MASK_TX_FS_PHASE_RPT;
	return tx_phase;
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
	u32 rx_counter = ((SPORTx->SP_RX_COUNTER2) & SP_MASK_RX_SPORT_COUNTER) >> 5;
	return rx_counter;
}

/**
  * @brief  Get SPORT RX phase.
  * @param  index: select SPORT.
  */
u32 AUDIO_SP_GetRXPhaseVal(u32 index)
{
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[index].SPORTx;
	u32 rx_phase = (SPORTx->SP_RX_COUNTER2) & SP_MASK_RX_FS_PHASE_RPT;
	return rx_phase;
}

/**
  * @brief  Set SPORT directout mode.
  * @param  index_src: select directout source SPORT.
  * @param  index_dir: select directout SPORT.
  */
void AUDIO_SP_SetDirectOutMode(u32 index_src, u32 index_dir)
{
	AUDIO_SPORT_TypeDef *SPORTx_DIR = AUDIO_DEV_TABLE[index_dir].SPORTx;
	SPORTx_DIR->SP_CTRL1 |= SP_BIT_DIRECT_MODE_EN;
	SPORTx_DIR->SP_CTRL1 &= ~SP_MASK_DIRECT_SRC_SEL;
	SPORTx_DIR->SP_CTRL1 |= SP_DIRECT_SRC_SEL(index_src);
}

/**
  * @brief  Deinit SPORT.
  * @param  index: select SPORT.
  * @param  direction: SP_DIR_TX OR SP_DIR_RX.
  */
void AUDIO_SP_Deinit(u32 index, u32 direction)
{
	assert_param(IS_SP_SET_DIR(direction));

	if (direction == SP_DIR_TX) {
		AUDIO_SP_Unregister(index, direction);
		AUDIO_SP_TXStart(index, DISABLE);
	} else {
		AUDIO_SP_Unregister(index, direction);
		AUDIO_SP_RXStart(index, DISABLE);
	}

}


/******************* (C) COPYRIGHT 2017 Realtek Semiconductor *****END OF FILE****/
