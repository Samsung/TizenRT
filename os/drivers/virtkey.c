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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <poll.h>
#include <errno.h>
#include <tinyara/fs/fs.h>
#include <assert.h>
#include <debug.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef CONFIG_VIRTKEY_QUEUE_LEN
#define MAX_LEN CONFIG_VIRTKEY_QUEUE_LEN
#else
#define MAX_LEN 1024
#endif
/****************************************************************************
 * Private Types
 ****************************************************************************/
struct virtkey_dev_s {
	sem_t datasem;
	char *events;
	uint16_t size;
	uint16_t head;
	uint16_t tail;
#ifndef CONFIG_DISABLE_POLL
	sem_t pollsem;
	struct pollfd *fds[CONFIG_SERIAL_NPOLLWAITERS];
#endif
};

/****************************************************************************
 * Private Data
 ****************************************************************************/
static FAR struct virtkey_dev_s g_virtkey_priv;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static ssize_t virtkey_read(FAR struct file *, FAR char *, size_t);
static ssize_t virtkey_write(FAR struct file *, FAR const char *, size_t);
#ifndef CONFIG_DISABLE_POLL
static void virtkey_pollnotify(FAR struct virtkey_dev_s *dev, pollevent_t eventset);
static int virtkey_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations virtkey_fops = {
	0,			/* open */
	0,			/* close */
	virtkey_read,		/* read */
	virtkey_write,		/* write */
	0,			/* seek */
	0			/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, virtkey_poll		/* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static inline int virtkey_takesem(FAR sem_t *sem, bool errout)
{
	/* Take a count from the semaphore, possibly waiting */
	if (sem_wait(sem) < 0) {
		/* EINTR is the only error that we expect */
		int errcode = get_errno();
		DEBUGASSERT(errcode == EINTR);
		if (errout) {
			return errcode;
		}
	}

	return 0;
}

static inline void virtkey_givesem(sem_t *sem)
{
	sem_post(sem);
}


/****************************************************************************
 * Name: virtkey_read
 ****************************************************************************/

static ssize_t virtkey_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	int ret = 0;

	struct virtkey_dev_s *priv = &g_virtkey_priv;
	
	virtkey_takesem(&priv->datasem, false);
	
	if (priv->size == 0) {
		ret = 0;
	} else {
		for (int i = 0; i < len; i++) {
			if (priv->size == 0) {
				break;
			}
			buffer[i] = priv->events[priv->head];
			priv->head = (priv->head + 1) % MAX_LEN;
			priv->size--;
			ret++;
		}
	}

	virtkey_givesem(&priv->datasem);
		
	return ret;
}

/****************************************************************************
 * Name: virtkey_write
 ****************************************************************************/

static ssize_t virtkey_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	int ret = 0;

	struct virtkey_dev_s *priv = &g_virtkey_priv;

	virtkey_takesem(&priv->datasem, false);

	if (priv->size == MAX_LEN) {
		ret = 0;
	} else {
		if (len > (MAX_LEN - priv->size)) {
			virtkey_givesem(&priv->datasem);
			return 0;
		}
		for (int i = 0; i < len; i++) {
			priv->events[priv->tail] = buffer[i];
			priv->tail = (priv->tail + 1) % MAX_LEN;
			priv->size++;
			ret++;
		}
	}

	virtkey_givesem(&priv->datasem);

	if (ret > 0) {
		/* notify only if we got some data */
		virtkey_pollnotify(priv, POLLIN);
	}

	return ret;
}

#ifndef CONFIG_DISABLE_POLL
static void virtkey_pollnotify(FAR struct virtkey_dev_s *dev, pollevent_t eventset)
{
	int i;
	for (i = 0; i < CONFIG_SERIAL_NPOLLWAITERS; i++) {
		struct pollfd *fds = dev->fds[i];
		if (fds) {
			fds->revents |= (fds->events & eventset);
			if (fds->revents != 0) {
				sem_post(fds->sem);
			}
		}
	}
}
#else
#define virtkey_pollnotify(dev, event)
#endif

#ifndef CONFIG_DISABLE_POLL
int virtkey_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct virtkey_dev_s *dev = inode->i_private;
	pollevent_t eventset;
	int ret;
	int i;

	/* Some sanity checking */

#ifdef CONFIG_DEBUG
	if (!dev || !fds) {
		return -ENODEV;
	}
#endif

	/* Are we setting up the poll?  Or tearing it down? */

	ret = virtkey_takesem(&dev->pollsem, true);
	if (ret < 0) {
		/* A signal received while waiting for access to the poll data
		 * will abort the operation.
		 */

		return ret;
	}

	if (setup) {
		/* This is a request to set up the poll.  Find an available
		 * slot for the poll structure reference
		 */

		for (i = 0; i < CONFIG_SERIAL_NPOLLWAITERS; i++) {
			/* Find an available slot */

			if (!dev->fds[i]) {
				/* Bind the poll structure and this slot */

				dev->fds[i] = fds;
				fds->priv = &dev->fds[i];
				fds->filep = (void *)filep;
				break;
			}
		}

		if (i >= CONFIG_SERIAL_NPOLLWAITERS) {
			fds->priv = NULL;
			fds->filep = NULL;
			ret = -EBUSY;
			goto errout;
		}

		eventset = 0;
		(void)virtkey_takesem(&dev->datasem, false);

		/* use poll only for reading events */
		if (dev->tail != dev->head) {
			eventset |= (fds->events & POLLIN);
		}

		virtkey_givesem(&dev->datasem);

		if (eventset) {
			virtkey_pollnotify(dev, eventset);
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
	virtkey_givesem(&dev->pollsem);
	return ret;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: virtkey_register
 *
 * Description:
 *   Register /dev/virktey
 *
 ****************************************************************************/

void virtkey_register(void)
{
	struct virtkey_dev_s *priv = &g_virtkey_priv;
	
	priv->events = (char *)kmm_malloc(MAX_LEN);

	if (priv->events == NULL) {
		dbg("failed to register virtkey driver\n");
		return;
	}

	priv->head = 0;
	priv->tail = 0;
	priv->size = 0;
	sem_init(&priv->datasem, 0, 1);
	sem_init(&priv->pollsem, 0, 1);

#ifndef CONFIG_DISABLE_POLL
	(void)virtkey_takesem(&priv->pollsem, false);
	for (int i = 0; i < CONFIG_SERIAL_NPOLLWAITERS; i++) {
			priv->fds[i] = NULL;
	}
	virtkey_givesem(&priv->pollsem);
#endif

	(void)register_driver("/dev/virtkey", &virtkey_fops, 0666, priv);
}
