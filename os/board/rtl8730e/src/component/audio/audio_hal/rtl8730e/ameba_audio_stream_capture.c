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

#include "audio_hw_compat.h"
#include <inttypes.h>

#include "osal_c/osal_time.h"
#include "platform_stdlib.h"
#include "ameba_audio_clock.h"
#include "ameba_soc.h"
#include "ameba_rcc.h"
#include "ameba_audio_hw_usrcfg.h"
#include "audio_hw_debug.h"
#include "hardware/audio/audio_hw_types.h"

#include "ameba_audio_stream_control.h"
#include "ameba_audio_stream_utils.h"
#include "ameba_audio_stream_buffer.h"
#include "ameba_audio_types.h"

#define IS_6_8_CHANNEL(NUM) (((NUM) == 6) || ((NUM) == 8))

extern void PLL_I2S_24P576M(u32 NewState);
extern void AUDIO_SP_SetMclk(u32 index, u32 NewState);
extern void AUDIO_SP_SetMclkDiv(u32 index, u32 mck_div);

static void ameba_audio_stream_rx_sport_init(CaptureStream **stream, StreamConfig config, uint32_t device)
{
	CaptureStream *cstream = *stream;
	AUDIO_ClockParams Clock_Params;
	AUDIO_InitParams Init_Params;
	uint32_t clock_mode;
	uint32_t mck_div;

	AUDIO_SP_StructInit(&cstream->stream.sp_initstruct);
	cstream->stream.sp_initstruct.SP_SelI2SMonoStereo = ameba_audio_get_channel(config.channels);
	cstream->stream.sp_initstruct.SP_SelWordLen = ameba_audio_get_sp_format(config.format, cstream->stream.direction);
	cstream->stream.sp_initstruct.SP_SelTDM = ameba_audio_get_sp_tdm(config.channels);
	cstream->stream.sp_initstruct.SP_SelFIFO = ameba_audio_get_fifo_num(config.channels);
	cstream->stream.sp_initstruct.SP_SR = ameba_audio_get_sp_rate(config.rate);
	HAL_AUDIO_VERBOSE("selmo:%lu, wordlen:%lu, sr:%lu, seltdm:%lu, selfifo:%lu,",
					  cstream->stream.sp_initstruct.SP_SelI2SMonoStereo,
					  cstream->stream.sp_initstruct.SP_SelWordLen,
					  cstream->stream.sp_initstruct.SP_SR,
					  cstream->stream.sp_initstruct.SP_SelTDM,
					  cstream->stream.sp_initstruct.SP_SelFIFO);

	Init_Params.chn_len = SP_CL_32;
	Init_Params.chn_cnt = config.channels;
	Init_Params.sr = cstream->stream.sp_initstruct.SP_SR;

	if (AUDIO_HW_IN_SPORT_CLK_TYPE == 0) {
		Init_Params.codec_multiplier_with_rate = 0;
		Init_Params.sport_mclk_fixed_max = 0;
		Audio_Clock_Choose(XTAL_CLOCK, &Init_Params, &Clock_Params);
	}

	if (AUDIO_HW_IN_SPORT_CLK_TYPE == 1) {
		Init_Params.codec_multiplier_with_rate = AUDIO_HW_IN_MCLK_MULITIPLIER;
		Init_Params.sport_mclk_fixed_max = 0;
		Audio_Clock_Choose(PLL_CLK, &Init_Params, &Clock_Params);
	}

	if (AUDIO_HW_IN_SPORT_CLK_TYPE == 2) {
		Init_Params.codec_multiplier_with_rate = 0;
		Init_Params.sport_mclk_fixed_max = AUDIO_HW_IN_FIXED_MCLK;
		Audio_Clock_Choose(PLL_CLK, &Init_Params, &Clock_Params);
	}

	switch (Clock_Params.Clock) {
	case PLL_CLOCK_24P576M:
		PLL_I2S_24P576M(ENABLE);
		RCC_PeriphClockSource_SPORT(cstream->stream.sport_dev_num, CKSL_I2S_PLL24M);
		PLL_I2S_Div(cstream->stream.sport_dev_num, Clock_Params.PLL_DIV);
		clock_mode = PLL_CLOCK_24P576M / Clock_Params.PLL_DIV;
		break;

	case PLL_CLOCK_45P1584M:
		PLL_I2S_45P158M(ENABLE);
		RCC_PeriphClockSource_SPORT(cstream->stream.sport_dev_num, CKSL_I2S_PLL45M);
		PLL_I2S_Div(cstream->stream.sport_dev_num, Clock_Params.PLL_DIV);
		PLL_I2S_45P158M_ClkTune(0, PLL_AUTO);
		clock_mode = PLL_CLOCK_45P1584M / Clock_Params.PLL_DIV;
		break;

	case PLL_CLOCK_98P304M:
		PLL_I2S_98P304M(ENABLE);
		RCC_PeriphClockSource_SPORT(cstream->stream.sport_dev_num, CKSL_I2S_PLL98M);
		PLL_I2S_Div(cstream->stream.sport_dev_num, Clock_Params.PLL_DIV);
		clock_mode = PLL_CLOCK_98P304M / Clock_Params.PLL_DIV;
		break;

	case I2S_CLOCK_XTAL40M:
		clock_mode = I2S_CLOCK_XTAL40M;
		RCC_PeriphClockSource_SPORT(cstream->stream.sport_dev_num, CKSL_I2S_XTAL40M);
		break;
	}

	cstream->stream.sp_initstruct.SP_SelClk = clock_mode;

	if (AUDIO_I2S_IN_MULTIIO_EN == 1) {
		cstream->stream.sp_initstruct.SP_SetMultiIO = SP_RX_MULTIIO_EN;
	} else {
		cstream->stream.sp_initstruct.SP_SetMultiIO = SP_RX_MULTIIO_DIS;
	}

	AUDIO_SP_Init(cstream->stream.sport_dev_num, SP_DIR_RX, &cstream->stream.sp_initstruct);

	if (device == AMEBA_AUDIO_IN_I2S) {
		if (AUDIO_I2S_IN_NEED_MCLK_OUT == 1) {
			//enable mclk and set div
			AUDIO_SP_SetMclk(cstream->stream.sport_dev_num, ENABLE);
			switch (Clock_Params.MCLK_DIV) {
			case 1:
				mck_div = 2;
				break;
			case 2:
				mck_div = 1;
				break;
			case 4:
				mck_div = 0;
				break;
			}
			AUDIO_SP_SetMclkDiv(cstream->stream.sport_dev_num, mck_div);
		}

		if (AUDIO_I2S_IN_ROLE == AUDIO_I2S_SLAVE) {
			AUDIO_SP_SetMasterSlave(cstream->stream.sport_dev_num, SLAVE);
		}
	}
}

static void ameba_audio_stream_rx_codec_adc_reset(void)
{
	uint32_t idx = 1;
	for (; idx <= MAX_AD_NUM; idx++) {
		uint32_t adc_chn_idx = ameba_audio_stream_get_adc_chn_idx(idx);
		uint32_t adc_idx = ameba_audio_stream_get_adc_idx(idx);

		AUDIO_CODEC_EnableADC(adc_chn_idx, DISABLE);
		AUDIO_CODEC_EnableADCFifo(adc_chn_idx, DISABLE);
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
		HAL_AUDIO_VERBOSE("configure default adc");
		uint32_t j = 1;
		for (; j <= config.channels; j++) {
			uint32_t adc_chn_idx = ameba_audio_stream_get_adc_chn_idx(j);
			uint32_t adc_idx = ameba_audio_stream_get_adc_idx(j);

			AUDIO_CODEC_EnableADC(adc_chn_idx, ENABLE);
			AUDIO_CODEC_EnableADCFifo(adc_chn_idx, ENABLE);
			AUDIO_CODEC_SetADCHPF(adc_idx, 3, ENABLE);
			AUDIO_CODEC_SetADCMute(adc_idx, dc->mute_for_adc[j - 1] ? MUTE : UNMUTE);
			AUDIO_CODEC_SetADCVolume(adc_idx, dc->volume_for_adc[j - 1]);
			//for I2S PLL case
			if (AUDIO_HW_IN_SPORT_CLK_TYPE == 1) {
				AUDIO_CODEC_SetADCASRC(I2S1, adc_idx, ameba_audio_get_codec_rate(config.rate), ENABLE);
			}
		}
	} else {  // use adcs according to user customize
		uint32_t k = 1;
		for (; k <= MAX_AD_NUM; k++) {
			if ((dc->adc_use_status >> (k - 1)) & 1) {
				uint32_t adc_chn_idx = ameba_audio_stream_get_adc_chn_idx(k);
				uint32_t adc_idx = ameba_audio_stream_get_adc_idx(k);

				AUDIO_CODEC_EnableADC(adc_chn_idx, ENABLE);
				AUDIO_CODEC_EnableADCFifo(adc_chn_idx, ENABLE);
				AUDIO_CODEC_SetADCHPF(adc_idx, 3, ENABLE);
				AUDIO_CODEC_SetADCMute(adc_idx, dc->mute_for_adc[k - 1] ? MUTE : UNMUTE);
				AUDIO_CODEC_SetADCVolume(adc_idx, dc->volume_for_adc[k - 1]);
				//for I2S PLL case
				if (AUDIO_HW_IN_SPORT_CLK_TYPE == 1) {
					AUDIO_CODEC_SetADCASRC(I2S1, adc_idx, ameba_audio_get_codec_rate(config.rate), ENABLE);
				}
			}
		}
	}

	//enable amic/dmic
	for (; i <= adc_num; i++) {
		uint32_t adc_idx = ameba_audio_stream_get_adc_idx(i);
		uint32_t adc_chn_idx = ameba_audio_stream_get_adc_chn_idx(i);
		uint32_t mic_idx = ameba_audio_stream_get_mic_idx(dc->mic_category_for_adc[i - 1]);

		HAL_AUDIO_VERBOSE("adc_idx: %" PRIu32 ", adc_chn: %" PRIu32 ", mic_idx: %" PRIu32 ", mic_category: %" PRIu32 ", ",
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
			AUDIO_CODEC_SetMicBstChnMute(mic_idx, MICIN, dc->mute_for_mic_bst[dc->mic_category_for_adc[i - 1]] ? MUTE : UNMUTE);
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
	cstream->stream.i2s_initstruct.CODEC_SelRxI2STdm = ameba_audio_get_codec_tdm(config.channels);
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

		ch_lli[j].BlockSize = cstream->stream.period_bytes / cstream->stream.config.frame_size; // 4_bytes / item
		ch_lli[j].LliEle.Sarx = (uint32_t)&AUDIO_DEV_TABLE[index].SPORTx->SP_RX_FIFO_0_RD_ADDR;
	}
}

/*
 * when sport LRCLK delivered sport_compare_val frames,
 * the interrupt callback is triggered.
 */
uint32_t ameba_audio_stream_rx_sport_interrupt(void *data)
{
	CaptureStream *cstream = (CaptureStream *) data;

	cstream->stream.sport_irq_count++;
	cstream->stream.total_counter += cstream->stream.sport_compare_val;
	if (cstream->stream.total_counter >= cstream->stream.total_counter_boundary) {
		cstream->stream.total_counter -= cstream->stream.total_counter_boundary;
		cstream->stream.sport_irq_count = 0;
	}

	HAL_AUDIO_PVERBOSE("total_counter:%" PRIu64 " \n", cstream->stream.total_counter);
	AUDIO_SP_ClearRXCounterIrq(cstream->stream.sport_dev_num);

	return 0;
}

int  ameba_audio_stream_rx_get_position(Stream *stream, uint64_t *captured_frames, struct timespec *tstamp)
{
	//now nsec;
	uint64_t nsec;
	//current total i2s counter of audio frames;
	uint64_t now_counter = 0;
	//means the delta_counter between now counter and last irq total counter.
	uint32_t delta_counter = 0;

	CaptureStream *cstream = (CaptureStream *)stream;
	if (!cstream) {
		return -1;
	}

	AUDIO_SP_SetPhaseLatch(cstream->stream.sport_dev_num);
	delta_counter = AUDIO_SP_GetRXCounterVal(cstream->stream.sport_dev_num);
	now_counter = cstream->stream.total_counter + delta_counter;

	*captured_frames = now_counter;

	//tv_sec is lld, tv_nsec is ld
	//usec will exceed at (2^64 / 50M / 3600 / 24 / 365 / 20 = 584 days)
	nsec = ameba_audio_get_now_ns();
	tstamp->tv_sec = nsec / 1000000000LL;
	tstamp->tv_nsec = nsec - tstamp->tv_sec * 1000000000LL;

	HAL_AUDIO_PVERBOSE("captured_frames:%" PRIu64 ", trigger:%" PRIu64 ", usec:%" PRIu64 ", tv_sec:%" PRIu64 ", tv_nsec:%" PRIu32 "",
					   *captured_frames, cstream->stream.trigger_tstamp, nsec, tstamp->tv_sec, tstamp->tv_nsec);

	return 0;
}

int  ameba_audio_stream_rx_get_time(Stream *stream, int64_t *now_ns, int64_t *audio_ns)
{
	//now nsec;
	uint64_t nsec;
	//current total i2s counter of audio frames;
	uint64_t now_counter = 0;
	//means the delta_counter between now counter and last irq total counter.
	uint32_t delta_counter = 0;

	uint32_t phase = 0;

	CaptureStream *cstream = (CaptureStream *)stream;
	if (!cstream) {
		return -1;
	}

	AUDIO_SP_SetPhaseLatch(cstream->stream.sport_dev_num);
	delta_counter = AUDIO_SP_GetRXCounterVal(cstream->stream.sport_dev_num);
	phase = AUDIO_SP_GetRXPhaseVal(cstream->stream.sport_dev_num);
	now_counter = cstream->stream.total_counter + delta_counter;

	//nsec will exceed at (2^64 / 50M / 3600 / 24 / 365 / 20 = 584 years)
	nsec = ameba_audio_get_now_ns();

	*now_ns = nsec;
	*audio_ns = (int64_t)((double)((double)now_counter + (double)phase / (double)32) / (double)cstream->stream.config.rate * (double)1000000000);

	return 0;
}

Stream *ameba_audio_stream_rx_init(uint32_t device, StreamConfig config)
{
	CaptureStream *cstream;
	size_t buf_size;

	cstream = (CaptureStream *)calloc(1, sizeof(CaptureStream));
	if (!cstream) {
		HAL_AUDIO_ERROR("calloc stream fail");
		return NULL;
	}

	cstream->stream.config = config;
	cstream->stream.direction = STREAM_IN;
	if (device == AMEBA_AUDIO_IN_I2S) {
		cstream->stream.sport_dev_num = AUDIO_I2S_IN_SPORT_INDEX;
		if (AUDIO_I2S_IN_NEED_MCLK_OUT) {
			Pinmux_Config(AUDIO_I2S_IN_MCLK_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_IN_SPORT_INDEX));
		}
		Pinmux_Config(AUDIO_I2S_IN_BCLK_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_IN_SPORT_INDEX));
		Pinmux_Config(AUDIO_I2S_IN_LRCLK_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_IN_SPORT_INDEX));
		Pinmux_Config(AUDIO_I2S_IN_DATA0_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_IN_SPORT_INDEX));
		if (AUDIO_I2S_IN_MULTIIO_EN) {
			Pinmux_Config(AUDIO_I2S_IN_DATA1_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_IN_SPORT_INDEX));
			Pinmux_Config(AUDIO_I2S_IN_DATA2_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_IN_SPORT_INDEX));
			Pinmux_Config(AUDIO_I2S_IN_DATA3_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_IN_SPORT_INDEX));
		}
	} else if (device == AMEBA_AUDIO_IN_MIC) {
		cstream->stream.sport_dev_num = 1;
	}
	cstream->stream.sport_dev_addr = ameba_audio_get_sport_addr(cstream->stream.sport_dev_num);

	/*configure perphclock*/
	ameba_audio_periphclock_init(cstream->stream.sport_dev_num);

	if (!ameba_audio_is_audio_ip_in_use(SPORT0)) {
		AUDIO_SP_Reset(cstream->stream.sport_dev_num);
	}

	/*configure sport according to the parameters*/
	ameba_audio_stream_rx_sport_init(&cstream, config, device);
	if (cstream->stream.sport_dev_num == 0) {
		ameba_audio_set_audio_ip_use_status(cstream->stream.direction, SPORT0, true);
	} else if (cstream->stream.sport_dev_num == 1) {
		ameba_audio_set_audio_ip_use_status(cstream->stream.direction, SPORT1, true);
	} else if (cstream->stream.sport_dev_num == 2) {
		ameba_audio_set_audio_ip_use_status(cstream->stream.direction, SPORT2, true);
	} else if (cstream->stream.sport_dev_num == 3) {
		ameba_audio_set_audio_ip_use_status(cstream->stream.direction, SPORT3, true);
	}

	if (device == AMEBA_AUDIO_IN_MIC) {
		/* enable audio IP */
		if (!ameba_audio_is_audio_ip_in_use(AUDIOIP)) {
			AUDIO_CODEC_SetAudioIP(ENABLE);
		}
		ameba_audio_set_audio_ip_use_status(cstream->stream.direction, AUDIOIP, true);

		/*configure codec according to the parameters*/
		ameba_audio_stream_rx_codec_init(&cstream, config);
		ameba_audio_set_audio_ip_use_status(cstream->stream.direction, CODEC, true);
	}

	buf_size = config.period_size * config.frame_size * config.period_count;
	cstream->stream.period_bytes = config.period_size * config.frame_size;
	cstream->stream.period_count = config.period_count;
	cstream->stream.stream_mode = config.mode;
	cstream->stream.rbuffer = NULL;

	if (!IS_6_8_CHANNEL(config.channels)) {
		cstream->stream.channel = config.channels;
		cstream->stream.extra_channel = 0;
		cstream->stream.rbuffer = ameba_audio_stream_buffer_create();
		if (cstream->stream.rbuffer) {
			ameba_audio_stream_buffer_alloc(cstream->stream.rbuffer, buf_size);
		}
		cstream->stream.extra_rbuffer = NULL;
	} else {
		cstream->stream.channel = 4;
		cstream->stream.extra_channel = config.channels - cstream->stream.channel;
		cstream->stream.rbuffer = ameba_audio_stream_buffer_create();
		if (cstream->stream.rbuffer) {
			ameba_audio_stream_buffer_alloc(cstream->stream.rbuffer, buf_size * cstream->stream.channel / config.channels);
		}
		cstream->stream.extra_rbuffer = ameba_audio_stream_buffer_create();
		if (cstream->stream.extra_rbuffer) {
			ameba_audio_stream_buffer_alloc(cstream->stream.extra_rbuffer, buf_size * cstream->stream.extra_channel / config.channels);
		}
	}

	cstream->stream.gdma_cnt = 0;
	cstream->stream.gdma_irq_cnt = 0;
	cstream->stream.start_gdma = false;
	cstream->stream.gdma_struct = NULL;
	cstream->stream.sem_need_post = false;
	cstream->stream.sem_gdma_end_need_post = false;

	cstream->stream.gdma_struct = (GdmaCallbackData *)calloc(1, sizeof(GdmaCallbackData));
	if (!cstream->stream.gdma_struct) {
		HAL_AUDIO_ERROR("calloc gdma_struct fail");
		return NULL;
	}
	cstream->stream.gdma_struct->stream = (Stream *)cstream;
	cstream->stream.gdma_struct->gdma_id = 0;
	rtw_init_sema(&cstream->stream.sem, 0);
	rtw_init_sema(&cstream->stream.sem_gdma_end, 0);

	cstream->stream.restart_by_user = false;
	cstream->stream.frame_size = config.frame_size * cstream->stream.channel / config.channels;
	cstream->stream.gdma_need_stop = false;
	cstream->stream.extra_gdma_struct = NULL;

	if (IS_6_8_CHANNEL(config.channels)) {
		cstream->stream.extra_frame_size = config.frame_size * cstream->stream.extra_channel / config.channels;
		cstream->stream.extra_gdma_cnt = 0;
		cstream->stream.extra_gdma_irq_cnt = 0;
		cstream->stream.extra_restart_by_user = false;
		cstream->stream.extra_sem_need_post = false;
		cstream->stream.extra_sem_gdma_end_need_post = false;

		cstream->stream.extra_gdma_struct = (GdmaCallbackData *)calloc(1, sizeof(GdmaCallbackData));
		if (!cstream->stream.extra_gdma_struct) {
			HAL_AUDIO_ERROR("calloc extra SPGdmaStruct fail");
			return NULL;
		}
		cstream->stream.extra_gdma_struct->stream = (Stream *)cstream;
		cstream->stream.extra_gdma_struct->gdma_id = 1;

		rtw_init_sema(&cstream->stream.extra_sem, 0);
		rtw_init_sema(&cstream->stream.extra_sem_gdma_end, 0);
	}

	cstream->stream.gdma_ch_lli = (struct GDMA_CH_LLI *)ameba_audio_gdma_calloc(cstream->stream.period_count, sizeof(struct GDMA_CH_LLI));

	if (!cstream->stream.gdma_ch_lli) {
		HAL_AUDIO_ERROR("calloc gdma_ch_lli fail");
		return NULL;
	}

	if (cstream->stream.stream_mode == AMEBA_AUDIO_DMA_NOIRQ_MODE) {
		stream_rx_llp_init(&cstream->stream);
	}

	cstream->stream.trigger_tstamp = 0;
	cstream->stream.total_counter = 0;
	cstream->stream.sport_irq_count = 0;
	cstream->stream.sport_compare_val = config.period_size * config.period_count;
	cstream->stream.total_counter_boundary = UINT64_MAX;

	while (cstream->stream.sport_compare_val * 2 <= AUDIO_HW_MAX_SPORT_IRQ_X) {
		cstream->stream.sport_compare_val *= 2;
	}

	uint32_t irq = ameba_audio_get_sport_irq(cstream->stream.sport_dev_num);
	InterruptDis(irq);
	InterruptUnRegister(irq);
	InterruptRegister((IRQ_FUN)ameba_audio_stream_rx_sport_interrupt, irq, (uint32_t)cstream, 4);
	InterruptEn(irq, 4);


	ameba_audio_set_native_time();

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
		rx_length = cstream->stream.period_bytes * cstream->stream.channel / (cstream->stream.channel + cstream->stream.extra_channel);
		ameba_audio_stream_buffer_update_rx_writeptr(cstream->stream.rbuffer, rx_length);

		cstream->stream.gdma_irq_cnt++;

		if (cstream->stream.sem_gdma_end_need_post) {
			rtw_up_sema_from_isr(cstream->stream.sem_gdma_end);
			return 0;
		}

		if (cstream->stream.restart_by_user == true || cstream->stream.gdma_need_stop) {
			return 0;
		}

		if (ameba_audio_stream_buffer_get_available_size(cstream->stream.rbuffer) == 0) {
			HAL_AUDIO_IRQ_INFO("buffer full, overrun(%lldns)", OsalGetSysTimeNs(OSAL_TIME_MONOTONIC));
			cstream->stream.restart_by_user = true;
			AUDIO_SP_SetPhaseLatch(cstream->stream.sport_dev_num);
			cstream->stream.total_counter += AUDIO_SP_GetRXCounterVal(cstream->stream.sport_dev_num);
			AUDIO_SP_SetRXCounter(cstream->stream.sport_dev_num, DISABLE);
		} else {
			rx_addr = (uint32_t)(cstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->stream.rbuffer));

			if ((ameba_audio_stream_buffer_get_available_size(cstream->stream.rbuffer) >= rx_length) && !(cstream->stream.gdma_need_stop)) {
				AUDIO_SP_RXGDMA_Restart(rxgdma_initstruct->GDMA_Index, rxgdma_initstruct->GDMA_ChNum, rx_addr, rx_length);
				cstream->stream.gdma_cnt++;
			} else {
				HAL_AUDIO_IRQ_INFO("buffer near full, overrun(%lldns)", OsalGetSysTimeNs(OSAL_TIME_MONOTONIC));
				cstream->stream.restart_by_user = true;
				AUDIO_SP_SetPhaseLatch(cstream->stream.sport_dev_num);
				cstream->stream.total_counter += AUDIO_SP_GetRXCounterVal(cstream->stream.sport_dev_num);
				AUDIO_SP_SetRXCounter(cstream->stream.sport_dev_num, DISABLE);
			}
		}

		if (cstream->stream.sem_need_post) {
			rtw_up_sema_from_isr(&cstream->stream.sem);
		}
	} else if (gdata->gdma_id == 1) {
		rx_length = cstream->stream.period_bytes * cstream->stream.extra_channel / (cstream->stream.channel + cstream->stream.extra_channel);
		ameba_audio_stream_buffer_update_rx_writeptr(cstream->stream.extra_rbuffer, rx_length);
		cstream->stream.extra_gdma_irq_cnt++;

		if (cstream->stream.extra_sem_gdma_end_need_post) {
			rtw_up_sema_from_isr(cstream->stream.extra_sem_gdma_end);
			return 0;
		}

		if (cstream->stream.extra_restart_by_user == true || cstream->stream.gdma_need_stop) {
			return 0;
		}

		if (ameba_audio_stream_buffer_get_available_size(cstream->stream.extra_rbuffer) == 0) {
			HAL_AUDIO_IRQ_INFO("extra buffer full, overrun");
			cstream->stream.extra_restart_by_user = true;
			AUDIO_SP_SetPhaseLatch(cstream->stream.sport_dev_num);
			cstream->stream.total_counter += AUDIO_SP_GetRXCounterVal(cstream->stream.sport_dev_num);
			AUDIO_SP_SetRXCounter(cstream->stream.sport_dev_num, DISABLE);
		} else {
			rx_addr = (uint32_t)(cstream->stream.extra_rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->stream.extra_rbuffer));

			if ((ameba_audio_stream_buffer_get_available_size(cstream->stream.extra_rbuffer) >= rx_length) && !(cstream->stream.gdma_need_stop)) {
				AUDIO_SP_RXGDMA_Restart(rxgdma_initstruct->GDMA_Index, rxgdma_initstruct->GDMA_ChNum, rx_addr, rx_length);
				cstream->stream.extra_gdma_cnt++;
			} else {
				HAL_AUDIO_IRQ_INFO("extra buffer near full, overrun(%lldns)", OsalGetSysTimeNs(OSAL_TIME_MONOTONIC));
				cstream->stream.extra_restart_by_user = true;
				AUDIO_SP_SetPhaseLatch(cstream->stream.sport_dev_num);
				cstream->stream.total_counter += AUDIO_SP_GetRXCounterVal(cstream->stream.sport_dev_num);
				AUDIO_SP_SetRXCounter(cstream->stream.sport_dev_num, DISABLE);
			}
		}
		if (cstream->stream.extra_sem_need_post) {
			rtw_up_sema_from_isr(&cstream->stream.extra_sem);
		}
	}

	return 0;
}

static void ameba_audio_stream_rx_start_in_noirq_mode(Stream *stream)
{
	CaptureStream *cstream = (CaptureStream *)stream;
	PGDMA_InitTypeDef sp_rxgdma_initstruct = &(cstream->stream.gdma_struct->u.SpRxGdmaInitStruct);

	AUDIO_SP_DmaCmd(cstream->stream.sport_dev_num, ENABLE);

	if (!cstream->stream.start_gdma) {
		cstream->stream.start_gdma = true;
		AUDIO_SP_LLPRXGDMA_Init(cstream->stream.sport_dev_num, GDMA_INT, sp_rxgdma_initstruct, cstream->stream.gdma_struct,
								(IRQ_FUN)NULL,
								cstream->stream.period_bytes, cstream->stream.period_count, cstream->stream.gdma_ch_lli);

		HAL_AUDIO_INFO("gdma init: index:%d, chNum:%d", sp_rxgdma_initstruct->GDMA_Index,
					   sp_rxgdma_initstruct->GDMA_ChNum);

		AUDIO_SP_RXStart(cstream->stream.sport_dev_num, ENABLE);

	}
}

static void ameba_audio_stream_rx_start_in_irq_mode(Stream *stream)
{
	CaptureStream *cstream = (CaptureStream *)stream;
	PGDMA_InitTypeDef sp_rxgdma_initstruct = &(cstream->stream.gdma_struct->u.SpRxGdmaInitStruct);

	AUDIO_SP_DmaCmd(cstream->stream.sport_dev_num, ENABLE);

	cstream->stream.gdma_need_stop = false;

	if (!cstream->stream.start_gdma) {
		uint32_t rx_addr;
		uint32_t len = cstream->stream.period_bytes * cstream->stream.channel / (cstream->stream.channel + cstream->stream.extra_channel);
		rx_addr = (uint32_t)(cstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->stream.rbuffer));
		AUDIO_SP_RXGDMA_Init(cstream->stream.sport_dev_num, GDMA_INT, sp_rxgdma_initstruct, cstream->stream.gdma_struct,
							 (IRQ_FUN)ameba_audio_stream_rx_complete, (u8 *)rx_addr, len);
		cstream->stream.gdma_cnt++;

		if (cstream->stream.extra_channel) {
			uint32_t extra_rx_addr;
			uint32_t extra_len = cstream->stream.period_bytes * cstream->stream.extra_channel / (cstream->stream.channel + cstream->stream.extra_channel);
			extra_rx_addr = (uint32_t)(cstream->stream.extra_rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->stream.extra_rbuffer));
			PGDMA_InitTypeDef extra_sp_rxgdma_initstruct = &(cstream->stream.extra_gdma_struct->u.SpRxGdmaInitStruct);
			AUDIO_SP_RXGDMA_Init(cstream->stream.sport_dev_num, GDMA_EXT, extra_sp_rxgdma_initstruct, cstream->stream.extra_gdma_struct,
								 (IRQ_FUN)ameba_audio_stream_rx_complete, (u8 *)extra_rx_addr, extra_len);
			cstream->stream.extra_gdma_cnt++;
		}

		HAL_AUDIO_VERBOSE("RX START AT:%lld", OsalGetSysTimeNs(OSAL_TIME_MONOTONIC));
		AUDIO_SP_RXStart(cstream->stream.sport_dev_num, ENABLE);

		cstream->stream.start_gdma = true;
	}

#if HAL_AUDIO_CAPTURE_DUMP_DEBUG
	ameba_audio_dump_gdma_regs(sp_rxgdma_initstruct->GDMA_ChNum);
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
	cstream->stream.trigger_tstamp = ameba_audio_get_now_ns();
	cstream->stream.total_counter = 0;
	cstream->stream.sport_irq_count = 0;
	AUDIO_SP_SetRXCounterCompVal(cstream->stream.sport_dev_num, cstream->stream.sport_compare_val);
	AUDIO_SP_SetRXCounter(cstream->stream.sport_dev_num, ENABLE);
	AUDIO_SP_SetPhaseLatch(cstream->stream.sport_dev_num);

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
	if (cstream->stream.restart_by_user) {
		uint32_t rx_addr;
		uint32_t bytes = cstream->stream.period_bytes * cstream->stream.channel / (cstream->stream.channel + cstream->stream.extra_channel);
		if (ameba_audio_stream_buffer_get_available_size(cstream->stream.rbuffer) >= bytes) {
			GDMA_InitTypeDef sp_rxgdma_initstruct = cstream->stream.gdma_struct->u.SpRxGdmaInitStruct;
			rx_addr = (uint32_t)(cstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->stream.rbuffer));

			AUDIO_SP_RXGDMA_Restart(sp_rxgdma_initstruct.GDMA_Index, sp_rxgdma_initstruct.GDMA_ChNum, rx_addr, bytes);
			AUDIO_SP_SetRXCounter(cstream->stream.sport_dev_num, ENABLE);
			AUDIO_SP_SetPhaseLatch(cstream->stream.sport_dev_num);
			cstream->stream.gdma_cnt++;
			cstream->stream.restart_by_user = false;
		}
	}

	if (cstream->stream.extra_restart_by_user) {
		uint32_t extra_rx_addr;
		uint32_t extra_bytes = cstream->stream.period_bytes * cstream->stream.extra_channel / (cstream->stream.channel + cstream->stream.extra_channel);

		if (ameba_audio_stream_buffer_get_available_size(cstream->stream.extra_rbuffer) >= extra_bytes) {
			GDMA_InitTypeDef extra_sp_rxgdma_initstruct = cstream->stream.extra_gdma_struct->u.SpRxGdmaInitStruct;
			extra_rx_addr = (uint32_t)(cstream->stream.extra_rbuffer->raw_data + ameba_audio_stream_buffer_get_rx_writeptr(cstream->stream.extra_rbuffer));

			AUDIO_SP_RXGDMA_Restart(extra_sp_rxgdma_initstruct.GDMA_Index, extra_sp_rxgdma_initstruct.GDMA_ChNum, extra_rx_addr, extra_bytes);
			AUDIO_SP_SetRXCounter(cstream->stream.sport_dev_num, ENABLE);
			AUDIO_SP_SetPhaseLatch(cstream->stream.sport_dev_num);
			cstream->stream.extra_gdma_cnt++;
			cstream->stream.extra_restart_by_user = false;
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

	uint32_t total_bytes = bytes * cstream->stream.channel / (cstream->stream.channel + cstream->stream.extra_channel);
	uint32_t bytes_to_read = total_bytes;
	uint32_t bytes_read = 0;

	char *p_buf = NULL;
	char *p_extra_buf = NULL;

	if (cstream->stream.extra_channel) {
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
	if (cstream->stream.extra_channel) {
		uint32_t extra_total_bytes = bytes * cstream->stream.extra_channel / (cstream->stream.channel + cstream->stream.extra_channel);
		uint32_t extra_bytes_to_read = extra_total_bytes;
		uint32_t extra_bytes_read = 0;

		p_extra_buf = (char *)calloc(extra_total_bytes, sizeof(char));

		while (extra_bytes_to_read != 0) {
			ameba_audio_stream_rx_check_and_start_gdma(cstream);
			extra_bytes_read = ameba_audio_stream_buffer_read(cstream->stream.extra_rbuffer, (u8 *)p_extra_buf + extra_total_bytes - extra_bytes_to_read,
							   extra_bytes_to_read,
							   cstream->stream.stream_mode);
			extra_bytes_to_read -= extra_bytes_read;

			if (ameba_audio_stream_buffer_get_remain_size(cstream->stream.extra_rbuffer) < extra_bytes_to_read) {
				cstream->stream.extra_sem_need_post = true;
				rtw_down_sema(&cstream->stream.extra_sem);
			}
			cstream->stream.extra_sem_need_post = false;
		}

	}

	ameba_audio_stream_rx_check_and_start_gdma(cstream);

	if (cstream->stream.extra_channel) {
		uint32_t idx = 0;
		for (; idx < bytes / cstream->stream.config.frame_size; idx++) {
			memcpy((char *)data + idx * cstream->stream.config.frame_size, p_buf + idx * cstream->stream.frame_size, cstream->stream.frame_size);
			memcpy((char *)data + idx * cstream->stream.config.frame_size + cstream->stream.frame_size,
				   p_extra_buf + idx * cstream->stream.extra_frame_size, cstream->stream.extra_frame_size);
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

void ameba_audio_stream_rx_stop(Stream *stream)
{
	CaptureStream *cstream = (CaptureStream *)stream;
	cstream->stream.trigger_tstamp = ameba_audio_get_now_ns();
	cstream->stream.total_counter = 0;
	cstream->stream.sport_irq_count = 0;

	AUDIO_SP_SetRXCounter(cstream->stream.sport_dev_num, DISABLE);

	cstream->stream.gdma_need_stop = true;

	if (cstream->stream.gdma_cnt != cstream->stream.gdma_irq_cnt) {
		cstream->stream.sem_gdma_end_need_post = true;
		rtw_down_sema(&cstream->stream.sem_gdma_end);
		cstream->stream.sem_gdma_end_need_post = false;
	}
	if (cstream->stream.extra_gdma_cnt != cstream->stream.extra_gdma_irq_cnt) {
		cstream->stream.extra_sem_gdma_end_need_post = true;
		rtw_down_sema(&cstream->stream.extra_sem_gdma_end);
		cstream->stream.extra_sem_gdma_end_need_post = false;
	}

	AUDIO_SP_DmaCmd(cstream->stream.sport_dev_num, DISABLE);
	AUDIO_SP_RXStart(cstream->stream.sport_dev_num, DISABLE);

}

void ameba_audio_stream_rx_close(Stream *stream)
{
	CaptureStream *cstream = (CaptureStream *)stream;

	if (cstream) {

		GDMA_InitTypeDef sp_rxgdma_initstruct = cstream->stream.gdma_struct->u.SpRxGdmaInitStruct;
		HAL_AUDIO_INFO("dma clear: index:%d, chNum:%d", sp_rxgdma_initstruct.GDMA_Index, sp_rxgdma_initstruct.GDMA_ChNum);

		GDMA_ClearINT(sp_rxgdma_initstruct.GDMA_Index, sp_rxgdma_initstruct.GDMA_ChNum);
		GDMA_Cmd(sp_rxgdma_initstruct.GDMA_Index, sp_rxgdma_initstruct.GDMA_ChNum, DISABLE);
		GDMA_ChnlFree(sp_rxgdma_initstruct.GDMA_Index, sp_rxgdma_initstruct.GDMA_ChNum);

		if (cstream->stream.extra_channel) {
			GDMA_InitTypeDef extra_sp_rxgdma_initstruct = cstream->stream.extra_gdma_struct->u.SpRxGdmaInitStruct;
			GDMA_ClearINT(extra_sp_rxgdma_initstruct.GDMA_Index, extra_sp_rxgdma_initstruct.GDMA_ChNum);
			GDMA_Cmd(extra_sp_rxgdma_initstruct.GDMA_Index, extra_sp_rxgdma_initstruct.GDMA_ChNum, DISABLE);
			GDMA_ChnlFree(extra_sp_rxgdma_initstruct.GDMA_Index, extra_sp_rxgdma_initstruct.GDMA_ChNum);
		}

		AUDIO_SP_Deinit(cstream->stream.sport_dev_num, SP_DIR_RX);

		ameba_audio_reset_audio_ip_status((Stream *)cstream);

		rtw_free_sema(&cstream->stream.sem);
		rtw_free_sema(&cstream->stream.extra_sem);
		rtw_free_sema(&cstream->stream.sem_gdma_end);
		rtw_free_sema(&cstream->stream.extra_sem_gdma_end);

		if (cstream->stream.rbuffer) {
			ameba_audio_stream_buffer_release(cstream->stream.rbuffer);
		}

		if (cstream->stream.extra_rbuffer) {
			ameba_audio_stream_buffer_release(cstream->stream.extra_rbuffer);
		}

		if (cstream->stream.gdma_struct) {
			free(cstream->stream.gdma_struct);
			cstream->stream.gdma_struct = NULL;
		}

		if (cstream->stream.extra_gdma_struct) {
			free(cstream->stream.extra_gdma_struct);
			cstream->stream.extra_gdma_struct = NULL;
		}

		if (cstream->stream.gdma_ch_lli) {
			ameba_audio_gdma_free(cstream->stream.gdma_ch_lli);
			cstream->stream.gdma_ch_lli = NULL;
		}

		free(cstream);

	}
}
