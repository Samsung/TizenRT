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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <errno.h>
#include <sys/types.h>
#include <tinyara/prodconfig.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int prodconfig_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t prodconfig_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t prodconfig_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct file_operations prodconfig_fops = {
	0,                          /* open */
	0,                          /* close */
	prodconfig_read,               /* read */
	prodconfig_write,              /* write */
	0,                          /* seek */
	prodconfig_ioctl               /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static ssize_t prodconfig_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	char config = 0;
	int ret;
	ret = up_check_prodswd();
	if (ret == OK) {
		config |= SWD_ENABLED;
	}
	ret = up_check_proddownload();
	if (ret == OK) {
		config |= DOWNLOAD_ENABLED;
	}

	*buffer = config;

	return 0;
}

static ssize_t prodconfig_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return 0;
}

/************************************************************************************
 * Name: prodconfig_ioctl
 *
 * Description: The ioctl method for prodconfig.
 *
 ************************************************************************************/
static int prodconfig_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: prodconfig_register
 *
 * Description:
 *   Register prodconfig driver path, PRODCONFIG_DRVPATH
 *
 ****************************************************************************/

void prodconfig_register(void)
{
	(void)register_driver(PRODCONFIG_DRVPATH, &prodconfig_fops, 0666, NULL);
}
