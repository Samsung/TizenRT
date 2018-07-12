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
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <apps/builtin.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/task_manager_drv.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
static int g_taskmgr_fd;
static uint32_t g_lasthandle;
static mqd_t g_tm_recv_mqfd;
static int builtin_cnt;
task_list_t tm_task_list[CONFIG_TASK_MANAGER_MAX_TASKS];
static bool g_handle_hash[CONFIG_TASK_MANAGER_MAX_TASKS];

#define MAX_HANDLE_MASK (CONFIG_TASK_MANAGER_MAX_TASKS - 1)
#define HANDLE_HASH(handle)  ((handle) & MAX_HANDLE_MASK)
#define TYPE_UNICAST     1
#define TYPE_BROADCAST   2

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int taskmgr_get_drvfd(void)
{
	return g_taskmgr_fd;
}

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int taskmgr_open_driver(void)
{
	g_taskmgr_fd = open(TASK_MANAGER_DRVPATH, O_RDWR);
	if (g_taskmgr_fd < 0) {
		tmdbg("Failed to open task management driver %d\n", errno);
		return ERROR;
	}
	return g_taskmgr_fd;
}

static int taskmgr_assign_handle(void)
{
	int hash_ndx;
	int tries;

	/* Disable pre-emption.  This should provide sufficient protection
	 * for the following operation.
	 */

	(void)sched_lock();

	/* We'll try every allowable  */

	for (tries = 0; tries < CONFIG_TASK_MANAGER_MAX_TASKS; tries++) {
		/* Get the next handle candidate */

		g_lasthandle++;

		/* Get the hash_ndx associated with the next_handle */

		hash_ndx = HANDLE_HASH(g_lasthandle);

		/* Check if there is a (potential) duplicate of this handle */

		if (!g_handle_hash[hash_ndx]) {
			/* Assign this handle to the task */

			g_handle_hash[hash_ndx] = true;

			(void)sched_unlock();
			return hash_ndx;
		}
	}

	/* If we get here, then the g_handle_hash[] table is completely full.
	 * We cannot allow another task to be started.
	 */

	(void)sched_unlock();

	return TM_BUSY;
}

static int taskmgr_register(char *name, int permission, int caller_pid)
{
	int chk_idx;
	int handle;

	if (permission < 0 || caller_pid < 0 || name == NULL) {
		return TM_INVALID_PARAM;
	}

	handle = TM_OPERATION_FAIL;

	/* Check that task is in builtin-list or not */
	for (chk_idx = 0; chk_idx < builtin_cnt; chk_idx++) {
		if (strncmp((char *)builtin_list[chk_idx].name, name, CONFIG_TASK_NAME_SIZE) == 0) {
			/* Requested name is in builtin-list */
			handle = taskmgr_assign_handle();
			if (handle < 0) {
				tmdbg("Failed to assign handle\n");
				return handle;
			}
			break;
		}
	}

	if (handle >= 0) {
		tm_task_list[handle].addr = (void *)TM_ALLOC(sizeof(task_list_data_t));
		if (tm_task_list[handle].addr == NULL) {
			return TM_OUT_OF_MEMORY;
		}
		TASK_BUILTIN_IDX(handle) = chk_idx;
		TASK_TM_GID(handle) = caller_pid;
		TASK_STATUS(handle) = TM_TASK_STATE_STOP;
		TASK_PERMISSION(handle) = permission;
		tmvdbg("Registered handle %d\n", handle);
	}

	return handle;
}

static int taskmgr_unregister(int handle)
{
	if (IS_INVALID_HANDLE(handle)) {
		return TM_INVALID_PARAM;
	}
	if (taskmgr_get_task_state(handle) == TM_TASK_STATE_UNREGISTERED) {
		return TM_UNREGISTERED_TASK;
	}

	TASK_PID(handle) = 0;
	TM_FREE(tm_task_list[handle].addr);
	tm_task_list[handle].addr = NULL;
	g_handle_hash[handle] = false;
	tmvdbg("Unregistered handle %d\n", handle);

	return OK;
}

static int taskmgr_start(int handle, int caller_pid)
{
	int ret;
	int fd;
	int pid;

	if (IS_INVALID_HANDLE(handle)) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret != TM_TASK_STATE_STOP) {
		return -ret;
	}

	/* handle is for starting task, pid is for caller */
	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	fd = taskmgr_get_drvfd();
	if (fd < 0) {
		return TM_INVALID_DRVFD;
	}

	/* handle is in task_list */
	builtin_info_t *task_info = (builtin_info_t *)&builtin_list[TASK_BUILTIN_IDX(handle)];
	pid = task_create(task_info->name, task_info->priority, task_info->stacksize, task_info->entry, (char * const *)NULL);
	if (pid == ERROR) {
		tmdbg("Fail to create new task\n");
		return TM_OPERATION_FAIL;
	}

	ret = ioctl(fd, TMIOC_START, pid);
	if (ret == ERROR) {
		(void)task_delete(pid);
		return TM_OPERATION_FAIL;
	}

	/* task created well */
	TASK_PID(handle) = pid;
	TASK_STATUS(handle) = TM_TASK_STATE_RUNNING;

	return OK;
}

static int taskmgr_stop(int handle, int caller_pid)
{
	int ret;

	if (IS_INVALID_HANDLE(handle)) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret == TM_TASK_STATE_UNREGISTERED || ret == TM_TASK_STATE_STOP) {
		return -ret;
	}

	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	/* handle is in task_list */
	ret = task_delete(TASK_PID(handle));
	if (ret != OK) {
		tmdbg("Fail to delete the task\n");
		return TM_OPERATION_FAIL;
	}
	/* task deleted well */
	TASK_STATUS(handle) = TM_TASK_STATE_STOP;

	return OK;
}

static int taskmgr_restart(int handle, int caller_pid)
{
	int ret;

	if (IS_INVALID_HANDLE(handle) || caller_pid < 0) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);

	if (ret == TM_TASK_STATE_UNREGISTERED) {
		return -ret;
	}

	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	ret = task_restart(TASK_PID(handle));
	if (ret != OK) {
		tmdbg("Fail to restart the task\n");
		return TM_OPERATION_FAIL;
	}

	TASK_STATUS(handle) = TM_TASK_STATE_RUNNING;

	return OK;
}

static int taskmgr_pause(int handle, int caller_pid)
{
	int ret;
	int fd;

	if (IS_INVALID_HANDLE(handle) || caller_pid < 0) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret != TM_TASK_STATE_RUNNING) {
		return -ret;
	}

	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	fd = taskmgr_get_drvfd();
	if (fd < 0) {
		return TM_INVALID_DRVFD;
	}

	ret = ioctl(fd, TMIOC_PAUSE, TASK_PID(handle));
	if (ret != OK) {
		return TM_OPERATION_FAIL;
	}

	ret = kill(TASK_PID(handle), SIGTM_PAUSE);
	if (ret != OK) {
		tmdbg("Fail to pause the task\n");
		return TM_OPERATION_FAIL;
	}

	TASK_STATUS(handle) = TM_TASK_STATE_PAUSE;

	return OK;
}

static int taskmgr_resume(int handle, int caller_pid)
{
	int ret;

	if (IS_INVALID_HANDLE(handle)) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret != TM_TASK_STATE_PAUSE) {
		return -ret;
	}

	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	ret = kill(TASK_PID(handle), SIGTM_RESUME);
	if (ret != OK) {
		tmdbg("Fail to resume the task\n");
		return TM_OPERATION_FAIL;
	}

	TASK_STATUS(handle) = TM_TASK_STATE_RUNNING;

	return OK;
}

static int taskmgr_unicast(int handle, int caller_pid, void *data)
{
	int ret;
	int fd;
	union sigval msg;

	if (IS_INVALID_HANDLE(handle)) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret != TM_TASK_STATE_RUNNING) {
		return -ret;
	}

	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	fd = taskmgr_get_drvfd();
	if (fd < 0) {
		return TM_INVALID_DRVFD;
	}

	ret = ioctl(fd, TMIOC_UNICAST, TASK_PID(handle));
	if (ret < 0) {
		return TM_OPERATION_FAIL;
	}

	/* handle is in task_list */
	msg.sival_ptr = data;
	ret = sigqueue(TASK_PID(handle), SIGTM_UNICAST, msg);
	if (ret != OK) {
		tmdbg("Fail to send signal, errno : %d\n", errno);
		return TM_OPERATION_FAIL;
	}

	return OK;
}

static int taskmgr_get_task_info(task_info_list_t **data, int handle)
{
	int len;
	task_info_list_t *item;

	item = (task_info_list_t *)TM_ALLOC(sizeof(task_info_list_t));
	if (item == NULL) {
		tmdbg("Memory allocation Failed\n");
		return TM_OUT_OF_MEMORY;
	}

	len = strlen(builtin_list[TASK_BUILTIN_IDX(handle)].name);
	item->task.name = (char *)TM_ALLOC(len + 1);
	if (item->task.name == NULL) {
		tmdbg("Memory allocation for name Failed\n");
		TM_FREE(item);
		return TM_OUT_OF_MEMORY;
	}
	strncpy(item->task.name, builtin_list[TASK_BUILTIN_IDX(handle)].name, len + 1);
	item->task.tm_gid = TASK_TM_GID(handle);
	item->task.handle = handle;
	item->task.status = TASK_STATUS(handle);
	item->task.permission = TASK_PERMISSION(handle);
	item->next = *data;

	*data = item;

	return OK;
}

static int taskmgr_getinfo_with_name(char *name, tm_response_t *response_msg)
{
	int chk_idx;
	int ret;

	if (name == NULL) {
		return TM_INVALID_PARAM;
	}

	ret = TM_UNREGISTERED_TASK;
	response_msg->data = NULL;

	for (chk_idx = 0; chk_idx < CONFIG_TASK_MANAGER_MAX_TASKS; chk_idx++) {
		if (TASK_LIST_ADDR(chk_idx) && (strncmp(builtin_list[TASK_BUILTIN_IDX(chk_idx)].name, name, strlen(name)) == 0)) {
			tmvdbg("found handle = %d\n", chk_idx);
			ret = taskmgr_get_task_info(&response_msg->data, chk_idx);
			if (ret != OK) {
				return ret;
			}
		}
	}

	return ret;
}

static int taskmgr_getinfo_with_group(int group, tm_response_t *response_msg)
{
	int chk_idx;
	int ret;

	if (group < 0) {
		return TM_INVALID_PARAM;
	}

	ret = TM_UNREGISTERED_TASK;
	response_msg->data = NULL;

	for (chk_idx = 0; chk_idx < CONFIG_TASK_MANAGER_MAX_TASKS; chk_idx++) {
		if (TASK_LIST_ADDR(chk_idx) && TASK_TM_GID(chk_idx) == group) {
			tmvdbg("found handle = %d\n", chk_idx);
			ret = taskmgr_get_task_info(&response_msg->data, chk_idx);
			if (ret != OK) {
				return ret;
			}
		}
	}

	return ret;
}

static int taskmgr_getinfo_with_handle(int handle, tm_response_t *response_msg)
{
	if (IS_INVALID_HANDLE(handle)) {
		return TM_INVALID_PARAM;
	}

	if (TASK_LIST_ADDR(handle) == NULL) {
		return TM_UNREGISTERED_TASK;
	}

	return taskmgr_get_task_info(&response_msg->data, handle);
}

static void taskmgr_termination_callback(void)
{
	mq_close(g_tm_recv_mqfd);
	mq_unlink(TM_PUBLIC_MQ);
}

static int taskmgr_check_msg_mask(int msg_mask, int handle)
{
	if (msg_mask & TASK_MSG_MASK(handle)) {
		return OK;
	}
	return ERROR;
}

int taskmgr_get_handle_by_pid(int pid)
{
	int handle;
	for (handle = 0; handle < CONFIG_TASK_MANAGER_MAX_TASKS; handle++) {
		if (TASK_PID(handle) == pid) {
			return handle;
		}
	}
	return TM_UNREGISTERED_TASK;
}

static void taskmgr_broadcast(int msg)
{
	int handle;
	int fd;
	int ret;
	union sigval msg_broad;

	fd = taskmgr_get_drvfd();
	if (fd < 0) {
		return;
	}
	msg_broad.sival_int = msg;
	for (handle = 0; handle < CONFIG_TASK_MANAGER_MAX_TASKS; handle++) {
		if (TASK_LIST_ADDR(handle) != NULL) {
			ret = taskmgr_get_task_state(handle);
			if (ret == TM_TASK_STATE_STOP || ret == TM_TASK_STATE_UNREGISTERED) {
				continue;
			}
			ret = taskmgr_check_msg_mask(msg, handle);
			if (ret != OK) {
				continue;
			}
			ret = ioctl(fd, TMIOC_BROADCAST, TASK_PID(handle));
			if (ret != OK) {
				continue;
			}
			(void)sigqueue(TASK_PID(handle), SIGTM_BROADCAST, msg_broad);
		}
	}
}

static int taskmgr_set_msg_cb(int type, void *data, int pid)
{
	int handle;

	if (data == NULL) {
		return TM_INVALID_PARAM;
	}

	handle = taskmgr_get_handle_by_pid(pid);
	if (handle == TM_UNREGISTERED_TASK) {
		return handle;
	}

	if (type == TYPE_UNICAST) {
		TASK_UNICAST_CB(handle) = (_tm_unicast_t)data;
	} else {
		TASK_MSG_MASK(handle) = ((tm_broadcast_t *)data)->msg_mask;
		TASK_BROADCAST_CB(handle) = ((tm_broadcast_t *)data)->cb;
	}
	return OK;
}

/****************************************************************************
 * Main Function
 ****************************************************************************/
int task_manager(int argc, char *argv[])
{
	int nbytes;
	int ret;
	tm_request_t request_msg;
	tm_response_t response_msg;
	struct mq_attr attr;
#ifdef CONFIG_SCHED_HAVE_PARENT
	sigignore(SIGCHLD);
#endif
	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(tm_request_t);
	attr.mq_flags = 0;

	builtin_cnt = get_builtin_list_cnt();

	if (builtin_cnt < 1) {
		tmdbg("Failed to start task manager\n");
		return 0;
	}
	/* service register : register, set tm_gid to its pid */

	ret = taskmgr_open_driver();
	if (ret < 0) {
		return 0;
	}

	g_tm_recv_mqfd = mq_open(TM_PUBLIC_MQ, O_RDONLY | O_CREAT, 0666, &attr);
	if (g_tm_recv_mqfd < 0) {
		return 0;
	}

	/* Register callback when termination */
	if (atexit((void *)taskmgr_termination_callback) != OK) {
		return 0;
	}

	while (1) {
		ret = ERROR;

		nbytes = mq_receive(g_tm_recv_mqfd, (char *)&request_msg, sizeof(tm_request_t), NULL);
		if (nbytes <= 0) {
			continue;
		}

		sched_lock();

		tmvdbg("Recevied Request msg : cmd = %d\n", request_msg.cmd);
		switch (request_msg.cmd) {
		case TASKMGR_REGISTER:
			ret = taskmgr_register(request_msg.data, request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGR_UNREGISTER:
			ret = taskmgr_unregister(request_msg.handle);
			break;

		case TASKMGR_START:
			ret = taskmgr_start(request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGR_STOP:
			ret = taskmgr_stop(request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGR_RESTART:
			ret = taskmgr_restart(request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGR_PAUSE:
			ret = taskmgr_pause(request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGR_RESUME:
			ret = taskmgr_resume(request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGR_SCAN_NAME:
			ret = taskmgr_getinfo_with_name(request_msg.data, &response_msg);
			break;

		case TASKMGR_SCAN_GROUP:
			ret = taskmgr_getinfo_with_group(request_msg.handle, &response_msg);
			break;

		case TASKMGR_SCAN_HANDLE:
			ret = taskmgr_getinfo_with_handle(request_msg.handle, &response_msg);
			break;

		case TASKMGR_UNICAST:
			ret = taskmgr_unicast(request_msg.handle, request_msg.caller_pid, request_msg.data);
			break;

		case TASKMGR_BROADCAST:
			(void)taskmgr_broadcast(*((int *)request_msg.data));
			break;

		case TASKMGR_SET_BROADCAST_CB:
			ret = taskmgr_set_msg_cb(TYPE_BROADCAST, request_msg.data, request_msg.caller_pid);
			break;

		case TASKMGR_SET_UNICAST_CB:
			ret = taskmgr_set_msg_cb(TYPE_UNICAST, request_msg.data, request_msg.caller_pid);
			break;
			
		default:
			break;
		}

		if (request_msg.timeout != TM_NO_RESPONSE) {
			response_msg.status = ret;
			taskmgr_send_response((char *)request_msg.q_name, &response_msg);
		}

		if (request_msg.data != NULL && request_msg.cmd != TASKMGR_SET_UNICAST_CB) {
			TM_FREE(request_msg.data);
			request_msg.data = NULL;
		}
		sched_unlock();
	}
	tmdbg("Task manager OUT\n");
	return 0;
}
