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
#include <queue.h>
#include <assert.h>
#include <time.h>
#include <debug.h>
#include <tinyara/pm/pm.h>
#include <tinyara/irq.h>
#include <tinyara/lcd/lcd_dev.h>
#include <tinyara/wqueue.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "pm.h"

#ifdef CONFIG_PM

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LCD_DEVPATH "/dev/lcd0"

/****************************************************************************
 * Private Variable
 ****************************************************************************/

static struct work_s pm_lcd_worker;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int pm_lcd_get_power(void)
{
	int power = 0;
	if (g_pmglobals.state == PM_NORMAL) {
		power = CONFIG_LCD_NORMAL_POWER;
	} else if (g_pmglobals.state == PM_IDLE) {
		power = CONFIG_LCD_IDLE_POWER;
	}
	return power;
}

static void pm_lcd_set_power(void *arg)
{
	int fd;
	/* Change LCD Power */
	fd = open(LCD_DEVPATH, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		pmdbg("Unable to open LCD Driver\n");
		return;
	}
	if (ioctl(fd, LCDDEVIO_SETPOWER, pm_lcd_get_power()) != OK) {
		pmdbg("Unable to change LCD Power\n");
	}
	close(fd);
}
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_lcd_change_backlight
 *
 * Description:
 *   It create work queue to change LCD backlight. Since we can't change LCD
 *   Backlight in ISR, we need work-queue to facilitate it.
 *
 * Input parameters:
 *   None
 *
 * Returned Value:
 *   Zero on success, a negative errno on failure
 *
 ****************************************************************************/
int pm_lcd_change_backlight(void)
{
	return work_queue(HPWORK, &pm_lcd_worker, pm_lcd_set_power, (void *)0, 0);
}

#endif /* CONFIG_PM */
