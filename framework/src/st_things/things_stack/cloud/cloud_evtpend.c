/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "logging/things_logger.h"
#include "cloud_evtpend.h"
#include "easy-setup/resource_handler.h"

#define TAG "[cloudevtpend]"

static es_cloud_pending_event_s pending_event = { false, false, false, };

static pthread_mutex_t g_pending_mutex = PTHREAD_MUTEX_INITIALIZER;

void ci_cp_init_event_pend(void)
{
	pthread_mutex_lock(&g_pending_mutex);
	pending_event.exist = false;
	pending_event.is_there_ci_prov = false;
	pending_event.using_count = 0;
	init_es_cloud_prov_data(&pending_event.event);
	pthread_mutex_unlock(&g_pending_mutex);
}

es_cloud_prov_data_s *ci_cp_get_pended_data(void)	// [GET] If there pending-Data, then return point of Data.
{
	es_cloud_prov_data_s *p_event = NULL;

	pthread_mutex_lock(&g_pending_mutex);
	if (pending_event.exist == true) {
		p_event = &pending_event.event;
	}
	pthread_mutex_unlock(&g_pending_mutex);

	return p_event;
}

bool ci_cp_get_is_there_cp(void)	// [GET] return is_there_ci_prov value.
{
	bool is_there = false;

	pthread_mutex_lock(&g_pending_mutex);
	is_there = pending_event.is_there_ci_prov;
	pthread_mutex_unlock(&g_pending_mutex);

	return is_there;
}

bool ci_cp_cas_is_there_cp_if_false(void)	// [CAS] If is_there_ci_prov value is false, then set true and return is_there_ci_prov value.
{
	bool is_there = false;

	pthread_mutex_lock(&g_pending_mutex);
	is_there = pending_event.is_there_ci_prov;
	if (is_there == false) {
		pending_event.is_there_ci_prov = true;
	}
	pthread_mutex_unlock(&g_pending_mutex);

	return is_there;
}

void ci_cp_del_pended_data(void)	// [SET] Setting false to Flag about existing of pending-Data.
{
	bool b_continue = true;

	while (b_continue) {
		pthread_mutex_lock(&g_pending_mutex);
		if (pending_event.exist == false || pending_event.using_count == 0) {
			pending_event.exist = false;
			b_continue = false;
		}
		pthread_mutex_unlock(&g_pending_mutex);

		if (b_continue == true) {
			usleep(100000);		// 0.1 seconds delay.
		}
	}
}

void ci_cp_del_is_there_cp(void)	// [SET] Setting false to Flag about existing of new CP-data.
{
	pthread_mutex_lock(&g_pending_mutex);
	pending_event.is_there_ci_prov = false;
	pthread_mutex_unlock(&g_pending_mutex);
}

int ci_cp_pend_event_data(es_cloud_prov_data_s *event_data)	// [SET] Setting true all Flag and Store Event-Data to pending repo.
{
	if (event_data == NULL) {
		THINGS_LOG_E(TAG, "Event Data is NULL.");
		return 0;
	}

	bool b_continue = true;

	while (b_continue) {
		pthread_mutex_lock(&g_pending_mutex);
		if (pending_event.using_count == 0) {
			pending_event.exist = true;
			pending_event.is_there_ci_prov = true;

			if (event_data != &pending_event.event) {
				clone_es_cloud_prov_data(&pending_event.event, event_data);
			}
			b_continue = false;
		}
		pthread_mutex_unlock(&g_pending_mutex);

		if (b_continue == true) {
			usleep(100000);		// 0.1 seconds delay.
		}
	}

	return 1;
}

int ci_cp_enter_pend_data_block(void)
{
	int using_cnt = 0;

	pthread_mutex_lock(&g_pending_mutex);
	pending_event.using_count++;
	using_cnt = pending_event.using_count;
	pthread_mutex_unlock(&g_pending_mutex);

	return using_cnt;
}

int ci_cp_release_penddata_block(void)
{
	int using_cnt = 0;

	pthread_mutex_lock(&g_pending_mutex);
	pending_event.using_count--;
	using_cnt = pending_event.using_count;
	pthread_mutex_unlock(&g_pending_mutex);

	return using_cnt;
}
