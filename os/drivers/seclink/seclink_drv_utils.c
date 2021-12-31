/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <fcntl.h>
#include <tinyara/timer.h>
#include "seclink_drv_utils.h"

int sldrv_start_time(sldrv_timer_handle *hnd)
{
	hnd->fd = open("/dev/timer0", O_RDONLY);
	if (hnd->fd < 0) {
		return -1;
	}

	ioctl(hnd->fd, TCIOC_SETMODE, MODE_FREERUN);
	ioctl(hnd->fd, TCIOC_START, 0);
	ioctl(hnd->fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&hnd->start);
	return 0;
}

int sldrv_get_time(sldrv_timer_handle *hnd)
{
	ioctl(hnd->fd, TCIOC_GETSTATUS, (unsigned long)(uintptr_t)&hnd->end);
	ioctl(hnd->fd, TCIOC_STOP, 0);
	close(hnd->fd);
	hnd->elapsed = hnd->end.timeleft - hnd->start.timeleft;
	return 0;
}
