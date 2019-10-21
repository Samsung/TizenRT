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

#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <poll.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/lwnl/lwnl80211.h>
#include "lwnl_evt_queue.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define DMA_BUFFER_MAX_SIZE 65536	/* 64K */

#define DMA_BUFFER_MIN_SIZE 4096	/* 4K */

#define LWNLDEV_LOCK(upper)						\
	do {										\
		ret = sem_wait(&upper->exclsem);		\
		if (ret < 0) {							\
			LWNL80211_ERR;						\
			return ret;							\
		}										\
	} while (0)

#define LWNLDEV_UNLOCK(upper)					\
	do {										\
		sem_post(&upper->exclsem);				\
	} while (0)


/****************************************************************************
 * Private Types
 ****************************************************************************/

struct lwnl80211_open_s {

	/* The following will be true if we are closing */
	volatile bool io_closing;

#ifndef CONFIG_DISABLE_POLL
	/*
	 * The following is a list if poll structures of threads waiting
	 * for driver events
	 */
	FAR struct pollfd *io_fds[LWNL_NPOLLWAITERS];
#endif
};

struct lwnl80211_upperhalf_s {
	uint8_t crefs;
	volatile bool started;
	sem_t exclsem;
	void *lower; /* Arch-specific operations */
	struct lwnl80211_open_s ln_open;
	struct lwnl_queue *queue;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int lwnl80211_open(struct file *filep);
static int lwnl80211_close(struct file *filep);
static ssize_t lwnl80211_read(struct file *filep, char *buffer, size_t len);
static ssize_t lwnl80211_write(struct file *filep, const char *buffer, size_t len);
static int lwnl80211_ioctl(struct file *filep, int cmd, unsigned long arg);
#ifndef CONFIG_DISABLE_POLL
static int lwnl80211_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup);
#endif


/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_lwnl80211_fops = {
	lwnl80211_open,                                          /* open */
	lwnl80211_close,                                         /* close */
	lwnl80211_read,                                          /* read */
	lwnl80211_write,                                         /* write */
	0,                                                       /* seek */
	lwnl80211_ioctl                                          /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, lwnl80211_poll                                                      /* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int lwnl80211_open(struct file *filep)
{
	LWNL80211_ENTER;
	struct inode *inode = filep->f_inode;
	struct lwnl80211_upperhalf_s *upper = inode->i_private;
	int tmp_crefs;
	int ret = -EMFILE;

	tmp_crefs = upper->crefs + 1;
	if (tmp_crefs == 0) {
		goto errout;
	}

	upper->crefs = tmp_crefs;

	int res = lwnl_add_listener(filep);
	if (res < 0) {
		goto errout;
	}

	ret = OK;

errout:
	LWNL80211_LEAVE;
	return ret;
}


static int lwnl80211_close(struct file *filep)
{
	LWNL80211_ENTER;
	struct inode *inode = filep->f_inode;
	struct lwnl80211_upperhalf_s *upper = inode->i_private;
	int ret = OK;

	if (upper->crefs > 0) {
		upper->crefs--;
	} else {
		ret = -ENOSYS;
	}

	int res = lwnl_remove_listener(filep);
	if (res < 0) {
		ret = -ENOSYS;
		goto errout;
	}

errout:
	LWNL80211_LEAVE;
	return ret;
}


static ssize_t lwnl80211_read(struct file *filep, char *buffer, size_t len)
{
	LWNL80211_ENTER;
	int res = lwnl_get_event(filep, buffer, len);
	// todo_net : convert res to vfs error style?
	LWNL80211_LEAVE;
	return res;
}

#ifndef CONFIG_NET_NETMGR
extern int lwnl_message_handle(struct lwnl80211_lowerhalf_s* lower, const char *msg, int msg_len);
#endif

static ssize_t lwnl80211_write(struct file *filep, const char *buffer, size_t len)
{
	LWNL80211_ENTER;
	int ret = -EINVAL;
	struct inode *inode = filep->f_inode;
	struct lwnl80211_upperhalf_s *upper = inode->i_private;

	LWNLDEV_LOCK(upper);

#ifdef CONFIG_NET_NETMGR
	ret = netdev_lwnlioctl(cmd, arg);
#else
	ret = lwnl_message_handle((struct lwnl80211_lowerhalf_s*)upper->lower, buffer, len);
#endif

	LWNLDEV_UNLOCK(upper);
	LWNL80211_LEAVE;
	if (ret < 0) {
		return -1;
	}
	return len;
}


static int lwnl80211_ioctl(struct file *filep, int cmd, unsigned long arg)
{
	LWNL80211_ENTER;

	return 0;
}


static int lwnl80211_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup)
{
	LWNL80211_ENTER;

	FAR struct inode *inode;
	FAR struct lwnl80211_upperhalf_s *upper;
	FAR struct lwnl80211_open_s *opriv;
	int ret = 0;

	DEBUGASSERT(filep && filep->f_inode);
	inode = filep->f_inode;
	DEBUGASSERT(inode->i_private);
	upper  = (FAR struct lwnl80211_upperhalf_s *)inode->i_private;
	opriv = &upper->ln_open;

	/* Get exclusive access to the driver structure */
	LWNLDEV_LOCK(upper);

	/* Are we setting up the poll? Or tearing it down? */
	if (setup) {
		/*
		 * This is a request to set up the poll. Find an available
		 * slot for the poll structure reference
		 */
		int i = 0;
		for (; i < LWNL_NPOLLWAITERS; i++) {
			/* Find an available slot */
			if (!opriv->io_fds[i]) {
				/* Bind the poll structure and this slot */
				opriv->io_fds[i] = fds;
				fds->priv = &opriv->io_fds[i];
				break;
			}
		}

		if (i >= LWNL_NPOLLWAITERS) {
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
	LWNLDEV_UNLOCK(upper);

	return ret;
}


/*
 * Public APIs
 */
struct lwnl80211_upperhalf_s *g_lwnl_upper = NULL;

int lwnl80211_register(struct lwnl80211_lowerhalf_s *dev)
{
	LWNL80211_ENTER;
	struct lwnl80211_upperhalf_s *upper = NULL;
	int ret;

	upper = (struct lwnl80211_upperhalf_s *)kmm_zalloc(sizeof(struct lwnl80211_upperhalf_s));
	if (!upper) {
		LWNL80211_ERR;
		return -ENOMEM;
	}

	for (int i = 0; i < LWNL_NPOLLWAITERS; i++) {
		upper->ln_open.io_fds[i] = NULL;
	}

	sem_init(&upper->exclsem, 0, 1);
	upper->lower = (void *)dev;
	dev->parent = upper;
	g_lwnl_upper = upper;

	lwnl_queue_initialize();

	ret = register_driver(LWNL80211_PATH, &g_lwnl80211_fops, 0666, upper);
	if (ret < 0) {
		LWNL80211_ERR;
		goto errout_with_priv;
	}

	LWNL80211_LEAVE;

	return OK;

errout_with_priv:
	sem_destroy(&upper->exclsem);
	kmm_free(upper);
	return ret;
}

int lwnl80211_unregister(struct lwnl80211_lowerhalf_s *dev)
{
	return 0;
}

int lwnl80211_postmsg(lwnl80211_cb_status evttype, void *buffer)
{
	if (!g_lwnl_upper) {
		return -1;
	}

	int res = lwnl_add_event(evttype, buffer);
	if (res < 0) {
		return -1;
	}

	for (int i = 0; i < LWNL_NPOLLWAITERS; i++) {
		struct pollfd *fds = g_lwnl_upper->ln_open.io_fds[i];
		if (fds) {
			fds->revents |= (fds->events & POLLIN);
			if (fds->revents != 0) {
				sem_post(fds->sem);
			}
		}
	}

	return 0;
}

