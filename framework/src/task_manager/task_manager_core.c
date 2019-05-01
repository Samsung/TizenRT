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
#include <stdint.h>
#include <sched.h>
#include <signal.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>
#include <queue.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <apps/builtin.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/clock.h>
#include <tinyara/task_manager_drv.h>
#include <task_manager/task_manager.h>
#ifdef CONFIG_TASK_MANAGER_USER_SPECIFIC_BROADCAST
#include <task_manager/task_manager_broadcast_list.h>
#endif
#include "task_manager_internal.h"
/****************************************************************************
 * Private Definitions
 ****************************************************************************/
static int g_taskmgr_fd;
static uint32_t g_lasthandle;
static mqd_t g_tm_recv_mqfd;
static int handle_cnt;
static int task_cnt;
#ifndef CONFIG_DISABLE_PTHREAD
static int pthread_cnt;
tm_pthread_info_t tm_pthread_list[CONFIG_TASK_MANAGER_MAX_TASKS];
#endif
app_list_t tm_app_list[CONFIG_TASK_MANAGER_MAX_TASKS];
tm_task_info_t tm_task_list[CONFIG_TASK_MANAGER_MAX_TASKS];
static bool g_handle_hash[CONFIG_TASK_MANAGER_MAX_TASKS];
int tm_broadcast_msg[TM_BROADCAST_MSG_MAX + CONFIG_TASK_MANAGER_MAX_TASKS];
static int task_manager_pid;

#define MAX_HANDLE_MASK (CONFIG_TASK_MANAGER_MAX_TASKS - 1)
#define HANDLE_HASH(handle)  ((handle) & MAX_HANDLE_MASK)
#define TYPE_CANCEL      1
#define TYPE_EXIT        2

#define CB_MSG_OF(X)        (X)->cb_data->msg
#define CB_MSGSIZE_OF(X)    (X)->cb_data->msg_size

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

	/* Disable pre-emption. This should provide sufficient protection
	 * for the following operation.
	 */

	(void)sched_lock();

	/* We'll try every allowable */

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

static void taskmgr_dealloc_cb_info(tm_termination_info_t **cb_info)
{
	if ((*cb_info)->cb_data != NULL) {
		if (CB_MSG_OF(*cb_info) != NULL) {
			TM_FREE(CB_MSG_OF(*cb_info));
			CB_MSG_OF(*cb_info) = NULL;
		}
		TM_FREE((*cb_info)->cb_data);
		(*cb_info)->cb_data = NULL;
	}
	TM_FREE(*cb_info);
	*cb_info = NULL;
}

static int taskmgr_register_builtin(char *name, int permission, int caller_pid)
{
	int chk_idx;
	int handle;
	unsigned int builtin_cnt;

	if (permission < 0 || caller_pid <= 0 || name == NULL) {
		return TM_INVALID_PARAM;
	}

	if (handle_cnt >= CONFIG_TASK_MANAGER_MAX_TASKS) {
		return TM_OUT_OF_MEMORY;
	}

	handle = TM_OPERATION_FAIL;
	builtin_cnt = get_builtin_list_cnt();
	if (builtin_cnt == 0) {
		return TM_NOT_SUPPORTED;
	}

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
		tm_app_list[handle].addr = (void *)TM_ALLOC(sizeof(app_list_data_t));
		if (tm_app_list[handle].addr == NULL) {
			return TM_OUT_OF_MEMORY;
		}
		TM_TYPE(handle) = TM_BUILTIN_TASK;
		TM_IDX(handle) = chk_idx;
		TM_GID(handle) = caller_pid;
		TM_STATUS(handle) = TM_APP_STATE_STOP;
		TM_PERMISSION(handle) = permission;
		TM_STOP_CB_INFO(handle) = NULL;
		TM_EXIT_CB_INFO(handle) = NULL;
		sq_init(&TM_BROADCAST_INFO_LIST(handle));
		tmvdbg("Registered handle %d\n", handle);
		handle_cnt++;
	}

	return handle;
}

static void taskmgr_clear_broadcast_info_list(int handle)
{
	tm_broadcast_info_t *curr;

	while ((curr = (tm_broadcast_info_t *)sq_remfirst(&TM_BROADCAST_INFO_LIST(handle))) != NULL) {
		TM_FREE(curr);
	}
}

static void taskmgr_execute_unregister(int handle)
{
	/* If type is TM_TASK or TM_PTHREAD, remove the data in the list */
	if (TM_TYPE(handle) == TM_TASK) {
		TM_FREE(tm_task_list[TM_IDX(handle)].name);
		tm_task_list[TM_IDX(handle)].name = NULL;
		tm_task_list[TM_IDX(handle)].priority = 0;
		tm_task_list[TM_IDX(handle)].stack_size = 0;
		tm_task_list[TM_IDX(handle)].entry = NULL;
	}
#ifndef CONFIG_DISABLE_PTHREAD
	else if (TM_TYPE(handle) == TM_PTHREAD) {
		TM_FREE(tm_pthread_list[TM_IDX(handle)].name);
		tm_pthread_list[TM_IDX(handle)].name = NULL;
		pthread_attr_destroy(tm_pthread_list[TM_IDX(handle)].attr);
		TM_FREE(tm_pthread_list[TM_IDX(handle)].attr);
		tm_pthread_list[TM_IDX(handle)].entry = NULL;
		tm_pthread_list[TM_IDX(handle)].arg = NULL;
	}
#endif

	TM_PID(handle) = 0;
	if (TM_STOP_CB_INFO(handle) != NULL) {
		taskmgr_dealloc_cb_info(&TM_STOP_CB_INFO(handle));
	}
	if (TM_EXIT_CB_INFO(handle) != NULL) {
		taskmgr_dealloc_cb_info(&TM_EXIT_CB_INFO(handle));
	}

	taskmgr_clear_broadcast_info_list(handle);
	TM_FREE(tm_app_list[handle].addr);
	tm_app_list[handle].addr = NULL;
	g_handle_hash[handle] = false;
	tmvdbg("Unregistered handle %d\n", handle);
	handle_cnt--;
}

static void *taskmgr_late_unregister(void *arg)
{
	int ret;

	while (1) {
		ret = taskmgr_get_task_state((int)arg);
		if (ret == TM_APP_STATE_STOP || ret == TM_APP_STATE_UNREGISTERED) {
			break;
		}
		sleep(TM_INTERVAL_TRY_UNREGISTER);
	}
	taskmgr_execute_unregister((int)arg);
	return NULL;
}

static int taskmgr_unregister(int handle)
{
	int state;
	int ret;
	int unregister_handle;
	pthread_t pth;
	pthread_attr_t attr;

	unregister_handle = handle;

	if (IS_INVALID_HANDLE(unregister_handle)) {
		return TM_INVALID_PARAM;
	}

	state = taskmgr_get_task_state(unregister_handle);
	if (state == TM_APP_STATE_CANCELLING) {
		TM_STATUS(unregister_handle) = TM_APP_STATE_WAIT_UNREGISTER;
		/* If task is on cancelling state, unregister will be treated later. */
		ret = pthread_attr_init(&attr);
		if (ret != OK) {
			return TM_OPERATION_FAIL;
		}
		attr.priority = TM_LATE_UNREGISTER_PRIO;
		ret = pthread_create(&pth, &attr, taskmgr_late_unregister, (void *)unregister_handle);
		if (ret != OK) {
			return TM_OPERATION_FAIL;
		}
		pthread_setname_np(pth, "tm_late_unregister");
		(void)pthread_detach(pth);

		return OK;
	} else if (state == TM_APP_STATE_STOP) {
		/* If task is on stop state, unregister immediately. */
		taskmgr_execute_unregister(unregister_handle);
	} else {
		return -state;
	}

	return OK;
}

static int taskmgr_handle_tcb(int type, int pid)
{
	int fd;
	int ret;

	fd = taskmgr_get_drvfd();
	if (fd < 0) {
		return TM_INVALID_DRVFD;
	}

	ret = ioctl(fd, type, pid);
	if (ret == ERROR) {
		return TM_OPERATION_FAIL;
	}
	return OK;
}

#if defined(HAVE_TASK_GROUP) && !defined(CONFIG_DISABLE_PTHREAD)
static int taskmgr_adj_pthread_parent(int parent_pid, int child_pid)
{
	int ret;
	tm_pthread_pid_t group_info;

	group_info.parent_pid = parent_pid;
	group_info.child_pid = child_pid;

	ret = taskmgr_handle_tcb(TMIOC_PTHREAD_PARENT, (int)&group_info);
	if (ret != OK) {
		ret = TM_OPERATION_FAIL;
	}

	return ret;
}
#endif			/* HAVE_TASK_GROUP && !CONFIG_DISABLE_PTHREAD */

static int taskmgr_start(int handle, int caller_pid)
{
	int ret;
	int pid = ERROR;

	if (IS_INVALID_HANDLE(handle)) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret != TM_APP_STATE_STOP) {
		return -ret;
	}

	/* handle is for starting task, pid is for caller */
	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	if (TM_TYPE(handle) == TM_BUILTIN_TASK) {
		builtin_info_t *task_info = (builtin_info_t *)&builtin_list[TM_IDX(handle)];
		pid = task_create(task_info->name, task_info->priority, task_info->stacksize, task_info->entry, (char * const *)NULL);
	} else if (TM_TYPE(handle) == TM_TASK) {
		tm_task_info_t *task_info = (tm_task_info_t *)&tm_task_list[TM_IDX(handle)];
		pid = task_create(task_info->name, task_info->priority, task_info->stack_size, task_info->entry, task_info->argv);
	}
#ifndef CONFIG_DISABLE_PTHREAD
	else {
		tm_pthread_info_t *pthread_info = (tm_pthread_info_t *)&tm_pthread_list[TM_IDX(handle)];
		pthread_t thread = ERROR;
		ret = pthread_create(&thread, pthread_info->attr, pthread_info->entry, pthread_info->arg);
		if (ret != OK) {
			tmdbg("Fail to create new pthread\n");
			return TM_OPERATION_FAIL;
		}
		pthread_setname_np(thread, pthread_info->name);
		pthread_detach(thread);
		pid = (int)thread;
#ifdef HAVE_TASK_GROUP
		taskmgr_adj_pthread_parent(caller_pid, pid);
#endif
	}
#endif

	if (pid == ERROR) {
		tmdbg("Fail to create new task\n");
		return TM_OPERATION_FAIL;
	}

	ret = taskmgr_handle_tcb(TMIOC_START, pid);
	if (ret < 0) {
		(void)task_delete(pid);
		return ret;
	}

	/* task created well */
	TM_PID(handle) = pid;
	TM_STATUS(handle) = TM_APP_STATE_RUNNING;

	return OK;
}

void taskmgr_update_stop_status(int signo, siginfo_t *data)
{
	int ret;
	int handle;
	handle = taskmgr_get_handle_by_pid(data->si_value.sival_int);

	/* Terminate based on task type */
	ret = taskmgr_handle_tcb(TMIOC_TERMINATE, TM_PID(handle));
	if (ret < 0) {
		tmdbg("Fail to terminate the task\n");
	}

	/* task or pthread terminated well */
	if (TM_STATUS(handle) == TM_APP_STATE_WAIT_UNREGISTER) {
		TM_STATUS(handle) = TM_APP_STATE_UNREGISTERED;
	} else {
		TM_STATUS(handle) = TM_APP_STATE_STOP;
	}

	TM_FREE(TM_STOP_CB_INFO(handle));
	TM_STOP_CB_INFO(handle) = NULL;
}

static int taskmgr_stop(int handle, int caller_pid)
{
	int ret;
	union sigval msg;
	tm_termination_info_t *cb_info;

	if (IS_INVALID_HANDLE(handle)) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret == TM_APP_STATE_UNREGISTERED || ret == TM_APP_STATE_STOP || ret == TM_APP_STATE_CANCELLING) {
		return -ret;
	}

	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	/* Call stop callback */
	TM_STATUS(handle) = TM_APP_STATE_CANCELLING;

	if (TM_STOP_CB_INFO(handle) != NULL) {
		cb_info = (tm_termination_info_t *)TM_ALLOC(sizeof(tm_termination_info_t));
		if (cb_info == NULL) {
			return TM_OUT_OF_MEMORY;
		}
		cb_info->cb = TM_STOP_CB_INFO(handle)->cb;
		cb_info->cb_data = TM_STOP_CB_INFO(handle)->cb_data;
		msg.sival_ptr = cb_info;

		ret = sigqueue(TM_PID(handle), SIGTM_TERMINATION, msg);
		if (ret != OK) {
			tmdbg("Fail to send signal, errno : %d\n", errno);
			return TM_OPERATION_FAIL;
		}
	} else {
		if (TM_TYPE(handle) == TM_BUILTIN_TASK || TM_TYPE(handle) == TM_TASK) {
			ret = task_delete(TM_PID(handle));
		}
#ifndef CONFIG_DISABLE_PTHREAD
		else {
			ret = pthread_cancel(TM_PID(handle));
		}
#endif
		if (ret != OK) {
			tmdbg("Fail to delete the task\n");
			return TM_OPERATION_FAIL;
		}
		/* task or pthread terminated well */
		TM_STATUS(handle) = TM_APP_STATE_STOP;
	}

	return OK;
}

static int taskmgr_restart(int handle, int caller_pid)
{
	int ret;

	if (IS_INVALID_HANDLE(handle) || caller_pid < 0) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret == TM_APP_STATE_UNREGISTERED) {
		return -ret;
	}

	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	/* After restart, previous callback information is not needed,
	 * because restarted task will re-allocate the callback information again. */
	if (TM_STOP_CB_INFO(handle) != NULL) {
		taskmgr_dealloc_cb_info(&TM_STOP_CB_INFO(handle));
	}
	if (TM_EXIT_CB_INFO(handle) != NULL) {
		taskmgr_dealloc_cb_info(&TM_EXIT_CB_INFO(handle));
	}

	ret = task_restart(TM_PID(handle));
	if (ret != OK) {
		tmdbg("Fail to restart the task\n");
		return TM_OPERATION_FAIL;
	}

	TM_STATUS(handle) = TM_APP_STATE_RUNNING;

	return OK;
}

static int taskmgr_pause(int handle, int caller_pid)
{
	int ret;

	if (IS_INVALID_HANDLE(handle) || caller_pid < 0) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret != TM_APP_STATE_RUNNING) {
		return -ret;
	}

	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	ret = taskmgr_handle_tcb(TMIOC_PAUSE, TM_PID(handle));
	if (ret < 0) {
		return TM_OPERATION_FAIL;
	}

	ret = kill(TM_PID(handle), SIGTM_PAUSE);
	if (ret != OK) {
		tmdbg("Fail to pause the task\n");
		return TM_OPERATION_FAIL;
	}

	TM_STATUS(handle) = TM_APP_STATE_PAUSE;

	return OK;
}

static int taskmgr_resume(int handle, int caller_pid)
{
	int ret;

	if (IS_INVALID_HANDLE(handle)) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret != TM_APP_STATE_PAUSE) {
		return -ret;
	}

	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	ret = kill(TM_PID(handle), SIGTM_RESUME);
	if (ret != OK) {
		tmdbg("Fail to resume the task\n");
		return TM_OPERATION_FAIL;
	}

	TM_STATUS(handle) = TM_APP_STATE_RUNNING;

	return OK;
}

static int taskmgr_unicast_async(int handle, int caller_pid, void *data)
{
	int ret;
	union sigval msg;

	if (IS_INVALID_HANDLE(handle)) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret != TM_APP_STATE_RUNNING) {
		return -ret;
	}

	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	ret = taskmgr_handle_tcb(TMIOC_UNICAST, TM_PID(handle));
	if (ret < 0) {
		return TM_OPERATION_FAIL;
	}

	/* handle is in app_list */
	msg.sival_ptr = data;
	ret = sigqueue(TM_PID(handle), SIGTM_UNICAST, msg);
	if (ret != OK) {
		tmdbg("Fail to send signal, errno : %d\n", errno);
		return TM_OPERATION_FAIL;
	}

	return OK;
}

int taskmgr_calc_time(struct timespec *result_time, int offset)
{
	int ret;
	time_t sec;
	uint32_t nsec;

	sec = offset / MSEC_PER_SEC;
	nsec = (offset - MSEC_PER_SEC * sec) * NSEC_PER_MSEC;

	ret = clock_gettime(CLOCK_REALTIME, result_time);
	if (ret != 0) {
		return ERROR;
	}
	result_time->tv_sec += sec;
	result_time->tv_nsec += nsec;
	if (result_time->tv_nsec >= NSEC_PER_SEC) {
		result_time->tv_sec++;
		result_time->tv_nsec -= NSEC_PER_SEC;
	}
	return OK;
}

static int taskmgr_unicast_sync(int handle, int caller_pid, tm_internal_msg_t *data, tm_msg_t *response_msg, int timeout)
{
	int ret;
	union sigval msg;
	mqd_t unicast_mqfd;
	struct mq_attr attr;
	tm_msg_t recv_msg;
	FAR struct timespec time;

	if (IS_INVALID_HANDLE(handle)) {
		return TM_INVALID_PARAM;
	}

	ret = taskmgr_get_task_state(handle);
	if (ret != TM_APP_STATE_RUNNING) {
		return -ret;
	}

	if (!taskmgr_is_permitted(handle, caller_pid)) {
		return TM_NO_PERMISSION;
	}

	ret = taskmgr_handle_tcb(TMIOC_UNICAST, TM_PID(handle));
	if (ret < 0) {
		return TM_OPERATION_FAIL;
	}

	/* handle is in app_list */
	msg.sival_ptr = data;
	ret = sigqueue(TM_PID(handle), SIGTM_UNICAST, msg);
	if (ret != OK) {
		tmdbg("Fail to send signal, errno : %d\n", errno);
		return TM_OPERATION_FAIL;
	}

	/* For sync, wait a mq for receiving reply msg */
	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(tm_msg_t);
	attr.mq_flags = 0;

	unicast_mqfd = mq_open(TM_UNICAST_MQ, O_RDONLY | O_CREAT, 0666, &attr);
	if (unicast_mqfd == (mqd_t)ERROR) {
		tmdbg("mq_open failed!\n");
		return TM_COMMUCATION_FAIL;
	}

	if (timeout > 0) {
		ret = taskmgr_calc_time(&time, timeout);
		if (ret != OK) {
			return TM_OPERATION_FAIL;
		}
		ret = mq_timedreceive(unicast_mqfd, (char *)&recv_msg, sizeof(tm_msg_t), 0, &time);
	} else {
#if CONFIG_TASK_MANAGER_UNICAST_REPLY_TIMEOUT > 0
		ret = taskmgr_calc_time(&time, CONFIG_TASK_MANAGER_UNICAST_REPLY_TIMEOUT);
		if (ret != OK) {
			return TM_OPERATION_FAIL;
		}
		ret = mq_timedreceive(unicast_mqfd, (char *)&recv_msg, sizeof(tm_msg_t), 0, &time);
#else
		ret = mq_receive(unicast_mqfd, (char *)&recv_msg, sizeof(tm_msg_t), 0);
#endif
	}
	if (ret <= 0) {
		mq_close(unicast_mqfd);
		mq_unlink(TM_UNICAST_MQ);
		tmdbg("mq_receive failed! %d\n", errno);
		if (errno == ETIMEDOUT) {
			return TM_REPLY_TIMEOUT;
		}
		return TM_COMMUCATION_FAIL;
	}

	mq_close(unicast_mqfd);
	mq_unlink(TM_UNICAST_MQ);

	response_msg->msg_size = recv_msg.msg_size;
	response_msg->msg = TM_ALLOC(recv_msg.msg_size);
	if (response_msg->msg == NULL) {
		return TM_OUT_OF_MEMORY;
	}
	memcpy(response_msg->msg, recv_msg.msg, recv_msg.msg_size);

	return OK;
}

static int taskmgr_get_task_info(tm_appinfo_list_t **data, int handle)
{
	int name_len = 0;
	tm_appinfo_list_t *item;
	const char *name;
	int ret;

	ret = taskmgr_get_task_state(handle);
	if (ret == TM_APP_STATE_UNREGISTERED) {
		return -ret;
	}

	item = (tm_appinfo_list_t *)TM_ALLOC(sizeof(tm_appinfo_list_t));
	if (item == NULL) {
		tmdbg("Memory allocation Failed\n");
		return TM_OUT_OF_MEMORY;
	}

	if (TM_TYPE(handle) == TM_BUILTIN_TASK) {
		name = builtin_list[TM_IDX(handle)].name;
	} else if (TM_TYPE(handle) == TM_TASK) {
		name = tm_task_list[TM_IDX(handle)].name;
	}
#ifndef CONFIG_DISABLE_PTHREAD
	else {
		name = tm_pthread_list[TM_IDX(handle)].name;
	}
#endif

	name_len = strlen(name);

	item->task.name = (char *)TM_ALLOC(name_len + 1);
	if (item->task.name == NULL) {
		tmdbg("Memory allocation for name Failed\n");
		TM_FREE(item);
		return TM_OUT_OF_MEMORY;
	}

	strncpy(item->task.name, name, name_len + 1);

	item->task.tm_gid = TM_GID(handle);
	item->task.handle = handle;
	item->task.status = TM_STATUS(handle);
	item->task.permission = TM_PERMISSION(handle);
	item->next = *data;

	*data = item;

	return OK;
}

static int taskmgr_getinfo_with_name(char *name, tm_response_t *response_msg)
{
	int chk_idx;
	int ret;
	char *tm_stored_name = NULL;

	if (name == NULL) {
		return TM_INVALID_PARAM;
	}

	ret = TM_UNREGISTERED_APP;
	response_msg->data = NULL;

	for (chk_idx = 0; chk_idx < CONFIG_TASK_MANAGER_MAX_TASKS; chk_idx++) {
		if (TM_LIST_ADDR(chk_idx)) {
			if (TM_TYPE(chk_idx) == TM_BUILTIN_TASK) {
				tm_stored_name = (char *)builtin_list[TM_IDX(chk_idx)].name;
			} else if (TM_TYPE(chk_idx) == TM_TASK) {
				tm_stored_name = tm_task_list[TM_IDX(chk_idx)].name;
			}
#ifndef CONFIG_DISABLE_PTHREAD
			else if (TM_TYPE(chk_idx) == TM_PTHREAD) {
				tm_stored_name = tm_pthread_list[TM_IDX(chk_idx)].name;
			}
#endif
			if (tm_stored_name && !strncmp(tm_stored_name, name, strlen(name) + 1)) {
				tmvdbg("found handle = %d\n", chk_idx);
				ret = taskmgr_get_task_info((tm_appinfo_list_t **)&response_msg->data, chk_idx);
				if (ret != OK) {
					return ret;
				}
				tm_stored_name = NULL;
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

	ret = TM_UNREGISTERED_APP;
	response_msg->data = NULL;

	for (chk_idx = 0; chk_idx < CONFIG_TASK_MANAGER_MAX_TASKS; chk_idx++) {
		if (TM_LIST_ADDR(chk_idx) && TM_GID(chk_idx) == group) {
			tmvdbg("found handle = %d\n", chk_idx);
			ret = taskmgr_get_task_info((tm_appinfo_list_t **)&response_msg->data, chk_idx);
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

	if (TM_LIST_ADDR(handle) == NULL) {
		return TM_UNREGISTERED_APP;
	}

	return taskmgr_get_task_info((tm_appinfo_list_t **)&response_msg->data, handle);
}

static void taskmgr_termination_callback(void)
{
	task_manager_pid = TM_TASK_MGR_NOT_ALIVE;
	(void)close(g_taskmgr_fd);
	mq_close(g_tm_recv_mqfd);
	mq_unlink(TM_PUBLIC_MQ);
}

int taskmgr_get_handle_by_pid(int pid)
{
	int handle;
	for (handle = 0; handle < CONFIG_TASK_MANAGER_MAX_TASKS; handle++) {
		if (TM_PID(handle) == pid) {
			return handle;
		}
	}
	return TM_UNREGISTERED_APP;
}

static int taskmgr_check_broad_msg(int msg)
{
	if (tm_broadcast_msg[msg - 1] == msg) {
		return OK;
	}
	return TM_UNREGISTERED_MSG;
}

static tm_broadcast_info_t *taskmgr_search_broadcast_info(int msg, int handle)
{
	tm_broadcast_info_t *curr;

	if (taskmgr_check_broad_msg(msg) != OK) {
		tmdbg("Not regiseterd broadcast msg: %d\n", msg);
		return NULL;
	}
	curr = (tm_broadcast_info_t *)sq_peek(&TM_BROADCAST_INFO_LIST(handle));
	while (curr) {
		if (curr->msg == msg) {
			return curr;
		}
		curr = sq_next(curr);
	}
	return NULL;
}

static int taskmgr_broadcast(tm_internal_msg_t *arg)
{
	int handle;
	int ret;
	union sigval msg_broad;
	tm_broadcast_info_t *broadcast_info;
	tm_broadcast_internal_msg_t *bm;

	ret = taskmgr_check_broad_msg(arg->type);
	if (ret == TM_UNREGISTERED_MSG) {
		return ret;
	}

	for (handle = 0; handle < CONFIG_TASK_MANAGER_MAX_TASKS; handle++) {
		if (TM_LIST_ADDR(handle) != NULL) {
			ret = taskmgr_get_task_state(handle);
			if (ret == TM_APP_STATE_STOP || ret == TM_APP_STATE_UNREGISTERED) {
				continue;
			}
			ret = taskmgr_handle_tcb(TMIOC_BROADCAST, TM_PID(handle));
			if (ret != OK) {
				continue;
			}
			broadcast_info = taskmgr_search_broadcast_info(arg->type, handle);
			if (broadcast_info == NULL) {
				continue;
			}
			bm = (tm_broadcast_internal_msg_t *)TM_ALLOC(sizeof(tm_broadcast_internal_msg_t));
			if (bm == NULL) {
				return TM_OUT_OF_MEMORY;
			}
			if (arg->msg_size > 0) {
				bm->user_data = TM_ALLOC(arg->msg_size);
				if (bm->user_data == NULL) {
					TM_FREE(bm);
					return TM_OUT_OF_MEMORY;
				}
				bm->size = arg->msg_size;
				memcpy(bm->user_data, arg->msg, arg->msg_size);
			} else {
				bm->user_data = NULL;
				bm->size = arg->msg_size;
			}

			bm->cb_info = broadcast_info;
			msg_broad.sival_ptr = (void *)bm;
			(void)sigqueue(TM_PID(handle), SIGTM_BROADCAST, msg_broad);
		}
	}
	return OK;
}

static void taskmgr_broadcast_msg_init(void)
{
	int chk_idx;
	int msg_num;

	for (chk_idx = 0; chk_idx < TM_BROADCAST_MSG_MAX + CONFIG_TASK_MANAGER_MAX_TASKS; chk_idx++) {
		msg_num = chk_idx + 1;
		if (chk_idx < (TM_BROADCAST_MSG_MAX - 1)) {
			tm_broadcast_msg[chk_idx] = msg_num;
		} else {
			tm_broadcast_msg[chk_idx] = -1;
		}
	}
}

static int taskmgr_is_tm_broadcast_msg_init(void)
{
	if ((tm_broadcast_msg[0] != 1) && (tm_broadcast_msg[0] != -1)) {
		 return 0;
	}
	return 1;
}

static int taskmgr_set_unicast_cb(tm_unicast_callback_t data, int pid)
{
	int handle;

	if (data == NULL) {
		return TM_INVALID_PARAM;
	}

	handle = taskmgr_get_handle_by_pid(pid);
	if (handle == TM_UNREGISTERED_APP) {
		return handle;
	}

	TM_UNICAST_CB(handle) = data;

	return OK;
}

static int taskmgr_set_broadcast_cb(tm_broadcast_info_t *data, int pid)
{
	int handle;
	int ret;
	tm_broadcast_info_t *broadcast_info;

	if (data == NULL) {
		return TM_INVALID_PARAM;
	}

	handle = taskmgr_get_handle_by_pid(pid);
	if (handle == TM_UNREGISTERED_APP) {
		return handle;
	}

	if (!taskmgr_is_tm_broadcast_msg_init()) {
		taskmgr_broadcast_msg_init();
	}

	ret = taskmgr_check_broad_msg(data->msg);
	if (ret == TM_UNREGISTERED_MSG) {
		return ret;
	}

	broadcast_info = taskmgr_search_broadcast_info(data->msg, handle);
	if (broadcast_info == NULL) {
		broadcast_info = (tm_broadcast_info_t *)TM_ALLOC(sizeof(tm_broadcast_info_t));
		if (broadcast_info == NULL) {
			return TM_OUT_OF_MEMORY;
		}
		broadcast_info->flink = NULL;
		broadcast_info->msg = data->msg;
		broadcast_info->cb = data->cb;
		if (data->cb_data != NULL) {
			broadcast_info->cb_data = TM_ALLOC(sizeof(tm_msg_t));
			if (broadcast_info->cb_data == NULL) {
				TM_FREE(broadcast_info);
				return TM_OUT_OF_MEMORY;
			}
			if (CB_MSGSIZE_OF(data) != 0) {
				 CB_MSG_OF(broadcast_info) = TM_ALLOC(CB_MSGSIZE_OF(data));
				if (CB_MSG_OF(broadcast_info) == NULL) {
					TM_FREE(broadcast_info->cb_data);
					TM_FREE(broadcast_info);
					return TM_OUT_OF_MEMORY;
				}
				memcpy(CB_MSG_OF(broadcast_info), CB_MSG_OF(data), CB_MSGSIZE_OF(data));
				CB_MSGSIZE_OF(broadcast_info) = CB_MSGSIZE_OF(data);
			} else {
				CB_MSG_OF(broadcast_info) = NULL;
				CB_MSGSIZE_OF(broadcast_info) = 0;
			}
		} else {
			broadcast_info->cb_data = NULL;
		}
		sq_addlast((FAR sq_entry_t *)broadcast_info, &TM_BROADCAST_INFO_LIST(handle));
	} else {
		if ((broadcast_info->cb == data->cb) && (CB_MSGSIZE_OF(broadcast_info) == CB_MSGSIZE_OF(data)) && (memcmp(CB_MSG_OF(broadcast_info), CB_MSG_OF(data), CB_MSGSIZE_OF(data)) == 0)) {
			return TM_ALREADY_REGISTERED_CB;
		}
		broadcast_info->cb = data->cb;
		if (data->cb_data != NULL) {
			broadcast_info->cb_data = TM_ALLOC(sizeof(tm_msg_t));
			if (broadcast_info->cb_data == NULL) {
				return TM_OUT_OF_MEMORY;
			}
			if (CB_MSGSIZE_OF(data) != 0) {
				CB_MSG_OF(broadcast_info) = TM_ALLOC(CB_MSGSIZE_OF(data));
				if (CB_MSG_OF(broadcast_info) == NULL) {
					TM_FREE(broadcast_info->cb_data);
					return TM_OUT_OF_MEMORY;
				}
				memcpy(CB_MSG_OF(broadcast_info), CB_MSG_OF(data), CB_MSGSIZE_OF(data));
			} else {
				CB_MSG_OF(broadcast_info) = NULL;
				CB_MSGSIZE_OF(broadcast_info) = 0;
			}
		} else {
			broadcast_info->cb_data = NULL;
		}
	}

	if (data->cb_data != NULL) {
		if (CB_MSG_OF(data) != NULL) {
			TM_FREE(CB_MSG_OF(data));
		}
		TM_FREE(data->cb_data);
	}

	return OK;
}

static int taskmgr_alloc_broadcast_msg(void)
{
	int chk_idx;
	int msg_num;

	if (!taskmgr_is_tm_broadcast_msg_init()) {
		taskmgr_broadcast_msg_init();
	}
	for (chk_idx = 0; chk_idx < TM_BROADCAST_MSG_MAX + CONFIG_TASK_MANAGER_MAX_TASKS; chk_idx++) {
		msg_num = chk_idx + 1;
		if (tm_broadcast_msg[chk_idx] == -1) {
			tm_broadcast_msg[chk_idx] = msg_num;
			return msg_num;
		}
	}
	return TM_OUT_OF_MEMORY;
}

static int taskmgr_unset_broadcast_cb(int msg, int pid)
{
	int handle;
	tm_broadcast_info_t *broadcast_info;

	handle = taskmgr_get_handle_by_pid(pid);
	if (handle == TM_UNREGISTERED_APP) {
		return handle;
	}
	broadcast_info = taskmgr_search_broadcast_info(msg, handle);
	if (broadcast_info == NULL) {
		return TM_UNREGISTERED_MSG;
	}
	sq_rem((FAR sq_entry_t *)broadcast_info, &TM_BROADCAST_INFO_LIST(handle));
	if (broadcast_info->cb_data != NULL) {
		if (CB_MSG_OF(broadcast_info) != NULL) {
			TM_FREE(CB_MSG_OF(broadcast_info));
			CB_MSG_OF(broadcast_info) = NULL;
		}
		TM_FREE(broadcast_info->cb_data);
		broadcast_info->cb_data = NULL;
	}
	TM_FREE(broadcast_info);
	return OK;
}

static int taskmgr_register_task(tm_task_info_t *task_info, int permission, int caller_pid)
{
	int chk_idx;
	int handle;

	if (permission < 0 || caller_pid < 0 || task_info == NULL) {
		return TM_INVALID_PARAM;
	}

	if (handle_cnt >= CONFIG_TASK_MANAGER_MAX_TASKS) {
		return TM_OUT_OF_MEMORY;
	}

	handle = TM_OPERATION_FAIL;

	/* Check that this task is already registered or not */
	for (chk_idx = 0; chk_idx < CONFIG_TASK_MANAGER_MAX_TASKS; chk_idx++) {
		if (strcmp(tm_task_list[chk_idx].name, task_info->name) == 0) {
			/* Already registered task */
			return TM_OPERATION_FAIL;
		}
	}

	/* Update the tm_task_list with new task information */
	while (1) {
		if (task_cnt >= CONFIG_TASK_MANAGER_MAX_TASKS) {
			task_cnt = 0;
		}

		if (tm_task_list[task_cnt].entry == NULL) {
			tm_task_list[task_cnt].name = (char *)TM_ALLOC(strlen(task_info->name) + 1);
			if (tm_task_list[task_cnt].name == NULL) {
				return TM_OUT_OF_MEMORY;
			}
			strncpy(tm_task_list[task_cnt].name, task_info->name, strlen(task_info->name) + 1);
			tm_task_list[task_cnt].priority = task_info->priority;
			tm_task_list[task_cnt].stack_size = task_info->stack_size;
			tm_task_list[task_cnt].entry = task_info->entry;
			tm_task_list[task_cnt].argv = task_info->argv;
			task_cnt++;
			break;
		}
		task_cnt++;
	}

	handle = taskmgr_assign_handle();
	if (handle >= 0) {
		tm_app_list[handle].addr = (void *)TM_ALLOC(sizeof(app_list_data_t));
		if (tm_app_list[handle].addr == NULL) {
			return TM_OUT_OF_MEMORY;
		}
		TM_TYPE(handle) = TM_TASK;
		TM_IDX(handle) = task_cnt - 1;
		TM_GID(handle) = caller_pid;
		TM_STATUS(handle) = TM_APP_STATE_STOP;
		TM_PERMISSION(handle) = permission;
		TM_STOP_CB_INFO(handle) = NULL;
		TM_EXIT_CB_INFO(handle) = NULL;
		sq_init(&TM_BROADCAST_INFO_LIST(handle));
		tmvdbg("Registered handle %d\n", handle);
		handle_cnt++;
	}

	return handle;
}

#ifndef CONFIG_DISABLE_PTHREAD
static int taskmgr_register_pthread(tm_pthread_info_t *pthread_info, int permission, int caller_pid)
{
	int chk_idx;
	int handle;

	if (permission < 0 || caller_pid < 0 || pthread_info == NULL) {
		return TM_INVALID_PARAM;
	}

	if (handle_cnt >= CONFIG_TASK_MANAGER_MAX_TASKS) {
		return TM_OUT_OF_MEMORY;
	}

	handle = TM_OPERATION_FAIL;

	/* Check that this task is already registered or not */
	for (chk_idx = 0; chk_idx < CONFIG_TASK_MANAGER_MAX_TASKS; chk_idx++) {
		if (strcmp(tm_pthread_list[chk_idx].name, pthread_info->name) == 0) {
			/* Already registered task */
			return TM_OPERATION_FAIL;
		}
	}

	/* Update the tm_pthread_list with new task information */
	while (1) {
		if (pthread_cnt >= CONFIG_TASK_MANAGER_MAX_TASKS) {
			pthread_cnt = 0;
		}

		if (tm_pthread_list[pthread_cnt].entry == NULL) {
			tm_pthread_list[pthread_cnt].name = (char *)TM_ALLOC(strlen(pthread_info->name) + 1);
			if (tm_pthread_list[pthread_cnt].name == NULL) {
				return TM_OUT_OF_MEMORY;
			}
			strncpy(tm_pthread_list[pthread_cnt].name, pthread_info->name, strlen(pthread_info->name) + 1);
			tm_pthread_list[pthread_cnt].attr = pthread_info->attr;
			tm_pthread_list[pthread_cnt].entry = pthread_info->entry;
			tm_pthread_list[pthread_cnt].arg = pthread_info->arg;
			pthread_cnt++;
			break;
		}
		pthread_cnt++;
	}

	handle = taskmgr_assign_handle();
	if (handle >= 0) {
		tm_app_list[handle].addr = (void *)TM_ALLOC(sizeof(app_list_data_t));
		if (tm_app_list[handle].addr == NULL) {
			return TM_OUT_OF_MEMORY;
		}
		TM_TYPE(handle) = TM_PTHREAD;
		TM_IDX(handle) = pthread_cnt - 1;
		TM_GID(handle) = caller_pid;
		TM_STATUS(handle) = TM_APP_STATE_STOP;
		TM_PERMISSION(handle) = permission;
		TM_STOP_CB_INFO(handle) = NULL;
		TM_EXIT_CB_INFO(handle) = NULL;
		sq_init(&TM_BROADCAST_INFO_LIST(handle));
		tmvdbg("Registered handle %d\n", handle);
		handle_cnt++;
	}

	return handle;
}
#endif

static int taskmgr_alloc_cb(tm_termination_info_t **cb_info, tm_termination_info_t *param)
{
	if (*cb_info == NULL) {
		*cb_info = (tm_termination_info_t *)TM_ZALLOC(sizeof(tm_termination_info_t));
		if (*cb_info == NULL) {
			return TM_OUT_OF_MEMORY;
		}
	}

	(*cb_info)->cb = param->cb;
	if (param->cb_data != NULL) {
		/* In this case, User wants to pass callback data to callback. */
		if ((*cb_info)->cb_data == NULL) {
			(*cb_info)->cb_data = (tm_msg_t *)TM_ZALLOC(sizeof(tm_msg_t));
			if ((*cb_info)->cb_data == NULL) {
				TM_FREE(*cb_info);
				return TM_OUT_OF_MEMORY;
			}
		}

		/* If previous callback message size is different with new callback message size,
		 * we should deallocate the old one, and set to new. */
		if (CB_MSGSIZE_OF(*cb_info) != CB_MSGSIZE_OF(param)) {
			if (CB_MSG_OF(*cb_info) != NULL) {
				TM_FREE(CB_MSG_OF(*cb_info));
			}
			CB_MSG_OF(*cb_info) = TM_ALLOC(CB_MSGSIZE_OF(param));
			if (CB_MSG_OF(*cb_info) == NULL) {
				TM_FREE((*cb_info)->cb_data);
				TM_FREE(*cb_info);
				return TM_OUT_OF_MEMORY;
			}
			CB_MSGSIZE_OF(*cb_info) = CB_MSGSIZE_OF(param);
		}

		memcpy(CB_MSG_OF(*cb_info), CB_MSG_OF(param), CB_MSGSIZE_OF(param));
		TM_FREE(CB_MSG_OF(param));
		TM_FREE(param->cb_data);
	} else {
		(*cb_info)->cb_data = NULL;
	}
	return OK;
}

static int taskmgr_set_termination_cb(int type, tm_termination_info_t *param, int pid)
{
	int handle;

	if (param == NULL) {
		return TM_INVALID_PARAM;
	}

	handle = taskmgr_get_handle_by_pid(pid);
	if (handle == TM_UNREGISTERED_APP) {
		return handle;
	}

	if (type == TYPE_CANCEL) {
		return taskmgr_alloc_cb(&TM_STOP_CB_INFO(handle), param);
	} else {
		return taskmgr_alloc_cb(&TM_EXIT_CB_INFO(handle), param);
	}

	return OK;
}

int task_manager_run_exit_cb(int pid)
{
	int handle;
	handle = taskmgr_get_handle_by_pid(pid);
	if (handle == TM_UNREGISTERED_APP) {
		return handle;
	}
	/* Run exit callback when normally terminated. */
	if (TM_STATUS(handle) != TM_APP_STATE_CANCELLING) {
		if (TM_EXIT_CB_INFO(handle) != NULL) {
			if (TM_EXIT_CB_INFO(handle)->cb_data != NULL) {
				(*(TM_EXIT_CB_INFO(handle)->cb))(CB_MSG_OF(TM_EXIT_CB_INFO(handle)));
				TM_FREE(CB_MSG_OF(TM_EXIT_CB_INFO(handle)));
				CB_MSG_OF(TM_EXIT_CB_INFO(handle)) = NULL;
				TM_FREE(TM_EXIT_CB_INFO(handle)->cb_data);
				TM_EXIT_CB_INFO(handle)->cb_data = NULL;
			} else {
				(*(TM_EXIT_CB_INFO(handle)->cb))(NULL);
			}
		}
	}
	return OK;
}

static int taskmgr_dealloc_broadcast_msg(int msg)
{
	int chk_idx;

	for (chk_idx = 0; chk_idx < TM_BROADCAST_MSG_MAX + CONFIG_TASK_MANAGER_MAX_TASKS; chk_idx++) {
		if (tm_broadcast_msg[chk_idx] == msg) {
			tm_broadcast_msg[chk_idx] = -1;
			return OK;
		}
	}
	return TM_UNREGISTERED_MSG;
}

int taskmgr_get_task_manager_pid(void)
{
	if (task_manager_pid > 0) {
		return task_manager_pid;
	}
	return TM_TASK_MGR_NOT_ALIVE;
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
	struct sigaction act;
#ifdef CONFIG_SCHED_HAVE_PARENT
	sigignore(SIGCHLD);
#endif
	attr.mq_maxmsg = CONFIG_TASK_MANAGER_MAX_MSG;
	attr.mq_msgsize = sizeof(tm_request_t);
	attr.mq_flags = 0;

	task_manager_pid = getpid();

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

	act.sa_sigaction = (_sa_sigaction_t)taskmgr_update_stop_status;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);

	ret = sigaddset(&act.sa_mask, SIGTM_TERMINATION);
	if (ret < 0) {
		tmdbg("Failed to add signal set\n");
		return TM_OPERATION_FAIL;
	}

	ret = sigaction(SIGTM_TERMINATION, &act, NULL);
	if (ret == (int)SIG_ERR) {
		tmdbg("sigaction Failed\n");
		return TM_OPERATION_FAIL;
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
		case TASKMGRCMD_REGISTER_BUILTIN:
			ret = taskmgr_register_builtin(request_msg.data, request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGRCMD_UNREGISTER:
			ret = taskmgr_unregister(request_msg.handle);
			break;

		case TASKMGRCMD_START:
			ret = taskmgr_start(request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGRCMD_STOP:
			ret = taskmgr_stop(request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGRCMD_RESTART:
#ifndef CONFIG_DISABLE_PTHREAD
			if (TM_TYPE(request_msg.handle) == TM_PTHREAD) {
				ret = TM_NOT_SUPPORTED;
				break;
			}
#endif
			ret = taskmgr_restart(request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGRCMD_PAUSE:
			ret = taskmgr_pause(request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGRCMD_RESUME:
			ret = taskmgr_resume(request_msg.handle, request_msg.caller_pid);
			break;

		case TASKMGRCMD_SCAN_NAME:
			ret = taskmgr_getinfo_with_name(request_msg.data, &response_msg);
			break;

		case TASKMGRCMD_SCAN_GROUP:
			ret = taskmgr_getinfo_with_group(request_msg.handle, &response_msg);
			break;

		case TASKMGRCMD_SCAN_HANDLE:
			ret = taskmgr_getinfo_with_handle(request_msg.handle, &response_msg);
			break;

		case TASKMGRCMD_SCAN_PID:
			ret = taskmgr_get_handle_by_pid(request_msg.caller_pid);
			if (ret == TM_UNREGISTERED_APP) {
				break;
			}
			ret = taskmgr_getinfo_with_handle(ret, &response_msg);
			break;

		case TASKMGRCMD_UNICAST:
			if (((tm_internal_msg_t *)request_msg.data)->type == TM_UNICAST_SYNC) {
				response_msg.data = TM_ALLOC(sizeof(tm_msg_t));
				if (response_msg.data == NULL) {
					response_msg.status = TM_OUT_OF_MEMORY;
					break;
				}
				ret = taskmgr_unicast_sync(request_msg.handle, request_msg.caller_pid, (tm_internal_msg_t *)request_msg.data, response_msg.data, request_msg.timeout);
			} else {
				ret = taskmgr_unicast_async(request_msg.handle, request_msg.caller_pid, (tm_msg_t *)request_msg.data);
			}
			break;

		case TASKMGRCMD_BROADCAST:
			ret = taskmgr_broadcast((tm_internal_msg_t *)request_msg.data);
			break;

		case TASKMGRCMD_ALLOC_BROADCAST_MSG:
			ret = taskmgr_alloc_broadcast_msg();
			break;

		case TASKMGRCMD_SET_BROADCAST_CB:
			ret = taskmgr_set_broadcast_cb((tm_broadcast_info_t *)request_msg.data, request_msg.caller_pid);
			break;

		case TASKMGRCMD_SET_UNICAST_CB:
			ret = taskmgr_set_unicast_cb((tm_unicast_callback_t)request_msg.data, request_msg.caller_pid);
			break;

		case TASKMGRCMD_REGISTER_TASK:
			ret = taskmgr_register_task((tm_task_info_t *)request_msg.data, request_msg.handle, request_msg.caller_pid);
			if (((tm_task_info_t *)request_msg.data)->name != NULL) {
				TM_FREE(((tm_task_info_t *)request_msg.data)->name);
				((tm_task_info_t *)request_msg.data)->name = NULL;
			}
			break;
#ifndef CONFIG_DISABLE_PTHREAD
		case TASKMGRCMD_REGISTER_PTHREAD:
			ret = taskmgr_register_pthread((tm_pthread_info_t *)request_msg.data, request_msg.handle, request_msg.caller_pid);
			if (((tm_pthread_info_t *)request_msg.data)->name != NULL) {
				TM_FREE(((tm_pthread_info_t *)request_msg.data)->name);
				((tm_pthread_info_t *)request_msg.data)->name = NULL;
			}
			break;
#endif
		case TASKMGRCMD_SET_STOP_CB:
			ret = taskmgr_set_termination_cb(TYPE_CANCEL, (tm_termination_info_t *)request_msg.data, request_msg.caller_pid);
			break;

		case TASKMGRCMD_SET_EXIT_CB:
			ret = taskmgr_set_termination_cb(TYPE_EXIT, (tm_termination_info_t *)request_msg.data, request_msg.caller_pid);
			break;

		case TASKMGRCMD_UNSET_BROADCAST_CB:
			ret = taskmgr_unset_broadcast_cb(*((int *)request_msg.data), request_msg.caller_pid);
			break;

		case TASKMGRCMD_DEALLOC_BROADCAST_MSG:
			ret = taskmgr_dealloc_broadcast_msg(*((int *)request_msg.data));
			break;
			
		default:
			break;
		}

		if (request_msg.timeout != TM_NO_RESPONSE) {
			response_msg.status = ret;
			taskmgr_send_response((char *)request_msg.q_name, &response_msg);
		}

		if (request_msg.data != NULL && request_msg.cmd != TASKMGRCMD_SET_UNICAST_CB && request_msg.cmd != TASKMGRCMD_ALLOC_BROADCAST_MSG && request_msg.cmd != TASKMGRCMD_UNICAST) {
			if (request_msg.cmd == TASKMGRCMD_BROADCAST && ((tm_internal_msg_t *)request_msg.data)->msg != NULL) {
				TM_FREE(((tm_internal_msg_t *)request_msg.data)->msg);
			}
			TM_FREE(request_msg.data);
			request_msg.data = NULL;
		}
		sched_unlock();
	}
	tmdbg("Task manager OUT\n");
	return 0;
}
