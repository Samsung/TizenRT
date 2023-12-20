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

#include "audio_hw_compat.h"
#include <inttypes.h>

#include "ameba_audio_stream_control.h"
#include "ameba_audio_stream_utils.h"
#include "ameba_audio_stream_buffer.h"
#include "ameba_audio_types.h"
#include "ameba_audio_clock.h"
#include "ameba_audio_hw_usrcfg.h"
#include "ameba_soc.h"
#include "ameba_rcc.h"
#include "audio_hw_debug.h"
#include "audio_hw_osal_errnos.h"
#include "osal_c/osal_time.h"
#include "platform_stdlib.h"

extern void PLL_I2S_24P576M(u32 NewState);
extern void AUDIO_SP_SetMclk(u32 index, u32 NewState);
extern void AUDIO_SP_SetMclkDiv(u32 index, u32 mck_div);

#define DEBUG_FRAMES  0
#if DEBUG_FRAMES
static uint64_t s_counter_frames = 0;
static uint64_t s_dma_frames = 0;
#endif

int ameba_audio_stream_tx_set_amp_state(bool state)
{
	StreamControl *control = ameba_audio_get_ctl();
	if (control == NULL) {
		HAL_AUDIO_ERROR("Audio driver control is null initialized\n");
		return -1;
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

	rstream->stream.sport_irq_count++;
	rstream->stream.total_counter += rstream->stream.sport_compare_val;
	if (rstream->stream.total_counter >= rstream->stream.total_counter_boundary) {
		rstream->stream.total_counter -= rstream->stream.total_counter_boundary;
		rstream->stream.sport_irq_count = 0;
	}

	HAL_AUDIO_PVERBOSE("total_counter:%" PRIu64 " \n", rstream->stream.total_counter);
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

	AUDIO_SP_SetPhaseLatch(rstream->stream.sport_dev_num);
	counter = AUDIO_SP_GetTXCounterVal(rstream->stream.sport_dev_num);
	total_counter = counter + (uint64_t)rstream->stream.sport_irq_count * rstream->stream.sport_compare_val;

	return total_counter;
}

static uint64_t ameba_audio_stream_tx_get_counter_ntime(RenderStream *rstream)
{
	uint64_t usec = 0;
	//current total i2s counter of audio frames;
	uint64_t now_counter = 0;
	//means the delta_counter between now counter and last irq total counter.
	uint32_t delta_counter = 0;

	if (!rstream) {
		return -1;
	}

	AUDIO_SP_SetPhaseLatch(rstream->stream.sport_dev_num);
	delta_counter = AUDIO_SP_GetTXCounterVal(rstream->stream.sport_dev_num);
	now_counter = rstream->stream.total_counter + delta_counter;

	usec = now_counter * 1000000LL / rstream->stream.rate;
	HAL_AUDIO_PVERBOSE("now_counter:%" PRIu64 ", usec:%" PRIu64 " delta_counter:%" PRIu32 ", total:%" PRIu64 "\n",
					   now_counter, usec, delta_counter, rstream->stream.total_counter);

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
	usec += rstream->stream.trigger_tstamp / 1000;

	//tv_sec is lld, tv_nsec is ld
	tstamp->tv_sec = usec / 1000000;
	tstamp->tv_nsec = (long)((usec - tstamp->tv_sec * 1000000LL) * 1000);

	*avail = ameba_audio_stream_buffer_get_available_size(rstream->stream.rbuffer) / rstream->stream.frame_size;

	HAL_AUDIO_PVERBOSE("avail:%" PRIu32 ", trigger:%" PRIu64 ", usec:%" PRIu64 ", tv_sec:%" PRIu64 ", tv_nsec:%" PRIu32 "",
					   *avail, rstream->stream.trigger_tstamp, usec, tstamp->tv_sec, tstamp->tv_nsec);

	return 0;
}

int  ameba_audio_stream_tx_get_position(Stream *stream, uint64_t *rendered_frames, struct timespec *tstamp)
{
	//now nsec;
	uint64_t nsec;
	//current total i2s counter of audio frames;
	uint64_t now_counter = 0;
	//means the delta_counter between now counter and last irq total counter.
	uint32_t delta_counter = 0;

	uint64_t ratio_ns = 0;

	RenderStream *rstream = (RenderStream *)stream;
	if (!rstream) {
		return -1;
	}

	PGDMA_InitTypeDef sp_txgdma_initstruct = &(rstream->stream.gdma_struct->u.SpTxGdmaInitStruct);

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_COUNTER || DEBUG_FRAMES) {
		AUDIO_SP_SetPhaseLatch(rstream->stream.sport_dev_num);
		delta_counter = AUDIO_SP_GetTXCounterVal(rstream->stream.sport_dev_num);
		now_counter = rstream->stream.total_counter + delta_counter;
#if DEBUG_FRAMES
		s_counter_frames = now_counter;
#else
		*rendered_frames = now_counter;
#endif
	}

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_DMA || DEBUG_FRAMES) {
#if DEBUG_FRAMES
		s_dma_frames = (rstream->stream.total_dma_bytes +
						GDMA_GetBlkSize(sp_txgdma_initstruct->GDMA_Index, sp_txgdma_initstruct->GDMA_ChNum)) / rstream->stream.frame_size;
#else
		*rendered_frames = (rstream->stream.total_dma_bytes +
							GDMA_GetBlkSize(sp_txgdma_initstruct->GDMA_Index, sp_txgdma_initstruct->GDMA_ChNum)) / rstream->stream.frame_size;
#endif
	}

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_RATIO) {
		ratio_ns = (OsalGetAudioClockTimeNs(OSAL_TIME_MONOTONIC, OSAL_AUDIO_CLOCK_1) - rstream->stream.start_atstamp);
		*rendered_frames =  ratio_ns * rstream->stream.rate / 1000000000LL;
	}

	//tv_sec is lld, tv_nsec is ld
	//nsec will exceed at (2^64 / 50M / 3600 / 24 / 365 / 20 = 584 years)
	nsec = ameba_audio_get_now_ns();
	tstamp->tv_sec = nsec / 1000000000LL;
	tstamp->tv_nsec = nsec - tstamp->tv_sec * 1000000000LL;

	HAL_AUDIO_PVERBOSE("rendered_frames:%" PRIu64 ", trigger:%" PRIu64 ", usec:%" PRIu64 ", tv_sec:%" PRIu64 ", tv_nsec:%" PRIu32 "",
					   *rendered_frames, rstream->stream.trigger_tstamp, nsec, tstamp->tv_sec, tstamp->tv_nsec);

#if DEBUG_FRAMES
	if (rstream->write_cnt % 2000 == 0) {
		HAL_AUDIO_DEBUG("counter frames:%llu, dma frames:%llu, ratio frames:%llu, ratio ns:%llu", s_counter_frames, s_dma_frames, *rendered_frames, ratio_ns);
	}
#endif

	return 0;
}

int  ameba_audio_stream_tx_get_time(Stream *stream, int64_t *now_ns, int64_t *audio_ns)
{
	//now nsec;
	uint64_t nsec;
	//current total i2s counter of audio frames;
	uint64_t now_counter = 0;
	//means the delta_counter between now counter and last irq total counter.
	uint32_t delta_counter = 0;

	uint32_t phase = 0;

	RenderStream *rstream = (RenderStream *)stream;
	if (!rstream) {
		return -1;
	}

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_COUNTER) {
		AUDIO_SP_SetPhaseLatch(rstream->stream.sport_dev_num);
		delta_counter = AUDIO_SP_GetTXCounterVal(rstream->stream.sport_dev_num);
		phase = AUDIO_SP_GetTXPhaseVal(rstream->stream.sport_dev_num);
		now_counter = rstream->stream.total_counter + delta_counter;
	} else {
		HAL_AUDIO_ERROR("not supported tstamp type(%d) now.", AUDIO_OUT_TSTAMP_TYPE);
	}

	//nsec will exceed at (2^64 / 50M / 3600 / 24 / 365 / 20 = 584 years)
	nsec = ameba_audio_get_now_ns();

	*now_ns = nsec;
	*audio_ns = (int64_t)((double)((double)now_counter + (double)phase / (double)32) / (double)rstream->stream.config.rate * (double)1000000000);

	return 0;
}

static void ameba_audo_stream_tx_codec_configure(uint32_t i2s, uint32_t type, uint32_t channels, I2S_InitTypeDef *i2s_initstruct)
{
	AUDIO_CODEC_SetI2SIP(i2s, ENABLE);
	AUDIO_CODEC_SetI2SSRC(i2s, INTERNAL_SPORT);
	AUDIO_CODEC_SetI2SParameters(i2s, DACPATH, i2s_initstruct);

	AUDIO_CODEC_SetDACSRSrc(SOURCE0, i2s_initstruct->CODEC_SelI2STxSR);
	AUDIO_CODEC_EnableDAC(DAC_L, ENABLE);
	AUDIO_CODEC_EnableDACFifo(ENABLE);
	AUDIO_CODEC_SetDACHPF(DAC_L, ENABLE);
	AUDIO_CODEC_SetDACMute(DAC_L, ameba_audio_get_ctl()->tx_state);
	AUDIO_CODEC_SetDACSrc(i2s, I2SL, I2SR);

	//for I2S PLL case
	if (AUDIO_HW_OUT_SPORT_CLK_TYPE == 1) {
		AUDIO_CODEC_SetDACASRC(i2s_initstruct->CODEC_SelI2STxSR, ENABLE);
	}

	if (channels == 2) {
		AUDIO_CODEC_EnableDAC(DAC_R, ENABLE);
		AUDIO_CODEC_EnableDACFifo(ENABLE);
		AUDIO_CODEC_SetDACHPF(DAC_R, ENABLE);
		AUDIO_CODEC_SetDACMute(DAC_R, ameba_audio_get_ctl()->tx_state);
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
			ameba_audio_stream_tx_set_amp_state(ameba_audio_get_ctl()->amp_state);
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

static void ameba_audio_stream_tx_sport_init(RenderStream **stream, StreamConfig config, uint32_t device)
{
	RenderStream *rstream = *stream;
	AUDIO_ClockParams Clock_Params;
	AUDIO_InitParams Init_Params;
	uint32_t clock_mode;
	uint32_t mck_div;

	AUDIO_SP_StructInit(&rstream->stream.sp_initstruct);
	rstream->stream.sp_initstruct.SP_SelI2SMonoStereo = ameba_audio_get_channel(config.channels);
	rstream->stream.sp_initstruct.SP_SelWordLen = ameba_audio_get_sp_format(config.format, rstream->stream.direction);
	rstream->stream.sp_initstruct.SP_SR = ameba_audio_get_sp_rate(config.rate);
	rstream->stream.sp_initstruct.SP_SelTDM = ameba_audio_get_sp_tdm(config.channels);
	rstream->stream.sp_initstruct.SP_SelFIFO = ameba_audio_get_fifo_num(config.channels);
	HAL_AUDIO_VERBOSE("selmo:%lu, wordlen:%lu, sr:%lu, seltdm:%lu, selfifo:%lu,",
					  rstream->stream.sp_initstruct.SP_SelI2SMonoStereo,
					  rstream->stream.sp_initstruct.SP_SelWordLen,
					  rstream->stream.sp_initstruct.SP_SR,
					  rstream->stream.sp_initstruct.SP_SelTDM,
					  rstream->stream.sp_initstruct.SP_SelFIFO);

	Init_Params.chn_len = SP_CL_32;
	Init_Params.chn_cnt = config.channels;
	Init_Params.sr = rstream->stream.sp_initstruct.SP_SR;

	if (AUDIO_HW_OUT_SPORT_CLK_TYPE == 0) {
		Init_Params.codec_multiplier_with_rate = 0;
		Init_Params.sport_mclk_fixed_max = 0;
		Audio_Clock_Choose(XTAL_CLOCK, &Init_Params, &Clock_Params);
	}

	if (AUDIO_HW_OUT_SPORT_CLK_TYPE == 1) {
		Init_Params.codec_multiplier_with_rate = AUDIO_HW_OUT_MCLK_MULITIPLIER;
		Init_Params.sport_mclk_fixed_max = 0;
		Audio_Clock_Choose(PLL_CLK, &Init_Params, &Clock_Params);
	}

	if (AUDIO_HW_OUT_SPORT_CLK_TYPE == 2) {
		Init_Params.codec_multiplier_with_rate = 0;
		Init_Params.sport_mclk_fixed_max = AUDIO_HW_OUT_FIXED_MCLK;
		Audio_Clock_Choose(PLL_CLK, &Init_Params, &Clock_Params);
	}

	switch (Clock_Params.Clock) {
	case PLL_CLOCK_24P576M:
		PLL_I2S_24P576M(ENABLE);
		RCC_PeriphClockSource_SPORT(rstream->stream.sport_dev_num, CKSL_I2S_PLL24M);
		PLL_I2S_Div(rstream->stream.sport_dev_num, Clock_Params.PLL_DIV);
		clock_mode = PLL_CLOCK_24P576M / Clock_Params.PLL_DIV;
		break;

	case PLL_CLOCK_45P1584M:
		PLL_I2S_45P158M(ENABLE);
		RCC_PeriphClockSource_SPORT(rstream->stream.sport_dev_num, CKSL_I2S_PLL45M);
		PLL_I2S_Div(rstream->stream.sport_dev_num, Clock_Params.PLL_DIV);
		PLL_I2S_45P158M_ClkTune(0, PLL_AUTO);
		clock_mode = PLL_CLOCK_45P1584M / Clock_Params.PLL_DIV;
		break;

	case PLL_CLOCK_98P304M:
		PLL_I2S_98P304M(ENABLE);
		RCC_PeriphClockSource_SPORT(rstream->stream.sport_dev_num, CKSL_I2S_PLL98M);
		PLL_I2S_Div(rstream->stream.sport_dev_num, Clock_Params.PLL_DIV);
		clock_mode = PLL_CLOCK_98P304M / Clock_Params.PLL_DIV;
		break;

	case I2S_CLOCK_XTAL40M:
		clock_mode = I2S_CLOCK_XTAL40M;
		RCC_PeriphClockSource_SPORT(rstream->stream.sport_dev_num, CKSL_I2S_XTAL40M);
		break;
	}

	rstream->stream.sp_initstruct.SP_SelClk = clock_mode;

	if (AUDIO_I2S_OUT_MULTIIO_EN == 1) {
		rstream->stream.sp_initstruct.SP_SetMultiIO = SP_TX_MULTIIO_EN;
	} else {
		rstream->stream.sp_initstruct.SP_SetMultiIO = SP_TX_MULTIIO_DIS;
	}

	AUDIO_SP_Init(rstream->stream.sport_dev_num, SP_DIR_TX, &rstream->stream.sp_initstruct);

	if (device == AMEBA_AUDIO_DEVICE_I2S) {
		if (AUDIO_I2S_OUT_NEED_MCLK_OUT == 1) {
			//enable mclk and set div
			AUDIO_SP_SetMclk(rstream->stream.sport_dev_num, ENABLE);
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
			AUDIO_SP_SetMclkDiv(rstream->stream.sport_dev_num, mck_div);
		}

		AUDIO_SP_SetMasterSlave(rstream->stream.sport_dev_num, MASTER);
	}
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
		HAL_AUDIO_ERROR("unsupported device:%ld", ameba_audio_ctl_get_device_category(ameba_audio_get_ctl()));
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
		HAL_AUDIO_VERBOSE("ameba_audio_stream_tx_llp_init, addr: %lx", ch_lli[j].LliEle.Sarx);

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

Stream *ameba_audio_stream_tx_init(uint32_t device, StreamConfig config)
{
	RenderStream *rstream;
	size_t buf_size;

	rstream = (RenderStream *)calloc(1, sizeof(RenderStream));
	if (!rstream) {
		HAL_AUDIO_ERROR("calloc stream fail");
		return NULL;
	}

	rstream->write_cnt = 0;
	rstream->stream.config = config;
	rstream->stream.direction = STREAM_OUT;
	rstream->stream.device = device;

	if (device == AMEBA_AUDIO_DEVICE_I2S) {
		rstream->stream.sport_dev_num = AUDIO_I2S_OUT_SPORT_INDEX;
		if (AUDIO_I2S_OUT_NEED_MCLK_OUT) {
			Pinmux_Config(AUDIO_I2S_OUT_MCLK_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_OUT_SPORT_INDEX));
		}
		Pinmux_Config(AUDIO_I2S_OUT_BCLK_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_OUT_SPORT_INDEX));
		Pinmux_Config(AUDIO_I2S_OUT_LRCLK_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_OUT_SPORT_INDEX));
		Pinmux_Config(AUDIO_I2S_OUT_DATA0_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_OUT_SPORT_INDEX));
		if (AUDIO_I2S_OUT_MULTIIO_EN == 1) {
			Pinmux_Config(AUDIO_I2S_OUT_DATA1_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_OUT_SPORT_INDEX));
			Pinmux_Config(AUDIO_I2S_OUT_DATA2_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_OUT_SPORT_INDEX));
			Pinmux_Config(AUDIO_I2S_OUT_DATA3_PIN, ameba_audio_get_i2s_pin_func(AUDIO_I2S_OUT_SPORT_INDEX));
		}
	} else {
		rstream->stream.sport_dev_num = 0;
	}
	rstream->stream.sport_dev_addr = ameba_audio_get_sport_addr(rstream->stream.sport_dev_num);

	ameba_audio_periphclock_init(rstream->stream.sport_dev_num);

	if (!ameba_audio_is_audio_ip_in_use(SPORT0)) {
		AUDIO_SP_Reset(rstream->stream.sport_dev_num);
	}

	/*configure sport according to the parameters*/
	ameba_audio_stream_tx_sport_init(&rstream, config, device);
	if (rstream->stream.sport_dev_num == 0) {
		ameba_audio_set_audio_ip_use_status(rstream->stream.direction, SPORT0, true);
	} else if (rstream->stream.sport_dev_num == 1) {
		ameba_audio_set_audio_ip_use_status(rstream->stream.direction, SPORT1, true);
	} else if (rstream->stream.sport_dev_num == 2) {
		ameba_audio_set_audio_ip_use_status(rstream->stream.direction, SPORT2, true);
	} else if (rstream->stream.sport_dev_num == 3) {
		ameba_audio_set_audio_ip_use_status(rstream->stream.direction, SPORT3, true);
	}

	if (device != AMEBA_AUDIO_DEVICE_I2S) {
		/* enable audio IP */
		if (!ameba_audio_is_audio_ip_in_use(AUDIOIP)) {
			AUDIO_CODEC_SetAudioIP(ENABLE);
		}
		ameba_audio_set_audio_ip_use_status(rstream->stream.direction, AUDIOIP, true);

		/*configure codec according to the parameters*/
		ameba_audio_stream_tx_codec_init(&rstream, config);
		ameba_audio_set_audio_ip_use_status(rstream->stream.direction, CODEC, true);
	}

	buf_size = config.period_size * config.frame_size * config.period_count;
	rstream->stream.stream_mode = config.mode;
	rstream->stream.period_count = config.period_count;
	rstream->stream.period_bytes = config.period_size * config.frame_size;
	rstream->stream.rate = config.rate;

	if (!IS_6_8_CHANNEL(config.channels)) {
		rstream->stream.channel = config.channels;
		rstream->stream.extra_channel = 0;
		rstream->stream.rbuffer = ameba_audio_stream_buffer_create();
		if (rstream->stream.rbuffer) {
			ameba_audio_stream_buffer_alloc(rstream->stream.rbuffer, buf_size);
		}
		rstream->stream.extra_rbuffer = NULL;
	} else {
		rstream->stream.channel = 4;
		rstream->stream.extra_channel = config.channels - rstream->stream.channel;
		rstream->stream.rbuffer = ameba_audio_stream_buffer_create();
		if (rstream->stream.rbuffer) {
			ameba_audio_stream_buffer_alloc(rstream->stream.rbuffer, buf_size * rstream->stream.channel / config.channels);
		}
		rstream->stream.extra_rbuffer = ameba_audio_stream_buffer_create();
		if (rstream->stream.extra_rbuffer) {
			ameba_audio_stream_buffer_alloc(rstream->stream.extra_rbuffer, buf_size * rstream->stream.extra_channel / config.channels);
		}
	}

	rstream->stream.start_gdma = false;
	rstream->stream.gdma_need_stop = false;
	rstream->stream.frame_size = config.frame_size * rstream->stream.channel / config.channels;
	rstream->stream.gdma_cnt = 0;
	rstream->stream.gdma_irq_cnt = 0;
	rstream->stream.sem_need_post = false;
	rstream->stream.sem_gdma_end_need_post = false;

	rstream->stream.gdma_struct = (GdmaCallbackData *)calloc(1, sizeof(GdmaCallbackData));
	if (!rstream->stream.gdma_struct) {
		HAL_AUDIO_ERROR("calloc gdma_struct fail");
		return NULL;
	}
	rstream->stream.gdma_struct->stream = (Stream *)rstream;
	rstream->stream.gdma_struct->gdma_id = 0;
	rtw_init_sema(&rstream->stream.sem, 0);
	rtw_init_sema(&rstream->stream.sem_gdma_end, 0);

	rstream->stream.extra_gdma_struct = NULL;
	rstream->stream.extra_frame_size = config.frame_size * rstream->stream.extra_channel / config.channels;
	rstream->stream.extra_gdma_cnt = 0;
	rstream->stream.extra_gdma_irq_cnt = 0;
	rstream->stream.extra_sem_need_post = false;
	rstream->stream.extra_sem_gdma_end_need_post = false;

	if (IS_6_8_CHANNEL(config.channels)) {
		rstream->stream.extra_gdma_struct = (GdmaCallbackData *)calloc(1, sizeof(GdmaCallbackData));
		if (!rstream->stream.extra_gdma_struct) {
			HAL_AUDIO_ERROR("calloc extra SPGdmaStruct fail");
			return NULL;
		}
		rstream->stream.extra_gdma_struct->stream = (Stream *)rstream;
		rstream->stream.extra_gdma_struct->gdma_id = 1;

		rtw_init_sema(&rstream->stream.extra_sem, 0);
		rtw_init_sema(&rstream->stream.extra_sem_gdma_end, 0);
	}

	rstream->stream.trigger_tstamp = 0;
	rstream->stream.start_atstamp = 0;
	rstream->stream.total_dma_bytes = 0;
	rstream->stream.total_counter = 0;
	rstream->stream.sport_irq_count = 0;
	rstream->stream.sport_compare_val = config.period_size * config.period_count;
	rstream->stream.total_counter_boundary = UINT64_MAX;
	rstream->total_written_from_tx_start = 0;

	while (rstream->stream.sport_compare_val * 2 <= AUDIO_HW_MAX_SPORT_IRQ_X) {
		rstream->stream.sport_compare_val *= 2;
	}

	rstream->stream.gdma_ch_lli = (struct GDMA_CH_LLI *)ameba_audio_gdma_calloc(rstream->stream.period_count, sizeof(struct GDMA_CH_LLI));

	if (!rstream->stream.gdma_ch_lli) {
		HAL_AUDIO_ERROR("calloc gdma_ch_lli fail");
		return NULL;
	}

	if (rstream->stream.stream_mode == AMEBA_AUDIO_DMA_NOIRQ_MODE) {
		ameba_audio_stream_tx_llp_init(&rstream->stream);
	}

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_COUNTER || DEBUG_FRAMES) {
		//enable sport interrupt
		uint32_t irq = ameba_audio_get_sport_irq(rstream->stream.sport_dev_num);
		InterruptDis(irq);
		InterruptUnRegister(irq);
		InterruptRegister((IRQ_FUN)ameba_audio_stream_tx_sport_interrupt, irq, (uint32_t)rstream, 4);
		InterruptEn(irq, 4);
	}

	ameba_audio_set_native_time();

	rstream->stream.state = STATE_INITED;

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

void ameba_audio_stream_tx_start(Stream *stream, int32_t state)
{
	RenderStream *rstream = (RenderStream *)stream;

	if (state == rstream->stream.state) {
		return;
	}

	ameba_audio_ctl_set_tx_mute(ameba_audio_get_ctl(), ameba_audio_get_ctl()->tx_state, true, false);

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_COUNTER || DEBUG_FRAMES) {
		AUDIO_SP_SetTXCounterCompVal(rstream->stream.sport_dev_num, rstream->stream.sport_compare_val);
	}

	AUDIO_SP_TXSetFifo(rstream->stream.sport_dev_num, rstream->stream.sp_initstruct.SP_SelFIFO, ENABLE);

	if (rstream->stream.device == AMEBA_AUDIO_DEVICE_SPEAKER || rstream->stream.device == AMEBA_AUDIO_DEVICE_HEADPHONE) {
		AUDIO_CODEC_EnableDACFifo(ENABLE);
	}

	AUDIO_SP_DmaCmd(rstream->stream.sport_dev_num, ENABLE);

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_COUNTER || DEBUG_FRAMES) {
		AUDIO_SP_SetPhaseLatch(rstream->stream.sport_dev_num);
		AUDIO_SP_SetTXCounter(rstream->stream.sport_dev_num, ENABLE);
	}

	AUDIO_SP_TXStart(rstream->stream.sport_dev_num, ENABLE);

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_RATIO) {
		rstream->stream.start_atstamp = OsalGetAudioClockTimeNs(OSAL_TIME_MONOTONIC, OSAL_AUDIO_CLOCK_1);
	}

	rstream->stream.trigger_tstamp = ameba_audio_get_now_ns();
	rstream->stream.total_counter = 0;
	rstream->stream.sport_irq_count = 0;
	rstream->stream.gdma_need_stop = false;
	//should not set zero here, because when user write data after xrun, it may not up to start threhold bytes.
	//rstream->total_written_from_tx_start = 0;

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_DMA || DEBUG_FRAMES) {
		rstream->stream.total_dma_bytes = 0;
	}

	rstream->stream.state = state;

}

void ameba_audio_stream_tx_stop(Stream *stream, int32_t state)
{
	RenderStream *rstream = (RenderStream *)stream;

	//if xrun happens, and fifo should be flushed, if directly flushed, may cause more pop noise than xrun itself.
	//for example, if xrun happens when data is on high level, it will remain in this high level until xrun disappear.
	//but if directly flused, data will directly change to 0, which will cause bigger noise.
	//we need to add mute for this case, and should not add zdet here, because it will takes more time.
	//If mute first, the data will stay on it's high level, only mute function will work.
	bool should_mute_zdet = state == STATE_XRUN ? false : true;

	if (state == rstream->stream.state) {
		return;
	}

	ameba_audio_ctl_set_tx_mute(ameba_audio_get_ctl(), true, should_mute_zdet, false);

	AUDIO_SP_DmaCmd(rstream->stream.sport_dev_num, DISABLE);
	AUDIO_SP_TXStart(rstream->stream.sport_dev_num, DISABLE);

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_COUNTER || DEBUG_FRAMES) {
		AUDIO_SP_SetTXCounter(rstream->stream.sport_dev_num, DISABLE);
		AUDIO_SP_TXSetFifo(rstream->stream.sport_dev_num, rstream->stream.sp_initstruct.SP_SelFIFO, DISABLE);
	}

	if (rstream->stream.device == AMEBA_AUDIO_DEVICE_SPEAKER || rstream->stream.device == AMEBA_AUDIO_DEVICE_HEADPHONE) {
		//flush codec fifo
		AUDIO_CODEC_EnableDACFifo(DISABLE);
	}

	rstream->stream.trigger_tstamp = ameba_audio_get_now_ns();
	rstream->stream.total_counter = 0;
	rstream->stream.sport_irq_count = 0;
	rstream->total_written_from_tx_start = 0;

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_RATIO) {
		rstream->stream.start_atstamp = 0;
	}

	if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_DMA || DEBUG_FRAMES) {
		rstream->stream.total_dma_bytes = 0;
	}

	ameba_audio_stream_tx_buffer_flush(stream);
	rstream->stream.state = state;

}

void ameba_audio_stream_tx_standby(Stream *stream)
{
	RenderStream *rstream = (RenderStream *)stream;
	rstream->stream.gdma_need_stop = true;

	if (rstream->stream.gdma_cnt != rstream->stream.gdma_irq_cnt) {
		rstream->stream.sem_gdma_end_need_post = true;
		rtw_down_sema(&rstream->stream.sem_gdma_end);
		rstream->stream.sem_gdma_end_need_post = false;
	}
	if (rstream->stream.extra_gdma_cnt != rstream->stream.extra_gdma_irq_cnt) {
		rstream->stream.extra_sem_gdma_end_need_post = true;
		rtw_down_sema(&rstream->stream.extra_sem_gdma_end);
		rstream->stream.extra_sem_gdma_end_need_post = false;
	}

	ameba_audio_stream_tx_stop(stream, STATE_STANDBY);
}

//gdma done moving one period size data IRQ. Data is gdma_cb_data
uint32_t ameba_audio_stream_tx_complete(void *data)
{
	uint32_t tx_addr;
	uint32_t tx_length;
	uint32_t extra_tx_addr;
	uint32_t extra_tx_length;

	GdmaCallbackData *gdata = (GdmaCallbackData *) data;
	PGDMA_InitTypeDef txgdma_initstruct = &(gdata->u.SpTxGdmaInitStruct);

	/* Clear Pending ISR */
	GDMA_ClearINT(txgdma_initstruct->GDMA_Index, txgdma_initstruct->GDMA_ChNum);

	RenderStream *rstream = (RenderStream *)(gdata->stream);

	if (gdata->gdma_id == 0) {
		tx_length = rstream->stream.period_bytes * rstream->stream.channel / (rstream->stream.channel + rstream->stream.extra_channel);
		ameba_audio_stream_buffer_update_tx_readptr(rstream->stream.rbuffer, tx_length);
		rstream->stream.gdma_irq_cnt++;

		if (AUDIO_OUT_TSTAMP_TYPE == AUDIO_TSTAMP_BY_DMA || DEBUG_FRAMES) {
			rstream->stream.total_dma_bytes += tx_length;
		}

		if (rstream->stream.sem_gdma_end_need_post) {
			rtw_up_sema_from_isr(rstream->stream.sem_gdma_end);
			return 0;
		}

		if (rstream->stream.gdma_need_stop) {
			return 0;
		}

		if (ameba_audio_stream_buffer_get_available_size(rstream->stream.rbuffer) == rstream->stream.rbuffer->capacity ||
			ameba_audio_stream_buffer_get_remain_size(rstream->stream.rbuffer) < tx_length) {
			HAL_AUDIO_IRQ_INFO("buffer empty,underrun(%lldns)", OsalGetSysTimeNs(OSAL_TIME_MONOTONIC));
			ameba_audio_stream_tx_stop(gdata->stream, STATE_XRUN);
		} else {
			tx_addr = (uint32_t)(rstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.rbuffer));
			AUDIO_SP_TXGDMA_Restart(txgdma_initstruct->GDMA_Index, txgdma_initstruct->GDMA_ChNum, tx_addr, tx_length);
			rstream->stream.gdma_cnt++;
		}

		if (rstream->stream.sem_need_post) {
			rtw_up_sema_from_isr(&rstream->stream.sem);
		}
	} else if (gdata->gdma_id == 1) {
		extra_tx_length = rstream->stream.period_bytes * rstream->stream.extra_channel / (rstream->stream.channel + rstream->stream.extra_channel);
		ameba_audio_stream_buffer_update_tx_readptr(rstream->stream.extra_rbuffer, extra_tx_length);
		rstream->stream.extra_gdma_irq_cnt++;

		if (rstream->stream.extra_sem_gdma_end_need_post) {
			rtw_up_sema_from_isr(rstream->stream.extra_sem_gdma_end);
			return 0;
		}

		if (rstream->stream.gdma_need_stop) {
			return 0;
		}

		if (ameba_audio_stream_buffer_get_available_size(rstream->stream.extra_rbuffer) == rstream->stream.extra_rbuffer->capacity ||
			ameba_audio_stream_buffer_get_remain_size(rstream->stream.extra_rbuffer) < extra_tx_length) {
			HAL_AUDIO_IRQ_INFO("buffer empty,underrun(%lldns)", OsalGetSysTimeNs(OSAL_TIME_MONOTONIC));
			ameba_audio_stream_tx_stop(gdata->stream, STATE_XRUN);
		} else {
			extra_tx_addr = (uint32_t)(rstream->stream.extra_rbuffer->raw_data + ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.extra_rbuffer));
			AUDIO_SP_TXGDMA_Restart(txgdma_initstruct->GDMA_Index, txgdma_initstruct->GDMA_ChNum, extra_tx_addr, extra_tx_length);
			rstream->stream.extra_gdma_cnt++;
		}

		if (rstream->stream.extra_sem_need_post) {
			rtw_up_sema_from_isr(&rstream->stream.extra_sem);
		}
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
			HAL_AUDIO_PVERBOSE("bytes: %" PRIu32 ", rstream->stream.period_bytes:%" PRIu32 ", remain size:%u", bytes, rstream->stream.period_bytes,
							   ameba_audio_stream_buffer_get_remain_size(rstream->stream.rbuffer));
			if (ameba_audio_stream_buffer_get_remain_size(rstream->stream.rbuffer) >= rstream->stream.period_bytes) {
				rstream->stream.start_gdma = true;

				AUDIO_SP_LLPTXGDMA_Init(rstream->stream.sport_dev_num, GDMA_INT, sp_txgdma_initstruct, rstream->stream.gdma_struct,
										(IRQ_FUN)NULL,
										rstream->stream.period_bytes, rstream->stream.period_count, rstream->stream.gdma_ch_lli);
				AUDIO_SP_DmaCmd(rstream->stream.sport_dev_num, ENABLE);
				AUDIO_SP_TXStart(rstream->stream.sport_dev_num, ENABLE);
			}
		}

		bytes_left_to_write -= bytes_written;
	}

	rstream->total_written_from_tx_start += bytes / rstream->stream.config.frame_size;

	return bytes;
}

static int ameba_audio_stream_tx_write_in_irq_mode(Stream *stream, const void *data, uint32_t bytes, bool block)
{
	RenderStream *rstream = (RenderStream *)stream;

	bool has_extra_dma = false;
	uint32_t total_bytes = bytes * rstream->stream.channel / (rstream->stream.channel + rstream->stream.extra_channel);
	uint32_t bytes_left_to_write = bytes * rstream->stream.channel / (rstream->stream.channel + rstream->stream.extra_channel);
	uint32_t bytes_written = 0;
	uint32_t tx_addr;
	PGDMA_InitTypeDef sp_txgdma_initstruct = &(rstream->stream.gdma_struct->u.SpTxGdmaInitStruct);
	(void) block;

	uint32_t extra_total_bytes = 0;
	uint32_t extra_bytes_left_to_write = 0;
	uint32_t extra_bytes_written = 0;
	uint32_t extra_tx_addr;
	PGDMA_InitTypeDef extra_sp_txgdma_initstruct = &(rstream->stream.extra_gdma_struct->u.SpTxGdmaInitStruct);

	char *p_buf = NULL;
	char *p_extra_buf = NULL;

	rstream->write_cnt++;

	if (rstream->stream.state == STATE_XRUN) {
		//If xrun ,return -EPIPE. Application should handle xrun according to the return value.
		//HAL_AUDIO_INFO("xrun happens, state change to STATE_XRUN_NOTIFIED");
		rstream->stream.state = STATE_XRUN_NOTIFIED;
		return HAL_OSAL_ERR_DEAD_OBJECT;
	}

	if (AUDIO_OUT_DEBUG_BUFFER_LEVEL == 1) {
		if (rstream->write_cnt % 100 == 0) {
			HAL_AUDIO_DEBUG("wr cnt:%llu, remain:%dbytes, avail:%dbytes",
							rstream->write_cnt, ameba_audio_stream_buffer_get_remain_size(rstream->stream.rbuffer),
							ameba_audio_stream_buffer_get_available_size(rstream->stream.rbuffer));
		}
	}

	if (rstream->stream.extra_channel) {
		has_extra_dma = true;
	}

	if (has_extra_dma) {
		extra_total_bytes = bytes * rstream->stream.extra_channel / (rstream->stream.channel + rstream->stream.extra_channel);
		extra_bytes_left_to_write = bytes * rstream->stream.extra_channel / (rstream->stream.channel + rstream->stream.extra_channel);
		p_buf = (char *)calloc(total_bytes, sizeof(char));
		p_extra_buf = (char *)calloc(extra_total_bytes, sizeof(char));

		uint32_t idx = 0;
		for (; idx < bytes / rstream->stream.config.frame_size; idx++) {
			memcpy(p_buf + idx * rstream->stream.frame_size, (char *)data + idx * rstream->stream.config.frame_size, rstream->stream.frame_size);
			memcpy(p_extra_buf + idx * rstream->stream.extra_frame_size, (char *)data + idx * rstream->stream.config.frame_size + rstream->stream.frame_size,
				   rstream->stream.extra_frame_size);
		}
	} else {
		p_buf = (char *)data;
	}

	while (bytes_left_to_write != 0 || (extra_bytes_left_to_write != 0)) {
		bytes_written = ameba_audio_stream_buffer_write(rstream->stream.rbuffer, (u8 *)p_buf + total_bytes - bytes_left_to_write, bytes_left_to_write);
		uint32_t dma_len = rstream->stream.period_bytes * rstream->stream.channel / (rstream->stream.channel + rstream->stream.extra_channel);
		uint32_t extra_dma_len = 0;

		if (has_extra_dma) {
			extra_bytes_written = ameba_audio_stream_buffer_write(rstream->stream.extra_rbuffer, (u8 *)p_extra_buf + extra_total_bytes - extra_bytes_left_to_write,
								  extra_bytes_left_to_write);
			extra_dma_len = rstream->stream.period_bytes * rstream->stream.extra_channel / (rstream->stream.channel + rstream->stream.extra_channel);
		}

		if (rstream->stream.state == STATE_INITED) {
			if (ameba_audio_stream_buffer_get_remain_size(rstream->stream.rbuffer) >= dma_len) {
				rstream->stream.start_gdma = true;
				tx_addr = (uint32_t)(rstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.rbuffer));

				AUDIO_SP_TXGDMA_Init(rstream->stream.sport_dev_num, GDMA_INT, sp_txgdma_initstruct, rstream->stream.gdma_struct,
									 (IRQ_FUN)ameba_audio_stream_tx_complete, (u8 *)tx_addr, dma_len);
				rstream->stream.gdma_cnt++;
				HAL_AUDIO_INFO("gdma init: index:%d, chNum:%d, tx_addr:0x%lx, dma_len:%lu at:%llu ns",
							   sp_txgdma_initstruct->GDMA_Index, sp_txgdma_initstruct->GDMA_ChNum, tx_addr, dma_len, OsalGetSysTimeNs(OSAL_TIME_MONOTONIC));

				if (has_extra_dma) {
					extra_tx_addr = (uint32_t)(rstream->stream.extra_rbuffer->raw_data + ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.extra_rbuffer));
					AUDIO_SP_TXGDMA_Init(rstream->stream.sport_dev_num, GDMA_EXT, extra_sp_txgdma_initstruct, rstream->stream.extra_gdma_struct,
										 (IRQ_FUN)ameba_audio_stream_tx_complete, (u8 *)extra_tx_addr, extra_dma_len);
					rstream->stream.extra_gdma_cnt++;
					HAL_AUDIO_INFO("gdma extra init: index:%d, chNum:%d, tx_addr:0x%lx, extra_dma_len:%lu",
								   extra_sp_txgdma_initstruct->GDMA_Index, extra_sp_txgdma_initstruct->GDMA_ChNum, extra_tx_addr, extra_dma_len);
				}

				ameba_audio_stream_tx_start(stream, STATE_STARTED);

#if HAL_AUDIO_PLAYBACK_DUMP_DEBUG
				ameba_audio_dump_gdma_regs(sp_txgdma_initstruct->GDMA_ChNum);
				ameba_audio_dump_sport_regs(SPORT0_REG_BASE);
				ameba_audio_dump_codec_regs();
#endif
			}
		}

		if (rstream->stream.state == STATE_XRUN_NOTIFIED || rstream->stream.state == STATE_STANDBY) {
			if (ameba_audio_stream_buffer_get_remain_size(rstream->stream.rbuffer) >= dma_len) {
				tx_addr = (uint32_t)(rstream->stream.rbuffer->raw_data + ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.rbuffer));
				HAL_AUDIO_VERBOSE("restart gdma at rp:%u", ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.rbuffer));
				AUDIO_SP_TXGDMA_Restart(sp_txgdma_initstruct->GDMA_Index, sp_txgdma_initstruct->GDMA_ChNum, tx_addr, dma_len);
				rstream->stream.gdma_cnt++;

				if (has_extra_dma) {
					extra_tx_addr = (uint32_t)(rstream->stream.extra_rbuffer->raw_data + ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.extra_rbuffer));
					HAL_AUDIO_VERBOSE("restart extra gdma at rp:%u", ameba_audio_stream_buffer_get_tx_readptr(rstream->stream.extra_rbuffer));
					AUDIO_SP_TXGDMA_Restart(extra_sp_txgdma_initstruct->GDMA_Index, extra_sp_txgdma_initstruct->GDMA_ChNum, extra_tx_addr, extra_dma_len);
					rstream->stream.extra_gdma_cnt++;
				}
				ameba_audio_stream_tx_start(stream, STATE_STARTED);
			}
		}

		bytes_left_to_write -= bytes_written;
		if (ameba_audio_stream_buffer_get_available_size(rstream->stream.rbuffer) < bytes_left_to_write) {
			rstream->stream.sem_need_post = true;
			rtw_down_sema(&rstream->stream.sem);
		}
		rstream->stream.sem_need_post = false;

		if (has_extra_dma) {
			extra_bytes_left_to_write -= extra_bytes_written;
			if (ameba_audio_stream_buffer_get_available_size(rstream->stream.extra_rbuffer) < extra_bytes_left_to_write) {
				rstream->stream.extra_sem_need_post = true;
				rtw_down_sema(&rstream->stream.extra_sem);
			}
			rstream->stream.extra_sem_need_post = false;
		}

	}

	if (has_extra_dma) {
		if (p_buf) {
			free(p_buf);
			p_buf = NULL;
		}

		if (p_extra_buf) {
			free(p_extra_buf);
			p_extra_buf = NULL;
		}
	}

	rstream->total_written_from_tx_start += bytes / rstream->stream.config.frame_size;
	return bytes;
}

uint64_t ameba_audio_stream_tx_get_frames_written(Stream *stream)
{
	RenderStream *rstream = (RenderStream *)stream;
	return rstream->total_written_from_tx_start;
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

		rstream->stream.trigger_tstamp = ameba_audio_get_now_ns();

		AUDIO_SP_DmaCmd(rstream->stream.sport_dev_num, DISABLE);
		AUDIO_SP_TXStart(rstream->stream.sport_dev_num, DISABLE);

		AUDIO_SP_Deinit(rstream->stream.sport_dev_num, SP_DIR_TX);
		//AUDIO_CODEC_DeInit(APP_LINE_OUT);

		ameba_audio_reset_audio_ip_status((Stream *)rstream);

		rtw_free_sema(&rstream->stream.sem);
		rtw_free_sema(&rstream->stream.sem_gdma_end);
		rtw_free_sema(&rstream->stream.extra_sem_gdma_end);

		ameba_audio_stream_buffer_release(rstream->stream.rbuffer);
		if (rstream->stream.gdma_struct) {
			free(rstream->stream.gdma_struct);
			rstream->stream.gdma_struct = NULL;
		}

		if (rstream->stream.gdma_ch_lli) {
			ameba_audio_gdma_free(rstream->stream.gdma_ch_lli);
			rstream->stream.gdma_ch_lli = NULL;
		}

		rstream->stream.state = STATE_DEINITED;
		free(rstream);
	}
}
