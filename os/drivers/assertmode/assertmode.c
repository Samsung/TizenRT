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

#include <sys/types.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/assertmode.h>

#ifdef CONFIG_ASSERTMODE_DRIVER

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int assertmode_open(FAR struct file *filep);
static int assertmode_close(FAR struct file *filep);
static ssize_t assertmode_read(FAR struct file *filep,
                               FAR char *buffer, size_t buflen);
static ssize_t assertmode_write(FAR struct file *filep,
                                FAR const char *buffer, size_t buflen);
static int assertmode_ioctl(FAR struct file *filep,
                             int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations assertmode_fops = {
	assertmode_open,      /* open */
	assertmode_close,     /* close */
	assertmode_read,      /* read */
	assertmode_write,     /* write */
	NULL,                 /* seek */
	assertmode_ioctl,     /* ioctl */
	NULL                  /* poll */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int assertmode_open(FAR struct file *filep)
{
	return OK;
}

static int assertmode_close(FAR struct file *filep)
{
	return OK;
}

static ssize_t assertmode_read(FAR struct file *filep,
                               FAR char *buffer, size_t buflen)
{
	return 0;
}

static ssize_t assertmode_write(FAR struct file *filep,
                                FAR const char *buffer, size_t buflen)
{
	return buflen;
}

/****************************************************************************
 * Name: assertmode_ioctl
 *
 * Description:
 *   IOCTL interface for the assertmode driver.
 *
 *   ASSERTMODEIOC_GET_MODE - Get current assert action mode
 *     arg: pointer to struct assertmode_info_s
 *
 *   ASSERTMODEIOC_SET_MODE - Set assert action mode
 *     arg: pointer to struct assertmode_info_s with action field set
 *
 ****************************************************************************/

static int assertmode_ioctl(FAR struct file *filep,
                             int cmd, unsigned long arg)
{
	int ret = OK;
	FAR struct assertmode_info_s *info;

	switch (cmd) {
	case ASSERTMODEIOC_GET_MODE:
		info = (FAR struct assertmode_info_s *)arg;
		if (!info) {
			ret = -EINVAL;
		} else {
			info->action = get_assert_action();
		}
		break;

	case ASSERTMODEIOC_SET_MODE:
		info = (FAR struct assertmode_info_s *)arg;
		if (!info) {
			ret = -EINVAL;
		} else {
			ret = set_assert_action(info->action);
		}
		break;

	default:
		ret = -ENOTTY;
		break;
	}

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: assertmode_register
 *
 * Description:
 *   Register the assertmode character driver at /dev/assertmode.
 *
 ****************************************************************************/

void assertmode_register(void)
{
	int ret;

	ret = register_driver(ASSERTMODE_DRVPATH, &assertmode_fops, 0666, NULL);
	if (ret < 0) {
		lldbg("Failed to register assertmode driver: %d\n", ret);
	}
}

#endif /* CONFIG_ASSERTMODE_DRIVER */
