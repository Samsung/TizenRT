/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/fs/fs.h>
#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/arch.h>
#include <tinyara/ringbuf.h>

#include <arch/irq.h>

/****************************************************************************
 * Private Types
 ****************************************************************************/

#define TTRACE_START           's'
#define TTRACE_OVERWRITE       'o'
#define TTRACE_FINISH          'f'
#define TTRACE_INFO            'i'
#define TTRACE_SELECTED_TAG    't'
#define TTRACE_FUNC_TAG        'g'
#define TTRACE_SET_BUFSIZE     'z'
#define TTRACE_USED_BUFSIZE    'u'
#define TTRACE_BUFFER          'b'

#define TTRACE_STATE_IDLE       0
#define TTRACE_STATE_RUNNING    1

#define TTRACE_OVERFLOW        -2
#define TTRACE_INVALID         -1
#define TTRACE_VALID            0

#define NO_HOLDER               ((pid_t)-1)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct ttrace_dev_s {
	uint16_t ttrace_head;      /* The head index (where data is added) */
	size_t ttrace_bufsize;     /* Size of the trace buffer */
	FAR char *ttrace_packets;  /* Trace packets buffer */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* Character driver methods */
static ssize_t ttrace_read(FAR struct file *, FAR char *, size_t);
static ssize_t ttrace_write(FAR struct file *, FAR const char *, size_t);
static int ttrace_ioctl(FAR struct file *, int, unsigned long);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_ttracefops = {
	0,            /* open */
	0,            /* close */
	ttrace_read,  /* read */
	ttrace_write, /* write */
	0,            /* seek */
	ttrace_ioctl  /* ioctl */
};

/* This is the pre-allocated buffer used for the T-trace */
static struct ringbuf g_ringbuf = {
	{0,},
	CONFIG_TTRACE_BUFSIZE,
	0,
	0,
	0
};

static uint32_t g_state = TTRACE_STATE_IDLE;
static uint32_t g_selected_tag = 0;

/* This is the device structure for the T-trace function. It
 * must be statically initialized because the T-trace ttrace_putc function
 * could be called before the driver initialization logic executes.
 */

static struct ttrace_dev_s g_sysdev = {
	0,                        /* ttrace_head */
	CONFIG_TTRACE_BUFSIZE,    /* ttrace_bufsize */
	g_ringbuf.buffer          /* ttrace_packets_buffer */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ttrace_read
 ****************************************************************************/

static ssize_t ttrace_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	struct inode *inode = filep->f_inode;
	struct ttrace_dev_s *priv = inode->i_private;

	if (TTRACE_STATE_IDLE != g_state) {
		return TTRACE_INVALID;
	}

	DEBUGASSERT(priv);
	sched_lock();

	ttdbg("buffer: %p, ringbuf: %p\r\n", buffer, g_ringbuf.buffer);
	ttdbg("ringbuf_index: %d\r\n", priv->ttrace_head);
	ttdbg("ringbuf_is_overwritten: %d\r\n", g_ringbuf.is_overwritten);
	ttdbg("ringbuf_is_overwritable: %d\r\n", g_ringbuf.is_overwritable);
	ringbuf_read(buffer, len, &g_ringbuf);
	priv->ttrace_head = g_ringbuf.index;

	sched_unlock();
	return (ssize_t)len;
}

/****************************************************************************
 * Name: ttrace_write
 ****************************************************************************/

static ssize_t ttrace_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	struct inode *inode = filep->f_inode;
	struct ttrace_dev_s *priv = inode->i_private;

	if (TTRACE_STATE_RUNNING != g_state) {
		return TTRACE_INVALID;
	}

	DEBUGASSERT(priv);
	sched_lock();

	ringbuf_write(buffer, len, &g_ringbuf);
	priv->ttrace_head = g_ringbuf.index;

	sched_unlock();
	return (ssize_t)len;
}

/****************************************************************************
 * Name: ttrace_ioctl
 ****************************************************************************/

static int ttrace_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	struct ttrace_dev_s *priv = inode->i_private;
	int ret = TTRACE_VALID;

	DEBUGASSERT(priv);
	sched_lock();

	switch (cmd) {
	case TTRACE_START:
		g_state = TTRACE_STATE_RUNNING;
		priv->ttrace_head = 0;
		break;
	case TTRACE_OVERWRITE:
		g_ringbuf.is_overwritable = arg;
		break;
	case TTRACE_FINISH:
		g_selected_tag = 0;
		g_state = TTRACE_STATE_IDLE;
		break;
	case TTRACE_INFO:
		ttdbg("Available tags: apps libs lock ipc task\r\n");
		ttdbg("State: %d\r\n", g_state);
		ttdbg("Selected tags: %d\r\n", g_selected_tag);
		ttdbg("Buffer index: %d\r\n", g_ringbuf.index);
		ttdbg("Real Buffer size: %d\r\n", g_ringbuf.bufsize);
		ttdbg("Given buffer size: %d\r\n", CONFIG_TTRACE_BUFSIZE);
		ttdbg("Buffer is_overwritten: %d\r\n", g_ringbuf.is_overwritten);
		ttdbg("Buffer is_overwritable: %d\r\n", g_ringbuf.is_overwritable);
		break;
	case TTRACE_SELECTED_TAG:
		g_selected_tag |= arg;
		break;
	case TTRACE_FUNC_TAG:
		ret = g_selected_tag;
		break;
	case TTRACE_SET_BUFSIZE:
		g_ringbuf.bufsize = CONFIG_TTRACE_BUFSIZE - (CONFIG_TTRACE_BUFSIZE % arg);
		break;
	case TTRACE_USED_BUFSIZE:
		if (g_ringbuf.is_overwritten == 0) {
			ret = priv->ttrace_head;
		} else {
			ret = CONFIG_TTRACE_BUFSIZE;
		}
		ttdbg("used bufsize: %d\r\n", ret);
		break;
	case TTRACE_BUFFER:
		ttdbg("Resize of trace buffer is not supported yet.\r\n");
		ttdbg("Trace buffer size should be defined by menuconfig.\r\n");
		break;
	default:
		ttdbg("Invalid commands, cmd: %c, arg: %d\r\n", cmd, arg);
		break;
	}

	sched_unlock();
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ttrace_init
 *
 * Description:
 *   Use a pre-allocated T-trace device and register it at the path
 *   specified by CONFIG_TTRACE_DEVPATH
 *
 ****************************************************************************/

int ttrace_init(void)
{
	/* Register the syslog character driver */
	return register_driver(CONFIG_TTRACE_DEVPATH, &g_ttracefops, 0666, &g_sysdev);
}
