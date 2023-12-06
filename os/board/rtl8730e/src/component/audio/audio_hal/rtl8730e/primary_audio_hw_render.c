/*
 * Copyright (c) 2021 Realtek, LLC.
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
#include "audio_hw_compat.h"
#include <inttypes.h>

#include "ameba_audio_hw_usrcfg.h"
#include "ameba_audio_types.h"
#include "ameba_audio_stream_control.h"
#include "ameba_audio_stream_render.h"
#include "audio_hw_osal_errnos.h"
#include "audio_hw_debug.h"
#include "hardware/audio/audio_hw_types.h"
#include "hardware/audio/audio_hw_utils.h"
#include "hardware/audio/audio_hw_render.h"
#include "osdep_service.h"
#include "audio_hw_params_handle.h"
#include "primary_audio_hw_adapter.h"

#define DEFAULT_OUT_SAMPLING_RATE 16000
#define NOIRQ_SHORT_PERIOD_SIZE   384
#define SHORT_PERIOD_SIZE         1024
#define SHORT_PERIOD_COUNT        4
#define AMPLIFIER_EN_PIN          "amp_pin"

#define DUMP_FRAME            192000
#define DUMP_ENABLE           0

StreamConfig stream_output_config = {
	.channels = 2,
	.rate = DEFAULT_OUT_SAMPLING_RATE,
	.format = AUDIO_HW_FORMAT_PCM_16_BIT,
	.period_size = SHORT_PERIOD_SIZE,
	.period_count = SHORT_PERIOD_COUNT,
};

struct PrimaryAudioHwRender {
	struct AudioHwRender stream;

	_mutex lock;
	StreamConfig config;
	Stream *out_pcm;
	char *buffer;
	size_t buffer_frames;
	int standby;
	AudioHwFormat format;
	uint32_t channel_count;
	uint32_t sample_rate;
	uint32_t period_size;
	struct PrimaryAudioHwAdapter *adap;
	struct AudioHwDeviceDescriptor desc;
	uint32_t amp_pin;

	//max value should sync with ameba audio driver's total_counter_boundary.
	uint64_t written;
};

static inline size_t PrimaryAudioHwRenderFrameSize(const struct AudioHwRender *s)
{
	int chan_samp_sz;
	AudioHwFormat format = s->common.GetFormat(&s->common);

	if (AudioIsLinearPCM(format)) {
		chan_samp_sz = GetAudioBytesPerSample(format);
		return s->common.GetChannels(&s->common) * chan_samp_sz;
	}

	return sizeof(int8_t);
}

static uint32_t PrimaryGetRenderSampleRate(const struct AudioHwStream *stream)
{
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;
	return out->sample_rate;
}

static int PrimarySetRenderSampleRate(struct AudioHwStream *stream, uint32_t rate)
{
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;
	out->sample_rate = rate;
	return 0;
}

static size_t PrimaryGetRenderBufferSize(const struct AudioHwStream *stream)
{
	size_t ret = 0;
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;
	size_t size = (out->period_size * out->sample_rate) / out->sample_rate;
	size = ((size + 15) / 16) * 16;
	ret = size * PrimaryAudioHwRenderFrameSize((const struct AudioHwRender *)stream);
	return ret;
}

static uint32_t PrimaryGetRenderChannels(const struct AudioHwStream *stream)
{
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;

	return out->channel_count;
}

static int PrimarySetRenderChannels(const struct AudioHwStream *stream, uint32_t channel)
{
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;

	out->channel_count = channel;
	return 0;
}

static AudioHwFormat PrimaryGetRenderFormat(const struct AudioHwStream *stream)
{
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;

	return out->format;
}

static int PrimarySetRenderFormat(struct AudioHwStream *stream, AudioHwFormat format)
{
	(void) stream;
	(void) format;
	return 0;
}

/* must be called with hw device and output stream mutexes locked */
static int DoStandbyOutput(struct PrimaryAudioHwRender *out)
{
	if (!out->standby) {
		out->standby = 1;
		if (AUDIO_HW_AMPLIFIER_MUTE_ENABLE) {
			ameba_audio_stream_tx_set_amp_state(false);
		}

		ameba_audio_stream_tx_standby(out->out_pcm);
		ameba_audio_stream_buffer_flush(out->out_pcm->rbuffer);
	}
	return 0;
}

static int PrimaryStandbyRender(struct AudioHwStream *stream)
{
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;
	int status;

	//rtw_mutex_get(&out->adap->lock);
	rtw_mutex_get(&out->lock);
	status = DoStandbyOutput(out);
	rtw_mutex_put(&out->lock);
	//rtw_mutex_put(&out->adap->lock);

	return status;
}

static int PrimaryDumpRender(const struct AudioHwStream *stream, int fd)
{
	(void) stream;
	(void) fd;
	return 0;
}

static uint32_t PrimaryGetRenderBufferStatus(struct AudioHwStream *stream)
{
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;
	if (!out) {
		HAL_AUDIO_ERROR("PrimaryGetRenderBufferStatus render is not initialized");
		return 0;
	}


	return ameba_audio_stream_tx_get_buffer_status(out->out_pcm);
}

static int PrimarySetRenderParameters(struct AudioHwStream *stream, const char *str_pairs)
{
	HAL_AUDIO_INFO("%s, keys = %s", __FUNCTION__, str_pairs);
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;
	struct string_cell *cells;
	cells = string_cells_create_from_str(str_pairs);
	int value;

	if (string_cells_has_key(cells, AMPLIFIER_EN_PIN)) {
		string_cells_get_int(cells, AMPLIFIER_EN_PIN, &value);
		out->amp_pin = value;
		ameba_audio_ctl_set_amp_pin(ameba_audio_get_ctl(), out->amp_pin);
	}

	string_cells_destroy(cells);
	return 0;
}

static char *PrimaryGetRenderParameters(const struct AudioHwStream *stream, const char *keys)
{
	(void) stream;
	(void) keys;
	return (char *)strdup("");
}

static uint32_t PrimaryGetRenderLatency(const struct AudioHwRender *stream)
{
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;
	uint64_t sport_out_frames = ameba_audio_stream_tx_sport_rendered_frames(out->out_pcm);
	uint32_t latency_ms = (ameba_audio_stream_tx_get_frames_written(out->out_pcm) - sport_out_frames) * 1000 / out->config.rate;
	HAL_AUDIO_VERBOSE("hal written_to_driver:%llu, sport_out_frames:%llu, latency_ms:%lu", ameba_audio_stream_tx_get_frames_written(out->out_pcm), sport_out_frames,
					  latency_ms);

	uint32_t dma_buf_add_codec_latency_ms = (out->config.period_size * out->config.period_count + 36) * 1000 / out->config.rate;
	if (latency_ms > dma_buf_add_codec_latency_ms * 2) {
		return dma_buf_add_codec_latency_ms;
	} else {
		return latency_ms;
	}
}

static int PrimaryGetPresentationPosition(const struct AudioHwRender *stream, uint64_t *frames, struct timespec *timestamp)
{
	HAL_AUDIO_VERBOSE("primaryGetPresentationPosition latency:%lu", PrimaryGetRenderLatency(stream));

	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;
	int ret = -1;

	rtw_mutex_get(&out->lock);

	if (out->out_pcm) {
		uint64_t rendered_frames;
		if (ameba_audio_stream_tx_get_position(out->out_pcm, &rendered_frames, timestamp) == 0) {
			int64_t signed_frames = out->written - (ameba_audio_stream_tx_get_frames_written(out->out_pcm) - rendered_frames);
			HAL_AUDIO_VERBOSE("out->written:%llu, rendered_frames:%llu, written to driver:%llu, signed_frames:%lld, sec:%lld, nsec:%ld", out->written, rendered_frames,
							  ameba_audio_stream_tx_get_frames_written(out->out_pcm),
							  signed_frames, timestamp->tv_sec, timestamp->tv_nsec);
			if (signed_frames >= 0) {
				*frames = signed_frames;
				HAL_AUDIO_VERBOSE("frames:%llu", *frames);
				rtw_mutex_put(&out->lock);
				return 0;
			}
		} else {
			HAL_AUDIO_ERROR("get ts fail");
		}
	} else {
		HAL_AUDIO_ERROR("%s no out_pcm", __func__);
	}

	rtw_mutex_put(&out->lock);

	return ret;
}

static int PrimaryGetPresentTime(const struct AudioHwRender *stream, int64_t *now_ns, int64_t *audio_ns)
{
	HAL_AUDIO_VERBOSE("primaryGetPresentationPosition latency:%lu", PrimaryGetRenderLatency(stream));

	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;
	int ret = -1;
	int64_t tmp_now_ns = 0;
	int64_t tmp_audio_ns = 0;

	rtw_mutex_get(&out->lock);

	if (out->out_pcm) {
		int64_t written_to_driver_frames = ameba_audio_stream_tx_get_frames_written(out->out_pcm);
		ret = ameba_audio_stream_tx_get_time(out->out_pcm, &tmp_now_ns, &tmp_audio_ns);
		int64_t out_written_to_hal_ns = (int64_t)(((double)out->written / (double)PrimaryGetRenderSampleRate((const struct AudioHwStream *)stream) *
										(double)1000000000));
		int64_t written_to_driver_ns = (int64_t)(((double)written_to_driver_frames / (double)PrimaryGetRenderSampleRate((const struct AudioHwStream *)stream) *
									   (double)1000000000));
		*audio_ns = out_written_to_hal_ns - (written_to_driver_ns - tmp_audio_ns);
		*now_ns = tmp_now_ns;
	} else {
		HAL_AUDIO_ERROR("%s no out_pcm", __func__);
	}

	rtw_mutex_put(&out->lock);

	return ret;
}

static int PrimarySetRenderVolume(struct AudioHwRender *stream, float left,
								  float right)
{
	HAL_AUDIO_VERBOSE("PrimarySetRenderVolume enter, left: %f, right: %f", left, right);
	(void) stream;
	ameba_audio_ctl_set_tx_volume(ameba_audio_get_ctl(), left, right);
	return 0;
}

/* must be called with hw device and output stream mutexes locked */
static int StartAudioHwRender(struct PrimaryAudioHwRender *out)
{
	HAL_AUDIO_VERBOSE("start output stream enter");
	(void) out;
	//ameba_audio_ctl_set_tx_mute(ameba_audio_get_ctl(), false);
	if (AUDIO_HW_AMPLIFIER_MUTE_ENABLE) {
		ameba_audio_stream_tx_set_amp_state(true);
	}
	return HAL_OSAL_OK;
}

static ssize_t PrimaryRenderWrite(struct AudioHwRender *stream, const void *buffer,
								  size_t bytes, bool block)
{
	HAL_AUDIO_PVERBOSE("primaryRenderWrite: bytes: %u", bytes);

	int ret = 0;
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)stream;
	//struct PrimaryAudioHwAdapter *adap = out->adap;

	size_t frame_size = PrimaryAudioHwRenderFrameSize((const struct AudioHwRender *)stream);

	//rtw_mutex_get(&adap->lock);
	rtw_mutex_get(&out->lock);
	if (out->standby) {
		ret = StartAudioHwRender(out);
		if (ret != 0) {
			HAL_AUDIO_ERROR("start render fail");
			//rtw_mutex_put(&adap->lock);
			goto exit;
		}
		out->standby = 0;
	}
	//rtw_mutex_put(&adap->lock);

	/* Write to all active PCMs */
	if (out->out_pcm) {
		ret = ameba_audio_stream_tx_write(out->out_pcm, (void *)buffer, bytes, block);
	} else {
		HAL_AUDIO_ERROR("out pcm is NULL!!!");
	}

	if (DUMP_ENABLE) {
		if (out->written * frame_size + bytes <= DUMP_FRAME * frame_size) {
			memcpy(out->buffer + out->written * frame_size, buffer, bytes);
			DCache_Invalidate((u32)(out->buffer + out->written * frame_size), bytes);
		}
	}

	//write successfully
	if (ret >= 0) {
		out->written += ret / frame_size;
		//sync with ameba audio driver's total_counter_boundary max value.
		if (out->written > UINT64_MAX) {
			out->written = 0;
		}
	}

exit:
	rtw_mutex_put(&out->lock);

	return ret;
}

void CloseAudioHwRender(struct AudioHwRender *render)
{
	struct PrimaryAudioHwRender *out = (struct PrimaryAudioHwRender *)render;

	PrimaryStandbyRender(&render->common);

	if (out->out_pcm) {
		ameba_audio_stream_tx_close(out->out_pcm);
		out->out_pcm = NULL;
	}

	if (DUMP_ENABLE) {
		rtw_free(out->buffer);
	}

	rtw_mutex_free(&out->lock);
	rtw_free(render);
	//render = NULL;
}

struct AudioHwRender *GetAudioHwRenderFuncs(struct AudioHwAdapter *adapter, const struct AudioHwDeviceDescriptor *desc,
		const struct AudioHwSampleAttributes *attrs, uint32_t flags)
{
	struct PrimaryAudioHwAdapter *adap = (struct PrimaryAudioHwAdapter *)adapter;
	struct PrimaryAudioHwRender *out;

	out = (struct PrimaryAudioHwRender *)rtw_zmalloc(sizeof(struct PrimaryAudioHwRender));
	if (!out) {
		return NULL;
	}

	out->adap = adap;
	out->desc = *desc;

	out->stream.common.GetSampleRate = PrimaryGetRenderSampleRate;
	out->stream.common.SetSampleRate = PrimarySetRenderSampleRate;
	out->stream.common.GetBufferSize = PrimaryGetRenderBufferSize;
	out->stream.common.GetChannels = PrimaryGetRenderChannels;
	out->stream.common.SetChannels = PrimarySetRenderChannels;
	out->stream.common.GetFormat = PrimaryGetRenderFormat;
	out->stream.common.SetFormat = PrimarySetRenderFormat;
	out->stream.common.Standby = PrimaryStandbyRender;
	out->stream.common.Dump = PrimaryDumpRender;
	out->stream.common.SetParameters = PrimarySetRenderParameters;
	out->stream.common.GetParameters = PrimaryGetRenderParameters;
	out->stream.common.GetBufferStatus = PrimaryGetRenderBufferStatus;
	out->stream.GetPresentationPosition = PrimaryGetPresentationPosition;
	out->stream.GetPresentTime = PrimaryGetPresentTime;
	out->stream.GetLatency = PrimaryGetRenderLatency;
	out->stream.SetVolume = PrimarySetRenderVolume;
	out->stream.Write = PrimaryRenderWrite;

	out->format = AUDIO_HW_FORMAT_PCM_16_BIT;
	out->channel_count = 2;
	out->sample_rate = DEFAULT_OUT_SAMPLING_RATE;
	out->period_size = SHORT_PERIOD_SIZE;
	out->adap = adap;
	out->standby = 1;
	out->written = 0;
	out->amp_pin = -1;

	AudioHwFormat format = out->stream.common.GetFormat(&out->stream.common);
	uint32_t channel_count =  out->stream.common.GetChannels(&out->stream.common);
	uint32_t sample = out->stream.common.GetSampleRate(&out->stream.common);

	if (format != attrs->format) {
		out->format =  attrs->format;
	}
	if (channel_count != attrs->channel_count) {
		out->channel_count = attrs->channel_count;
	}
	if (sample != attrs->sample_rate) {
		out->sample_rate = attrs->sample_rate;
	}

	rtw_mutex_init(&out->lock);

	adap->output = out;

	out->config = stream_output_config;

	out->config.rate = out->sample_rate; // update sample_rate according to top level player
	out->config.format = out->format;
	out->config.channels = out->channel_count;
	out->config.frame_size = PrimaryAudioHwRenderFrameSize(&out->stream);

	if (flags & AUDIO_HW_OUTPUT_FLAG_NOIRQ) {
		HAL_AUDIO_INFO("startAudioHwRender in noirq mode");
		out->config.mode = AUDIO_HW_DMA_NOIRQ_MODE;
		if (attrs->buffer_bytes) {
			out->period_size = attrs->buffer_bytes / out->config.frame_size;
		} else {
			out->period_size = NOIRQ_SHORT_PERIOD_SIZE;
		}
	} else {
		out->config.mode = AUDIO_HW_DMA_IRQ_MODE;
		if (attrs->buffer_bytes) {
			out->period_size = attrs->buffer_bytes / out->config.frame_size;
		} else {
			out->period_size = SHORT_PERIOD_SIZE;
		}
	}

	out->config.period_size = out->period_size;

	/*stream_tx_init can only be set here, because if it's in first write, then set parameters will stuck.Because setparameters should be called before write*/
	if (out->out_pcm == NULL) {
		HAL_AUDIO_INFO("startAudioHwRender samplerate:%" PRIu32 ", format:%" PRIu32 ", channel:%" PRIu32 ", framesize:%" PRIu32 ", period_size:%" PRIu32 "",
					   out->config.rate, out->config.format,
					   out->config.channels,
					   out->config.frame_size, out->config.period_size);
		out->out_pcm = ameba_audio_stream_tx_init(AMEBA_AUDIO_DEVICE_SPEAKER, out->config);
	} else {
		HAL_AUDIO_DEBUG("out pcm has been opened");
	}

	if (DUMP_ENABLE) {
		out->buffer = (char *)rtw_zmalloc(DUMP_FRAME * out->config.frame_size);
		HAL_AUDIO_INFO("dump buffer:%p", out->buffer);
	}

	return &out->stream;
}
