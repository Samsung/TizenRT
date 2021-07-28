#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <tinyara/timer.h>
#include <netutils/netlib.h>
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
	hnd->elapsed = hnd->end.timeleft - hnd->start.timeleft;
	return 0;
}

int sldrv_destroy_time(sldrv_timer_handle *hnd)
{
	close(hnd->fd);
	return 0;
}
