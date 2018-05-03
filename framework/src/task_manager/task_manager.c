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
#include <stdio.h>
#include <signal.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <tinyara/sched.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/taskmgt.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

/****************************************************************************
 * Private Structs
 ****************************************************************************/
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
};
typedef struct task_list_data_s task_list_data_t;

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
#define TASK_LIST_ADDR(i)   ((task_list_data_t *)tm_task_list[i].addr)
#define TASK_PID(i)         tm_task_list[i].pid
#define TASK_BUILTIN_IDX(i) TASK_LIST_ADDR(i)->builtin_idx
#define TASK_TM_GID(i)      TASK_LIST_ADDR(i)->tm_gid
#define TASK_STATUS(i)      TASK_LIST_ADDR(i)->status
#define TASK_PERMISSION(i)  TASK_LIST_ADDR(i)->permission

static int builtin_cnt;
static mqd_t g_tm_recv_mqfd;
extern const task_builtin_list_t tm_builtin_lists[];
/* [0] : tm_gid, [1] : the address of task list data */
static task_list_t tm_task_list[CONFIG_TASK_MANAGER_MAX_TASKS];
extern int get_tm_builtin_cnt(void);

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void tm_update_task_state(int handle)
{
	struct tcb_s *tcb;

	tcb = sched_gettcb(TASK_PID(handle));
	/* if terminated normally, tcb will be released, but task list is not updated yet. */
	if (tcb == NULL && TASK_LIST_ADDR(handle) != NULL) {
		TASK_STATUS(handle) = TM_TASK_STATE_STOP;
	}
}

static int tm_get_task_state(int handle)
{
	if (handle >= CONFIG_TASK_MANAGER_MAX_TASKS || TASK_LIST_ADDR(handle) == NULL) {
		return TM_TASK_STATE_UNREGISTERED;
	}

	return (TASK_STATUS(handle));
}

static int tm_check_permission(int handle, pid_t pid)
{
	int ret = ERROR;
	int chk_idx;
	int caller_gid = -1;

	switch (TASK_PERMISSION(handle)) {
	case TM_TASK_PERMISSION_DEDICATE:
		if (TASK_TM_GID(handle) == pid) {
			ret = OK;
		}
		break;
	case TM_TASK_PERMISSION_GROUP:
		/* If requester's pid is as same as handle's gid, then requester registered that handle. */
		if (pid == TASK_TM_GID(handle)) {
			ret = OK;
			break;
		}
		/* check pid's group id */
		for (chk_idx = 0; chk_idx < builtin_cnt; chk_idx++) {
			if (TASK_PID(chk_idx) == pid) {
				caller_gid = TASK_TM_GID(chk_idx);
				break;
			}
		}

		if (caller_gid == TASK_TM_GID(handle)) {
			ret = OK;
		}
		break;
	case TM_TASK_PERMISSION_ALL:
		ret = OK;
		break;
	default:
		tmdbg("Invalid permission\n");
		break;
	}
	return ret;
}

static int tm_handle_private_queue(char *q_name, tm_response_t *msg)
{
	int ret;
	mqd_t priv_q;
	struct mq_attr attr;
	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(tm_response_t);
	attr.mq_flags = 0;

	priv_q = mq_open(q_name, O_WRONLY | O_CREAT, 0666, &attr);
	if (priv_q == (mqd_t)-1) {
		tmdbg("mq_open failed!\n");
		return ERROR;
	}

	ret = mq_send(priv_q, (char *)msg, sizeof(tm_response_t), TM_MQ_PRIO);
	if (ret != OK) {
		tmdbg("mq_send failed!\n");
		mq_close(priv_q);
		return ERROR;
	}
	
	mq_close(priv_q);

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int tm_get_task_info(task_info_list_t **data, int handle)
{
	int len;
	task_info_list_t *item;

	item = (task_info_list_t *)TM_ALLOC(sizeof(task_info_list_t));
	if (item == NULL) {
		tmdbg("Memory allocation Failed\n");
		return -1;
	}

	len = strlen(tm_builtin_lists[TASK_BUILTIN_IDX(handle)].name);
	item->task.name = (char *)TM_ALLOC(len + 1);
	if (item->task.name == NULL) {
		tmdbg("Memory allocation for name Failed\n");
		TM_FREE(item);
		return -1;
	}
	strncpy(item->task.name, tm_builtin_lists[TASK_BUILTIN_IDX(handle)].name, len + 1);
	item->task.gid = TASK_TM_GID(handle);
	item->task.handle = handle;
	item->task.status = TASK_STATUS(handle);
	item->task.permission = TASK_PERMISSION(handle);
	item->next = *data;

	*data = item;

	return OK;
}

static int tm_search_task_list(tm_request_t *msg, tm_response_t *response_msg)
{
	int chk_idx;
	int cnt = 0;
	int type = msg->cmd;

	if (type != TASKMGT_SCAN_NAME && type  != TASKMGT_SCAN_GROUP) {
		return TM_INVALID_PARAM;
	}

	for(chk_idx = 0; chk_idx < CONFIG_MAX_TASKS; chk_idx++) {
		if (TASK_LIST_ADDR(chk_idx) == NULL) {
			continue;
		}
		if ((type == TASKMGT_SCAN_NAME && strncmp(tm_builtin_lists[TASK_BUILTIN_IDX(chk_idx)].name, msg->data, strlen(msg->data)) == 0) \
			|| (type == TASKMGT_SCAN_GROUP && TASK_TM_GID(chk_idx) == msg->handle)) {
				tmvdbg("FIND!!!!!!! handle = %d\n", chk_idx);
				tm_get_task_info(&response_msg->data, chk_idx);
				cnt++;
		}
	}
	return cnt;
}

int task_manager(int argc, char *argv[])
{
	int nbytes;
	int ret;
	tm_request_t request_msg;
	tm_response_t response_msg;
	int fd;
	int chk_idx;
	int register_permission;
	struct mq_attr attr;

	sigignore(SIGCHLD);
	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(tm_request_t);
	attr.mq_flags = 0;

	builtin_cnt = get_tm_builtin_cnt();
	if (builtin_cnt < 2) {
		tmdbg("Failed to start task manager\n");
		return 0;
	}
	/* service register : register, set tm_gid to its pid */

	fd = open(TASKMGT_DRVPATH, O_RDWR);
	if (fd < 0) {
		tmdbg("Failed to open task management driver %d\n", errno);
		return 0;
	}

	g_tm_recv_mqfd = mq_open(TM_PUBLIC_MQ, O_RDONLY | O_CREAT, 0666, &attr);
	while (1) {
		ret = ERROR;

		nbytes = mq_receive(g_tm_recv_mqfd, (char *)&request_msg, sizeof(tm_request_t), NULL);
		if (nbytes <= 0) {
			continue;
		}

		sched_lock();

		tmvdbg("Recevied Request msg : cmd = %d\n", request_msg.cmd);
		switch (request_msg.cmd) {
		case TASKMGT_REGISTER_TASK:
			register_permission = request_msg.handle;
			/* Check that task is in builtin-list or not */
			for (chk_idx = 0; chk_idx < builtin_cnt; chk_idx++) {
				if (strncmp((char *)tm_builtin_lists[chk_idx].name, request_msg.data, strlen(request_msg.data)) == 0) {
					/* Requested name is in builtin-list */
					ret = ioctl(fd, TMIOC_REGISTER_TASK, &request_msg);
					break;
				}
			}
			/* In this case, there is no info in builtin with its name */
			if (chk_idx >= builtin_cnt) {
				tmdbg("Cannot find the task\n");
				ret = TM_FAIL_NOT_REGISTERED;
				break;
			}

			if (ret == OK) {
				tm_task_list[request_msg.handle].addr = (void *)TM_ALLOC(sizeof(task_list_data_t));
				TASK_BUILTIN_IDX(request_msg.handle) = chk_idx;
				TASK_TM_GID(request_msg.handle) = request_msg.caller_pid;
				TASK_STATUS(request_msg.handle) = TM_TASK_STATE_STOP;
				TASK_PERMISSION(request_msg.handle) = register_permission;
				ret = request_msg.handle;
			}
			break;

		case TASKMGT_UNREGISTER_TASK:
			ret = tm_get_task_state(request_msg.handle);
			if (ret == TM_TASK_STATE_UNREGISTERED) {
				ret = TM_FAIL_UNREGISTERED_TASK;
				break;
			}
			TASK_PID(request_msg.handle) = 0;
			TM_FREE(tm_task_list[request_msg.handle].addr);
			tm_task_list[request_msg.handle].addr = NULL;

			ret = ioctl(fd, TMIOC_UNREGISTER_TASK, &request_msg);
			break;

		case TASKMGT_START:
			tm_update_task_state(request_msg.handle);
			ret = tm_get_task_state(request_msg.handle);
			if (ret != TM_TASK_STATE_STOP) {
				ret = -ret;
				break;
			}
			/* handle is for starting task, pid is for caller */
			ret = tm_check_permission(request_msg.handle, request_msg.caller_pid);
			if (ret != OK) {
				break;
			}
			/* handle is in task_list */
			ret = ioctl(fd, TMIOC_START, &tm_builtin_lists[TASK_BUILTIN_IDX(request_msg.handle)]);
			if (ret > 0) {
				/* task created well */
				TASK_PID(request_msg.handle) = ret;
				TASK_STATUS(request_msg.handle) = TM_TASK_STATE_RUNNING;
				ret = OK;
			} else {
				/* task created fail */
				ret = TM_FAIL_START_NOT_CREATED;
			}
			break;

		case TASKMGT_TERMINATE:
			tm_update_task_state(request_msg.handle);
			ret = tm_get_task_state(request_msg.handle);
			if (ret == TM_TASK_STATE_UNREGISTERED || ret == TM_TASK_STATE_STOP) {
				ret = -ret;
				break;
			}
			ret = tm_check_permission(request_msg.handle, request_msg.caller_pid);
			if (ret != OK) {
				break;
			}
			/* handle is in task_list */
			ret = ioctl(fd, TMIOC_TERMINATE, TASK_PID(request_msg.handle));
			if (ret >= 0) {
				/* task deleted well */
				TASK_STATUS(request_msg.handle) = TM_TASK_STATE_STOP;
				ret = OK;
			}
			break;

		case TASKMGT_RESTART:
			break;

		case TASKMGT_PAUSE:
			break;

		case TASKMGT_RESUME:
			break;

		case TASKMGT_SCAN_NAME:
		case TASKMGT_SCAN_GROUP:
			tm_update_task_state(request_msg.handle);
			response_msg.data = NULL;
			ret = tm_search_task_list(&request_msg, &response_msg);
			if (ret <= 0) {
				ret = TM_FAIL_NOT_REGISTERED;
			} else {
				ret = OK;
			}
			break;

		case TASKMGT_SCAN_HANDLE:
			tm_update_task_state(request_msg.handle);
			ret = tm_get_task_state(request_msg.handle);
			if (ret == TM_TASK_STATE_UNREGISTERED) {
				break;
			}
			response_msg.status = OK;
			tm_get_task_info(&response_msg.data, request_msg.handle);
			break;

		case TASKMGT_UNICAST:
			tm_update_task_state(request_msg.handle);
			ret = tm_get_task_state(request_msg.handle);
			if (ret == TM_TASK_STATE_UNREGISTERED || ret == TM_TASK_STATE_STOP) {
				ret = -ret;
				break;
			}
			ret = tm_check_permission(request_msg.handle, request_msg.caller_pid);
			if (ret != OK) {
				break;
			}

			/* handle is in task_list */
			union sigval msg;
			msg.sival_ptr = request_msg.data;
			ret = sigqueue(TASK_PID(request_msg.handle), SIGTM, msg);
			if (ret != OK) {
				tmdbg("Fail to send signal, errno : %d\n", errno);
				TM_FREE(request_msg.data);
				request_msg.data = NULL;
				ret = TM_FAIL_SEND_MSG;
			}
			break;

		case TASKMGT_BROADCAST:
			break;

		default:
			break;
		}

		if (request_msg.timeout != TM_NO_RESPONSE) {
			response_msg.status = ret;
			tm_handle_private_queue((char *)request_msg.q_name, &response_msg);
		}

		if (request_msg.data != NULL) {
			TM_FREE(request_msg.data);
			request_msg.data = NULL;
		}
		sched_unlock();
	}

	tmdbg("Task manager OUT\n");
	return 0;
}
