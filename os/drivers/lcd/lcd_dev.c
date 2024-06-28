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
 * drivers/lcd/lcd_dev.c
 *
 *   Copyright (C) 2017=-2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <poll.h>
#include <errno.h>
#include <debug.h>
#include <stdio.h>

#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/lcd/lcd_dev.h>

#define MAX_NO_PLANES 3
/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This structure provides the state of the lcd_dev driver */

struct lcd_s {
	FAR struct lcd_dev_s *dev;
	struct lcd_planeinfo_s planeinfo[MAX_NO_PLANES];
	sem_t sem;
	int16_t crefs;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Character driver methods */

static int lcddev_open(FAR struct file *filep);
static int lcddev_close(FAR struct file *filep);
static int lcddev_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t lcddev_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t lcddev_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_lcddev_fops = {
	lcddev_open,				/* open */
	lcddev_close,				/* close */
	lcddev_read,				/* read */
	lcddev_write,				/* write */
	NULL,						/* seek */
	lcddev_ioctl,				/* ioctl */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/************************************************************************************
 * Name: lcd_semtake
 ************************************************************************************/

void lcd_semtake(sem_t *sem)
{
	/* Take the semaphore (perhaps waiting) */

	while (sem_wait(sem) != 0) {
		/* The only case that an error should occur here is if
		 * the wait was awakened by a signal.
		 */

		ASSERT(*get_errno_ptr() == EINTR);
	}
}

/****************************************************************************
 * Name: lcd_semgive
 ****************************************************************************/

void lcd_semgive(sem_t *sem)
{
	sem_post(sem);
}

/****************************************************************************
 * Name: lcddev_open
 ****************************************************************************/

static int lcddev_open(FAR struct file *filep)
{
	FAR struct lcd_s *priv;
	priv = filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	lcd_semtake(&priv->sem);
	priv->crefs++;
	DEBUGASSERT(priv->crefs > 0);
	lcd_semgive(&priv->sem);

	return OK;
}

/****************************************************************************
 * Name: lcddev_close
 ****************************************************************************/

static int lcddev_close(FAR struct file *filep)
{
	FAR struct lcd_s *priv;
	priv = filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	lcd_semtake(&priv->sem);
	DEBUGASSERT(priv->crefs > 0);
	priv->crefs--;
	lcd_semgive(&priv->sem);

	return OK;
}

static ssize_t lcddev_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	return OK;
}

static ssize_t lcddev_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	return OK;
}

/****************************************************************************
 * Name: lcddev_ioctl
 ****************************************************************************/

static int lcddev_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct lcd_s *priv;
	int ret = OK;
	priv = filep->f_inode->i_private;
	if (!priv) {
		return -EINVAL;
	}

	lcd_semtake(&priv->sem);

	switch (cmd) {
	case LCDDEVIO_GETRUN: {
		FAR struct lcddev_run_s *lcd_run = (FAR struct lcddev_run_s *)arg;
		if (priv->planeinfo[lcd_run->planeno].getrun) {
			ret = priv->planeinfo[lcd_run->planeno].getrun(priv->dev, lcd_run->row, lcd_run->col, lcd_run->data, lcd_run->npixels);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_PUTRUN: {
		FAR const struct lcddev_run_s *lcd_run = (FAR const struct lcddev_run_s *)arg;
		if (priv->planeinfo[lcd_run->planeno].putrun) {
			ret = priv->planeinfo[lcd_run->planeno].putrun(priv->dev, lcd_run->row, lcd_run->col, lcd_run->data, lcd_run->npixels);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_GETAREA: {
		FAR struct lcddev_area_s *lcd_area = (FAR struct lcddev_area_s *)arg;
		size_t cols = lcd_area->col_end - lcd_area->col_start + 1;
		size_t pixel_size = priv->planeinfo[lcd_area->planeno].bpp > 1 ? priv->planeinfo[lcd_area->planeno].bpp >> 3 : 1;
		size_t row_size = lcd_area->stride > 0 ? lcd_area->stride : cols * pixel_size;

		if (priv->planeinfo[lcd_area->planeno].getarea) {
			ret = priv->planeinfo[lcd_area->planeno].getarea(priv->dev, lcd_area->row_start, lcd_area->row_end, lcd_area->col_start, lcd_area->col_end, lcd_area->data, row_size);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_PUTAREA: {
		FAR const struct lcddev_area_s *lcd_area = (FAR const struct lcddev_area_s *)arg;

		size_t cols = lcd_area->col_end - lcd_area->col_start + 1;
		size_t pixel_size = priv->planeinfo[lcd_area->planeno].bpp > 1 ? priv->planeinfo[lcd_area->planeno].bpp >> 3 : 1;
		size_t row_size = lcd_area->stride > 0 ? lcd_area->stride : cols * pixel_size;

		if (priv->planeinfo[lcd_area->planeno].putarea) {
			ret = priv->planeinfo[lcd_area->planeno].putarea(priv->dev, lcd_area->row_start, lcd_area->row_end, lcd_area->col_start, lcd_area->col_end, lcd_area->data, row_size);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_GETPOWER: {
		if (priv->dev->getpower) {
			*((FAR int *)arg) = priv->dev->getpower(priv->dev);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_SETPOWER: {
		if (priv->dev->setpower) {
			ret = priv->dev->setpower(priv->dev, (int)arg);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_GETCONTRAST: {
		if (priv->dev->getcontrast) {
			*((FAR int *)arg) = priv->dev->getcontrast(priv->dev);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_SETCONTRAST: {
		if (priv->dev->setcontrast) {
			ret = priv->dev->setcontrast(priv->dev, (unsigned int)arg);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_GETPLANEINFO: {
		*((FAR struct lcd_planeinfo_s *)arg) = priv->planeinfo[0];
	}
	break;
	case LCDDEVIO_GETVIDEOINFO: {
		if (priv->dev->getvideoinfo) {
			ret = priv->dev->getvideoinfo(priv->dev, (FAR struct fb_videoinfo_s *)arg);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_INIT: {
		if (priv->dev->init) {
			ret = priv->dev->init(priv->dev);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_SETORIENTATION: {
		if (priv->dev->setorientation) {
			ret = priv->dev->setorientation(priv->dev, (unsigned int)arg);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	default: {
		lcddbg("ERROR: Unsupported IOCTL command: %d\n", cmd);
		ret = -ENOSYS;
	}
	break;
	}
	lcd_semgive(&priv->sem);
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lcddev_register
 *
 * Description:
 *   Register the LCD character driver as /dev/lcdN.
 *
 * Input Parameters:
 *   devno - The LCD device number.
 *
 * Returned Value:
 *   Zero (OK) is returned on success.  Otherwise a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/

int lcddev_register(struct lcd_dev_s *dev)
{
	FAR struct lcd_s *priv;
	char devname[16] = { 0, };

	if (!dev) {
		return -EINVAL;
	}

	/* Allocate a new lcd_dev driver instance */
	priv = (struct lcd_s *)kmm_zalloc(sizeof(struct lcd_s));

	if (!priv) {
		return -ENOMEM;
	}

	priv->dev = dev;

	sem_init(&priv->sem, 0, 1);
	if (priv->dev->getplaneinfo) {
		priv->dev->getplaneinfo(priv->dev, 0, &priv->planeinfo);	//plane no is taken 0 here
		snprintf(devname, 16, "/dev/lcd0");
		return register_driver(devname, &g_lcddev_fops, 0666, priv);
	}
	else
	{
		sem_destroy(&priv->sem);
		kmm_free(priv);
		return -ENOSYS;
	}
}
