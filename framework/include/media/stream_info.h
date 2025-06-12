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
	STREAM_FOCUS_STATE_DUCKED	/**< Focus state for ducking*/
};

typedef enum stream_focus_state_e stream_focus_state_t;

/**
 * @brief Stream Policy, high value means higher priority
 */
enum stream_policy_e {
	STREAM_TYPE_INVALID = -1,
	STREAM_TYPE_BASE = 0,
	STREAM_TYPE_MEDIA = 1, /* Default value, music play */
	STREAM_TYPE_NOTIFY = 2, /* For notification */
	STREAM_TYPE_BIXBY = 3,
	STREAM_TYPE_VOICE_RECORD = 4, /* Record */
	STREAM_TYPE_VOIP = 5, /* Call */
	STREAM_TYPE_EMERGENCY = 6,
	MAX_STREAM_POLICY_NUM = 7
};

/**
 * @brief Used to indicate change in audio focus state, or a request of audio focus.
*/
enum focus_state_e {
	/* Used to indicate a temporary loss of audio focus, anticipated to last a short amount of time, and where it is acceptable to keep playing in duck mode */
	FOCUS_LOSS_TRANSIENT_CAN_DUCK = -3,
	/* Used to indicate a temporary loss of audio focus, anticipated to last a short amount of time */
	FOCUS_LOSS_TRANSIENT = -2,
	/* Used to indicate a loss of audio focus of unknown duration */
	FOCUS_LOSS = -1,
	/* Used to indicate no audio focus change */
	FOCUS_NONE = 0,
	/* Used to indicate a gain of audio focus, or a request of audio focus, of unknown duration. E.g. music playback */
	FOCUS_GAIN = 1,
	/* Used to indicate a temporary gain or request of audio focus, anticipated to last a short amount of time. E.g. event notification playback */
	FOCUS_GAIN_TRANSIENT = 2,
	/* Used to indicate a temporary gain or request of audio focus, anticipated to last a short amount of time, and where it is acceptable for other audio applications to keep playing in duck mode i.e. lowered volume. E.g. UI tap sounds */
	FOCUS_GAIN_TRANSIENT_MAY_DUCK = 3
};

typedef enum stream_policy_e stream_policy_t;
typedef size_t stream_info_id_t;
typedef enum focus_state_e focus_state_t;

struct stream_info_s {
	stream_info_id_t id;
	stream_policy_t policy;
};

int stream_info_create(stream_policy_t stream_policy, stream_info_t **stream_info);
int stream_info_destroy(stream_info_t *stream_info);

#if defined(__cplusplus)
} /* extern "C" */
#endif
#endif
/** @} */ // end of MEDIA group
