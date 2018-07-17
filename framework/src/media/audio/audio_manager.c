/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>
#include <tinyara/audio/audio.h>
#include <tinyalsa/tinyalsa.h>

#include "audio_manager.h"
#include "resample/samplerate.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MAX_AUDIO_DEVICE_NUM 3
#define AUDIO_STREAM_CHANNEL_MONO 1
#define AUDIO_STREAM_CHANNEL_STEREO 2
// ToDO: Expand with policies, Large data requires a large period.
#define AUDIO_STREAM_VOIP_PERIOD_SIZE 2048
#define AUDIO_STREAM_VOIP_PERIOD_COUNT 8
#define AUDIO_STREAM_VOIP_SAMPLE_RATE 16000

#define AUDIO_STREAM_RETRY_COUNT 2

#define AUDIO_DEVICE_UNDEF 0x0000
#define AUDIO_DEVICE_IN_MICROPHONE 0x0001
#define AUDIO_DEVICE_OUT_EARPIECE 0x0100
#define AUDIO_DEVICE_OUT_SPEAKER 0x0200
#define AUDIO_DEVICE_OUT_HEADSET 0x0400

#define AUDIO_DEVICE_MAX_VOLUME 10

#ifndef CONFIG_AUDIO_MAX_INPUT_CARD_NUM
#define CONFIG_AUDIO_MAX_INPUT_CARD_NUM 2
#endif

#ifndef CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM
#define CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM 2
#endif

#ifndef CONFIG_AUDIO_RESAMPLER_BUFSIZE
#define CONFIG_AUDIO_RESAMPLER_BUFSIZE 4096
#endif

#define INVALID_ID -1

/****************************************************************************
 * Private Types
 ****************************************************************************/
enum audio_card_status_e {
	AUDIO_CARD_NONE = 0,		// A card is not found yet.
	AUDIO_CARD_IDLE = 1,
	AUDIO_CARD_READY,
	AUDIO_CARD_RUNNING,
	AUDIO_CARD_PAUSE
};

enum manufacturer_type_e {
	AUDIO_CARD_REALTEK = 0
};

enum audio_card_type_e {
	INPUT = 0,
	OUTPUT
};

enum audio_card_operation_e {
	ENABLE = 0,
	FIND
};

struct audio_config_s {
	uint8_t volume;
	uint16_t max_volume;
	char *manufacturer_name;
	enum manufacturer_type_e manufacturer_type;
	int audio_card_type;
};

struct audio_resample_s {
	bool necessary;
	uint8_t samprate_types;
	unsigned int from;
	unsigned int to;
	void *buffer;
	unsigned int buffer_size;
	float ratio;
	src_handle_t handle;
};

struct audio_card_info_s {
	uint8_t card_id;
	char card_path[256];
	enum audio_card_status_e status;
	uint8_t device_id;
	struct audio_config_s config;
	struct pcm *pcm;
	struct audio_resample_s resample;
	pthread_mutex_t card_mutex;
};

struct audio_samprate_map_entry_s {
	uint8_t samprate_types;
	unsigned int samprate;
};

typedef enum audio_card_status_e audio_card_status_t;
typedef enum manufacturer_type_e manufacturer_t;
typedef enum audio_card_type_e audio_card_type_t;
typedef enum audio_card_operation_e audio_card_operation_t;
typedef struct audio_config_s audio_config_t;
typedef struct audio_card_info_s audio_card_info_t;

static int g_num_found_in_cards = 0;
static int g_num_found_out_cards = 0;

static const struct audio_samprate_map_entry_s g_audio_samprate_entry[] = {
	{AUDIO_SAMP_RATE_TYPE_8K, AUDIO_SAMP_RATE_8K},
	{AUDIO_SAMP_RATE_TYPE_11K, AUDIO_SAMP_RATE_11K},
	{AUDIO_SAMP_RATE_TYPE_16K, AUDIO_SAMP_RATE_16K},
	{AUDIO_SAMP_RATE_TYPE_22K, AUDIO_SAMP_RATE_22K},
	{AUDIO_SAMP_RATE_TYPE_32K, AUDIO_SAMP_RATE_32K},
	{AUDIO_SAMP_RATE_TYPE_44K, AUDIO_SAMP_RATE_44K},
	{AUDIO_SAMP_RATE_TYPE_48K, AUDIO_SAMP_RATE_48K},
	{AUDIO_SAMP_RATE_TYPE_96K, AUDIO_SAMP_RATE_96K}
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static audio_manager_result_t get_audio_card_in(audio_card_operation_t card_operation, int card_id, audio_handler_t **handler);
static audio_manager_result_t get_audio_card_out(audio_card_operation_t card_operation, int card_id, audio_handler_t **handler);
static audio_manager_result_t find_audio_card(audio_card_type_t card_type);
static void get_hardware_params(audio_handler_t *handler, audio_card_type_t card_type);
static audio_manager_result_t get_supported_sample_rate(int fd, uint8_t *sample_type, audio_card_type_t type);
static uint32_t get_closest_samprate(audio_handler_t *handler, unsigned origin_samprate);
static unsigned int resample_stream_in(audio_handler_t *handler, void *data, unsigned int frames);
static unsigned int resample_stream_out(audio_handler_t *handler, void *data, unsigned int frames);
static audio_manager_result_t get_audio_volume(int fd, audio_config_t *config, audio_card_type_t card_type);
static audio_manager_result_t set_audio_volume(audio_handler_t *handler, audio_card_type_t type, uint8_t volume);

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static audio_manager_result_t get_audio_card_in(audio_card_operation_t card_operation, int card_id, audio_handler_t **handler)
{
	int i = 0;
	static audio_card_info_t in_audio_card_info[CONFIG_AUDIO_MAX_INPUT_CARD_NUM];

	switch (card_operation) {
	case ENABLE:
		ASSERT(card_id >= 0 && card_id <= CONFIG_AUDIO_MAX_INPUT_CARD_NUM);
		*handler = &in_audio_card_info[card_id];
		return AUDIO_MANAGER_SUCCESS;
		break;
	case FIND:
		// Find an available card if card_id is not specified.
		for (i = 0; i < g_num_found_in_cards; i++) {
			if ((in_audio_card_info[i].status == AUDIO_CARD_IDLE)
				|| (in_audio_card_info[i].status == AUDIO_CARD_READY)
				|| (in_audio_card_info[i].status == AUDIO_CARD_PAUSE)) {
				*handler = &in_audio_card_info[i];
				return AUDIO_MANAGER_SUCCESS;
			}
		}
		break;
	}

	return AUDIO_MANAGER_CARD_NOT_READY;
}

static audio_manager_result_t get_audio_card_out(audio_card_operation_t card_operation, int card_id, audio_handler_t **handler)
{
	int i = 0;
	static audio_card_info_t out_audio_card_info[CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM];

	switch (card_operation) {
	case (ENABLE):
		ASSERT(card_id >= 0 && card_id <= CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM);
		*handler = &out_audio_card_info[card_id];
		return AUDIO_MANAGER_SUCCESS;
		break;
	case (FIND):
		// Find an available card if card_id is not specified.
		for (i = 0; i < g_num_found_out_cards; i++) {
			if ((out_audio_card_info[i].status == AUDIO_CARD_IDLE)
				|| (out_audio_card_info[i].status == AUDIO_CARD_READY)
				|| (out_audio_card_info[i].status == AUDIO_CARD_PAUSE)) {
				*handler = &out_audio_card_info[i];
				return AUDIO_MANAGER_SUCCESS;
			}
		}
		break;
	}

	return AUDIO_MANAGER_CARD_NOT_READY;
}

static audio_manager_result_t find_audio_card(audio_card_type_t card_type)
{
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	int card_id = INVALID_ID;
	int device_id = INVALID_ID;
	char type;
	struct dirent *dir_entry;
	DIR *dir_info;
	audio_card_info_t *card_ptr;
	int max_card_num = CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM;
	char type_chr = 'p';

	if (!(dir_info = opendir("/dev/audio/"))) {
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	if (card_type == INPUT) {
		max_card_num = CONFIG_AUDIO_MAX_INPUT_CARD_NUM;
		type_chr = 'c';
	}

	while ((dir_entry = readdir(dir_info)) != NULL) {
		// TODO: Add cases for various drivers. Currently, identify 'pcm' drivers only.
		if ((dir_entry->d_name[0] != 'p') || (dir_entry->d_name[1] != 'c') || (dir_entry->d_name[2] != 'm') || (sscanf(&dir_entry->d_name[3], "C%uD%u%c", &card_id, &device_id, &type) != 3)) {
			continue;
		}

		/* Find next card if driver name isn't follow naming policy */
		if ((card_id < 0) || (card_id >= max_card_num) || (device_id < 0) || (device_id >= MAX_AUDIO_DEVICE_NUM)) {
			meddbg("Found a wrong card_id : %d device_id : %d\n", card_id, device_id);
			continue;
		}

		medvdbg("Card id = %d, path = %s\n", card_id, dir_entry->d_name);

		if (type == type_chr) {
			if (card_type == INPUT) {
				if ((ret = get_audio_card_in(ENABLE, card_id, &card_ptr)) != AUDIO_MANAGER_SUCCESS) {
					meddbg("Input audio card %d is unavailable", card_id);
					return AUDIO_MANAGER_CARD_NOT_READY;
				}

				if (card_ptr->status == AUDIO_CARD_NONE) {
					pthread_mutex_init(&card_ptr->card_mutex, NULL);
				}

				pthread_mutex_lock(&card_ptr->card_mutex);
			} else {
				if ((ret = get_audio_card_out(ENABLE, card_id, &card_ptr)) != AUDIO_MANAGER_SUCCESS) {
					meddbg("Output audio card %d is unavailable", card_id);
					return AUDIO_MANAGER_CARD_NOT_READY;
				}

				if (card_ptr->status == AUDIO_CARD_NONE) {
					pthread_mutex_init(&card_ptr->card_mutex, NULL);
				}

				pthread_mutex_lock(&card_ptr->card_mutex);
			}

			card_ptr->card_id = card_id;
			card_ptr->status = AUDIO_CARD_IDLE;
			card_ptr->device_id = device_id;
			snprintf(card_ptr->card_path, strlen("/dev/audio/") + strlen(dir_entry->d_name) + 1, "%s%s", "/dev/audio/", dir_entry->d_name);

			get_hardware_params(card_ptr, card_type);

			if (card_type == INPUT) {
				g_num_found_in_cards++;
				medvdbg("Found an audio card, total card : %d id : %d, card_path : %s\n", g_num_found_in_cards, card_id, card_ptr->card_path);

				pthread_mutex_unlock(&card_ptr->card_mutex);
			} else {
				g_num_found_out_cards++;
				medvdbg("Found an audio card, total card : %d id : %d, card_path : %s\n", g_num_found_out_cards, card_id, card_ptr->card_path);

				pthread_mutex_unlock(&card_ptr->card_mutex);
			}
		}
	}

	/* If nothing founded */
	if (((card_type == INPUT) && (g_num_found_in_cards == 0)) || ((card_type == OUTPUT) && (g_num_found_out_cards == 0))) {
		meddbg("Failed to find card\n");
		ret = AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	closedir(dir_info);
	return ret;
}

static void get_hardware_params(audio_handler_t *handler, audio_card_type_t card_type)
{
	int fd;
	int ret;
	audio_config_t *config;

	fd = open(handler->card_path, O_RDONLY);
	if (fd < 0) {
		meddbg("Failed to open audio driver, path : %s errno : %d\n", handler->card_path, errno);
		return;
	}

	config = &handler->config;
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	/* get volume of current card first */
	ret = get_audio_volume(fd, config, card_type);

	/* If it is failed, it doesn't return because we should get next h/w params */
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Get volume failed card path : %s card_type : %d ret : %d\n", handler->card_path, card_type, ret);
	}

	medvdbg("Max volume: %d, Current Volume : %d card path : %s\n", handler->config.max_volume, handler->config.volume, handler->card_path);
#endif
	/* get supported sample rate type */
	ret = get_supported_sample_rate(fd, &handler->resample.samprate_types, card_type);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Get supported sample rate failed card path : %s card_type : %d ret : %d\n", handler->card_path, card_type, ret);
	}
	medvdbg("card path : %s Supported sample rate: %x\n", handler->card_path, handler->resample.samprate_types);
	close(fd);
}

static audio_manager_result_t get_supported_sample_rate(int fd, uint8_t *sample_type, audio_card_type_t card_type)
{
	struct audio_caps_desc_s caps_desc;

	caps_desc.caps.ac_len = sizeof(struct audio_caps_s);
	caps_desc.caps.ac_type = AUDIO_TYPE_OUTPUT;
	if (card_type == INPUT) {
		caps_desc.caps.ac_type = AUDIO_TYPE_INPUT;
	}
	caps_desc.caps.ac_subtype = AUDIO_TYPE_QUERY;

	if (ioctl(fd, AUDIOIOC_GETCAPS, (unsigned long *)&caps_desc.caps) < 0) {
		meddbg("Fail to ioctl AUDIOIOC_GETCAPS\n");
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	*sample_type = caps_desc.caps.ac_controls.b[0];

	return AUDIO_MANAGER_SUCCESS;
}

static uint32_t get_closest_samprate(audio_handler_t *handler, unsigned int origin_samprate)
{
	int i;
	uint32_t result;
	int count = sizeof(g_audio_samprate_entry) / sizeof(struct audio_samprate_map_entry_s);
	ASSERT(count > 0);

	result = g_audio_samprate_entry[count - 1].samprate;
	for (i = count - 1; i >= 0; i--) {
		if (g_audio_samprate_entry[i].samprate_types & handler->resample.samprate_types) {
			if (g_audio_samprate_entry[i].samprate >= origin_samprate) {
				result = g_audio_samprate_entry[i].samprate;
			}
		}
	}

	return result;
}

static unsigned int resample_stream_in(audio_handler_t *handler, void *data, unsigned int frames)
{
	unsigned int used_frames = 0;
	unsigned int resampled_frames = 0;
	src_data_t srcData = { 0, };

	srcData.channels_num = 2;
	srcData.origin_sample_rate = handler->resample.from;
	srcData.origin_sample_width = SAMPLE_WIDTH_16BITS;
	srcData.desired_sample_rate = handler->resample.to;
	srcData.desired_sample_width = SAMPLE_WIDTH_16BITS;
	srcData.input_frames_used = 0;
	srcData.data_out = data;
	srcData.out_buf_length = handler->resample.buffer_size;

	while (frames > used_frames) {
		srcData.data_in = handler->resample.buffer + get_input_frames_to_byte(handler, used_frames);
		srcData.input_frames = frames - used_frames;
		srcData.data_out = data + get_input_frames_to_byte(handler, resampled_frames);
		medvdbg("data_in addr = 0x%x + %d\t", srcData.data_in, get_input_frames_to_byte(handler, used_frames));
		if (src_simple(handler->resample.handle, &srcData) != SRC_ERR_NO_ERROR) {
			meddbg("Fail to resample in:%d/%d, out:%d, to %u from %u\n", used_frames, frames, srcData.desired_sample_rate, srcData.origin_sample_rate);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}

		if (srcData.output_frames_gen > 0) {
			resampled_frames += srcData.output_frames_gen;
			used_frames += srcData.input_frames_used;
			medvdbg("Record resampled in:%d/%d, out:%d\n", used_frames, frames, resampled_frames);
		} else {
			meddbg("Wrong output_frames_gen : %d\n", srcData.output_frames_gen);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}
	}

	return resampled_frames;
}

static unsigned int resample_stream_out(audio_handler_t *handler, void *data, unsigned int frames)
{
	unsigned int used_frames = 0;
	unsigned int resampled_frames = 0;
	src_data_t srcData = { 0, };

	srcData.channels_num = 2;
	srcData.origin_sample_rate = handler->resample.from;
	srcData.origin_sample_width = SAMPLE_WIDTH_16BITS;
	srcData.desired_sample_rate = handler->resample.to;
	srcData.desired_sample_width = SAMPLE_WIDTH_16BITS;
	srcData.input_frames_used = 0;
	srcData.data_out = handler->resample.buffer;
	srcData.out_buf_length = handler->resample.buffer_size;
	medvdbg("resampler buffer_size = %d, buffer_addr = 0x%x\n", handler->resample.buffer_size, handler->resample.buffer);

	while (frames > used_frames) {
		srcData.data_in = data + get_output_frames_to_byte(handler, used_frames);
		srcData.input_frames = frames - used_frames;
		srcData.data_out = handler->resample.buffer + get_output_frames_to_byte(handler, resampled_frames);
		medvdbg("buffer addr = 0x%x   data_out addr = 0x%x   ", handler->resample.buffer, srcData.data_out);
		if (src_simple(handler->resample.handle, &srcData) != SRC_ERR_NO_ERROR) {
			meddbg("Fail to resample in:%d/%d, out:%d, to %u from %u\n", used_frames, frames, srcData.desired_sample_rate, srcData.origin_sample_rate);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}

		if (srcData.output_frames_gen > 0) {
			resampled_frames += srcData.output_frames_gen;
			used_frames += srcData.input_frames_used;
		} else {
			meddbg("Wrong output_frames_gen : %d\n", srcData.output_frames_gen);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}
		medvdbg("%d resampled from (%d/%d) @ 0x%x\t", resampled_frames, used_frames, frames, srcData.data_out);
	}
	medvdbg("Resample finished\n");

	return resampled_frames;
}

static audio_manager_result_t get_audio_volume(int fd, audio_config_t *config, audio_card_type_t card_type)
{
	struct audio_caps_desc_s caps_desc;
	int ret;
	uint16_t max_volume;
	uint16_t cur_volume;

	caps_desc.caps.ac_len = sizeof(struct audio_caps_s);
	caps_desc.caps.ac_type = AUDIO_TYPE_FEATURE;
	caps_desc.caps.ac_format.hw = AUDIO_FU_VOLUME;

	if (card_type == INPUT) {
		caps_desc.caps.ac_format.hw = AUDIO_FU_INP_GAIN;
	}

	ret = ioctl(fd, AUDIOIOC_GETCAPS, (unsigned long *)&caps_desc.caps);
	if (ret < 0) {
		meddbg("Fail to ioctl AUDIOIOC_GETCAPS\n");
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	max_volume = caps_desc.caps.ac_controls.hw[0];
	cur_volume = caps_desc.caps.ac_controls.hw[1];

	/* scale here */
	cur_volume = cur_volume * AUDIO_DEVICE_MAX_VOLUME / (max_volume - (max_volume % AUDIO_DEVICE_MAX_VOLUME));

	config->max_volume = max_volume;
	config->volume = (uint8_t) cur_volume;

	return AUDIO_MANAGER_SUCCESS;
}

static audio_manager_result_t set_audio_volume(audio_handler_t *handler, audio_card_type_t type, uint8_t volume)
{
	int ret;
	int fd;
	struct audio_caps_desc_s caps_desc;

	if (type == INPUT) {
		caps_desc.caps.ac_format.hw = AUDIO_FU_INP_GAIN;
	} else {
		caps_desc.caps.ac_format.hw = AUDIO_FU_VOLUME;
	}

	if (volume > AUDIO_DEVICE_MAX_VOLUME) {
		volume = AUDIO_DEVICE_MAX_VOLUME;
	}

	caps_desc.caps.ac_controls.hw[0] = volume * (handler->config.max_volume / AUDIO_DEVICE_MAX_VOLUME);
	caps_desc.caps.ac_len = sizeof(struct audio_caps_s);
	caps_desc.caps.ac_type = AUDIO_TYPE_FEATURE;

	fd = open(handler->card_path, O_RDONLY);
	if (fd < 0) {
		meddbg("Failed to open device path : %s type : %d\n", handler->card_path, type);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	ret = ioctl(fd, AUDIOIOC_CONFIGURE, (unsigned long)&caps_desc);
	if (ret < 0) {
		meddbg("Fail to set a volume, ret = %d\n", ret);
		ret = AUDIO_MANAGER_DEVICE_FAIL;
		goto errout;
	}

	ret = AUDIO_MANAGER_SUCCESS;
	handler->config.volume = volume;
	medvdbg("Volume = %d\n", volume);

errout:
	close(fd);
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
audio_manager_result_t init_audio_stream_in(audio_handler_t **handler)
{
	if (g_num_found_in_cards == 0) {
		// Find all cards since there's no one found.
		if (find_audio_card(INPUT) != AUDIO_MANAGER_SUCCESS) {
			meddbg("Found no active input audio card\n");
			return AUDIO_MANAGER_NO_AVAIL_CARD;
		}
	}
	// Get an available card
	if (get_audio_card_in(FIND, INVALID_ID, handler) != AUDIO_MANAGER_SUCCESS) {
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}
	pthread_mutex_lock(&(*handler)->card_mutex);
	if ((*handler)->status == AUDIO_CARD_IDLE) {
		(*handler)->status = AUDIO_CARD_READY;
	}
	pthread_mutex_unlock(&(*handler)->card_mutex);

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t init_audio_stream_out(audio_handler_t **handler)
{
	if (g_num_found_out_cards == 0) {
		// Find all cards since there's no one found.
		if (find_audio_card(OUTPUT) != AUDIO_MANAGER_SUCCESS) {
			meddbg("Found no active output audio card\n");
			return AUDIO_MANAGER_NO_AVAIL_CARD;
		}
	}
	// Get an available card
	if (get_audio_card_out(FIND, INVALID_ID, handler) != AUDIO_MANAGER_SUCCESS) {
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}
	pthread_mutex_lock(&(*handler)->card_mutex);
	if ((*handler)->status == AUDIO_CARD_IDLE) {
		(*handler)->status = AUDIO_CARD_READY;
	}
	pthread_mutex_unlock(&(*handler)->card_mutex);

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_audio_stream_in(audio_handler_t *handler, unsigned int channels, unsigned int sample_rate, int format)
{
	struct pcm_config config;
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;

	ASSERT(handler != NULL);
	pthread_mutex_lock(&handler->card_mutex);

	if ((channels == 0) || (sample_rate == 0)) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	if (handler->status == AUDIO_CARD_PAUSE) {
		medvdbg("Reset the previous pcm\n");
		reset_audio_stream_in(handler);
	}

	if (channels > AUDIO_STREAM_CHANNEL_MONO) {
		channels = AUDIO_STREAM_CHANNEL_STEREO;
	}

	memset(&config, 0, sizeof(struct pcm_config));
	config.channels = channels;
	config.rate = get_closest_samprate(handler, sample_rate);
	config.format = format;
	config.period_size = AUDIO_STREAM_VOIP_PERIOD_SIZE;
	config.period_count = AUDIO_STREAM_VOIP_PERIOD_COUNT;

	medvdbg("[IN] Device samplerate: %u, User requested: %u\n", config.rate, sample_rate);
	handler->pcm = pcm_open(handler->card_id, handler->device_id, PCM_IN, &config);
	if (!pcm_is_ready(handler->pcm)) {
		meddbg("fail to pcm_is_ready() error : %s", pcm_get_error(handler->pcm));
		ret = AUDIO_MANAGER_CARD_NOT_READY;
		goto error_with_pcm;
	}

	handler->status = AUDIO_CARD_READY;

	if (sample_rate == config.rate) {
		handler->resample.necessary = false;
	} else {
		handler->resample.necessary = true;
		handler->resample.from = config.rate;
		handler->resample.to = sample_rate;
		handler->resample.ratio = (float)sample_rate / (float)config.rate;
		handler->resample.handle = src_init(CONFIG_AUDIO_RESAMPLER_BUFSIZE);
		handler->resample.buffer = malloc((int)((float)get_input_frames_to_byte(handler, get_input_frame_count(handler)) / handler->resample.ratio) + 1);	// +1 for floating point margin
		if (!handler->resample.buffer) {
			meddbg("malloc for a resampling buffer(stream_in) is failed\n");
			ret = AUDIO_MANAGER_RESAMPLE_FAIL;
			goto error_with_pcm;
		}
	}

	pthread_mutex_unlock(&handler->card_mutex);
	return ret;

error_with_pcm:
	pcm_close(handler->pcm);
	pthread_mutex_unlock(&handler->card_mutex);
	return ret;
}

audio_manager_result_t set_audio_stream_out(audio_handler_t *handler, unsigned int channels, unsigned int sample_rate, int format)
{
	struct pcm_config config;
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;

	ASSERT(handler != NULL);
	pthread_mutex_lock(&handler->card_mutex);

	if ((channels == 0) || (sample_rate == 0)) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	if (handler->status == AUDIO_CARD_PAUSE) {
		medvdbg("Reset the previous pcm\n");
		reset_audio_stream_out(handler);
	}

	if (channels > AUDIO_STREAM_CHANNEL_MONO) {
		channels = AUDIO_STREAM_CHANNEL_STEREO;
	}

	memset(&config, 0, sizeof(struct pcm_config));
	config.channels = channels;
	config.rate = get_closest_samprate(handler, sample_rate);
	config.format = format;
	config.period_size = AUDIO_STREAM_VOIP_PERIOD_SIZE;
	config.period_count = AUDIO_STREAM_VOIP_PERIOD_COUNT;

	if (sample_rate == config.rate) {
		handler->resample.necessary = false;
	} else {
		handler->resample.necessary = true;
		handler->resample.from = sample_rate;
		handler->resample.to = config.rate;
		handler->resample.ratio = (float)config.rate / (float)sample_rate;
	}

	medvdbg("[OUT] Device samplerate: %u, User requested: %u, channels: %u\n", config.rate, sample_rate, channels);

	medvdbg("actual output card id = %d\n", handler->card_id);
	handler->pcm = pcm_open(handler->card_id, handler->device_id, PCM_OUT, &config);

	if (!pcm_is_ready(handler->pcm)) {
		meddbg("fail to pcm_is_ready() error : %s", pcm_get_error(handler->pcm));
		ret = AUDIO_MANAGER_CARD_NOT_READY;
		goto error_with_pcm;
	}

	handler->status = AUDIO_CARD_READY;

	if (handler->resample.necessary) {
		handler->resample.handle = src_init(CONFIG_AUDIO_RESAMPLER_BUFSIZE);
		handler->resample.buffer_size = (int)((float)get_output_frames_to_byte(handler, get_output_frame_count(handler)) * handler->resample.ratio) + 1;	// +1 for floating point margin
		handler->resample.buffer = malloc(handler->resample.buffer_size);
		if (!handler->resample.buffer) {
			meddbg("malloc for a resampling buffer(stream_out) is failed\n");
			ret = AUDIO_MANAGER_RESAMPLE_FAIL;
			goto error_with_pcm;
		}
		medvdbg("resampler ratio = %f, buffer_size = %d\t", handler->resample.ratio, handler->resample.buffer_size);
		medvdbg("buffer address = 0x%x\n", handler->resample.buffer);
	}

	pthread_mutex_unlock(&handler->card_mutex);
	return ret;

error_with_pcm:
	pcm_close(handler->pcm);
	pthread_mutex_unlock(&handler->card_mutex);
	return ret;
}

int start_audio_stream_in(audio_handler_t *handler, void *data, unsigned int frames)
{
	int ret;
	int prepare_retry = AUDIO_STREAM_RETRY_COUNT;

	ASSERT(handler != NULL);
	pthread_mutex_lock(&handler->card_mutex);

	if (handler->status == AUDIO_CARD_PAUSE) {
		ret = ioctl(pcm_get_file_descriptor(handler->pcm), AUDIOIOC_RESUME, NULL);
		if (ret < 0) {
			meddbg("Fail to ioctl AUDIOIOC_RESUME, ret = %d\n", ret);
			ret = AUDIO_MANAGER_DEVICE_FAIL;
			goto error_with_lock;
		}
		//ToDo: AUDIO_CARD_RUNNING should be set with proper logics in other APIs.
		handler->status = AUDIO_CARD_READY;
		medvdbg("Resume the input audio card!!\n");
	}

	do {
		// Todo: Logic needs to be changed.
		//       From: 1 pcm_readi - 1 resampling
		//       To  : * pcm_readi - 1 resampling
		if (handler->resample.necessary) {
			unsigned int frames_to_read = (unsigned int)((float)frames / handler->resample.ratio);
			if (frames_to_read > (get_input_frame_count(handler) / handler->resample.ratio)) {
				frames_to_read = (get_input_frame_count(handler) / handler->resample.ratio);
			}

			handler->resample.buffer_size = get_input_frames_to_byte(handler, frames);

			ret = pcm_readi(handler->pcm, handler->resample.buffer, frames_to_read);
			medvdbg("Read frames (%d/%u) for resample to %u\n", ret, frames_to_read, frames);
		} else {
			ret = pcm_readi(handler->pcm, data, frames);
			medvdbg("Read %d frames\n", ret);
		}

		if (ret == -EPIPE) {
			ret = pcm_prepare(handler->pcm);
			meddbg("PCM is reprepared\n");
			if (ret != OK) {
				meddbg("Fail to pcm_prepare()\n");
				ret = AUDIO_MANAGER_XRUN_STATE;
				goto error_with_lock;
			}
		} else if (ret == -EINVAL) {
			ret = AUDIO_MANAGER_INVALID_PARAM;
			goto error_with_lock;
		} else {
			break;
		}
	} while ((ret == OK) && (prepare_retry--));

	if (handler->resample.necessary) {
		ret = resample_stream_in(handler, data, ret);
		medvdbg("Resampled frames = %d\n", ret);
	}

error_with_lock:
	pthread_mutex_unlock(&handler->card_mutex);

	return ret;
}

int start_audio_stream_out(audio_handler_t *handler, void *data, unsigned int frames)
{
	int ret;
	unsigned int resampled_frames = 0;
	int prepare_retry = AUDIO_STREAM_RETRY_COUNT;
	medvdbg("start_audio_stream_out(%u)\n", frames);

	ASSERT(handler != NULL);
	pthread_mutex_lock(&handler->card_mutex);

	if (handler->status == AUDIO_CARD_PAUSE) {
		ret = ioctl(pcm_get_file_descriptor(handler->pcm), AUDIOIOC_RESUME, NULL);
		if (ret < 0) {
			meddbg("Fail to ioctl AUDIOIOC_RESUME, ret = %d\n", ret);
			ret = AUDIO_MANAGER_DEVICE_FAIL;
			goto error_with_lock;
		}
		//ToDo: AUDIO_CARD_RUNNING should be set with proper logics in other APIs.
		handler->status = AUDIO_CARD_READY;
		medvdbg("Resume the output audio card!!\n");
	}

	if (handler->resample.necessary) {
		resampled_frames = resample_stream_out(handler, data, frames);
	}

	do {
		medvdbg("Start Playing!! Resample : %d\t", handler->resample.necessary);
		if (handler->resample.necessary) {
			ret = pcm_writei(handler->pcm, handler->resample.buffer, resampled_frames);
		} else {
			ret = pcm_writei(handler->pcm, data, frames);
		}

		if (ret < 0) {
			if (ret == -EPIPE) {
				if (prepare_retry > 0) {
					ret = pcm_prepare(handler->pcm);
					if (ret != OK) {
						meddbg("Fail to pcm_prepare()\n");
						ret = AUDIO_MANAGER_XRUN_STATE;
						goto error_with_lock;
					}
					prepare_retry--;
				} else {
					meddbg("prepare_retry = 0\n");
					ret = AUDIO_MANAGER_XRUN_STATE;
					goto error_with_lock;
				}
			} else if (ret == -EINVAL) {
				meddbg("pcm_writei = -EINVAL\n");
				ret = AUDIO_MANAGER_INVALID_PARAM;
				goto error_with_lock;
			} else {
				ret = AUDIO_MANAGER_FAIL;
				goto error_with_lock;
			}
		}
	} while (ret == OK);

error_with_lock:
	pthread_mutex_unlock(&handler->card_mutex);
	return ret;
}

audio_manager_result_t pause_audio_stream_in(audio_handler_t *handler)
{
	int ret;

	ASSERT(handler != NULL);
	pthread_mutex_lock(&handler->card_mutex);

	ret = ioctl(pcm_get_file_descriptor(handler->pcm), AUDIOIOC_PAUSE, NULL);
	if (ret < 0) {
		meddbg("Fail to ioctl AUDIOIOC_PAUSE, ret = %d\n", ret);
		pthread_mutex_unlock(&handler->card_mutex);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	handler->status = AUDIO_CARD_PAUSE;

	pthread_mutex_unlock(&handler->card_mutex);

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t pause_audio_stream_out(audio_handler_t *handler)
{
	int ret;

	ASSERT(handler != NULL);
	pthread_mutex_lock(&handler->card_mutex);

	ret = ioctl(pcm_get_file_descriptor(handler->pcm), AUDIOIOC_PAUSE, NULL);
	if (ret < 0) {
		meddbg("Fail to ioctl AUDIOIOC_PAUSE, ret = %d\n", ret);
		pthread_mutex_unlock(&handler->card_mutex);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	handler->status = AUDIO_CARD_PAUSE;

	pthread_mutex_unlock(&handler->card_mutex);

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t reset_audio_stream_in(audio_handler_t *handler)
{
	ASSERT(handler != NULL);
	pthread_mutex_lock(&handler->card_mutex);

	pcm_close(handler->pcm);
	handler->pcm = NULL;

	if (handler->resample.necessary) {
		if (handler->resample.buffer) {
			free(handler->resample.buffer);
		}
		src_destroy(handler->resample.handle);
	}

	handler->status = AUDIO_CARD_IDLE;

	pthread_mutex_unlock(&handler->card_mutex);

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t reset_audio_stream_out(audio_handler_t *handler)
{
	ASSERT(handler != NULL);
	pthread_mutex_lock(&handler->card_mutex);

	pcm_close(handler->pcm);
	handler->pcm = NULL;

	if (handler->resample.necessary) {
		if (handler->resample.buffer) {
			free(handler->resample.buffer);
		}
		src_destroy(handler->resample.handle);
	}

	handler->status = AUDIO_CARD_IDLE;

	pthread_mutex_unlock(&handler->card_mutex);

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t stop_audio_stream_in(audio_handler_t *handler)
{
	int ret;

	ASSERT(handler != NULL);
	pthread_mutex_lock(&handler->card_mutex);

	ret = pcm_drop(handler->pcm);
	if (ret < 0) {
		meddbg("pcm_drop Failed, ret = %d\n", ret);
		pthread_mutex_unlock(&handler->card_mutex);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	handler->status = AUDIO_CARD_READY;

	pthread_mutex_unlock(&handler->card_mutex);

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t stop_audio_stream_out(audio_handler_t *handler)
{
	int ret;

	ASSERT(handler != NULL);
	pthread_mutex_lock(&handler->card_mutex);

	ret = pcm_drain(handler->pcm);
	if (ret < 0) {
		meddbg("pcm_drain failed ret = %d\n", ret);
		pthread_mutex_unlock(&handler->card_mutex);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	handler->status = AUDIO_CARD_READY;

	pthread_mutex_unlock(&handler->card_mutex);

	return AUDIO_MANAGER_SUCCESS;
}

unsigned int get_input_frame_count(audio_handler_t *handler)
{
	ASSERT(handler != NULL);
	return pcm_get_buffer_size(handler->pcm);
}

unsigned int get_input_frames_to_byte(audio_handler_t *handler, unsigned int frames)
{
	if (frames == 0) {
		return 0;
	}

	ASSERT(handler != NULL);
	return pcm_frames_to_bytes(handler->pcm, frames);
}

unsigned int get_input_bytes_to_frame(audio_handler_t *handler, unsigned int bytes)
{
	if (bytes == 0) {
		return 0;
	}

	ASSERT(handler != NULL);
	return pcm_bytes_to_frames(handler->pcm, bytes);
}

unsigned int get_output_frame_count(audio_handler_t *handler)
{
	ASSERT(handler != NULL);
	return pcm_get_buffer_size(handler->pcm);
}

unsigned int get_output_frames_to_byte(audio_handler_t *handler, unsigned int frames)
{
	if (frames == 0) {
		return 0;
	}

	ASSERT(handler != NULL);
	return pcm_frames_to_bytes(handler->pcm, frames);
}

uint32_t get_output_bytes_to_frame(audio_handler_t *handler, unsigned int bytes)
{
	if (bytes == 0) {
		return 0;
	}

	ASSERT(handler != NULL);
	return pcm_bytes_to_frames(handler->pcm, bytes);
}

uint16_t get_max_audio_volume(void)
{
	return AUDIO_DEVICE_MAX_VOLUME;
}

int get_input_audio_volume(audio_handler_t *handler)
{
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME	//TODO consider about config for gain.
	int ret;
	int fd;

	ASSERT(handler != NULL);
	fd = open(handler->card_path, O_RDONLY);
	if (fd < 0) {
		meddbg("Failed to open audio driver, path : %s errno : %d\n", handler->card_path, errno);
		return AUDIO_MANAGER_CARD_NOT_READY;
	}

	/* get volume of current card first */
	if ((ret = get_audio_volume(fd, &(handler->config), INPUT)) != AUDIO_MANAGER_SUCCESS) {
		meddbg("Get volume failed card path : %s card_type : %d ret : %d\n", handler->card_path, INPUT, ret);
		close(fd);
		return ret;
	}
	medvdbg("Max volume: %d, Current Volume : %d card path : %s\n", handler->config.max_volume, handler->config.volume, handler->card_path);

	close(fd);

	return (int)handler->config.volume;
#endif
	return 0;
}

int get_output_audio_volume(audio_handler_t *handler)
{
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	int ret;
	int fd;

	ASSERT(handler != NULL);
	fd = open(handler->card_path, O_RDONLY);
	if (fd < 0) {
		meddbg("Failed to open audio driver, path : %s errno : %d\n", handler->card_path, errno);
		return AUDIO_MANAGER_CARD_NOT_READY;
	}

	/* get volume of current card first */
	if ((ret = get_audio_volume(fd, &(handler->config), OUTPUT)) != AUDIO_MANAGER_SUCCESS) {
		meddbg("Get volume failed card path : %s card_type : %d ret : %d\n", handler->card_path, INPUT, ret);
		close(fd);
		return ret;
	}
	medvdbg("Max volume: %d, Current Volume : %d card path : %s\n", handler->config.max_volume, handler->config.volume, handler->card_path);

	close(fd);

	return (int)handler->config.volume;
#endif
	return 0;
}

audio_manager_result_t set_input_audio_volume(audio_handler_t *handler, uint8_t volume)
{
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	ASSERT(handler != NULL);
	if (handler->config.volume == volume) {
		return AUDIO_MANAGER_SUCCESS;
	}

	return set_audio_volume(handler, INPUT, volume);
#endif
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_output_audio_volume(audio_handler_t *handler, uint8_t volume)
{
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	ASSERT(handler != NULL);
	if (handler->config.volume == volume) {
		return AUDIO_MANAGER_SUCCESS;
	}

	return set_audio_volume(handler, OUTPUT, volume);
#endif
	return AUDIO_MANAGER_SUCCESS;
}
