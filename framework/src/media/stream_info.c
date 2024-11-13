/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <media/stream_info.h>

static pthread_mutex_t g_stream_info_mutex;
static uint64_t g_stream_info_id = 0;

int stream_info_init(void)
{
	int ret;
	static bool is_stream_info_init = false;
	if (is_stream_info_init) {
		return OK;
	}

	ret = pthread_mutex_init(&g_stream_info_mutex, NULL);
	if (ret != OK) {
		return ret;
	}
	is_stream_info_init = true;
	return ret;
}

static uint64_t stream_info_id_generate(void)
{
	uint64_t id;
	pthread_mutex_lock(&g_stream_info_mutex);
	/**
	 * ToDo: Handle overflow condition of uint64_t type.
	*/
	id = ++g_stream_info_id;
	pthread_mutex_unlock(&g_stream_info_mutex);
	return id;
}

int stream_info_create(stream_policy_t stream_policy, stream_info_t **stream_info)
{
	if (stream_info == NULL) {
		return -EINVAL;
	}

	*stream_info = (stream_info_t *)calloc(1, sizeof(stream_info_t));
	if (*stream_info == NULL) {
		return -ENOMEM;
	}
	(*stream_info)->id = stream_info_id_generate();
	(*stream_info)->policy = stream_policy;

	return OK;
}

int stream_info_destroy(stream_info_t *stream_info)
{
	if (stream_info == NULL) {
		return -EINVAL;
	}

	/* Make sure stream_info has been released */
	free(stream_info);
	stream_info = NULL;
	return OK;
}
