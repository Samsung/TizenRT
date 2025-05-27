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
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <tinyara/audio/audio.h>
#include <tinyalsa/tinyalsa.h>
#include <json/cJSON.h>

#include "audio_manager.h"
#include "resample/speex_resampler.h"
#include "../utils/remix.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifndef CONFIG_AUDIO_DEV_PATH
#define CONFIG_AUDIO_DEV_PATH "/dev/audio"
#endif

#define AUDIO_DEV_PATH_LENGTH 11
#define AUDIO_PCM_PATH_LENGTH 9	//length of pcmC%uD%u%c + 1;
#define AUDIO_DEVICE_FULL_PATH_LENGTH (AUDIO_DEV_PATH_LENGTH + AUDIO_PCM_PATH_LENGTH)

#define AUDIO_DEVICE_PROCESS_QUEUE_PATH "/mqueue"
#define AUDIO_DEVICE_PROCESS_QUEUE_PATH_STRING_LENGTH 7 /* strlen(AUDIO_DEVICE_PROCESS_QUEUE_PATH) is 7*/
#define AUDIO_DEVICE_PROCESS_QUEUE_PATH_LENGTH AUDIO_DEVICE_FULL_PATH_LENGTH + AUDIO_DEVICE_PROCESS_QUEUE_PATH_STRING_LENGTH

#define AUDIO_STREAM_BUFFER_SHORT_PERIOD 1024
#define AUDIO_STREAM_BUFFER_LONG_PERIOD 2048

#define AUDIO_STREAM_BUFFER_SHORT_PERIOD_COUNT 4
#define AUDIO_STREAM_BUFFER_LONG_PERIOD_COUNT 8

#define AUDIO_STREAM_CHANNEL_MONO 1
#define AUDIO_STREAM_CHANNEL_STEREO 2

#define AUDIO_STREAM_MEDIA_PERIOD_SIZE AUDIO_STREAM_BUFFER_LONG_PERIOD
#define AUDIO_STREAM_MEDIA_PERIOD_COUNT 8
#define AUDIO_STREAM_MEDIA_SAMPLE_RATE AUDIO_SAMP_RATE_44K
#define AUDIO_STREAM_MEDIA_CHANNEL AUDIO_STREAM_CHANNEL_STEREO

#define AUDIO_STREAM_VOIP_PERIOD_SIZE AUDIO_STREAM_BUFFER_LONG_PERIOD
#define AUDIO_STREAM_VOIP_PERIOD_COUNT AUDIO_STREAM_BUFFER_LONG_PERIOD_COUNT
#define AUDIO_STREAM_VOIP_SAMPLE_RATE AUDIO_SAMP_RATE_16K
#define AUDIO_STREAM_VOIP_CHANNEL AUDIO_STREAM_CHANNEL_STEREO

#define AUDIO_STREAM_VOICE_RECOGNITION_PERIOD_SIZE AUDIO_STREAM_BUFFER_SHORT_PERIOD
#define AUDIO_STREAM_VOICE_RECOGNITION_PERIOD_COUNT AUDIO_STREAM_BUFFER_SHORT_PERIOD_COUNT
#define AUDIO_STREAM_VOICE_RECOGNITION_SAMPLE_RATE AUDIO_SAMP_RATE_16K
#define AUDIO_STREAM_VOICE_RECOGNITION_CHANNEL AUDIO_STREAM_CHANNEL_STEREO

#define AUDIO_STREAM_RETRY_COUNT 2

#define AUDIO_DEVICE_MAX_VOLUME 15

#ifndef CONFIG_AUDIO_MAX_INPUT_CARD_NUM
#define CONFIG_AUDIO_MAX_INPUT_CARD_NUM 2
#endif

#ifndef CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM
#define CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM 2
#endif

#ifndef CONFIG_AUDIO_MAX_DEVICE_NUM
#define CONFIG_AUDIO_MAX_DEVICE_NUM 3
#endif

#ifndef CONFIG_AUDIO_RESAMPLER_BUFSIZE
#define CONFIG_AUDIO_RESAMPLER_BUFSIZE 4096
#endif

#ifndef CONFIG_PROCESS_MSG_TIMEOUT_MSEC
#define CONFIG_PROCESS_MSG_TIMEOUT_MSEC 150
#endif

#define INVALID_ID -1

#define AUDIO_METADATA_JSON_PATH "/mnt/audio_metadata.json"

#define STREAM_TYPE_MUTE_STATUS_KEY "STREAM_TYPE_MUTE_STATUS"

#define CJSON_GETOBJECTITEM(jsonValue, jsonObject, jsonKey)	\
	{	\
		jsonValue = cJSON_GetObjectItem(jsonObject, jsonKey);	\
		if (!jsonValue) {	\
			meddbg("%s: No such key %s in JSON\n", __FUNCTION__, jsonKey);	\
			return AUDIO_MANAGER_OPERATION_FAIL;	\
		}	\
	}	\

#define RESAMPLING_QUALITY 5 // Resampling quality between 0 and 10, where 0 has poor quality and 10 has very high quality.
#define MAX_RESAMPLING_QUALITY 10

/****************************************************************************
 * Private Types
 ****************************************************************************/
enum audio_card_status_e {
	AUDIO_CARD_NONE = 0,		// card not found yet, shouldn't be assigned explicitly.
	AUDIO_CARD_IDLE = 1,
	AUDIO_CARD_READY,
	AUDIO_CARD_RUNNING,
	AUDIO_CARD_PAUSE
};

enum audio_io_direction_e {
	INPUT = 0,
	OUTPUT = 1
};

struct audio_device_config_s {
	enum audio_card_status_e status;
	uint8_t volume;
	uint8_t max_volume;
	audio_device_type_t device_type;
	device_process_type_t device_process_type;
	mqd_t process_handler;
	uint8_t handler_refcnt;
};

struct audio_resample_s {
	bool necessary;				// if resampling/rechanneling is needed (format-converting is not supported now)
	void *buffer;				// pointer to the buffer used for resampling
	uint32_t buffer_size;			// size in bytes of the buffer
	uint32_t frames;			// number of frames in the buffer
	float ratio;				// sample rate converting ratio
	SpeexResamplerState *speex_resampler;	// handle of speex resampler
	void *rechannel_buffer;			// pointer to the buffer used for rechanneling
	uint32_t rechannel_buffer_size;		// size of the rechannel buffer in bytes
	/* user provided/desired */
	uint32_t user_sample_rate;		// sample rate from a user
	uint32_t user_channel;			// channel info from a user
	uint8_t user_format;			// bytes per sample of user format
	/* card supported */
	uint8_t samprate_types;			// sample rate types supported by card
};

// ToDo: Exapnd audio card structure to handle volume level
struct audio_card_info_s {
	uint8_t card_id;			//current card id
	uint8_t device_id;			//current device id
	struct audio_device_config_s config[CONFIG_AUDIO_MAX_DEVICE_NUM];
	struct pcm *pcm;
	stream_policy_t policy;
	stream_info_id_t stream_id;
	struct audio_resample_s resample;
	pthread_mutex_t card_mutex;
	uint8_t volume[MAX_STREAM_POLICY_NUM];
};

struct audio_samprate_map_entry_s {
	uint8_t samprate_types;
	unsigned int samprate;
};

typedef enum audio_card_status_e audio_card_status_t;
typedef enum audio_io_direction_e audio_io_direction_t;
typedef struct audio_device_config_s audio_config_t;
typedef struct audio_card_info_s audio_card_info_t;

static audio_card_info_t g_audio_in_cards[CONFIG_AUDIO_MAX_INPUT_CARD_NUM];
static audio_card_info_t g_audio_out_cards[CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM];

static int g_actual_audio_in_card_id = INVALID_ID;
static int g_actual_audio_out_card_id = INVALID_ID;

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

static const uint8_t g_audio_stream_volume_entry[7][16] = {
	{0, 66, 69, 72, 75, 78, 81, 84, 86, 88, 90, 92, 94, 96, 98, 100}, //STREAM_TYPE_BASE
	{0, 54, 58, 65, 70, 73, 76, 79, 81, 84, 86, 88, 90, 92, 94, 97}, //STREAM_TYPE_MEDIA
	{0, 66, 69, 72, 75, 78, 81, 84, 86, 88, 90, 92, 94, 96, 98, 100}, //STREAM_TYPE_NOTIFY
	{0, 66, 69, 72, 75, 78, 81, 84, 86, 88, 90, 92, 94, 96, 98, 100}, //STREAM_TYPE_BIXBY
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0}, //STREAM_TYPE_RECORDER, not available for now
	{0, 54, 64, 69, 74, 79, 83, 88, 88, 88, 88, 88, 88, 88, 88, 88}, //STREAM_TYPE_VOIP
	{0, 66, 69, 72, 75, 78, 81, 84, 86, 88, 90, 92, 94, 96, 98, 100}, //STREAM_TYPE_EMERGENCY
};

static cJSON *gJSON = NULL;
static uint8_t gDefaultVolumeLevel = 0;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static audio_manager_result_t find_audio_card(audio_io_direction_t direct);
static audio_manager_result_t get_actual_audio_in_card_id(void);
static audio_manager_result_t get_actual_audio_out_card_id(void);
static audio_manager_result_t get_supported_capability(audio_io_direction_t direct, unsigned int *channel);
static audio_manager_result_t get_supported_process_type(int card_id, int device_id, audio_io_direction_t direct);
static uint32_t get_closest_samprate(unsigned origin_samprate, audio_io_direction_t direct);
static unsigned int resample_stream_in(audio_card_info_t *card, void *data, unsigned int frames);
static unsigned int resample_stream_out(audio_card_info_t *card, void *data, unsigned int frames);
static audio_manager_result_t get_audio_volume(audio_io_direction_t direct);
static audio_manager_result_t set_audio_volume(audio_io_direction_t direct, uint8_t volume);
static audio_manager_result_t set_audio_equalizer(audio_io_direction_t direct, uint32_t preset);
static audio_manager_result_t set_audio_mute(audio_io_direction_t direct, stream_policy_t stream_policy, bool mute);
static audio_manager_result_t create_audio_metadata_json(void);
static audio_manager_result_t parse_audio_metadata_json(void);
static audio_manager_result_t update_audio_metadata_json(void);
static audio_manager_result_t verify_audio_metadata_json(void);
static const char *getJSONKey(stream_policy_t stream_policy);
static inline audio_manager_result_t validate_stream_policy(stream_policy_t stream_policy);

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void get_card_path(char *card_path, uint8_t card_id, uint8_t device_id, audio_io_direction_t direct)
{
	char type_chr = (direct == INPUT) ? 'c' : 'p';

	snprintf(card_path, (AUDIO_DEVICE_FULL_PATH_LENGTH), "%s/pcmC%uD%u%c", CONFIG_AUDIO_DEV_PATH, card_id, device_id, type_chr);
	medvdbg("card_path : %s\n", card_path);
}

/* TODO Think we can remove direct option here...*/
static audio_manager_result_t find_audio_card(audio_io_direction_t direct)
{
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	int card_id = INVALID_ID;
	int device_id = INVALID_ID;
	char type;
	struct dirent *dir_entry;
	DIR *dir_info;
	char type_chr;
	audio_card_info_t *card;
	int max_card_num;
	int found_cards = 0;

	if (!(dir_info = opendir(CONFIG_AUDIO_DEV_PATH))) {
		return AUDIO_MANAGER_DEVICE_FAIL;
	}

	if (direct == INPUT) {
		type_chr = 'c';
		card = g_audio_in_cards;
		max_card_num = CONFIG_AUDIO_MAX_INPUT_CARD_NUM;
	} else {
		type_chr = 'p';
		card = g_audio_out_cards;
		max_card_num = CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM;
	}

	while ((dir_entry = readdir(dir_info)) != NULL) {
		if ((dir_entry->d_name[0] != 'p') || (dir_entry->d_name[1] != 'c') || (dir_entry->d_name[2] != 'm') || (sscanf(&dir_entry->d_name[3], "C%uD%u%c", &card_id, &device_id, &type) != 3)) {
			continue;
		}

		/* Find next card if driver name isn't follow naming policy */
		if ((card_id < 0) || (card_id >= max_card_num) || (device_id < 0) || (device_id >= CONFIG_AUDIO_MAX_DEVICE_NUM)) {
			meddbg("Found wrong card card_id : %d device_id : %d\n", card_id, device_id);
			continue;
		}

		if (type == type_chr) {
			pthread_mutex_init(&(card[card_id].card_mutex), NULL);
			pthread_mutex_lock(&(card[card_id].card_mutex));
			card[card_id].config[device_id].status = AUDIO_CARD_IDLE;
			card[card_id].card_id = card_id;
			card[card_id].device_id = device_id;
			found_cards++;
			medvdbg("Found an audio card, total card : %d id : %d device : %d\n", found_cards, card_id, device_id);
			pthread_mutex_unlock(&(card[card_id].card_mutex));
		}
	}

	/* If nothing founded */
	if (found_cards == 0) {
		meddbg("Failed to find card\n");
		ret = AUDIO_MANAGER_NO_AVAIL_CARD;
		goto error_out;
	}

	if (direct == INPUT) {
		if (get_actual_audio_in_card_id() != AUDIO_MANAGER_SUCCESS) {
			ret = AUDIO_MANAGER_NO_AVAIL_CARD;
		}
	} else {
		if (get_actual_audio_out_card_id() != AUDIO_MANAGER_SUCCESS) {
			ret = AUDIO_MANAGER_NO_AVAIL_CARD;
		}
	}
#ifdef CONFIG_DEBUG_MEDIA_INFO
	dump_audio_card_info();
#endif

error_out:
	closedir(dir_info);
	return ret;
}

static audio_manager_result_t control_audio_stream_device(const char *path, int cmd, unsigned long arg)
{
	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		meddbg("Failed to open device path : %s errno : %d\n", path, get_errno());
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	int ret = ioctl(fd, cmd, arg);
	if (ret < 0) {
		meddbg("Fail to ioctl(%d, %d, %lu), errno : %d\n", fd, cmd, arg, get_errno());
		ret = AUDIO_MANAGER_DEVICE_FAIL;
	} else {
		ret = AUDIO_MANAGER_SUCCESS;
	}

	close(fd);
	return static_cast<audio_manager_result_t>(ret);
}

static audio_manager_result_t get_supported_capability(audio_io_direction_t direct, unsigned int *channel)
{
	struct audio_caps_desc_s caps_desc;
	audio_card_info_t *card;
	char card_path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	audio_manager_result_t ret;

	if (direct == INPUT) {
		card = &g_audio_in_cards[g_actual_audio_in_card_id];
		caps_desc.caps.ac_type = AUDIO_TYPE_INPUT;
	} else {
		card = &g_audio_out_cards[g_actual_audio_out_card_id];
		caps_desc.caps.ac_type = AUDIO_TYPE_OUTPUT;
	}

	caps_desc.caps.ac_subtype = AUDIO_TYPE_QUERY;
	caps_desc.caps.ac_len = sizeof(struct audio_caps_s);
	get_card_path(card_path, card->card_id, card->device_id, direct);

	pthread_mutex_lock(&(card->card_mutex));

	ret = control_audio_stream_device(card_path, AUDIOIOC_GETCAPS, (unsigned long)&caps_desc.caps);
	if (ret == AUDIO_MANAGER_SUCCESS) {
		*channel = caps_desc.caps.ac_channels;
		card->resample.samprate_types = caps_desc.caps.ac_controls.b[0];
		medvdbg("Samprate_type : %d,  Channel_num : %d\n\n", card->resample.samprate_types, *channel);
	}

	pthread_mutex_unlock(&(card->card_mutex));
	return ret;
}

/* TODO this function should consider priority of each function in future */
static audio_manager_result_t get_actual_audio_in_card_id()
{
	int i, j;
	int cnt = 0;

	for (j = 0; j < CONFIG_AUDIO_MAX_DEVICE_NUM; j++) {
		for (i = 0; i < CONFIG_AUDIO_MAX_INPUT_CARD_NUM; i++) {
			if (g_audio_in_cards[i].config[j].status != AUDIO_CARD_NONE) {
				if (g_audio_in_cards[i].config[j].status == AUDIO_CARD_IDLE) {
					cnt++;
				}
			}
		}
	}
	/* Return basic one If we found device & all devices in each card are IDLE */
	if (cnt > 0) {
		g_actual_audio_in_card_id = 0;
		g_audio_in_cards[0].card_id = 0;
		g_audio_in_cards[0].device_id = 0;
		return AUDIO_MANAGER_SUCCESS;
	}
	return AUDIO_MANAGER_NO_AVAIL_CARD;
}

static audio_manager_result_t get_actual_audio_out_card_id()
{
	int i, j;
	int cnt = 0;

	for (j = 0; j < CONFIG_AUDIO_MAX_DEVICE_NUM; j++) {
		for (i = 0; i < CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM; i++) {
			if (g_audio_out_cards[i].config[j].status != AUDIO_CARD_NONE) {
				if (g_audio_out_cards[i].config[j].status == AUDIO_CARD_IDLE) {
					cnt++;
				}
			}
		}
	}
	/* Return basic one If we found device & all devices in each card are IDLE */
	if (cnt > 0) {
		g_actual_audio_out_card_id = 0;
		g_audio_out_cards[0].card_id = 0;
		g_audio_out_cards[0].device_id = 0;
		return AUDIO_MANAGER_SUCCESS;
	}
	return AUDIO_MANAGER_NO_AVAIL_CARD;
}

static uint32_t get_closest_samprate(unsigned int origin_samprate, audio_io_direction_t direct)
{
	int i;
	uint32_t result = 0;
	int count = sizeof(g_audio_samprate_entry) / sizeof(struct audio_samprate_map_entry_s);
	audio_card_info_t *card;

	ASSERT(count > 0);

	if (direct == INPUT) {
		ASSERT(g_actual_audio_in_card_id >= 0);
		card = &g_audio_in_cards[g_actual_audio_in_card_id];
	} else {
		ASSERT(g_actual_audio_out_card_id >= 0);
		card = &g_audio_out_cards[g_actual_audio_out_card_id];
	}

	for (i = 0; i < count; i++) {
		if (g_audio_samprate_entry[i].samprate_types & card->resample.samprate_types) {
			result = g_audio_samprate_entry[i].samprate;
			if (g_audio_samprate_entry[i].samprate >= origin_samprate) {
				break;
			}
		}
	}

	return result;
}

/*
 * card: Pointer to audio card information structure
 *       card->resample.buffer points to the input frames read from card,
 *       card->resample.frames gives the number of frames in above buffer.
 * data: Pointer to the output buffer to save generated frames.
 * frames: Capability in frames of the output buffer.
 * return: On success, returns number of frames generated in the output buffer.
 *         Otherwise, returns negative error codes on failure.
 */
static unsigned int resample_stream_in(audio_card_info_t *card, void *data, unsigned int frames)
{
	unsigned int used_frames = 0;
	unsigned int resampled_frames = 0;
	unsigned int rechanneled_frames;
	unsigned int original_channel_num;
	unsigned int original_sample_rate;
	spx_int16_t *data_in;
	spx_uint32_t input_frames;
	spx_int16_t *data_out;
	spx_uint32_t output_frames;
	int ret;

	original_channel_num = pcm_get_channels(card->pcm);
	original_sample_rate = pcm_get_rate(card->pcm);
	if (card->resample.user_sample_rate == original_sample_rate) {
		// Only rechanneling is required.
		rechanneled_frames = rechannel(ch2layout(original_channel_num), ch2layout(card->resample.user_channel),
						(const int16_t *)card->resample.buffer, card->resample.frames,
						(int16_t *)data, frames);
		if (rechanneled_frames != card->resample.frames) {
			meddbg("Failed to rechannel each frame, %u/%u\n", rechanneled_frames, card->resample.frames);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}
		return rechanneled_frames;
	}

	// Rechannel/Copy frames in resample buffer to rechannel buffer
	rechanneled_frames = rechannel(ch2layout(original_channel_num), ch2layout(card->resample.user_channel),
					(const int16_t *)card->resample.buffer, card->resample.frames,
					(int16_t *)card->resample.rechannel_buffer, get_user_input_bytes_to_frame(card->resample.rechannel_buffer_size));
	if (rechanneled_frames != card->resample.frames) {
		meddbg("Fail to rechannel each frame, %u/%u\n", rechanneled_frames, card->resample.frames);
		return AUDIO_MANAGER_RESAMPLE_FAIL;
	}

	while (card->resample.frames > used_frames) {
		data_in = (spx_int16_t *)((char *)(card->resample.rechannel_buffer) + get_user_input_frames_to_byte(used_frames));
		input_frames = card->resample.frames - used_frames;
		data_out = (spx_int16_t *)((char *)data + get_user_input_frames_to_byte(resampled_frames));
		output_frames = frames - resampled_frames; // set to maximum frames given output buffer can hold.
		medvdbg("data_in 0x%x, input_frames %d\n", data_in, input_frames);
		medvdbg("data_out 0x%x, output_frames resample buffer can hold %d\n", data_out, output_frames);

		ret = speex_resampler_process_interleaved_int(card->resample.speex_resampler, data_in, &input_frames, data_out, &output_frames);
		if (ret != RESAMPLER_ERR_SUCCESS) {
			meddbg("Fail to resample in:%u/%u, error %d\n", used_frames, card->resample.frames, ret);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}

		used_frames += input_frames;
		if (output_frames > 0) {
			resampled_frames += output_frames;
		} else if (card->resample.frames != used_frames) {
			meddbg("Error: output buffer is full, used input frames %d/%d\n", used_frames, card->resample.frames);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}
		medvdbg("%d frames generated from %d/%d\n", resampled_frames, used_frames, card->resample.frames);
	}

	medvdbg("resampled frames count: %u\n", resampled_frames);
	return resampled_frames;
}

/*
 * card: Pointer to audio card information structure
 *       card->resample.buffer retrieves generated frames for output,
 *       card->resample.frames returns the number of frames saved in above buffer.
 * data: Pointer to the input buffer contains frames to resample.
 * frames: Gives the number of frames in the input buffer
 * return: On success, returns number of frames generated in resample.buffer,
 *         besides, card->resample.frames retrieves the same value.
 *         Otherwise, returns negative error codes on failure.
 */
static unsigned int resample_stream_out(audio_card_info_t *card, void *data, unsigned int frames)
{
	unsigned int used_frames = 0;
	unsigned int resampled_frames = 0;
	unsigned int rechanneled_frames;
	unsigned int desired_channel_num;
	unsigned int desired_sample_rate;
	spx_int16_t *data_in;
	spx_uint32_t input_frames;
	spx_int16_t *data_out;
	spx_uint32_t output_frames;
	int ret;

	desired_channel_num = pcm_get_channels(card->pcm);
	desired_sample_rate = pcm_get_rate(card->pcm);
	if (desired_sample_rate == card->resample.user_sample_rate) {
		// Only rechanneling is required.
		rechanneled_frames = rechannel(ch2layout(card->resample.user_channel), ch2layout(desired_channel_num), (const int16_t *)data, frames,
						(int16_t *)card->resample.buffer, get_card_output_bytes_to_frame(card->resample.buffer_size));
		if (rechanneled_frames != frames) {
			meddbg("Failed to rechannel each frame, %u/%u\n", rechanneled_frames, frames);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}
		card->resample.frames = rechanneled_frames;
		return rechanneled_frames;
	}

	// Rechannel/Copy input frames to rechannel buffer
	rechanneled_frames = rechannel(ch2layout(card->resample.user_channel), ch2layout(desired_channel_num), (const int16_t *)data, frames,
					(int16_t *)card->resample.rechannel_buffer, get_card_output_bytes_to_frame(card->resample.rechannel_buffer_size));
	if (rechanneled_frames != frames) {
		meddbg("Fail to rechannel each frame, %u/%u\n", rechanneled_frames, frames);
		return AUDIO_MANAGER_RESAMPLE_FAIL;
	}

	while (frames > used_frames) {
		data_in = (spx_int16_t *)((char *)(card->resample.rechannel_buffer) + get_card_output_frames_to_byte(used_frames));
		input_frames = frames - used_frames;
		data_out = (spx_int16_t *)((char *)card->resample.buffer + get_card_output_frames_to_byte(resampled_frames));
		output_frames = get_card_output_bytes_to_frame(card->resample.buffer_size) - resampled_frames; // set to maximum frames resample buffer can hold.
		medvdbg("data_in 0x%x, input_frames %d\n", data_in, input_frames);
		medvdbg("data_out 0x%x, output_frames resample buffer can hold %d\n", data_out, output_frames);

		ret = speex_resampler_process_interleaved_int(card->resample.speex_resampler, data_in, &input_frames, data_out, &output_frames);
		if (ret != RESAMPLER_ERR_SUCCESS) {
			meddbg("Fail to resample in:%u/%u, error %d\n", used_frames, frames, ret);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}

		used_frames += input_frames;
		if (output_frames > 0) {
			resampled_frames += output_frames;
		} else if (frames != used_frames) {
			meddbg("Error: output buffer is full, used input frames %d/%d\n", used_frames, frames);
			return AUDIO_MANAGER_RESAMPLE_FAIL;
		}
		medvdbg("%d frames generated from %d/%d\n", resampled_frames, used_frames, frames);
	}

	medvdbg("resampled frames count: %u\n", resampled_frames);
	card->resample.frames = resampled_frames;
	return resampled_frames;
}

static audio_manager_result_t get_audio_volume(audio_io_direction_t direct)
{
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	struct audio_caps_desc_s caps_desc;
	uint8_t max_volume;
	uint8_t cur_volume;
	pthread_mutex_t *card_mutex;
	audio_card_info_t *card;
	audio_config_t *config;
	char card_path[AUDIO_DEVICE_FULL_PATH_LENGTH];

	caps_desc.caps.ac_len = sizeof(struct audio_caps_s);
	caps_desc.caps.ac_type = AUDIO_TYPE_FEATURE;

	if (direct == INPUT) {
		caps_desc.caps.ac_subtype = AUDIO_FU_INP_GAIN;
		card = &g_audio_in_cards[g_actual_audio_in_card_id];
		card_mutex = &g_audio_in_cards[g_actual_audio_in_card_id].card_mutex;
	} else {
		caps_desc.caps.ac_subtype = AUDIO_FU_VOLUME;
		card = &g_audio_out_cards[g_actual_audio_out_card_id];
		card_mutex = &g_audio_out_cards[g_actual_audio_out_card_id].card_mutex;
	}
	get_card_path(card_path, card->card_id, card->device_id, direct);

	pthread_mutex_lock(card_mutex);

	ret = control_audio_stream_device(card_path, AUDIOIOC_GETCAPS, (unsigned long)&caps_desc.caps);
	if (ret == AUDIO_MANAGER_SUCCESS) {
		max_volume = caps_desc.caps.ac_controls.hw[0];
		cur_volume = caps_desc.caps.ac_controls.hw[1];

		/* scale here */
		medvdbg("Device Max_vol = %d,  cur_vol = %d\n", max_volume, cur_volume);
#if 0
		cur_volume = cur_volume * AUDIO_DEVICE_MAX_VOLUME / (max_volume - (max_volume % AUDIO_DEVICE_MAX_VOLUME));
		if (cur_volume > AUDIO_DEVICE_MAX_VOLUME) {
			cur_volume = AUDIO_DEVICE_MAX_VOLUME;
		}
#endif
		int i;
		for (i = 0; i <= AUDIO_DEVICE_MAX_VOLUME; i++) {
			if (g_audio_stream_volume_entry[card->policy][i] == cur_volume) {
				break;
			}
				
		}
		config = &card->config[card->device_id];
		config->max_volume = max_volume;
		config->volume = i;
		medvdbg("Max_vol = %d,  cur_vol = %d\n", config->max_volume, config->volume);
	}

	pthread_mutex_unlock(card_mutex);
	return ret;
}

static audio_manager_result_t set_audio_volume(audio_io_direction_t direct, uint8_t volume)
{
	audio_manager_result_t ret;
	struct audio_caps_desc_s caps_desc;
	audio_card_info_t *card;
	audio_config_t *config;
	char card_path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	pthread_mutex_t *card_mutex;

	if (volume > AUDIO_DEVICE_MAX_VOLUME) {
		volume = AUDIO_DEVICE_MAX_VOLUME;
	}

	/* get system volume before set */
	ret = get_audio_volume(direct);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	if (direct == INPUT) {
		caps_desc.caps.ac_format.hw = AUDIO_FU_INP_GAIN;
		card = &g_audio_in_cards[g_actual_audio_in_card_id];
		card_mutex = &g_audio_in_cards[g_actual_audio_in_card_id].card_mutex;
	} else {
		caps_desc.caps.ac_format.hw = AUDIO_FU_VOLUME;
		card = &g_audio_out_cards[g_actual_audio_out_card_id];
		card_mutex = &g_audio_out_cards[g_actual_audio_out_card_id].card_mutex;
	}

	config = &card->config[card->device_id];
	if (volume == config->volume) {
		medvdbg("Volume already set to %d\n", volume);
		return AUDIO_MANAGER_SUCCESS;
	}
	caps_desc.caps.ac_controls.hw[0] = g_audio_stream_volume_entry[card->policy][volume];//volume * (config->max_volume / AUDIO_DEVICE_MAX_VOLUME);
	medvdbg("streaminfo :  %d volume : %d value : %d\n", card->policy, volume, g_audio_stream_volume_entry[card->policy][volume]);
	caps_desc.caps.ac_len = sizeof(struct audio_caps_s);
	caps_desc.caps.ac_type = AUDIO_TYPE_FEATURE;

	get_card_path(card_path, card->card_id, card->device_id, direct);

	pthread_mutex_lock(card_mutex);

	ret = control_audio_stream_device(card_path, AUDIOIOC_CONFIGURE, (unsigned long)&caps_desc);
	if (ret == AUDIO_MANAGER_SUCCESS) {
		config->volume = volume;
		medvdbg("Volume = %d (%d)\n", volume, caps_desc.caps.ac_controls.hw[0]);
	} else {
		meddbg("Fail to set a volume, ret = %d errno : %d\n", ret, get_errno());
		if (get_errno() == EACCES) {
			ret = AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
		}
	}

	pthread_mutex_unlock(card_mutex);
	return ret;
}

//ToDo: In the future, driver will be changed to load the script that exists in each product app.
static audio_manager_result_t set_audio_equalizer(audio_io_direction_t direct, uint32_t preset)
{
	audio_manager_result_t ret;
	struct audio_caps_desc_s caps_desc;
	audio_card_info_t *card;
	char card_path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	pthread_mutex_t *card_mutex;

	if (direct == INPUT) {
		caps_desc.caps.ac_format.hw = AUDIO_FU_EQUALIZER;
		card = &g_audio_in_cards[g_actual_audio_in_card_id];
		card_mutex = &g_audio_in_cards[g_actual_audio_in_card_id].card_mutex;
	} else {
		caps_desc.caps.ac_format.hw = AUDIO_FU_EQUALIZER;
		card = &g_audio_out_cards[g_actual_audio_out_card_id];
		card_mutex = &g_audio_out_cards[g_actual_audio_out_card_id].card_mutex;
	}

	caps_desc.caps.ac_controls.w = preset;
	medvdbg("preset :  %d\n", preset);
	caps_desc.caps.ac_len = sizeof(struct audio_caps_s);
	caps_desc.caps.ac_type = AUDIO_TYPE_FEATURE;

	get_card_path(card_path, card->card_id, card->device_id, direct);

	pthread_mutex_lock(card_mutex);

	ret = control_audio_stream_device(card_path, AUDIOIOC_CONFIGURE, (unsigned long)&caps_desc);
	if (ret == AUDIO_MANAGER_SUCCESS) {
		medvdbg("Successfully set equalizer\n");
	} else {
		meddbg("Fail to set equalizer, ret = %d errno : %d\n", ret, get_errno());
		if (get_errno() == EACCES) {
			ret = AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
		}
	}

	pthread_mutex_unlock(card_mutex);
	return ret;
}

static audio_manager_result_t set_audio_mute(audio_io_direction_t direct, stream_policy_t stream_policy, bool mute)
{
	audio_manager_result_t ret;
	struct audio_caps_desc_s caps_desc;
	audio_card_info_t *card;
	char card_path[AUDIO_DEVICE_FULL_PATH_LENGTH];

	if (direct == INPUT) {
		if (g_actual_audio_in_card_id < 0) {
			meddbg("Found no active input audio card\n");
			return AUDIO_MANAGER_NO_AVAIL_CARD;
		}
		card = &g_audio_in_cards[g_actual_audio_in_card_id];
	} else {
		if (g_actual_audio_out_card_id < 0) {
			meddbg("Found no active output audio card\n");
			return AUDIO_MANAGER_NO_AVAIL_CARD;
		}
		card = &g_audio_out_cards[g_actual_audio_out_card_id];
		if (stream_policy != card->policy) {
			meddbg("Policy mismatch, given_policy: %d, card_policy: %d. Mute state for given policy will be set when it will play\n", stream_policy, card->policy);
			return AUDIO_MANAGER_SUCCESS;
		}
	}

	caps_desc.caps.ac_type = AUDIO_TYPE_FEATURE;
	caps_desc.caps.ac_format.hw = AUDIO_FU_MUTE;
	caps_desc.caps.ac_controls.b[0] = mute;

	get_card_path(card_path, card->card_id, card->device_id, direct);

	pthread_mutex_lock(&(card->card_mutex));

	ret = control_audio_stream_device(card_path, AUDIOIOC_CONFIGURE, (unsigned long)&caps_desc);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Fail to mute audio, ret = %d errno : %d\n", ret, get_errno());
		if (get_errno() == EACCES) {
			ret = AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
		}
		pthread_mutex_unlock(&(card->card_mutex));
		return ret;
	}
	medvdbg("Audio Mute Success\n");

	pthread_mutex_unlock(&(card->card_mutex));
	return ret;
}

audio_manager_result_t create_audio_metadata_json(void)
{
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	int fd = open(AUDIO_METADATA_JSON_PATH, O_WRONLY | O_CREAT, 0777);
	if (fd == -1) {
		meddbg("Failed to open volume level json file. errno: %d\n", errno);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	cJSON *json = cJSON_CreateObject();
	if (!json) {
		meddbg("Failed to create volume level json object\n");
		close(fd);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	cJSON *muteStatusJsonObj = cJSON_CreateObject();
	if (!muteStatusJsonObj) {
		meddbg("Failed to create json object for stream mute status\n");
		cJSON_Delete(json);
		close(fd);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	const char *jsonKey;
	for (uint8_t i = 0; i < MAX_STREAM_POLICY_NUM; i++) {
		jsonKey = getJSONKey((stream_policy_t)i);
		cJSON_AddNumberToObject(json, jsonKey, gDefaultVolumeLevel);
		cJSON_AddBoolToObject(muteStatusJsonObj, jsonKey, false);
	}
	cJSON_AddItemToObject(json, STREAM_TYPE_MUTE_STATUS_KEY, muteStatusJsonObj);
	char *jsonString = cJSON_Print(json);
	if (!jsonString) {
		meddbg("Failed to print volume level json object\n");
		cJSON_Delete(muteStatusJsonObj);
		cJSON_Delete(json);
		close(fd);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	ssize_t bytesWritten = write(fd, jsonString, strlen(jsonString));
	if (bytesWritten == -1) {
		meddbg("Failed to write volume level json file. errno: %d\n", errno);
		ret = AUDIO_MANAGER_OPERATION_FAIL;
	} else {
		medvdbg("To be written JSON string: %s\n, bytes written: %zd\n", jsonString, bytesWritten);
	}
	free(jsonString);
	cJSON_Delete(muteStatusJsonObj);
	cJSON_Delete(json);
	close(fd);
	return ret;
}

audio_manager_result_t parse_audio_metadata_json(void)
{
	struct stat jsonFileStat;
	if (stat(AUDIO_METADATA_JSON_PATH, &jsonFileStat) != 0) {
		meddbg("Failed to fetch volume level json file information. errno: %d\n", errno);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	char *buffer = (char *)malloc(jsonFileStat.st_size + 1);
	if (!buffer) {
		meddbg("Failed to allocate memory to hold volume level json file content\n");
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	int fd = open(AUDIO_METADATA_JSON_PATH, O_RDONLY);
	if (fd == -1) {
		meddbg("Failed to open volume level json. errno: %d", errno);
		free(buffer);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	ssize_t bytesRead = read(fd, buffer, jsonFileStat.st_size);
	if (bytesRead == -1) {
		meddbg("Failed to read from volume level json. errno: %d", errno);
		close(fd);
		free(buffer);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	medvdbg("Total bytes read from volume level json: %d\n", bytesRead);
	gJSON = cJSON_Parse(buffer);
	if (!gJSON) {
		meddbg("Failed to parse volume level json file.\n");
		close(fd);
		free(buffer);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	close(fd);
	free(buffer);
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t update_audio_metadata_json(void)
{
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	int fd = open(AUDIO_METADATA_JSON_PATH, O_WRONLY, 0777);
	if (fd == -1) {
		meddbg("Failed to open volume level json file. errno: %d\n", errno);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	char *jsonString = cJSON_Print(gJSON);
	if (!jsonString) {
		meddbg("Failed to print volume level json object\n");
		close(fd);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	ssize_t bytesWritten = write(fd, jsonString, strlen(jsonString));
	if (bytesWritten == -1) {
		meddbg("Failed to write volume level json file. errno: %d\n", errno);
		ret = AUDIO_MANAGER_OPERATION_FAIL;
	} else {
		medvdbg("To be written JSON string: %s\n, bytes written: %zd\n", jsonString, bytesWritten);
	}
	free(jsonString);
	close(fd);
	return ret;
}

audio_manager_result_t verify_audio_metadata_json(void)
{
	audio_manager_result_t ret;
	const char *jsonKey;
	cJSON *jsonValue;
	cJSON *muteStatusJsonObj;

	jsonKey = STREAM_TYPE_MUTE_STATUS_KEY;
	muteStatusJsonObj = cJSON_GetObjectItem(gJSON, jsonKey);
	if (!muteStatusJsonObj) {
		meddbg("%s not found in json, creating it.\n", STREAM_TYPE_MUTE_STATUS_KEY);
		muteStatusJsonObj = cJSON_CreateObject();
		if (!muteStatusJsonObj) {
			meddbg("Failed to create json object for stream mute status\n");
			return AUDIO_MANAGER_OPERATION_FAIL;
		}
		cJSON_AddItemToObject(gJSON, jsonKey, muteStatusJsonObj);
	}

	for (uint8_t i = 0; i < MAX_STREAM_POLICY_NUM; i++) {
		jsonKey = getJSONKey((stream_policy_t)i);
		jsonValue = cJSON_GetObjectItem(gJSON, jsonKey);
		if (!jsonValue) {
			meddbg("Volume level for policy = %d not found in json, creating it.\n", i);
			cJSON_AddNumberToObject(gJSON, jsonKey, gDefaultVolumeLevel);
		}
		jsonValue = cJSON_GetObjectItem(muteStatusJsonObj, jsonKey);
		if (!jsonValue) {
			meddbg("Mute state for policy = %d not found in json, creating it.\n", i);
			cJSON_AddBoolToObject(muteStatusJsonObj, jsonKey, false);
		}
	}	

	ret = update_audio_metadata_json();
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Failed to update volume level json. ret: %d\n", ret);
	}

	return ret;
}

const char *getJSONKey(stream_policy_t stream_policy)
{
	switch (stream_policy) {
	case STREAM_TYPE_BASE:
		return "STREAM_TYPE_BASE";
	case STREAM_TYPE_MEDIA:
		return "STREAM_TYPE_MEDIA";
	case STREAM_TYPE_VOIP:
		return "STREAM_TYPE_VOIP";
	case STREAM_TYPE_NOTIFY:
		return "STREAM_TYPE_NOTIFY";
	case STREAM_TYPE_VOICE_RECORD:
		return "STREAM_TYPE_VOICE_RECORD";
	case STREAM_TYPE_EMERGENCY:
		return "STREAM_TYPE_EMERGENCY";
	case STREAM_TYPE_BIXBY:
		return "STREAM_TYPE_BIXBY";
	default:
		return "STREAM_TYPE_NA";
	}
}

static inline audio_manager_result_t validate_stream_policy(stream_policy_t stream_policy)
{
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	if (stream_policy <= STREAM_TYPE_INVALID || stream_policy >= MAX_STREAM_POLICY_NUM) {
		meddbg("stream policy = %d out of range\n", stream_policy);
		ret = AUDIO_MANAGER_INVALID_PARAM;
	}
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

audio_manager_result_t audio_manager_init(void)
{
	audio_manager_result_t ret;
	static int am_initialized = 0;
	int found_card = 0;
	int retVal;
	struct stat jsonFileStat;
	const char *jsonKey;
	cJSON *jsonValue;

	if (am_initialized) {
		return AUDIO_MANAGER_SUCCESS;
	}
	am_initialized = 1;
	
	ret = find_audio_card(INPUT);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("find input card failed result : %d\n", ret);
	} else {
		found_card++;
	}
	
	ret = find_audio_card(OUTPUT);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("find output card failed result : %d\n", ret);
	} else {
		found_card++;
	}
	if (found_card == 0) {
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	ret = get_output_audio_volume(&gDefaultVolumeLevel);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Failed to get output audio volume. ret: %d\n", ret);
		return ret;
	}

	retVal = stat(AUDIO_METADATA_JSON_PATH, &jsonFileStat);
	if (retVal != OK) {
		if (errno != ENOENT) {
			meddbg("Failed to fetch json file information. errno: %d\n", errno);
			return AUDIO_MANAGER_OPERATION_FAIL;
		}
		ret = create_audio_metadata_json();
		if (ret != AUDIO_MANAGER_SUCCESS) {
			meddbg("Failed to create volume level json. ret: %d", ret);
			return ret;
		}
	} else if (jsonFileStat.st_size == 0) {
		ret = create_audio_metadata_json();
		if (ret != AUDIO_MANAGER_SUCCESS) {
			meddbg("Failed to create volume level json. ret: %d", ret);
			return ret;
		}
	}

	ret = parse_audio_metadata_json();
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Failed to parse volume level json. ret: %d\n", ret);
		return ret;
	}

	ret = verify_audio_metadata_json();
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Failed to verify volume level json. ret: %d\n", ret);
		return ret;
	}

	audio_card_info_t *card = &g_audio_out_cards[g_actual_audio_out_card_id];
	for (uint16_t i = 0; i < MAX_STREAM_POLICY_NUM; i++) {
		jsonKey = getJSONKey((stream_policy_t)i);
		CJSON_GETOBJECTITEM(jsonValue, gJSON, jsonKey)
		card->volume[i] = jsonValue->valueint;
	}

	ret = set_audio_stream_mute_from_json(STREAM_TYPE_VOICE_RECORD);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Failed to set mic mute status from json. ret: %d\n", ret);
	}

	return ret;
}

audio_manager_result_t set_audio_stream_in(unsigned int channels, unsigned int sample_rate, int format)
{
	audio_card_info_t *card;
	audio_config_t *card_config;
	struct pcm_config config;
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	unsigned int channel_num;
	int err_code = 0;

	if ((channels == 0) || (sample_rate == 0)) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	if (g_actual_audio_in_card_id < 0) {
		meddbg("Found no active input audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	ret = get_supported_capability(INPUT, &channel_num);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	card = &g_audio_in_cards[g_actual_audio_in_card_id];
	card_config = &card->config[card->device_id];

	if (card_config->status == AUDIO_CARD_PAUSE) {
		medvdbg("reset previous preparing\n");
		reset_audio_stream_in();
	}

	pthread_mutex_lock(&(card->card_mutex));

	memset(&config, 0, sizeof(struct pcm_config));
	config.rate = get_closest_samprate(sample_rate, INPUT);
	config.format = static_cast<pcm_format>(format);
	config.period_size = AUDIO_STREAM_VOICE_RECOGNITION_PERIOD_SIZE;
	config.period_count = AUDIO_STREAM_VOICE_RECOGNITION_PERIOD_COUNT;
	config.channels = channel_num;
	medvdbg("Device samplerate: %u, User requested: %u\n", config.rate, sample_rate);
	medvdbg("Device channel: %u User requested: %u\n", config.channels, channels);
	card->pcm = pcm_open(g_actual_audio_in_card_id, card->device_id, PCM_IN, &config);
	if (!pcm_is_ready(card->pcm)) {
		meddbg("fail to pcm_is_ready() error : %s", pcm_get_error(card->pcm));
		ret = AUDIO_MANAGER_CARD_NOT_READY;
		goto error_with_pcm;
	}

	card->resample.necessary = false;
	card->resample.buffer = NULL;
	card->resample.rechannel_buffer = NULL;
	card->resample.user_channel = channels;
	card->resample.user_sample_rate = sample_rate;
	card->resample.user_format = pcm_format_to_bits((enum pcm_format)format) >> 3;
	card->resample.ratio = (float)card->resample.user_sample_rate / (float)config.rate; // ratio = user / card

	// Check if rechanneling or resampling is required
	if ((card->resample.user_channel != config.channels) || (card->resample.user_sample_rate != config.rate)) {
		// Yes, it is necessary, and rechanneling would be processed in rechannel() & resampling would be processed in speex_resampler_process_interleaved_int().
		card->resample.necessary = true;
		uint32_t rechannel_buffer_frames = (int)((float)get_input_frame_count() / card->resample.ratio);
		card->resample.rechannel_buffer_size = get_user_input_frames_to_byte(rechannel_buffer_frames);
		card->resample.rechannel_buffer = malloc(card->resample.rechannel_buffer_size);
		if (!card->resample.rechannel_buffer) {
			meddbg("malloc for a rechannel buffer(stream_in) is failed, rechannel_buffer_frames = %d\n", rechannel_buffer_frames);
			goto error_with_pcm;
		}
		medvdbg("rechanneling buffer 0x%x, buffer_size %u\n", card->resample.rechannel_buffer, card->resample.rechannel_buffer_size);

		/* TODO resampling quality (between 0 and 10) need to be changed manually. 0 has poor quality and 10 has very high quality. */
		int resampling_quality = RESAMPLING_QUALITY;
		/* if sampling rates are integral multiples e.g. 48K -> 96K or 48K -> 16K, use highest quality. Otherwise, use lower quality to avoid stutter */
		if (((card->resample.user_sample_rate >= config.rate) && (card->resample.user_sample_rate % config.rate == 0)) ||
			((card->resample.user_sample_rate <= config.rate) && (config.rate % card->resample.user_sample_rate == 0))) {
			resampling_quality = MAX_RESAMPLING_QUALITY;
		}
		card->resample.speex_resampler = speex_resampler_init(card->resample.user_channel, config.rate, card->resample.user_sample_rate, resampling_quality, &err_code);
		if (!card->resample.speex_resampler) {
			meddbg("Failed to create resampler. errno: %d\n",err_code);
			speex_resampler_strerror(err_code);
			free(card->resample.rechannel_buffer);
			card->resample.rechannel_buffer = NULL;
			goto error_with_pcm;
		}

		// Calculate the buffer size required for resampling.
		float resample_buffer_frames = (float)get_input_frame_count();
		resample_buffer_frames /= card->resample.ratio;
		if (resample_buffer_frames - (int)resample_buffer_frames > 0) {
			resample_buffer_frames = (int)resample_buffer_frames + 1;
		}
		medvdbg("resampling ratio %f, frames %u <- %d\n", card->resample.ratio, get_input_frame_count(), (int)resample_buffer_frames);

		card->resample.buffer_size = get_card_input_frames_to_byte((int)resample_buffer_frames);
		card->resample.buffer = malloc(card->resample.buffer_size);
		if (!card->resample.buffer) {
			meddbg("malloc for a resampling buffer(stream_in) is failed, resample_buffer_frames = %d\n", (int)resample_buffer_frames);
			ret = AUDIO_MANAGER_RESAMPLE_FAIL;
			speex_resampler_destroy(card->resample.speex_resampler);
			card->resample.speex_resampler = NULL;
			free(card->resample.rechannel_buffer);
			card->resample.rechannel_buffer = NULL;
			goto error_with_pcm;
		}
		medvdbg("resampling buffer 0x%x, buffer_size %u\n", card->resample.buffer, card->resample.buffer_size);
	}

	card_config->status = AUDIO_CARD_READY;
	pthread_mutex_unlock(&(card->card_mutex));
	return ret;

error_with_pcm:
	pcm_close(card->pcm);
	pthread_mutex_unlock(&(card->card_mutex));
	return ret;
}

audio_manager_result_t set_audio_stream_out(unsigned int channels, unsigned int sample_rate, int format, stream_info_id_t stream_id)
{
	audio_card_info_t *card;
	audio_config_t *card_config;
	struct pcm_config config;
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	unsigned int channel_num;
	int err_code = 0;

	if ((channels == 0) || (sample_rate == 0)) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	if (g_actual_audio_out_card_id < 0) {
		meddbg("Found no active output audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	ret = get_supported_capability(OUTPUT, &channel_num);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	card = &g_audio_out_cards[g_actual_audio_out_card_id];
	card_config = &card->config[card->device_id];
	medvdbg("[%s] state : %d\n", __func__, card_config->status);
	medvdbg("card->stream_id : %d stream_id : %d\n", card->stream_id, stream_id);
	if (card->stream_id != stream_id) {
		if (card_config->status != AUDIO_CARD_IDLE) {
			reset_audio_stream_out(card->stream_id);
		}
	}
	if (card_config->status == AUDIO_CARD_PAUSE) {
		medvdbg("reset previous preparing\n");
		reset_audio_stream_out(card->stream_id);
	}

	pthread_mutex_lock(&(card->card_mutex));

	memset(&config, 0, sizeof(struct pcm_config));
	config.rate = get_closest_samprate(sample_rate, OUTPUT);
	config.format = static_cast<pcm_format>(format);	// ToDo: Convert properly before the assignment.
	config.period_size = AUDIO_STREAM_VOICE_RECOGNITION_PERIOD_SIZE;
	config.period_count = AUDIO_STREAM_VOICE_RECOGNITION_PERIOD_COUNT;
	config.channels = channel_num;
	medvdbg("[OUT] Device samplerate: %u, User requested: %u\n", config.rate, sample_rate);
	medvdbg("[OUT] Device channel: %u, User requested: %u\n", config.channels, channels);
	if (pcm_is_ready(card->pcm)) {
		meddbg("card is already in use, reuse it!!\n");
	} else {
		card->pcm = pcm_open(g_actual_audio_out_card_id, card->device_id, PCM_OUT, &config);
	}
	/* check reserve state of card again */
	if (!pcm_is_ready(card->pcm)) {
		meddbg("fail to pcm_is_ready() error : %s", pcm_get_error(card->pcm));
		ret = AUDIO_MANAGER_CARD_NOT_READY;
		goto error_with_pcm;
	}

	card->resample.necessary = false;
	card->resample.buffer = NULL;
	card->resample.rechannel_buffer = NULL;
	card->resample.user_channel = channels;
	card->resample.user_sample_rate = sample_rate;
	card->resample.user_format = pcm_format_to_bits((enum pcm_format)format) >> 3;
	card->resample.ratio = (float)config.rate / (float)card->resample.user_sample_rate; // ratio = card / user

	// Check if rechanneling or resampling is required
	if ((config.channels != card->resample.user_channel) || (config.rate != card->resample.user_sample_rate)) {
		// Yes, it is necessary, and rechanneling would be processed in rechannel() & resampling would be processed in speex_resampler_process_interleaved_int().
		card->resample.necessary = true;
		card->resample.rechannel_buffer_size = pcm_get_buffer_size(card->pcm) / card->resample.ratio;
		card->resample.rechannel_buffer = malloc(card->resample.rechannel_buffer_size);
		if (!card->resample.rechannel_buffer) {
			meddbg("malloc for a rechannel buffer(stream_out) is failed, rechannel_buffer_size = %d\n", card->resample.rechannel_buffer_size);
			goto error_with_pcm;
		}
		medvdbg("rechanneling buffer 0x%x, buffer_size %u\n", card->resample.rechannel_buffer, card->resample.rechannel_buffer_size);

		/* TODO resampling quality (between 0 and 10) need to be changed manually. 0 has poor quality and 10 has very high quality. */
		int resampling_quality = RESAMPLING_QUALITY;
		/* if sampling rates are integral multiples e.g. 16K -> 48K or 96K -> 48K, use highest quality. Otherwise, use lower quality to avoid stutter */
		if (((config.rate >= card->resample.user_sample_rate) && (config.rate % card->resample.user_sample_rate == 0)) ||
			((config.rate <= card->resample.user_sample_rate) && (card->resample.user_sample_rate % config.rate == 0))) {
			resampling_quality = MAX_RESAMPLING_QUALITY;
		}
		card->resample.speex_resampler = speex_resampler_init(config.channels, card->resample.user_sample_rate, config.rate, resampling_quality, &err_code);
		if (!card->resample.speex_resampler) {
			meddbg("Failed to create resampler. errno: %d\n",err_code);
			speex_resampler_strerror(err_code);
			free(card->resample.rechannel_buffer);
			card->resample.rechannel_buffer = NULL;
			goto error_with_pcm;
		}

		card->resample.buffer_size = pcm_get_buffer_size(card->pcm);
		card->resample.buffer = malloc(card->resample.buffer_size);
		if (!card->resample.buffer) {
			meddbg("malloc for a resampling buffer(stream_out) is failed, resample_buffer_size = %d\n", card->resample.buffer_size);
			ret = AUDIO_MANAGER_RESAMPLE_FAIL;
			speex_resampler_destroy(card->resample.speex_resampler);
			card->resample.speex_resampler = NULL;
			free(card->resample.rechannel_buffer);
			card->resample.rechannel_buffer = NULL;
			goto error_with_pcm;
		}
		medvdbg("resampling buffer 0x%x, buffer_size %u\n", card->resample.buffer, card->resample.buffer_size);
	}

	card_config->status = AUDIO_CARD_READY;
	card->stream_id = stream_id;
	/* TEMP CODE, Below is very rare case but need to be handled by audio manager */
	audio_card_info_t *inputput_card;
	inputput_card = &g_audio_in_cards[g_actual_audio_in_card_id];
	start_stream_in_device_process_type(inputput_card->card_id, inputput_card->device_id, AUDIO_DEVICE_SPEECH_DETECT_AEC);

	pthread_mutex_unlock(&(card->card_mutex));
	return ret;

error_with_pcm:
	pcm_close(card->pcm);
	pthread_mutex_unlock(&(card->card_mutex));
	return ret;
}

int start_audio_stream_in(void *data, unsigned int frames)
{
	int ret = 0;
	int prepare_retry = AUDIO_STREAM_RETRY_COUNT;
	audio_card_info_t *card;
	medvdbg("start_audio_stream_in(%u)\n", frames);

	if (g_actual_audio_in_card_id < 0) {
		meddbg("Found no active input audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	card = &g_audio_in_cards[g_actual_audio_in_card_id];

	pthread_mutex_lock(&(card->card_mutex));

	void *buffer_ptr = data;
	unsigned int frames_to_read = frames;

	if (card->config[card->device_id].status == AUDIO_CARD_PAUSE) {
		ret = ioctl(pcm_get_file_descriptor(card->pcm), AUDIOIOC_RESUME, 0UL);
		if (ret < 0) {
			meddbg("Fail to ioctl AUDIOIOC_RESUME, ret = %d\n", ret);
			ret = AUDIO_MANAGER_DEVICE_FAIL;
			goto error_with_lock;
		}
	}

	card->config[card->device_id].status = AUDIO_CARD_RUNNING;

	if (card->resample.necessary) {
		buffer_ptr = card->resample.buffer;
		if (frames_to_read > get_input_frame_count()) {
			frames_to_read = get_input_frame_count();
		}
		if (card->resample.ratio != 1) {
			// Calculate matching frames to read.
			frames_to_read = (unsigned int)((float)frames_to_read / card->resample.ratio);
		}
	}

	do {
		ret = pcm_readi(card->pcm, buffer_ptr, frames_to_read);
		medvdbg("Read %d frames\n", ret);

		if (ret == -EPIPE) {
			ret = pcm_prepare(card->pcm);
			medvdbg("PCM is reprepared\n");
			if (ret != OK) {
				meddbg("Fail to pcm_prepare()\n");
				ret = AUDIO_MANAGER_XRUN_STATE;
				goto error_with_lock;
			}
		} else if (ret == -EINVAL) {
			ret = AUDIO_MANAGER_INVALID_PARAM;
			goto error_with_lock;
		} else if (ret == -ESTRPIPE) {
			ret = AUDIO_MANAGER_DEVICE_SUSPENDED;
			goto error_with_lock;
		} else if (ret == -EHOSTUNREACH) {
			ret = AUDIO_MANAGER_DEVICE_DEAD;
		} else {
			break;
		}
	} while ((ret == OK) && (prepare_retry--));

	if (card->resample.necessary && ret > 0) {
		// Tell the number of frames saved in resampling buffer
		card->resample.frames = ret;
		// Process resampling
		ret = (int)resample_stream_in(card, data, frames);
		if (ret < 0) {
			meddbg("Fail to resample!!\n");
			goto error_with_lock;
		}
	}

error_with_lock:
	pthread_mutex_unlock(&(card->card_mutex));

	return ret;
}

int start_audio_stream_out(void *data, unsigned int frames)
{
	int ret = 0;
	int prepare_retry = AUDIO_STREAM_RETRY_COUNT;
	audio_card_info_t *card;
	medvdbg("start_audio_stream_out(%u)\n", frames);

	if (g_actual_audio_out_card_id < 0) {
		meddbg("Found no active output audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	card = &g_audio_out_cards[g_actual_audio_out_card_id];

	pthread_mutex_lock(&(card->card_mutex));
	if (card->resample.necessary) {
		if (frames > get_output_frame_count()) {
			frames = get_output_frame_count();
		}
		// Process resampling
		ret = (int)resample_stream_out(card, data, frames);
		if (ret < 0) {
			meddbg("Fail to resample!!\n");
			goto error_with_lock;
		}
		// Redirect `data` to resampling buffer and update `frames`
		data = card->resample.buffer;
		frames = card->resample.frames;
		uint32_t resampled_bytes = get_card_output_frames_to_byte(frames);
		if (resampled_bytes < card->resample.buffer_size) {
			uint32_t remaining_bytes = card->resample.buffer_size - resampled_bytes;
			memset(((char *)card->resample.buffer) + resampled_bytes, 0x00, remaining_bytes);
			frames += get_card_output_bytes_to_frame(remaining_bytes);
		}
	}

	if (card->config[card->device_id].status == AUDIO_CARD_PAUSE) {
		ret = ioctl(pcm_get_file_descriptor(card->pcm), AUDIOIOC_RESUME, 0UL);
		if (ret < 0) {
			meddbg("Fail to ioctl AUDIOIOC_RESUME, ret = %d\n", ret);
			ret = AUDIO_MANAGER_DEVICE_FAIL;
			goto error_with_lock;
		}
	}

	card->config[card->device_id].status = AUDIO_CARD_RUNNING;

	do {
		ret = pcm_writei(card->pcm, data, frames);
		if (ret < 0) {
			if (ret == -EPIPE) {
				if (prepare_retry > 0) {
					ret = pcm_prepare(card->pcm);
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
				ret = AUDIO_MANAGER_OPERATION_FAIL;
				goto error_with_lock;
			}
		}
	} while (ret == OK);

error_with_lock:
	pthread_mutex_unlock(&(card->card_mutex));

	return ret;
}

static audio_manager_result_t pause_audio_stream(audio_io_direction_t direct)
{
	audio_manager_result_t ret;
	audio_card_info_t *card;
	enum audio_card_status_e *status;
	int card_id;

	card_id = (direct == INPUT) ? g_actual_audio_in_card_id : g_actual_audio_out_card_id;
	if (card_id < 0) {
		meddbg("Found no active audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	card = (direct == INPUT) ? (&g_audio_in_cards[card_id]) : (&g_audio_out_cards[card_id]);
	status = &(card->config[card->device_id].status);
	medvdbg("[%s] state : %d\n", __func__, card->config[card->device_id].status);

	pthread_mutex_lock(&(card->card_mutex));

	if (direct == OUTPUT && *status == AUDIO_CARD_RUNNING) {
		if ((ret = static_cast<audio_manager_result_t>(pcm_drain(card->pcm))) < 0) {
			if (ret == -EPIPE) {
				ret = AUDIO_MANAGER_SUCCESS;
			} else {
				meddbg("pcm_drain faled, ret = %d\n", ret);
			}
		}
	}
	
	ret = static_cast<audio_manager_result_t>(ioctl(pcm_get_file_descriptor(card->pcm), AUDIOIOC_PAUSE, 0UL));
	if (ret < 0) {
		meddbg("Fail to ioctl AUDIOIOC_PAUSE, ret = %d\n", ret);
		pthread_mutex_unlock(&(card->card_mutex));
		return AUDIO_MANAGER_DEVICE_FAIL;
	}
	
	*status = AUDIO_CARD_PAUSE;

	pthread_mutex_unlock(&(card->card_mutex));

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t pause_audio_stream_in(void)
{
	return pause_audio_stream(INPUT);
}

audio_manager_result_t pause_audio_stream_out(void)
{
	return pause_audio_stream(OUTPUT);
}

audio_manager_result_t stop_audio_stream_in(void)
{
	audio_manager_result_t ret;
	audio_card_info_t *card;

	if (g_actual_audio_in_card_id < 0) {
		meddbg("Found no active input audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	card = &g_audio_in_cards[g_actual_audio_in_card_id];
	medvdbg("[%s] state : %d\n", __func__, card->config[card->device_id].status);

	pthread_mutex_lock(&(card->card_mutex));

	ret = static_cast<audio_manager_result_t>(pcm_drop(card->pcm));
	if (ret < 0) { // FIXME
		//pthread_mutex_unlock(&(card->card_mutex));
		meddbg("pcm_drop Failed, ret = %d\n", ret);
		//return AUDIO_MANAGER_DEVICE_FAIL;
	}

	card->config[card->device_id].status = AUDIO_CARD_READY;

	pthread_mutex_unlock(&(card->card_mutex));

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t stop_audio_stream_out(bool drain)
{
	audio_manager_result_t ret;
	audio_card_info_t *card;

	if (g_actual_audio_out_card_id < 0) {
		meddbg("Found no active output audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	card = &g_audio_out_cards[g_actual_audio_out_card_id];

	pthread_mutex_lock(&(card->card_mutex));
	medvdbg("[%s] state : %d\n", __func__, card->config[card->device_id].status);
	if (drain) {
		if ((ret = static_cast<audio_manager_result_t>(pcm_drain(card->pcm))) < 0) {
			if (ret == -EPIPE) {
				ret = AUDIO_MANAGER_SUCCESS;
			} else {
				meddbg("pcm_drain faled, ret = %d\n", ret);
			}
		}
	} else {
		if ((ret = static_cast<audio_manager_result_t>(pcm_drop(card->pcm))) < 0) {
			meddbg("pcm_drop faled, ret = %d\n", ret);
		}
	}
	card->config[card->device_id].status = AUDIO_CARD_READY;
	pthread_mutex_unlock(&(card->card_mutex));

	if (ret < 0) {
		return AUDIO_MANAGER_DEVICE_FAIL;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t reset_audio_stream_in(void)
{
	audio_card_info_t *card;
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;

	if (g_actual_audio_in_card_id < 0) {
		meddbg("Found no active input audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	card = &g_audio_in_cards[g_actual_audio_in_card_id];

	pthread_mutex_lock(&(card->card_mutex));

	pcm_close(card->pcm);
	card->pcm = NULL;

	if (card->resample.necessary) {
		card->resample.necessary = false;
		if (card->resample.buffer) {
			free(card->resample.buffer);
			card->resample.buffer = NULL;
		}
		if (card->resample.rechannel_buffer) {
			free(card->resample.rechannel_buffer);
			card->resample.rechannel_buffer = NULL;
		}
		if (card->resample.speex_resampler) {
			speex_resampler_destroy(card->resample.speex_resampler);
			card->resample.speex_resampler = NULL;
		}
	}

	card->config[card->device_id].status = AUDIO_CARD_IDLE;
	card->policy = STREAM_TYPE_MEDIA;
	pthread_mutex_unlock(&(card->card_mutex));

	return ret;
}

audio_manager_result_t reset_audio_stream_out(stream_info_id_t stream_id)
{
	audio_card_info_t *card;
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;

	if (g_actual_audio_out_card_id < 0) {
		meddbg("Found no active output audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	card = &g_audio_out_cards[g_actual_audio_out_card_id];
	if (stream_id != card->stream_id) {
		medvdbg("audio manager already got reset for stream_id = %d, currently being used by stream_id = %d\n", stream_id, card->stream_id);
		return AUDIO_MANAGER_SUCCESS;
	}
	pthread_mutex_lock(&(g_audio_out_cards[g_actual_audio_out_card_id].card_mutex));
	medvdbg("[%s] state : %d\n", __func__, card->config[card->device_id].status);

	pcm_close(card->pcm);
	card->pcm = NULL;

	if (card->resample.necessary) {
		card->resample.necessary = false;
		if (card->resample.buffer) {
			free(card->resample.buffer);
			card->resample.buffer = NULL;
		}
		if (card->resample.rechannel_buffer) {
			free(card->resample.rechannel_buffer);
			card->resample.rechannel_buffer = NULL;
		}
		if (card->resample.speex_resampler) {
			speex_resampler_destroy(card->resample.speex_resampler);
			card->resample.speex_resampler = NULL;
		}
	}

	card->config[card->device_id].status = AUDIO_CARD_IDLE;
	card->policy = STREAM_TYPE_MEDIA;

	pthread_mutex_unlock(&(card->card_mutex));

	return ret;
}

unsigned int get_input_frame_count(void)
{
	if (g_actual_audio_in_card_id < 0) {
		meddbg("No input audio card is active.\n");
		return 0;
	}

	return get_user_input_bytes_to_frame(pcm_get_buffer_size(g_audio_in_cards[g_actual_audio_in_card_id].pcm));
}

unsigned int get_card_input_frames_to_byte(unsigned int frames)
{
	if ((g_actual_audio_in_card_id < 0) || (frames == 0)) {
		return 0;
	}

	return pcm_frames_to_bytes(g_audio_in_cards[g_actual_audio_in_card_id].pcm, frames);
}

unsigned int get_card_input_bytes_to_frame(unsigned int bytes)
{
	if ((g_actual_audio_in_card_id < 0) || (bytes == 0)) {
		return 0;
	}

	return pcm_bytes_to_frames(g_audio_in_cards[g_actual_audio_in_card_id].pcm, bytes);
}

unsigned int get_user_input_frames_to_byte(unsigned int frames)
{
	int buffer_size = 0;
	audio_card_info_t *card;
	if ((g_actual_audio_in_card_id < 0) || (frames == 0)) {
		return 0;
	}
	card = &g_audio_in_cards[g_actual_audio_in_card_id];
	buffer_size = frames * card->resample.user_channel * card->resample.user_format;
	return buffer_size;
}

unsigned int get_user_input_bytes_to_frame(unsigned int bytes)
{
	int frame_size = 0;
	audio_card_info_t *card;

	if ((g_actual_audio_in_card_id < 0) || (bytes == 0)) {
		return 0;
	}
	card = &g_audio_in_cards[g_actual_audio_in_card_id];

	if (card->resample.user_channel == 0 || card->resample.user_format == 0) {
		return 0;
	}
	frame_size = bytes / card->resample.user_channel / card->resample.user_format;

	return frame_size;
}

unsigned int get_output_frame_count(void)
{
	if (g_actual_audio_out_card_id < 0) {
		meddbg("No output audio card is active.\n");
		return 0;
	}

	return get_user_output_bytes_to_frame(pcm_get_buffer_size(g_audio_out_cards[g_actual_audio_out_card_id].pcm));
}

unsigned int get_card_output_frames_to_byte(unsigned int frames)
{
	if ((g_actual_audio_out_card_id < 0) || (frames == 0)) {
		return 0;
	}

	return pcm_frames_to_bytes(g_audio_out_cards[g_actual_audio_out_card_id].pcm, frames);
}

unsigned int get_card_output_bytes_to_frame(unsigned int bytes)
{
	if ((g_actual_audio_out_card_id < 0) || (bytes == 0)) {
		return 0;
	}

	return pcm_bytes_to_frames(g_audio_out_cards[g_actual_audio_out_card_id].pcm, bytes);
}

unsigned int get_user_output_frames_to_byte(unsigned int frames)
{
	int byte_size = 0;
	audio_card_info_t *card;

	if ((g_actual_audio_out_card_id < 0) || (frames == 0)) {
		return 0;
	}
	card = &g_audio_out_cards[g_actual_audio_out_card_id];
	byte_size = frames * card->resample.user_channel * card->resample.user_format;

	return byte_size;
}

unsigned int get_user_output_bytes_to_frame(unsigned int bytes)
{
	int frame_size = 0;
	audio_card_info_t *card;

	if ((g_actual_audio_out_card_id < 0) || (bytes == 0)) {
		return 0;
	}
	card = &g_audio_out_cards[g_actual_audio_out_card_id];
	frame_size = bytes / card->resample.user_channel / card->resample.user_format;

	return frame_size;
}

float get_output_sample_rate_ratio(void)
{
	if (g_actual_audio_out_card_id >= 0) {
		return g_audio_out_cards[g_actual_audio_out_card_id].resample.ratio;
	}
	return 1;
}

unsigned int get_card_buffer_size(audio_io_direction_t direct)
{
	audio_card_info_t *card;
	struct ap_buffer_info_s buf_info;
	char path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	int fd;
	if (direct == INPUT) {
		if ((g_actual_audio_in_card_id < 0)) {
			return 0;
		}
		card = &g_audio_in_cards[g_actual_audio_in_card_id];
	} else {
		if ((g_actual_audio_out_card_id < 0)) {
			return 0;
		}
		card = &g_audio_out_cards[g_actual_audio_out_card_id];
	}
	get_card_path(path, card->card_id, card->device_id, direct);
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		meddbg("card open fail.. path : %s errno : %d\n", path, errno);
		return 0;
	}
	if (ioctl(fd, AUDIOIOC_GETBUFFERINFO, (unsigned long)&buf_info) < 0) {
		/* Driver doesn't reveal buffer size, so return default value we defined here */
		meddbg("ioctl failed. errno : %d\n", errno);
		buf_info.buffer_size = AUDIO_STREAM_VOICE_RECOGNITION_PERIOD_SIZE * (pcm_format_to_bits((enum pcm_format) PCM_FORMAT_S16_LE) >> 3);
	}
	close(fd);
	return buf_info.buffer_size;
}

unsigned int get_input_card_buffer_size(void)
{
	return get_card_buffer_size(INPUT);
}

unsigned int get_output_card_buffer_size(void)
{
	return get_card_buffer_size(OUTPUT);	
}

audio_manager_result_t get_max_audio_volume(uint8_t *volume)
{
	// ToDo: Add logics to consider 'decibel'.
	if (volume == NULL) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}
	*volume = AUDIO_DEVICE_MAX_VOLUME;

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_input_audio_gain(uint8_t *gain)
{
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	audio_card_info_t *card;

	if (gain == NULL) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}
	ret = get_audio_volume(INPUT);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	card = &g_audio_in_cards[g_actual_audio_in_card_id];

	*gain = card->config[card->device_id].volume;
	medvdbg("Max volume: %d, Gain : %d card id : %d device id : %d\n", card->config[card->device_id].max_volume, *gain, g_actual_audio_in_card_id, card->device_id);

	return ret;
}

audio_manager_result_t get_output_audio_volume(uint8_t *volume)
{
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	audio_card_info_t *card;

	if (volume == NULL) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	card = &g_audio_out_cards[g_actual_audio_out_card_id];
	card->policy = STREAM_TYPE_MEDIA;

	ret = get_audio_volume(OUTPUT);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	*volume = card->config[card->device_id].volume;
	medvdbg("Max volume: %d, Volume : %d card id : %d device id : %d\n", card->config[card->device_id].max_volume, *volume, g_actual_audio_out_card_id, card->device_id);

	return ret;
}

audio_manager_result_t get_output_stream_volume(uint8_t *volume, stream_policy_t stream_policy)
{
	audio_manager_result_t ret;
	audio_card_info_t *card;

	if (!volume) {
		meddbg("volume ptr is null\n");
		return AUDIO_MANAGER_INVALID_PARAM;
	}
	ret = validate_stream_policy(stream_policy);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Invalid stream policy. ret: %d\n", ret);
		return ret;
	}
	if (g_actual_audio_out_card_id) {
		meddbg("Found no active output audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	card = &g_audio_out_cards[g_actual_audio_out_card_id];
	*volume = card->volume[stream_policy];

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_input_audio_gain(uint8_t gain)
{
	return set_audio_volume(INPUT, gain);
}

audio_manager_result_t set_output_audio_volume(uint8_t volume, stream_policy_t stream_policy)
{
	audio_manager_result_t ret;
	audio_card_info_t *card;
	const char *jsonKey;
	cJSON *jsonValue;
	cJSON *streamMuteStatus;

	ret = validate_stream_policy(stream_policy);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Invalid stream policy. ret: %d\n", ret);
		return ret;
	}
	if (g_actual_audio_out_card_id) {
		meddbg("Found no active output audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	CJSON_GETOBJECTITEM(jsonValue, gJSON, STREAM_TYPE_MUTE_STATUS_KEY)
	jsonKey = getJSONKey(stream_policy);
	CJSON_GETOBJECTITEM(streamMuteStatus, jsonValue, jsonKey)
	if (streamMuteStatus->type == cJSON_True) {
		cJSON_ReplaceItemInObject(jsonValue, jsonKey, cJSON_CreateBool(false));
		CJSON_GETOBJECTITEM(jsonValue, gJSON, jsonKey)
		if (jsonValue->valueint != volume) {
			cJSON_ReplaceItemInObject(gJSON, jsonKey, cJSON_CreateNumber(volume));
		}
		ret = update_audio_metadata_json();
		if (ret != AUDIO_MANAGER_SUCCESS) {
			meddbg("Failed to update volume level json. ret: %d\n", ret);
			return ret;
		}
	} else {
		CJSON_GETOBJECTITEM(jsonValue, gJSON, jsonKey)
		if (jsonValue->valueint == volume) {
			return ret;
		}
		cJSON_ReplaceItemInObject(gJSON, jsonKey, cJSON_CreateNumber(volume));
		ret = update_audio_metadata_json();
		if (ret != AUDIO_MANAGER_SUCCESS) {
			meddbg("Failed to update volume level json. ret: %d\n", ret);
			return ret;
		}
	}

	card = &g_audio_out_cards[g_actual_audio_out_card_id];
	card->volume[stream_policy] = volume;

	if (stream_policy != card->policy) {
		meddbg("Policy mismatch, given_policy: %d, card_policy: %d. Volume for given policy will be set when it will play\n", stream_policy, card->policy);
		return ret;
	}

	ret = set_audio_volume(OUTPUT, volume);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_audio_volume failed, ret: %d\n", ret);
	}

	return ret;
}

audio_manager_result_t set_output_stream_volume(stream_policy_t stream_policy)
{
	audio_manager_result_t ret = validate_stream_policy(stream_policy);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Invalid stream policy. ret: %d\n", ret);
		return ret;
	}

	const char *policyKey = getJSONKey(stream_policy);
	cJSON *policy;
	CJSON_GETOBJECTITEM(policy, gJSON, policyKey)
	uint8_t volume = policy->valueint;

	ret = set_audio_volume(OUTPUT, volume);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_audio_volume failed, ret: %d\n", ret);
	}

	return ret;
}

//ToDo: In the future, driver will be changed to load the script that exists in each product app.
audio_manager_result_t set_output_audio_equalizer(uint32_t preset)
{
	return set_audio_equalizer(OUTPUT, preset);
}

//ToDo: In the future, driver will be changed to load the script that exists in each product app.
audio_manager_result_t set_input_audio_equalizer(uint32_t preset)
{
	return set_audio_equalizer(INPUT, preset);
}

uint8_t get_process_type_audio_param_value(device_process_type_t type)
{
	switch (type) {
	case AUDIO_DEVICE_PROCESS_TYPE_NONE:
		return AUDIO_PU_UNDEF;
	case AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR:
		return AUDIO_PU_SPEECH_DETECT;
	default:
		return AUDIO_PU_UNDEF;
	}
}

uint8_t get_subprocess_type_audio_param_value(device_process_subtype_t type)
{
	switch (type) {
	case AUDIO_DEVICE_SPEECH_DETECT_NONE:
		return AUDIO_SD_UNDEF;
	case AUDIO_DEVICE_SPEECH_DETECT_EPD:
		return AUDIO_SD_ENDPOINT_DETECT;
	case AUDIO_DEVICE_SPEECH_DETECT_KD:
		return AUDIO_SD_KEYWORD_DETECT; 
	case AUDIO_DEVICE_SPEECH_DETECT_LOCAL:
		return AUDIO_SD_LOCAL;
	case AUDIO_DEVICE_SPEECH_DETECT_AEC:
		return AUDIO_SD_AEC;
	default:
		return AUDIO_PU_UNDEF;
	}
}

static audio_manager_result_t get_supported_process_type(int card_id, int device_id, audio_io_direction_t direct)
{
	struct audio_caps_desc_s caps_desc;
	char path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	audio_card_info_t *card;
	audio_manager_result_t ret;

	if (direct == OUTPUT) {		// TODO add output logic in future.
		medvdbg("Do not support output for now...\n");
		return AUDIO_MANAGER_SUCCESS;
	}
	card = &g_audio_in_cards[card_id];

	get_card_path(path, card_id, device_id, INPUT);

	caps_desc.caps.ac_len = sizeof(struct audio_caps_s);
	caps_desc.caps.ac_type = AUDIO_TYPE_PROCESSING;
	caps_desc.caps.ac_subtype = AUDIO_PU_UNDEF;

	pthread_mutex_lock(&(card->card_mutex));

	ret = control_audio_stream_device(path, AUDIOIOC_GETCAPS, (unsigned long)&caps_desc.caps);
	if (ret == AUDIO_MANAGER_SUCCESS) {
		card->config[device_id].device_process_type = static_cast<device_process_type_t>(caps_desc.caps.ac_controls.b[0]);
		medvdbg("process type = 0x%x\n", card->config[device_id].device_process_type);
	}

	pthread_mutex_unlock(&(card->card_mutex));
	return ret;
}

/* TODO below functions should be changed,
  * currently we using static array for cards, but below should be modified when we change to handler structure */
audio_manager_result_t find_stream_in_device_with_process_type(device_process_type_t type, device_process_subtype_t subtype, int *card_id, int *device_id)
{
	int i, j;
	uint8_t process_type;
	uint8_t subprocess_type;
	audio_manager_result_t ret;
	char path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	struct audio_caps_desc_s caps_desc;
	audio_card_info_t *card;

	if (type == AUDIO_DEVICE_PROCESS_TYPE_NONE) {
		/* TODO : will be implemented later based on audio subsystem's implementation */
		medvdbg("Type is AUDIO_DEVICE_PROCESS_TYPE_NONE\n");
		for (j = 0; j < CONFIG_AUDIO_MAX_DEVICE_NUM; j++) {
			for (i = 0; i < CONFIG_AUDIO_MAX_INPUT_CARD_NUM; i++) {
				card = &g_audio_in_cards[i];
				if (card->config[j].status == AUDIO_CARD_NONE) {
					medvdbg("Card is not avalialbe, card_id : %d device_id : %d continue\n", i, j);
					continue;
				}
				*card_id = i;
				*device_id = j;
				medvdbg("found! card_id : %d device_id : %d subtype : %d\n", *card_id, *device_id, subtype);
				return AUDIO_MANAGER_SUCCESS;
			}
		}
		return AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
	}

	process_type = get_process_type_audio_param_value(type);
	subprocess_type = get_subprocess_type_audio_param_value(subtype);
	caps_desc.caps.ac_len = sizeof(struct audio_caps_s);
	caps_desc.caps.ac_type = AUDIO_TYPE_PROCESSING;
	caps_desc.caps.ac_subtype = process_type;

	for (j = 0; j < CONFIG_AUDIO_MAX_DEVICE_NUM; j++) {
		for (i = 0; i < CONFIG_AUDIO_MAX_INPUT_CARD_NUM; i++) {
			card = &g_audio_in_cards[i];
			if (card->config[j].status == AUDIO_CARD_NONE) {
				medvdbg("Card is not avalialbe, card_id : %d device_id : %d continue\n", i, j);
				continue;
			}

			ret = get_supported_process_type(i, j, INPUT);
			if (ret != AUDIO_MANAGER_SUCCESS) {
				medvdbg("get process type failed, find next ret : %d\n", ret);
				continue;
			}

			/* If process type is matched */
			if ((process_type & card->config[j].device_process_type) != 0) {
				get_card_path(path, i, j, INPUT);
				pthread_mutex_lock(&(card->card_mutex));

				if (control_audio_stream_device(path, AUDIOIOC_GETCAPS, (unsigned long)&caps_desc.caps) < 0) {
					medvdbg("An ioctl error occurs, find next anyway\n");
					/* ioctl failed, try next one */
					pthread_mutex_unlock(&(card->card_mutex));
					continue;
				}

				/* Now we get subtype from h/w, compare it and subtype(param) */
				if ((subprocess_type & caps_desc.caps.ac_controls.b[0]) != 0) {
					*card_id = i;
					*device_id = j;
					pthread_mutex_unlock(&(card->card_mutex));
					medvdbg("found! card_id : %d device_id : %d subtype : %d\n", *card_id, *device_id, subtype);
					return AUDIO_MANAGER_SUCCESS;
				}
				pthread_mutex_unlock(&(card->card_mutex));
			}
		}
	}

	meddbg("We didn't find any device with process type : %d subtype : %d process_type : %d subprocess_type : %d\n", type, subtype, process_type, subprocess_type);
	return AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
}

audio_manager_result_t request_stream_in_device_process_type(int card_id, int device_id, int cmd, device_process_subtype_t subtype)
{
	char path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	audio_card_info_t *card;
	audio_config_t *config;
	uint8_t subprocess_type;
	audio_manager_result_t ret;

	card = &g_audio_in_cards[card_id];
	config = &card->config[device_id];

	/* Check card register state first, AEC can be set before KD started. */
	if (config->process_handler == NULL && subtype != AUDIO_DEVICE_SPEECH_DETECT_AEC) {
		return AUDIO_MANAGER_CARD_NOT_READY;
	}

	get_card_path(path, card_id, device_id, INPUT);
	pthread_mutex_lock(&(card->card_mutex));

	subprocess_type = get_subprocess_type_audio_param_value(subtype);
	ret = control_audio_stream_device(path, cmd, subprocess_type);
	pthread_mutex_unlock(&(card->card_mutex));

	return ret;
}

audio_manager_result_t start_stream_in_device_process_type(int card_id, int device_id, device_process_subtype_t subtype)
{
	return request_stream_in_device_process_type(card_id, device_id, AUDIOIOC_STARTPROCESS, subtype);
}

audio_manager_result_t stop_stream_in_device_process_type(int card_id, int device_id, device_process_subtype_t subtype)
{
	audio_card_info_t *card;
	audio_manager_result_t result;
	struct audio_msg_s msg;
	int size;
	int prio;
	struct timespec st_time;

	card = &g_audio_in_cards[card_id];

	/* Stop First */
	result = request_stream_in_device_process_type(card_id, device_id, AUDIOIOC_STOPPROCESS, subtype);

	/* Consume all remained msg */
	if (result == AUDIO_MANAGER_SUCCESS) {
		do {
			clock_gettime(CLOCK_REALTIME, &st_time);
			size = mq_timedreceive(card->config[device_id].process_handler, (FAR char *)&msg, sizeof(msg), &prio, &st_time);
		} while (size > 0);
	}
	return result;
}

audio_manager_result_t register_stream_in_device_process_type(int card_id, int device_id, device_process_type_t type, device_process_subtype_t subtype)
{
	char path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	audio_manager_result_t ret;
	audio_card_info_t *card;
	uint8_t process_type;
	uint8_t subprocess_type;
	struct audio_caps_desc_s cap_desc;

	card = &g_audio_in_cards[card_id];
	get_card_path(path, card_id, device_id, INPUT);

	pthread_mutex_lock(&(card->card_mutex));
	process_type = get_process_type_audio_param_value(type);

	subprocess_type = get_subprocess_type_audio_param_value(subtype);
	cap_desc.caps.ac_len = sizeof(struct audio_caps_s);
	cap_desc.caps.ac_type = AUDIO_TYPE_PROCESSING;
	cap_desc.caps.ac_subtype = process_type;
	cap_desc.caps.ac_controls.w = subprocess_type;

	ret = control_audio_stream_device(path, AUDIOIOC_CONFIGURE, (unsigned long)&cap_desc);
	pthread_mutex_unlock(&(card->card_mutex));
	if (ret < 0) {
		if (errno == EINVAL) {
			ret = AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
		} else {
			ret = AUDIO_MANAGER_OPERATION_FAIL;
		}
		meddbg("register process type failed, ret : %d\n", ret);
		return ret;
	}

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t register_stream_in_device_process_handler(int card_id, int device_id, device_process_type_t type)
{
	char path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	char mq_path[AUDIO_DEVICE_PROCESS_QUEUE_PATH_LENGTH];
	audio_card_info_t *card;
	audio_config_t *config;
	uint8_t process_type;
	audio_manager_result_t ret;
	struct mq_attr attr;

	if (type == AUDIO_DEVICE_PROCESS_TYPE_NONE) {
		meddbg("Type is AUDIO_DEVICE_PROCESS_TYPE_NONE\n");
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	ret = get_supported_process_type(card_id, device_id, INPUT);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		return ret;
	}

	card = &g_audio_in_cards[card_id];
	config = &card->config[device_id];

	if (config->process_handler != NULL) {
		medvdbg("Handler is already registered\n");
		++config->handler_refcnt;
		return AUDIO_MANAGER_SUCCESS;
	}

	process_type = get_process_type_audio_param_value(type);

	if (((config->device_process_type) & process_type) == 0) {
		meddbg("device doesn't support this type, device_process_type : %d request type : %d\n", process_type);
		return AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
	}
	get_card_path(path, card_id, device_id, INPUT);

	pthread_mutex_lock(&(card->card_mutex));

	/* Create a message queue */
	attr.mq_maxmsg = 16;
	attr.mq_msgsize = sizeof(struct audio_msg_s);
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;
	snprintf(mq_path, AUDIO_DEVICE_PROCESS_QUEUE_PATH_LENGTH, "%s%s", path, AUDIO_DEVICE_PROCESS_QUEUE_PATH);
	config->process_handler = mq_open(path, O_RDWR | O_CREAT, 0644, &attr);
	config->handler_refcnt = 1;

	/* Register our message queue with the audio device */
	ret = control_audio_stream_device(path, AUDIOIOC_REGISTERPROCESS, (unsigned long)config->process_handler);
	pthread_mutex_unlock(&(card->card_mutex));

	if (ret < 0) {
		if (errno == EBUSY) {
			ret = AUDIO_MANAGER_DEVICE_ALREADY_IN_USE;
		} else if (errno == EINVAL) {
			ret = AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
		} else {
			ret = AUDIO_MANAGER_DEVICE_FAIL;
		}
		meddbg("Register Process failed, ret : %d\n", ret);
	}

	return ret;
}

audio_manager_result_t unregister_stream_in_device_process(int card_id, int device_id)
{
	char path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	audio_manager_result_t ret;
	audio_card_info_t *card;
	audio_config_t *config;

	card = &g_audio_in_cards[card_id];
	config = &card->config[device_id];

	/* Check card register state first */
	if (config->process_handler == NULL) {
		meddbg("Handler is not registered\n");
		return AUDIO_MANAGER_INVALID_DEVICE;
	}
	if (--config->handler_refcnt > 0) {
		return AUDIO_MANAGER_SUCCESS;
	}
	get_card_path(path, card_id, device_id, INPUT);

	pthread_mutex_lock(&(card->card_mutex));

	/* Register our message queue with the audio device */
	ret = control_audio_stream_device(path, AUDIOIOC_UNREGISTERPROCESS, 0);
	pthread_mutex_unlock(&(card->card_mutex));
	if (ret < 0) {
		meddbg("Unregister Failed : %d\n", ret);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}

	mq_close(config->process_handler);
	config->process_handler = NULL;
	return AUDIO_MANAGER_SUCCESS;

}

audio_manager_result_t get_device_process_handler_message(int card_id, int device_id, uint16_t *msgId)
{
	struct audio_msg_s msg;
	int prio;
	ssize_t size = 0;
	struct timespec st_time;
	audio_card_info_t *card;

	card = &g_audio_in_cards[card_id];
	if (card->config[device_id].process_handler == NULL) {
		meddbg("Device doesn't registered process handler\n");
		return AUDIO_MANAGER_INVALID_DEVICE;
	}
	clock_gettime(CLOCK_REALTIME, &st_time);
	st_time.tv_sec += (CONFIG_PROCESS_MSG_TIMEOUT_MSEC) / 1000;
	st_time.tv_nsec += (CONFIG_PROCESS_MSG_TIMEOUT_MSEC % 1000) * 1000000;
	if (st_time.tv_nsec >= 1000000000) {
		st_time.tv_sec += (st_time.tv_nsec) / 1000000000;
		st_time.tv_nsec %= 1000000000;
	}
	size = mq_timedreceive(card->config[device_id].process_handler, (FAR char *)&msg, sizeof(msg), &prio, &st_time);

	if (size != sizeof(msg)) {
		medvdbg("wrong message id : %ld\n", msg.msgId);
		return AUDIO_MANAGER_OPERATION_FAIL;
	} else {
		switch (msg.msgId) {
		case AUDIO_MSG_EPD:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_EPD;
			break;
		case AUDIO_MSG_SPD:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_SPD;
			break;
		case AUDIO_MSG_KD:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_KD;
			break;
		case AUDIO_MSG_LOCAL0:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_LOCAL0;
			break;
		case AUDIO_MSG_LOCAL1:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_LOCAL1;
			break;
		case AUDIO_MSG_LOCAL2:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_LOCAL2;
			break;
		case AUDIO_MSG_LOCAL3:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_LOCAL3;
			break;
		case AUDIO_MSG_LOCAL4:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_LOCAL4;
			break;
		case AUDIO_MSG_LOCAL5:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_LOCAL5;
			break;
		case AUDIO_MSG_LOCAL6:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_LOCAL6;
			break;
		case AUDIO_MSG_LOCAL7:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_LOCAL7;
			break;
		default:
			*msgId = (uint16_t) AUDIO_DEVICE_SPEECH_DETECT_NONE;
			break;
		}
		medvdbg("size : %d sizeofmsg : %d id : %ld\n", size, sizeof(msg), msg.msgId);
	}
	return AUDIO_MANAGER_SUCCESS;

}

audio_manager_result_t change_stream_device(int card_id, int device_id, audio_io_direction_t direct)
{
	char path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	struct stat st;
	audio_card_info_t *card;
	int status;

	if (direct == INPUT) {
		card = &g_audio_in_cards[card_id];
	} else {
		card = &g_audio_out_cards[card_id];
	}

	/* Check current card status first */
	status = card->config[card->device_id].status;
	if (status != AUDIO_CARD_IDLE) {
		meddbg("card status is not IDLE status : %d card_id : %d device_id : %d\n", status, card_id, device_id);
		return AUDIO_MANAGER_CARD_NOT_READY;
	}

	get_card_path(path, card_id, device_id, direct);

	/* check target path is available as a driver path */
	if ((stat(path, &st) != OK) && (errno == ENOENT)) {
		meddbg("no available stream device path : %s\n", path);
		return AUDIO_MANAGER_INVALID_DEVICE;
	}
	pthread_mutex_lock(&(card->card_mutex));

	/* update new card */
	if (direct == INPUT) {
		g_actual_audio_in_card_id = card_id;
		card = &g_audio_in_cards[card_id];
	} else {
		g_actual_audio_out_card_id = card_id;
		card = &g_audio_out_cards[card_id];
	}

	card->card_id = card_id;
	card->device_id = device_id;

	/* update state as same as previous state */
	card->config[card->device_id].status = AUDIO_CARD_IDLE;

	pthread_mutex_unlock(&(card->card_mutex));
	medvdbg("Card changed path  : %s\n", path);
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t change_stream_in_device(int card_id, int device_id)
{
	if (card_id >= CONFIG_AUDIO_MAX_INPUT_CARD_NUM) {
		meddbg("invalid card id : %d\n", card_id);
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	if (device_id >= CONFIG_AUDIO_MAX_DEVICE_NUM) {
		meddbg("invalid device id : %d\n", device_id);
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	if ((card_id == g_actual_audio_in_card_id) && device_id == (g_audio_in_cards[g_actual_audio_in_card_id].device_id)) {
		meddbg("Same card & device id card_id : %d device_id : %d\n", card_id, device_id);
		return AUDIO_MANAGER_DEVICE_ALREADY_IN_USE;
	}
	return change_stream_device(card_id, device_id, INPUT);
}

audio_manager_result_t change_stream_out_device(int card_id, int device_id)
{
	if (card_id >= CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM) {
		meddbg("invalid card id : %d\n", card_id);
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	if (device_id >= CONFIG_AUDIO_MAX_DEVICE_NUM) {
		meddbg("invalid device id : %d\n", device_id);
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	if ((card_id == g_actual_audio_out_card_id) && (device_id == g_audio_out_cards[g_actual_audio_out_card_id].device_id)) {
		meddbg("Same card & device id card_id : %d device_id : %d\n", card_id, device_id);
		return AUDIO_MANAGER_DEVICE_ALREADY_IN_USE;
	}

	return change_stream_device(card_id, device_id, OUTPUT);
}

/* TODO policy should be merged logic of focus manager */
audio_manager_result_t set_stream_policy(stream_policy_t policy, audio_io_direction_t direct)
{
	audio_manager_result_t ret;
	audio_card_info_t *card;

	ret = validate_stream_policy(policy);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Invalid stream policy. ret: %d\n", ret);
		return ret;
	}

	if (direct == INPUT) {
		card = &g_audio_in_cards[g_actual_audio_in_card_id];
	} else {
		card = &g_audio_out_cards[g_actual_audio_out_card_id];
	}

	/* TODO please consider that we should provides it before stream structure created?? or called in media's prepare.... */
	if (card->config[card->device_id].status == AUDIO_CARD_NONE) {
		meddbg("Failed to set stream policy, card is not available\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	/* TODO Consider that reset stream & set values(channel, buffer size) based on policy */
	pthread_mutex_lock(&(card->card_mutex));
	card->policy = policy;
	pthread_mutex_unlock(&(card->card_mutex));
	medvdbg("stream policy set : %d\n", policy);
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_stream_policy(stream_policy_t *policy, audio_io_direction_t direct)
{
	audio_card_info_t *card;

	if (direct == INPUT) {
		card = &g_audio_in_cards[g_actual_audio_in_card_id];
	} else {
		card = &g_audio_out_cards[g_actual_audio_out_card_id];
	}

	if (card->config[card->device_id].status == AUDIO_CARD_NONE) {
		meddbg("Failed to get stream policy, card is not available\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	*policy = card->policy;

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_stream_in_policy(stream_policy_t policy)
{
	return set_stream_policy(policy, INPUT);
}

audio_manager_result_t set_stream_out_policy(stream_policy_t policy)
{
	return set_stream_policy(policy, OUTPUT);
}

audio_manager_result_t get_stream_in_policy(stream_policy_t *policy)
{
	return get_stream_policy(policy, INPUT);
}

audio_manager_result_t get_stream_out_policy(stream_policy_t *policy)
{
	return get_stream_policy(policy, OUTPUT);
}

audio_manager_result_t get_keyword_buffer_size(uint32_t *keywordBufferSize)
{
	audio_card_info_t *card;
	char path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	int fd;
	if (g_actual_audio_in_card_id < 0) {
		meddbg("card id is not valid\n");
		return AUDIO_MANAGER_INVALID_DEVICE;
	}
	card = &g_audio_in_cards[g_actual_audio_in_card_id];
	get_card_path(path, card->card_id, card->device_id, INPUT);
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		meddbg("card open fail.. path : %s errno : %d\n", path, errno);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	if (ioctl(fd, AUDIOIOC_GETKDBUFSIZE, (unsigned long)keywordBufferSize) < 0) {
		meddbg("get keyword buffer size ioctl failed. errno : %d\n", errno);
		close(fd);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	close(fd);
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_keyword_data(uint8_t *buffer)
{
	audio_card_info_t *card;
	char path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	int fd;
	if (g_actual_audio_in_card_id < 0) {
		meddbg("card id is not valid\n");
		return AUDIO_MANAGER_INVALID_DEVICE;
	}
	card = &g_audio_in_cards[g_actual_audio_in_card_id];
	get_card_path(path, card->card_id, card->device_id, INPUT);
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		meddbg("card open fail.. path : %s errno : %d\n", path, errno);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	if (ioctl(fd, AUDIOIOC_GETKDDATA, (unsigned long)buffer) < 0) {
		meddbg("get keyword data ioctl failed. errno : %d\n", errno);
		close(fd);
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	close(fd);
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_mic_mute(void)
{
	audio_manager_result_t ret;
	struct audio_caps_desc_s caps_desc;
	audio_card_info_t *card;
	char card_path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	cJSON *jsonValue;
	cJSON *micMuteStatus;

	if (g_actual_audio_in_card_id < 0) {
		meddbg("Found no active input audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	card = &g_audio_in_cards[g_actual_audio_in_card_id];
	caps_desc.caps.ac_type = AUDIO_TYPE_FEATURE;
	caps_desc.caps.ac_format.hw = AUDIO_FU_MUTE;
	caps_desc.caps.ac_controls.b[0] = true;

	get_card_path(card_path, card->card_id, card->device_id, INPUT);

	pthread_mutex_lock(&(card->card_mutex));

	ret = control_audio_stream_device(card_path, AUDIOIOC_CONFIGURE, (unsigned long)&caps_desc);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Fail to mute mic, ret = %d errno : %d\n", ret, get_errno());
		if (get_errno() == EACCES) {
			ret = AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
		}
		pthread_mutex_unlock(&(card->card_mutex));
		return ret;
	}
	medvdbg("Mic Mute Success\n");

	pthread_mutex_unlock(&(card->card_mutex));

	CJSON_GETOBJECTITEM(jsonValue, gJSON, STREAM_TYPE_MUTE_STATUS_KEY)
	CJSON_GETOBJECTITEM(micMuteStatus, jsonValue, "STREAM_TYPE_VOICE_RECORD")
	if (micMuteStatus->type == cJSON_False) {
		cJSON_ReplaceItemInObject(jsonValue, "STREAM_TYPE_VOICE_RECORD", cJSON_CreateBool(true));
		ret = update_audio_metadata_json();
		if (ret != AUDIO_MANAGER_SUCCESS) {
			meddbg("Failed to update volume level json. ret: %d\n", ret);
		}
	}
	return ret;
}

audio_manager_result_t set_mic_unmute(void)
{
	audio_manager_result_t ret;
	struct audio_caps_desc_s caps_desc;
	audio_card_info_t *card;
	char card_path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	cJSON *jsonValue;
	cJSON *micMuteStatus;

	if (g_actual_audio_in_card_id < 0) {
		meddbg("Found no active input audio card\n");
		return AUDIO_MANAGER_NO_AVAIL_CARD;
	}

	card = &g_audio_in_cards[g_actual_audio_in_card_id];
	caps_desc.caps.ac_type = AUDIO_TYPE_FEATURE;
	caps_desc.caps.ac_format.hw = AUDIO_FU_MUTE;
	caps_desc.caps.ac_controls.b[0] = false;

	get_card_path(card_path, card->card_id, card->device_id, INPUT);

	pthread_mutex_lock(&(card->card_mutex));

	ret = control_audio_stream_device(card_path, AUDIOIOC_CONFIGURE, (unsigned long)&caps_desc);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Fail to unmute mic, ret = %d errno : %d\n", ret, get_errno());
		if (get_errno() == EACCES) {
			ret = AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
		}
		pthread_mutex_unlock(&(card->card_mutex));
		return ret;
	}
	medvdbg("Mic Unmute Success\n");

	pthread_mutex_unlock(&(card->card_mutex));

	CJSON_GETOBJECTITEM(jsonValue, gJSON, STREAM_TYPE_MUTE_STATUS_KEY)
	CJSON_GETOBJECTITEM(micMuteStatus, jsonValue, "STREAM_TYPE_VOICE_RECORD")
	if (micMuteStatus->type == cJSON_True) {
		cJSON_ReplaceItemInObject(jsonValue, "STREAM_TYPE_VOICE_RECORD", cJSON_CreateBool(false));
		ret = update_audio_metadata_json();
		if (ret != AUDIO_MANAGER_SUCCESS) {
			meddbg("Failed to update volume level json. ret: %d\n", ret);
		}
	}
	return ret;
}

audio_manager_result_t set_audio_stream_mute(stream_policy_t stream_policy, bool mute)
{
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;
	audio_io_direction_t direct;
	cJSON *jsonValue;
	const char *jsonKey;
	cJSON *streamMuteStatus;

	if (stream_policy != STREAM_TYPE_VOICE_RECORD && stream_policy != STREAM_TYPE_NOTIFY) {
		meddbg("Invalid stream policy for set stream mute. policy: %d\n", stream_policy);
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	CJSON_GETOBJECTITEM(jsonValue, gJSON, STREAM_TYPE_MUTE_STATUS_KEY)
	jsonKey = getJSONKey(stream_policy);
	CJSON_GETOBJECTITEM(streamMuteStatus, jsonValue, jsonKey)
	if ((streamMuteStatus->type == cJSON_True) != mute) {
		cJSON_ReplaceItemInObject(jsonValue, jsonKey, cJSON_CreateBool(mute));
		ret = update_audio_metadata_json();
		if (ret != AUDIO_MANAGER_SUCCESS) {
			meddbg("Failed to update volume level json. ret: %d\n", ret);
			return ret;
		}
		direct = (stream_policy == STREAM_TYPE_VOICE_RECORD) ? INPUT : OUTPUT;
		ret = set_audio_mute(direct, stream_policy, mute);
		if (ret != AUDIO_MANAGER_SUCCESS) {
			meddbg("Failed to set audio to mute. direct: %d, stream_policy: %d, mute: %d, ret: %d\n", direct, stream_policy, mute, ret);
		}
	}

	return ret;
}

audio_manager_result_t set_audio_stream_mute_from_json(stream_policy_t stream_policy)
{
	audio_manager_result_t ret;
	audio_io_direction_t direct;
	cJSON *jsonValue;
	const char *jsonKey;
	cJSON *streamMuteStatus;
	bool mute;

	ret = validate_stream_policy(stream_policy);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Invalid stream policy. ret: %d\n", ret);
		return ret;
	}

	CJSON_GETOBJECTITEM(jsonValue, gJSON, STREAM_TYPE_MUTE_STATUS_KEY)
	jsonKey = getJSONKey(stream_policy);
	CJSON_GETOBJECTITEM(streamMuteStatus, jsonValue, jsonKey)
	mute = (streamMuteStatus->type == cJSON_True) ? true : false;
	direct = (stream_policy == STREAM_TYPE_VOICE_RECORD) ? INPUT : OUTPUT;

	ret = set_audio_mute(direct, stream_policy, mute);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Failed to set audio to mute. direct: %d, stream_policy: %d, mute: %d, ret: %d\n", direct, stream_policy, mute, ret);
	}

	return ret;
}

audio_manager_result_t get_audio_stream_mute_state(stream_policy_t stream_policy, bool *mute)
{
	audio_manager_result_t ret;
	cJSON *jsonValue;
	const char *jsonKey;
	cJSON *streamMuteStatus;

	ret = validate_stream_policy(stream_policy);
	if (ret != AUDIO_MANAGER_SUCCESS) {
		meddbg("Invalid stream policy. ret: %d\n", ret);
		return ret;
	}

	CJSON_GETOBJECTITEM(jsonValue, gJSON, STREAM_TYPE_MUTE_STATUS_KEY)
	jsonKey = getJSONKey(stream_policy);
	CJSON_GETOBJECTITEM(streamMuteStatus, jsonValue, jsonKey)
	*mute = (streamMuteStatus->type == cJSON_True) ? true : false;

	return AUDIO_MANAGER_SUCCESS;
}

#ifdef CONFIG_DEBUG_MEDIA_INFO
void print_audio_card_info(audio_io_direction_t direct)
{
	audio_card_info_t *card;
	audio_card_status_t status;
	int i, j, max_card, actual_card_id;
	char path[AUDIO_DEVICE_FULL_PATH_LENGTH];
	unsigned int channel;

	if (direct == INPUT) {
		max_card = CONFIG_AUDIO_MAX_OUTPUT_CARD_NUM;
		actual_card_id = g_actual_audio_in_card_id;
	} else {
		max_card = CONFIG_AUDIO_MAX_INPUT_CARD_NUM;
		actual_card_id = g_actual_audio_out_card_id;
	}
	get_audio_volume(direct);
	get_supported_capability(direct, &channel);
	for (j = 0; j < CONFIG_AUDIO_MAX_DEVICE_NUM; j++) {
		for (i = 0; i < max_card; i++) {
			if (direct == INPUT) {
				card = &g_audio_in_cards[i];
			} else {
				card = &g_audio_out_cards[i];
			}
			status = card->config[j].status;
			if (status == AUDIO_CARD_NONE) {
				dbg_noarg("AUDIO_CARD_NONE(%d)\n", AUDIO_CARD_NONE);
			} else {
				get_card_path(path, i, j, direct);

				dbg_noarg("\nDevice Path : %s\n", path);
				dbg_noarg("Card Number : %d\t Device Number : %d\t", i, j);
				if ((card->card_id == actual_card_id) && (j == card->device_id)) {
					dbg_noarg("---> Actual Card\n");
				} else {
					dbg_noarg("---> Not using now\n");
				}
				dbg_noarg("Status : ");
				switch (status) {
				case AUDIO_CARD_IDLE:
					dbg_noarg("%s(%d)\n", "AUDIO_CARD_IDLE", AUDIO_CARD_IDLE);
					break;
				case AUDIO_CARD_PAUSE:
					dbg_noarg("%s(%d)\n", "AUDIO_CARD_PAUSE", AUDIO_CARD_PAUSE);
					break;
				case AUDIO_CARD_READY:
					dbg_noarg("%s(%d)\n", "AUDIO_CARD_READY", AUDIO_CARD_READY);
					break;
				case AUDIO_CARD_RUNNING:
					dbg_noarg("%s(%d)\n", "AUDIO_CARD_RUNNING", AUDIO_CARD_RUNNING);
					break;
				default:
					dbg_noarg("%s(%d)\n", "ERROR!!", AUDIO_MANAGER_DEVICE_FAIL);
					break;
				}

				dbg_noarg("Volume : %d Max Volume : %d\n", card->config[j].volume, card->config[j].max_volume);
				dbg_noarg("Supports Process Type : ");
				if ((card->config[j].device_process_type & get_process_type_audio_param_value(AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR)) != 0) {
					dbg_noarg("%s(%x)\t", "AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR", AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR);
				}

				if ((card->config[j].device_process_type & get_process_type_audio_param_value(AUDIO_DEVICE_PROCESS_TYPE_NONE)) != 0) {
					dbg_noarg("%s(%x)\t", "AUDIO_DEVICE_PROCESS_TYPE_NONE", AUDIO_DEVICE_PROCESS_TYPE_NONE);
				}
				dbg_noarg("\n");

				if (status != AUDIO_CARD_IDLE) {
					dbg_noarg("Channel :%d\nSampleRate :%d\nPcmFormat : %d\nBuffer Frame Size : %d\n\n", pcm_get_channels(card->pcm), pcm_get_rate(card->pcm), pcm_get_format(card->pcm), pcm_get_buffer_size(card->pcm));
				}
			}
		}
	}
}
#endif

#ifdef CONFIG_DEBUG_MEDIA_INFO
void dump_audio_card_info()
{
	print_audio_card_info(INPUT);
	print_audio_card_info(OUTPUT);
}
#endif
