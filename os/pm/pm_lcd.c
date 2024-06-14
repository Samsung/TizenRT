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
#include <tinyara/arch.h>
#include <tinyara/lcd/lcd_dev.h>
#include <tinyara/kthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "pm.h"

#ifdef CONFIG_PM

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int pm_get_lcd_power(void)
{
	int power = 0;
	if(g_pmglobals.state == PM_NORMAL) {
		power = CONFIG_LCD_NORMAL_POWER;
	}
	else if(g_pmglobals.state == PM_IDLE) {
		power = CONFIG_LCD_IDLE_POWER;
	}
	return power;
}

static pthread_addr_t pm_lcd(pthread_addr_t arg)
{
    int fd;
    for (;;) {
		pm_lcd_thread_lock();
        /* Change LCD Power */
		fd = open(CONFIG_LCD_DEVPATH, O_RDWR | O_SYNC, 0666);
		if (fd < 0) {
			pmdbg("Unable to open LCD Driver\n");
			continue;
		}
		if(ioctl(fd, LCDDEVIO_SETPOWER, pm_get_lcd_power()) != OK) {
			pmdbg("Unable to change LCD Power\n");
		}
		close(fd);
    }
	return NULL;
}
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: pm_lcd_thread
 *
 * Description:
 *   This function creates and activates a kernel thread which controls the 
 *   LCD backlight power and returns its system-assigned ID.
 *
 * Return Value:
 *   Returns the non-zero process ID of the kernel thread on success
 *   ERROR on failure. The errno will be set to indicate the nature of the error.
 * 
 ****************************************************************************/
int pm_lcd_thread(void) {
	return kernel_thread("pm_lcd", CONFIG_PM_LCD_THREAD_PRIORITY, CONFIG_PM_LCD_THREAD_STACK_SIZE, (main_t)pm_lcd, NULL);
}

#endif