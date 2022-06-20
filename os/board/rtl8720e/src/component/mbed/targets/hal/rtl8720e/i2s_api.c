/** mbed Microcontroller Library
  ******************************************************************************
  * @file    i2s_api.c
  * @author
  * @version V1.0.0
  * @date    2016-08-01
  * @brief   This file provides mbed API for I2S.
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
#include "i2s_api.h"
#include "pinmap.h"
#include "ameba_soc.h"

#define SP_MAX_DMA_PAGE_NUM 8

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @defgroup MBED_I2S
 *  @brief      MBED_I2S driver modules.
 *  @{
 */

/** @defgroup MBED_I2S_Exported_Types MBED_I2S Exported Types
  * @{
  */

typedef struct {
	VOID (*TxCCB)(uint32_t id, char *pbuf);
	u32 TxCBId;
	VOID (*RxCCB)(uint32_t id, char *pbuf);
	u32 RxCBId;
} I2S_USER_CB;

typedef struct {
	u8 tx_gdma_own;
	u32 tx_addr;
} TX_BLOCK, *pTX_BLOCK;

typedef struct {
	TX_BLOCK tx_block[SP_MAX_DMA_PAGE_NUM];
	TX_BLOCK tx_zero_block;
	u8 tx_gdma_cnt;
	u8 tx_usr_cnt;
	u8 tx_empty_flag;
	u8 tx_page_num;
	u16 tx_page_size;

} SP_TX_INFO, *pSP_TX_INFO;

typedef struct {
	u8 rx_gdma_own;
	u32 rx_addr;

} RX_BLOCK, *pRX_BLOCK;

typedef struct {
	RX_BLOCK rx_block[SP_MAX_DMA_PAGE_NUM];
	RX_BLOCK rx_full_block;
	u8 rx_gdma_cnt;
	u8 rx_usr_cnt;
	u8 rx_full_flag;
	u8 rx_page_num;
	u16 rx_page_size;

} SP_RX_INFO, *pSP_RX_INFO;

typedef struct {
	GDMA_InitTypeDef       	SpTxGdmaInitStruct;
	GDMA_InitTypeDef       	SpRxGdmaInitStruct;
} SP_GDMA_STRUCT, *pSP_GDMA_STRUCT;

static SP_InitTypeDef SP_InitStruct;
static SP_GDMA_STRUCT SPGdmaStruct;
static SP_TX_INFO sp_tx_info;
static SP_RX_INFO sp_rx_info;
static I2S_USER_CB I2SUserCB; //Pointer to I2S User Callback


/**
  * @}
  */

/** @defgroup MBED_I2S_Exported_Functions MBED_I2S Exported Functions
  * @{
  */

static void i2s_release_tx_page(void)
{
	pTX_BLOCK ptx_block = &(sp_tx_info.tx_block[sp_tx_info.tx_gdma_cnt]);

	if (sp_tx_info.tx_empty_flag) {
	} else {
		ptx_block->tx_gdma_own = 0;
		sp_tx_info.tx_gdma_cnt++;
		if (sp_tx_info.tx_gdma_cnt == sp_tx_info.tx_page_num) {
			sp_tx_info.tx_gdma_cnt = 0;
		}
	}
}

static u32 *i2s_get_ready_tx_page(void)
{
	pTX_BLOCK ptx_block = &(sp_tx_info.tx_block[sp_tx_info.tx_gdma_cnt]);

	if (ptx_block->tx_gdma_own) {
		sp_tx_info.tx_empty_flag = 0;
		return (u32 *)ptx_block->tx_addr;
	} else {
		sp_tx_info.tx_empty_flag = 1;
		return (u32 *)sp_tx_info.tx_zero_block.tx_addr;	//for audio buffer empty case
	}
}

static void i2s_release_rx_page(void)
{
	pRX_BLOCK prx_block = &(sp_rx_info.rx_block[sp_rx_info.rx_gdma_cnt]);

	if (sp_rx_info.rx_full_flag) {
	} else {
		prx_block->rx_gdma_own = 0;
		sp_rx_info.rx_gdma_cnt++;
		if (sp_rx_info.rx_gdma_cnt == sp_rx_info.rx_page_num) {
			sp_rx_info.rx_gdma_cnt = 0;
		}
	}
}

static u32 *i2s_get_free_rx_page(void)
{
	pRX_BLOCK prx_block = &(sp_rx_info.rx_block[sp_rx_info.rx_gdma_cnt]);


	if (prx_block->rx_gdma_own) {
		sp_rx_info.rx_full_flag = 0;
		return (u32 *)prx_block->rx_addr;
	} else {

		sp_rx_info.rx_full_flag = 1;
		return (u32 *)sp_rx_info.rx_full_block.rx_addr;	//for audio buffer full case
	}
}

static void I2S_TX_ISR(void)
{
	char *tx_addr;

	SP_GDMA_STRUCT *gs = &SPGdmaStruct;
	PGDMA_InitTypeDef GDMA_InitStruct;
	GDMA_InitStruct = &(gs->SpTxGdmaInitStruct);

	/* Clear Pending ISR */
	GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

	i2s_release_tx_page();
	tx_addr = (char *)i2s_get_ready_tx_page();
	AUDIO_SP_TXGDMA_Restart(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, (u32)tx_addr, sp_tx_info.tx_page_size);
}

static void I2S_RX_ISR(void)
{
	char *rx_addr;

	SP_GDMA_STRUCT *gs = &SPGdmaStruct;
	PGDMA_InitTypeDef GDMA_InitStruct;
	GDMA_InitStruct = &(gs->SpRxGdmaInitStruct);

	/* Clear Pending ISR */
	GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
	i2s_release_rx_page();

	//read data
	pRX_BLOCK prx_block = &(sp_rx_info.rx_block[sp_rx_info.rx_usr_cnt]);
	DCache_CleanInvalidate(((u32)prx_block->rx_addr & CACHE_LINE_ADDR_MSK), (sp_rx_info.rx_page_size + CACHE_LINE_SIZE));   //clean before read rx page
	I2SUserCB.RxCCB(0, (void *)(u32)prx_block->rx_addr);

	rx_addr = (char *)i2s_get_free_rx_page();
	AUDIO_SP_RXGDMA_Restart(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, (u32)rx_addr, sp_rx_info.rx_page_size);
}

/**
  * @brief  Initializes the I2S device, include clock/function/interrupt/I2S registers.
  * @param  obj: i2s object define in application software.
  * @param  sck: Serial clock PinName according to pinmux spec.
  * @param  ws: Word select PinName according to pinmux spec.
  * @param  sd_tx: Tx PinName according to pinmux spec.
  * @param  sd_rx: Rx PinName according to pinmux spec.
  * @param  mck: Master clock PinName according to pinmux spec.
  * @retval none
  */
void i2s_init(i2s_t *obj, PinName sck, PinName ws, PinName sd_tx, PinName sd_rx, PinName mck)
{
	DBG_PRINTF(MODULE_I2S, LEVEL_INFO, "%s: Use I2S%d \n", __func__, obj->i2s_idx);

	assert_param(IS_SP_SEL_I2S(obj->i2s_idx));

	u32 tmp;
	u32 clock;
	RCC_PeriphClockSource_SPORT(CKSL_I2S_XTAL40M);

	if (obj->i2s_idx == I2S1) {
		RCC_PeriphClockCmd(APBPeriph_SPORT1, APBPeriph_SPORT1_CLOCK, ENABLE);
		Pinmux_Config(mck, PINMUX_FUNCTION_I2S1_MCLK);
		Pinmux_Config(sck, PINMUX_FUNCTION_I2S1_BCLK);
		Pinmux_Config(ws, PINMUX_FUNCTION_I2S1_WS);
		Pinmux_Config(sd_tx, PINMUX_FUNCTION_I2S1_DIO3);
		Pinmux_Config(sd_rx, PINMUX_FUNCTION_I2S1_DIO0);
		AUDIO_SP_SetPinMux(I2S1, DOUT0_FUNC);
		AUDIO_SP_SetPinMux(I2S1, DIN0_FUNC);

	} else {
		RCC_PeriphClockCmd(APBPeriph_SPORT0, APBPeriph_SPORT0_CLOCK, ENABLE);
		Pinmux_Config(mck, PINMUX_FUNCTION_I2S0_MCLK);
		Pinmux_Config(sck, PINMUX_FUNCTION_I2S0_BCLK);
		Pinmux_Config(ws, PINMUX_FUNCTION_I2S0_WS);
		Pinmux_Config(sd_tx, PINMUX_FUNCTION_I2S0_DIO3);
		Pinmux_Config(sd_rx, PINMUX_FUNCTION_I2S0_DIO0);
		AUDIO_SP_SetPinMux(I2S0, DOUT0_FUNC);
		AUDIO_SP_SetPinMux(I2S0, DIN0_FUNC);

	}

	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[obj->i2s_idx].SPORTx;
	/*Sport Init*/
	AUDIO_SP_Reset(SPORTx);
	AUDIO_SP_StructInit(&SP_InitStruct);
	SP_InitStruct.SP_SelI2S0MonoStereo = obj->channel_num;
	SP_InitStruct.SP_SelWordLen = obj->word_length;
	SP_InitStruct.SP_SelChLen = obj->channel_length;
	SP_InitStruct.SP_SR = obj->sampling_rate;
	SP_InitStruct.SP_SetMultiIO = SP_TX_MULTIIO_EN;
	SP_InitStruct.SP_SetMultiIO = SP_RX_MULTIIO_EN;
	SP_InitStruct.SP_SelClk = clock;
	if (obj->direction == I2S_DIR_TX) {
		AUDIO_SP_Init(obj->i2s_idx, SP_DIR_TX, &SP_InitStruct);
	} else {
		AUDIO_SP_Init(obj->i2s_idx, SP_DIR_RX, &SP_InitStruct);
	}

	AUDIO_SP_SetMasterSlave(SPORTx, obj->role);
}

/**
  * @brief  Sets page number, page size, page address.
  * @param  obj: i2s object define in application software.
  * @param  tx_buf: pointer to the start address of Tx page.
  * @param  rx_buf: pointer to the start address of Rx page.
  * @param  page_num: page number. This parameter must be set to a value in the 2~4 range
  * @param  page_size: page size. This parameter must be set to a value in the 4~16384 bytes range
  * @retval none
  */
void i2s_set_dma_buffer(i2s_t *obj, char *tx_buf, char *rx_buf,
						uint32_t page_num, uint32_t page_size)
{
	//uint8_t i2s_idx = obj->i2s_idx;
	u32 i;

	if ((page_num < 2) || (page_num > 8) || (page_size < 8)) {
		DBG_PRINTF(MODULE_I2S, LEVEL_INFO, "%s: PageNum(%d) valid value is 2~8; PageSize(%d must > 8)\r\n", \
				   __FUNCTION__, page_num, page_size);
		return;
	}

	if (obj->direction == I2S_DIR_TX) {

		sp_tx_info.tx_gdma_cnt = 0;
		sp_tx_info.tx_usr_cnt = 0;
		sp_tx_info.tx_empty_flag = 0;

		for (i = 0; i < page_num; i++) {
			sp_tx_info.tx_block[i].tx_gdma_own = 0;
			sp_tx_info.tx_block[i].tx_addr = (u32)tx_buf + i * page_size;
		}

		sp_tx_info.tx_zero_block.tx_addr = (u32)tx_buf + page_num * page_size;

		for (i = 0; i < page_size; i++) {
			((u8 *)(sp_tx_info.tx_zero_block.tx_addr))[i] = 0;
		}

		sp_tx_info.tx_page_size = page_size;
		sp_tx_info.tx_page_num = page_num;

	} else {

		sp_rx_info.rx_gdma_cnt = 0;
		sp_rx_info.rx_usr_cnt = 0;
		sp_rx_info.rx_full_flag = 0;

		for (i = 0; i < page_num; i++) {
			sp_rx_info.rx_block[i].rx_gdma_own = 1;
			sp_rx_info.rx_block[i].rx_addr = (u32)rx_buf + i * page_size;
		}

		sp_rx_info.rx_full_block.rx_addr = (u32)rx_buf + page_num * page_size;

		for (i = 0; i < page_size; i++) {
			((u8 *)(sp_rx_info.rx_full_block.rx_addr))[i] = 0;
		}

		sp_rx_info.rx_page_size = page_size;
		sp_rx_info.rx_page_num = page_num;


	}

}


/**
  * @brief  Sets TX interrupt handler.
  * @param  obj: i2s object define in application software.
  * @param  handler: TX interrupt callback function.
  * @param  id: TX interrupt callback function parameter.
  * @retval none
  */
void i2s_tx_irq_handler(i2s_t *obj, i2s_irq_handler handler, uint32_t id)
{
	(void) obj;
	(void) handler;
	(void) id;

	u32 *tx_addr = (u32 *)i2s_get_ready_tx_page();
	AUDIO_SP_TXGDMA_Init(obj->i2s_idx, GDMA_INT, &SPGdmaStruct.SpTxGdmaInitStruct, &SPGdmaStruct, (IRQ_FUN)I2S_TX_ISR, (u8 *)tx_addr, sp_tx_info.tx_page_size);
}

/**
  * @brief  Sets RX interrupt handler.
  * @param  obj: i2s object define in application software.
  * @param  handler: RX interrupt callback function.
  * @param  id: RX interrupt callback function parameter.
  * @retval none
  */
void i2s_rx_irq_handler(i2s_t *obj, i2s_irq_handler handler, uint32_t id)
{
	(void) obj;
	(void) id;

	I2SUserCB.RxCCB = handler;

	u32 *rx_addr = (u32 *)i2s_get_free_rx_page();
	AUDIO_SP_RXGDMA_Init(obj->i2s_idx, GDMA_INT, &SPGdmaStruct.SpRxGdmaInitStruct, &SPGdmaStruct, (IRQ_FUN)I2S_RX_ISR, (u8 *)rx_addr, sp_rx_info.rx_page_size);
}

/**
  * @brief  Sets i2s data transfer direction.
  * @param  obj: i2s object define in application software.
  * @param  trx_type: transfer direction.
  *          This parameter can be one of the following values:
  *            @arg I2S_DIR_RX: Rx receive direction
  *            @arg I2S_DIR_TX: Tx transmission direction
  *            @arg I2S_DIR_TXRX: Tx & Rx bi-direction
  * @retval none
  */
void i2s_set_direction(i2s_t *obj, int trx_type)
{
	obj->direction = trx_type;

	if (trx_type == I2S_DIR_TX) {
		AUDIO_SP_Init(obj->i2s_idx, SP_DIR_TX, &SP_InitStruct);
	} else {
		AUDIO_SP_Init(obj->i2s_idx, SP_DIR_RX, &SP_InitStruct);
	}
}

/**
  * @brief  Sets i2s channel number, sample rate, word length.
  * @param  obj: i2s object define in application software.
  * @param  channel_num: this parameter can be one of the following values:
  *            @arg CH_STEREO: stereo channel
  *            @arg CH_MONO: mono channel
  * @param  rate: this parameter can be one of the following values:
  *            @arg SR_8KHZ: sample rate is 8kHz
  *            @arg SR_12KHZ: sample rate is 12kHz
  *            @arg SR_16KHZ: sample rate is 16kHz
  *            @arg SR_24KHZ: sample rate is 24kHz
  *            @arg SR_32KHZ: sample rate is 32kHz
  *            @arg SR_48KHZ: sample rate is 48kHz
  *            @arg SR_64KHZ: sample rate is 64kHz
  *            @arg SR_96KHZ: sample rate is 96kHz
  *            @arg SR_192KHZ: sample rate is 192kHz
  *            @arg SR_384KHZ: sample rate is 384kHz
  *            @arg SR_7p35KHZ: sample rate is 7.35kHz
  *            @arg SR_11p025KHZ: sample rate is 11.025kHz
  *            @arg SR_14p7KHZ: sample rate is 14.7kHz
  *            @arg SR_22p05KHZ: sample rate is 22.05kHz
  *            @arg SR_29p4KHZ: sample rate is 29.4kHz
  *            @arg SR_44p1KHZ: sample rate is 44.1kHz
  *            @arg SR_58p8KHZ: sample rate is 58.8kHz
  *            @arg SR_88p2KHZ: sample rate is 88.2kHz
  *            @arg SR_176p4KHZ: sample rate is 176.4kHz
  * @param  word_len: this parameter can be one of the following values:
  *            @arg WL_16b: sample bit is 16 bit
  *            @arg WL_24b: sample bit is 24 bit
  *            @arg WL_32b: sample bit is 32 bit
  * @retval none
  */
void i2s_set_param(i2s_t *obj, int channel_num, int rate, int word_len)
{
	obj->channel_num = channel_num;
	obj->sampling_rate = rate;
	obj->word_length = word_len;

	SP_InitStruct.SP_SelI2S0MonoStereo = channel_num;
	SP_InitStruct.SP_SelWordLen = word_len;
	SP_InitStruct.SP_SR = rate;

	if (obj->direction == I2S_DIR_TX) {
		AUDIO_SP_Init(obj->i2s_idx, SP_DIR_TX, &SP_InitStruct);
	} else {
		AUDIO_SP_Init(obj->i2s_idx, SP_DIR_RX, &SP_InitStruct);
	}
}

/**
  * @brief  Deinitializes the I2S device, include function/interrupt/I2S registers.
  * @param  obj: i2s object define in application software.
  * @retval none
  */
void i2s_deinit(i2s_t *obj)
{
	if (obj->i2s_idx == I2S1) {
		RCC_PeriphClockCmd(APBPeriph_SPORT1, APBPeriph_SPORT1_CLOCK, DISABLE);
	} else {
		RCC_PeriphClockCmd(APBPeriph_SPORT0, APBPeriph_SPORT0_CLOCK, DISABLE);
	}
}

/**
  * @brief  Gets current tx page address.
  * @param  obj: i2s object define in application software.
  * @retval address of current tx page or NULL
  * @note current page own by cpu, return address of current tx page
  * @note current page own by i2s, return NULL
  */
int *i2s_get_tx_page(i2s_t *obj)
{
	/* To avoid gcc warnings */
	(void) obj;

	pTX_BLOCK ptx_block = &(sp_tx_info.tx_block[sp_tx_info.tx_usr_cnt]);

	if (ptx_block->tx_gdma_own) {
		return NULL;
	} else {
		return (int *)ptx_block->tx_addr;
	}

}

/**
  * @brief  Sets current tx page own by i2s.
  * @param  obj: i2s object define in application software.
  * @param  pbuf: tx buffer adderss.
  * @retval none
  */
void i2s_send_page(i2s_t *obj, uint32_t *pbuf)
{
	/* To avoid gcc warnings */
	(void) obj;

	pTX_BLOCK ptx_block = &(sp_tx_info.tx_block[sp_tx_info.tx_usr_cnt]);

	memcpy((void *)ptx_block->tx_addr, pbuf, sp_tx_info.tx_page_size);
	ptx_block->tx_gdma_own = 1;
	sp_tx_info.tx_usr_cnt++;
	if (sp_tx_info.tx_usr_cnt == sp_tx_info.tx_page_num) {
		sp_tx_info.tx_usr_cnt = 0;
	}

}

/**
  * @brief  Sets current rx page own by i2s.
  * @param  obj: i2s object define in application software.
  * @retval none
  */
void i2s_recv_page(i2s_t *obj)
{
	/* To avoid gcc warnings */
	(void) obj;

	pRX_BLOCK prx_block = &(sp_rx_info.rx_block[sp_rx_info.rx_usr_cnt]);

	prx_block->rx_gdma_own = 1;
	sp_rx_info.rx_usr_cnt++;
	if (sp_rx_info.rx_usr_cnt == sp_rx_info.rx_page_num) {
		sp_rx_info.rx_usr_cnt = 0;
	}

}

/**
  * @brief  Enable i2s interrupt and function.
  * @param  obj: i2s object define in application software.
  * @retval none
  */
void i2s_enable(i2s_t *obj)
{

	/* To avoid gcc warnings */
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[obj->i2s_idx].SPORTx;

	if (obj->direction == I2S_DIR_TX) {
		AUDIO_SP_DmaCmd(SPORTx, ENABLE);
		AUDIO_SP_TXStart(SPORTx, ENABLE);
	} else {
		AUDIO_SP_DmaCmd(SPORTx, ENABLE);
		AUDIO_SP_RXStart(SPORTx, ENABLE);
	}

}

/**
  * @brief  Disable i2s interrupt and function.
  * @param  obj: i2s object define in application software.
  * @retval none
  */
void i2s_disable(i2s_t *obj)
{
	/* To avoid gcc warnings */
	AUDIO_SPORT_TypeDef *SPORTx = AUDIO_DEV_TABLE[obj->i2s_idx].SPORTx;

	if (obj->direction == I2S_DIR_TX) {
		GDMA_ClearINT(SPGdmaStruct.SpTxGdmaInitStruct.GDMA_Index, SPGdmaStruct.SpTxGdmaInitStruct.GDMA_ChNum);
		GDMA_Cmd(SPGdmaStruct.SpTxGdmaInitStruct.GDMA_Index, SPGdmaStruct.SpTxGdmaInitStruct.GDMA_ChNum, DISABLE);
		GDMA_ChnlFree(SPGdmaStruct.SpTxGdmaInitStruct.GDMA_Index, SPGdmaStruct.SpTxGdmaInitStruct.GDMA_ChNum);

		AUDIO_SP_DmaCmd(SPORTx, DISABLE);
		AUDIO_SP_TXStart(SPORTx, DISABLE);

	} else {

		GDMA_ClearINT(SPGdmaStruct.SpRxGdmaInitStruct.GDMA_Index, SPGdmaStruct.SpRxGdmaInitStruct.GDMA_ChNum);
		GDMA_Cmd(SPGdmaStruct.SpRxGdmaInitStruct.GDMA_Index, SPGdmaStruct.SpRxGdmaInitStruct.GDMA_ChNum, DISABLE);
		GDMA_ChnlFree(SPGdmaStruct.SpRxGdmaInitStruct.GDMA_Index, SPGdmaStruct.SpRxGdmaInitStruct.GDMA_ChNum);

		AUDIO_SP_DmaCmd(SPORTx, DISABLE);
		AUDIO_SP_RXStart(SPORTx, DISABLE);
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
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/

