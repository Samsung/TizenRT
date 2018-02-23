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

/**
 * @file audio_manager.h
 * @brief All macros, structures and functions to manager audio devices.
 */

#ifndef __AUDIO_MANAGER_H
#define __AUDIO_MANAGER_H

#include <sys/time.h>
#include <stddef.h>
#include <stdint.h>

/****************************************************************************
 * Private Declarations
 ****************************************************************************/
enum audio_device_status_e {
	AUDIO_DEVICE_STATE_UNIDENTIFIED = -1,
	AUDIO_DEVICE_STATE_IDENTIFIED,
	AUDIO_DEVICE_STATE_RESERVED
};

enum manufacturer_type_e {
	AUDIO_DEVICE_EARPIECE,
	AUDIO_DEVICE_SPEAKER,
	HEADSET
};

struct audio_config_s {
	unsigned int volume;
	char *manufacturer_name;
	enum manufacturer_type_e manufacturer_type;
};

struct audio_dev_info_s {
//  int audio_dev_id;
	char *dev_name;
	enum audio_device_status_e status;
	struct audio_config_s config;
//  audio_dev_type_e direction;
};

typedef enum audio_device_status_e audio_device_status_t;
typedef struct audio_config_s audio_config_t;
typedef struct audio_dev_info_s audio_dev_info_t;

#if defined(__cplusplus)
extern "C" {
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MAX_IN_AUDIO_DEV_NUM 2
#define MAX_OUT_AUDIO_DEV_NUM 2

/****************************************************************************
 * Public Data
 ****************************************************************************/
/**
 * @brief Result types of Audio Manager APIs such as FAIL, SUCCESS, or INVALID ARGS
 */
enum audio_manager_result_e {
	AUDIO_MANAGER_FAIL = -1,
	AUDIO_MANAGER_SUCCESS,
	AUDIO_MANAGER_INVALID_PARAMS
};

enum audio_volume_e {
	AUDIO_VOLUME_LOW = 30,
	AUDIO_VOLUME_MEDIUM = 80,
	AUDIO_VOLUME_HIGH = 100
};

typedef enum audio_manager_result_e audio_manager_result_t;
typedef enum audio_volume_e audio_volume_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
audio_manager_result_t audio_manager_init(void);
audio_manager_result_t set_audio_volume(int fd, uint16_t volume);
int get_avail_audio_card_id(void);
int get_avail_audio_device_id(void);

#if defined(__cplusplus)
}								/* extern "C" */
#endif
/** @} */// end of TinyAlsa group
#endif
