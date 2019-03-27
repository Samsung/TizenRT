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
#include <tinyara/fscmd_drv.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/ramdisk.h>
#include <tinyara/fs/mksmartfs.h>
#include <stdio.h>
#include <errno.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int fcdrv_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t fcdrv_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t fcdrv_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct file_operations fc_fops = {
	0,													/* open */
	0,													/* close */
	fcdrv_read,											/* read */
	fcdrv_write,										/* write */
	0,													/* seek */
	fcdrv_ioctl											/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0													/* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static ssize_t fcdrv_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	printf("%s\n", __FUNCTION__);
	return 0;
}

static ssize_t fcdrv_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	printf("%s\n", __FUNCTION__);
	return 0;
}

/****************************************************************************
 * Name: fcdrv_ioctl
 *
 * Description:  The ioctl method for fscmd.
 *
 ****************************************************************************/
static int fcdrv_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret = -EINVAL;

	switch (cmd) {
	case FCIOC_FOREACH_MOUNTPOINT: {
		struct foreach_mountpoint_s *info = (struct foreach_mountpoint_s *)arg;
		ret = foreach_mountpoint(info->handler, (FAR void *)info->arg);
	}
	break;

	case FCIOC_RAMDISK_REGISTER: {
		struct ramdisk_info_s *info = (struct ramdisk_info_s *)arg;
		ret = ramdisk_register(info->minor, info->buffer, info->nsectors, info->sectsize, info->rdflags);
	}
	break;

	case FCIOC_MAKE_SMARTFS: {
		struct mksmartfs_info_s *info = (struct mksmartfs_info_s *)arg;
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
		ret = mksmartfs(info->pathname, info->nrootdirs, info->force);
#else
		ret = mksmartfs(info->pathname, info->force);
#endif
	}
	break;

	}
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fscmd_drv_register
 *
 * Description:
 *   Register /dev/fscmd
 *
 ****************************************************************************/

void fscmd_drv_register(void)
{
	(void)register_driver(FSCMD_DRVPATH, &fc_fops, 0666, NULL);
}
