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

#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <poll.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/gpio.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define GPIO_PREVENT_MULTIPLE_OPEN   0

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int     gpio_fopen(FAR struct file *filep);
static int     gpio_fclose(FAR struct file *filep);
static ssize_t gpio_read(FAR struct file *filep, FAR char *buffer,
			 size_t buflen);
static ssize_t gpio_write(FAR struct file *filep, FAR const char *buffer,
			  size_t buflen);
static int     gpio_ioctl(FAR struct file *filep, int cmd,
			  unsigned long arg);
#ifndef CONFIG_DISABLE_POLL
static int     gpio_poll(FAR struct file *filep, FAR struct pollfd *fds,
			 bool setup);
#endif

/****************************************************************************
 * Private Variables
 ****************************************************************************/
static const struct file_operations g_gpioops = {
	gpio_fopen,	/* open  */
	gpio_fclose,	/* close */
	gpio_read,	/* read  */
	gpio_write,	/* write */
	NULL,		/* seek  */
	gpio_ioctl,	/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	gpio_poll	/* poll  */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sem_reinit
 *
 * Description:
 *    Reinitialize semaphore
 *
 ****************************************************************************/
#ifndef CONFIG_DISABLE_POLL
static int sem_reinit(FAR sem_t *sem, int pshared, unsigned int value)
{
	sem_destroy(sem);

	return sem_init(sem, pshared, value);
}
#endif

/****************************************************************************
 * Name: gpio_takesem
 *
 * Description:
 *    Take the lock, waiting as necessary
 *
 ****************************************************************************/
static int gpio_takesem(FAR sem_t *sem, bool errout)
{
	/*
	 * Loop, ignoring interrupts, until we have successfully acquired
	 * the lock
	 */

	while (sem_wait(sem) != OK) {
		/*
		 * The only case that an error should occur here is if the
		 * wait was awakened by a signal.
		 */
		ASSERT(get_errno() == EINTR);

		/*
		 * When the signal is received, should we errout? Or should
		 * we just continue waiting until we have the semaphore?
		 */
		if (errout) {
			return -EINTR;
		}
	}

	return OK;
}

/****************************************************************************
 * Name: gpio_givesem
 *
 * Description:
 *    release the lock
 *
 ****************************************************************************/
static inline void gpio_givesem(sem_t *sem)
{
	sem_post(sem);
}

/****************************************************************************
 * Name: gpio_pollnotify
 *
 * Description:
 *    notify file descriptor. It is necessary to handle async I/O.
 *
 ****************************************************************************/
#ifndef CONFIG_DISABLE_POLL
static void gpio_pollnotify(FAR struct gpio_dev_s *dev, pollevent_t eventset)
{
	int i;

	for (i = 0; i < CONFIG_GPIO_NPOLLWAITERS; i++) {
		struct pollfd *fds = dev->fds[i];
		if (fds) {
			fds->revents |= (fds->events & eventset);

			if (fds->revents != 0) {
				fvdbg("Report events: %02x\n", fds->revents);
				sem_post(fds->sem);
			}
		}
	}
}
#else
#define gpio_pollnotify(dev, event)
#endif

/****************************************************************************
 * Name: gpio_write
 *
 * Description:
 *    This function is called when you handle the write() API in
 *    File system.
 *
 ****************************************************************************/
static ssize_t gpio_write(FAR struct file *filep, FAR const char *buffer,
			  size_t buflen)
{
	int32_t value;
	FAR struct inode *inode = filep->f_inode;
	FAR struct gpio_dev_s *dev = inode->i_private;

	value = *(int32_t *) buffer;
	GPIO_SET(dev, value);

	return buflen;
}

/****************************************************************************
 * Name: gpio_read
 *
 * Description:
 *    This function is automatically called when you handle the read() API in
 *    File system.
 *
 ****************************************************************************/
static ssize_t gpio_read(FAR struct file *filep, FAR char *buffer,
			 size_t buflen)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct gpio_dev_s *dev = inode->i_private;
	int ret;
	int value;

	/*
	 * if you read gpio with 'cat', you have to return 0, Otherwise cat
	 * will be cleaned up properly. So If it is the first time, then
	 * return size, and resize fpos. next time return zero.
	 */
	if (filep->f_pos == 0) {
		value = GPIO_GET(dev);

		memcpy(buffer, &value, sizeof(int));

		/* move file position */
		filep->f_pos = 2;
		ret = 2;
	} else {
		ret = 0;
	}

	return ret;

}

/****************************************************************************
 * Name: gpio_ioctl
 *
 * Description:
 *    This function is allow you to control the event.
 *
 ****************************************************************************/
static int gpio_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret;
	FAR struct inode *inode = filep->f_inode;
	FAR struct gpio_dev_s *dev = inode->i_private;

	ret = GPIO_CTRL(dev, cmd, arg);

	return ret;
}

/****************************************************************************
 * Name: gpio_poll
 *
 * Description:
 *    This function is called when you want to wait for events.
 *    You don't know when the event will be occured.
 *
 ****************************************************************************/
#ifndef CONFIG_DISABLE_POLL
static int gpio_poll(FAR struct file *filep, FAR struct pollfd *fds,
		     bool setup)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct gpio_dev_s *dev = inode->i_private;
	int ret;
	int i;

	/* Some sanity checking */
	if (!dev || !fds) {
		return -ENODEV;
	}

	/* Are we setting up the poll?  Or tearing it down? */
	ret = gpio_takesem(&dev->pollsem, true);
	if (ret < 0) {
		/*
		 * A signal received while waiting for access to the poll data
		 * will abort the operation.
		 */
		return ret;
	}

	if (setup) {
		/*
		 * This is a request to set up the poll.  Find an available
		 * slot for the poll structure reference
		 */
		for (i = 0; i < CONFIG_GPIO_NPOLLWAITERS; i++) {
			/* Find an available slot */
			if (!dev->fds[i]) {
				/* Bind the poll structure and this slot */
				dev->fds[i] = fds;
				fds->priv = &dev->fds[i];
				break;
			}
		}

		if (i >= CONFIG_GPIO_NPOLLWAITERS) {
			fds->priv = NULL;
			ret = -EBUSY;
			goto errout;
		}
	} else if (fds->priv) {
		/* This is a request to tear down the poll. */
		struct pollfd **slot = (struct pollfd **)fds->priv;

		if (!slot) {
			ret = -EIO;
			goto errout;
		}

		/* Remove all memory of the poll setup */
		*slot = NULL;
		fds->priv = NULL;
	}

errout:
	gpio_givesem(&dev->pollsem);
	return ret;
}
#endif /* CONFIG_DISABLE_POLL */

/****************************************************************************
 * Name: gpio_fclose
 *
 * Description:
 *   This routine is called when the gpio gets closed.
 *
 ****************************************************************************/
static int gpio_fclose(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct gpio_dev_s *dev = inode->i_private;

	(void)gpio_takesem(&dev->closesem, false);
	if (dev->open_count > 1) {
		dev->open_count--;
		gpio_givesem(&dev->closesem);
		return OK;
	}

	/* There are no more references to the port */
	dev->open_count = 0;

	GPIO_CLOSE(dev);

#ifndef CONFIG_DISABLE_POLL
	sem_reinit(&dev->pollsem, 0, 1);
#endif
	gpio_givesem(&dev->closesem);

	return OK;
}

/*****************************************************************************
 * Name: gpio_fopen
 *
 * Description:
 *   This routine is called whenever gpio is opened.
 *
 ****************************************************************************/
static int gpio_fopen(FAR struct file *filep)
{
	int ret;
	FAR struct inode *inode = filep->f_inode;
	FAR struct gpio_dev_s *dev = inode->i_private;
	uint8_t tmp;

	/*
	 * If the port is the middle of closing, wait until the close is
	 * finished. If a signal is received while we are waiting, then
	 * return EINTR.
	 */
	ret = gpio_takesem(&dev->closesem, true);
	if (ret < 0) {
		/*
		 * A signal received while waiting for the last close
		 * operation.
		 */
		return ret;
	}

	tmp = dev->open_count + 1;
	if (tmp == 0) {
		/* More than 255 opens; uint8_t overflows to zero */
		ret = -EMFILE;
		goto errout_with_sem;
	}

	/* Check if this is the first time that the driver has been opened. */
	if (tmp == 1) {
		ret = GPIO_OPEN(dev);
		if (ret != OK) {
			goto errout_with_sem;
		}
	}
#if GPIO_PREVENT_MULTIPLE_OPEN == 1
	else if (tmp > 1) {
		ret = -EBUSY;
		goto errout_with_sem;
	}
#endif

	dev->open_count = tmp;

errout_with_sem:
	gpio_givesem(&dev->closesem);

	return ret;
}

/*****************************************************************************
 * Name: gpio_notify
 *
 * Description:
 *   This routine is called from irq handler. If you want to handle with
 *   async call or event, you have to register your fd with poll().
 *
 ****************************************************************************/
void gpio_notify(FAR struct gpio_dev_s *dev)
{
	gpio_pollnotify(dev, POLLIN);

	return;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: gpio_register
 *
 * Description:
 *   Register GPIO device.
 *
 ****************************************************************************/
int gpio_register(FAR const char *path, FAR struct gpio_dev_s *dev)
{
	sem_init(&dev->closesem, 0, 1);
#ifndef CONFIG_DISABLE_POLL
	sem_init(&dev->pollsem, 0, 1);
#endif

	dbg("Registering %s\n", path);
	return register_driver(path, &g_gpioops, 0666, dev);
}

/****************************************************************************
 * Name: gpio_unregister
 *
 * Description:
 *   unregister GPIO device.
 *
 ****************************************************************************/
int gpio_unregister(FAR const char *path)
{
	return unregister_driver(path);
}
