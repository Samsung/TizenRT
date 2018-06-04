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
#define TASKMGT_REGISTER_TASK           0
#define TASKMGT_REGISTER_THREAD         1
#define TASKMGT_START                   2
#define TASKMGT_TERMINATE               3
#define TASKMGT_RESTART                 4
#define TASKMGT_PAUSE                   5
#define TASKMGT_RESUME                  6
#define TASKMGT_SCAN                    7
#define TASKMGT_UNICAST                 8
#define TASKMGT_BROADCAST               9
#define TASKMGT_SET_UNICAST_CB          10
#define TASKMGT_SCAN_NAME               11
#define TASKMGT_SCAN_HANDLE             12
#define TASKMGT_SCAN_GROUP              13
#define TASKMGT_UNREGISTER_TASK         14
#define TASKMGT_SET_BROADCAST_HANDLER   15

/* Message Queue Values */
#define TM_MQ_PRIO   50
#define TM_PUBLIC_MQ "tm_public_mq"
#define TM_PRIVATE_MQ "tm_priv_mq"

/* Wrapper of allocation APIs */
#define TM_ALLOC(a)  malloc(a)
#define TM_FREE(a)   free(a)

typedef void (*_tm_callback_t)(void *);

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
	_tm_callback_t unicast_cb;
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

#define IS_INVALID_HANDLE(i) (i < 0 || i >= CONFIG_TASK_MANAGER_MAX_TASKS)

#define TASK_LIST_ADDR(handle)       ((task_list_data_t *)tm_task_list[handle].addr)
#define TASK_PID(handle)             tm_task_list[handle].pid
#define TASK_BUILTIN_IDX(handle)     TASK_LIST_ADDR(handle)->builtin_idx
#define TASK_TM_GID(handle)          TASK_LIST_ADDR(handle)->tm_gid
#define TASK_STATUS(handle)          TASK_LIST_ADDR(handle)->status
#define TASK_PERMISSION(handle)      TASK_LIST_ADDR(handle)->permission
#define TASK_MSG_MASK(handle)        TASK_LIST_ADDR(handle)->msg_mask
#define TASK_UNICAST_CB(handle)      TASK_LIST_ADDR(handle)->unicast_cb

extern task_list_t tm_task_list[CONFIG_TASK_MANAGER_MAX_TASKS];

int taskmgr_send_request(tm_request_t *request_msg);
int taskmgr_send_response(char *q_name, tm_response_t *response_msg);
int taskmgr_receive_response(char *q_name, tm_response_t *response_msg, int timeout);

bool taskmgr_is_permitted(int handle, pid_t pid);
int taskmgr_get_task_state(int handle);
int taskmgr_get_drvfd(void);
int taskmgr_get_handle_by_pid(int pid);

#endif
