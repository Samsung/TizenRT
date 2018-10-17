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
 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include <debug.h>
#include <signal.h>
#include <queue.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <libtuv/uv.h>
#include <libtuv/uv__types.h>
#include <eventloop/eventloop.h>

#include "eventloop_internal.h"

/* The structure for a group of event nodes which have same event type, event_group_t */
struct event_group_s {
	struct event_group_s *flink;
	int type;
	sq_queue_t event_list; // list node type : event_node_t
};
typedef struct event_group_s event_group_t;

/* The structure for wrapping of event handle to be kept in a list internally. */
struct event_node_s {
	struct event_node_s *flink;
	el_event_t *handle;
};
typedef struct event_node_s event_node_t;

/* The structure which has information of event handle user registered.
 * The handle of event_node_t has it in data field, and use data values when calling callback function.
 */
struct event_data_s {
	int type;
	int pid;
	int event_in;
	event_callback func;
	void *cb_data;
	void *event_data;
};
typedef struct event_data_s event_data_t;

sq_queue_t g_event_list;  // list node type : event_group_t

static event_group_t *get_event_group(int type)
{
	event_group_t *ptr;

	if (type < 0) {
		eldbg("Invalid parameter\n");
		return NULL;
	}

	ptr = (event_group_t *)sq_peek(&g_event_list);
	while (ptr != NULL) {
		if (ptr->type == type) {
			return ptr;
		}
		ptr = (event_group_t *)sq_next(ptr);
	}

	return ptr;
}

static bool is_registered_event_cb(el_event_t *handle)
{
	event_group_t *group_ptr;
	event_node_t *node_ptr;

	if (handle == NULL) {
		return false;
	}

	group_ptr = (event_group_t *)sq_peek(&g_event_list);
	while (group_ptr != NULL) {
		node_ptr = (event_node_t *)sq_peek(&group_ptr->event_list);
		while (node_ptr != NULL && node_ptr->handle != NULL) {
			if (node_ptr->handle == handle) {
				return true;
			}
			node_ptr = (event_node_t *)sq_next(node_ptr);
		}
		group_ptr = (event_group_t *)sq_next(group_ptr);
	}

	return false;
}

static event_group_t *eventloop_new_event_group(int type)
{
	event_group_t *event_group;

	event_group = (event_group_t *)EL_ALLOC(sizeof(event_group_t));
	if (event_group == NULL) {
		eldbg("Failed to allocate event group\n");
		return NULL;
	}

	sq_init(&event_group->event_list);
	event_group->flink = NULL;
	event_group->type = type;
	sq_addlast((FAR sq_entry_t *)event_group, &g_event_list);

	return event_group;
}
static int eventloop_register_event_cb(el_event_t *handle)
{
	event_group_t *event_group;
	event_node_t *event_node;
	int type;
	
	if (handle == NULL || handle->data == NULL) {
		eldbg("Invalid Parameter\n");
		return ERROR;
	}

	type = ((event_data_t *)handle->data)->type;
	event_group = get_event_group(type);
	if (event_group == NULL) {
		event_group = eventloop_new_event_group(type);
		if (event_group == NULL) {
			return ERROR;
		}
	}
	event_node = (event_node_t *)EL_ALLOC(sizeof(event_node_t));
	if (event_node == NULL) {
		eldbg("Failed to allocate event node\n");
		if (sq_empty(&event_group->event_list)) {
			sq_rem((FAR sq_entry_t *)event_group, &g_event_list);
			EL_FREE(event_group);
		}
		return ERROR;
	}
	event_node->flink = NULL;
	event_node->handle = handle;
	sq_addlast((FAR sq_entry_t *)event_node, &event_group->event_list);

	return OK;
}

void eventloop_unregister_event_cb(el_event_t *handle)
{
	event_group_t *event_group;
	event_node_t *ptr;
	event_data_t *data;

	if (handle == NULL || handle->data == NULL) {
		return;
	}

	data = (event_data_t *)handle->data;

	event_group = get_event_group(data->type);
	if (event_group != NULL) {
		ptr = (event_node_t *)sq_peek(&event_group->event_list);
		while (ptr != NULL && ptr->handle != NULL) {
			if (ptr->handle == handle) {
				sq_rem((FAR sq_entry_t *)ptr, &event_group->event_list);
				if (data->event_data != NULL) {
					EL_FREE(data->event_data);
				}
				EL_FREE(data);
				EL_FREE(handle);
				EL_FREE(ptr);
				break;
			}
			ptr = (event_node_t *)sq_next(ptr);
		}
		if (sq_empty(&event_group->event_list)) {
			sq_rem((FAR sq_entry_t *)event_group, &g_event_list);
			EL_FREE(event_group);
		}
	}
}

static void event_callback_func(el_event_t *event, int signum)
{
	int ret;
	event_data_t *data = NULL;

	if (event == NULL || event->data == NULL) {
		eldbg("Invalid event callback\n");
		return;
	}

	data = (event_data_t *)event->data;

	elvdbg("[%d] Event callback!! type : %d\n", getpid(), data->type);
	if (data->event_in == data->type && data->func) {
		ret = data->func(data->cb_data, data->event_data);
		/* It is true if eventloop_loop_stop is called in callback function. */
		if (LOOP_IS_STOPPED(event->loop)) {
			return;
		}
		/* If callback function returns EVENTLOOP_CALLBACK_STOP, close and unregister the event handler.  */
		if (ret == EVENTLOOP_CALLBACK_STOP) {
			uv_close((uv_handle_t *)event, (uv_close_cb)eventloop_unregister_event_cb);
			return;
		}
	}
	if (data->event_data != NULL) {
		EL_FREE(data->event_data);
	}
	data->event_data = NULL;
	data->event_in = -1;

	return;
}

static int eventloop_send_event_sig(int type, void *event_data, int data_size)
{
	event_group_t *event_group;
	event_node_t *ptr;

	if (type < 0 || type >= EL_EVENT_MAX || data_size < 0) {
		eldbg("Invalid Parameter\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	event_group = get_event_group(type);
	if (event_group != NULL) {
		ptr = (event_node_t *)sq_peek(&event_group->event_list);
		while (ptr != NULL && ptr->handle != NULL) {
			event_data_t *cb_data = (event_data_t *)ptr->handle->data;
			cb_data->event_in = type;
			if (data_size == 0) {
				cb_data->event_data = NULL;
			} else {
				cb_data->event_data = EL_ALLOC(data_size);
				if (cb_data->event_data == NULL) {
					eldbg("Failed to allocate callback info\n");
					return EVENTLOOP_OUT_OF_MEMORY;
				}
				memcpy(cb_data->event_data, event_data, data_size);
			}
			/* Send signal to task which registered event */
			int ret = kill(cb_data->pid, SIGEL_EVENT);
			if (ret < 0) {
				eldbg("kill failed %d \n", errno);
				if (cb_data->event_data != NULL) {
					EL_FREE(cb_data->event_data);
					cb_data->event_data = NULL;
				}
			}
			ptr = (event_node_t *)sq_next(ptr);
		}
	}

	return OK;
}

el_event_t *eventloop_add_event_handler(int type, event_callback func, void *data)
{
	int ret;
	el_loop_t *loop;
	el_event_t *handle;
	event_data_t *event_cb;

	if (type < 0 || type >= EL_EVENT_MAX || func == NULL) {
		eldbg("Invalid Parameter\n");
		return NULL;
	}

	loop = get_app_loop();
	if (loop == NULL) {
		eldbg("Failed to get loop\n");
		return NULL;
	}

	handle = (el_event_t *)EL_ALLOC(sizeof(el_event_t));
	if (handle == NULL) {
		eldbg("Failed to allocate event\n");
		return NULL;
	}

	event_cb = (event_data_t *)EL_ALLOC(sizeof(event_data_t));
	if (event_cb == NULL) {
		eldbg("Failed to allocate callback\n");
		EL_FREE(handle);
		return NULL;
	}

	event_cb->type = type;
	event_cb->pid = getpid();
	event_cb->event_in = -1;
	event_cb->func = func;
	event_cb->cb_data = data;
	event_cb->event_data = NULL;
	handle->data = (void *)event_cb;

	ret = uv_signal_init(loop, handle);
	if (ret != 0) {
		eldbg("Failed to initialize event\n");
		goto errout;
	}

	ret = uv_signal_start(handle, event_callback_func, SIGEL_EVENT);
	if (ret != 0) {
		eldbg("Failed to initialize event\n");
		goto errout;
	}

	/* Add event handle to a list of handles */
	ret = eventloop_register_event_cb(handle);
	if (ret != OK) {
		eldbg("Failed to register signal for event\n");
		uv_close((uv_handle_t *)handle, NULL);
		goto errout;
	}
	elvdbg("created event handle %p, type = %d\n", handle, type);

	return handle;
errout:
	EL_FREE(event_cb);
	EL_FREE(handle);

	return NULL;
}

int eventloop_del_event_handler(el_event_t *handle)
{
	if (handle == NULL) {
		eldbg("Invalid Parameter\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	if (!is_registered_event_cb(handle) || uv__is_closing(handle)) {
		return EVENTLOOP_INVALID_HANDLE;
	}

	uv_close((uv_handle_t *)handle, (uv_close_cb)eventloop_unregister_event_cb);

	return OK;
}

int eventloop_send_event(int type, void *event_data, int data_size)
{
	if (type < 0 || type >= EL_EVENT_MAX || data_size < 0) {
		eldbg("Invalid Parameter\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	return eventloop_send_event_sig(type, event_data, data_size);
}
