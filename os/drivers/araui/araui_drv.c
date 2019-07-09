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
#include <sys/types.h>
#include <arch/board/esp_ili9341.h>
#include <tinyara/araui_drv.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int ui_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t ui_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t ui_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct file_operations ui_fops = {
	0,                          /* open */
	0,                          /* close */
	ui_read,                    /* read */
	ui_write,                   /* write */
	0,                          /* seek */
	ui_ioctl                    /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static ssize_t ui_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;
}

static ssize_t ui_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return 0;
}

/************************************************************************************
 * Name: ui_ioctl
 *
 * Description:  The ioctl method for AraUI Framework.
 *
 ************************************************************************************/
static int ui_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret = ERROR;
	ui_flush_area_t *area;

	/* Handle built-in ioctl commands */

	switch (cmd) {
	case UIIOC_LCDINIT:
		uivdbg("lcd init!\n");
		up_lcdinitialize();
		ret = OK;
		break;

	case UIIOC_LCDFLUSH:
		area = (ui_flush_area_t *)arg;
		esp_lcd_flush(area->x1, area->y1, area->x2, area->y2, area->color);
		ret = OK;
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
 * Name: araui_drv_register
 *
 * Description:
 *   Register AraUI path, ARAUI_DRVPATH
 *
 ****************************************************************************/

void araui_drv_register(void)
{
	(void)register_driver(ARAUI_DRVPATH, &ui_fops, 0666, NULL);
}

