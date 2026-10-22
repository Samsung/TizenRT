/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <errno.h>
#include <stdint.h>

#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/health_monitor.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_read
 *
 * Description:
 *   Reject data reads from the ioctl-only device.
 *
 * Inputs:
 *   filep  - Open file, unused.
 *   buffer - Destination buffer, unused.
 *   len    - Requested byte count, unused.
 *
 * Return Value:
 *   -ENOSYS.
 *
 ****************************************************************************/

static ssize_t health_monitor_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return -ENOSYS;
}

/****************************************************************************
 * Name: health_monitor_write
 *
 * Description:
 *   Reject data writes to the ioctl-only device.
 *
 * Inputs:
 *   filep  - Open file, unused.
 *   buffer - Source buffer, unused.
 *   len    - Requested byte count, unused.
 *
 * Return Value:
 *   -ENOSYS.
 *
 ****************************************************************************/

static ssize_t health_monitor_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return -ENOSYS;
}

/****************************************************************************
 * Name: health_monitor_ioctl
 *
 * Description:
 *   Dispatch START, KICK and STOP to the registry for the calling thread.
 *
 * Inputs:
 *   filep - Open file, unused.
 *   cmd   - Health monitor ioctl command.
 *   arg   - START timeout in milliseconds by value; ignored by KICK/STOP.
 *
 * Return Value:
 *   Registry result; -ENOTTY for an unknown command.
 *
 * Assumptions:
 *   Called in the requesting thread's context.
 *
 ****************************************************************************/

static int health_monitor_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	switch (cmd) {
	case HMIOC_START:
		return health_monitor_start((uint32_t)arg);

	case HMIOC_KICK:
		health_monitor_kick();
		return OK;

	case HMIOC_STOP:
		return health_monitor_stop();

	default:
		return -ENOTTY;
	}
}

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Open and close do not START or STOP monitoring. */

static const struct file_operations g_health_monitor_fops = {
	.read = health_monitor_read,
	.write = health_monitor_write,
	.ioctl = health_monitor_ioctl,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_register
 *
 * Description:
 *   Register /dev/health_monitor during system bring-up.
 *
 * Inputs:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Assumptions:
 *   Called once after VFS initialization and before application startup.
 *
 ****************************************************************************/

void health_monitor_register(void)
{
	(void)register_driver(HEALTH_MONITOR_DEVPATH, &g_health_monitor_fops, 0666, NULL);
}
