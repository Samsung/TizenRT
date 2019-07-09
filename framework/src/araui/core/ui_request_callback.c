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


#include <pthread.h>
#include <araui/ui_commons.h>
#include "ui_request_callback.h"
#include "libs/ui_list.h"

typedef struct {
	request_callback request_cb;
	void *userdata;
} request_item_t;

static ui_list_t g_request_cb_list;
static pthread_mutex_t g_mutex;

ui_error_t ui_request_callback_init(void)
{
	if (pthread_mutex_init(&g_mutex, NULL)) {
		return UI_INIT_FAILURE;
	}

	pthread_mutex_lock(&g_mutex);

	if (ui_list_init(&g_request_cb_list) != UI_OK) {
		pthread_mutex_unlock(&g_mutex);
		return UI_INIT_FAILURE;
	}

	pthread_mutex_unlock(&g_mutex);

	return UI_OK;
}

ui_error_t ui_request_callback_deinit(void)
{
	pthread_mutex_lock(&g_mutex);

	if (ui_list_deinit(&g_request_cb_list) != UI_OK) {
		pthread_mutex_unlock(&g_mutex);
		return UI_DEINIT_FAILURE;
	}

	pthread_mutex_unlock(&g_mutex);

	pthread_mutex_destroy(&g_mutex);

	return UI_OK;
}

ui_error_t ui_request_callback(request_callback request_cb, void *userdata)
{
	request_item_t *item;

	if (!request_cb) {
		return UI_INVALID_PARAM;
	}

	item = (request_item_t *)UI_ALLOC(sizeof(request_item_t));
	if (!item) {
		return UI_NOT_ENOUGH_MEMORY;
	}

	item->request_cb = request_cb;
	item->userdata = userdata;

	pthread_mutex_lock(&g_mutex);

	if (ui_list_push_back(&g_request_cb_list, item) != UI_OK) {
		pthread_mutex_unlock(&g_mutex);
		return UI_OPERATION_FAIL;
	}

	pthread_mutex_unlock(&g_mutex);

	return UI_OK;
}

void ui_process_all_requests(void)
{
	request_item_t *item;

	while (true) {
		pthread_mutex_lock(&g_mutex);

		if (ui_list_empty(&g_request_cb_list)) {
			pthread_mutex_unlock(&g_mutex);
			break;
		}

		item = ui_list_front(&g_request_cb_list);
		(void)ui_list_remove_front(&g_request_cb_list);

		pthread_mutex_unlock(&g_mutex);

		item->request_cb(item->userdata);

		UI_FREE(item);
	}
}
