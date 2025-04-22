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
#include <tinyara/mmwave/mmwave_dev.h>
#include <tinyara/silent_reboot.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int mmwave_open(FAR struct file *filep);
static int mmwave_close(FAR struct file *filep);
static ssize_t mmwave_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t mmwave_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static int mmwave_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static int mmwave_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_mmwavedev_fileops = {
	mmwave_open,  /* open */
	mmwave_close, /* close */
	mmwave_read,  /* read */
	mmwave_write, /* write */
	NULL,		  /* seek */
	mmwave_ioctl, /* ioctl */
	NULL,
	NULL /* unlink */
};

/************************************************************************************
 * Name: mmwave_semtake
 ************************************************************************************/

static inline int mmwave_semtake(FAR sem_t *sem, bool errout)
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
 * Name: mmwave_semgive
 ****************************************************************************/

static inline void mmwave_semgive(sem_t *sem)
{
	sem_post(sem);
}

/****************************************************************************
 * Name: mmwave_open
 ****************************************************************************/

static int mmwave_open(FAR struct file *filep)
{
	struct mmwave_dev_s *priv = (struct mmwave_dev_s *)filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	mmwave_semtake(&priv->sem, false);

	priv->crefs++;
	DEBUGASSERT(priv->crefs > 0);
	mmwave_semgive(&priv->sem);
	return OK;
}

/****************************************************************************
 * Name: mmwave_close
 ****************************************************************************/

static int mmwave_close(FAR struct file *filep)
{
	struct mmwave_dev_s *priv = (struct mmwave_dev_s *)filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	mmwave_semtake(&priv->sem, false);
	DEBUGASSERT(priv->crefs > 0);
	priv->crefs--;
	mmwave_semgive(&priv->sem);
	return OK;
}

/****************************************************************************
 * Name: mmwave_read
 ****************************************************************************/

static ssize_t mmwave_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	struct mmwave_dev_s *priv = (struct mmwave_dev_s *)filep->f_inode->i_private;
	int ret;
	priv->ops->mmwave_get_cubes(priv, (uint8_t *)buffer, buflen);
	return OK;
}

/****************************************************************************
 * Name: mmwave_write
 ****************************************************************************/

static ssize_t mmwave_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	// TODO
	return OK;
}

/****************************************************************************
 * Name: mmwave_ioctl
 *
 * Description:
 *   This IOCTL is used to configure the mmwave screen device.
 *
 ****************************************************************************/

static int mmwave_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct mmwave_dev_s *priv;

	int ret = OK;

	priv = (struct mmwave_dev_s *)filep->f_inode->i_private;
	if (!priv) {
		return -EINVAL;
	}

	mmwave_semtake(&priv->sem, false);

	mmwave_semgive(&priv->sem);
	return ret;
}

/****************************************************************************
 * Name: mmwave_poll
 ****************************************************************************/

static int mmwave_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup)
{
	FAR struct mmwave_dev_s *priv = filep->f_inode->i_private;
	if (!priv || !fds) {
		return -ENODEV;
	}
	return OK;
}

int mmwave_register(struct mmwave_dev_s *dev)
{
	int ret;
	bool is_silent_mode;
	sem_init(&dev->sem, 0, 1);
	dev->crefs = 0;
	ret = register_driver(MMWAVE_DEV_PATH, &g_mmwavedev_fileops, 0666, dev);
	if (ret < 0) {
		kmm_free(dev);
		sem_destroy(&dev->sem);
		lldbg("MMWave Driver registration failed\n");
		return ret;
	}
	lldbg("MMWAVE Driver registered Successfully\n");
	return OK;
}
