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
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/taskmgt.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
struct taskmgt_dev_s {
	mqd_t mqdes;	   /* A mqueue descriptor */
};

static struct taskmgt_dev_s g_taskmgt_dev = {
	0,                 /* mqdes */
};

static int taskmgt_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations taskmgt_fops = {
	0,                          /* open */
	0,                          /* close */
	0,                          /* read */
	0,                          /* write */
	0,                          /* seek */
	taskmgt_ioctl               /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/************************************************************************************
 * Name: taskmgt_ioctl
 *
 * Description:  The ioctl method for task management.
 *
 ************************************************************************************/

static int taskmgt_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	int ret = -EINVAL;

	vdbg("cmd: %d arg: %ld\n", cmd, arg);

	/* Handle built-in ioctl commands */

	switch (cmd) {
	case TASKMGT_REGISTER_TASK:
		//search built-in list and add it to task list
		break;
	case TASKMGT_REGISTER_THREAD:
		//add to list
		break;
	case TASKMGT_UNREGISTER:
		//remove from list
		break;
	case TASKMGT_START:
		//task create
		break;
	case TASKMGT_TERMINATE:
		//task terminate
		break;
	case TASKMGT_PAUSE:
		break;
	case TASKMGT_RESUME:
		break;
	case TASKMGT_SCAN:
		//show task list
		break;
	case TASKMGT_UNICAST:
		break;
	case TASKMGT_BROADCAST:
		break;
	default:
		vdbg("Unrecognized cmd: %d arg: %ld\n", cmd, arg);
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
 *   Register task management path, "/dev/taskmgt"
 *
 ****************************************************************************/

void taskmgt_register(void)
{
	(void)register_driver("/dev/taskmgt", &taskmgt_fops, 0666, NULL);
}
