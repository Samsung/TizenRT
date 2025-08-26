/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * drivers/sensors/sensor.c
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
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <fcntl.h>
#include <tinyara/sensors/sensor.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int sensor_open(FAR struct file *filep);
static int sensor_close(FAR struct file *filep);
static ssize_t sensor_read(FAR struct file *filep, FAR void *buffer, size_t buflen);
static ssize_t sensor_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static int sensor_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t sensor_push_event(FAR void *priv, FAR const void *data, size_t bytes);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_sensor_fops =
{
  sensor_open,    /* open  */
  sensor_close,   /* close */
  sensor_read,    /* read  */
  sensor_write,   /* write */
  NULL,           /* seek  */
  sensor_ioctl,   /* ioctl */
  NULL,           /* mmap */
  NULL,           /* truncate */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/*****************************************************************************
 * Name: sensor_semtake
 ****************************************************************************/

static inline int sensor_semtake(FAR sem_t *sem, bool errout)
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
 * Name: sensor_semgive
 ****************************************************************************/

static inline void sensor_semgive(sem_t *sem)
{
	sem_post(sem);
}

static int sensor_open(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct sensor_upperhalf_s *priv = inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	sensor_semtake(&priv->sem, false);
	priv->crefs++;
	DEBUGASSERT(priv->crefs > 0);
	sensor_semgive(&priv->sem);
	return OK;
}

static int sensor_close(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct sensor_upperhalf_s *priv = inode->i_private;
	if (!priv) {
		sndbg("ERROR: sensor close failed, upper priv uninitialized/deleted\n");
		return -EINVAL;
	}

	sensor_semtake(&priv->sem, false);
	DEBUGASSERT(priv->crefs > 0);
	priv->crefs--;

	sensor_semgive(&priv->sem);
	return OK;
}

static ssize_t sensor_read(FAR struct file *filep, FAR void *buffer, size_t len)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct sensor_upperhalf_s *upper = inode->i_private;
	upper->ops->sensor_read(upper, buffer);
	return OK;
}

static ssize_t sensor_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct sensor_upperhalf_s *upper = inode->i_private;
	return OK;
}

static int sensor_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct sensor_upperhalf_s *priv = inode->i_private;
	int ret = OK;
	if (!priv || !priv->ops) {
		sndbg("ERROR: upper_half priv is NULL/ops not found\n");
		return ERROR;
	}
	switch (cmd) {
		case SENSOR_SET_SAMPRATE: {
			priv->ops->sensor_set_samprate(priv, arg);
			break;
		}
		case SENSOR_SET_DATABIT: {
			priv->ops->sensor_setbit_perchannel(priv, arg);
			break;
		}
		case SENSOR_START: {
			ret = priv->ops->sensor_start(priv);
			break;
		}
		case SENSOR_STOP: {
			priv->ops->sensor_stop(priv);
			break;
		}
		case SENSOR_VERIFY: {
			priv->ops->sensor_verify(priv);
			break;
		}
		case SENSOR_REGISTERMQ: {
			ret = priv->ops->sensor_register_mq(priv, arg);
			break;
		}
		case SENSOR_GET_BUFNUM: {
			priv->ops->sensor_get_bufnum(priv, arg);
			break;
		}
		case SENSOR_GET_BUFSIZE: {
			priv->ops->sensor_get_bufsize(priv, arg);
			break;
		}
		case SENSOR_SENDBUFFER: {
			priv->ops->sensor_send_buffer(priv, arg);
			break;
		}
		case SENSOR_SHOW: {
			priv->ops->sensor_show(arg);
			break;
		}
		default:
			ret = ERROR;
			sndbg("ERROR: IOCTL command not found: %d\n", cmd);
	}
	return ret;
}

int sensor_register(const char *path, struct sensor_upperhalf_s *dev)
{
	sem_init(&dev->sem, 0, 1);

	int ret = register_driver(path, &g_sensor_fops, 0666, dev);
	if (ret != OK) {
		kmm_free(dev);
		sem_destroy(&dev->sem);
		sndbg("ERROR: Sensor Driver registration failed\n");
		return ret;
	}
	snvdbg("Sensor Driver registered Successfully\n");
	return OK;
}
