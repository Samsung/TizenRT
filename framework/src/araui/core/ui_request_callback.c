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
#include <vec/vec.h>
#include <araui/ui_commons.h>
#include "ui_request_callback.h"

typedef struct {
	request_callback request_cb;
	void *userdata;
} request_item_t;

static vec_void_t g_reqcb_list;
static pthread_mutex_t g_mutex;

ui_error_t ui_request_callback_init(void)
{
	if (pthread_mutex_init(&g_mutex, NULL)) {
		return UI_INIT_FAILURE;
	}

	pthread_mutex_lock(&g_mutex);
	vec_init(&g_reqcb_list);
	pthread_mutex_unlock(&g_mutex);

	return UI_OK;
}

ui_error_t ui_request_callback_deinit(void)
{
	pthread_mutex_lock(&g_mutex);
	vec_deinit(&g_reqcb_list);
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

	if (vec_push(&g_reqcb_list, item) != UI_OK) {
		pthread_mutex_unlock(&g_mutex);
		return UI_OPERATION_FAIL;
	}

	pthread_mutex_unlock(&g_mutex);

	return UI_OK;
}

void ui_process_all_requests(void)
{
	request_item_t *item;
	int iter;

	pthread_mutex_lock(&g_mutex);

	// Below logic will work correctly.
	// Actually, in the request_cb function, a new request callback can be added.
	// But it will be added only at the end of the vector.
	// So, this vec_foreach will traverse all items including dynamically added item.
	vec_foreach(&g_reqcb_list, item, iter) {
		pthread_mutex_unlock(&g_mutex);
		item->request_cb(item->userdata);
		UI_FREE(item);
		pthread_mutex_lock(&g_mutex);
	}

	vec_clear(&g_reqcb_list);
	pthread_mutex_unlock(&g_mutex);
}
