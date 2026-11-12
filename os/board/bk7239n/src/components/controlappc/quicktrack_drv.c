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
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * /dev/quicktrack
 *
 * The QuickTrack agent runs in kernel space (it drives the UART and talks to
 * wpa_supplicant directly), so a user space TASH command cannot call
 * controlappc_start() when CONFIG_APP_BINARY_SEPARATION is enabled. This
 * driver exposes the single "start the agent" operation as an ioctl.
 ****************************************************************************/

#include <tinyara/config.h>

#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

extern void controlappc_start(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

static int quicktrack_open(FAR struct file *filep);
static int quicktrack_close(FAR struct file *filep);
static ssize_t quicktrack_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t quicktrack_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static int quicktrack_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

static const struct file_operations g_quicktrack_fops = {
	quicktrack_open,			/* open */
	quicktrack_close,			/* close */
	quicktrack_read,			/* read */
	quicktrack_write,			/* write */
	NULL,						/* seek */
	quicktrack_ioctl,			/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	NULL,						/* poll */
#endif
	NULL						/* unlink */
};

static int quicktrack_open(FAR struct file *filep)
{
	return OK;
}

static int quicktrack_close(FAR struct file *filep)
{
	return OK;
}

static ssize_t quicktrack_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	return 0;
}

static ssize_t quicktrack_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	return buflen;
}

static int quicktrack_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	switch (cmd) {
	case QTIOC_START:
		controlappc_start(NULL, 0, 0, NULL);
		return OK;

	default:
		return -ENOTTY;
	}
}

int quicktrack_drv_register(void)
{
	int ret = register_driver("/dev/quicktrack", &g_quicktrack_fops, 0666, NULL);

	if (ret < 0) {
		lldbg("Failed to register /dev/quicktrack: %d\n", ret);
	}

	return ret;
}
