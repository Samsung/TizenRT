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
#include <stdint.h>
#include <errno.h>
#include <debug.h>
#include <signal.h>
#include <apps/builtin.h>
#include <sys/types.h>
#include <tinyara/sched.h>
#include <tinyara/signal.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/wdog.h>
#include <tinyara/task_manager_internal.h>

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
static void taskmgr_pause_handler(int signo, siginfo_t *info, void *extra)
{
	if (signo != SIGTM_PAUSE) {
		tmdbg("[TM] Invalid signal. signo = %d\n", signo);
		return;
	}

	sigpause(SIGTM_RESUME);
}

static int taskmgr_task_init(pid_t pid)
{
	struct tcb_s *tcb;
	struct sigaction act;

	tcb = sched_gettcb(pid);
	if (!tcb) {
		tmdbg("[TM] tcb is invalid. pid = %d.\n", pid);
		return ERROR;
	}

	memset(&act, '\0', sizeof(act));
	act.sa_handler = (_sa_handler_t)&taskmgr_pause_handler;

	return sig_sethandler(tcb, SIGTM_PAUSE, &act);
}

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
	int ret = -EINVAL;
	struct tcb_s *tcb;

	tmvdbg("cmd: %d arg: %ld\n", cmd, arg);

	/* Handle built-in ioctl commands */

	switch (cmd) {
	case TMIOC_START:
		ret = taskmgr_task_init((int)arg);
		if (ret != OK) {
			tmdbg("Fail to init new task\n");
		}
		break;
	case TMIOC_PAUSE:
		tcb = sched_gettcb((int)arg);
		if (tcb == NULL) {
			tmdbg("Invalid pid\n");
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
		tcb = sched_gettcb((int)arg);
		if (tcb == NULL) {
			tmdbg("Invalid pid\n");
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
	case TMIOC_RESTART:
		break;
	case TMIOC_BROADCAST:
		tcb = sched_gettcb((int)arg);
		if (tcb == NULL) {
			tmdbg("Invalid pid\n");
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
		tcb = sched_gettcb((int)arg);
		if (tcb == NULL) {
			tmdbg("Invalid pid\n");
			return ERROR;
		}
		ret = OK;
		break;
	default:
		tmdbg("Unrecognized cmd: %d arg: %ld\n", cmd, arg);
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
