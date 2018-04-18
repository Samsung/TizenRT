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

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MAX_AUDIO_DEVICE_NUM			3
#define AUDIO_STREAM_CHANNEL_MONO			1
#define AUDIO_STREAM_CHANNEL_STEREO			2
// ToDO: Expand with policies, Large data requires a large period.
#define AUDIO_STREAM_VOIP_PERIOD_SIZE			512
#define AUDIO_STREAM_VOIP_PERIOD_COUNT			8
#define AUDIO_STREAM_VOIP_SAMPLE_RATE			16000

#define AUDIO_STREAM_RETRY_COUNT			2

#define AUDIO_DEVICE_UNDEF			0x0000
#define AUDIO_DEVICE_IN_MICROPHONE		0x0001
#define AUDIO_DEVICE_OUT_EARPIECE		0x0100
#define AUDIO_DEVICE_OUT_SPEAKER		0x0200
#define AUDIO_DEVICE_OUT_HEADSET		0x0400

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

enum audio_device_type_e {
	INPUT = 0,
	OUTPUT
};

struct audio_config_s {
	uint16_t volume;
	char *manufacturer_name;
	enum manufacturer_type_e manufacturer_type;
	int audio_card_type;
};

struct audio_card_info_s {
	char *card_name;
	enum audio_card_status_e status;
	uint8_t device_id;
	struct audio_config_s config;
	struct pcm *pcm;
};

typedef enum audio_card_status_e audio_card_status_t;
typedef enum manufacturer_type_e manufacturer_t;
typedef enum audio_device_type_e audio_device_type_t;
typedef struct audio_config_s audio_config_t;
typedef struct audio_card_info_s audio_dev_info_t;

audio_dev_info_t g_audio_in_cards[CONFIG_AUDIO_MAX_INPUT_CARD_NUM];
audio_dev_info_t g_audio_out_cards[CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM];

static unsigned int g_audio_in_card_num = 0;
static unsigned int g_audio_out_card_num = 0;

static int g_actual_audio_in_card_id = -1;
static int g_actual_audio_out_card_id = -1;

static int get_avail_audio_in_card_id(void);
static int get_avail_audio_out_card_id(void);
static audio_manager_result_t get_active_audio_device_pcm(struct pcm **pcm, audio_device_type_t type);

/****************************************************************************
 * Public Functions
 ****************************************************************************/
audio_manager_result_t find_input_audio_card(void)
{
	unsigned int card_id = -1;
	unsigned int device_id = -1;
	char type;
	struct dirent *dir_entry;
	DIR *dir_info;

	if (!(dir_info = opendir("/dev/audio/"))) {
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	while ((dir_entry = readdir(dir_info)) != NULL) {
		auddbg("Found a directory %s\n", dir_entry->d_name);
		// TODO: Add cases for various drivers. Currently, identify 'pcm' drivers only.
		if ((dir_entry->d_name[0] != 'p')
			|| (dir_entry->d_name[1] != 'c')
			|| (dir_entry->d_name[2] != 'm')
			|| (sscanf(&dir_entry->d_name[3], "C%uD%u%c", &card_id, &device_id, &type) != 3)) {
			closedir(dir_info);
			return AUDIO_MANAGER_INVALID_DEVICE_NAME;
		}

		if ((type == 'c') && (card_id < CONFIG_AUDIO_MAX_INPUT_CARD_NUM) && (device_id < MAX_AUDIO_DEVICE_NUM)) {
			g_audio_in_cards[card_id].status = AUDIO_CARD_READY;
			g_audio_in_cards[card_id].device_id = device_id;
			g_audio_in_card_num++;
			auddbg("Found an input audio card, id=%d, count=%d\n", card_id, g_audio_in_card_num);
		}
	}
	closedir(dir_info);

	if (!g_audio_in_card_num) {
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t find_output_audio_card(void)
{
	unsigned int card_id = -1;
	unsigned int device_id = -1;
	char type;
	struct dirent *dir_entry;
	DIR *dir_info;

	if (!(dir_info = opendir("/dev/audio/"))) {
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	while ((dir_entry = readdir(dir_info)) != NULL) {
		audvdbg("Found a directory %s\n", dir_entry->d_name);
		// TODO: Add cases for various drivers. Currently, identify 'pcm' drivers only.
		if ((dir_entry->d_name[0] != 'p')
			|| (dir_entry->d_name[1] != 'c')
			|| (dir_entry->d_name[2] != 'm')
			|| (sscanf(&dir_entry->d_name[3], "C%uD%u%c", &card_id, &device_id, &type) != 3)) {
			closedir(dir_info);
			return AUDIO_MANAGER_INVALID_DEVICE_NAME;
		}

		if ((type == 'p') && (card_id < CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM) && (device_id < MAX_AUDIO_DEVICE_NUM)) {
			g_audio_out_cards[card_id].status = AUDIO_CARD_READY;
			g_audio_out_cards[card_id].device_id = device_id;
			g_audio_out_card_num++;
			auddbg("Found an output audio card, id=%d, count=%d\n", card_id, g_audio_out_card_num);
		}
	}
	closedir(dir_info);

	if (!g_audio_out_card_num) {
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_audio_volume(uint16_t volume)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, OUTPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	struct audio_caps_desc_s caps_desc;
	FAR struct audio_caps_s *caps = &(caps_desc.caps);
	ret = ioctl(pcm_get_file_descriptor(pcm), AUDIOIOC_GETCAPS, (unsigned long)caps);
	if (ret < 0) {
		auddbg("Fail to ioctl AUDIOIOC_CONFIGURE, ret = %d\n", ret);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	caps->ac_controls.hw[0] = volume;

	caps->ac_type = AUDIO_TYPE_FEATURE;
	caps->ac_format.hw = AUDIO_FU_VOLUME;

	ret = ioctl(pcm_get_file_descriptor(pcm), AUDIOIOC_CONFIGURE, (unsigned long)&caps_desc);
	if (ret < 0) {
		auddbg("Fail to set a volume, ret = %d\n", ret);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	return AUDIO_MANAGER_SUCCESS;
}

int get_audio_volume(void)
{
	int ret;
	int volume = -1;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, OUTPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	ret = ioctl(pcm_get_file_descriptor(pcm), AUDIOIOC_GETVOLUME, (unsigned long)&volume);
	if (ret < 0) {
		auddbg("Fail to ioctl AUDIOIOC_GETVOLUME, ret = %d\n", ret);
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	return volume;
}

int get_input_frame_count(void)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, INPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	return pcm_get_buffer_size(pcm);
}

int get_input_frames_byte_size(int frames)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, INPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	return pcm_frames_to_bytes(pcm, frames);
}

int get_input_bytes_frame_count(unsigned int bytes)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, INPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	return pcm_bytes_to_frames(pcm, bytes);
}

int get_output_frame_count(void)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, OUTPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	return pcm_get_buffer_size(pcm);
}

int get_output_frames_byte_size(int frames)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, OUTPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	return pcm_frames_to_bytes(pcm, frames);
}

int get_output_bytes_frame_count(unsigned int bytes)
{
	int ret;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, OUTPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	return pcm_bytes_to_frames(pcm, bytes);
}

static audio_manager_result_t get_active_audio_device_pcm(struct pcm **pcm, audio_device_type_t type)
{
	if (type == INPUT) {
		if (g_actual_audio_in_card_id < 0) {
			auddbg("Found no active input audio card\n");
			return AUDIO_MANAGER_NO_AVAIL_CARD;
		}

		*pcm = g_audio_in_cards[g_actual_audio_in_card_id].pcm;
	}
	else {
		if (g_actual_audio_out_card_id < 0) {
			auddbg("Found no active output audio card\n");
			return AUDIO_MANAGER_NO_AVAIL_CARD;
		}

		*pcm = g_audio_out_cards[g_actual_audio_out_card_id].pcm;
	}

	if (!pcm_is_ready(*pcm)) {
		pcm_close(*pcm);
		auddbg("Found a null pcm\n");
		return AUDIO_MANAGER_CARD_NOT_READY;
	}

	return AUDIO_MANAGER_SUCCESS;
}

static int get_avail_audio_in_card_id()
{
	int i;
	for (i=0; i<CONFIG_AUDIO_MAX_INPUT_CARD_NUM; i++) {
		if (g_audio_in_cards[i].status == AUDIO_CARD_READY) {
			return i;
		}
	}

	audvdbg("Fail to find an avail input audio card\n");
	return AUDIO_MANAGER_NO_AVAIL_CARD;
}

static int get_avail_audio_out_card_id()
{
	int i;
	for (i=0; i<CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM; i++) {
		if (g_audio_out_cards[i].status == AUDIO_CARD_READY) {
			return i;
		}
	}

	audvdbg("Fail to find an avail output audio card\n");
	return AUDIO_MANAGER_NO_AVAIL_CARD;
}

int start_audio_stream_in(void *data, int frames)
{
	int ret;
	int retry = AUDIO_STREAM_RETRY_COUNT;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, INPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	if (g_audio_in_cards[g_actual_audio_in_card_id].status == AUDIO_CARD_PAUSE) {
		ret = ioctl(pcm_get_file_descriptor(pcm), AUDIOIOC_RESUME, NULL);
		if (ret < 0) {
			auddbg("Fail to ioctl AUDIOIOC_RESUME, ret = %d\n", ret);
			return AUDIO_MANAGER_DEVICE_FAIL;
		}

		g_audio_in_cards[g_actual_audio_in_card_id].status = AUDIO_CARD_RUNNING;
		audvdbg("Resume the input audio card!!\n");
	}

	do {
		ret = pcm_readi(pcm, data, frames);
		audvdbg("Read %d frames\n", ret);
		if (ret == -EPIPE) {
			ret = pcm_prepare(pcm);
			if (ret != OK) {
				auddbg("Fail to pcm_prepare()\n");
				return AUDIO_MANAGER_XRUN_STATE;
			}
		}
		else if (ret == -EINVAL) {
			return AUDIO_MANAGER_INVALID_PARAM;
		}
		else {
			return ret;
		}
	} while ((ret == OK) && (retry--));

	return ret;
}

int start_audio_stream_out(void *data, int frames)
{
	int ret;
	int retry = AUDIO_STREAM_RETRY_COUNT;
	struct pcm *pcm;

	if ((ret = get_active_audio_device_pcm(&pcm, OUTPUT)) != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	if (g_audio_out_cards[g_actual_audio_out_card_id].status == AUDIO_CARD_PAUSE) {
		ret = ioctl(pcm_get_file_descriptor(pcm), AUDIOIOC_RESUME, NULL);
		if (ret < 0) {
			auddbg("Fail to ioctl AUDIOIOC_RESUME, ret = %d\n", ret);
			return AUDIO_MANAGER_DEVICE_FAIL;
		}

		g_audio_out_cards[g_actual_audio_out_card_id].status = AUDIO_CARD_RUNNING;
		auddbg("Resume the output audio card!!\n");
	}

	do {
		audvdbg("Start Playing!!\n");
		ret = pcm_writei(pcm, data, frames);
		if (ret == -EPIPE) {
			ret = pcm_prepare(pcm);
			if (ret != OK) {
				auddbg("Fail to pcm_prepare()\n");
				return AUDIO_MANAGER_XRUN_STATE;
			}
		}
		else if (ret == -EINVAL) {
			return AUDIO_MANAGER_INVALID_PARAM;
		}
		else {
			return ret;
		}
	} while ((ret == OK) && (retry--));

	return ret;
}

audio_manager_result_t set_audio_stream_in(int channels, int sample_rate, int format)
{
	int card_id = -1;
	struct pcm_config config;

	if ((channels <= 0) || (sample_rate <= 0) || (format < 0)) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	if ((g_audio_in_card_num == 0) && (find_input_audio_card() != AUDIO_MANAGER_SUCCESS)) {
		auddbg("Found no active input audio card\n");
		return AUDIO_MANAGER_CARD_NOT_FOUND;
	}

	audvdbg("Print the current card id = %d\n", card_id);
	card_id = get_avail_audio_in_card_id();
	if (card_id < 0) {
		auddbg("Found no available card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	if (channels > AUDIO_STREAM_CHANNEL_MONO) {
		config.channels = AUDIO_STREAM_CHANNEL_STEREO;
	}
	config.channels = channels;
	config.rate = sample_rate;
	config.format = format;
	config.period_size = AUDIO_STREAM_VOIP_PERIOD_SIZE;
	config.period_count = AUDIO_STREAM_VOIP_PERIOD_COUNT;

	audvdbg("Print the found card id = %d\n", card_id);
	g_audio_in_cards[card_id].pcm = pcm_open(card_id, 0, PCM_IN, &config);
	if (!pcm_is_ready(g_audio_in_cards[card_id].pcm)) {
		pcm_close(g_audio_in_cards[card_id].pcm);
		auddbg("fail to pcm_is_ready()\n");
		return AUDIO_MANAGER_CARD_NOT_READY;
	}

	g_actual_audio_in_card_id = card_id;
	g_audio_in_cards[card_id].status = AUDIO_CARD_RUNNING;

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_audio_stream_out(int channels, int sample_rate, int format)
{
	int card_id = -1;
	struct pcm_config config;

	if ((channels <= 0) || (sample_rate <= 0) || (format < 0)) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	if ((g_audio_out_card_num == 0) && (find_output_audio_card() != AUDIO_MANAGER_SUCCESS)) {
		auddbg("Found no active output audio card\n");
		return AUDIO_MANAGER_CARD_NOT_FOUND;
	}

	audvdbg("Print the current card id = %d\n", card_id);
	card_id = get_avail_audio_out_card_id();
	if (card_id < 0) {
		auddbg("Found no available card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	if (channels > AUDIO_STREAM_CHANNEL_MONO) {
		config.channels = AUDIO_STREAM_CHANNEL_STEREO;
	}
	config.channels = channels;
	config.rate = sample_rate;
	config.format = format;
	config.period_size = AUDIO_STREAM_VOIP_PERIOD_SIZE;
	config.period_count = AUDIO_STREAM_VOIP_PERIOD_COUNT;

	audvdbg("Print the found card id = %d\n", card_id);
	g_audio_out_cards[card_id].pcm = pcm_open(card_id, 0, PCM_OUT, &config);
	if (!pcm_is_ready(g_audio_out_cards[card_id].pcm)) {
		pcm_close(g_audio_out_cards[card_id].pcm);
		auddbg("fail to pcm_is_ready()\n");
		return AUDIO_MANAGER_CARD_NOT_READY;
	}

	g_actual_audio_out_card_id = card_id;
	g_audio_out_cards[card_id].status = AUDIO_CARD_RUNNING;

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
		auddbg("Fail to ioctl AUDIOIOC_PAUSE, ret = %d\n", ret);
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
		auddbg("Fail to ioctl AUDIOIOC_PAUSE, ret = %d\n", ret);
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
		auddbg("Fail to ioctl AUDIOIOC_STOP, ret = %d\n", ret);
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
		auddbg("Fail to ioctl AUDIOIOC_STOP, ret = %d\n", ret);
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

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t destroy_audio_stream_in(void)
{
	if (g_actual_audio_in_card_id < 0) {
		auddbg("Found no active input audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	audvdbg("Clean all of audio card %d\n", g_actual_audio_in_card_id);
	if (g_audio_in_cards[g_actual_audio_in_card_id].status == AUDIO_CARD_RUNNING) {
		g_audio_in_cards[g_actual_audio_in_card_id].status = AUDIO_CARD_READY;
		g_audio_in_cards[g_actual_audio_in_card_id].device_id = -1;
		g_audio_in_cards[g_actual_audio_in_card_id].config.volume = 0;
		g_audio_in_cards[g_actual_audio_in_card_id].pcm = NULL;
		g_actual_audio_in_card_id = -1;
	}

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t destroy_audio_stream_out(void)
{
	if (g_actual_audio_out_card_id < 0) {
		auddbg("Found no active output audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	audvdbg("Clean all of audio card %d\n", g_actual_audio_out_card_id);
	if (g_audio_out_cards[g_actual_audio_out_card_id].status == AUDIO_CARD_RUNNING) {
		g_audio_out_cards[g_actual_audio_out_card_id].status = AUDIO_CARD_READY;
		g_audio_out_cards[g_actual_audio_out_card_id].device_id = -1;
		g_audio_out_cards[g_actual_audio_out_card_id].config.volume = 0;
		g_audio_out_cards[g_actual_audio_out_card_id].pcm = NULL;
		g_actual_audio_out_card_id = -1;
	}

	return AUDIO_MANAGER_SUCCESS;
}
