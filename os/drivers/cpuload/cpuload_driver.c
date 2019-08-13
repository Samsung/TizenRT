/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <debug.h>
#include <errno.h>
#include <time.h>

#include <sys/types.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/sched.h>
#include <tinyara/cpuload.h>


/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int cpuload_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t cpuload_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t cpuload_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct file_operations cpuload_fops = {
	0,                          /* open */
	0,                          /* close */
	cpuload_read,               /* read */
	cpuload_write,              /* write */
	0,                          /* seek */
	cpuload_ioctl               /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static ssize_t cpuload_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;
}

static ssize_t cpuload_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return 0;
}

/************************************************************************************
 * Name: cpuload_ioctl
 *
 * Description: The ioctl method for cpuload.
 *
 ************************************************************************************/
static int cpuload_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret;
	int ticks;
	pid_t *result_addr;

	ret = -EINVAL;

	/* Handle built-in ioctl commands */

	switch (cmd) {
	case CPULOADIOC_START:
		if (arg > 0) {
			ticks = arg * CLOCKS_PER_SEC;
			ret = sched_start_cpuload_snapshot(ticks);
		}
		break;
	case CPULOADIOC_STOP:
			sched_clear_cpuload_snapshot();
			ret = OK;
		break;
	case CPULOADIOC_GETVALUE:
		result_addr = (pid_t *)arg;
		if (result_addr != NULL) {
			sched_get_cpuload_snapshot(result_addr);
			ret = OK;
		}
		break;
	default:
		break;
	}
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cpuload_initialize
 *
 * Description:
 *   Register cpuload driver path, CPULOAD_DRVPATH
 *
 ****************************************************************************/

void cpuload_initialize(void)
{
	/* Register cpuload driver path, CPULOAD_DRVPATH. */
	(void)register_driver(CPULOAD_DRVPATH, &cpuload_fops, 0666, NULL);
}
