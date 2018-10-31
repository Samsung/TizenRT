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
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <eventloop/eventloop.h>

#include "eventloop_internal.h"

/* The structure for a group of event nodes which have same event type, event_group_t */
struct event_group_s {
	struct event_group_s *flink;
	int type;
	sq_queue_t event_list; // list node type : event_node_t
};
typedef struct event_group_s event_group_t;

sq_queue_t g_event_list;  // list node type : event_group_t

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

static void eventloop_unregister_event_type(int type)
{
	el_handle_t *handle;
	event_group_t *event_group;

	if (type < 0 || type >= EL_EVENT_MAX) {
		return;
	}

	event_group = get_event_group(type);
	if (event_group != NULL) {
		handle = (el_handle_t *)sq_peek(&event_group->event_list);
		while (handle) {
			if (handle->pid == getpid()) {
				sq_rem((FAR sq_entry_t *)handle, &event_group->event_list);
				eventloop_decrease_app_handle_cnt(handle->pid);
				EL_FREE(handle);
				handle = NULL;
				break;
			}
			handle = (el_handle_t *)sq_next(handle);
		}
		if (sq_empty(&event_group->event_list)) {
			sq_rem((FAR sq_entry_t *)event_group, &g_event_list);
			EL_FREE(event_group);
			event_group = NULL;
		}
	}
}

static int eventloop_register_event_handle(el_handle_t *handle)
{
	el_handle_t *handle_ptr;
	el_event_t *event_handler;
	event_group_t *event_group;
	
	if (handle == NULL) {
		eldbg("Invalid Parameter\n");
		return ERROR;
	}

	event_handler = (el_event_t *)&handle->data.event;

	event_group = get_event_group(event_handler->event_type);
	if (event_group == NULL) {
		event_group = eventloop_new_event_group(event_handler->event_type);
		if (event_group == NULL) {
			return ERROR;
		}
		handle_ptr = (el_handle_t *)sq_peek(&event_group->event_list);
		while (handle_ptr) {
			if (handle_ptr->pid == handle->pid) {
				/* Remove old event handler for type */
				sq_rem((FAR sq_entry_t *)handle_ptr, &event_group->event_list);
				EL_FREE(handle_ptr);
				break;
			}
			handle_ptr = (el_handle_t *)sq_next(handle_ptr);
		}
	}

	sq_addlast((FAR sq_entry_t *)handle, &event_group->event_list);
	eventloop_increase_app_handle_cnt(handle->pid);

	return OK;
}

static int eventloop_send_event_handle(el_handle_t *handle, void *event_data, int data_size)
{
	int ret;
	bool is_empty;
	el_handle_t *new_handle;
	el_event_t *event;

	if (handle == NULL || data_size < 0) {
		eldbg("Invalid Parameter\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	new_handle = (el_handle_t *)EL_ALLOC(sizeof(el_handle_t));
	if (new_handle == NULL) {
		return EVENTLOOP_OUT_OF_MEMORY;
	}
	memcpy(new_handle, handle, sizeof(el_handle_t));

	event = &new_handle->data.event;
	if (data_size == 0) {
		event->event_data = NULL;
	} else {
		event->event_data = EL_ALLOC(data_size);
		if (event->event_data == NULL) {
			eldbg("Failed to allocate callback info\n");
			return EVENTLOOP_OUT_OF_MEMORY;
		}
		memcpy(event->event_data, event_data, data_size);
	}

	ret = eventloop_job_empty(handle, &is_empty);
	if (ret != OK) {
		return EVENTLOOP_OPERATION_FAIL;
	}

	/* Add handle to a queue of jobs */
	ret = eventloop_push_job(new_handle);
	if (ret == OK && is_empty) {
		/* Send signal to notify event */
		ret = kill(new_handle->pid, SIGEL_EVENT);
		if (ret == OK) {
			elvdbg("[%d] Sending event signal to %d Success\n", getpid(), new_handle->pid);
		} else {
			eldbg("Fail to send signal, errno %d\n", errno);
			return EVENTLOOP_OPERATION_FAIL;
		}
	}

	/* Wake up task */
	if (eventloop_get_state(new_handle->pid) == EL_STATE_WAIT_WAKEUP) {
		dbg("[%d] SEND WAKEUP [%d]\n", getpid(), new_handle->pid);
		ret = kill(new_handle->pid, SIGEL_WAKEUP);
		if (ret == OK) {
			elvdbg("[%d] Sending wake up signal to %d Success\n", getpid(), new_handle->pid);
		} else {
			eldbg("Fail to send signal, errno %d\n", errno);
			return EVENTLOOP_OPERATION_FAIL;
		}
	}

	return OK;
}

void eventloop_clean_event_handle(el_handle_t *handle)
{
	el_event_t *event;

	if (handle == NULL) {
		return;
	}

	event = (el_event_t *)&handle->data.event;

	/* Clean event handle */
	if (event->event_data != NULL) {
		EL_FREE(event->event_data);
		event->event_data = NULL;
	}
	EL_FREE(handle);
}

/* Remove all event handlers that task/pthread 'pid' registered */
void eventloop_clean_app_event_handles(void)
{
	int i;
	el_handle_t *handle;
	event_group_t *event_group;

	for (i = 1; i < EL_EVENT_MAX; i++) {
		event_group = get_event_group(i);
		if (event_group != NULL) {
			handle = (el_handle_t *)sq_peek(&event_group->event_list);
			while (handle) {
				if (handle->pid == getpid() && handle->mode == EL_MODE_DEFAULT) {
					sq_rem((FAR sq_entry_t *)handle, &event_group->event_list);
					eventloop_decrease_app_handle_cnt(handle->pid);
					eventloop_clean_event_handle(handle);
					break;
				}
				handle = (el_handle_t *)sq_next(handle);
			}
			if (sq_empty(&event_group->event_list)) {
				sq_rem((FAR sq_entry_t *)event_group, &g_event_list);
				EL_FREE(event_group);
			}
		}
	}
}

/* Event callback function called in user side */
void eventloop_event_user_callback(el_handle_t *handle)
{
	int ret;
	el_event_t *event;

	if (handle == NULL) {
		return;
	}

	elvdbg("[%d] Event Callback!\n", getpid());

	event = (el_event_t *)&handle->data.event;
	if (event->cb_func) {
		/* Execute event callback */
		ret = event->cb_func(event->cb_data, event->event_data);
		if (ret == EVENTLOOP_CALLBACK_STOP) {
			eventloop_unregister_event_type(event->event_type);
		}
	}

	eventloop_clean_event_handle(handle);
}

el_event_t *eventloop_add_event_handler(int type, event_callback func, void *data)
{
	int ret;
	el_handle_t *handle;
	el_event_t *event_handler;

	if (type < 0 || type >= EL_EVENT_MAX || func == NULL) {
		eldbg("Invalid Parameter\n");
		return NULL;
	}

	handle = (el_handle_t *)EL_ALLOC(sizeof(el_handle_t));
	if (handle == NULL) {
		return NULL;
	}
	handle->pid = getpid();
	handle->type = EL_HANDLE_TYPE_EVENT;
	handle->mode = EL_MODE_DEFAULT;

	event_handler = (el_event_t *)&handle->data.event;
	event_handler->event_type = type;
	event_handler->cb_func = func;
	event_handler->cb_data = data;
	event_handler->event_data = NULL;

	/* Set a handler to call user callback */
	ret = eventloop_set_user_cb_handler();
	if (ret != OK) {
		EL_FREE(handle);
		return NULL;
	}

	/* Register event handle */
	ret = eventloop_register_event_handle(handle);
	if (ret != OK) {
		EL_FREE(handle);
		return NULL;
	}

	return event_handler;
}

int eventloop_del_event_handler(el_event_t *event_handler)
{
	el_handle_t *handle;
	event_group_t *event_group;

	if (event_handler == NULL) {
		eldbg("Invalid parameter\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	event_group = get_event_group(event_handler->event_type);
	if (event_group != NULL) {
		handle = (el_handle_t *)sq_peek(&event_group->event_list);
		while (handle) {
			if (&handle->data.event == event_handler) {
				sq_rem((FAR sq_entry_t *)handle, &event_group->event_list);
				eventloop_decrease_app_handle_cnt(handle->pid);
				if (event_handler->event_data != NULL) {
					EL_FREE(event_handler->event_data);
				}
				EL_FREE(handle);
				return OK;
			}
			handle = (el_handle_t *)sq_next(handle);
		}
		if (sq_empty(&event_group->event_list)) {
			sq_rem((FAR sq_entry_t *)event_group, &g_event_list);
			EL_FREE(event_group);
		}
	}
	return EVENTLOOP_INVALID_HANDLE;
}

int eventloop_send_event(int type, void *event_data, int data_size)
{
	int ret;
	el_handle_t *handle;
	event_group_t *event_group;

	if (type < 0 || type >= EL_EVENT_MAX || data_size < 0) {
		eldbg("Invalid Parameter\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	event_group = get_event_group(type);
	if (event_group != NULL) {
		handle = (el_handle_t *)sq_peek(&event_group->event_list);
		while (handle) {
			ret = eventloop_send_event_handle(handle, event_data, data_size);
			if (ret != OK) {
				return ret;
			}
			handle = (el_handle_t *)sq_next(handle);
		}
	}
	return OK;
}

int eventloop_set_event_handler_mode(el_event_t *event_handler, int mode)
{
	el_handle_t *handle;
	event_group_t *event_group;

	if (event_handler == NULL || mode < 0 || mode >= EL_MODE_MAX) {
		eldbg("Invalid Parameter\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	event_group = get_event_group(event_handler->event_type);
	if (event_group != NULL) {
		handle = (el_handle_t *)sq_peek(&event_group->event_list);
		while (handle) {
			if (&handle->data.event == event_handler) {
				handle->mode = mode;
				return OK;
			}
			handle = (el_handle_t *)sq_next(handle);
		}
	}
	return EVENTLOOP_INVALID_HANDLE;
}
