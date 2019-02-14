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

#include <stdlib.h>
#include <media/stream_info.h>

int create_stream_information(stream_policy_t stream_policy, stream_focus_state_changed_cb callback, void *user_data, stream_info_t **stream_info)
{
	if (stream_info == NULL) {
		return -1;
	}

	*stream_info = (stream_info_t *)calloc(1, sizeof(stream_info_t));
	(*stream_info)->policy = stream_policy;
	(*stream_info)->user_cb = callback;
	(*stream_info)->user_data = user_data;

	return 0;
}

int destroy_stream_information(stream_info_t *stream_info)
{
	if (stream_info == NULL) {
		return -1;
	}

	free(stream_info);
	stream_info = NULL;
	return 0;
}