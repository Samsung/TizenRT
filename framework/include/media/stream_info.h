/* ****************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/**
 * @ingroup MEDIA
 * @{
 */

/**
 * @file media/stream_info.h
 * @brief Provide definiton of stream_info
 */

#ifndef __STREAM_INFO_H
#define __STREAM_INFO_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>

typedef struct stream_info_s stream_info_t;

/**
 * @brief Enumeration for stream focus state.
 */
enum stream_focus_state_e {
	STREAM_FOCUS_STATE_RELEASED, /**< Focus state for release */
	STREAM_FOCUS_STATE_ACQUIRED, /**< Focus state for acquisition */
};

typedef enum stream_focus_state_e stream_focus_state_t;

/**
 * @brief Stream Policy, high value means higher priority
 */
enum stream_policy_e {
	STREAM_TYPE_MEDIA = 0, /* Default value, music play */
	STREAM_TYPE_NOTIFY = 1, /* For notification */
	STREAM_TYPE_BIXBY = 2,
	STREAM_TYPE_VOICE_RECORD = 3, /* Record */
	STREAM_TYPE_VOIP = 4, /* Call */
	STREAM_TYPE_EMERGENCY = 5
};

typedef enum stream_policy_e stream_policy_t;
typedef uint64_t stream_info_id_t;

struct stream_info_s {
	stream_info_id_t id;
	stream_policy_t policy;
};

int stream_info_init(void);
int stream_info_create(stream_policy_t stream_policy, stream_info_t **stream_info);
int stream_info_destroy(stream_info_t *stream_info);

#if defined(__cplusplus)
} /* extern "C" */
#endif
#endif
/** @} */ // end of MEDIA group
