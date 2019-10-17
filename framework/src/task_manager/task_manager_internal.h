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

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#ifndef CONFIG_DISABLE_PTHREAD
#include <pthread.h>
#endif
#include <queue.h>
#include <sys/types.h>
#include <task_manager/task_manager.h>

/* Command Types */
#define TASKMGRCMD_REGISTER_BUILTIN        0
#define TASKMGRCMD_UNREGISTER              1
#define TASKMGRCMD_START                   2
#define TASKMGRCMD_STOP                    3
#define TASKMGRCMD_RESTART                 4
#define TASKMGRCMD_PAUSE                   5
#define TASKMGRCMD_RESUME                  6
#define TASKMGRCMD_UNICAST_SYNC            7
#define TASKMGRCMD_UNICAST_ASYNC           8
#define TASKMGRCMD_BROADCAST               9
#define TASKMGRCMD_SET_UNICAST_CB          10
#define TASKMGRCMD_SET_BROADCAST_CB        11
#define TASKMGRCMD_SCAN_NAME               12
#define TASKMGRCMD_SCAN_HANDLE             13
#define TASKMGRCMD_SCAN_GROUP              14
#define TASKMGRCMD_REGISTER_TASK           15
#ifndef CONFIG_DISABLE_PTHREAD
#define TASKMGRCMD_REGISTER_PTHREAD        16
#endif
#define TASKMGRCMD_SET_STOP_CB             17
#define TASKMGRCMD_SET_EXIT_CB             18
#define TASKMGRCMD_ALLOC_BROADCAST_MSG     19
#define TASKMGRCMD_UNSET_BROADCAST_CB      20
#define TASKMGRCMD_DEALLOC_BROADCAST_MSG   21
#define TASKMGRCMD_SCAN_PID                22

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
#define TM_UNICAST_MQ "tm_unicast_mq"

/* Wrapper of allocation APIs */
#define TM_ALLOC(a)  malloc(a)
#define TM_FREE(a)   free(a)
#define TM_ZALLOC(a) zalloc(a)
#ifdef CONFIG_CPP_HAVE_VARARGS
#define TM_ASPRINTF(p, f, ...) asprintf(p, f, ##__VA_ARGS__)
#else
#define TM_ASPRINTF asprintf
#endif

/**
 * @brief Late Unregister Options
 */
#ifdef CONFIG_SCHED_LPWORK
#define TM_LATE_UNREGISTER_PRIO CONFIG_SCHED_LPWORKPRIORITY
#else
#define TM_LATE_UNREGISTER_PRIO 50
#endif

#define TM_INTERVAL_TRY_UNREGISTER 3

/**
 * @brief Unicast Type
 */
#define TM_UNICAST_SYNC      (0)
#define TM_UNICAST_ASYNC     (1)

/**
 * @brief Represent the msg size when input is NULL
 */
#define TM_NULL_MSG_SIZE     (-1)

struct tm_termination_info_s {
	tm_termination_callback_t cb;
	tm_msg_t *cb_data;
};
typedef struct tm_termination_info_s tm_termination_info_t;

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
	tm_unicast_callback_t unicast_cb;
	sq_queue_t broadcast_info_list;
	tm_termination_info_t *stop_cb_info;
	tm_termination_info_t *exit_cb_info;
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
	void *data;
};
typedef struct tm_response_s tm_response_t;

struct tm_broadcast_info_s {
	struct tm_broadcast_info_s *flink;
	int msg;
	tm_broadcast_callback_t cb;
	tm_msg_t *cb_data;
};
typedef struct tm_broadcast_info_s tm_broadcast_info_t;

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

struct tm_internal_msg_s {
	int msg_size;
	void *msg;
	int type;
};
typedef struct tm_internal_msg_s tm_internal_msg_t;

struct tm_broadcast_internal_msg_s {
	int size;
	void *user_data;
	tm_broadcast_info_t *cb_info;
};
typedef struct tm_broadcast_internal_msg_s tm_broadcast_internal_msg_t;

#define IS_INVALID_HANDLE(i) (i < 0 || i >= CONFIG_TASK_MANAGER_MAX_TASKS)

app_list_t *taskmger_get_applist(int handle);
#define TM_LIST_ADDR(handle)            ((app_list_data_t *)taskmger_get_applist(handle)->addr)
#define TM_PID(handle)                  taskmger_get_applist(handle)->pid
#define TM_TYPE(handle)                 TM_LIST_ADDR(handle)->type
#define TM_IDX(handle)                  TM_LIST_ADDR(handle)->idx
#define TM_GID(handle)                  TM_LIST_ADDR(handle)->tm_gid
#define TM_STATUS(handle)               TM_LIST_ADDR(handle)->status
#define TM_PERMISSION(handle)           TM_LIST_ADDR(handle)->permission
#define TM_UNICAST_CB(handle)           TM_LIST_ADDR(handle)->unicast_cb
#define TM_BROADCAST_INFO_LIST(handle)  TM_LIST_ADDR(handle)->broadcast_info_list
#define TM_STOP_CB_INFO(handle)         TM_LIST_ADDR(handle)->stop_cb_info
#define TM_EXIT_CB_INFO(handle)         TM_LIST_ADDR(handle)->exit_cb_info

#define SET_QUEUE_NAME_WITH_DEALLOC_DATA(request_msg, timeout)							\
	do {													\
		if (timeout != TM_NO_RESPONSE) {								\
			TM_ASPRINTF(&request_msg.q_name, "%s%d", TM_PRIVATE_MQ, request_msg.caller_pid);	\
			if (request_msg.q_name == NULL) {							\
				if (request_msg.data != NULL) {							\
					TM_FREE(request_msg.data);						\
				}										\
				return TM_OUT_OF_MEMORY;							\
			}											\
		}												\
	} while (0)

#define SEND_REQUEST_TO_TM_WITH_DEALLOC_DATA(request_msg, status)	\
	do {								\
		status = taskmgr_send_request(&request_msg);		\
		if (status < 0) {					\
			if (request_msg.data != NULL) {			\
				TM_FREE(request_msg.data);		\
			}						\
			if (request_msg.q_name != NULL) {		\
				TM_FREE(request_msg.q_name);		\
			}						\
			return status;					\
		}							\
	} while (0)

#define RECV_RESPONSE_FROM_TM(request_msg, response_msg, status, timeout)				\
	do {												\
		status = taskmgr_receive_response(request_msg.q_name, &response_msg, timeout);	\
		TM_FREE(request_msg.q_name);							\
	} while (0)

#define SET_TERMINATION_CB(cb)						\
	do {								\
		act.sa_sigaction = (_sa_sigaction_t)cb;			\
		act.sa_flags = 0;					\
		(void)sigemptyset(&act.sa_mask);			\
									\
		ret = sigaddset(&act.sa_mask, SIGTM_TERMINATION);	\
		if (ret < 0) {						\
			tmdbg("Failed to add signal set\n");		\
			return TM_OPERATION_FAIL;			\
		}							\
									\
		ret = sigaction(SIGTM_TERMINATION, &act, NULL);		\
		if (ret == (int)SIG_ERR) {				\
			tmdbg("sigaction Failed\n");			\
			return TM_OPERATION_FAIL;			\
		}							\
	} while (0)

int taskmgr_send_request(tm_request_t *request_msg);
void taskmgr_send_response(char *q_name, int timeout, tm_response_t *response_msg, int ret_status);
int taskmgr_receive_response(char *q_name, tm_response_t *response_msg, int timeout);

bool taskmgr_is_permitted(int handle, pid_t pid);
int taskmgr_get_task_state(int handle);
int taskmgr_get_drvfd(void);
int taskmgr_get_handle_by_pid(int pid);
int taskmgr_calc_time(struct timespec *time, int timeout);
int taskmgr_get_task_manager_pid(void);
#endif
