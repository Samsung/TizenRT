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

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#ifndef CONFIG_DISABLE_PTHREAD
#include <pthread.h>
#endif
#include <sys/types.h>

/* Command Types */
#define TASKMGRCMD_REGISTER                0
#define TASKMGRCMD_UNREGISTER              1
#define TASKMGRCMD_START                   2
#define TASKMGRCMD_STOP                    3
#define TASKMGRCMD_RESTART                 4
#define TASKMGRCMD_PAUSE                   5
#define TASKMGRCMD_RESUME                  6
#define TASKMGRCMD_UNICAST                 7
#define TASKMGRCMD_BROADCAST               8
#define TASKMGRCMD_SET_UNICAST_CB          9
#define TASKMGRCMD_SET_BROADCAST_CB        10
#define TASKMGRCMD_SCAN_NAME               11
#define TASKMGRCMD_SCAN_HANDLE             12
#define TASKMGRCMD_SCAN_GROUP              13
#define TASKMGRCMD_REGISTER_TASK           14
#ifndef CONFIG_DISABLE_PTHREAD
#define TASKMGRCMD_REGISTER_PTHREAD        15
#endif
#define TASKMGRCMD_SET_STOP_CB             16
#define TASKMGRCMD_SET_EXIT_CB             17

/* Task Type */
#define TM_BUILTIN_TASK                    0
#define TM_TASK                            1
#ifndef CONFIG_DISABLE_PTHREAD
#define TM_PTHREAD                         2
#endif

/* Message Queue Values */
#define TM_MQ_PRIO   50
#define TM_PUBLIC_MQ "tm_public_mq"
#define TM_PRIVATE_MQ "tm_priv_mq"

/* Wrapper of allocation APIs */
#define TM_ALLOC(a)  malloc(a)
#define TM_FREE(a)   free(a)

/* Temporary State for Cancel */
#define TM_APP_STATE_CANCELLING -1

typedef void (*_tm_unicast_t)(void *);
typedef void (*_tm_broadcast_t)(int);
typedef void (*_tm_termination_t)(void);

struct app_list_s {
	int pid;
	void *addr;
};
typedef struct app_list_s app_list_t;

struct app_list_data_s {
	int type;
	int idx;
	int pid;
	int tm_gid;
	int status;
	int permission;
	int msg_mask;
	_tm_unicast_t unicast_cb;
	_tm_broadcast_t broadcast_cb;
	_tm_termination_t stop_cb;
	_tm_termination_t exit_cb;
};
typedef struct app_list_data_s app_list_data_t;

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
	app_info_list_t *data;
};
typedef struct tm_response_s tm_response_t;

struct tm_broadcast_s {
	int msg_mask;
	_tm_broadcast_t cb;
};
typedef struct tm_broadcast_s tm_broadcast_t;

struct tm_task_info_s {
	char *name;
	int priority;
	int stack_size;
	main_t entry;
	char **argv;
};
typedef struct tm_task_info_s tm_task_info_t;

#ifndef CONFIG_DISABLE_PTHREAD
struct tm_pthread_info_s {
	char *name;
	pthread_attr_t *attr;
	pthread_startroutine_t entry;
	pthread_addr_t arg;
};
typedef struct tm_pthread_info_s tm_pthread_info_t;
#endif

#define IS_INVALID_HANDLE(i) (i < 0 || i >= CONFIG_TASK_MANAGER_MAX_TASKS)

#define TM_LIST_ADDR(handle)       ((app_list_data_t *)tm_app_list[handle].addr)
#define TM_PID(handle)             tm_app_list[handle].pid
#define TM_TYPE(handle)            TM_LIST_ADDR(handle)->type
#define TM_IDX(handle)             TM_LIST_ADDR(handle)->idx
#define TM_GID(handle)             TM_LIST_ADDR(handle)->tm_gid
#define TM_STATUS(handle)          TM_LIST_ADDR(handle)->status
#define TM_PERMISSION(handle)      TM_LIST_ADDR(handle)->permission
#define TM_MSG_MASK(handle)        TM_LIST_ADDR(handle)->msg_mask
#define TM_UNICAST_CB(handle)      TM_LIST_ADDR(handle)->unicast_cb
#define TM_BROADCAST_CB(handle)    TM_LIST_ADDR(handle)->broadcast_cb
#define TM_STOP_CB(handle)         TM_LIST_ADDR(handle)->stop_cb
#define TM_EXIT_CB(handle)         TM_LIST_ADDR(handle)->exit_cb

extern app_list_t tm_app_list[CONFIG_TASK_MANAGER_MAX_TASKS];

int taskmgr_send_request(tm_request_t *request_msg);
int taskmgr_send_response(char *q_name, tm_response_t *response_msg);
int taskmgr_receive_response(char *q_name, tm_response_t *response_msg, int timeout);

bool taskmgr_is_permitted(int handle, pid_t pid);
int taskmgr_get_task_state(int handle);
int taskmgr_get_drvfd(void);
int taskmgr_get_handle_by_pid(int pid);

#endif
