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
 * drivers/input/touchscreen.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <poll.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <fcntl.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/input/touchscreen.h>

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

#if !defined(CONFIG_TOUCHL_NPOLLWAITERS)
#define CONFIG_TOUCH_NPOLLWAITERS 2
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int touch_open(FAR struct file *filep);
static int touch_close(FAR struct file *filep);
static ssize_t touch_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t touch_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static int touch_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static int touch_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_touchdev_fileops = {
	touch_open,     /* open */
	touch_close,    /* close */
	touch_read,     /* read */
	touch_write,    /* write */
	NULL,           /* seek */
	touch_ioctl,    /* ioctl */
#if !defined(CONFIG_DISABLE_POLL)
	touch_poll,     /* poll */
#endif
	NULL            /* unlink */
};

/************************************************************************************
 * Name: touch_semtake
 ************************************************************************************/

static inline int touch_semtake(FAR sem_t *sem, bool errout)
{
	/* Take a count from the semaphore, possibly waiting */
	while (sem_wait(sem) != OK) {
		/* EINTR is the only error that we expect */
		ASSERT(get_errno() == EINTR);
		if (errout) {
			return -EINTR;
		}
	}
	return OK;
}

/****************************************************************************
 * Name: touch_semgive
 ****************************************************************************/

static inline void touch_semgive(sem_t *sem)
{
	sem_post(sem);
}

/****************************************************************************
 * Name: touch_open
 ****************************************************************************/

static int touch_open(FAR struct file *filep)
{
	struct touchscreen_s *priv = (struct touchscreen_s *)filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	touch_semtake(&priv->sem, false);
	if (priv->crefs == 0) {

		priv->tp_buf.head = 0;
		priv->tp_buf.tail = 0;

		priv->tp_buf.buffer = (struct touch_sample_s *)kmm_zalloc(sizeof(struct touch_sample_s) * CONFIG_TOUCH_BUFSIZE);
		if (!priv->tp_buf.buffer) {
			touch_semgive(&priv->sem);
			return -ENOMEM;
		}
		priv->tp_buf.size = CONFIG_TOUCH_BUFSIZE;

		if (priv->ops && priv->ops->touch_enable) {
			priv->ops->touch_enable(priv);
		} else {
			kmm_free(priv->tp_buf.buffer);
			touch_semgive(&priv->sem);
			return -EINVAL;
		}
	}
	priv->crefs++;
	DEBUGASSERT(priv->crefs > 0);
	touch_semgive(&priv->sem);
	return OK;
}

/****************************************************************************
 * Name: touch_close
 ****************************************************************************/

static int touch_close(FAR struct file *filep)
{
	struct touchscreen_s *priv = (struct touchscreen_s *)filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	touch_semtake(&priv->sem, false);
	DEBUGASSERT(priv->crefs > 0);
	priv->crefs--;
	if (priv->crefs == 0) {
		priv->ops->touch_disable(priv);
#if !defined(CONFIG_DISABLE_POLL)
		/* Check if this file is registered in a list of waiters for polling.
		* For example, when task A is blocked by calling poll and task B try to terminate task A,
		* a pollfd of A remains in this list. If it is, it should be cleared.
		*/
		(void)touch_semtake(&priv->pollsem, false);
		for (int i = 0; i < CONFIG_TOUCH_NPOLLWAITERS; i++) {
			struct pollfd *fds = priv->fds[i];
			if (fds && (FAR struct file *)fds->filep == filep) {
				priv->fds[i] = NULL;
			}
		}
		touch_semgive(&priv->pollsem);
#endif
		sem_wait(&priv->tp_buf.sem);
		kmm_free(priv->tp_buf.buffer);
		priv->tp_buf.buffer = NULL;
		priv->tp_buf.size = 0;
		sem_post(&priv->tp_buf.sem);
	}
	touch_semgive(&priv->sem);
	return OK;
}

/****************************************************************************
 * Name: touch_read
 ****************************************************************************/

static ssize_t touch_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	FAR struct touchscreen_s *dev;
	struct touch_sample_buffer_s *tp_buf;
	ssize_t recvd = 0;
	int16_t tail;
	int ret;

	dev = filep->f_inode->i_private;
	if (!dev || buflen < sizeof(struct touch_sample_s)) {
		return -EINVAL;
	}
	/* Wait for semaphore to prevent concurrent reads */
	touch_semtake(&dev->sem, false);

	DEBUGASSERT(dev->tp_buf.buffer);

	tp_buf = &dev->tp_buf;

	/* Wait for semaphore to prevent concurrent reads */
	touch_semtake(&tp_buf->sem, false);
	while (recvd < buflen) {
		tail = tp_buf->tail;
		if (tp_buf->head != tail) {
			memcpy(buffer, &tp_buf->buffer[tail],  sizeof(struct touch_sample_s));

			if (++tail >= tp_buf->size) {
				tail = 0;
			}
			tp_buf->tail = tail;
			recvd += sizeof(struct touch_sample_s);
			buffer += sizeof(struct touch_sample_s);
		} else if (recvd > 0) {
			/* Yes.. break out of the loop and return the number of bytes
			 * received up to the wait condition.
			 */

			break;
		} else if ((filep->f_oflags & O_NONBLOCK) != 0) {
			/* Break out of the loop returning -EAGAIN */

			recvd = -EAGAIN;
			break;
		} else {
			if (tp_buf->head == tp_buf->tail) {
				touch_semgive(&tp_buf->sem);
				ret = touch_semtake(&dev->waitsem, true);
				touch_semtake(&tp_buf->sem, false);
				if (ret < 0) {
					recvd = -EINTR;
					break;
				}
			}
		}
	}

	touch_semgive(&tp_buf->sem);
	touch_semgive(&dev->sem);

	return recvd;
}

/****************************************************************************
 * Name: touch_write
 ****************************************************************************/

static ssize_t touch_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	//TODO
	return OK;
}

/****************************************************************************
 * Name: touch_ioctl
 *
 * Description:
 *   This IOCTL is used to configure the touch screen device.
 *
 ****************************************************************************/

static int touch_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct touchscreen_s *priv;
	struct touchscreen_cmd_s *args;
	int ret = OK;

	priv = (struct touchscreen_s *)filep->f_inode->i_private;
	if (!priv) {
		return -EINVAL;
	}

	touch_semtake(&priv->sem, false);
	switch (cmd) {		
		case TSIOC_CMD:
			args = (struct touchscreen_cmd_s *)arg;
			if (priv->ops && priv->ops->cmd) {
				ret = priv->ops->cmd(priv, args->argc, args->argv);
			} else {
				ret = -EINVAL;
			}
		break;
		default: {
			touchdbg("ERROR: ioctl not found, cmd: %d\n", cmd);
		}
		break;
	}
	touch_semgive(&priv->sem);
	return ret;
}

#if !defined(CONFIG_DISABLE_POLL)

/****************************************************************************
 * Name: touch_pollnotify
 ****************************************************************************/

void touch_pollnotify(FAR struct touchscreen_s *dev, pollevent_t eventset)
{
	int itr;
	for (itr = 0; itr < CONFIG_TOUCH_NPOLLWAITERS; itr++) {
		struct pollfd *fds = dev->fds[itr];
		if (fds) {
			fds->revents |= (fds->events & eventset);
			if (fds->revents != 0) {
				sem_post(fds->sem);
			}
		}
	}
}

/****************************************************************************
 * Name: touch_poll
 ****************************************************************************/

int touch_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup)
{
	FAR struct touchscreen_s *priv = filep->f_inode->i_private;
	pollevent_t eventset;
	int ret;
	int i;

	if (!priv || !fds) {
		return -ENODEV;
	}

	ret = touch_semtake(&priv->pollsem, true);
	if (ret < 0) {
		/* A signal received while waiting for access to the poll data
		 * will abort the operation.
		 */
		return ret;
	}

	if (setup) {
		/* This is a request to set up the poll. Find an available
		 * slot for the poll structure reference
		 */
		for (i = 0; i < CONFIG_TOUCH_NPOLLWAITERS; i++) {
			/* Find an available slot */
			if (!priv->fds[i]) {
				/* Bind the poll structure and this slot */
				priv->fds[i] = fds;
				fds->priv = &priv->fds[i];
				fds->filep = (void *)filep;
				break;
			}
		}

		if (i >= CONFIG_TOUCH_NPOLLWAITERS) {
			fds->priv = NULL;
			fds->filep = NULL;
			ret = -EBUSY;
			goto errout;
		}

		eventset = 0;
		touch_semtake(&priv->tp_buf.sem, false);
		if (priv->tp_buf.head != priv->tp_buf.tail) {
			eventset |= (fds->events & POLLIN);
		}
		touch_semgive(&priv->tp_buf.sem);
		if (eventset) {
			touch_pollnotify(priv, POLLIN);
		}

	} else if (fds->priv) {
		/* This is a request to tear down the poll. */
		struct pollfd **slot = (struct pollfd **)fds->priv;
		/* Remove all memory of the poll setup */
		*slot = NULL;
		fds->priv = NULL;
		fds->filep = NULL;
	}
errout:
	touch_semgive(&priv->pollsem);
	return ret;
}
#endif // CONFIG_DISABLE_POLL

/****************************************************************************
 * Name: touch_report
 ****************************************************************************/

void touch_report(struct touchscreen_s *dev, struct touch_sample_s *data)
{
	struct touch_sample_buffer_s *tp_buf;
	int nexthead;
	int semcount;
	int ret;

	DEBUGASSERT(dev);

	tp_buf = &dev->tp_buf;

	if (!tp_buf->buffer) {
		/* The touch reading app is not ready, Skip buffer touch data. */
		return;
	}

	touch_semtake(&tp_buf->sem, false);

	nexthead = tp_buf->head + 1;
	if (nexthead >= tp_buf->size) {
		nexthead = 0;
	}

	if (nexthead == tp_buf->tail) {
		touchdbg("buffer is full\n");
		if (++tp_buf->tail >= tp_buf->size) {
			tp_buf->tail = 0;
		}
	}

	memcpy(&tp_buf->buffer[tp_buf->head], data, sizeof(struct touch_sample_s));
	tp_buf->head = nexthead;

	touch_semgive(&tp_buf->sem);

	ret = sem_getvalue(&dev->waitsem, &semcount);
	if (ret == OK && semcount < 1) {
		sem_post(&dev->waitsem);
	}
#if !defined(CONFIG_DISABLE_POLL)
	touch_pollnotify(dev, POLLIN);
#endif

}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: touch_register
 *
 * Description:
 *   Register the TOUCH character driver as /dev/touchN.
 *
 * Input Parameters:
 *   dev - The TOUCH driver address.
 *
 * Returned Value:
 *   Zero (OK) is returned on success.  Otherwise a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/

int touch_register(const char *path, struct touchscreen_s *dev)
{
	sem_init(&dev->sem, 0, 1);
	sem_init(&dev->pollsem, 0, 1);
	sem_init(&dev->waitsem, 0, 0);
	sem_init(&dev->tp_buf.sem, 0, 1);
	dev->crefs = 0;
#if !defined(CONFIG_DISABLE_POLL)
	(void)touch_semtake(&dev->pollsem, false);
	for (int i = 0; i < CONFIG_TOUCH_NPOLLWAITERS; i++) {
			dev->fds[i] = NULL;
	}
	touch_semgive(&dev->pollsem);
#endif
	int ret = register_driver(path, &g_touchdev_fileops, 0666, dev);
	if (ret < 0) {
		kmm_free(dev);
		sem_destroy(&dev->sem);
		sem_destroy(&dev->pollsem);
		touchdbg("Touch Driver registration failed\n");
		return ret;
	}
	touchvdbg("Touch Driver registered Successfully\n");
	return OK;
}
