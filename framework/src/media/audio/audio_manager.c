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
 * Private Declarations
 ****************************************************************************/
enum audio_card_status_e {
	AUDIO_CARD_IDLE = 0,
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

struct audio_config_s {
	uint16_t volume;
	uint16_t max_volume;
	char *manufacturer_name;
	enum manufacturer_type_e manufacturer_type;
	int audio_card_type;
};

struct audio_resample_s {
	bool necessary;
	uint32_t samprate_types;
	uint32_t from;
	uint32_t to;
	void *buffer;
	uint16_t buffer_size;
	float ratio;
	src_handle_t handle;
};

struct audio_card_info_s {
	char card_path[256];
	enum audio_card_status_e status;
	uint8_t device_id;
	struct audio_config_s config;
	struct pcm *pcm;
	struct audio_resample_s resample;
};

struct audio_samprate_map_entry_s {
	uint32_t samprate_types;
	uint32_t samprate;
};

typedef enum audio_card_status_e audio_card_status_t;
typedef enum manufacturer_type_e manufacturer_t;
typedef enum audio_card_type_e audio_card_type_t;
typedef struct audio_config_s audio_config_t;
typedef struct audio_card_info_s audio_card_info_t;

audio_card_info_t g_audio_in_cards[CONFIG_AUDIO_MAX_INPUT_CARD_NUM];
audio_card_info_t g_audio_out_cards[CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM];

static int g_actual_audio_in_card_id = INVALID_ID;
static int g_actual_audio_out_card_id = INVALID_ID;

static void get_hardware_params(audio_card_info_t *card, audio_card_type_t card_type);
static int get_actual_audio_in_card_id(void);
static int get_actual_audio_out_card_id(void);
static audio_manager_result_t get_active_audio_device_pcm(struct pcm **pcm, audio_card_type_t type);
static audio_manager_result_t get_audio_volume(int fd, audio_config_t *config, audio_card_type_t card_type);
static audio_manager_result_t set_audio_volume(audio_card_type_t type, uint8_t volume);
static audio_manager_result_t get_supported_sample_rate(int fd, uint32_t *sample_type, audio_card_type_t type);
static uint32_t get_closest_samprate(uint32_t origin_samprate, audio_card_type_t type);
static int resample_stream_in(audio_card_info_t *cur_card, void *data, uint32_t frames);
static int resample_stream_out(audio_card_info_t *cur_card, void *data, uint32_t frames);

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
 * Public Functions
 ****************************************************************************/
static audio_manager_result_t find_audio_card(audio_card_type_t card_type)
{
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	int card_id = INVALID_ID;
	int device_id = INVALID_ID;
	char type;
	struct dirent *dir_entry;
	DIR *dir_info;
	audio_card_info_t *card = g_audio_out_cards;
	int max_card_num = CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM;
	char type_chr = 'p';
	int found_cards = 0;

	if (!(dir_info = opendir("/dev/audio/"))) {
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	if (card_type == INPUT) {
		max_card_num = CONFIG_AUDIO_MAX_INPUT_CARD_NUM;
		type_chr = 'c';
		card = g_audio_in_cards;
	}

	while ((dir_entry = readdir(dir_info)) != NULL) {
		// TODO: Add cases for various drivers. Currently, identify 'pcm' drivers only.
		if ((dir_entry->d_name[0] != 'p') || (dir_entry->d_name[1] != 'c') || (dir_entry->d_name[2] != 'm') || (sscanf(&dir_entry->d_name[3], "C%uD%u%c", &card_id, &device_id, &type) != 3)) {
			continue;
		}

		/* Find next card if driver name isn't follow naming policy */
		if ((card_id < 0) || (card_id >= max_card_num) || (device_id < 0) || (device_id >= MAX_AUDIO_DEVICE_NUM)) {
			meddbg("Found wrong card card_id : %d device_id : %d\n", card_id, device_id);
			continue;
		}

		if (type == type_chr) {
			snprintf(card[card_id].card_path, strlen("/dev/audio/") + strlen(dir_entry->d_name) + 1, "%s%s", "/dev/audio/", dir_entry->d_name);
			card[card_id].status = AUDIO_CARD_IDLE;
			card[card_id].device_id = device_id;
			found_cards++;
			medvdbg("Found an audio card, total card : %d id : %d, card_path : %s\n", found_cards, card_id, card[card_id].card_path);

			/* update actual card here */
			if (card_type == INPUT) {
				g_actual_audio_in_card_id = get_actual_audio_in_card_id();
			} else {
				g_actual_audio_out_card_id = get_actual_audio_out_card_id();
			}

			get_hardware_params(&card[card_id], card_type);
		}
	}

	/* If nothing founded */
	if (found_cards == 0) {
		meddbg("Failed to find card\n");
		ret = AUDIO_MANAGER_NO_AVAIL_CARD;
		goto error_out;
	}

error_out:
	closedir(dir_info);
	return ret;
}

static audio_manager_result_t find_audio_card_if_empty(audio_card_type_t card_type)
{
	int id = g_actual_audio_out_card_id;

	if (card_type == INPUT) {
		id = g_actual_audio_in_card_id;
	}

	if ((id == INVALID_ID) && (find_audio_card(card_type) != AUDIO_MANAGER_SUCCESS)) {
		meddbg("Found no active audio card type : %d\n", card_type);
		return AUDIO_MANAGER_CARD_NOT_FOUND;
	}
	return AUDIO_MANAGER_SUCCESS;
}

static audio_manager_result_t set_audio_volume(audio_card_type_t type, uint8_t volume)
{
	int ret;
	int fd;
	struct audio_caps_desc_s caps_desc;
	audio_card_info_t *card;

	/* check actual card and try to find all card if there is no actual card */
	ret = find_audio_card_if_empty(type);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	if (volume > AUDIO_DEVICE_MAX_VOLUME) {
		volume = AUDIO_DEVICE_MAX_VOLUME;
	}

	if (type == INPUT) {
		caps_desc.caps.ac_format.hw = AUDIO_FU_INP_GAIN;
		card = &g_audio_in_cards[g_actual_audio_in_card_id];
	} else {
		caps_desc.caps.ac_format.hw = AUDIO_FU_VOLUME;
		card = &g_audio_out_cards[g_actual_audio_out_card_id];

	}
	caps_desc.caps.ac_controls.hw[0] = volume * (int)(card->config.max_volume / AUDIO_DEVICE_MAX_VOLUME);
	caps_desc.caps.ac_len = sizeof(struct audio_caps_s);
	caps_desc.caps.ac_type = AUDIO_TYPE_FEATURE;

	fd = open(card->card_path, O_RDONLY);
	if (fd < 0) {
		meddbg("Failed to open device path : %s type : %d\n", card->card_path, type);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	ret = ioctl(fd, AUDIOIOC_CONFIGURE, (unsigned long)&caps_desc);
	if (ret < 0) {
		meddbg("Fail to set a volume, ret = %d\n", ret);
		ret = AUDIO_MANAGER_DEVICE_FAIL;
		goto errout;
	}

	ret = AUDIO_MANAGER_SUCCESS;
	card->config.volume = volume;
	medvdbg("Volume = %d\n", volume);

errout:
	close(fd);
	return ret;
}

audio_manager_result_t set_input_audio_volume(uint8_t volume)
{
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	if (g_audio_in_cards[g_actual_audio_in_card_id].config.volume == volume) {
		return AUDIO_MANAGER_SUCCESS;
	}

	return set_audio_volume(INPUT, volume);
#endif
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_output_audio_volume(uint8_t volume)
{
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	if (g_audio_out_cards[g_actual_audio_out_card_id].config.volume == volume) {
		return AUDIO_MANAGER_SUCCESS;
	}

	return set_audio_volume(OUTPUT, volume);
#endif
	return AUDIO_MANAGER_SUCCESS;
}

int get_input_audio_volume(void)
{
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME	//TODO consider about config for gain.
	int fd;
	int ret;

	fd = open(g_audio_in_cards[g_actual_audio_in_card_id].card_path, O_RDONLY);
	if (fd < 0) {
		meddbg("Fail to open an input driver\n");
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	ret = get_audio_volume(fd, &g_audio_in_cards[g_actual_audio_in_card_id].config, INPUT);
	close(fd);
	if (ret == AUDIO_MANAGER_SUCCESS) {
		return g_audio_in_cards[g_actual_audio_in_card_id].config.volume;
	}

	return AUDIO_MANAGER_DEVICE_FAIL;
#endif
	return 0;
}

int get_output_audio_volume(void)
{
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	int fd;
	int ret;

	fd = open(g_audio_out_cards[g_actual_audio_out_card_id].card_path, O_RDONLY);
	if (fd < 0) {
		meddbg("Fail to open an input driver\n");
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	ret = get_audio_volume(fd, &g_audio_out_cards[g_actual_audio_out_card_id].config, OUTPUT);
	close(fd);
	if (ret == AUDIO_MANAGER_SUCCESS) {
		return g_audio_out_cards[g_actual_audio_out_card_id].config.volume;
	}

	return AUDIO_MANAGER_DEVICE_FAIL;
#endif
	return 0;
}

uint16_t get_max_audio_volume(void)
{
	return AUDIO_DEVICE_MAX_VOLUME;
}

uint32_t get_input_frame_count(void)
{
	if (g_actual_audio_in_card_id < 0) {
		meddbg("No input audio card is active.\n");
		return 0;
	}

	return pcm_get_buffer_size(g_audio_in_cards[g_actual_audio_in_card_id].pcm);
}

uint32_t get_input_frames_byte_size(uint32_t frames)
{
	if (g_actual_audio_in_card_id < 0) {
		meddbg("No input audio card is active.\n");
		return 0;
	}

	return pcm_frames_to_bytes(g_audio_in_cards[g_actual_audio_in_card_id].pcm, frames);
}

uint32_t get_input_bytes_frame_count(uint32_t bytes)
{
	if (g_actual_audio_in_card_id < 0) {
		meddbg("No input audio card is active.\n");
		return 0;
	}

	return pcm_bytes_to_frames(g_audio_in_cards[g_actual_audio_in_card_id].pcm, bytes);
}

uint32_t get_output_frame_count(void)
{
	if (g_actual_audio_out_card_id < 0) {
		meddbg("No output audio card is active.\n");
		return 0;
	}

	return pcm_get_buffer_size(g_audio_out_cards[g_actual_audio_out_card_id].pcm);
}

uint32_t get_output_frames_byte_size(uint32_t frames)
{
	if (g_actual_audio_out_card_id < 0) {
		meddbg("No output audio card is active.\n");
		return 0;
	}

	return pcm_frames_to_bytes(g_audio_out_cards[g_actual_audio_out_card_id].pcm, frames);
}

uint32_t get_output_bytes_frame_count(uint32_t bytes)
{
	if (g_actual_audio_out_card_id < 0) {
		meddbg("No output audio card is active.\n");
		return 0;
	}

	return pcm_bytes_to_frames(g_audio_out_cards[g_actual_audio_out_card_id].pcm, bytes);
}

static audio_manager_result_t get_active_audio_device_pcm(struct pcm **pcm, audio_card_type_t type)
{
	if (type == INPUT) {
		if (g_actual_audio_in_card_id < 0) {
			meddbg("Found no active input audio card\n");
			return AUDIO_MANAGER_NO_AVAIL_CARD;
		}

		*pcm = g_audio_in_cards[g_actual_audio_in_card_id].pcm;
	} else {
		if (g_actual_audio_out_card_id < 0) {
			meddbg("Found no active output audio card\n");
			return AUDIO_MANAGER_NO_AVAIL_CARD;
		}

		*pcm = g_audio_out_cards[g_actual_audio_out_card_id].pcm;
	}

	if (!pcm_is_ready(*pcm)) {
		pcm_close(*pcm);
		meddbg("Found a null pcm\n");
		return AUDIO_MANAGER_CARD_NOT_READY;
	}

	return AUDIO_MANAGER_SUCCESS;
}

/* TODO this function should consider priority of each function in future */
static int get_actual_audio_in_card_id()
{
	int i;
	for (i = 0; i < CONFIG_AUDIO_MAX_INPUT_CARD_NUM; i++) {
		if (g_audio_in_cards[i].status != AUDIO_CARD_IDLE) {
			return i;
		}
	}

	/* Now all cards are IDLE state, we select card which has highest priority */
	g_audio_in_cards[0].status = AUDIO_CARD_READY;
	return 0;
}

static int get_actual_audio_out_card_id()
{
	int i;
	for (i = 0; i < CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM; i++) {
		if (g_audio_out_cards[i].status != AUDIO_CARD_IDLE) {
			return i;
		}
	}
	/* Now all cards are IDLE state, we select card which has highest priority */
	g_audio_out_cards[0].status = AUDIO_CARD_READY;
	return 0;
}

static int resample_stream_in(audio_card_info_t *cur_card, void *data, uint32_t frames)
{
	int used_frames = 0;
	int resampled_frames = 0;
	src_data_t srcData = { 0, };

	srcData.channels_num = 2;
	srcData.origin_sample_rate = cur_card->resample.from;
	srcData.origin_sample_width = SAMPLE_WIDTH_16BITS;
	srcData.desired_sample_rate = cur_card->resample.to;
	srcData.desired_sample_width = SAMPLE_WIDTH_16BITS;
	srcData.input_frames_used = 0;
	srcData.data_out = data;
	srcData.out_buf_length = cur_card->resample.buffer_size;

	while (frames > used_frames) {
		srcData.data_in = cur_card->resample.buffer + get_input_frames_byte_size(used_frames);
		srcData.input_frames = frames - used_frames;
		srcData.data_out = data + get_input_frames_byte_size(resampled_frames);
		medvdbg("data_in addr = 0x%x + %d\t", srcData.data_in, get_input_frames_byte_size(used_frames));
		if (src_simple(cur_card->resample.handle, &srcData) != SRC_ERR_NO_ERROR) {
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

int start_audio_stream_in(void *data, uint32_t frames)
{
	int ret;
	int prepare_retry = AUDIO_STREAM_RETRY_COUNT;
	struct pcm *pcm;
	audio_card_info_t *cur_card;

	if ((ret = get_active_audio_device_pcm(&pcm, INPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	cur_card = &g_audio_in_cards[g_actual_audio_in_card_id];
	if (cur_card->status == AUDIO_CARD_PAUSE) {
		ret = ioctl(pcm_get_file_descriptor(pcm), AUDIOIOC_RESUME, NULL);
		if (ret < 0) {
			meddbg("Fail to ioctl AUDIOIOC_RESUME, ret = %d\n", ret);
			return AUDIO_MANAGER_DEVICE_FAIL;
		}

		cur_card->status = AUDIO_CARD_RUNNING;
		medvdbg("Resume the input audio card!!\n");
	}

	do {
		// Todo: Logic needs to be changed.
		//       From: 1 pcm_readi - 1 resampling
		//       To  : * pcm_readi - 1 resampling
		if (cur_card->resample.necessary) {
			unsigned int frames_to_read = (unsigned int)((float)frames / cur_card->resample.ratio);
			if (frames_to_read > (get_input_frame_count() / cur_card->resample.ratio)) {
				frames_to_read = (get_input_frame_count() / cur_card->resample.ratio);
			}

			cur_card->resample.buffer_size = get_input_frames_byte_size(frames);

			ret = pcm_readi(pcm, cur_card->resample.buffer, frames_to_read);
			medvdbg("Read frames (%d/%u) for resample to %u\n", ret, frames_to_read, frames);
		} else {
			ret = pcm_readi(pcm, data, frames);
			medvdbg("Read %d frames\n", ret);
		}

		if (ret == -EPIPE) {
			ret = pcm_prepare(pcm);
			meddbg("PCM is reprepared\n");
			if (ret != OK) {
				meddbg("Fail to pcm_prepare()\n");
				return AUDIO_MANAGER_XRUN_STATE;
			}
		} else if (ret == -EINVAL) {
			return AUDIO_MANAGER_INVALID_PARAM;
		} else {
			break;
		}
	} while ((ret == OK) && (prepare_retry--));

	if (cur_card->resample.necessary) {
		ret = resample_stream_in(cur_card, data, ret);
		medvdbg("Resampled frames = %d\n", ret);
	}

	return ret;
}

static int resample_stream_out(audio_card_info_t *cur_card, void *data, uint32_t frames)
{
	int used_frames = 0;
	int resampled_frames = 0;
	src_data_t srcData = { 0, };

	srcData.channels_num = 2;
	srcData.origin_sample_rate = cur_card->resample.from;
	srcData.origin_sample_width = SAMPLE_WIDTH_16BITS;
	srcData.desired_sample_rate = cur_card->resample.to;
	srcData.desired_sample_width = SAMPLE_WIDTH_16BITS;
	srcData.input_frames_used = 0;
	srcData.data_out = cur_card->resample.buffer;
	srcData.out_buf_length = cur_card->resample.buffer_size;
	medvdbg("resampler buffer_size = %d, buffer_addr = 0x%x\n", cur_card->resample.buffer_size, cur_card->resample.buffer);

	while (frames > used_frames) {
		srcData.data_in = data + get_output_frames_byte_size(used_frames);
		srcData.input_frames = frames - used_frames;
		srcData.data_out = cur_card->resample.buffer + get_output_frames_byte_size(resampled_frames);
		medvdbg("buffer addr = 0x%x   data_out addr = 0x%x   ", cur_card->resample.buffer, srcData.data_out);
		if (src_simple(cur_card->resample.handle, &srcData) != SRC_ERR_NO_ERROR) {
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

int start_audio_stream_out(void *data, uint32_t frames)
{
	int ret;
	int resampled_frames = 0;
	int prepare_retry = AUDIO_STREAM_RETRY_COUNT;
	struct pcm *pcm;
	audio_card_info_t *cur_card;
	medvdbg("start_audio_stream_out(%u)\n", frames);

	if ((ret = get_active_audio_device_pcm(&pcm, OUTPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}
	cur_card = &g_audio_out_cards[g_actual_audio_out_card_id];

	if (cur_card->status == AUDIO_CARD_PAUSE) {
		ret = ioctl(pcm_get_file_descriptor(pcm), AUDIOIOC_RESUME, NULL);
		if (ret < 0) {
			meddbg("Fail to ioctl AUDIOIOC_RESUME, ret = %d\n", ret);
			return AUDIO_MANAGER_DEVICE_FAIL;
		}

		cur_card->status = AUDIO_CARD_RUNNING;
		medvdbg("Resume the output audio card!!\n");
	}

	if (cur_card->resample.necessary) {
		resampled_frames = resample_stream_out(cur_card, data, frames);
	}

	do {
		medvdbg("Start Playing!! Resample : %d\t", cur_card->resample.necessary);
		if (cur_card->resample.necessary) {
			ret = pcm_writei(pcm, cur_card->resample.buffer, resampled_frames);
		} else {
			ret = pcm_writei(pcm, data, frames);
		}

		if (ret < 0) {
			if (ret == -EPIPE) {
				if (prepare_retry > 0) {
					ret = pcm_prepare(pcm);
					if (ret != OK) {
						meddbg("Fail to pcm_prepare()\n");
						return AUDIO_MANAGER_XRUN_STATE;
					}
					prepare_retry--;
				} else {
					meddbg("prepare_retry = 0\n");
					return AUDIO_MANAGER_XRUN_STATE;
				}
			} else if (ret == -EINVAL) {
				meddbg("pcm_writei = -EINVAL\n");
				return AUDIO_MANAGER_INVALID_PARAM;
			} else {
				return AUDIO_MANAGER_FAIL;
			}
		}
	} while (ret == OK);

	return ret;
}

audio_manager_result_t set_audio_stream_in(uint8_t channels, uint32_t sample_rate, uint8_t format)
{
	struct pcm_config config;

	if ((channels == 0) || (sample_rate == 0)) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	/* check actual card and try to find all card if there is no actual card */
	if (find_audio_card_if_empty(INPUT) != AUDIO_MANAGER_SUCCESS) {
		meddbg("Found no active input audio card\n");
		return AUDIO_MANAGER_CARD_NOT_FOUND;
	}

	if (channels > AUDIO_STREAM_CHANNEL_MONO) {
		channels = AUDIO_STREAM_CHANNEL_STEREO;
	}

	memset(&config, 0, sizeof(struct pcm_config));
	config.channels = channels;
	config.rate = get_closest_samprate(sample_rate, INPUT);
	config.format = format;
	config.period_size = AUDIO_STREAM_VOIP_PERIOD_SIZE;
	config.period_count = AUDIO_STREAM_VOIP_PERIOD_COUNT;

	medvdbg("[IN] Device samplerate: %u, User requested: %u\n", config.rate, sample_rate);

	medvdbg("actual input card id = %d\n", g_actual_audio_in_card_id);
	g_audio_in_cards[g_actual_audio_in_card_id].pcm = pcm_open(g_actual_audio_in_card_id, 0, PCM_IN, &config);
	if (!pcm_is_ready(g_audio_in_cards[g_actual_audio_in_card_id].pcm)) {
		meddbg("fail to pcm_is_ready() error : %s", pcm_get_error(g_audio_in_cards[g_actual_audio_in_card_id].pcm));
		pcm_close(g_audio_in_cards[g_actual_audio_in_card_id].pcm);
		return AUDIO_MANAGER_CARD_NOT_READY;
	}

	g_audio_in_cards[g_actual_audio_in_card_id].status = AUDIO_CARD_READY;

	if (sample_rate == config.rate) {
		g_audio_in_cards[g_actual_audio_in_card_id].resample.necessary = false;
	} else {
		g_audio_in_cards[g_actual_audio_in_card_id].resample.necessary = true;
		g_audio_in_cards[g_actual_audio_in_card_id].resample.from = config.rate;
		g_audio_in_cards[g_actual_audio_in_card_id].resample.to = sample_rate;
		g_audio_in_cards[g_actual_audio_in_card_id].resample.ratio = (float)sample_rate / (float)config.rate;
		g_audio_in_cards[g_actual_audio_in_card_id].resample.handle = src_init(CONFIG_AUDIO_RESAMPLER_BUFSIZE);
		g_audio_in_cards[g_actual_audio_in_card_id].resample.buffer = malloc((int)((float)get_input_frames_byte_size(get_input_frame_count()) / g_audio_in_cards[g_actual_audio_in_card_id].resample.ratio) + 1);	// +1 for floating point margin
		if (!g_audio_in_cards[g_actual_audio_in_card_id].resample.buffer) {
			meddbg("malloc for a resampling buffer(stream_in) is failed\n");
			pcm_close(g_audio_in_cards[g_actual_audio_in_card_id].pcm);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}
	}

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_audio_stream_out(uint8_t channels, uint32_t sample_rate, uint8_t format)
{
	struct pcm_config config;

	if ((channels == 0) || (sample_rate == 0)) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	/* check actual card and try to find all card if there is no actual card */
	if (find_audio_card_if_empty(OUTPUT) != AUDIO_MANAGER_SUCCESS) {
		meddbg("Found no active output audio card\n");
		return AUDIO_MANAGER_CARD_NOT_FOUND;
	}

	if (channels > AUDIO_STREAM_CHANNEL_MONO) {
		channels = AUDIO_STREAM_CHANNEL_STEREO;
	}

	memset(&config, 0, sizeof(struct pcm_config));
	config.channels = channels;
	config.rate = get_closest_samprate(sample_rate, OUTPUT);
	config.format = format;
	config.period_size = AUDIO_STREAM_VOIP_PERIOD_SIZE;
	config.period_count = AUDIO_STREAM_VOIP_PERIOD_COUNT;

	if (sample_rate == config.rate) {
		g_audio_out_cards[g_actual_audio_out_card_id].resample.necessary = false;
	} else {
		g_audio_out_cards[g_actual_audio_out_card_id].resample.necessary = true;
		g_audio_out_cards[g_actual_audio_out_card_id].resample.from = sample_rate;
		g_audio_out_cards[g_actual_audio_out_card_id].resample.to = config.rate;
		g_audio_out_cards[g_actual_audio_out_card_id].resample.ratio = (float)config.rate / (float)sample_rate;
	}

	medvdbg("[OUT] Device samplerate: %u, User requested: %u, channels: %u\n", config.rate, sample_rate, channels);

	medvdbg("actual output card id = %d\n", g_actual_audio_out_card_id);
	g_audio_out_cards[g_actual_audio_out_card_id].pcm = pcm_open(g_actual_audio_out_card_id, 0, PCM_OUT, &config);

	if (!pcm_is_ready(g_audio_out_cards[g_actual_audio_out_card_id].pcm)) {
		meddbg("fail to pcm_is_ready() error : %s", pcm_get_error(g_audio_out_cards[g_actual_audio_out_card_id].pcm));
		pcm_close(g_audio_out_cards[g_actual_audio_out_card_id].pcm);
		return AUDIO_MANAGER_CARD_NOT_READY;
	}

	g_audio_out_cards[g_actual_audio_out_card_id].status = AUDIO_CARD_READY;

	if (g_audio_out_cards[g_actual_audio_out_card_id].resample.necessary) {
		g_audio_out_cards[g_actual_audio_out_card_id].resample.handle = src_init(CONFIG_AUDIO_RESAMPLER_BUFSIZE);
		g_audio_out_cards[g_actual_audio_out_card_id].resample.buffer_size = (int)((float)get_output_frames_byte_size(get_output_frame_count()) * g_audio_out_cards[g_actual_audio_out_card_id].resample.ratio) + 1;	// +1 for floating point margin
		g_audio_out_cards[g_actual_audio_out_card_id].resample.buffer = malloc(g_audio_out_cards[g_actual_audio_out_card_id].resample.buffer_size);
		if (!g_audio_out_cards[g_actual_audio_out_card_id].resample.buffer) {
			meddbg("malloc for a resampling buffer(stream_out) is failed\n");
			pcm_close(g_audio_out_cards[g_actual_audio_out_card_id].pcm);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}
		medvdbg("resampler ratio = %f, buffer_size = %d\t", g_audio_out_cards[g_actual_audio_out_card_id].resample.ratio, g_audio_out_cards[g_actual_audio_out_card_id].resample.buffer_size);
		medvdbg("buffer address = 0x%x\n", g_audio_out_cards[g_actual_audio_out_card_id].resample.buffer);
	}

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t pause_audio_stream_in(void)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, INPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	ret = ioctl(pcm_get_file_descriptor(pcm), AUDIOIOC_PAUSE, NULL);
	if (ret < 0) {
		meddbg("Fail to ioctl AUDIOIOC_PAUSE, ret = %d\n", ret);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	g_audio_in_cards[g_actual_audio_in_card_id].status = AUDIO_CARD_PAUSE;

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t pause_audio_stream_out(void)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, OUTPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	ret = ioctl(pcm_get_file_descriptor(pcm), AUDIOIOC_PAUSE, NULL);
	if (ret < 0) {
		meddbg("Fail to ioctl AUDIOIOC_PAUSE, ret = %d\n", ret);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	g_audio_out_cards[g_actual_audio_out_card_id].status = AUDIO_CARD_PAUSE;

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t stop_audio_stream_in(void)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, INPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	ret = ioctl(pcm_get_file_descriptor(pcm), AUDIOIOC_STOP, NULL);
	if (ret < 0) {
		meddbg("Fail to ioctl AUDIOIOC_STOP, ret = %d\n", ret);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	g_audio_in_cards[g_actual_audio_in_card_id].status = AUDIO_CARD_READY;

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t stop_audio_stream_out(void)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, OUTPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	ret = ioctl(pcm_get_file_descriptor(pcm), AUDIOIOC_STOP, NULL);
	if (ret < 0) {
		meddbg("Fail to ioctl AUDIOIOC_STOP, ret = %d\n", ret);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	g_audio_out_cards[g_actual_audio_out_card_id].status = AUDIO_CARD_READY;

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t reset_audio_stream_in(void)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, INPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	pcm_close(pcm);
	pcm = NULL;

	if (g_audio_in_cards[g_actual_audio_in_card_id].resample.necessary) {
		if (g_audio_in_cards[g_actual_audio_in_card_id].resample.buffer) {
			free(g_audio_in_cards[g_actual_audio_in_card_id].resample.buffer);
		}
		src_destroy(g_audio_in_cards[g_actual_audio_in_card_id].resample.handle);
	}

	g_audio_in_cards[g_actual_audio_in_card_id].status = AUDIO_CARD_READY;

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t reset_audio_stream_out(void)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, OUTPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	pcm_close(pcm);
	pcm = NULL;

	if (g_audio_out_cards[g_actual_audio_out_card_id].resample.necessary) {
		if (g_audio_out_cards[g_actual_audio_out_card_id].resample.buffer) {
			free(g_audio_out_cards[g_actual_audio_out_card_id].resample.buffer);
		}
		src_destroy(g_audio_out_cards[g_actual_audio_out_card_id].resample.handle);
	}

	g_audio_out_cards[g_actual_audio_out_card_id].status = AUDIO_CARD_READY;

	return AUDIO_MANAGER_SUCCESS;
}

static uint32_t get_closest_samprate(uint32_t origin_samprate, audio_card_type_t type)
{
	int i;
	uint32_t result;
	uint32_t samprate_types;
	int count = sizeof(g_audio_samprate_entry) / sizeof(struct audio_samprate_map_entry_s);

	ASSERT(count > 0);

	if (type == INPUT) {
		ASSERT(g_actual_audio_in_card_id >= 0);
		samprate_types = g_audio_in_cards[g_actual_audio_in_card_id].resample.samprate_types;
	} else {
		ASSERT(g_actual_audio_out_card_id >= 0);
		samprate_types = g_audio_out_cards[g_actual_audio_out_card_id].resample.samprate_types;
	}

	result = g_audio_samprate_entry[count - 1].samprate;

	for (i = count - 1; i >= 0; i--) {
		if (g_audio_samprate_entry[i].samprate_types & samprate_types) {
			if (g_audio_samprate_entry[i].samprate >= origin_samprate) {
				result = g_audio_samprate_entry[i].samprate;
			}
		}
	}

	return result;
}

static audio_manager_result_t get_supported_sample_rate(int fd, uint32_t *sample_type, audio_card_type_t card_type)
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

static audio_manager_result_t get_audio_volume(int fd, audio_config_t *config, audio_card_type_t card_type)
{
	struct audio_caps_desc_s caps_desc;
	int ret;
	int max_volume;
	int cur_volume;

	/* check actual card and try to find all card if there is no actual card */
	ret = find_audio_card_if_empty(card_type);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

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

	max_volume = caps_desc.caps.ac_controls.b[0];
	cur_volume = caps_desc.caps.ac_controls.b[1];

	/* scale here */
	cur_volume = cur_volume * AUDIO_DEVICE_MAX_VOLUME / (max_volume - (max_volume % AUDIO_DEVICE_MAX_VOLUME));

	config->max_volume = max_volume;
	config->volume = cur_volume;

	return AUDIO_MANAGER_SUCCESS;
}

static void get_hardware_params(audio_card_info_t *card, audio_card_type_t card_type)
{
	int fd;
	int ret;
	audio_config_t *config;

	fd = open(card->card_path, O_RDONLY);
	if (fd < 0) {
		meddbg("Failed to open audio driver, path : %s errno : %d\n", card->card_path, errno);
		return;
	}

	config = &card->config;
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	/* get volume of current card first */
	ret = get_audio_volume(fd, config, card_type);

	/* If it is failed, it doesn't return because we should get next h/w params */
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Get volume failed card path : %s card_type : %d ret : %d\n", card->card_path, card_type, ret);
	}

	medvdbg("Max volume: %d, Current Volume : %d card path : %s\n", card->config.max_volume, card->config.volume, card->card_path);
#endif
	/* get supported sample rate type */
	ret = get_supported_sample_rate(fd, &card->resample.samprate_types, card_type);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Get supported sample rate failed card path : %s card_type : %d ret : %d\n", card->card_path, card_type, ret);
	}
	medvdbg("card path : %s Supported sample rate: %x\n", card->card_path, card->resample.samprate_types);
	close(fd);
}
