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
#include "ameba_audio_stream_capture.h"

#include <inttypes.h>

#include "platform_stdlib.h"
#include "ameba_soc.h"
#include "ameba_audio_hw_usrcfg.h"
#include "audio_hw_debug.h"
#include "hardware/audio/audio_hw_types.h"

#include "ameba_audio_stream_control.h"
#include "ameba_audio_stream_utils.h"
#include "ameba_audio_stream_buffer.h"
#include "ameba_audio_types.h"

#define REG_DUMP 0
#define IS_6_8_CHANNEL(NUM) (((NUM) == 6) || ((NUM) == 8))

static void ameba_audio_stream_rx_sport_init(CaptureStream **stream, StreamConfig config)
{
	CaptureStream *cstream = *stream;

	AUDIO_SP_StructInit(&cstream->stream.sp_initstruct);
	cstream->stream.sp_initstruct.SP_SelI2SMonoStereo = ameba_audio_get_channel(config.channels);
	cstream->stream.sp_initstruct.SP_SelWordLen = ameba_audio_get_sp_format(config.format, cstream->stream.direction);
	cstream->stream.sp_initstruct.SP_SelTDM = ameba_audio_get_tdm(config.channels);
	cstream->stream.sp_initstruct.SP_SelFIFO = ameba_audio_get_fifo_num(config.channels);
	cstream->stream.sp_initstruct.SP_SR = ameba_audio_get_sp_rate(config.rate);
	AUDIO_SP_Init(cstream->stream.sport_dev_num, SP_DIR_RX, &cstream->stream.sp_initstruct);
}

static void ameba_audio_stream_rx_codec_adc_reset(void)
{
	HAL_AUDIO_ENTER;
	uint32_t idx = 1;
	for (; idx <= MAX_AD_NUM; idx++) {
		uint32_t adc_chn_idx = ameba_audio_stream_get_adc_chn_idx(idx);
		uint32_t adc_idx = ameba_audio_stream_get_adc_idx(idx);

		AUDIO_CODEC_EnableADC(adc_chn_idx, DISABLE);
		AUDIO_CODEC_SetADCHPF(idx, 3, DISABLE);
		AUDIO_CODEC_SetADCMute(adc_idx, MUTE);
		AUDIO_CODEC_SetADCMixMute(adc_idx, ANAAD, MUTE);
		AUDIO_CODEC_SetADCANAFilter(adc_idx, DISABLE);
		AUDIO_CODEC_SetADCMixMute(adc_idx, DMIC, MUTE);
		AUDIO_CODEC_SetADCDmicFilter(adc_idx, DISABLE);
	}
}

static void ameba_audio_stream_rx_adc_mic_configure(StreamConfig config)
{
	StreamControl *dc = ameba_audio_get_ctl();
	bool amic_inited = false;
	bool dmic_inited = false;
	uint32_t i = 1;
	uint32_t adc_num = config.channels;

	if (!dc) {
		return;
	}

	//enable adc
	if (dc->adc_use_status == 0) { // use defaut adcs
		HAL_AUDIO_INFO("configure default adc");
		uint32_t j = 1;
		for (; j <= config.channels; j++) {
			uint32_t adc_chn_idx = ameba_audio_stream_get_adc_chn_idx(j);
			uint32_t adc_idx = ameba_audio_stream_get_adc_idx(j);

			AUDIO_CODEC_EnableADC(adc_chn_idx, ENABLE);
			AUDIO_CODEC_SetADCHPF(adc_idx, 3, ENABLE);
			AUDIO_CODEC_SetADCMute(adc_idx, UNMUTE);
			AUDIO_CODEC_SetADCVolume(adc_idx, dc->volume_for_adc[j - 1]);
		}
	} else {  // use adcs according to user customize
		uint32_t k = 1;
		for (; k <= MAX_AD_NUM; k++) {
			if ((dc->adc_use_status >> (k - 1)) & 1) {
				uint32_t adc_chn_idx = ameba_audio_stream_get_adc_chn_idx(k);
				uint32_t adc_idx = ameba_audio_stream_get_adc_idx(k);

				AUDIO_CODEC_EnableADC(adc_chn_idx, ENABLE);
				AUDIO_CODEC_SetADCHPF(adc_idx, 3, ENABLE);
				AUDIO_CODEC_SetADCMute(adc_idx, UNMUTE);
				AUDIO_CODEC_SetADCVolume(adc_idx, dc->volume_for_adc[k - 1]);
			}
		}
	}

	//enable amic/dmic
	for (; i <= adc_num; i++) {
		uint32_t adc_idx = ameba_audio_stream_get_adc_idx(i);
		uint32_t adc_chn_idx = ameba_audio_stream_get_adc_chn_idx(i);
		uint32_t mic_idx = ameba_audio_stream_get_mic_idx(dc->mic_category_for_adc[i - 1]);

		HAL_AUDIO_INFO("adc_idx: %" PRIu32 ", adc_chn: %" PRIu32 ", mic_idx: %" PRIu32 ", mic_category: %" PRIu32 ", ",
					   adc_idx, adc_chn_idx, mic_idx, dc->mic_category_for_adc[i - 1]);

		switch (dc->mic_category_for_adc[i - 1]) {
		case AMEBA_AUDIO_AMIC1:
			AUDIO_CODEC_DisPAD(PAD_MIC1);
			goto common_amic_set;
		case AMEBA_AUDIO_AMIC2:
			AUDIO_CODEC_DisPAD(PAD_MIC2);
			goto common_amic_set;
		case AMEBA_AUDIO_AMIC3:
			AUDIO_CODEC_DisPAD(PAD_MIC3);
			goto common_amic_set;
		case AMEBA_AUDIO_AMIC4:
			AUDIO_CODEC_DisPAD(PAD_MIC4);
			goto common_amic_set;
		case AMEBA_AUDIO_AMIC5: {
			AUDIO_CODEC_DisPAD(PAD_MIC5);
common_amic_set:
			if (!amic_inited) {
				//AUDIO_CODEC_SetLDOMode(POWER_ON);
				AUDIO_CODEC_SetMicBiasPowerMode(POWER_ON);
				amic_inited = true;
			}
			AUDIO_CODEC_SetADCMixMute(adc_idx, ANAAD, UNMUTE); // 1 -- ADC1
			AUDIO_CODEC_SetADCANAFilter(adc_idx, ENABLE);
			AUDIO_CODEC_SetADCANASrc(adc_chn_idx, mic_idx);
			AUDIO_CODEC_SetMicBstPowerMode(mic_idx, NORMALPOWER);
			AUDIO_CODEC_SetMicBiasPCUTMode(mic_idx, POWER_ON);
			AUDIO_CODEC_SetMicBstChnMute(mic_idx, MICIN, UNMUTE);
			AUDIO_CODEC_SetMicBstInputMode(mic_idx, DIFF);
			AUDIO_CODEC_SetMicBstGain(mic_idx, ameba_audio_get_ctl()->gain_for_micbst[mic_idx - 1]);
		}
		break;
		case AMEBA_AUDIO_DMIC1:
		case AMEBA_AUDIO_DMIC2:
			Pinmux_Config(AUDIO_HW_DMIC_DATA0_PIN, PINMUX_FUNCTION_DMIC);
			goto common_dmic_set;
		case AMEBA_AUDIO_DMIC3:
		case AMEBA_AUDIO_DMIC4:
			Pinmux_Config(AUDIO_HW_DMIC_DATA1_PIN, PINMUX_FUNCTION_DMIC);
			goto common_dmic_set;
		case AMEBA_AUDIO_DMIC5:
		case AMEBA_AUDIO_DMIC6:
			Pinmux_Config(AUDIO_HW_DMIC_DATA2_PIN, PINMUX_FUNCTION_DMIC);
			goto common_dmic_set;
		case AMEBA_AUDIO_DMIC7:
		case AMEBA_AUDIO_DMIC8: {
			Pinmux_Config(AUDIO_HW_DMIC_DATA3_PIN, PINMUX_FUNCTION_DMIC);
common_dmic_set:
			if (!dmic_inited) {
				Pinmux_Config(AUDIO_HW_DMIC_CLK_PIN, PINMUX_FUNCTION_DMIC);
				HAL_AUDIO_INFO("set DMIC clock");
				if (ameba_audio_get_codec_rate(config.rate) == SR_96K) {
					AUDIO_CODEC_SetDmicClk(DMIC_5M, ENABLE);
					vTaskDelay(AUDIO_HW_DMIC_STEADY_TIME / portTICK_RATE_MS);
				} else {
					AUDIO_CODEC_SetDmicClk(DMIC_2P5M, ENABLE);
					vTaskDelay(AUDIO_HW_DMIC_STEADY_TIME / portTICK_RATE_MS);
				}
				dmic_inited = true;
			}
			AUDIO_CODEC_SetADCMixMute(adc_idx, DMIC, UNMUTE);
			AUDIO_CODEC_SetADCDmicFilter(adc_idx, ENABLE);
			AUDIO_CODEC_SetDmicSrc(adc_chn_idx, mic_idx);
		}
		break;
		default:
			break;
		}

	}
}


static void ameba_audio_stream_rx_codec_configure(CaptureStream *stream, StreamConfig config)
{
	if (!stream) {
		return;
	}

	ameba_audio_stream_rx_codec_adc_reset();

	ameba_audio_stream_rx_adc_mic_configure(config);
}

static void ameba_audio_stream_rx_codec_init(CaptureStream **stream, StreamConfig config)
{
	CaptureStream *cstream = *stream;

	if (!ameba_audio_is_audio_ip_in_use(POWER)) {
		AUDIO_CODEC_SetLDOMode(POWER_ON);
	}
	ameba_audio_set_audio_ip_use_status(cstream->stream.direction, POWER, true);

	AUDIO_CODEC_I2S_StructInit(&cstream->stream.i2s_initstruct);
	cstream->stream.i2s_initstruct.CODEC_SelRxI2STdm = ameba_audio_get_tdm(config.channels);
	cstream->stream.i2s_initstruct.CODEC_SelI2SRxSR = ameba_audio_get_codec_rate(config.rate);

	AUDIO_CODEC_SetI2SIP(I2S1, ENABLE);
	AUDIO_CODEC_SetI2SSRC(I2S1, INTERNAL_SPORT);
	AUDIO_CODEC_SetADCSRSrc(SOURCE1, cstream->stream.i2s_initstruct.CODEC_SelI2SRxSR);
	AUDIO_CODEC_SetANAClk(ENABLE);
	AUDIO_CODEC_SetI2SParameters(I2S1, ADCPATH, &(cstream->stream.i2s_initstruct));

	//route all ADC path to I2S1.
	AUDIO_CODEC_SetADCSrc(I2S1, ADC1);
	AUDIO_CODEC_SetADCSrc(I2S1, ADC2);
	AUDIO_CODEC_SetADCSrc(I2S1, ADC3);
	AUDIO_CODEC_SetADCSrc(I2S1, ADC4);
	AUDIO_CODEC_SetADCSrc(I2S1, ADC5);
	AUDIO_CODEC_SetADCSrc(I2S1, ADC6);
	AUDIO_CODEC_SetADCSrc(I2S1, ADC7);
	AUDIO_CODEC_SetADCSrc(I2S1, ADC8);

	ameba_audio_stream_rx_codec_configure(cstream, config);
}

//FIX ME for 6/8channel
static void stream_rx_llp_init(Stream *stream)
{
	uint32_t j;
	int index = 1;

	CaptureStream *cstream = (CaptureStream *)stream;
	struct GDMA_CH_LLI *ch_lli = cstream->stream.gdma_ch_lli;

	if (!stream || cstream->stream.stream_mode == AMEBA_AUDIO_DMA_IRQ_MODE) {
		return;
	}

	uint32_t rx_addr = (uint32_t)(cstream->stream.rbuffer->raw_data);
	for (j = 0; j < cstream->stream.period_count; j++) {
		ch_lli[j].LliEle.Darx = (uint32_t)rx_addr + j * cstream->stream.period_bytes;

		if (j == cstream->stream.period_count - 1) {
			ch_lli[j].pNextLli = &ch_lli[0];
		} else {
			ch_lli[j].pNextLli = &ch_lli[j + 1];
		}

		ch_lli[j].BlockSize = cstream->stream.period_bytes / cstream->stream.frame_size; // 4_bytes / item
		ch_lli[j].LliEle.Sarx = (uint32_t)&AUDIO_DEV_TABLE[index].SPORTx->SP_RX_FIFO_0_RD_ADDR;
	}
}

Stream *ameba_audio_stream_rx_init(StreamConfig config)
{
	CaptureStream *cstream;
	size_t buf_size;

	cstream = (CaptureStream *)calloc(1, sizeof(CaptureStream));
	if (!cstream) {
		HAL_AUDIO_ERROR("calloc stream fail");
		return NULL;
	}

	cstream->stream.direction = STREAM_IN;
	cstream->stream.sport_dev_num = 1;

	/*configure perphclock*/
	ameba_audio_periphclock_init(cstream->stream.sport_dev_num);

	if (cstream->stream.sport_dev_num == 0 && !ameba_audio_is_audio_ip_in_use(SPORT0)) {
		AUDIO_SP_Reset(AUDIO_SPORT0_DEV);
	} else if (cstream->stream.sport_dev_num == 1 && !ameba_audio_is_audio_ip_in_use(SPORT1)) {
		AUDIO_SP_Reset(AUDIO_SPORT1_DEV);
	}

	/*configure sport according to the parameters*/
	ameba_audio_stream_rx_sport_init(&cstream, config);
	if (cstream->stream.sport_dev_num == 0) {
		ameba_audio_set_audio_ip_use_status(cstream->stream.direction, SPORT0, true);
	} else if (cstream->stream.sport_dev_num == 1) {
		ameba_audio_set_audio_ip_use_status(cstream->stream.direction, SPORT1, true);
	}

	/* enable audio IP */
	if (!ameba_audio_is_audio_ip_in_use(AUDIOIP)) {
		AUDIO_CODEC_SetAudioIP(ENABLE);
	}
	ameba_audio_set_audio_ip_use_status(cstream->stream.direction, AUDIOIP, true);

	/*configure codec according to the parameters*/
	ameba_audio_stream_rx_codec_init(&cstream, config);
	ameba_audio_set_audio_ip_use_status(cstream->stream.direction, CODEC, true);

	buf_size = config.period_size * config.frame_size * config.period_count;
	cstream->stream.frame_size = config.frame_size;
	cstream->stream.period_bytes = config.period_size * config.frame_size;
	cstream->stream.period_count = config.period_count;
	cstream->stream.stream_mode = config.mode;
	cstream->stream.rbuffer = NULL;

	if (!IS_6_8_CHANNEL(config.channels)) {
		cstream->channel = config.channels;
		cstream->extra_channel = 0;
		cstream->stream.rbuffer = ameba_audio_stream_buffer_create();
		if (cstream->stream.rbuffer) {
			ameba_audio_stream_buffer_alloc(cstream->stream.rbuffer, buf_size);
		}
	} else {
		cstream->channel = 4;
		cstream->extra_channel = config.channels - cstream->channel;
		cstream->stream.rbuffer = ameba_audio_stream_buffer_create();
		if (cstream->stream.rbuffer) {
			ameba_audio_stream_buffer_alloc(cstream->stream.rbuffer, buf_size * cstream->channel / config.channels);
		}
		cstream->extra_rbuffer = ameba_audio_stream_buffer_create();
		if (cstream->extra_rbuffer) {
			ameba_audio_stream_buffer_alloc(cstream->extra_rbuffer, buf_size * cstream->extra_channel / config.channels);
		}
	}

	cstream->stream.gdma_cnt = 0;
	cstream->stream.gdma_irq_cnt = 0;
	cstream->stream.start_gdma = false;
	cstream->stream.gdma_struct = NULL;
	cstream->stream.sem_need_post = false;

	cstream->stream.gdma_struct = (GdmaCallbackData *)calloc(1, sizeof(GdmaCallbackData));
	if (!cstream->stream.gdma_struct) {
		HAL_AUDIO_ERROR("calloc gdma_struct fail");
		return NULL;
	}
	cstream->stream.gdma_struct->stream = (Stream *)cstream;
	cstream->stream.gdma_struct->gdma_id = 0;
	rtw_init_sema(&cstream->stream.sem, 0);

	cstream->restart_by_read = false;
	cstream->frame_size = cstream->stream.frame_size * cstream->channel / config.channels;
	cstream->gdma_need_stop = false;

	if (IS_6_8_CHANNEL(config.channels)) {
		cstream->extra_frame_size = cstream->stream.frame_size * cstream->extra_channel / config.channels;
		cstream->extra_gdma_cnt = 0;
		cstream->extra_gdma_irq_cnt = 0;
		cstream->extra_restart_by_read = false;
		cstream->extra_gdma_struct = NULL;
		cstream->extra_sem_need_post = false;

		cstream->extra_gdma_struct = (GdmaCallbackData *)calloc(1, sizeof(GdmaCallbackData));
		if (!cstream->extra_gdma_struct) {
			HAL_AUDIO_ERROR("calloc extra SPGdmaStruct fail");
			return NULL;
		}
		cstream->extra_gdma_struct->stream = (Stream *)cstream;
		cstream->extra_gdma_struct->gdma_id = 1;

		rtw_init_sema(&cstream->extra_sem, 0);
	}

	cstream->stream.gdma_ch_lli = (struct GDMA_CH_LLI *)ameba_audio_gdma_calloc(cstream->stream.period_count, sizeof(struct GDMA_CH_LLI));

	if (!cstream->stream.gdma_ch_lli) {
		HAL_AUDIO_ERROR("calloc gdma_ch_lli fail");
		return NULL;
	}

	if (cstream->stream.stream_mode == AMEBA_AUDIO_DMA_NOIRQ_MODE) {
		stream_rx_llp_init(&cstream->stream);
	}

	return &cstream->stream;
}

uint32_t ameba_audio_stream_rx_complete(void *data)
{
	uint32_t rx_addr;
	uint32_t rx_length;

	GdmaCallbackData *gdata = (GdmaCallbackData *) data;
	PGDMA_InitTypeDef rxgdma_initstruct = &(gdata->u.SpRxGdmaInitStruct);

	/* Clear Pending ISR */
	GDMA_ClearINT(rxgdma_initstruct->GDMA_Index, rxgdma_initstruct->GDMA_ChNum);

	CaptureStream *cstream = (CaptureStream *)(gdata->stream);

	if (gdata->gdma_id == 0) {
		rx_length = cstream->stream.period_bytes * cstream->channel / (cstream->channel + cstream->extra_channel);
		ameba_audio_stream_buffer_update_rx_writeptr(cstream->stream.rbuffer, rx_length);

		cstream->stream.gdma_irq_cnt++;

		if (cstream->restart_by_read == true) {
			return 0;
		}

		if (ameba_audio_stream_buffer_get_available_size(cstream->stream.rbuffer) == 0) {
			HAL_AUDIO_INFO("buffer full, overrun");
			cstream->restart_by_read = true;
		} else {
			rx_addr = (uint32_t)(cstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->stream.rbuffer));

			if ((ameba_audio_stream_buffer_get_available_size(cstream->stream.rbuffer) >= rx_length) && !(cstream->gdma_need_stop)) {
				AUDIO_SP_RXGDMA_Restart(rxgdma_initstruct->GDMA_Index, rxgdma_initstruct->GDMA_ChNum, rx_addr, rx_length);
				cstream->stream.gdma_cnt++;
			} else {
				HAL_AUDIO_INFO("buffer near full, overrun");
				cstream->restart_by_read = true;
			}
		}

		if (cstream->stream.sem_need_post) {
			rtw_up_sema_from_isr(&cstream->stream.sem);
		}
	} else if (gdata->gdma_id == 1) {
		rx_length = cstream->stream.period_bytes * cstream->extra_channel / (cstream->channel + cstream->extra_channel);
		ameba_audio_stream_buffer_update_rx_writeptr(cstream->extra_rbuffer, rx_length);
		cstream->extra_gdma_irq_cnt++;

		if (cstream->extra_restart_by_read == true) {
			return 0;
		}

		if (ameba_audio_stream_buffer_get_available_size(cstream->extra_rbuffer) == 0) {
			HAL_AUDIO_VERBOSE("extra buffer full, overrun");
			cstream->extra_restart_by_read = true;
		} else {
			rx_addr = (uint32_t)(cstream->extra_rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->extra_rbuffer));

			if ((ameba_audio_stream_buffer_get_available_size(cstream->extra_rbuffer) >= rx_length) && !(cstream->gdma_need_stop)) {
				AUDIO_SP_RXGDMA_Restart(rxgdma_initstruct->GDMA_Index, rxgdma_initstruct->GDMA_ChNum, rx_addr, rx_length);
				cstream->extra_gdma_cnt++;
			} else {
				cstream->extra_restart_by_read = true;
			}
		}
		if (cstream->extra_sem_need_post) {
			rtw_up_sema_from_isr(&cstream->extra_sem);
		}
	}

	return 0;
}

static void ameba_audio_stream_rx_start_in_noirq_mode(Stream *stream)
{
	CaptureStream *cstream = (CaptureStream *)stream;
	PGDMA_InitTypeDef sp_rxgdma_initstruct = &(cstream->stream.gdma_struct->u.SpRxGdmaInitStruct);

	if (cstream->stream.sport_dev_num == 0) {
		AUDIO_SP_DmaCmd(AUDIO_SPORT0_DEV, ENABLE);
	} else if (cstream->stream.sport_dev_num == 1) {
		AUDIO_SP_DmaCmd(AUDIO_SPORT1_DEV, ENABLE);
	}

	if (!cstream->stream.start_gdma) {
		cstream->stream.start_gdma = true;
		AUDIO_SP_LLPRXGDMA_Init(cstream->stream.sport_dev_num, GDMA_INT, sp_rxgdma_initstruct, cstream->stream.gdma_struct,
								(IRQ_FUN)NULL,
								cstream->stream.period_bytes, cstream->stream.period_count, cstream->stream.gdma_ch_lli);

		HAL_AUDIO_INFO("gdma init: index:%d, chNum:%d", sp_rxgdma_initstruct->GDMA_Index,
					   sp_rxgdma_initstruct->GDMA_ChNum);
		if (cstream->stream.sport_dev_num == 0) {
			AUDIO_SP_RXStart(AUDIO_SPORT0_DEV, ENABLE);
		} else if (cstream->stream.sport_dev_num == 1) {
			AUDIO_SP_RXStart(AUDIO_SPORT1_DEV, ENABLE);
		}
	}
}

static void ameba_audio_stream_rx_start_in_irq_mode(Stream *stream)
{
	CaptureStream *cstream = (CaptureStream *)stream;
	PGDMA_InitTypeDef sp_rxgdma_initstruct = &(cstream->stream.gdma_struct->u.SpRxGdmaInitStruct);

	if (cstream->stream.sport_dev_num == 0) {
		AUDIO_SP_DmaCmd(AUDIO_SPORT0_DEV, ENABLE);
	} else if (cstream->stream.sport_dev_num == 1) {
		AUDIO_SP_DmaCmd(AUDIO_SPORT1_DEV, ENABLE);
	}
	cstream->gdma_need_stop = false;

	if (!cstream->stream.start_gdma) {
		uint32_t rx_addr;
		uint32_t len = cstream->stream.period_bytes * cstream->channel / (cstream->channel + cstream->extra_channel);
		rx_addr = (uint32_t)(cstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->stream.rbuffer));

		AUDIO_SP_RXGDMA_Init(cstream->stream.sport_dev_num, GDMA_INT, sp_rxgdma_initstruct, cstream->stream.gdma_struct,
							 (IRQ_FUN)ameba_audio_stream_rx_complete, (u8 *)rx_addr, len);
		cstream->stream.gdma_cnt++;

		if (cstream->extra_channel) {
			uint32_t extra_rx_addr;
			uint32_t extra_len = cstream->stream.period_bytes * cstream->extra_channel / (cstream->channel + cstream->extra_channel);
			extra_rx_addr = (uint32_t)(cstream->extra_rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->extra_rbuffer));
			PGDMA_InitTypeDef extra_sp_rxgdma_initstruct = &(cstream->extra_gdma_struct->u.SpRxGdmaInitStruct);
			AUDIO_SP_RXGDMA_Init(cstream->stream.sport_dev_num, GDMA_EXT, extra_sp_rxgdma_initstruct, cstream->extra_gdma_struct,
								 (IRQ_FUN)ameba_audio_stream_rx_complete, (u8 *)extra_rx_addr, extra_len);
			cstream->extra_gdma_cnt++;
		}

		//DelayMs(100);
		if (cstream->stream.sport_dev_num == 0) {
			AUDIO_SP_RXStart(AUDIO_SPORT0_DEV, ENABLE);
		} else if (cstream->stream.sport_dev_num == 1) {
			AUDIO_SP_RXStart(AUDIO_SPORT1_DEV, ENABLE);
		}

		cstream->stream.start_gdma = true;
	}

#if REG_DUMP
	//ameba_audio_dump_gdma(sp_rxgdma_initstruct->GDMA_ChNum);
	if (cstream->stream.sport_dev_num == 0) {
		ameba_audio_dump_sport_regs(SPORT0_REG_BASE);
	} else if (cstream->stream.sport_dev_num == 1) {
		ameba_audio_dump_sport_regs(SPORT1_REG_BASE);
	}
	ameba_audio_dump_codec_regs();
#endif

}

void ameba_audio_stream_rx_start(Stream *stream)
{
	CaptureStream *cstream = (CaptureStream *)stream;
	if (cstream) {
		if (cstream->stream.stream_mode) {
			ameba_audio_stream_rx_start_in_noirq_mode(stream);
		} else {
			ameba_audio_stream_rx_start_in_irq_mode(stream);
		}
	}
}

static void ameba_audio_stream_rx_check_and_start_gdma(CaptureStream *cstream)
{
	if (cstream->restart_by_read) {
		uint32_t rx_addr;
		uint32_t bytes = cstream->stream.period_bytes * cstream->channel / (cstream->channel + cstream->extra_channel);
		if (ameba_audio_stream_buffer_get_available_size(cstream->stream.rbuffer) >= bytes) {
			GDMA_InitTypeDef sp_rxgdma_initstruct = cstream->stream.gdma_struct->u.SpRxGdmaInitStruct;
			rx_addr = (uint32_t)(cstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->stream.rbuffer));

			AUDIO_SP_RXGDMA_Restart(sp_rxgdma_initstruct.GDMA_Index, sp_rxgdma_initstruct.GDMA_ChNum, rx_addr, bytes);
			cstream->stream.gdma_cnt++;
			cstream->restart_by_read = false;
		}
	}

	if (cstream->extra_restart_by_read) {
		uint32_t extra_rx_addr;
		uint32_t extra_bytes = cstream->stream.period_bytes * cstream->extra_channel / (cstream->channel + cstream->extra_channel);

		if (ameba_audio_stream_buffer_get_available_size(cstream->extra_rbuffer) >= extra_bytes) {
			GDMA_InitTypeDef extra_sp_rxgdma_initstruct = cstream->extra_gdma_struct->u.SpRxGdmaInitStruct;
			extra_rx_addr = (uint32_t)(cstream->extra_rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->extra_rbuffer));

			AUDIO_SP_RXGDMA_Restart(extra_sp_rxgdma_initstruct.GDMA_Index, extra_sp_rxgdma_initstruct.GDMA_ChNum, extra_rx_addr, extra_bytes);
			cstream->extra_gdma_cnt++;
			cstream->extra_restart_by_read = false;
		}
	}
}

static int ameba_audio_stream_rx_read_in_noirq_mode(Stream *stream, void *data, unsigned int bytes)
{
	HAL_AUDIO_CVERBOSE("bytes:%" PRId32 "", __func__, bytes);

	unsigned int total_bytes_0 = bytes;
	unsigned int bytes_to_read_0 = total_bytes_0;
	unsigned int bytes_read_0 = 0;

	char *p_buf0 = (char *)data;
	CaptureStream *cstream = (CaptureStream *)stream;
	PGDMA_InitTypeDef sp_rxgdma_initstruct = &(cstream->stream.gdma_struct->u.SpRxGdmaInitStruct);

	while (bytes_to_read_0 != 0) {
		uint32_t rp = (uint32_t)(cstream->stream.rbuffer->raw_data + cstream->stream.rbuffer->read_ptr);
		uint32_t capacity = cstream->stream.rbuffer->capacity;
		uint32_t dma_addr = GDMA_GetDstAddr(sp_rxgdma_initstruct->GDMA_Index, sp_rxgdma_initstruct->GDMA_ChNum);
		uint32_t avail = (rp <= dma_addr) ? (dma_addr - rp) : (capacity - (rp - dma_addr));

		if (avail > 0) {
			bytes_read_0 = ameba_audio_stream_buffer_read(cstream->stream.rbuffer, (u8 *)p_buf0 + total_bytes_0 - bytes_to_read_0, bytes_to_read_0,
						   cstream->stream.stream_mode);
		}

		bytes_to_read_0 -= bytes_read_0;

	}

	return 0;
}

static int ameba_audio_stream_rx_read_in_irq_mode(Stream *stream, void *data, uint32_t bytes)
{
	CaptureStream *cstream = (CaptureStream *)stream;

	uint32_t total_bytes = bytes * cstream->channel / (cstream->channel + cstream->extra_channel);
	uint32_t bytes_to_read = total_bytes;
	uint32_t bytes_read = 0;

	char *p_buf = NULL;
	char *p_extra_buf = NULL;

	if (cstream->extra_channel) {
		p_buf = (char *)calloc(total_bytes, sizeof(char));
	} else {
		p_buf = (char *)data;
	}

	while (bytes_to_read != 0) {
		ameba_audio_stream_rx_check_and_start_gdma(cstream);
		bytes_read = ameba_audio_stream_buffer_read(cstream->stream.rbuffer, (u8 *)p_buf + total_bytes - bytes_to_read, bytes_to_read, cstream->stream.stream_mode);
		bytes_to_read -= bytes_read;

		if (ameba_audio_stream_buffer_get_remain_size(cstream->stream.rbuffer) < bytes_to_read) {
			cstream->stream.sem_need_post = true;
			rtw_down_sema(&cstream->stream.sem);
		}
		cstream->stream.sem_need_post = false;
	}

	//for channel > 4
	if (cstream->extra_channel) {
		uint32_t extra_total_bytes = bytes * cstream->extra_channel / (cstream->channel + cstream->extra_channel);
		uint32_t extra_bytes_to_read = extra_total_bytes;
		uint32_t extra_bytes_read = 0;

		p_extra_buf = (char *)calloc(extra_total_bytes, sizeof(char));

		while (extra_bytes_to_read != 0) {
			ameba_audio_stream_rx_check_and_start_gdma(cstream);
			extra_bytes_read = ameba_audio_stream_buffer_read(cstream->extra_rbuffer, (u8 *)p_extra_buf + extra_total_bytes - extra_bytes_to_read, extra_bytes_to_read,
							   cstream->stream.stream_mode);
			extra_bytes_to_read -= extra_bytes_read;

			if (ameba_audio_stream_buffer_get_remain_size(cstream->extra_rbuffer) < extra_bytes_to_read) {
				cstream->extra_sem_need_post = true;
				rtw_down_sema(&cstream->extra_sem);
			}
			cstream->extra_sem_need_post = false;
		}

	}

	ameba_audio_stream_rx_check_and_start_gdma(cstream);

	if (cstream->extra_channel) {
		uint32_t idx = 0;
		for (; idx < bytes / cstream->frame_size; idx++) {
			memcpy((char *)data + idx * cstream->stream.frame_size, p_buf + idx * cstream->frame_size, cstream->frame_size);
			memcpy((char *)data + idx * cstream->stream.frame_size + cstream->frame_size,
				   p_extra_buf + idx * cstream->extra_frame_size, cstream->extra_frame_size);
		}

		if (p_buf) {
			free(p_buf);
		}
		if (p_extra_buf) {
			free(p_extra_buf);
		}
	}

	return 0;
}

int ameba_audio_stream_rx_read(Stream *stream, void *data, uint32_t bytes)
{
	CaptureStream *cstream = (CaptureStream *)stream;

	if (cstream) {
		if (cstream->stream.stream_mode) {
			return ameba_audio_stream_rx_read_in_noirq_mode(stream, data, bytes);
		} else {
			return ameba_audio_stream_rx_read_in_irq_mode(stream, data, bytes);
		}
	}

	return 0;
}

void ameba_audio_stream_rx_close(Stream *stream)
{
	CaptureStream *cstream = (CaptureStream *)stream;

	if (cstream) {
		cstream->gdma_need_stop = true;
		do {
			vTaskDelay(4 / portTICK_RATE_MS);
			HAL_AUDIO_CVERBOSE("waiting for rx to complete");
		} while ((cstream->stream.gdma_cnt != cstream->stream.gdma_irq_cnt) || (cstream->extra_gdma_cnt != cstream->extra_gdma_irq_cnt));

		GDMA_InitTypeDef sp_rxgdma_initstruct = cstream->stream.gdma_struct->u.SpRxGdmaInitStruct;
		HAL_AUDIO_INFO("dma clear: index:%d, chNum:%d", sp_rxgdma_initstruct.GDMA_Index, sp_rxgdma_initstruct.GDMA_ChNum);

		GDMA_ClearINT(sp_rxgdma_initstruct.GDMA_Index, sp_rxgdma_initstruct.GDMA_ChNum);
		GDMA_Cmd(sp_rxgdma_initstruct.GDMA_Index, sp_rxgdma_initstruct.GDMA_ChNum, DISABLE);
		GDMA_ChnlFree(sp_rxgdma_initstruct.GDMA_Index, sp_rxgdma_initstruct.GDMA_ChNum);

		if (cstream->extra_channel) {
			GDMA_InitTypeDef extra_sp_rxgdma_initstruct = cstream->extra_gdma_struct->u.SpRxGdmaInitStruct;
			GDMA_ClearINT(extra_sp_rxgdma_initstruct.GDMA_Index, extra_sp_rxgdma_initstruct.GDMA_ChNum);
			GDMA_Cmd(extra_sp_rxgdma_initstruct.GDMA_Index, extra_sp_rxgdma_initstruct.GDMA_ChNum, DISABLE);
			GDMA_ChnlFree(extra_sp_rxgdma_initstruct.GDMA_Index, extra_sp_rxgdma_initstruct.GDMA_ChNum);
		}

		if (cstream->stream.sport_dev_num == 0) {
			AUDIO_SP_DmaCmd(AUDIO_SPORT0_DEV, DISABLE);
			AUDIO_SP_RXStart(AUDIO_SPORT0_DEV, DISABLE);
		} else if (cstream->stream.sport_dev_num == 1) {
			AUDIO_SP_DmaCmd(AUDIO_SPORT1_DEV, DISABLE);
			AUDIO_SP_RXStart(AUDIO_SPORT1_DEV, DISABLE);
		}

		AUDIO_SP_Deinit(cstream->stream.sport_dev_num, SP_DIR_RX);

		ameba_audio_reset_audio_ip_status((Stream *)cstream);

		rtw_free_sema(&cstream->stream.sem);
		rtw_free_sema(&cstream->extra_sem);

		if (cstream->stream.rbuffer) {
			ameba_audio_stream_buffer_release(cstream->stream.rbuffer);
		}

		if (cstream->extra_rbuffer) {
			ameba_audio_stream_buffer_release(cstream->extra_rbuffer);
		}

		if (cstream->stream.gdma_struct) {
			free(cstream->stream.gdma_struct);
			cstream->stream.gdma_struct = NULL;
		}

		if (cstream->extra_gdma_struct) {
			free(cstream->extra_gdma_struct);
			cstream->extra_gdma_struct = NULL;
		}

		if (cstream->stream.gdma_ch_lli) {
			ameba_audio_gdma_free(cstream->stream.gdma_ch_lli);
			cstream->stream.gdma_ch_lli = NULL;
		}

		free(cstream);

	}
}
