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
#include <sys/types.h>
#include <tinyara/sched.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/taskmgt.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int taskmgt_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t taskmgt_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t taskmgt_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/
#define MAX_HANDLE_MASK (CONFIG_TASK_MANAGER_MAX_TASKS - 1)
#define HANDLE_HASH(i)  ((i) & MAX_HANDLE_MASK)

static uint32_t g_lasthandle;
static bool g_handle_hash[CONFIG_TASK_MANAGER_MAX_TASKS];

struct taskmgt_dev_s {
	mqd_t mqdes;	   /* A mqueue descriptor */
};

static const struct file_operations taskmgt_fops = {
	0,                          /* open */
	0,                          /* close */
	taskmgt_read,               /* read */
	taskmgt_write,              /* write */
	0,                          /* seek */
	taskmgt_ioctl               /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int taskmgt_assign_handle(void)
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
	set_errno(EBUSY);
	return ERROR;
}

static ssize_t taskmgt_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;
}

static ssize_t taskmgt_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return 0;
}

/************************************************************************************
 * Name: taskmgt_ioctl
 *
 * Description:  The ioctl method for task management.
 *
 ************************************************************************************/
static int taskmgt_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	tm_request_t *request_msg;
	task_builtin_list_t *task_info;

	tmvdbg("cmd: %d arg: %ld\n", cmd, arg);

	/* Handle built-in ioctl commands */

	switch (cmd) {
	case TMIOC_REGISTER_TASK:
		request_msg = (tm_request_t *)arg;
		request_msg->handle = taskmgt_assign_handle();
		tmvdbg("Registered handle %d\n", request_msg->handle);
		ret = OK;
		break;
	case TMIOC_UNREGISTER_TASK:
		request_msg = (tm_request_t *)arg;
		g_handle_hash[request_msg->handle] = false;
		tmvdbg("Unregistered handle %d\n", request_msg->handle);
		ret = OK;
		break;
	case TMIOC_START:
		task_info = (task_builtin_list_t *)arg;
		ret = task_create(task_info->name, task_info->priority, task_info->stacksize, task_info->entry, (char * const *)NULL);
		if (ret == ERROR) {
			tmdbg("Fail to create new task\n");
			return ret;
		}
		break;
	case TMIOC_TERMINATE:
		ret = task_delete((int)arg);
		if (ret != OK) {
			tmdbg("Fail to delete the task\n");
			return ret;
		}
		break;
	case TMIOC_RESTART:
		break;
	case TMIOC_PAUSE:
		break;
	case TMIOC_RESUME:
		break;
	case TMIOC_SCAN:
		break;
	case TMIOC_UNICAST:
		break;
	case TMIOC_BROADCAST:
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
 * Name: taskmgt_register
 *
 * Description:
 *   Register task management path, TASKMGT_DRVPATH
 *
 ****************************************************************************/

void taskmgt_register(void)
{
	(void)register_driver(TASKMGT_DRVPATH, &taskmgt_fops, 0666, NULL);
}
