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
#include "ameba_soc.h"
#include "ameba_audio_clock.h"
#include "amebasmart_i2s.h"

#define SP_MAX_DMA_PAGE_NUM 8

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @defgroup MBED_I2S
 *  @brief    MBED_I2S driver modules.
 *  @{
 */

/** @defgroup MBED_I2S_Exported_Types MBED_I2S Exported Types
  * @{
  */

typedef struct {
	VOID (*TxCCB)(uint32_t id, char *pbuf);
	uint32_t TxCBId;
	VOID (*RxCCB)(uint32_t id, char *pbuf);
	uint32_t RxCBId;
} I2S_USER_CB;

typedef struct {
	uint8_t tx_gdma_own;
	uint32_t tx_addr;
} TX_BLOCK, *pTX_BLOCK;

typedef struct {
	TX_BLOCK tx_block[SP_MAX_DMA_PAGE_NUM];
	TX_BLOCK tx_zero_block;
	uint8_t tx_gdma_cnt;
	uint8_t tx_usr_cnt;
	uint8_t tx_empty_flag;
	uint8_t tx_page_num;
	uint16_t tx_page_size;

} SP_TX_INFO, *pSP_TX_INFO;

typedef struct {
	uint8_t rx_gdma_own;
	uint32_t rx_addr;

} RX_BLOCK, *pRX_BLOCK;

typedef struct {
	RX_BLOCK rx_block[SP_MAX_DMA_PAGE_NUM];
	RX_BLOCK rx_full_block;
	uint8_t rx_gdma_cnt;
	uint8_t rx_usr_cnt;
	uint8_t rx_full_flag;
	uint8_t rx_page_num;
	uint16_t rx_page_size;

} SP_RX_INFO, *pSP_RX_INFO;

typedef struct {
	GDMA_InitTypeDef       SpTxGdmaInitStruct;	/* Pointer to GDMA_InitTypeDef */
	GDMA_InitTypeDef       SpRxGdmaInitStruct;	/* Pointer to GDMA_InitTypeDef */
	uint8_t                i2s_idx;
} SP_GDMA_STRUCT;

static SP_InitTypeDef SP_InitStruct[I2S_NUM_MAX];
static SP_GDMA_STRUCT SPGdmaStruct[I2S_NUM_MAX];
static SP_TX_INFO sp_tx_info[I2S_NUM_MAX];
static SP_RX_INFO sp_rx_info[I2S_NUM_MAX];
static I2S_USER_CB I2SUserCB[I2S_NUM_MAX]; /* Pointer to I2S User Callback */
static struct GDMA_CH_LLI LliTx[I2S_NUM_MAX][SP_MAX_DMA_PAGE_NUM];
static struct GDMA_CH_LLI LliRx[I2S_NUM_MAX][SP_MAX_DMA_PAGE_NUM];

/**
  * @}
  */

/** @defgroup MBED_I2S_Exported_Functions MBED_I2S Exported Functions
  * @{
  */

static void i2s_release_tx_page(uint8_t i2s_index)
{
	pTX_BLOCK ptx_block = &(sp_tx_info[i2s_index].tx_block[sp_tx_info[i2s_index].tx_gdma_cnt]);

	if (sp_tx_info[i2s_index].tx_empty_flag) {
	} else {
		ptx_block->tx_gdma_own = 0;
		sp_tx_info[i2s_index].tx_gdma_cnt++;
		if (sp_tx_info[i2s_index].tx_gdma_cnt == sp_tx_info[i2s_index].tx_page_num) {
			sp_tx_info[i2s_index].tx_gdma_cnt = 0;
		}
	}
}

static uint32_t *i2s_get_ready_tx_page(uint8_t i2s_index)
{
	pTX_BLOCK ptx_block = &(sp_tx_info[i2s_index].tx_block[sp_tx_info[i2s_index].tx_gdma_cnt]);

	if (ptx_block->tx_gdma_own) {
		sp_tx_info[i2s_index].tx_empty_flag = 0;
		return (uint32_t *)ptx_block->tx_addr;
	} else {
		sp_tx_info[i2s_index].tx_empty_flag = 1;
		return (uint32_t *)sp_tx_info[i2s_index].tx_zero_block.tx_addr;	//for audio buffer empty case
	}
}

static void i2s_release_rx_page(uint8_t i2s_index)
{
	pRX_BLOCK prx_block = &(sp_rx_info[i2s_index].rx_block[sp_rx_info[i2s_index].rx_gdma_cnt]);

	if (sp_rx_info[i2s_index].rx_full_flag) {
	} else {
		prx_block->rx_gdma_own = 0;
		sp_rx_info[i2s_index].rx_gdma_cnt++;
		if (sp_rx_info[i2s_index].rx_gdma_cnt == sp_rx_info[i2s_index].rx_page_num) {
			sp_rx_info[i2s_index].rx_gdma_cnt = 0;
		}
	}
}

static uint32_t *i2s_get_free_rx_page(uint8_t i2s_index)
{
	pRX_BLOCK prx_block = &(sp_rx_info[i2s_index].rx_block[sp_rx_info[i2s_index].rx_gdma_cnt]);

	if (prx_block->rx_gdma_own) {
		sp_rx_info[i2s_index].rx_full_flag = 0;
		return (uint32_t *)prx_block->rx_addr;
	} else {
		sp_rx_info[i2s_index].rx_full_flag = 1;
		return (uint32_t *)sp_rx_info[i2s_index].rx_full_block.rx_addr;	//for audio buffer full case
	}
}

static void i2s_tx_isr(void *sp_data)
{
	SP_GDMA_STRUCT *gs = sp_data;
	PGDMA_InitTypeDef GDMA_InitStruct;
	GDMA_InitStruct = &(gs->SpTxGdmaInitStruct);

	uint8_t i2s_index = gs->i2s_idx;

	/* Clear Pending ISR */
	GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);

	i2s_release_tx_page(i2s_index);
	i2s_get_ready_tx_page(i2s_index);
}

static void i2s_rx_isr(void *sp_data)
{
	SP_GDMA_STRUCT *gs = sp_data;
	PGDMA_InitTypeDef GDMA_InitStruct;
	GDMA_InitStruct = &(gs->SpRxGdmaInitStruct);

	uint8_t i2s_index = gs->i2s_idx;

	/* Clear Pending ISR */
	GDMA_ClearINT(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
	i2s_release_rx_page(i2s_index);

	/* Read data */
	pRX_BLOCK prx_block = &(sp_rx_info[i2s_index].rx_block[sp_rx_info[i2s_index].rx_usr_cnt]);
	DCache_CleanInvalidate((uint32_t)prx_block->rx_addr, sp_rx_info[i2s_index].rx_page_size);
	I2SUserCB[i2s_index].RxCCB((uint32_t)NULL, (void *)(uint32_t)prx_block->rx_addr);
	i2s_get_free_rx_page(i2s_index);
}

static void i2s_pinmux_config(uint32_t pin_name, uint32_t pin_func)
{
	if (pin_name != _PNC) {
		Pinmux_Config(pin_name, pin_func);
	} else {
		return;
	}
}

/**
  * @brief  Set page number, page size and page address.
  * @param  obj: I2S object defined in application software.
  * @param  tx_buf: Pointer to the start address of Tx page.
  * @param  rx_buf: Pointer to the start address of Rx page.
  * @param  page_num: Page number. This parameter must be set to a value in the 2~8 range.
  * @param  page_size: Page size. This parameter must be set to a value in the 2~32768 bytes range.
  * @retval none
  */
void i2s_set_dma_buffer(i2s_t *obj, char *tx_buf, char *rx_buf,
						uint32_t page_num, uint32_t page_size)
{
	uint8_t i2s_idx = obj->i2s_idx;
	uint32_t i, j;

	if ((page_num < 2) || (page_num > 8) || (page_size < 8)) {
		DBG_PRINTF(MODULE_I2S, LEVEL_INFO, "%s: PageNum(%d) valid value is 2~8; PageSize(%d must > 8)\r\n", \
				   __FUNCTION__, page_num, page_size);
		return;
	}

	if (obj->direction == I2S_DIR_TX) {

		sp_tx_info[i2s_idx].tx_gdma_cnt = 0;
		sp_tx_info[i2s_idx].tx_usr_cnt = 0;
		sp_tx_info[i2s_idx].tx_empty_flag = 0;

		for (i = 0; i < page_num; i++) {
			sp_tx_info[i2s_idx].tx_block[i].tx_gdma_own = 0;
			sp_tx_info[i2s_idx].tx_block[i].tx_addr = (uint32_t)tx_buf + i * page_size;
		}

		sp_tx_info[i2s_idx].tx_zero_block.tx_addr = (uint32_t)tx_buf + page_num * page_size;

		for (i = 0; i < page_size; i++) {
			((uint8_t *)(sp_tx_info[i2s_idx].tx_zero_block.tx_addr))[i] = 0;
		}

		sp_tx_info[i2s_idx].tx_page_size = page_size;
		sp_tx_info[i2s_idx].tx_page_num = page_num;

		for (j = 0; j < page_num + 1; j++) {

			if (j == 0) {
				LliTx[i2s_idx][j].LliEle.Sarx = (uint32_t)tx_buf + (page_num) * page_size;
			} else {
				LliTx[i2s_idx][j].LliEle.Sarx = (uint32_t)tx_buf + (j - 1) * page_size;
			}

			if (j == page_num) {
				LliTx[i2s_idx][j].pNextLli = &LliTx[i2s_idx][1];
			} else {
				LliTx[i2s_idx][j].pNextLli = &LliTx[i2s_idx][j + 1];
			}

		}
	} else {
		sp_rx_info[i2s_idx].rx_gdma_cnt = 0;
		sp_rx_info[i2s_idx].rx_usr_cnt = 0;
		sp_rx_info[i2s_idx].rx_full_flag = 0;

		for (i = 0; i < page_num; i++) {
			sp_rx_info[i2s_idx].rx_block[i].rx_gdma_own = 1;
			sp_rx_info[i2s_idx].rx_block[i].rx_addr = (uint32_t)rx_buf + i * page_size;
		}

		sp_rx_info[i2s_idx].rx_full_block.rx_addr = (uint32_t)rx_buf + page_num * page_size;

		for (i = 0; i < page_size; i++) {
			((uint8_t *)(sp_rx_info[i2s_idx].rx_full_block.rx_addr))[i] = 0;
		}

		sp_rx_info[i2s_idx].rx_page_size = page_size;
		sp_rx_info[i2s_idx].rx_page_num = page_num;

		for (j = 0; j < page_num; j++) {
			LliRx[i2s_idx][j].LliEle.Darx = (uint32_t)rx_buf + j * page_size;
			if (j == page_num - 1) {
				LliRx[i2s_idx][j].pNextLli = &LliRx[i2s_idx][0];
			} else {
				LliRx[i2s_idx][j].pNextLli = &LliRx[i2s_idx][j + 1];
			}
		}
	}
}


/**
  * @brief  Register TX interrupt handler.
  * @param  obj: I2S object defined in application software.
  * @param  handler: TX interrupt callback function.
  * @param  id: TX interrupt callback parameter.
  * @retval none
  */
void i2s_tx_irq_handler(i2s_t *obj, i2s_irq_handler handler, uint32_t id)
{
	(void) handler;
	(void) id;
	uint8_t i2s_index = obj->i2s_idx;
	SP_GDMA_STRUCT *sp_str = &SPGdmaStruct[i2s_index];
	sp_str->i2s_idx = i2s_index;	/* Store I2S index */

	i2s_get_ready_tx_page(i2s_index);
	AUDIO_SP_LLPTXGDMA_Init(i2s_index, GDMA_INT, &sp_str->SpTxGdmaInitStruct, sp_str, (IRQ_FUN )handler, sp_tx_info[i2s_index].tx_page_size,
							sp_tx_info[i2s_index].tx_page_num + 1, LliTx[i2s_index]);
	
}

/**
  * @brief  Register RX interrupt handler.
  * @param  obj: I2S object defined in application software.
  * @param  handler: RX interrupt callback function.
  * @param  id: RX interrupt callback parameter.
  * @retval none
  */
void i2s_rx_irq_handler(i2s_t *obj, i2s_irq_handler handler, uint32_t id)
{
	(void) obj;
	(void) id;
	uint8_t i2s_index = obj->i2s_idx;
	SP_GDMA_STRUCT *sp_str = &SPGdmaStruct[i2s_index];
	sp_str->i2s_idx = i2s_index;	/* Store I2S index */

	I2SUserCB[i2s_index].RxCCB = handler;

	i2s_get_free_rx_page(i2s_index);
	AUDIO_SP_LLPRXGDMA_Init(i2s_index, GDMA_INT, &sp_str->SpRxGdmaInitStruct, sp_str, (IRQ_FUN)handler, sp_rx_info[i2s_index].rx_page_size,
							sp_rx_info[i2s_index].rx_page_num, LliRx[i2s_index]);
}

/**
  * @brief  Set I2S data transfer direction.
  * @param  obj: I2S object defined in application software.
  * @param  trx_type: Transfer direction. This parameter can be one of the following values:
  *   @arg I2S_DIR_RX: Rx receive direction.
  *   @arg I2S_DIR_TX: Tx transmission direction.
  * @retval none
  */
void i2s_set_direction(i2s_t *obj, int trx_type)
{
	obj->direction = trx_type;

	AUDIO_SP_Init(obj->i2s_idx, obj->direction, &(SP_InitStruct[obj->i2s_idx]));
}

/**
  * @brief  Set I2S channel number, sample rate and word length.
  * @param  obj: I2S object defined in application software.
  * @param  channel_num: This parameter can be one of the following values:
  *   @arg CH_STEREO: Stereo channel.
  *   @arg CH_MONO: Mono channel.
  * @param  rate: This parameter can be one of the following values:
  *   @arg SR_8KHZ: Sample rate is 8kHz.
  *   @arg SR_12KHZ: Sample rate is 12kHz.
  *   @arg SR_16KHZ: Sample rate is 16kHz.
  *   @arg SR_24KHZ: Sample rate is 24kHz.
  *   @arg SR_32KHZ: Sample rate is 32kHz.
  *   @arg SR_48KHZ: Sample rate is 48kHz.
  *   @arg SR_64KHZ: Sample rate is 64kHz.
  *   @arg SR_96KHZ: Sample rate is 96kHz.
  *   @arg SR_192KHZ: Sample rate is 192kHz.
  *   @arg SR_384KHZ: Sample rate is 384kHz.
  *   @arg SR_11p025KHZ: Sample rate is 11.025kHz.
  *   @arg SR_22p05KHZ: Sample rate is 22.05kHz.
  *   @arg SR_44p1KHZ: Sample rate is 44.1kHz.
  *   @arg SR_88p2KHZ: Sample rate is 88.2kHz.
  *   @arg SR_176p4KHZ: Sample rate is 176.4kHz.
  * @param  word_len: This parameter can be one of the following values:
  *   @arg WL_16b: Sample bit is 16 bit.
  *   @arg WL_24b: Sample bit is 24 bit.
  *   @arg WL_32b: Sample bit is 32 bit.
  * @retval none
  */
void i2s_set_param(i2s_t *obj, int channel_num, int rate, int word_len)
{
	uint32_t clock_mode;
	AUDIO_ClockParams Clock_Params;
	AUDIO_InitParams Init_Params;

	obj->channel_num = channel_num;
	obj->sampling_rate = rate;
	obj->word_length = word_len;

	Init_Params.chn_len = SP_CL_32;
	Init_Params.chn_cnt = obj->channel_num;
	Init_Params.sr = obj->sampling_rate;;
	Init_Params.codec_multiplier_with_rate = 256;
	Init_Params.sport_mclk_fixed_max = (uint32_t) NULL;
	Audio_Clock_Choose(PLL_CLK, &Init_Params, &Clock_Params);
	obj->clock = Clock_Params.Clock;

	switch (Clock_Params.Clock) {
	case PLL_CLOCK_24P576M:
		PLL_I2S_24P576M(ENABLE);
		RCC_PeriphClockSource_SPORT(obj->i2s_idx, CKSL_I2S_PLL24M);
		PLL_I2S_Div(obj->i2s_idx, Clock_Params.PLL_DIV);
		clock_mode = PLL_CLOCK_24P576M / Clock_Params.PLL_DIV;
		break;

	case PLL_CLOCK_98P304M:
		PLL_I2S_98P304M(ENABLE);
		RCC_PeriphClockSource_SPORT(obj->i2s_idx, CKSL_I2S_PLL98M);
		PLL_I2S_Div(obj->i2s_idx, Clock_Params.PLL_DIV);
		clock_mode = PLL_CLOCK_98P304M / Clock_Params.PLL_DIV;
		break;
	}

	/* Sport Init */
	AUDIO_SP_Reset(obj->i2s_idx);
	AUDIO_SP_StructInit(&SP_InitStruct[obj->i2s_idx]);

	SP_InitStruct[obj->i2s_idx].SP_SelFIFO = obj->fifo_num;
	SP_InitStruct[obj->i2s_idx].SP_SelChLen = obj->channel_length;
	SP_InitStruct[obj->i2s_idx].SP_SetMultiIO = obj->mode;
	SP_InitStruct[obj->i2s_idx].SP_SR = obj->sampling_rate;
	SP_InitStruct[obj->i2s_idx].SP_SelClk = clock_mode;
	AUDIO_SP_Init(obj->i2s_idx, obj->direction, &(SP_InitStruct[obj->i2s_idx]));
}


/**
  * @brief  Initialize the I2S device, including clock, function, interrupt and I2S registers.
  * @param  obj: I2S object defined in application software.
  * @param  sck: Serial clock PinName according to pinmux spec.
  * @param  ws: Word select PinName according to pinmux spec.
  * @param  sd_tx: Tx PinName according to pinmux spec.
  * @param  sd_rx: Rx PinName according to pinmux spec.
  * @param  mck: Master clock PinName according to pinmux spec.
  * @retval none
  */
void i2s_init(i2s_t *obj, PinName sck, PinName ws, PinName sd_tx, PinName sd_rx, PinName mck)
{
	uint32_t pin_func;

	assert_param(IS_SP_SEL_I2S(obj->i2s_idx));

	if (obj->i2s_idx == I2S_NUM_2) {
		pin_func = PINMUX_FUNCTION_I2S2;

		/*Enable SPORT/AUDIO CODEC CLOCK and Function*/
		RCC_PeriphClockCmd(APBPeriph_SPORT2, APBPeriph_SPORT2_CLOCK, ENABLE);
	} else {
		pin_func = PINMUX_FUNCTION_I2S3;

		/*Enable SPORT/AUDIO CODEC CLOCK and Function*/
		RCC_PeriphClockCmd(APBPeriph_SPORT3, APBPeriph_SPORT3_CLOCK, ENABLE);
	}

	i2s_pinmux_config(mck, pin_func);
	i2s_pinmux_config(sck, pin_func);
	i2s_pinmux_config(ws, pin_func);
	i2s_pinmux_config(sd_tx, pin_func);
	i2s_pinmux_config(sd_rx, pin_func);

	i2s_set_param(obj, obj->channel_num, obj->sampling_rate, obj->word_length);

	if (obj->direction == SP_DIR_TX) {
		AUDIO_SP_TXCHNSrcSel(obj->i2s_idx, TXCHN2, TX_FIFO0_REG0_L);
		AUDIO_SP_TXCHNSrcSel(obj->i2s_idx, TXCHN3, TX_FIFO0_REG0_R);
	} else {
		AUDIO_SP_SetMasterSlave(obj->i2s_idx, obj->role);  //only smart support, testchip not support.
		AUDIO_SP_RXFIFOSrcSel(obj->i2s_idx, RX_FIFO0_REG0_L, RXCHN6);
		AUDIO_SP_RXFIFOSrcSel(obj->i2s_idx, RX_FIFO0_REG0_R, RXCHN7);
	}
}

/**
  * @brief  Deinitialize the I2S device, including function, interrupt and I2S registers.
  * @param  obj: I2S object defined in application software.
  * @retval none
  */
void i2s_deinit(i2s_t *obj)
{
	switch (obj->clock) {
	case PLL_CLOCK_24P576M:
		PLL_I2S_24P576M(DISABLE);
		break;

	case PLL_CLOCK_98P304M:
		PLL_I2S_98P304M(DISABLE);
		break;
	}

	if (obj->i2s_idx == I2S2) {
		RCC_PeriphClockCmd(APBPeriph_SPORT2, APBPeriph_SPORT2_CLOCK, DISABLE);
	} else {
		RCC_PeriphClockCmd(APBPeriph_SPORT3, APBPeriph_SPORT3_CLOCK, DISABLE);
	}
}

/**
  * @brief  Get current tx page address.
  * @param  obj: I2S object defined in application software.
  * @return Address of current tx page if it is owned by CPU or NULL if it is owned by I2S.
  */
int *i2s_get_tx_page(i2s_t *obj)
{
	uint8_t i2s_index = obj->i2s_idx;

	pTX_BLOCK ptx_block = &(sp_tx_info[i2s_index].tx_block[sp_tx_info[i2s_index].tx_usr_cnt]);

	if (ptx_block->tx_gdma_own) {
		return NULL;
	} else {
		return (int *)ptx_block->tx_addr;
	}

}

/**
  * @brief  Set current tx page owned by I2S.
  * @param  obj: I2S object defined in application software.
  * @param  pbuf: Tx buffer adderss.
  * @retval none
  */
void i2s_send_page(i2s_t *obj, uint32_t *pbuf)
{
	uint8_t i2s_index = obj->i2s_idx;

	pTX_BLOCK ptx_block = &(sp_tx_info[i2s_index].tx_block[sp_tx_info[i2s_index].tx_usr_cnt]);

	memcpy((void *)ptx_block->tx_addr, pbuf, sp_tx_info[i2s_index].tx_page_size);
	DCache_CleanInvalidate((uint32_t)ptx_block->tx_addr, sp_tx_info[i2s_index].tx_page_size);
	ptx_block->tx_gdma_own = 1;
	sp_tx_info[i2s_index].tx_usr_cnt++;
	if (sp_tx_info[i2s_index].tx_usr_cnt == sp_tx_info[i2s_index].tx_page_num) {
		sp_tx_info[i2s_index].tx_usr_cnt = 0;
	}
}

/**
  * @brief  Set current rx page owned by I2S.
  * @param  obj: I2S object defined in application software.
  * @retval none
  */
void i2s_recv_page(i2s_t *obj)
{
	uint8_t i2s_index = obj->i2s_idx;

	pRX_BLOCK prx_block = &(sp_rx_info[i2s_index].rx_block[sp_rx_info[i2s_index].rx_usr_cnt]);

	prx_block->rx_gdma_own = 1;
	sp_rx_info[i2s_index].rx_usr_cnt++;
	if (sp_rx_info[i2s_index].rx_usr_cnt == sp_rx_info[i2s_index].rx_page_num) {
		sp_rx_info[i2s_index].rx_usr_cnt = 0;
	}
}

/**
  * @brief  Enable I2S interrupt and function.
  * @param  obj: I2S object defined in application software.
  * @retval none
  */
void i2s_enable(i2s_t *obj)
{
	AUDIO_SP_DmaCmd(obj->i2s_idx, ENABLE);
	if (obj->direction == I2S_DIR_TX) {
		AUDIO_SP_TXStart(obj->i2s_idx, ENABLE);
	} else {
		AUDIO_SP_RXStart(obj->i2s_idx, ENABLE);
	}
}

/**
  * @brief  Disable I2S interrupt and function.
  * @param  obj: I2S object defined in application software.
  * @retval none
  */
void i2s_disable(i2s_t *obj)
{
	SP_GDMA_STRUCT *l_SPGdmaStruct = &SPGdmaStruct[obj->i2s_idx];

	if (obj->direction == I2S_DIR_TX) {
		GDMA_ClearINT(l_SPGdmaStruct->SpTxGdmaInitStruct.GDMA_Index, l_SPGdmaStruct->SpTxGdmaInitStruct.GDMA_ChNum);
		GDMA_Cmd(l_SPGdmaStruct->SpTxGdmaInitStruct.GDMA_Index, l_SPGdmaStruct->SpTxGdmaInitStruct.GDMA_ChNum, DISABLE);
		GDMA_ChnlFree(l_SPGdmaStruct->SpTxGdmaInitStruct.GDMA_Index, l_SPGdmaStruct->SpTxGdmaInitStruct.GDMA_ChNum);

		AUDIO_SP_DmaCmd(obj->i2s_idx, DISABLE);
		AUDIO_SP_TXStart(obj->i2s_idx, DISABLE);
	} else {

		GDMA_ClearINT(l_SPGdmaStruct->SpRxGdmaInitStruct.GDMA_Index, l_SPGdmaStruct->SpRxGdmaInitStruct.GDMA_ChNum);
		GDMA_Cmd(l_SPGdmaStruct->SpRxGdmaInitStruct.GDMA_Index, l_SPGdmaStruct->SpRxGdmaInitStruct.GDMA_ChNum, DISABLE);
		GDMA_ChnlFree(l_SPGdmaStruct->SpRxGdmaInitStruct.GDMA_Index, l_SPGdmaStruct->SpRxGdmaInitStruct.GDMA_ChNum);

		AUDIO_SP_DmaCmd(obj->i2s_idx, DISABLE);
		AUDIO_SP_RXStart(obj->i2s_idx, DISABLE);
	}
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
