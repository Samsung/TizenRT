/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef __MEDIA_PLAYER_H__
#define __MEDIA_PLAYER_H__

#include <sys/types.h>
#include <stdbool.h>
#include <media/media_types.h>
#include <sched.h>
#include <tinyalsa/tinyalsa.h>

/***
 * Enumerations
 */

enum play_result_e {
	PLAY_OK,
	PLAY_ERROR_THREAD_INIT,
	PLAY_ERROR_MALLOC,
	PLAY_ERROR_DRIVER,
	PLAY_ERROR_INVALID_STATE
};
typedef enum play_result_e play_result_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

play_result_t media_play_init(void);
play_result_t media_play(int fd, media_format_t format);
int media_get_duration(int fd);
play_result_t media_stop_play(void);
play_result_t media_pause_play(void);
play_result_t media_resume_play(void);

play_result_t media_set_vol(unsigned char vol);
unsigned char media_get_vol(void);
play_result_t media_set_position(int time_ms);
int media_get_position(void);

bool media_is_available(void);
bool media_is_playing(void);


#ifdef __cplusplus
}
#endif

#endif
