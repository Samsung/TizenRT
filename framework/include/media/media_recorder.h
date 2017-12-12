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

#ifndef __MEDIA_RECORDER_H__
#define __MEDIA_RECORDER_H__

#include <sys/types.h>
#include <stdbool.h>
#include <media/media_types.h>
#include <sched.h>
#include <tinyalsa/tinyalsa.h>

/***
 * Enumerations
 */

enum record_result_e {
	RECORD_OK,
	RECORD_ERROR_THREAD_INIT,
	RECORD_ERROR_MALLOC,
	RECORD_ERROR_DRIVER,
	RECORD_ERROR_INVALID_STATE,
	RECORD_ALREADY_INITIALIZED
};
typedef enum record_result_e record_result_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

record_result_t media_record_set_config(int channel, int sample_rate, int pcm_format, media_format_t format);
record_result_t media_record_prepare(void);
record_result_t media_record(int fd);
record_result_t media_stop_record(void);
record_result_t media_pause_record(void);
record_result_t media_resume_record(void);

bool media_is_recording(void);

record_result_t media_record_init(void);

#ifdef __cplusplus
}
#endif

#endif
