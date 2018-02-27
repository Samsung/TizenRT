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
#include "audio_manager.h"

/****************************************************************************
 * Private Declarations
 ****************************************************************************/
audio_dev_info_t g_input_audio_devices[MAX_IN_AUDIO_DEV_NUM];
audio_dev_info_t g_output_audio_devices[MAX_OUT_AUDIO_DEV_NUM];

/****************************************************************************
 * Public Functions
 ****************************************************************************/
audio_manager_result_t audio_manager_init(void)
{
	unsigned int card_id, device_id;
	char type;
	struct dirent *dir_entry;
	DIR *dir_info;
	int i;

	for (i = 0; i < MAX_IN_AUDIO_DEV_NUM; i++) {
		g_input_audio_devices[i].status = AUDIO_DEVICE_STATE_UNIDENTIFIED;
	}
	for (i = 0; i < MAX_OUT_AUDIO_DEV_NUM; i++) {
		g_output_audio_devices[i].status = AUDIO_DEVICE_STATE_UNIDENTIFIED;
	}

	if (!(dir_info = opendir("/dev/audio/"))) {
		return AUDIO_MANAGER_FAIL;
	}

	while ((dir_entry = readdir(dir_info)) != NULL) {
		audvdbg("%s\n", dir_entry->d_name);
		// TODO: Add cases for various drivers. Currently, identify 'pcm' drivers only.
		if ((dir_entry->d_name[0] != 'p')
			|| (dir_entry->d_name[1] != 'c')
			|| (dir_entry->d_name[2] != 'm')
			|| (sscanf(&dir_entry->d_name[3], "C%uD%u%c", &card_id, &device_id, &type) != 3)) {
			closedir(dir_info);
			return AUDIO_MANAGER_INVALID_PARAMS;
		}

		if (type == 'c' && device_id < MAX_IN_AUDIO_DEV_NUM && g_input_audio_devices[device_id].status == AUDIO_DEVICE_STATE_UNIDENTIFIED) {
			g_input_audio_devices[device_id].status = AUDIO_DEVICE_STATE_IDENTIFIED;
		} else if (type == 'p' && device_id < MAX_OUT_AUDIO_DEV_NUM && g_output_audio_devices[device_id].status == AUDIO_DEVICE_STATE_UNIDENTIFIED) {
			g_output_audio_devices[device_id].status = AUDIO_DEVICE_STATE_IDENTIFIED;
		} else {
			return AUDIO_MANAGER_INVALID_PARAMS;
		}
	}
	closedir(dir_info);

	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_audio_volume(int fd, uint16_t volume)
{
	audio_manager_result_t ret = AUDIO_MANAGER_SUCCESS;

	ret = ioctl(fd, AUDIOIOC_SETVOLUME, (unsigned int)volume);
	if (ret < 0) {
		ret = AUDIO_MANAGER_FAIL;
		printf("AUDIOIOC_SETVOLUME ioctl failed, ret = %d\n", ret);
	}
	return ret;
}

uint16_t get_audio_volume(int fd)
{
	int ret;
	uint16_t *audio_volume = (uint16_t *)malloc(sizeof(uint16_t));

	ret = ioctl(fd, AUDIOIOC_GETVOLUME, (unsigned int *)audio_volume);
	if (ret < 0) {
		printf("AUDIOIOC_GETVOLUME ioctl failed, ret = %d\n", ret);
		return -1;
	}
	return *audio_volume;
}

int get_avail_audio_card_id(void)
{
	printf("[audio_manager] get_avail_audio_card_id() is called\n");
	return 0;
}

int get_avail_audio_device_id(void)
{
	printf("[audio_manager] get_avail_audio_device_id() is called\n");
	return 0;
}

