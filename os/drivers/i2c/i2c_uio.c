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
 * drivers/i2c/i2c_uio.c
 *
 *   Copyright (C) 2016 Gregory Nutt. All rights reserved.
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
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/i2c.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#include <semaphore.h>

/******************************************************************************
 * Private Variables
 *****************************************************************************/
static int i2c_uio_open(FAR struct file *filep);
static int i2c_uio_close(FAR struct file *filep);
static int i2c_uioctrl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t i2c_uioread(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t i2c_uiowrite(FAR struct file *filep, FAR const char *buffer, size_t buflen);

static const struct file_operations g_i2cops = {
	i2c_uio_open,				/* open */
	i2c_uio_close,				/* close */
	i2c_uioread,				/* read */
	i2c_uiowrite,				/* write */
	NULL,						/* seek */
	i2c_uioctrl,				/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	NULL						/* poll */
#endif
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#if 1

/****************************************************************************
 * Name: i2cdrvr_open
 ****************************************************************************/

static int i2c_uio_open(FAR struct file *filep)
{
	FAR struct inode *inode;
	FAR struct i2c_dev_s *dev;
//  int ret;

	/* Get our private data structure */

	DEBUGASSERT(filep != NULL && filep->f_inode != NULL);
	inode = filep->f_inode;

	dev = (FAR struct i2c_dev_s *)inode->i_private;
	DEBUGASSERT(dev);
#if 0
	/* Get exclusive access to the I2C driver state structure */

	ret = sem_wait(&dev->exclsem);
	if (ret < 0) {
		int errcode = errno;
		DEBUGASSERT(errcode < 0);
		return -errcode;
	}

	/* Increment the count of open references on the RTC driver */

	dev->crefs++;
	DEBUGASSERT(dev->crefs > 0);

	sem_post(&dev->exclsem);
#endif
	return OK;
}

/****************************************************************************
 * Name: i2cdrvr_close
 ****************************************************************************/

static int i2c_uio_close(FAR struct file *filep)
{
	FAR struct inode *inode;
	FAR struct i2c_dev_s *dev;
//  int ret;

	/* Get our private data structure */

	DEBUGASSERT(filep != NULL && filep->f_inode != NULL);
	inode = filep->f_inode;

	dev = (FAR struct i2c_dev_s *)inode->i_private;
	DEBUGASSERT(dev);
#if 0
	/* Get exclusive access to the I2C driver state structure */

	ret = sem_wait(&dev->exclsem);
	if (ret < 0) {
		int errcode = errno;
		DEBUGASSERT(errcode < 0);
		return -errcode;
	}

	/* Decrement the count of open references on the RTC driver */

	DEBUGASSERT(dev->crefs > 0);
	dev->crefs--;

	/* If the count has decremented to zero and the driver has been unlinked,
	 * then commit Hara-Kiri now.
	 */

	if (dev->crefs <= 0 && dev->unlinked) {
		sem_destroy(&dev->exclsem);
		kmm_free(dev);
		return OK;
	}

	sem_post(&dev->exclsem);
#endif
	return OK;
}

#endif

/****************************************************************************
 * Name: i2c_uioread
 *
 * Description:
 *   Read I2C Data. This function is automatically called when you're doing
 *   read() API on userspace.
 *
 ****************************************************************************/

static ssize_t i2c_uioread(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct i2c_dev_s *dev = inode->i_private;

	return I2C_READ(dev, (uint8_t *)buffer, buflen);
}

/****************************************************************************
 * Name: i2c_uiowrite
 *
 * Description:
 *   Write I2C Data. This function is automatically called when you're doing
 *   write() API on userspace.
 *
 ****************************************************************************/

static ssize_t i2c_uiowrite(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct i2c_dev_s *dev = inode->i_private;

	return I2C_WRITE(dev, (uint8_t *)buffer, buflen);
}

/****************************************************************************
 * Name: i2c_uioctrl
 *
 * Description:
 *   Control I2C Data. This function is automatically called when you're doing
 *   ctrl() API on userspace.
 *
 ****************************************************************************/

static int i2c_uioctrl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int32_t ret;
	int addr;
	int32_t freq;
	FAR struct inode *inode = filep->f_inode;
	FAR struct i2c_dev_s *dev = inode->i_private;
	FAR struct i2c_rdwr_ioctl_data_s *rdwr;

	switch (cmd) {
	case I2C_SLAVE:
	case I2C_SLAVE_FORCE:
		addr = *((int *)arg);
		ret = I2C_SETADDRESS(dev, addr, -1);
		break;
	case I2C_TENBIT:
		/* 0 for 7 bit addrs, != 0 for 10 bit */
		if (arg == 0) {
			ret = I2C_SETADDRESS(dev, -1, 7);
		} else {
			ret = I2C_SETADDRESS(dev, -1, 10);
		}
		break;
	case I2C_RDWR:
		rdwr = (struct i2c_rdwr_ioctl_data_s *)(arg);
		ret = I2C_TRANSFER(dev, rdwr->msgs, rdwr->nmsgs);
		break;

	case I2C_FREQUENCY:
		freq = *((uint32_t *)arg);
		ret = I2C_SETFREQUENCY(dev, freq);
		break;

	default:
		dbg("Unknown cmd(%x)\n", cmd);
		ret = -EINVAL;
		break;
	}

	return ret;
}

/****************************************************************************
 * Name: i2c_uioregister
 *
 * Description:
 *   register device.
 *
 ****************************************************************************/
int i2c_uioregister(FAR const char *path, FAR struct i2c_dev_s *dev)
{
	dbg("Registering %s\n", path);
	return register_driver(path, &g_i2cops, 0666, dev);
}
