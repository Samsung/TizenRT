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
#include <tinyara/common_logs/common_logs.h>
#include <stdint.h>
#include <string.h>
#include <debug.h>
#include <signal.h>
#include <sys/types.h>

#include <apps/builtin.h>
#include <tinyara/sched.h>
#include <tinyara/signal.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/wdog.h>
#include <tinyara/task_manager_drv.h>

#include <task_manager/task_manager.h>

#include "sched/sched.h"
#if defined(HAVE_TASK_GROUP) && !defined(CONFIG_DISABLE_PTHREAD)
#include "group/group.h"
#endif

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int taskmgr_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t taskmgr_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t taskmgr_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/

struct taskmgr_dev_s {
	mqd_t mqdes;	   /* A mqueue descriptor */
};

static const struct file_operations taskmgr_fops = {
	0,                          /* open */
	0,                          /* close */
	taskmgr_read,               /* read */
	taskmgr_write,              /* write */
	0,                          /* seek */
	taskmgr_ioctl               /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int taskmgr_task_init(pid_t pid, void *usr_pause_handler)
{
	struct tcb_s *tcb;
	struct sigaction act;

	tcb = sched_gettcb(pid);
	if (!tcb) {
		tmdbg("%s [TM] tcb. pid = %d.\n", clog_message_str[CMN_LOG_INVALID_VAL], pid);
		return ERROR;
	}

	memset(&act, '\0', sizeof(act));
	act.sa_handler = (_sa_handler_t)usr_pause_handler;

	return sig_sethandler(tcb, SIGTM_PAUSE, &act);
}

#if defined(HAVE_TASK_GROUP) && !defined(CONFIG_DISABLE_PTHREAD)
static void taskmgr_pthread_group_detach(pid_t pthread_pid)
{
	FAR struct tcb_s *pthread_tcb;

	pthread_tcb = sched_gettcb(pthread_pid);
	if (pthread_tcb) {
		group_leave(pthread_tcb);
	}
}

static struct task_group_s *taskmgr_get_group_struct(pid_t pid)
{
	struct tcb_s *tcb;

	tcb = sched_gettcb(pid);
	if (!tcb) {
		tmdbg("%s [TM] tcb. pid = %d.\n", clog_message_str[CMN_LOG_INVALID_VAL], pid);
		return NULL;
	}

	return tcb->group;
}

static int taskmgr_group_bind(struct task_group_s *parent_group, struct pthread_tcb_s *child_tcb)
{
	if (parent_group == NULL || child_tcb == NULL) {
		tmdbg("%s [TM] parameter! %s Group bind\n", clog_message_str[CMN_LOG_NULL_CHECK_FAIL], clog_message_str[CMN_LOG_NULL_CHECK_FAIL]);
		return ERROR;
	}

	child_tcb->cmn.group = parent_group;
	return OK;
}

static int taskmgr_pthread_group_join(pid_t parent_pid, pid_t child_pid)
{
	struct task_group_s *parent_group;
	struct pthread_tcb_s *child_tcb;
	int ret;

	parent_group = taskmgr_get_group_struct(parent_pid);
	child_tcb = (struct pthread_tcb_s *)sched_gettcb(child_pid);
	if (child_tcb == NULL) {
		tmdbg("%s\n", clog_message_str[CMN_LOG_NULL_CHECK_FAIL]);
		return ERROR;
	}
	ret = taskmgr_group_bind(parent_group, child_tcb);
	if (ret != OK) {
		tmdbg("%s: taskmgr_group_bind\n", clog_message_str[CMN_LOG_FAILED_OP]);
		return ERROR;
	}

	ret = group_join(child_tcb);
	if (ret != OK) {
		tmdbg("%s\n", clog_message_str[CMN_LOG_FAILED_OP]);
		return ERROR;
	}

	return OK;
}

#if defined(CONFIG_SCHED_HAVE_PARENT) && !defined(HAVE_GROUP_MEMBERS)
static int taskmgr_pthread_ppid_change(pid_t parent_pid, pid_t child_pid)
{
	struct pthread_tcb_s *ptcb;
	ptcb = (struct pthread_tcb_s *)sched_gettcb(child_pid);

	if (ptcb == NULL) {
		tmdbg("%s ptcb. %s Pthread ppid change\n", clog_message_str[CMN_LOG_NULL_CHECK_FAIL], clog_message_str[CMN_LOG_FAILED_OP]);
		return ERROR;
	}

	ptcb->cmn.group->tg_ppid = parent_pid;
	return OK;
}
#endif			/* CONFIG_SCHED_HAVE_PARENT && !HAVE_GROUP_MEMBERS */
#endif			/* HAVE_TASK_GROUP && !CONFIG_DISABLE_PTHREAD */

static ssize_t taskmgr_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;
}

static ssize_t taskmgr_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return 0;
}

/************************************************************************************
 * Name: taskmgr_ioctl
 *
 * Description:  The ioctl method for task management.
 *
 ************************************************************************************/
static int taskmgr_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret = ERROR;
	struct tcb_s *tcb;
	tm_pthread_pid_t *group_info;
	tm_drv_data_t *data;

	tmvdbg("cmd: %d arg: %ld\n", cmd, arg);

	data = (tm_drv_data_t *)arg;
	if (data == NULL) {
		tmdbg("%s data\n", clog_message_str[CMN_LOG_NULL_CHECK_FAIL]);
		return ERROR;
	}

	/* Handle built-in ioctl commands */

	switch (cmd) {
	case TMIOC_START:
		ret = taskmgr_task_init(data->pid, data->addr);
		if (ret != OK) {
			tmdbg("%s\n", clog_message_str[CMN_LOG_FAILED_OP]);
		}
		break;
	case TMIOC_PAUSE:
		tcb = sched_gettcb(data->pid);
		if (tcb == NULL) {
			tmdbg("%s pid\n", clog_message_str[CMN_LOG_NULL_CHECK_FAIL]);
			return ERROR;
		}
		if (tcb->task_state == TSTATE_WAIT_SIG && tcb->waitdog != NULL) {
			/* tcb is waiting another signal, e.g. sleep */
			wd_cancel(tcb->waitdog);
		} else if (tcb->task_state == TSTATE_WAIT_SEM) {
			tcb->waitsem = NULL;
			sched_removeblocked(tcb);
			sched_addblocked(tcb, TSTATE_WAIT_SIG);
		}
		ret = OK;
		break;
	case TMIOC_UNICAST:
		tcb = sched_gettcb(data->pid);
		if (tcb == NULL) {
			tmdbg("%s pid\n", clog_message_str[CMN_LOG_NULL_CHECK_FAIL]);
			return ERROR;
		}
		ret = (int)sig_is_handler_registered(tcb, SIGTM_UNICAST);
		if ((bool)ret != true) {
			tmdbg("handler is not registered for unicast\n");
			ret = ERROR;
		} else {
			ret = OK;
		}
		break;
	case TMIOC_BROADCAST:
		tcb = sched_gettcb(data->pid);
		if (tcb == NULL) {
			tmdbg("%s pid\n", clog_message_str[CMN_LOG_NULL_CHECK_FAIL]);
			return ERROR;
		}
		ret = (int)sig_is_handler_registered(tcb, SIGTM_BROADCAST);
		if ((bool)ret != true) {
			tmdbg("handler is not registered for broadcast\n");
			ret = ERROR;
		} else {
			ret = OK;
		}
		break;
	case TMIOC_CHECK_ALIVE:
		tcb = sched_gettcb(data->pid);
		if (tcb == NULL) {
			tmdbg("%s pid\n", clog_message_str[CMN_LOG_NULL_CHECK_FAIL]);
			return ERROR;
		}
		ret = OK;
		break;
	case TMIOC_TERMINATE:
		tcb = sched_gettcb(data->pid);
		if (tcb == NULL) {
			tmdbg("%s pid\n", clog_message_str[CMN_LOG_NULL_CHECK_FAIL]);
			return ERROR;
		}
		if ((tcb->flags & TCB_FLAG_TTYPE_MASK) >> TCB_FLAG_TTYPE_SHIFT == TCB_FLAG_TTYPE_TASK) {
			ret = task_delete(tcb->pid);
			if (ret != OK) {
				return ERROR;
			}
		}
#ifndef CONFIG_DISABLE_PTHREAD
		else {
			(void)pthread_detach(tcb->pid);
			ret = pthread_cancel(tcb->pid);
			if (ret != OK) {
				return ERROR;
			}
		}
#endif
		break;
#if defined(HAVE_TASK_GROUP) && !defined(CONFIG_DISABLE_PTHREAD)
	case TMIOC_PTHREAD_PARENT:
		group_info = (tm_pthread_pid_t *)data->addr;
		if (group_info == NULL) {
			return ERROR;
		}

		/* First, the group of pthread is detached from the task manager */
		taskmgr_pthread_group_detach(group_info->child_pid);

		/* After the group detach procedure, the pthread group is joined with the group of parent */
		ret = taskmgr_pthread_group_join(group_info->parent_pid, group_info->child_pid);
		if (ret != OK) {
			return ERROR;
		}

#if defined(CONFIG_SCHED_HAVE_PARENT) && !defined(HAVE_GROUP_MEMBERS)
		/* Afeter the pthread group join proceduere, the parent pid of pthread is changed into parent pid */
		ret = taskmgr_pthread_ppid_change(group_info->parent_pid, group_info->child_pid);
		if (ret != OK) {
			return ERROR;
		}
#endif
		break;
#endif
	case TMIOC_EXITCB:
		tm_set_exit_cb((tm_exit_cb_t)data->addr);
		ret = OK;
		break;
	default:
		tmdbg("%s cmd: %d arg: %ld\n", clog_message_str[CMN_LOG_INVALID_VAL], cmd, arg);
		break;
	}
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: task_manager_drv_register
 *
 * Description:
 *   Register task management path, TASK_MANAGER_DRVPATH
 *
 ****************************************************************************/

void task_manager_drv_register(void)
{
	(void)register_driver(TASK_MANAGER_DRVPATH, &taskmgr_fops, 0666, NULL);
}
