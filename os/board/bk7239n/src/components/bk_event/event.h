// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <os/os.h>
#include "bk_list.h"
#include <components/event.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EVENT_TAG "event"
#define EVENT_LOGI(...) BK_LOGI(EVENT_TAG, ##__VA_ARGS__)
#define EVENT_LOGW(...) BK_LOGW(EVENT_TAG, ##__VA_ARGS__)
#define EVENT_LOGE(...) BK_LOGE(EVENT_TAG, ##__VA_ARGS__)

#define EVENT_DEBUG     0
#if EVENT_DEBUG
#define EVENT_LOGD(...) BK_LOGI(EVENT_TAG, ##__VA_ARGS__)
#else
#define EVENT_LOGD(...) BK_LOGD(EVENT_TAG, ##__VA_ARGS__)
#endif

#ifndef CFG_EVENT_TASK_PRIORITY
#define EVENT_TASK_PRIORITY   5
#else
#define EVENT_TASK_PRIORITY   CFG_EVENT_TASK_PRIORITY
#endif

#ifndef CFG_EVENT_TASK_STACK_SIZE
#define EVENT_TASK_STACK_SIZE 2048
#else
#define EVENT_TASK_STACK_SIZE CFG_EVENT_TASK_STACK_SIZE
#endif

#define EVENT_QUEUE_SIZE      16

typedef struct {
	event_module_t event_module_id;
	struct list_head next;
	struct list_head event_node_list;
	struct list_head cb_list;
} event_module_node_t;

typedef struct {
	int event_id;
	struct list_head next;
	struct list_head cb_list;
} event_node_t;

typedef struct {
	event_cb_t event_cb;
	void *event_cb_arg;
	struct list_head next;
} event_cb_node_t;

typedef struct {
	event_module_t event_module_id;
	int event_id;
	void *event_data;
} event_info_t;

typedef struct {
	event_module_t event_module_id;
	int event_id;
	event_cb_t event_cb;
	void *event_cb_arg;
} event_register_info_t;

enum {
	EVENT_MSG_REGISTER = 0,
	EVENT_MSG_UNREGISTER,
	EVENT_MSG_DEINIT,
	EVENT_MSG_POST,
#if EVENT_DEBUG
	EVENT_MSG_DUMP,
#endif
	EVENT_MSG_COUNT,
};

typedef struct {
	int msg_type;
	int  sync_msg_ret;
	beken_semaphore_t sync_msg_sem;
	bool is_sync_msg;
	union {
		event_info_t event_info;
		event_register_info_t register_info;
	} msg;
} event_msg_t;

#ifdef __cplusplus
}
#endif
