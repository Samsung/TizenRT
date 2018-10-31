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

#ifndef __EVENTLOOP_INTERNAL_H__
#define __EVENTLOOP_INTERNAL_H__

#include <stdlib.h>
#include <eventloop/eventloop.h>

/* Wrapper of allocation APIs */
#define EL_ALLOC(a)  malloc(a)
#define EL_FREE(a)   free(a)

/**
 * @brief Handle Type of Event Loop
 */
enum el_handle_type_e {
	EL_HANDLE_TYPE_TIMER = 1,
	EL_HANDLE_TYPE_EVENT = 2,
	EL_HANDLE_TYPE_MAX,
};

/**
 * @brief App State in Event loop
 */
enum el_state_e {
	EL_STATE_NORMAL = 0,
	EL_STATE_WAIT_WAKEUP = 1,
	EL_STATE_MAX,
};

/**
 * @brief EventLoop Handle structure
 */
struct el_handle_s {
	struct el_handle_s *flink;
	int pid;
	int type;
	int mode;
	union {
		el_timer_t timer;
		el_event_t event;
	} data;
};
typedef struct el_handle_s el_handle_t;

struct el_app_handle_s {
	sq_queue_t jobs;
	int count;
	int state;
};
typedef struct el_app_handle_s el_app_handle_t;

#define EL_APP_STATE(app_handle)       app_handle->state
#define EL_APP_JOB_COUNT(app_handle)   app_handle->count
#define EL_APP_JOBS(app_handle)        app_handle->jobs

int get_async_task(void);

int eventloop_push_job(el_handle_t *handle);
el_handle_t *eventloop_pop_job(void);
int eventloop_job_empty(el_handle_t *handle, bool *result);
void eventloop_increase_app_handle_cnt(int pid);
void eventloop_decrease_app_handle_cnt(int pid);
int eventloop_get_app_handle_cnt(void);
int eventloop_get_state(int pid);
void eventloop_set_state(int state);


int eventloop_set_user_cb_handler(void);

void eventloop_clean_timer(el_handle_t *handle);
void eventloop_clean_event_handle(el_handle_t *handle);

void eventloop_timer_user_callback(el_handle_t *handle);
void eventloop_event_user_callback(el_handle_t *handle);

#endif
