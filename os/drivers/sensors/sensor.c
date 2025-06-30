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

#include <poll.h>
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

static void    sensor_pollnotify(FAR struct sensor_upperhalf_s *upper,
                                 pollevent_t eventset);
static int     sensor_open(FAR struct file *filep);
static int     sensor_close(FAR struct file *filep);
static ssize_t sensor_read(FAR struct file *filep, FAR char *buffer,
                           size_t buflen);
static ssize_t sensor_write(FAR struct file *filep, FAR const char *buffer,
                            size_t buflen);
static int     sensor_ioctl(FAR struct file *filep, int cmd,
                            unsigned long arg);
static int     sensor_poll(FAR struct file *filep, FAR struct pollfd *fds,
                           bool setup);
static ssize_t sensor_push_event(FAR void *priv, FAR const void *data,
                                 size_t bytes);

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
  sensor_poll     /* poll  */
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


static void sensor_pollnotify(FAR struct sensor_upperhalf_s *dev,
                              pollevent_t eventset)
{
	int itr;
	for (itr = 0; itr < CONFIG_SENSOR_NPOLLWAITERS; itr++) {
		struct pollfd *fds = dev->fds[itr];
		if (fds) {
			fds->revents |= (fds->events & eventset);
			if (fds->revents != 0) {
				sem_post(fds->sem);
			}
		}
	}
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
                return -EINVAL;
        }

        sensor_semtake(&priv->sem, false);
        DEBUGASSERT(priv->crefs > 0);
	priv->crefs--;
	if (priv->crefs == 0) {
#ifndef CONFIG_DISABLE_POLL
		/* Check if this file is registered in a list of waiters for polling.
		* For example, when task A is blocked by calling poll and task B try to terminate task A,
		* a pollfd of A remains in this list. If it is, it should be cleared.
		*/
		(void)sensor_semtake(&priv->pollsem, false);
		for (int i = 0; i < CONFIG_SENSOR_NPOLLWAITERS; i++) {
			struct pollfd *fds = priv->fds[i];
			if (fds && (FAR struct file *)fds->filep == filep) {
				priv->fds[i] = NULL;
			}
		}
		sensor_semgive(&priv->pollsem);
#endif
	}
	
	sensor_semgive(&priv->sem);
        return OK;

}

static ssize_t sensor_read(FAR struct file *filep, FAR char *buffer,
                           size_t len)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct sensor_upperhalf_s *upper = inode->i_private;
	upper->ops->sensor_read(upper, buffer);
	return OK;
}

static ssize_t sensor_write(FAR struct file *filep, FAR const char *buffer,
                            size_t buflen)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct sensor_upperhalf_s *upper = inode->i_private;
	return OK;
}

static int sensor_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct sensor_upperhalf_s *priv = inode->i_private;
	printf("cmd %d arg %d, aa:%d\n", cmd, arg, SENSOR_SET_SAMPRATE);
	if (!priv || !priv->ops) {
		printf("privs is null\n");
		return -1;
	}
	switch (cmd) {
		case SENSOR_SET_MCLK: {
			priv->ops->sensor_set_mclk(priv, arg);
		}
		break;
		
		case SENSOR_SET_BCLK: {
                        priv->ops->sensor_set_bclk(priv, arg);
                }
                break;

		case SENSOR_SET_SAMPRATE: {
                        priv->ops->sensor_set_samprate(priv, arg);
                }
                break;

		case SENSOR_SET_CHANNEL: {
                        priv->ops->sensor_setchannel_count(priv, arg);
                }
                break;

		case SENSOR_SET_DATABIT: {
                        priv->ops->sensor_setbit_perchannel(priv, arg);
                }
                break;

		case SENSOR_START: {
                        priv->ops->sensor_start(priv);
                }
		break;

		case SENSOR_STOP: {
			priv->ops->sensor_stop(priv);
		}
		break;

		default:
		break;
	}
	return OK;
}

static int sensor_poll(FAR struct file *filep,
                       FAR struct pollfd *fds, bool setup)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct sensor_upperhalf_s *upper = inode->i_private;
	return OK;
}

int sensor_register(const char *path, struct sensor_upperhalf_s *dev)
{
	sem_init(&dev->sem, 0, 1);
	sem_init(&dev->pollsem, 0, 1);

#ifndef CONFIG_DISABLE_POLL
	(void)sensor_semtake(&dev->pollsem, false);
	for (int i = 0; i < CONFIG_SENSOR_NPOLLWAITERS; i++) {
			dev->fds[i] = NULL;
	}
	sensor_semgive(&dev->pollsem);
#endif
	int ret = register_driver(path, &g_sensor_fops, 0666, dev);
	if (ret < 0) {
		kmm_free(dev);
		sem_destroy(&dev->sem);
		sem_destroy(&dev->pollsem);
		lldbg("Sensor Driver registration failed\n");
		return ret;
	}
	lldbg("Sensor Driver registered Successfully\n");
	return OK;
}
