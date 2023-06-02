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

#include <tinyara/kmalloc.h>
#include <tinyara/gpio.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct gpio_upperhalf_s {
	/* Saved binding to the lower-half GPIO driver */
	FAR struct gpio_lowerhalf_s *gu_lower; /* Arch-specific operations */

	sem_t gu_exclsem;	/* Supports exclusive access to the device */
	bool gu_sample;		/* Last sampled GPIO states */

	/*
	 * The following is a singly linked list of open references to the
	 * GPIO device.
	 */
	struct gpio_open_s *gu_open;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int     gpio_open(FAR struct file *filep);
static int     gpio_close(FAR struct file *filep);
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
	gpio_open,	/* open  */
	gpio_close,	/* close */
	gpio_read,	/* read  */
	gpio_write,	/* write */
	NULL,		/* seek  */
	gpio_ioctl,	/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	gpio_poll	/* poll  */
#endif
};

struct gpio_open_s {
	FAR struct gpio_open_s *go_flink;

	/* The following will be treu if we are closing */
	volatile bool go_closing;

#ifndef CONFIG_DISABLE_SIGNALS
	/* GPIO event notification information */
	pid_t go_pid;
	struct gpio_notify_s go_notify;
#endif

#ifndef CONFIG_DISABLE_POLL
	/* Poll event information */
	struct gpio_pollevents_s go_pollevents;

	/*
	 * The following is a list if poll structures of threads waiting
	 * for driver events
	 */
	FAR struct pollfd *go_fds[CONFIG_GPIO_NPOLLWAITERS];
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: gpio_takesem
 *
 * Description:
 *    Take the lock, waiting as necessary
 *
 ****************************************************************************/
static inline int gpio_takesem(FAR sem_t *sem)
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

static void gpio_sample(FAR struct gpio_upperhalf_s *priv)
{
	FAR struct gpio_lowerhalf_s *lower;
	FAR struct gpio_open_s *opriv;
#if !defined(CONFIG_DISABLE_POLL) || !defined(CONFIG_DISABLE_SIGNALS)
	bool sample, change, falling, rising;
#endif
	irqstate_t flags;
#ifndef CONFIG_DISABLE_POLL
	int i;
#endif

	DEBUGASSERT(priv && priv->gu_lower);
	lower = priv->gu_lower;

	/*
	 * This routine is called both task level and interrupt level,
	 * so interrupts must be disabled.
	 */
	flags = enter_critical_section();

	/* Sample the new GPIO state */
	DEBUGASSERT(lower->ops && lower->ops->get);
	sample = lower->ops->get(lower);

#if !defined(CONFIG_DISABLE_POLL) || !defined(CONFIG_DISABLE_SIGNALS)
	change  = sample ^ priv->gu_sample;
	rising  = (change && sample);
	falling = (change && !sample);

	/* Visit each opened reference to the device */
	for (opriv = priv->gu_open; opriv; opriv = opriv->go_flink) {
#ifndef CONFIG_DISABLE_POLL
		/* Have any poll events occurred? */
		if ((rising  & opriv->go_pollevents.gp_rising) != 0 ||
			(falling & opriv->go_pollevents.gp_falling) != 0) {
			for (i = 0; i < CONFIG_GPIO_NPOLLWAITERS; i++) {
				FAR struct pollfd *fds = opriv->go_fds[i];
				if (fds) {
					fds->revents |= (fds->events & POLLIN);
					if (fds->revents != 0) {
						sem_post(fds->sem);
					}
				}
			}
		}
#endif

#ifndef CONFIG_DISABLE_SIGNALS
		/* Have any signal events occurred? */
		if ((rising && opriv->go_notify.gn_rising) ||
				(falling && opriv->go_notify.gn_falling)) {
#ifdef CONFIG_CAN_PASS_STRUCTS
			union sigval value;
			value.sival_int = (int)sample;
			sigqueue(opriv->go_pid, opriv->go_notify.gn_signo, value);
#else
			sigqueue(opriv->go_pid, opriv->go_notify.gn_signo, sample);
#endif
		}
#endif
	}
#endif

	priv->gu_sample = sample;
	leave_critical_section(flags);
}

#if !defined(CONFIG_DISABLE_POLL) || !defined(CONFIG_DISABLE_SIGNALS)
/****************************************************************************
 * Name: gpio_interrupt
 *
 * Description:
 *    notify file descriptor. It is necessary to handle async I/O.
 *
 ****************************************************************************/
static void gpio_interrupt(FAR struct gpio_upperhalf_s *upper)
{
	DEBUGASSERT(upper);
	gpio_sample(upper);
}

static void gpio_enable(FAR struct gpio_upperhalf_s *priv)
{
	FAR struct gpio_lowerhalf_s *lower;
	FAR struct gpio_open_s *opriv;
	bool rising;
	bool falling;
	irqstate_t flags;
#ifndef CONFIG_DISABLE_POLL
	int i;
#endif

	DEBUGASSERT(priv && priv->gu_lower);
	lower = priv->gu_lower;

	/*
	 * This routine is called both task level and interrupt level, so
	 * interrupts must be disabled.
	 */
	flags = enter_critical_section();

	/* Visit each opened reference to the device */
	rising  = 0;
	falling = 0;

	for (opriv = priv->gu_open; opriv; opriv = opriv->go_flink) {
#ifndef CONFIG_DISABLE_POLL
		for (i = 0; i < CONFIG_GPIO_NPOLLWAITERS; i++) {
			if (opriv->go_fds[i]) {
				rising  |= opriv->go_pollevents.gp_rising;
				falling |= opriv->go_pollevents.gp_falling;
				break;
			}
		}
#endif

#ifndef CONFIG_DISABLE_SIGNALS
		/* OR in the signal events */
		rising  |= opriv->go_notify.gn_rising;
		falling |= opriv->go_notify.gn_falling;
#endif
	}

	/* Enable/disable GPIO interrupts */
	DEBUGASSERT(lower->ops->enable);
	if (rising || falling) {
		/*
		  * gu_sample saved the old status for GPIO, it used when interrupt coming to compare
		  * current status to gu_sample.
		  * It update on system startup during gpio_register().
		  * But, device driver init always after gpio init.
		  * If GPIO status changed after gpio_register(), it will not record.
		  * So, we update gu_sample before enable gpio irq.
		  */
		priv->gu_sample = lower->ops->get(lower);
		lower->ops->enable(lower, falling, rising, gpio_interrupt);
	} else {
		/* Disable further interrupts */
		lower->ops->enable(lower, false, false, NULL);
	}

	leave_critical_section(flags);
}
#endif

#ifdef CONFIG_IOTDEV
static int gpio_enable_interrupt(FAR struct gpio_upperhalf_s *priv, unsigned long arg)
{
	FAR struct gpio_lowerhalf_s *lower;
	
	DEBUGASSERT(priv && priv->gu_lower);
	lower = priv->gu_lower;

	bool rising;
	bool falling;
	int ret;
	irqstate_t flags;

	flags = enter_critical_section();

	switch (arg) {
	case GPIO_EDGE_NONE:
		rising = false;
		falling = false;
		break;
	case GPIO_EDGE_BOTH:
		rising = true;
		falling = true;
		break;
	case GPIO_EDGE_RISING:
		rising = true;
		falling = false;
		break;
	case GPIO_EDGE_FALLING:
		rising = false;
		falling = true;
		break;
	default:
		lldbg("Interrupt value is invalid\n");
		leave_critical_section(flags);
		return ERROR;
		break;
	}

	DEBUGASSERT(lower->ops->enable);
	if (rising || falling) {
		ret = lower->ops->enable(lower, falling, rising, gpio_interrupt);
	} else {
		/* Disable further interrupts */
		ret = lower->ops->enable(lower, false, false, NULL);
	}
	leave_critical_section(flags);

	return ret;
}
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
	ssize_t ret;
	int32_t value;
	FAR struct inode *inode = filep->f_inode;
	FAR struct gpio_upperhalf_s *priv = inode->i_private;
	FAR struct gpio_lowerhalf_s *lower = priv->gu_lower;

	ret = sscanf(buffer, "%d", &value);
	if (ret) {
		lower->ops->set(lower, value != 0);
	}

	return ret;
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
	int ret = 0;
	FAR struct inode *inode = filep->f_inode;
	FAR struct gpio_upperhalf_s *priv = inode->i_private;
	FAR struct gpio_lowerhalf_s *lower = priv->gu_lower;

	if (filep->f_pos == 0) {
		int value = lower->ops->get(lower);
		ret = snprintf(buffer, buflen, "%d", value);
	}

	filep->f_pos += ret;

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
	int ret = OK;
	FAR struct inode *inode = filep->f_inode;
	FAR struct gpio_upperhalf_s *priv = inode->i_private;
	FAR struct gpio_open_s *opriv = filep->f_priv;
	FAR struct gpio_lowerhalf_s *lower = priv->gu_lower;

	switch (cmd) {
	case GPIOIOC_SET_DIRECTION:
		ret = lower->ops->setdir(lower, arg);
#ifndef CONFIG_DISABLE_POLL
		opriv->go_pollevents.gp_rising  = false;
		opriv->go_pollevents.gp_falling = false;
#endif /* CONFIG_DISABLE_POLL */
		break;

	case GPIOIOC_SET_DRIVE:
		ret = lower->ops->pull(lower, arg);
		break;

#ifndef CONFIG_DISABLE_POLL
	case GPIOIOC_POLLEVENTS: {
		FAR struct gpio_pollevents_s *pollevents =
			(FAR struct gpio_pollevents_s *)((uintptr_t)arg);

		if (pollevents) {
			/* Save the poll events */
			opriv->go_pollevents.gp_rising  = pollevents->gp_rising;
			opriv->go_pollevents.gp_falling = pollevents->gp_falling;

			/* Enable/disable interrupt handling */
			gpio_enable(priv);
			ret = OK;
		}
		break;
	}
#endif /* CONFIG_DISABLE_POLL */

#ifndef CONFIG_DISABLE_SIGNALS
	case GPIOIOC_REGISTER: {
		FAR struct gpio_notify_s *notify =
			(FAR struct gpio_notify_s *)((uintptr_t)arg);

		if (notify) {
			/* Save the notification events */
			opriv->go_notify.gn_rising  = notify->gn_rising;
			opriv->go_notify.gn_falling = notify->gn_falling;
			opriv->go_notify.gn_signo   = notify->gn_signo;
			opriv->go_pid               = getpid();

			/* Enable/disable interrupt handling */
			gpio_enable(priv);
			ret = OK;
		}
		break;
	}
#endif /* CONFIG_DISABLE_SIGNALS */
#ifdef CONFIG_IOTDEV
	case GPIOIOC_SET_INTERRUPT: {
		ret = gpio_enable_interrupt(priv, arg);
		break;
	}
#endif
	default:
		ret = -ENOTTY;
		if (lower->ops->ioctl) {
			ret = lower->ops->ioctl(lower, cmd, arg);
		}
	}

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
	FAR struct inode *inode;
	FAR struct gpio_upperhalf_s *priv;
	FAR struct gpio_open_s *opriv;
	int ret;
	int i;

	DEBUGASSERT(filep && filep->f_priv && filep->f_inode);
	opriv = filep->f_priv;
	inode = filep->f_inode;
	DEBUGASSERT(inode->i_private);
	priv  = (FAR struct gpio_upperhalf_s *)inode->i_private;

	/* Get exclusive access to the driver structure */
	ret = gpio_takesem(&priv->gu_exclsem);
	if (ret < 0) {
		lldbg("ERROR: gpio_takesem failed: %d\n", ret);
		return ret;
	}

	/* Are we setting up the poll? Or tearing it down? */
	if (setup) {
		/*
		 * This is a request to set up the poll. Find an available
		 * slot for the poll structure reference
		 */
		for (i = 0; i < CONFIG_GPIO_NPOLLWAITERS; i++) {
			/* Find an available slot */
			if (!opriv->go_fds[i]) {
				/* Bind the poll structure and this slot */
				opriv->go_fds[i] = fds;
				fds->priv = &opriv->go_fds[i];
				fds->filep = (void *)filep;
				break;
			}
		}

		if (i >= CONFIG_GPIO_NPOLLWAITERS) {
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

	gpio_enable(priv);

errout_with_dusem:
	gpio_givesem(&priv->gu_exclsem);
	return ret;
}
#endif /* CONFIG_DISABLE_POLL */

/****************************************************************************
 * Name: gpio_close
 *
 * Description:
 *   This routine is called when the gpio gets closed.
 *
 ****************************************************************************/
static int gpio_close(FAR struct file *filep)
{
	FAR struct inode *inode;
	FAR struct gpio_upperhalf_s *priv;
	FAR struct gpio_open_s *opriv;
	FAR struct gpio_open_s *curr;
	FAR struct gpio_open_s *prev;
	irqstate_t flags;
	bool closing;
	int ret;
#ifndef CONFIG_DISABLE_POLL
	int waiter_idx;
#endif

	DEBUGASSERT(filep && filep->f_priv && filep->f_inode);
	opriv = filep->f_priv;
	inode = filep->f_inode;
	DEBUGASSERT(inode->i_private);
	priv = (FAR struct gpio_upperhalf_s *)inode->i_private;

	flags = enter_critical_section();
	closing = opriv->go_closing;
	opriv->go_closing = true;
	leave_critical_section(flags);

	if (closing) {
		/* Another thread is doing the close */
		return OK;
	}

	/* Get exclusive access to the driver structure */
	ret = gpio_takesem(&priv->gu_exclsem);
	if (ret < 0) {
		lldbg("ERROR: gpio_takesem failed: %d\n", ret);
		return ret;
	}

#ifndef CONFIG_DISABLE_POLL
	/*
	 * Check if this file is registered in a list of waiters for polling.
	 * If it is, the used slot should be cleared.
	 * Otherwise, an invalid pollfd remains in a list and this slot is not available forever.
	 */
	for (waiter_idx = 0; waiter_idx < CONFIG_GPIO_NPOLLWAITERS; waiter_idx++) {
		struct pollfd *fds = opriv->go_fds[waiter_idx];
		if (fds && (FAR struct file *)fds->filep == filep) {
			opriv->go_fds[waiter_idx] = NULL;
		}
	}
#endif

	/* Find the open structure in the list of open structures for the device */
	for (prev = NULL, curr = priv->gu_open; curr && curr != opriv;
			prev = curr, curr = curr->go_flink);

	DEBUGASSERT(curr);
	if (!curr) {
		lldbg("ERROR: Failed to find open entry\n");
		ret = -ENOENT;
		goto errout_with_exclsem;
	}

	/* Remove the structure from the device */
	if (prev) {
		prev->go_flink = opriv->go_flink;
	} else {
		priv->gu_open = opriv->go_flink;
	}

	/* And free the open structure */
	kmm_free(opriv);
	filep->f_priv = NULL;

	ret = OK;

errout_with_exclsem:
	gpio_givesem(&priv->gu_exclsem);

	return ret;
}

/*****************************************************************************
 * Name: gpio_open
 *
 * Description:
 *   This routine is called whenever gpio is opened.
 *
 ****************************************************************************/
static int gpio_open(FAR struct file *filep)
{
	int ret;
	FAR struct inode *inode = filep->f_inode;
	FAR struct gpio_upperhalf_s *priv = inode->i_private;
	FAR struct gpio_open_s *opriv;

	/*
	 * If the port is the middle of closing, wait until the close is
	 * finished. If a signal is received while we are waiting, then
	 * return EINTR.
	 */
	ret = gpio_takesem(&priv->gu_exclsem);
	if (ret < 0) {
		/*
		 * A signal received while waiting for the last close
		 * operation.
		 */
		return ret;
	}

	/* Allocate a new open structure */
	opriv = (FAR struct gpio_open_s *)kmm_zalloc(sizeof(struct gpio_open_s));
	if (!opriv) {
		lldbg("ERROR: Failed to allocate open structure\n");
		ret = -ENOMEM;
		goto errout_with_sem;
	}

	/* Initialize the open structure */
#ifndef CONFIG_DISABLE_POLL
	opriv->go_pollevents.gp_falling = true;
	opriv->go_pollevents.gp_rising  = true;
#endif

	/* Attach the open structure to the device */
	opriv->go_flink = priv->gu_open;
	priv->gu_open = opriv;

	/* Attach the open structure to the file structure */
	filep->f_priv = (FAR void *)opriv;
	ret = OK;

errout_with_sem:
	gpio_givesem(&priv->gu_exclsem);
	return ret;
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
int gpio_register(unsigned int minor, FAR struct gpio_lowerhalf_s *lower)
{
	int ret;
	char devpath[16];
	FAR struct gpio_upperhalf_s *priv;

	DEBUGASSERT(lower);

	/* Allocate a new GPIO driver instance */
	priv = (FAR struct gpio_upperhalf_s *)
			kmm_zalloc(sizeof(struct gpio_upperhalf_s));
	if (!priv) {
		lldbg("ERROR: Failed to allocate device structure\n");
		return -ENOMEM;
	}

	/* Make sure that all button interrupts are disabled */
	DEBUGASSERT(lower->ops->enable);
	lower->ops->enable(lower, 0, 0, NULL);

	/* Initiailize the new GPIO driver instance */
	priv->gu_lower = lower;
	lower->parent = priv;
	sem_init(&priv->gu_exclsem, 0, 1);

	DEBUGASSERT(lower->ops && lower->ops->get);
	priv->gu_sample = lower->ops->get(lower);

	snprintf(devpath, 16, "/dev/gpio%d", minor);

	/* And register the GPIO driver */
	ret = register_driver(devpath, &g_gpioops, 0666, priv);
	if (ret < 0) {
		lldbg("ERROR: register driver failed: %d\n", ret);
		goto errout_with_priv;
	}

	return OK;

errout_with_priv:
	sem_destroy(&priv->gu_exclsem);
	kmm_free(priv);
	return ret;
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
