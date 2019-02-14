/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

/**
 * @brief Enumeration for stream focus state.
 */
enum {
	STREAM_FOCUS_STATE_RELEASED, /**< Focus state for release */
	STREAM_FOCUS_STATE_ACQUIRED, /**< Focus state for acquisition */
} stream_focus_state_e;

typedef enum stream_focus_state_e stream_focus_state_t;

/**
 * @brief Stream Policy, high value means higher priority
 */
enum stream_policy_e {
	STREAM_TYPE_MEDIA = 0,
	STREAM_TYPE_VOIP = 1,
	STREAM_TYPE_NOTIFY = 2,
	STREAM_TYPE_VOICE_RECOGNITION = 3,
	STREAM_TYPE_EMERGENCY = 4
};

typedef enum stream_policy_e stream_policy_t;

typedef void (*stream_focus_state_changed_cb)(stream_info_t *stream_info, stream_focus_state_t state);

struct stream_info_s {
	stream_focus_state_changed_cb user_cb;
	void *user_data;
	stream_policy_t policy;
};

typedef struct stream_info_s stream_info_t;

int create_stream_information(stream_policy_t stream_policy, stream_focus_state_changed_cb callback, void *user_data, stream_info_t **stream_info);
int destroy_stream_information(stream_info_t *stream_info);

#endif
/** @} */ // end of MEDIA group
