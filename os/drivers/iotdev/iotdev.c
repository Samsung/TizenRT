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
#include <poll.h>
#include <semaphore.h>
#include <errno.h>
#include <debug.h>
#include <unistd.h>
#include <tinyara/fs/fs.h>
#include <tinyara/sched.h>
#include <tinyara/iotdev.h>
/* #include "clock/clock.h" */
/* #include "signal/signal.h" */
/* #include "timer/timer.h" */

#define IOTDEV_NPOLLWAITERS 3

#define IOTDEV_LOCK(priv)									\
	do {													\
		int iret = id_takesem(&priv->gu_exclsem);			\
		if (iret < 0) {										\
			lldbg("ERROR: id_takesem failed: %d\n", ret);	\
		}													\
	} while (0)

#define IOTDEV_UNLOCK(priv)						\
	do {										\
		id_givesem(&priv->gu_exclsem);			\
	} while (0)

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int id_drv_open(FAR struct file *filep);
static int id_drv_close(FAR struct file *filep);
static int id_drv_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup);
static int id_drv_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t id_drv_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t id_drv_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations id_drv_fops = {
	id_drv_open,                                                   /* open */
	id_drv_close,                                                   /* close */
	id_drv_read,                                /* read */
	id_drv_write,                               /* write */
	0,                                                   /* seek */
	id_drv_ioctl                                /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, id_drv_poll                                                  /* poll */
#endif
};

struct id_open_s {

	/* The following will be true if we are closing */
	volatile bool io_closing;

#ifndef CONFIG_DISABLE_POLL
	/*
	 * The following is a list if poll structures of threads waiting
	 * for driver events
	 */
	FAR struct pollfd *io_fds[IOTDEV_NPOLLWAITERS];
#endif
};

#define IOTDEV_QUEUE_SIZE 50

struct id_queue {
	int front;
	int rear;
	sem_t queue_lock;
	id_evt_type queue[IOTDEV_QUEUE_SIZE];
};


struct id_upperhalf_s {
	sem_t gu_exclsem;	/* Supports exclusive access to the device */
	bool gu_sample;		/* Last sampled GPIO states */
	struct id_queue evt_queue;
	struct id_open_s *gu_open;
};

static struct id_open_s g_id_open;
static struct id_upperhalf_s g_id_udev;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int _id_set_evt(struct id_queue *queue, id_evt_type evt)
{
	if (queue->rear == IOTDEV_QUEUE_SIZE) {
		queue->rear = 0;
	}
	if (queue->rear == queue->front) {
		// overflow!!!
		//printf("overflow\n");
		assert(0);
		return -1;
	}
	queue->queue[queue->rear++] = evt;

	return 0;
}

static int _id_get_evt(struct id_queue *queue, id_evt_type *evt)
{
	if (queue->front + 1 == queue->rear) {
		// invalid request generate crash to debug it
		// empty!!!
		//printf("underflow\n");
		assert(0);
		return -1;
	}
	if (queue->front + 1 == IOTDEV_QUEUE_SIZE) {
		queue->front = 0;
		*evt = queue->queue[0];
	} else {
		*evt = queue->queue[++queue->front];
	}

	return 0;
}

static inline int id_takesem(FAR sem_t *sem)
{
	/* Take a count from the semaphore, possibly waiting */
	if (sem_wait(sem) < 0) {
		/* EINTR is the only error that we expect */
		int errcode = get_errno();
		DEBUGASSERT(errcode == EINTR);
		return errcode;
	}

	return OK;
}

static inline void id_givesem(sem_t *sem)
{
	sem_post(sem);
}

int id_drv_open(FAR struct file *filep)
{
	// ToDo
	return 0;
}

int id_drv_close(FAR struct file *filep)
{
	// ToDo
	return 0;
}

/************************************************************************************
 * Name: id_drv_ioctl
 *
 * Description:  The standard ioctl method.
 *
 ************************************************************************************/

static int id_drv_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup)
{
	FAR struct inode *inode;
	FAR struct id_upperhalf_s *priv;
	FAR struct id_open_s *opriv;
	int ret;
	int i;

	//DEBUGASSERT(filep && filep->f_priv && filep->f_inode);
	DEBUGASSERT(filep && filep->f_inode);
	//opriv = filep->f_priv;
	inode = filep->f_inode;
	DEBUGASSERT(inode->i_private);
	priv  = (FAR struct id_upperhalf_s *)inode->i_private;
	opriv = priv->gu_open;

	/* Get exclusive access to the driver structure */
	IOTDEV_LOCK(priv);

	/* Are we setting up the poll? Or tearing it down? */
	if (setup) {
		/*
		 * This is a request to set up the poll. Find an available
		 * slot for the poll structure reference
		 */
		for (i = 0; i < IOTDEV_NPOLLWAITERS; i++) {
			/* Find an available slot */
			if (!opriv->io_fds[i]) {
				/* Bind the poll structure and this slot */
				opriv->io_fds[i] = fds;
				fds->priv = &opriv->io_fds[i];
				break;
			}
		}

		if (i >= IOTDEV_NPOLLWAITERS) {
			lldbg("ERROR: Too many poll waiters\n");
			fds->priv = NULL;
			ret       = -EBUSY;
			goto errout_with_dusem;
		}
	} else if (fds->priv) {
		/* This is a request to tear down the poll. */
		FAR struct pollfd **slot = (FAR struct pollfd **)fds->priv;

		/* Remove all memory of the poll setup */
		*slot = NULL;
		fds->priv = NULL;
	}

errout_with_dusem:
	IOTDEV_UNLOCK(priv);
	// id_givesem(&priv->gu_exclsem);
	return ret;
}



/************************************************************************************
 * Name: id_drv_ioctl
 *
 * Description:  The standard ioctl method.
 *
 ************************************************************************************/
static int id_drv_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	return 0;
}

/************************************************************************************
 * Name: id_drv_read
 *
 * Description:  The standard read method.
 *
 ************************************************************************************/
static ssize_t id_drv_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	FAR struct inode *inode;
	FAR struct id_upperhalf_s *priv;
//	FAR struct id_open_s *opriv;
	int ret;
//	int i;

	//DEBUGASSERT(filep && filep->f_priv && filep->f_inode);
	DEBUGASSERT(filep && filep->f_inode);
	//opriv = filep->f_priv;
	inode = filep->f_inode;
	DEBUGASSERT(inode->i_private);
	priv  = (FAR struct id_upperhalf_s *)inode->i_private;

	IOTDEV_LOCK(priv);
	id_evt_type evt;
	ret = _id_get_evt(&priv->evt_queue, &evt);
	if (ret < 0) {
		printf("No event");
		return -1;
	}
	IOTDEV_UNLOCK(priv);

	buffer[0] = (char)evt;

	return 1;
}

static ssize_t id_drv_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return len;                                     /* Say that everything was written */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: id_drv_register
 *
 * Description:
 *   Register /dev/testcase
 *
 ****************************************************************************/

void id_drv_register(void)
{
	struct id_upperhalf_s *dev = &g_id_udev;

	int i = 0;
	for (; i < IOTDEV_NPOLLWAITERS; i++) {
		g_id_open.io_fds[i] = NULL;
	}

	int res = sem_init(&g_id_udev.gu_exclsem, 0, 1);
	if (res == -1) {
		dbg("register device fail\n");
		return;
	}
	g_id_udev.gu_open = &g_id_open;

	// initialize evt queue
	dev->evt_queue.front = 0;
	dev->evt_queue.rear = 1;

	(void)register_driver(IOTDEV_DRVPATH, &id_drv_fops, 0666, dev);
}


static void _id_trigger_evt(FAR struct id_upperhalf_s *priv, id_evt_type evt)
{
	FAR struct id_open_s *opriv;
	irqstate_t flags;
	int i;

	/*
	 * This routine is called both task level and interrupt level,
	 * so interrupts must be disabled.
	 */
	flags = irqsave();

	_id_set_evt(&priv->evt_queue, evt);

	/* Visit each opened reference to the device */
	opriv = priv->gu_open;

	/* Have any poll events occurred? */
	for (i = 0; i < IOTDEV_NPOLLWAITERS; i++) {
		FAR struct pollfd *fds = opriv->io_fds[i];
		if (fds) {
			fds->revents |= (fds->events & POLLIN);
			if (fds->revents != 0) {
				sem_post(fds->sem);
			}
		}
	}

	irqrestore(flags);
}

/*
 * public
 */
int id_post_evt(id_evt_type evt)
{
	struct id_upperhalf_s *dev = &g_id_udev;

	if (!dev) {
		return -1;
	}

	/* IOTDEV_LOCK(dev); */
	/* dev->evt = evt; */
	/* IOTDEV_UNLOCK(dev); */

	_id_trigger_evt(dev, evt);
	return 0;
}

