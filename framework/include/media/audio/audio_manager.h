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

typedef enum audio_manager_result_e audio_manager_result_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
audio_manager_result_t audio_manager_init(void);
audio_manager_result_t set_audio_volume(int fd, uint16_t volume);
audio_manager_result_t get_audio_volume(int fd, uint16_t *volume);
audio_manager_result_t get_avail_audio_card_id(unsigned int *card_id);
audio_manager_result_t get_avail_audio_device_id(unsigned int *device_id);

#if defined(__cplusplus)
}								/* extern "C" */
#endif
/** @} */// end of TinyAlsa group
#endif
