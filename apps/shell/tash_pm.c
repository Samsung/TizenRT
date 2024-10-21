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

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <tinyara/pm/pm.h>
#include <errno.h>
#include "tash_internal.h"

#define TASH_PM_DOMAIN_NAME "TASH"

static int tash_pm_domain_id = -1;
static int pmdrv_fd = -1;

void tash_pm_open_driver(void)
{
	if (pmdrv_fd < 0) {
		pmdrv_fd = open(PM_DRVPATH, O_WRONLY);
		if (pmdrv_fd < 0) {
			shdbg("open %s failed(%d), \n", PM_DRVPATH, get_errno());
		}
	}
}

void tash_pm_close_driver(void)
{
	if (pmdrv_fd >= 0) {
		close(pmdrv_fd);
		pmdrv_fd = -1;
	}
}

int tash_pm_get_domain_id(void)
{
	pm_domain_arg_t pm_domain_arg;
	if (tash_pm_domain_id == -1) {
		pm_domain_arg.domain_name = TASH_PM_DOMAIN_NAME;
		if (pmdrv_fd < 0) {
			shdbg("First open pm driver using tash_pm_open_driver() to register %s domain\n", TASH_PM_DOMAIN_NAME);
		} else if (ioctl(pmdrv_fd, PMIOC_DOMAIN_REGISTER, &pm_domain_arg) != OK) {
			shdbg("pm_domain_register failed(%d)\n", get_errno());
		} else {
			tash_pm_domain_id = pm_domain_arg.domain_id;
		}
	}
	return tash_pm_domain_id;
}

void tash_pm_timedsuspend(uint32_t milliseconds)
{
	pm_suspend_arg_t pm_suspend_arg;
	pm_suspend_arg.domain_id = tash_pm_get_domain_id();
	pm_suspend_arg.timer_interval = milliseconds;
	if (pmdrv_fd < 0) {
		shdbg("First open pm driver using tash_pm_open_driver() to timedsuspend %s domain\n", TASH_PM_DOMAIN_NAME);
	} else if (ioctl(pmdrv_fd, PMIOC_TIMEDSUSPEND, &pm_suspend_arg) != OK) {
		shdbg("Unable to timedsuspend the %s domain for %d ms, error = %d\n", TASH_PM_DOMAIN_NAME, milliseconds, get_errno());
	}
}

void tash_pm_suspend(void)
{
	if (pmdrv_fd < 0) {
		shdbg("First open pm driver using tash_pm_open_driver() to register %s domain\n", TASH_PM_DOMAIN_NAME);
	} else if (ioctl(pmdrv_fd, PMIOC_SUSPEND, tash_pm_get_domain_id()) != OK) {
		shdbg("tash_pm_suspend failed(%d)\n", get_errno());
	}
}

void tash_pm_resume(void)
{
	if (pmdrv_fd < 0) {
		shdbg("First open pm driver using tash_pm_open_driver() to register %s domain\n", TASH_PM_DOMAIN_NAME);
	} else if (ioctl(pmdrv_fd, PMIOC_RESUME, tash_pm_get_domain_id()) != OK) {
		shdbg("tash_pm_resume failed(%d)\n", get_errno());
	}
}
