/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * drivers/power/battery_gauge.c
 * Upper-half, character driver for battery fuel gauge.
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/common_logs/common_logs.h>

#include <stdbool.h>
#include <semaphore.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/fs/fs.h>
#include <tinyara/power/battery_gauge.h>
#include <tinyara/power/battery_ioctl.h>

/* This driver requires:
 *
 * CONFIG_BATTERY_GAUGE - Upper half battery driver support
 */

#if defined(CONFIG_BATTERY_GAUGE)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Character driver methods */

static int bat_gauge_open(FAR struct file *filep);
static int bat_gauge_close(FAR struct file *filep);
static ssize_t bat_gauge_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t bat_gauge_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static int bat_gauge_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_batteryops = {
	bat_gauge_open,
	bat_gauge_close,
	bat_gauge_read,
	bat_gauge_write,
	0,
	bat_gauge_ioctl
#ifndef CONFIG_DISABLE_POLL
	, 0
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: bat_gauge_open
 *
 * Description:
 *   This function is called whenever the battery device is opened.
 *
 ****************************************************************************/

static int bat_gauge_open(FAR struct file *filep)
{
	return OK;
}

/****************************************************************************
 * Name: bat_gauge_close
 *
 * Description:
 *   This routine is called when the battery device is closed.
 *
 ****************************************************************************/

static int bat_gauge_close(FAR struct file *filep)
{
	return OK;
}

/****************************************************************************
 * Name: bat_gauge_read
 ****************************************************************************/

static ssize_t bat_gauge_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	/* Return nothing read */

	return 0;
}

/****************************************************************************
 * Name: bat_gauge_write
 ****************************************************************************/

static ssize_t bat_gauge_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	/* Return nothing written */

	return 0;
}

/****************************************************************************
 * Name: bat_gauge_ioctl
 ****************************************************************************/

static int bat_gauge_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct battery_gauge_dev_s *dev = inode->i_private;
	int ret;

	/* Inforce mutually exclusive access to the battery driver */

	ret = sem_wait(&dev->batsem);
	if (ret < 0) {
		return -errno;			/* Probably EINTR */
	}

	/* Procss the IOCTL command */

	ret = -EINVAL;				/* Assume a bad argument */
	switch (cmd) {
	case BATIOC_STATE: {
		FAR int *ptr = (FAR int *)((uintptr_t)arg);
		if (ptr) {
			ret = dev->ops->state(dev, ptr);
		}
	}
	break;

	case BATIOC_ONLINE: {
		FAR bool *ptr = (FAR bool *)((uintptr_t)arg);
		if (ptr) {
			ret = dev->ops->online(dev, ptr);
		}
	}
	break;

	case BATIOC_VOLTAGE: {
		FAR b16_t *ptr = (FAR b16_t *)((uintptr_t)arg);
		if (ptr) {
			ret = dev->ops->voltage(dev, ptr);
		}
	}
	break;

	case BATIOC_CAPACITY: {
		FAR b16_t *ptr = (FAR b16_t *)((uintptr_t)arg);
		if (ptr) {
			ret = dev->ops->capacity(dev, ptr);
		}
	}
	break;

	default:
		pmdbg("%s cmd: %d\n", clog_message_str[CMN_LOG_INVALID_VAL], cmd);
		ret = -ENOTTY;
		break;
	}

	sem_post(&dev->batsem);
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: battery_gauge_register
 *
 * Description:
 *   Register a lower half battery driver with the common, upper-half
 *   battery driver.
 *
 * Input parameters:
 *   devpath - The location in the pseudo-filesystem to create the driver.
 *     Recommended standard is "/dev/bat0", "/dev/bat1", etc.
 *   dev - An instance of the battery state structure .
 *
 * Returned value:
 *    Zero on success or a negated errno value on failure.
 *
 ****************************************************************************/

int battery_gauge_register(FAR const char *devpath, FAR struct battery_gauge_dev_s *dev)
{
	int ret;

	/* Register the character driver */

	ret = register_driver(devpath, &g_batteryops, 0555, dev);
	if (ret < 0) {
		pmdbg("%s %d\n", clog_message_str[CMN_LOG_FAILED_OP], ret);
	}

	return ret;
}
#endif							/* CONFIG_BATTERY_GAUGE */
