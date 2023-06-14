/*
 * Copyright (c) 2022 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ameba_audio_stream_render.h"

#include <inttypes.h>

#include "ameba_audio_stream_control.h"
#include "ameba_audio_stream_utils.h"
#include "ameba_audio_stream_buffer.h"
#include "ameba_audio_types.h"
#include "ameba_soc.h"
#include "ameba_audio_hw_usrcfg.h"
#include "audio_hw_debug.h"
#include "platform_stdlib.h"

#define REG_DUMP 0

int ameba_audio_stream_tx_set_amp_state(bool state)
{
	StreamControl *control = ameba_audio_get_ctl();
	if (control == NULL) {
		HAL_AUDIO_ERROR("Audio driver control is null initialized\n");
		return -1;
	}

	if (control->board_amp_pin < 0) {
		return 0;
	}

	GPIO_InitTypeDef gpio_initstruct_temp;
	gpio_initstruct_temp.GPIO_Pin = control->board_amp_pin;
	gpio_initstruct_temp.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(&gpio_initstruct_temp);

	if (state != control->amp_state) {
		if (state == true) {
			HAL_AUDIO_INFO("amp enable");
			GPIO_WriteBit(control->board_amp_pin, 1);
			vTaskDelay(AUDIO_HW_AMPLIFIER_ENABLE_TIME / portTICK_RATE_MS);
		} else {
			HAL_AUDIO_INFO("amp disable");
			GPIO_WriteBit(control->board_amp_pin, 0);
			vTaskDelay(AUDIO_HW_AMPLIFIER_DISABLE_TIME / portTICK_RATE_MS);
		}
	}

	ameba_audio_ctl_set_amp_state(control, state);

	return 0;
}

void ameba_audio_stream_tx_buffer_flush(Stream *stream)
{
	RenderStream *rstream = (RenderStream *) stream;
	if (rstream) {
		ameba_audio_stream_buffer_flush(rstream->stream.rbuffer);
	}
}

/*
 * when sport LRCLK delivered sport_compare_val frames,
 * the interrupt callback is triggered.
 */
uint32_t ameba_audio_stream_tx_sport_interrupt(void *data)
{
	RenderStream *rstream = (RenderStream *) data;

	rstream->sport_irq_count++;
	rstream->total_counter += rstream->sport_compare_val;
	if (rstream->total_counter >= rstream->total_counter_boundary) {
		rstream->total_counter -= rstream->total_counter_boundary;
		rstream->sport_irq_count = 0;
	}

	HAL_AUDIO_PVERBOSE("total_counter:%" PRIu32 " \n", rstream->total_counter);
	AUDIO_SP_ClearTXCounterIrq(rstream->stream.sport_dev_num);

	return 0;
}

uint64_t ameba_audio_stream_tx_sport_rendered_frames(Stream *stream)
{
	uint32_t counter = 0;
	/* frames totally delivered through LRCLK */
	uint64_t total_counter = 0;

	RenderStream *rstream = (RenderStream *)stream;
	if (!rstream) {
		return -1;
	}

	counter = AUDIO_SP_GetTXCounterVal(rstream->stream.sport_dev_num);
	total_counter = counter + (uint64_t)rstream->sport_irq_count * rstream->sport_compare_val;

	return total_counter;
}

static uint64_t ameba_audio_stream_tx_get_counter_ntime(RenderStream *rstream)
{
	uint64_t usec = 0;
	//current total i2s counter of audio frames;
	uint32_t now_counter = 0;
	//means the delta_counter between now counter and last irq total counter.
	uint32_t delta_counter = 0;

	if (!rstream) {
		return -1;
	}

	delta_counter = AUDIO_SP_GetTXCounterVal(rstream->stream.sport_dev_num);
	now_counter = rstream->total_counter + delta_counter;

	usec = now_counter * 1000000LL / rstream->stream.rate;
	HAL_AUDIO_PVERBOSE("now_counter:%" PRIu32 ", usec:%" PRIu64 " delta_counter:%" PRIu32 ", total:%" PRIu32 "\n",
					   now_counter, usec, delta_counter, rstream->total_counter);

	return usec;
}

int ameba_audio_stream_tx_get_htimestamp(Stream *stream, uint32_t *avail, struct timespec *tstamp)
{
	uint64_t usec;

	RenderStream *rstream = (RenderStream *)stream;
	if (!rstream) {
		return -1;
	}

	//tstamp get is us
	usec = ameba_audio_stream_tx_get_counter_ntime(rstream);

	//remove codec delay which is 36samples.
	usec -= 36 * 1000000LL / rstream->stream.rate;
	usec += rstream->trigger_tstamp;

	//tv_sec is lld, tv_nsec is ld
	tstamp->tv_sec = usec / 1000000;
	tstamp->tv_nsec = (long)((usec - tstamp->tv_sec * 1000000LL) * 1000);

	*avail = ameba_audio_stream_buffer_get_available_size(rstream->stream.rbuffer) / rstream->stream.frame_size;

	HAL_AUDIO_PVERBOSE("avail:%" PRIu32 ", trigger:%" PRIu32 ", usec:%" PRIu64 ", tv_sec:%" PRIu64 ", tv_nsec:%" PRIu32 "",
					   *avail, rstream->trigger_tstamp, usec, tstamp->tv_sec, tstamp->tv_nsec);

	return 0;
}

static void ameba_audo_stream_tx_codec_configure(uint32_t i2s, uint32_t type, uint32_t channels, I2S_InitTypeDef *i2s_initstruct)
{
	AUDIO_CODEC_SetI2SIP(i2s, ENABLE);
	AUDIO_CODEC_SetI2SSRC(i2s, INTERNAL_SPORT);
	AUDIO_CODEC_SetI2SParameters(i2s, DACPATH, i2s_initstruct);

	AUDIO_CODEC_SetDACSRSrc(SOURCE0, i2s_initstruct->CODEC_SelI2STxSR);
	AUDIO_CODEC_EnableDAC(DAC_L, ENABLE);
	AUDIO_CODEC_SetDACHPF(DAC_L, ENABLE);
	AUDIO_CODEC_SetDACMute(DAC_L, UNMUTE);
	AUDIO_CODEC_SetDACSrc(i2s, I2SL, I2SR);

	if (channels == 2) {
		AUDIO_CODEC_EnableDAC(DAC_R, ENABLE);
		AUDIO_CODEC_SetDACHPF(DAC_R, ENABLE);
		AUDIO_CODEC_SetDACMute(DAC_R, UNMUTE);
	}

	//5V power supply, the gain is set to 0X86 by default, otherwise clipping;
	//12V power supply, the maximum gain can be set to 0x96, otherwise clipping
	if (ameba_audio_get_ctl() == NULL) {
		AUDIO_CODEC_SetDACVolume(DAC_L, 0x50);
		if (channels == 2) {
			AUDIO_CODEC_SetDACVolume(DAC_R, 0x50);
		}
	} else {
		AUDIO_CODEC_SetDACVolume(DAC_L, ameba_audio_get_ctl()->volume_for_dacl);
		if (channels == 2) {
			AUDIO_CODEC_SetDACVolume(DAC_R, ameba_audio_get_ctl()->volume_for_dacr);
		}
	}

	if (type == APP_LINE_OUT) {
		if (!ameba_audio_is_audio_ip_in_use(LINEOUTLANA)) {
			AUDIO_CODEC_SetANAClk(ENABLE);
			AUDIO_CODEC_DisPAD(PAD_DACL);
			AUDIO_CODEC_SetDACPowerMode(DAC_L, POWER_ON);
			AUDIO_CODEC_SetLineOutPowerMode(DAC_L, POWER_ON);
			AUDIO_CODEC_SetLineOutMode(DAC_L, DIFF);
			AUDIO_CODEC_SetLineOutMute(DAC_L, DACIN, UNMUTE);
			ameba_audio_stream_tx_set_amp_state(true);
			ameba_audio_set_audio_ip_use_status(STREAM_OUT, LINEOUTLANA, true);
		}
		if (!ameba_audio_is_audio_ip_in_use(LINEOUTRANA)) {
			if (channels == 2) {
				AUDIO_CODEC_DisPAD(PAD_DACR);
				AUDIO_CODEC_SetDACPowerMode(DAC_R, POWER_ON);
				AUDIO_CODEC_SetLineOutPowerMode(DAC_R, POWER_ON);
				AUDIO_CODEC_SetLineOutMode(DAC_R, DIFF);
				AUDIO_CODEC_SetLineOutMute(DAC_R, DACIN, UNMUTE);
				ameba_audio_set_audio_ip_use_status(STREAM_OUT, LINEOUTRANA, true);
			}
		}
	} else if (type == APP_HPO_OUT) {
		if (!ameba_audio_is_audio_ip_in_use(HPOOUTLANA)) {
			AUDIO_CODEC_SetANAClk(ENABLE);
			AUDIO_CODEC_DisPAD(PAD_DACL);
			AUDIO_CODEC_SetDACPowerMode(DAC_L, POWER_ON);
			AUDIO_CODEC_SetHPOPowerMode(DAC_L, POWER_ON);
			AUDIO_CODEC_SetHPOMode(DAC_L, SINGLE);
			AUDIO_CODEC_SetHPOMute(DAC_L, DACIN, UNMUTE);
			ameba_audio_set_audio_ip_use_status(STREAM_OUT, HPOOUTLANA, true);
		}
		if (!ameba_audio_is_audio_ip_in_use(HPOOUTRANA)) {
			if (channels == 2) {
				AUDIO_CODEC_DisPAD(PAD_DACR);
				AUDIO_CODEC_SetDACPowerMode(DAC_R, POWER_ON);
				AUDIO_CODEC_SetHPOPowerMode(DAC_R, POWER_ON);
				AUDIO_CODEC_SetHPOMode(DAC_R, SINGLE);
				AUDIO_CODEC_SetHPOMute(DAC_R, DACIN, UNMUTE);
				ameba_audio_set_audio_ip_use_status(STREAM_OUT, HPOOUTRANA, true);
			}
		}
	}
}

static void ameba_audio_stream_tx_sport_init(RenderStream **stream, StreamConfig config)
{
	RenderStream *rstream = *stream;

	AUDIO_SP_StructInit(&rstream->stream.sp_initstruct);
	rstream->stream.sp_initstruct.SP_SelI2SMonoStereo = ameba_audio_get_channel(config.channels);
	rstream->stream.sp_initstruct.SP_SelWordLen = ameba_audio_get_sp_format(config.format, rstream->stream.direction);
	rstream->stream.sp_initstruct.SP_SR = ameba_audio_get_sp_rate(config.rate);
	rstream->stream.sp_initstruct.SP_SelTDM = ameba_audio_get_tdm(config.channels);
	rstream->stream.sp_initstruct.SP_SelFIFO = ameba_audio_get_fifo_num(config.channels);
	rstream->stream.sp_initstruct.SP_SelClk = CKSL_I2S_XTAL40M;

	AUDIO_SP_Init(rstream->stream.sport_dev_num, SP_DIR_TX, &rstream->stream.sp_initstruct);
}

static void ameba_audio_stream_tx_codec_init(RenderStream **stream, StreamConfig config)
{
	RenderStream *rstream = *stream;

	if (!ameba_audio_is_audio_ip_in_use(POWER)) {
		AUDIO_CODEC_SetLDOMode(POWER_ON);
	}
	ameba_audio_set_audio_ip_use_status(rstream->stream.direction, POWER, true);

	AUDIO_CODEC_I2S_StructInit(&rstream->stream.i2s_initstruct);
	rstream->stream.i2s_initstruct.CODEC_SelI2STxSR = ameba_audio_get_codec_rate(config.rate);;
	rstream->stream.i2s_initstruct.CODEC_SelI2STxWordLen = ameba_audio_get_codec_format(config.format, rstream->stream.direction);

	switch (ameba_audio_ctl_get_device_category(ameba_audio_get_ctl())) {
	case AMEBA_AUDIO_DEVICE_SPEAKER:
		ameba_audo_stream_tx_codec_configure(I2S0, APP_LINE_OUT, config.channels, &rstream->stream.i2s_initstruct);
		break;
	case AMEBA_AUDIO_DEVICE_HEADPHONE:
		ameba_audo_stream_tx_codec_configure(I2S0, APP_HPO_OUT, config.channels, &rstream->stream.i2s_initstruct);
		break;
	default:
		HAL_AUDIO_ERROR("unsupported device:%d", ameba_audio_ctl_get_device_category(ameba_audio_get_ctl()));
		break;
	}
}

//FIX ME for 6/8channel
static void ameba_audio_stream_tx_llp_init(Stream *stream)
{
	RenderStream *rstream = (RenderStream *)stream;
	struct GDMA_CH_LLI *ch_lli = rstream->stream.gdma_ch_lli;
	if (!stream || rstream->stream.stream_mode == AMEBA_AUDIO_DMA_IRQ_MODE) {
		return;
	}
	HAL_AUDIO_INFO("ameba_audio_stream_tx_llp_init, period_count: %" PRId32 ", frame_size: %" PRId32 "", rstream->stream.period_count, rstream->stream.frame_size);

	uint32_t j;
	uint32_t tx_addr = (uint32_t)(rstream->stream.rbuffer->raw_data);

	for (j = 0; j < rstream->stream.period_count; j++) {

		DCache_CleanInvalidate(((uint32_t)tx_addr + j * rstream->stream.period_bytes), (rstream->stream.period_bytes + CACHE_LINE_SIZE));

		ch_lli[j].LliEle.Sarx = (uint32_t)tx_addr + j * rstream->stream.period_bytes;
		HAL_AUDIO_VERBOSE("ameba_audio_stream_tx_llp_init, addr: %x", ch_lli[j].LliEle.Sarx);

		if (j == rstream->stream.period_count - 1) {
			ch_lli[j].pNextLli = &ch_lli[0];
		} else {
			ch_lli[j].pNextLli = &ch_lli[j + 1];
		}

		//ch_lli[j].BlockSize = rstream->stream.period_bytes >> 2;
		ch_lli[j].BlockSize = rstream->stream.period_bytes / rstream->stream.frame_size;
		ch_lli[j].LliEle.Darx = (uint32_t)&AUDIO_DEV_TABLE[0].SPORTx->SP_TX_FIFO_0_WR_ADDR;
	}
}

Stream *ameba_audio_stream_tx_init(StreamConfig config)
{
	RenderStream *rstream;
	size_t buf_size;

	rstream = (RenderStream *)calloc(1, sizeof(RenderStream));
	if (!rstream) {
		HAL_AUDIO_ERROR("calloc stream fail");
		return NULL;
	}

	rstream->stream.direction = STREAM_OUT;
	rstream->stream.sport_dev_num = 0;

	ameba_audio_periphclock_init(rstream->stream.sport_dev_num);

	if (rstream->stream.sport_dev_num == 0 && !ameba_audio_is_audio_ip_in_use(SPORT0)) {
		AUDIO_SP_Reset(AUDIO_SPORT0_DEV);
	} else if (rstream->stream.sport_dev_num == 1 && !ameba_audio_is_audio_ip_in_use(SPORT1)) {
		AUDIO_SP_Reset(AUDIO_SPORT1_DEV);
	}

	/*configure sport according to the parameters*/
	ameba_audio_stream_tx_sport_init(&rstream, config);
	if (rstream->stream.sport_dev_num == 0) {
		ameba_audio_set_audio_ip_use_status(rstream->stream.direction, SPORT0, true);
	} else if (rstream->stream.sport_dev_num == 1) {
		ameba_audio_set_audio_ip_use_status(rstream->stream.direction, SPORT1, true);
	}

	/* enable audio IP */
	if (!ameba_audio_is_audio_ip_in_use(AUDIOIP)) {
		AUDIO_CODEC_SetAudioIP(ENABLE);
	}
	ameba_audio_set_audio_ip_use_status(rstream->stream.direction, AUDIOIP, true);

	/*configure codec according to the parameters*/
	ameba_audio_stream_tx_codec_init(&rstream, config);
	ameba_audio_set_audio_ip_use_status(rstream->stream.direction, CODEC, true);

	buf_size = config.period_size * config.frame_size * config.period_count;
	rstream->stream.rbuffer = ameba_audio_stream_buffer_create();

	if (rstream->stream.rbuffer) {
		ameba_audio_stream_buffer_alloc(rstream->stream.rbuffer, buf_size);
	}

	rstream->stream.gdma_struct = (GdmaCallbackData *)calloc(1, sizeof(GdmaCallbackData));
	if (!rstream->stream.gdma_struct) {
		HAL_AUDIO_ERROR("calloc gdma_struct fail");
		return NULL;
	}
	rstream->stream.gdma_struct->stream = (Stream *)rstream;

	rtw_init_sema(&rstream->stream.sem, 0);

	rstream->stream.stream_mode = config.mode;
	rstream->stream.frame_size = config.frame_size;
	rstream->stream.period_count = config.period_count;
	rstream->stream.period_bytes = config.period_size * config.frame_size;
	rstream->stream.rate = config.rate;
	rstream->stream.gdma_cnt = 0;
	rstream->stream.gdma_irq_cnt = 0;
	rstream->stream.start_gdma = false;
	rstream->stream.sem_need_post = false;

	rstream->restart_by_write = false;
	rstream->trigger_tstamp = 0;
	rstream->total_counter = 0;
	rstream->sport_irq_count = 0;
	rstream->sport_compare_val = config.period_size * config.period_count;
	rstream->total_counter_boundary = UINT64_MAX;

	while (rstream->sport_compare_val * 2 <= AUDIO_HW_MAX_SPORT_IRQ_X) {
		rstream->sport_compare_val *= 2;
	}

	rstream->stream.gdma_ch_lli = (struct GDMA_CH_LLI *)ameba_audio_gdma_calloc(rstream->stream.period_count, sizeof(struct GDMA_CH_LLI));

	if (!rstream->stream.gdma_ch_lli) {
		HAL_AUDIO_ERROR("calloc gdma_ch_lli fail");
		return NULL;
	}

	if (rstream->stream.stream_mode == AMEBA_AUDIO_DMA_NOIRQ_MODE) {
		ameba_audio_stream_tx_llp_init(&rstream->stream);
	}

	//enable sport interrupt
	InterruptDis(SPORT0_IRQ);
	InterruptUnRegister(SPORT0_IRQ);
	InterruptRegister((IRQ_FUN)ameba_audio_stream_tx_sport_interrupt, SPORT0_IRQ, (uint32_t)rstream, 4);
	InterruptEn(SPORT0_IRQ, 4);

	return &rstream->stream;
}

uint32_t ameba_audio_stream_tx_get_buffer_status(Stream *stream)
{
	RenderStream *rstream = (RenderStream *)stream;
	PGDMA_InitTypeDef txgdma_initstruct = &(stream->gdma_struct->u.SpTxGdmaInitStruct);

	if (!rstream || !rstream->stream.rbuffer || !rstream->stream.gdma_struct
		|| rstream->stream.stream_mode == AMEBA_AUDIO_DMA_IRQ_MODE) {
		HAL_AUDIO_ERROR("stream is not initialized\n");
		return 0;
	}

	uint32_t wr = (uint32_t)(rstream->stream.rbuffer->raw_data + rstream->stream.rbuffer->write_ptr);
	uint32_t capacity = rstream->stream.rbuffer->capacity;
	uint32_t dma_addr = GDMA_GetSrcAddr(txgdma_initstruct->GDMA_Index, txgdma_initstruct->GDMA_ChNum);
	uint32_t remain = (wr < dma_addr) ? (capacity - (dma_addr - wr)) : (wr - dma_addr);

	return remain;
}

void ameba_audio_stream_tx_start(Stream *stream)
{
	RenderStream *rstream = (RenderStream *)stream;

	rstream->trigger_tstamp = DTimestamp_Get();
	rstream->total_counter = 0;
	rstream->sport_irq_count = 0;

	ameba_audio_ctl_set_tx_mute(ameba_audio_get_ctl(), false);
	if (AUDIO_HW_AMPLIFIER_MUTE_ENABLE) {
		ameba_audio_stream_tx_set_amp_state(true);
	}

	if (rstream->stream.sport_dev_num == 0) {
		HAL_AUDIO_PVERBOSE("stream_tx_start sport(%" PRId32 ") at:%" PRId32 ", sport_compare_val:%" PRId32 "\n",
						   rstream->stream.sport_dev_num, rstream->trigger_tstamp, rstream->sport_compare_val);
		AUDIO_SP_DmaCmd(AUDIO_SPORT0_DEV, ENABLE);
		AUDIO_SP_TXStart(AUDIO_SPORT0_DEV, ENABLE);
	} else {
		AUDIO_SP_DmaCmd(AUDIO_SPORT1_DEV, ENABLE);
		AUDIO_SP_TXStart(AUDIO_SPORT1_DEV, ENABLE);
	}

	AUDIO_SP_SetTXCounterCompVal(rstream->stream.sport_dev_num, rstream->sport_compare_val);
	AUDIO_SP_SetTXCounter(rstream->stream.sport_dev_num, ENABLE);
}

void ameba_audio_stream_tx_stop(Stream *stream)
{
	RenderStream *rstream = (RenderStream *)stream;

	if (AUDIO_HW_AMPLIFIER_MUTE_ENABLE) {
		ameba_audio_stream_tx_set_amp_state(false);
	}
	ameba_audio_ctl_set_tx_mute(ameba_audio_get_ctl(), true);

	do {
		vTaskDelay(4 / portTICK_RATE_MS);
		HAL_AUDIO_PVERBOSE("waiting for tx to complete, gdma_cnt: %" PRId32 ", irq_cnt: %" PRId32 "", rstream->stream.gdma_cnt, rstream->stream.gdma_irq_cnt);
	} while (rstream->stream.gdma_cnt != rstream->stream.gdma_irq_cnt);

	rstream->trigger_tstamp = DTimestamp_Get();
	rstream->total_counter = 0;
	rstream->sport_irq_count = 0;

	if (rstream->stream.sport_dev_num == 0) {
		HAL_AUDIO_PVERBOSE("stream_tx_stop sport(%" PRId32 ") at:%" PRId32 "\n", rstream->stream.sport_dev_num, rstream->trigger_tstamp);
		AUDIO_SP_DmaCmd(AUDIO_SPORT0_DEV, DISABLE);
		AUDIO_SP_TXStart(AUDIO_SPORT0_DEV, DISABLE);
	} else {
		AUDIO_SP_DmaCmd(AUDIO_SPORT1_DEV, DISABLE);
		AUDIO_SP_TXStart(AUDIO_SPORT1_DEV, DISABLE);
	}

	AUDIO_SP_SetTXCounter(rstream->stream.sport_dev_num, DISABLE);
	ameba_audio_stream_tx_buffer_flush((Stream *)rstream);
}

//gdma done moving one period size data IRQ. Data is gdma_cb_data
uint32_t ameba_audio_stream_tx_complete(void *data)
{
	uint32_t tx_addr;
	uint32_t tx_length;

	GdmaCallbackData *gdata = (GdmaCallbackData *) data;
	PGDMA_InitTypeDef txgdma_initstruct = &(gdata->u.SpTxGdmaInitStruct);

	/* Clear Pending ISR */
	GDMA_ClearINT(txgdma_initstruct->GDMA_Index, txgdma_initstruct->GDMA_ChNum);

	RenderStream *rstream = (RenderStream *)(gdata->stream);
	ameba_audio_stream_buffer_update_tx_readptr(rstream->stream.rbuffer, rstream->stream.period_bytes);
	rstream->stream.gdma_irq_cnt++;

	if (rstream->restart_by_write == true) {
		return 0;
	}

	if (ameba_audio_stream_buffer_get_available_size(rstream->stream.rbuffer) == rstream->stream.rbuffer->capacity ||
		ameba_audio_stream_buffer_get_remain_size(rstream->stream.rbuffer) < rstream->stream.period_bytes) {
		HAL_AUDIO_INFO("buffer empty,underrun");
		rstream->restart_by_write = true;
	} else {
		tx_addr = (uint32_t)(rstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.rbuffer));
		tx_length = rstream->stream.period_bytes;
		AUDIO_SP_TXGDMA_Restart(txgdma_initstruct->GDMA_Index, txgdma_initstruct->GDMA_ChNum, tx_addr, tx_length);
		rstream->stream.gdma_cnt++;
	}

	if (rstream->stream.sem_need_post) {
		rtw_up_sema_from_isr(&rstream->stream.sem);
	}

	return 0;
}

static int ameba_audio_stream_tx_write_in_noirq_mode(Stream *stream, const void *data, uint32_t bytes, bool block)
{
	uint32_t bytes_left_to_write = bytes;
	uint32_t bytes_written = 0;

	RenderStream *rstream = (RenderStream *)stream;
	PGDMA_InitTypeDef sp_txgdma_initstruct = &(rstream->stream.gdma_struct->u.SpTxGdmaInitStruct);

	while (bytes_left_to_write != 0) {
		if (rstream->stream.start_gdma) {
			uint32_t wr = (uint32_t)(rstream->stream.rbuffer->raw_data + rstream->stream.rbuffer->write_ptr);
			uint32_t capacity = rstream->stream.rbuffer->capacity;
			uint32_t dma_addr = GDMA_GetSrcAddr(sp_txgdma_initstruct->GDMA_Index, sp_txgdma_initstruct->GDMA_ChNum);
			uint32_t avail = (wr < dma_addr) ? (dma_addr - wr) : (capacity - (wr - dma_addr));

			if (avail > bytes_left_to_write) {
				// 	HAL_AUDIO_INFO("base: %" PRId32 ", wr: %" PRId32 ", dma_addr:%" PRId32 ", capacity:%" PRId32 ", bytes_to_write: %" PRId32 "",
				// 					(uint32_t)(rstream->stream.rbuffer->raw_data), wr, dma_addr, capacity, bytes_left_to_write);
				bytes_written = ameba_audio_stream_buffer_write_in_noirq_mode(rstream->stream.rbuffer, (u8 *)data + bytes - bytes_left_to_write, bytes_left_to_write,
								rstream->stream.period_bytes);
			} else if (!block) { // non-block mode
				HAL_AUDIO_INFO("stream_tx_write no buffer available in non-block mode\n");
				return bytes - bytes_left_to_write;
			}
		} else {
			bytes_written = ameba_audio_stream_buffer_write_in_noirq_mode(rstream->stream.rbuffer, (u8 *)data + bytes - bytes_left_to_write, bytes_left_to_write,
							rstream->stream.period_bytes);
		}

		if (!rstream->stream.start_gdma) {
			HAL_AUDIO_PVERBOSE("bytes: %" PRIu32 ", rstream->stream.period_bytes:%" PRIu32 ", remain size:%" PRId32 "", bytes, rstream->stream.period_bytes,
							   ameba_audio_stream_buffer_get_remain_size(rstream->stream.rbuffer));
			if (ameba_audio_stream_buffer_get_remain_size(rstream->stream.rbuffer) >= rstream->stream.period_bytes) {
				rstream->stream.start_gdma = true;

				AUDIO_SP_LLPTXGDMA_Init(rstream->stream.sport_dev_num, GDMA_INT, sp_txgdma_initstruct, rstream->stream.gdma_struct,
										(IRQ_FUN)NULL,
										rstream->stream.period_bytes, rstream->stream.period_count, rstream->stream.gdma_ch_lli);
				AUDIO_SP_DmaCmd(AUDIO_SPORT0_DEV, ENABLE);
				AUDIO_SP_TXStart(AUDIO_SPORT0_DEV, ENABLE);
			}
		}

		bytes_left_to_write -= bytes_written;
	}

	//return 0;
	return bytes;
}

static int ameba_audio_stream_tx_write_in_irq_mode(Stream *stream, const void *data, uint32_t bytes, bool block)
{
	uint32_t bytes_left_to_write = bytes;
	uint32_t bytes_written = 0;
	uint32_t tx_addr;

	RenderStream *rstream = (RenderStream *)stream;
	PGDMA_InitTypeDef sp_txgdma_initstruct = &(rstream->stream.gdma_struct->u.SpTxGdmaInitStruct);
	(void) block;

	while (bytes_left_to_write != 0) {
		bytes_written = ameba_audio_stream_buffer_write(rstream->stream.rbuffer, (u8 *)data + bytes - bytes_left_to_write, bytes_left_to_write);

		if (!rstream->stream.start_gdma) {
			if (ameba_audio_stream_buffer_get_remain_size(rstream->stream.rbuffer) >= rstream->stream.period_bytes) {
				rstream->stream.start_gdma = true;
				tx_addr = (uint32_t)(rstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.rbuffer));
				AUDIO_SP_TXGDMA_Init(rstream->stream.sport_dev_num, GDMA_INT, sp_txgdma_initstruct, rstream->stream.gdma_struct,
									 (IRQ_FUN)ameba_audio_stream_tx_complete, (u8 *)tx_addr, rstream->stream.period_bytes);
				rstream->stream.gdma_cnt++;
				HAL_AUDIO_INFO("gdma init: index:%d, chNum:%d", sp_txgdma_initstruct->GDMA_Index, sp_txgdma_initstruct->GDMA_ChNum);

#if REG_DUMP
				ameba_audio_dump_gdma(sp_txgdma_initstruct->GDMA_ChNum);
				ameba_audio_dump_sport_regs(SPORT0_REG_BASE);
				ameba_audio_dump_codec_regs();
#endif
			}
		}

		if (rstream->restart_by_write) {
			if (ameba_audio_stream_buffer_get_remain_size(rstream->stream.rbuffer) >= rstream->stream.period_bytes) {
				rstream->restart_by_write = false;
				tx_addr = (uint32_t)(rstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.rbuffer));
				HAL_AUDIO_PVERBOSE("restart gdma at rp:%" PRId32 "", ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.rbuffer));
				AUDIO_SP_TXGDMA_Restart(sp_txgdma_initstruct->GDMA_Index, sp_txgdma_initstruct->GDMA_ChNum, tx_addr, rstream->stream.period_bytes);
				rstream->stream.gdma_cnt++;
			}
		}

		bytes_left_to_write -= bytes_written;

		if (ameba_audio_stream_buffer_get_available_size(rstream->stream.rbuffer) < bytes_left_to_write) {
			rstream->stream.sem_need_post = true;
			rtw_down_sema(&rstream->stream.sem);
		}

		rstream->stream.sem_need_post = false;
	}

	return bytes;
}

int ameba_audio_stream_tx_write(Stream *stream, const void *data, uint32_t bytes, bool block)
{
	if (stream) {
		if (stream->stream_mode) {
			return ameba_audio_stream_tx_write_in_noirq_mode(stream, data, bytes, block);
		} else {
			return ameba_audio_stream_tx_write_in_irq_mode(stream, data, bytes, block);
		}
	}

	return 0;
}

void ameba_audio_stream_tx_close(Stream *stream)
{
	RenderStream *rstream = (RenderStream *)stream;

	if (rstream) {
		GDMA_InitTypeDef sp_txgdma_initstruct = rstream->stream.gdma_struct->u.SpTxGdmaInitStruct;
		HAL_AUDIO_INFO("dma clear: index:%d, chNum:%d", sp_txgdma_initstruct.GDMA_Index, sp_txgdma_initstruct.GDMA_ChNum);

		GDMA_ClearINT(sp_txgdma_initstruct.GDMA_Index, sp_txgdma_initstruct.GDMA_ChNum);
		GDMA_Cmd(sp_txgdma_initstruct.GDMA_Index, sp_txgdma_initstruct.GDMA_ChNum, DISABLE);
		GDMA_ChnlFree(sp_txgdma_initstruct.GDMA_Index, sp_txgdma_initstruct.GDMA_ChNum);

		rstream->trigger_tstamp = DTimestamp_Get();
		if (rstream->stream.sport_dev_num == 0) {
			AUDIO_SP_DmaCmd(AUDIO_SPORT0_DEV, DISABLE);
			AUDIO_SP_TXStart(AUDIO_SPORT0_DEV, DISABLE);
		} else if (rstream->stream.sport_dev_num == 1) {
			rstream->trigger_tstamp = DTimestamp_Get();
			AUDIO_SP_DmaCmd(AUDIO_SPORT1_DEV, DISABLE);
			AUDIO_SP_TXStart(AUDIO_SPORT1_DEV, DISABLE);
		}

		AUDIO_SP_Deinit(rstream->stream.sport_dev_num, SP_DIR_TX);
		//AUDIO_CODEC_DeInit(APP_LINE_OUT);

		ameba_audio_reset_audio_ip_status((Stream *)rstream);

		rtw_free_sema(&rstream->stream.sem);
		ameba_audio_stream_buffer_release(rstream->stream.rbuffer);
		if (rstream->stream.gdma_struct) {
			free(rstream->stream.gdma_struct);
			rstream->stream.gdma_struct = NULL;
		}

		if (rstream->stream.gdma_ch_lli) {
			ameba_audio_gdma_free(rstream->stream.gdma_ch_lli);
			rstream->stream.gdma_ch_lli = NULL;
		}

		free(rstream);
	}
}
