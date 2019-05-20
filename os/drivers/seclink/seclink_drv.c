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

#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <debug.h>

#include <tinyara/fs/fs.h>
#include <tinyara/testcase_drv.h>
#include <tinyara/sched.h>
#include <tinyara/seclink.h>
#include <tinyara/seclink_drv.h>
#include "seclink_drv_req.h"
#include "seclink_drv_utils.h"

#define SL_IS_COMMON_REQ(cmd)  ((cmd & 0xf0) == 0)
#define SL_IS_CRYPTO_REQ(cmd)  ((cmd & 0xf0) & (SECLINKIOC_CRYPTO & 0xf0))
#define SL_IS_AUTH_REQ(cmd)    ((cmd & 0xf0) & (SECLINKIOC_AUTH & 0xf0))
#define SL_IS_SS_REQ(cmd)      ((cmd & 0xf0) & (SECLINKIOC_SS & 0xf0))
#define SL_IS_KEYMGR_REQ(cmd)  ((cmd & 0xf0) & (SECLINKIOC_KEYMGR & 0xf0))

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int seclink_open(FAR struct file *filep);
static int seclink_close(FAR struct file *filep);
static ssize_t seclink_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t seclink_write(FAR struct file *filep, FAR const char *buffer, size_t len);
static int seclink_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_seclink_fops = {
	seclink_open,                                                   /* open */
	seclink_close,                                                   /* close */
	seclink_read,
	seclink_write,
	0,                                                   /* seek */
	seclink_ioctl                                        /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                                                  /* poll */
#endif
};

int seclink_open(FAR struct file *filep)
{
	SLDRV_ENTER;

	FAR struct inode *inode = filep->f_inode;
	FAR struct sec_upperhalf_s *upper = inode->i_private;
	upper->refcnt++;

	return 0;
}

int seclink_close(FAR struct file *filep)
{
	SLDRV_ENTER;

	FAR struct inode *inode = filep->f_inode;
	FAR struct sec_upperhalf_s *upper = inode->i_private;
	upper->refcnt--;

	return 0;
}

ssize_t seclink_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	SLDRV_ENTER;

	return 0;
}

ssize_t seclink_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	SLDRV_ENTER;

	return 0;
}

int seclink_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	SLDRV_ENTER;

	SLDRV_LOG("-->%s (%d)(%x)\n", __FUNCTION__, cmd, arg);

	FAR struct inode *inode = filep->f_inode;
	FAR struct sec_upperhalf_s *upper = inode->i_private;

	/* If there are a couple of tasks that request SE then
	 * calling lower's function should be protected. But
	 * current design consider that only one task that is seclink can
	 * request to HAL
	 */
	int res = 0;
	if (SL_IS_COMMON_REQ(cmd)) {
		res = hd_handle_common_request(cmd, arg, (void *)upper->lower);
	} else if (SL_IS_AUTH_REQ(cmd)) {
		res = hd_handle_auth_request(cmd, arg, (void *)upper->lower);
	} else if (SL_IS_KEYMGR_REQ(cmd)) {
		res = hd_handle_key_request(cmd, arg, (void *)upper->lower);
	} else if (SL_IS_SS_REQ(cmd)) {
		res = hd_handle_ss_request(cmd, arg, (void *)upper->lower);
	} else if (SL_IS_CRYPTO_REQ(cmd)) {
		res = hd_handle_crypto_request(cmd, arg, (void *)upper->lower);
	}

	return res;
}


/****************************************************************************
 * Name: se_register
 *
 * Description:
 *   Register Secure Element device.
 *
 ****************************************************************************/
int se_register(const char *path, struct sec_lowerhalf_s *lower)
{
	vdbg("Registering %s\n", path);

	if (!lower) {
		return -1;
	}

	struct sec_upperhalf_s *upper = (struct sec_upperhalf_s *)malloc(sizeof(struct sec_upperhalf_s));
	if (!upper) {
		return -1;
	}

	/*  initialize upper */
	char *drv_path = (char *)malloc(strlen(path) + 1);
	if (!drv_path) {
		free(upper);
		return -1;
	}
	memcpy(drv_path, path, strlen(path) + 1);
	upper->path = drv_path;
	upper->refcnt = 0;

	upper->lower = lower;
	lower->parent = upper;

	return register_driver(path, &g_seclink_fops, 0666, upper);
}


/****************************************************************************
 * Name: se_unregister
 *
 * Description:
 *   Unregister Secure Element device.
 *
 ****************************************************************************/
int se_unregister(FAR struct sec_lowerhalf_s *lower)
{
	if (!lower) {
		return -1;
	}
	// de-initialize  upper
	struct sec_upperhalf_s *upper = lower->parent;
	if (!upper) {
		dbg("upper is null\n");
		assert(0);
	}

	if (!upper->path) {
		dbg("upper path is null\n");
		assert(0);
	}

	vdbg("unregister driver(%s)\n", upper->path);
	int res = unregister_driver(upper->path);
	if (res != 0) {
		dbg("unregister driver path fail\n");
	}

	free(upper->path);
	free(upper);

	return res;
}
