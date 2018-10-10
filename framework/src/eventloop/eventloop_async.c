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
#include <tinyara/config.h>
#include <unistd.h>
#include <queue.h>
#include <semaphore.h>
#include <debug.h>
#include <sys/types.h>
#include <libtuv/uv.h>
#include <eventloop/eventloop.h>

#include "eventloop_internal.h"

struct thread_safe_func_s {
	struct thread_safe_func_s *flink;
	thread_safe_callback func;
	sem_t func_sem;
	int refs;
};
typedef struct thread_safe_func_s thread_safe_func_t;

struct thread_safe_cb_list_s {
	sq_queue_t cb_list;
	el_async_t *async_handle;
	sem_t list_sem;
};
typedef struct thread_safe_cb_list_s thread_safe_cb_list_t;

struct thread_safe_cb_s {
	struct thread_safe_cb_s *flink;
	thread_safe_func_t *func_handle;
	void *cb_data;
};
typedef struct thread_safe_cb_s thread_safe_cb_t;

static thread_safe_cb_list_t g_thread_safe_cb_list[CONFIG_MAX_TASKS];
static sq_queue_t g_thread_safe_func_list;

#define CB_LIST(idx)         g_thread_safe_cb_list[idx].cb_list
#define ASYNC_HANDLE(idx)    g_thread_safe_cb_list[idx].async_handle
#define LIST_SEM(idx)        g_thread_safe_cb_list[idx].list_sem

/* Define for the hash operation */
#define MAX_PID_MASK                       (CONFIG_MAX_TASKS - 1)
#define THREAD_SAFE_CB_LIST_IDX_HASH(pid)  ((pid) & MAX_PID_MASK)

static thread_safe_func_t *eventloop_get_func_handle(thread_safe_callback func)
{
	thread_safe_func_t *ret_func_handle;

	if (func == NULL) {
		eldbg("Invalid Parameter!\n");
		return NULL;
	}

	ret_func_handle = (thread_safe_func_t *)sq_peek(&g_thread_safe_func_list);
	while (ret_func_handle) {
		if (ret_func_handle->func == func) {
			break;
		}
		ret_func_handle = (thread_safe_func_t *)sq_next(ret_func_handle);
	}

	return ret_func_handle;
}

static thread_safe_func_t *eventlooop_func_handle_add(thread_safe_callback func)
{
	thread_safe_func_t *func_handle;

	if (func == NULL) {
		eldbg("Invalid Parameter!\n");
		return NULL;
	}

	func_handle = (thread_safe_func_t *)EL_ALLOC(sizeof(thread_safe_func_t));
	if (func_handle == NULL) {
		eldbg("Failed to allocate func_handle!\n");
		return NULL;
	}
	func_handle->func = func;
	func_handle->flink = NULL;
	func_handle->refs = 0;
	sem_init(&func_handle->func_sem, 0, 1);
	sq_addlast((sq_entry_t *)func_handle, &g_thread_safe_func_list);

	return func_handle;
}

static void eventloop_func_handle_remove(thread_safe_func_t *func_handle)
{
	if (func_handle == NULL) {
		eldbg("Invalid Parameter!\n");
		return;
	}

	sem_destroy(&func_handle->func_sem);
	func_handle->func = NULL;
	sq_rem((sq_entry_t *)func_handle, &g_thread_safe_func_list);
	EL_FREE(func_handle);
	func_handle = NULL;
}

static int eventloop_thread_safe_cb_list_node_init(int list_idx, thread_safe_func_t *func_handle)
{
	if (list_idx < 0 || list_idx > CONFIG_MAX_TASKS || func_handle == NULL) {
		eldbg("Invalid Parameter!\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	ASYNC_HANDLE(list_idx) = (el_async_t *)EL_ALLOC(sizeof(el_async_t));
	if (ASYNC_HANDLE(list_idx) == NULL) {
		eldbg("Failed to allocate handle!\n");
		return EVENTLOOP_OUT_OF_MEMORY;
	}
	
	sq_init(&CB_LIST(list_idx));
	sem_init(&LIST_SEM(list_idx), 0, 1);
	return OK;
}

static void eventloop_thread_safe_cb_list_node_deinit(int list_idx)
{
	if (list_idx < 0 || list_idx > CONFIG_MAX_TASKS) {
		eldbg("Invalid Parameter!\n");
		return;
	}

	ASYNC_HANDLE(list_idx)->async_cb = NULL;
	EL_FREE(ASYNC_HANDLE(list_idx));
	ASYNC_HANDLE(list_idx) = NULL;
	sem_destroy(&LIST_SEM(list_idx));
}

static thread_safe_cb_t *eventloop_thread_safe_cb_generate(thread_safe_func_t *func_handle, void *cb_data)
{
	thread_safe_cb_t *thread_safe_cb;

	if (func_handle == NULL) {
		eldbg("Invalid Parameter!\n");
		return NULL;
	}

	thread_safe_cb = (thread_safe_cb_t *)EL_ALLOC(sizeof(thread_safe_cb_t));
	if (thread_safe_cb == NULL) {
		eldbg("Failed to allocate async callback data!\n");
		return NULL;
	}
	thread_safe_cb->flink = NULL;
	thread_safe_cb->func_handle = func_handle;
	thread_safe_cb->cb_data = cb_data;
	
	return thread_safe_cb;
}

static void eventloop_thread_safe_cb_destroy(thread_safe_cb_t *thread_safe_cb)
{
	if (thread_safe_cb == NULL) {
		eldbg("Invalid Parameter!\n");
		return;
	}

	EL_FREE(thread_safe_cb);
}

void eventloop_unregister_thread_safe_cb(el_async_t *handle)
{
	int thread_safe_cb_list_idx;
	thread_safe_cb_t *curr;

	thread_safe_cb_list_idx = THREAD_SAFE_CB_LIST_IDX_HASH(getpid());
	while ((curr = (thread_safe_cb_t *)sq_remfirst(&CB_LIST(thread_safe_cb_list_idx)))) {
		curr->func_handle->refs--;
		if (curr->func_handle->refs == 0) {
			eventloop_func_handle_remove(curr->func_handle);
		}
		EL_FREE(curr);
	}

	eventloop_thread_safe_cb_list_node_deinit(thread_safe_cb_list_idx);
}

static void eventloop_thread_safe_cb_add(thread_safe_cb_t *thread_safe_cb, int list_idx)
{
	if (list_idx < 0 || list_idx > CONFIG_MAX_TASKS || thread_safe_cb == NULL) {
		eldbg("Invalid Parameter!\n");
		return;
	}

	sq_addlast((sq_entry_t *)thread_safe_cb, &CB_LIST(list_idx));
	sem_wait(&thread_safe_cb->func_handle->func_sem);
	thread_safe_cb->func_handle->refs++;
	sem_post(&thread_safe_cb->func_handle->func_sem);
}

static void eventloop_async_callback(el_async_t *handle)
{
	int thread_safe_cb_list_idx;
	el_loop_t *loop;
	thread_safe_cb_t *curr;

	loop = get_app_loop();
	if (loop == NULL) {
		eldbg("Failed to get loop\n");
		return;
	}

	thread_safe_cb_list_idx = THREAD_SAFE_CB_LIST_IDX_HASH(getpid());
	while ((curr = (thread_safe_cb_t *)sq_remfirst(&CB_LIST(thread_safe_cb_list_idx)))) {
		if (curr->func_handle->refs > 0) {
			if (curr->func_handle->func != NULL) {
				sem_wait(&curr->func_handle->func_sem);
				curr->func_handle->func(curr->cb_data);
				curr->func_handle->refs--;
				if (curr->func_handle->refs == 0) {
					sem_post(&curr->func_handle->func_sem);
					eventloop_func_handle_remove(curr->func_handle);
				} else {
					sem_post(&curr->func_handle->func_sem);
				}
			}
			EL_FREE(curr);
		}
		/* It is true if eventloop_loop_stop is called in callback function. */
		if (LOOP_IS_STOPPED(handle->loop)) {
			return;
		}
	}

	uv_async_deinit(loop, ASYNC_HANDLE(thread_safe_cb_list_idx));
	eventloop_thread_safe_cb_list_node_deinit(thread_safe_cb_list_idx);
}

int eventloop_thread_safe_function_call(thread_safe_callback func, void *cb_data)
{
	int ret;
	int thread_safe_cb_list_idx;
	el_loop_t *loop;
	thread_safe_cb_t *thread_safe_cb;
	thread_safe_func_t *func_handle;

	if (func == NULL) {
		eldbg("Invalid Parameter!\n");
		return EVENTLOOP_INVALID_PARAM;
	}

	loop = get_app_loop();
	if (loop == NULL) {
		eldbg("Failed to get loop\n");
		return EVENTLOOP_LOOP_FAIL;
	}

	func_handle = eventloop_get_func_handle(func);
	if (func_handle == NULL) {
		func_handle = eventlooop_func_handle_add(func);
		if (func_handle == NULL) {
			return EVENTLOOP_OUT_OF_MEMORY;
		}
	}

	thread_safe_cb = eventloop_thread_safe_cb_generate(func_handle, cb_data);
	if (thread_safe_cb == NULL) {
		ret = EVENTLOOP_OUT_OF_MEMORY;
		goto errout;
	}

	thread_safe_cb_list_idx = THREAD_SAFE_CB_LIST_IDX_HASH(getpid());
	if (ASYNC_HANDLE(thread_safe_cb_list_idx) == NULL) {
		ret = eventloop_thread_safe_cb_list_node_init(thread_safe_cb_list_idx, func_handle);
		if (ret != OK) {
			eventloop_thread_safe_cb_destroy(thread_safe_cb);
			goto errout;
		}
		ret = uv_async_init(loop, ASYNC_HANDLE(thread_safe_cb_list_idx), (uv_async_cb)eventloop_async_callback);
		if (ret < 0) {
			eldbg("Failed to initiate async!\n");
			eventloop_thread_safe_cb_destroy(thread_safe_cb);
			eventloop_thread_safe_cb_list_node_deinit(thread_safe_cb_list_idx);
			ret = EVENTLOOP_OPERATION_FAIL;
			goto errout;
		}
		sem_wait(&LIST_SEM(thread_safe_cb_list_idx));
		ret = uv_async_send(ASYNC_HANDLE(thread_safe_cb_list_idx));
		if (ret < 0) {
			eldbg("Failed to send async signal to main loop!");
			eventloop_thread_safe_cb_destroy(thread_safe_cb);
			eventloop_thread_safe_cb_list_node_deinit(thread_safe_cb_list_idx);
			uv_async_deinit(loop, ASYNC_HANDLE(thread_safe_cb_list_idx));
			sem_post(&LIST_SEM(thread_safe_cb_list_idx));
			ret = EVENTLOOP_OPERATION_FAIL;
			goto errout;
		}
	} else {
		sem_wait(&LIST_SEM(thread_safe_cb_list_idx));
		ret = uv_async_send(ASYNC_HANDLE(thread_safe_cb_list_idx));
		if (ret < 0) {
			eldbg("Failed to send async signal to main loop!");
			eventloop_thread_safe_cb_destroy(thread_safe_cb);
			sem_post(&LIST_SEM(thread_safe_cb_list_idx));
			ret = EVENTLOOP_OPERATION_FAIL;
			goto errout;
		}
	}

	eventloop_thread_safe_cb_add(thread_safe_cb, thread_safe_cb_list_idx);
	sem_post(&LIST_SEM(thread_safe_cb_list_idx));

	return OK;
errout:
	if (func_handle->refs == 0) {
		eventloop_func_handle_remove(func_handle);
	}

	return ret;
}
