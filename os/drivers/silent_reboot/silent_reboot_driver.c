/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#include <tinyara/silent_reboot.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int silent_reboot_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t silent_reboot_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t silent_reboot_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct file_operations silent_reboot_fops = {
	0,                          /* open */
	0,                          /* close */
	silent_reboot_read,         /* read */
	silent_reboot_write,        /* write */
	0,                          /* seek */
	silent_reboot_ioctl         /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static ssize_t silent_reboot_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;
}

static ssize_t silent_reboot_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return 0;
}

/************************************************************************************
 * Name: silent_reboot_ioctl
 *
 * Description: The ioctl method for silent reboot.
 *
 ************************************************************************************/
static int silent_reboot_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret;
	int ticks;
	pid_t *result_addr;

	ret = -EINVAL;

	/* Handle built-in ioctl commands */

	switch (cmd) {
	case SILENTRBIOC_LOCK:
		ret = silent_reboot_lock();
		break;
	case SILENTRBIOC_UNLOCK:
		ret = silent_reboot_unlock();
		break;
	case SILENTRBIOC_DELAY:
		ret = silent_reboot_delay((int)arg);
		break;
	case SILENTRBIOC_CHECKMODE:
		ret = silent_reboot_is_silent_mode((bool *)arg);
		break;
	case SILENTRBIOC_GETSTATUS:
		ret = silent_reboot_get_status((silent_reboot_status_t *)arg);
		break;
	case SILENTRBIOC_FORCE_REBOOT:
		ret = silent_reboot_force_perform_after_timeout((int)arg);
		break;
	default:
		dbg("Invalid cmd %d\n", cmd);
		break;
	}
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: silent_reboot_driver_register
 *
 * Description:
 *   Register a driver path, SILENT_REBOOT_DRVPATH
 *
 ****************************************************************************/

void silent_reboot_driver_register(void)
{
	/* Register silent_reboot driver path, SILENT_REBOOT_DRVPATH. */
	(void)register_driver(SILENT_REBOOT_DRVPATH, &silent_reboot_fops, 0666, NULL);
}
