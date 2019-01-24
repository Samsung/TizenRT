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
#ifndef _TINY_EVENT_MANAGER_H__
#define _TINY_EVENT_MANAGER_H__

#include <stdint.h>

typedef enum _tem_result {
	TINY_EVTMGR_SUCCESS,
	TINY_EVTMGR_FAIL,
} tem_result;

typedef struct _tem_msg {
	void *data;
} tem_msg;

typedef struct _tem_evt_attr {
	char *task_name;
	uint32_t stack_size;
} tem_evt_attr;

typedef struct tem_context *tem_hnd;

/*
 * Event loop which receive the message that an application pushed
 */
typedef void *(*tem_evt_loop)(tem_msg *msg);

/*
 * Free message which are stored in task queue
 * Tiny event manager couldn't know what data type is stored in tem_msg
 * So, application have to register tem_free_msg message to stop evtmgr.
 */
typedef void *(*tem_free_msg)(tem_msg *msg);

tem_result tiny_evtmgr_init(tem_hnd *hnd);

tem_result tiny_evtmgr_deinit(tem_hnd hnd);

/**
 * Event loop
 */
// asynchronous function
tem_result tiny_evtmgr_start(tem_hnd hnd, tem_evt_loop evtloop, tem_free_msg rfunc, tem_evt_attr *attr);

// synchronous function
tem_result tiny_evtmgr_stop(tem_hnd hnd);

/**
 * Message
 */
tem_result tiny_evtmgr_add_msg(tem_hnd hnd, tem_msg *msg, uint32_t delay);

#endif // _TINY_EVENT_MANAGER_H__
