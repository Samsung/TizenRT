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
 * drivers/timer.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Authors: Gregory Nutt <gnutt@nuttx.org>
 *            Bob Doiron
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

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/fs/fs.h>
#include <tinyara/irq.h>
#include <tinyara/kmalloc.h>
#include <tinyara/timer.h>

#ifdef CONFIG_TIMER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Definitions
 ****************************************************************************/

/* This structure describes the state of the upper half driver */

struct timer_upperhalf_s {
	uint8_t   crefs; /* The number of times the device has been opened */
	pid_t     pid;   /* The ID of the task/thread to receive the signal */
#ifdef CONFIG_PM
	pm_timer_type_t  timer_type; /* The timer type of the task/thread to receive the signal */
#endif
	FAR void *arg;   /* An argument to pass with the signal */
	FAR char *path;  /* Registration path */

	/* The contained lower-half driver */

	FAR struct timer_lowerhalf_s *lower;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static bool    timer_notifier(FAR uint32_t *next_interval_us, FAR void *arg);
static int     timer_open(FAR struct file *filep);
static int     timer_close(FAR struct file *filep);
static ssize_t timer_read(FAR struct file *filep, FAR char *buffer,
		size_t buflen);
static ssize_t timer_write(FAR struct file *filep, FAR const char *buffer,
		size_t buflen);
static int     timer_ioctl(FAR struct file *filep, int cmd,
		unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_timerops = {
	timer_open,	/* open */
	timer_close,	/* close */
	timer_read,	/* read */
	timer_write,	/* write */
	NULL,		/* seek */
	timer_ioctl	/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, NULL		/* poll */
#endif
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
	, NULL		/* unlink */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: timer_notifier
 *
 * Description:
 *   Notify the application via a signal when the timer interrupt occurs
 *
 * REVISIT: This function prototype is insufficient to support signaling
 *
 ****************************************************************************/

static bool timer_notifier(FAR uint32_t *next_interval_us, FAR void *arg)
{
	FAR struct timer_upperhalf_s *upper = (FAR struct timer_upperhalf_s *)arg;
	DEBUGASSERT(upper != NULL);

	/* Send notification to the waiter.. */
	/* Do we still need to notify caller here, if fin_wait is not invoked? */
	fin_notify(upper->pid, (int)upper->arg);

	/* In timer_ioctl(TCIOC_NOTIFICATION), the callback registered is timer_notifier (this API)
	   there is no invoke for PM related control API, need to have another step to fulfil this
	*/
#ifdef CONFIG_PM
	timer_int_handler(next_interval_us, upper->timer_type);
#endif

	return true;
}

/****************************************************************************
 * Name: timer_open
 *
 * Description:
 *   This function is called whenever the timer device is opened.
 *
 ****************************************************************************/

static int timer_open(FAR struct file *filep)
{
	FAR struct inode             *inode = filep->f_inode;
	FAR struct timer_upperhalf_s *upper = inode->i_private;
	uint8_t                       tmp;
	int                           ret;

	tmrdbg("crefs: %d\n", upper->crefs);

	/*
	 * Increment the count of references to the device. If this the first
	 * time that the driver has been opened for this device, then
	 * initialize the device.
	 */

	tmp = upper->crefs + 1;
	if (tmp == 0) {
		/* More than 255 opens; uint8_t overflows to zero */

		ret = -EMFILE;
		goto errout;
	}

	/* Save the new open count */

	upper->crefs = tmp;
	ret = OK;

errout:
	return ret;
}

/****************************************************************************
 * Name: timer_close
 *
 * Description:
 *   This function is called when the timer device is closed.
 *
 ****************************************************************************/

static int timer_close(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct timer_upperhalf_s *upper = inode->i_private;

	tmrdbg("crefs: %d\n", upper->crefs);

	/*
	 * Decrement the references to the driver. If the reference count
	 * will decrement to 0, then uninitialize the driver.
	 */

	if (upper->crefs > 0) {
		upper->crefs--;
	}

	return OK;
}

/****************************************************************************
 * Name: timer_read
 *
 * Description:
 *   A dummy read method.  This is provided only to satisfy the VFS layer.
 *
 ****************************************************************************/

static ssize_t timer_read(FAR struct file *filep, FAR char *buffer,
			  size_t buflen)
{
	/* Return zero - usually meaning end-of-file */

	return 0;
}

/****************************************************************************
 * Name: timer_write
 *
 * Description:
 *   A dummy write method.  This is provided only to satisfy the VFS layer.
 *
 ****************************************************************************/

static ssize_t timer_write(FAR struct file *filep, FAR const char *buffer,
			   size_t buflen)
{
	return 0;
}

/****************************************************************************
 * Name: timer_ioctl
 *
 * Description:
 *   The standard ioctl method.  This is where ALL of the timer work is
 *   done.
 *
 ****************************************************************************/

static int timer_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode             *inode = filep->f_inode;
	FAR struct timer_upperhalf_s *upper = inode->i_private;
	FAR struct timer_lowerhalf_s *lower = upper->lower;
	int                           ret;

	tmrdbg("cmd: %d arg: %ld\n", cmd, arg);
	DEBUGASSERT(upper && lower);

	/* Handle built-in ioctl commands */

	switch (cmd) {

	/*
	 * cmd:         TCIOC_START
	 * Description: Start the timer
	 * Argument:    Ignored
	 */

	case TCIOC_START: {
		/* Start the timer, resetting the time to the current timeout */

		if (lower->ops->start) {
			ret = lower->ops->start(lower);
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	/*
	 * cmd:         TCIOC_STOP
	 * Description: Stop the timer
	 * Argument:    Ignored
	 */

	case TCIOC_STOP: {
		/* Stop the timer */

		if (lower->ops->stop) {
			ret = lower->ops->stop(lower);
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	/*
	 * cmd:         TCIOC_GETSTATUS
	 * Description: Get the status of the timer.
	 * Argument:    A writable pointer to struct timer_status_s.
	 */

	case TCIOC_GETSTATUS: {
		FAR struct timer_status_s *status;

		/* Get the current timer status */

		if (lower->ops->getstatus) { /* Optional */
			status = (FAR struct timer_status_s *)((uintptr_t)arg);
			if (status) {
				ret = lower->ops->getstatus(lower, status);
			} else {
				ret = -EINVAL;
			}
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	/*
	 * cmd:         TCIOC_SETTIMEOUT
	 * Description: Reset the timeout to this value
	 * Argument:    A 32-bit timeout value in microseconds.
	 *
	 * TODO: pass pointer to uint64 ns? Need to determine if these timers
	 * are 16 or 32 bit...
	 */

	case TCIOC_SETTIMEOUT: {
		/* Set a new timeout value (and reset the timer) */

		if (lower->ops->settimeout) {	/* Optional */
			ret = lower->ops->settimeout(lower, (uint32_t)arg);
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	/*
	 * cmd:         TCIOC_NOTIFICATION
	 * Description: Notify application via a signal when the timer expires.
	 * Argument:    signal number
	 *
	 * NOTE: This ioctl cannot be support in the kernel build mode. In
	 * that case direct callbacks from kernel space into user space is
	 * forbidden.
	 */

	case TCIOC_NOTIFICATION: {
		FAR struct timer_notify_s *notify =
			(FAR struct timer_notify_s *)((uintptr_t)arg);

		if (notify != NULL) {
			upper->pid   = notify->pid;
			upper->arg   = notify->arg;
#ifdef CONFIG_PM
			upper->timer_type = notify->timer_type;
#endif

			ret = timer_setcallback((FAR void *)upper,
					timer_notifier, upper);
		} else {
			ret = -EINVAL;
		}
	 }
	 break;

	/* Any unrecognized IOCTL commands might be platform-specific ioctl commands */

	default: {
		tmrdbg("Forwarding unrecognized cmd: %d arg: %ld\n", cmd, arg);

		/*
		 * An ioctl commands that are not recognized by the
		 * "upper-half" driver are forwarded to the lower half driver
		 * through this method.
		 */

		if (lower->ops->ioctl) { /* Optional */
			ret = lower->ops->ioctl(lower, cmd, arg);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	}

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: timer_register
 *
 * Description:
 *   This function binds an instance of a "lower half" timer driver with the
 *   "upper half" timer device and registers that device so that can be used
 *   by application code.
 *
 *   When this function is called, the "lower half" driver should be in the
 *   disabled state (as if the stop() method had already been called).
 *
 * Input parameters:
 *   dev path - The full path to the driver to be registers in the TinyAra
 *     pseudo-filesystem.  The recommended convention is to name all timer
 *     drivers as "/dev/tc0", "/dev/tc1", etc.  where the driver
 *     path differs only in the "minor" number at the end of the device name.
 *   lower - A pointer to an instance of lower half timer driver.  This
 *     instance is bound to the timer driver and must persists as long as
 *     the driver persists.
 *
 * Returned Value:
 *   On success, a non-NULL handle is returned to the caller.  In the event
 *   of any failure, a NULL value is returned.
 *
 ****************************************************************************/

FAR void *timer_register(FAR const char *path,
			 FAR struct timer_lowerhalf_s *lower)
{
	FAR struct timer_upperhalf_s *upper;
	int ret;

	DEBUGASSERT(path && lower);
	tmrdbg("Entry: path=%s\n", path);

	/* Allocate the upper-half data structure */

	upper = (FAR struct timer_upperhalf_s *)
			kmm_zalloc(sizeof(struct timer_upperhalf_s));
	if (!upper) {
		tmrlldbg("ERROR: Upper half allocation failed\n");
		goto errout;
	}

	/*
	 * Initialize the timer device structure (it was already zeroed
	 * by kmm_zalloc()).
	 */

	upper->lower = lower;

	/* Copy the registration path */

	upper->path = strdup(path);
	if (!upper->path) {
		tmrlldbg("ERROR: Path allocation failed\n");
		goto errout_with_upper;
	}

	/* Register the timer device */

	ret = register_driver(path, &g_timerops, 0666, upper);
	if (ret < 0) {
		tmrlldbg("ERROR: register_driver failed: %d\n", ret);
		goto errout_with_path;
	}

	return (FAR void *)upper;

errout_with_path:
	kmm_free(upper->path);

errout_with_upper:
	kmm_free(upper);

errout:
	return NULL;
}

/****************************************************************************
 * Name: timer_unregister
 *
 * Description:
 *   This function can be called to disable and unregister the timer
 *   device driver.
 *
 * Input parameters:
 *   handle - This is the handle that was returned by timer_register()
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void timer_unregister(FAR void *handle)
{
	FAR struct timer_upperhalf_s *upper;
	FAR struct timer_lowerhalf_s *lower;

	/* Recover the pointer to the upper-half driver state */

	upper = (FAR struct timer_upperhalf_s *)handle;
	DEBUGASSERT(upper != NULL && upper->lower != NULL);
	lower = upper->lower;

	tmrdbg("Unregistering: %s\n", upper->path);

	/* Disable the timer */

	DEBUGASSERT(lower->ops->stop); /* Required */
	(void)lower->ops->stop(lower);

	/* Unregister the timer device */

	(void)unregister_driver(upper->path);

	/* Then free all of the driver resources */

	kmm_free(upper->path);
	kmm_free(upper);
}

/****************************************************************************
 * Name: timer_setcallback
 *
 * Description:
 *   This function can be called to add a callback into driver-related code
 *   to handle timer expirations. This is a strictly OS internal interface
 *   and may NOT be used by application code.
 *
 * Input parameters:
 *   handle   - This is the handle that was returned by timer_register()
 *   callback - The new timer interrupt callback
 *   arg      - Argument to be provided with the callback
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
int timer_setcallback(FAR void *handle, tccb_t callback, FAR void *arg)
{
	FAR struct timer_upperhalf_s *upper;
	FAR struct timer_lowerhalf_s *lower;

	/* Recover the pointer to the upper-half driver state */

	upper = (FAR struct timer_upperhalf_s *)handle;
	DEBUGASSERT(upper != NULL && upper->lower != NULL);
	lower = upper->lower;
	DEBUGASSERT(lower->ops != NULL);

	/* Check if the lower half driver supports the setcallback method */

	if (lower->ops->setcallback != NULL) { /* Optional */
		/* Yes.. Defer the handler attachment to the lower half driver */
		lower->ops->setcallback(lower, callback, arg);
		return OK;
	}

	return -ENOSYS;
}
#endif /* CONFIG_TIMER */
