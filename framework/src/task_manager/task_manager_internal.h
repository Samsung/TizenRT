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

#ifndef __TASK_MANAGER_INTERNAL_H__
#define __TASK_MANAGER_INTERNAL_H__

#include <stdbool.h>
#include <unistd.h>

/* Command Types */
#define TASKMGR_REGISTER                0
#define TASKMGR_UNREGISTER              1
#define TASKMGR_START                   2
#define TASKMGR_TERMINATE               3
#define TASKMGR_RESTART                 4
#define TASKMGR_PAUSE                   5
#define TASKMGR_RESUME                  6
#define TASKMGR_UNICAST                 7
#define TASKMGR_BROADCAST               8
#define TASKMGR_SET_UNICAST_CB          9
#define TASKMGR_SET_BROADCAST_CB        10
#define TASKMGR_SCAN_NAME               11
#define TASKMGR_SCAN_HANDLE             12
#define TASKMGR_SCAN_GROUP              13

/* Message Queue Values */
#define TM_MQ_PRIO   50
#define TM_PUBLIC_MQ "tm_public_mq"
#define TM_PRIVATE_MQ "tm_priv_mq"

/* Wrapper of allocation APIs */
#define TM_ALLOC(a)  malloc(a)
#define TM_FREE(a)   free(a)

typedef void (*_tm_unicast_t)(void *);
typedef void (*_tm_broadcast_t)(int);

struct task_list_s {
	int pid;
	void *addr;
};
typedef struct task_list_s task_list_t;

struct task_list_data_s {
	int builtin_idx;
	int pid;
	int tm_gid;
	int status;
	int permission;
	int msg_mask;
	_tm_unicast_t unicast_cb;
	_tm_broadcast_t broadcast_cb;
};
typedef struct task_list_data_s task_list_data_t;

struct tm_request_s {
	int cmd;
	int caller_pid;
	int handle;
	int timeout;
	char *q_name;
	void* data;
};
typedef struct tm_request_s tm_request_t;

struct tm_response_s {
	int status;
	task_info_list_t *data;
};
typedef struct tm_response_s tm_response_t;

struct tm_broadcast_s {
	int msg_mask;
	_tm_broadcast_t cb;
};
typedef struct tm_broadcast_s tm_broadcast_t;

#define IS_INVALID_HANDLE(i) (i < 0 || i >= CONFIG_TASK_MANAGER_MAX_TASKS)

#define TASK_LIST_ADDR(handle)       ((task_list_data_t *)tm_task_list[handle].addr)
#define TASK_PID(handle)             tm_task_list[handle].pid
#define TASK_BUILTIN_IDX(handle)     TASK_LIST_ADDR(handle)->builtin_idx
#define TASK_TM_GID(handle)          TASK_LIST_ADDR(handle)->tm_gid
#define TASK_STATUS(handle)          TASK_LIST_ADDR(handle)->status
#define TASK_PERMISSION(handle)      TASK_LIST_ADDR(handle)->permission
#define TASK_MSG_MASK(handle)        TASK_LIST_ADDR(handle)->msg_mask
#define TASK_UNICAST_CB(handle)      TASK_LIST_ADDR(handle)->unicast_cb
#define TASK_BROADCAST_CB(handle)    TASK_LIST_ADDR(handle)->broadcast_cb

extern task_list_t tm_task_list[CONFIG_TASK_MANAGER_MAX_TASKS];

int taskmgr_send_request(tm_request_t *request_msg);
int taskmgr_send_response(char *q_name, tm_response_t *response_msg);
int taskmgr_receive_response(char *q_name, tm_response_t *response_msg, int timeout);

bool taskmgr_is_permitted(int handle, pid_t pid);
int taskmgr_get_task_state(int handle);
int taskmgr_get_drvfd(void);
int taskmgr_get_handle_by_pid(int pid);

#endif
