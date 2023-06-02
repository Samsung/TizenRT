/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <errno.h>
#include <debug.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#include <tinyara/sensors/ppd42ns.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define PPD42NS_TIME_GET(time)          gettimeofday(&time, NULL)
#define PPD42NS_TIME_DIFF_USEC(old_time, cur_time) \
	((cur_time.tv_sec * 1000000 + cur_time.tv_usec) - (old_time.tv_sec * 1000000 + old_time.tv_usec))

#define GPIO_SIGNAL_LOW				0
#define GPIO_SIGNAL_HIGH			1

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct ppd42ns_dev_s {
	FAR struct ppd42ns_config_s *config;	/* ppd42ns driver config */
	int crefs;					/* reference count on the driver instance */
	sem_t exclsem;				/* exclusive access to the device */
	sem_t datasem;				/* exclusive access while reading sensor data */
	int old_signal;				/* previous gpio signal */
	int lowpulseoccupancy;		/* low pulse signal time */
	struct timeval start_time;	/* start time of total period */
	struct timeval lowpulse_start_time;	/* start time of low pulse signal period */
};

/****************************************************************************
 * Private Functions Prototype
 ****************************************************************************/
static int ppd42ns_open(FAR struct file *filep);
static int ppd42ns_close(FAR struct file *filep);
static ssize_t ppd42ns_read(FAR struct file *filep, FAR char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* ppd42ns driver instance */
static FAR struct ppd42ns_dev_s g_ppd42ns_priv;

/* This the vtable that supports the character driver interface */
static const struct file_operations g_ppd42ns_fops = {
	ppd42ns_open,				/* open */
	ppd42ns_close,				/* close */
	ppd42ns_read,				/* read */
	0,							/* write */
	0,							/* seek */
	0,							/* ioctl */
};

/****************************************************************************
 * Private Function
 ****************************************************************************/

/****************************************************************************
 * Name: ppd42ns_takesem
 *
 * Description:
 *    Take the lock, waiting as necessary
 *
 ****************************************************************************/
static inline int ppd42ns_takesem(FAR sem_t *sem)
{
	/* Take a count from the semaphore, possibly waiting */
	if (sem_wait(sem) < 0) {
		/* EINTR is the only error that we expect */
		int errcode = get_errno();
		DEBUGASSERT(errcode == EINTR);
		return errcode;
	}

	return 0;
}

/****************************************************************************
 * Name: ppd42ns_givesem
 *
 * Description:
 *    release the lock
 *
 ****************************************************************************/
static inline void ppd42ns_givesem(sem_t *sem)
{
	sem_post(sem);
}

/****************************************************************************
 * Name: ppd42ns_interrupt_handler
 *
 * Description:
 *    handle ppd42ns's digital pin interrupt
 *
 ****************************************************************************/
static void ppd42ns_interrupt_handler(FAR void *arg)
{
	/* this is both edge interrupt handler on ppd42ns's digital signal pin */
	FAR struct ppd42ns_dev_s *priv = (FAR struct ppd42ns_dev_s *)arg;
	struct timeval time;
	irqstate_t flags;

	if (priv == NULL) {
		return;
	}

	/* This ISR handles only when gpio signal is changed. */
	if (priv->config->read_gpio(priv->config) == priv->old_signal) {
		return;
	}

	/* disable interrupt */
	flags = enter_critical_section();

	if (priv->old_signal == GPIO_SIGNAL_LOW) {
		/* signal LOW -> HIGH */
		PPD42NS_TIME_GET(time);
		priv->lowpulseoccupancy += PPD42NS_TIME_DIFF_USEC(priv->lowpulse_start_time, time);
		priv->old_signal = GPIO_SIGNAL_HIGH;
	} else {
		/* signal HIGH -> LOW */
		PPD42NS_TIME_GET(priv->lowpulse_start_time);
		priv->old_signal = GPIO_SIGNAL_LOW;
	}

	/* enable interrupt */
	leave_critical_section(flags);
}

/****************************************************************************
 * Name: ppd42ns_open
 *
 * Description:
 *   Standard character driver open method.
 *
 ****************************************************************************/
static int ppd42ns_open(FAR struct file *filep)
{
	int ret;
	FAR struct inode *inode = filep->f_inode;
	FAR struct ppd42ns_dev_s *priv = inode->i_private;

	ret = ppd42ns_takesem(&priv->exclsem);
	if (ret < 0) {
		/*
		 * A signal received while waiting for the last close
		 * operation.
		 */
		lldbg("ERROR: ppd42ns_takesem() failed: %d\n", ret);
		return ret;
	}

	/* ppd42ns driver allows only 1 instance */
	if (priv->crefs > 0) {
		lldbg("ERROR: ppd42ns driver is already opened.\n");
		goto errout_with_sem;
	}

	/* set reference count */
	priv->crefs = 1;

	/* enable the gpio pin interrupt */
	ret = priv->config->enable(priv->config, 1);
	if (ret < 0) {
		lldbg("ERROR: failed to enable the interrupt handler. (ret=%d)\n", ret);
		goto errout_with_sem;
	}

	/* set start time */
	PPD42NS_TIME_GET(priv->start_time);
	priv->lowpulse_start_time = priv->start_time;

	/* get initial signal */
	priv->old_signal = priv->config->read_gpio(priv->config);

	/* initialize lowpulseoccupancy */
	priv->lowpulseoccupancy = 0;

	ret = 0;

errout_with_sem:
	ppd42ns_givesem(&priv->exclsem);
	return ret;
}

/****************************************************************************
 * Name: ppd42ns_close
 *
 * Description:
 *   Standard character driver close method.
 *
 ****************************************************************************/
static int ppd42ns_close(FAR struct file *filep)
{
	int ret;
	FAR struct inode *inode = filep->f_inode;
	FAR struct ppd42ns_dev_s *priv = inode->i_private;

	/* Get exclusive access to the driver structure */
	ret = ppd42ns_takesem(&priv->exclsem);
	if (ret < 0) {
		lldbg("ERROR: ppd42ns_takesem() failed: %d\n", ret);
		return ret;
	}

	if (priv->crefs == 0) {
		lldbg("ERROR: ppd42ns driver is already closed.\n");
		goto errout_with_sem;
	}

	priv->crefs = 0;

	/* disable the gpio pin interrupt */
	ret = priv->config->enable(priv->config, 0);
	if (ret < 0) {
		lldbg("ERROR: failed to disable the interrupt handler. (ret=%d)\n", ret);
		goto errout_with_sem;
	}

	ret = 0;

errout_with_sem:
	ppd42ns_givesem(&priv->exclsem);
	return ret;
}

/****************************************************************************
 * Name: ppd42ns_read
 *
 * Description:
 *   Standard character driver read method.
 *
 ****************************************************************************/
static ssize_t ppd42ns_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	int ret;
	FAR struct inode *inode = filep->f_inode;
	FAR struct ppd42ns_dev_s *priv = inode->i_private;
	int signal;
	float ratio = 0.0;
	float concentration;
	struct timeval time;

	ret = ppd42ns_takesem(&priv->datasem);
	if (ret < 0) {
		lldbg("ERROR: ppd42ns_takesem() failed: %d\n", ret);
		return ret;
	}

	/* disable gpio pin interrupt */
	priv->config->enable(priv->config, 0);

	/* get gpio signal */
	signal = priv->config->read_gpio(priv->config);

	PPD42NS_TIME_GET(time);
	if (signal == GPIO_SIGNAL_LOW) {
		priv->lowpulseoccupancy += PPD42NS_TIME_DIFF_USEC(priv->lowpulse_start_time, time);
		priv->lowpulse_start_time = time;
	}

	if (priv->lowpulseoccupancy != 0) {
		ratio = (float)priv->lowpulseoccupancy / (float)PPD42NS_TIME_DIFF_USEC(priv->start_time, time) * 100.0;	/* percentage 0~100 */
		concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62;	/* using spec sheet curve */
	} else {
		concentration = 0.0;
	}

#ifdef CONFIG_SENSOR_PPD42NS_DEBUG
	lldbg("lowpulse_time=%d, total_time=%d, ratio=%.2f, concentration=%.2f \n", priv->lowpulseoccupancy, PPD42NS_TIME_DIFF_USEC(priv->start_time, time), ratio, concentration);
#endif

	priv->start_time = time;
	priv->lowpulseoccupancy = 0;
	priv->old_signal = signal;

	/* enable gpio pin interrupt */
	priv->config->enable(priv->config, 1);

	memcpy(buffer, &concentration, sizeof(float));

	ppd42ns_givesem(&priv->datasem);

	return sizeof(float);
}

/****************************************************************************
 * Public Function
 ****************************************************************************/

/****************************************************************************
 * Name: ppd42ns_register
 *
 * Description:
 *  This function will register ppd42ns dust sensor driver as /dev/dustN where N
 *  is the minor device number
 *
 * Input Parameters:
 *   devname  - The full path to the driver to register. E.g., "/dev/dust0"
 *   config      - configuration for the ppd42ns driver.
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/
int ppd42ns_register(FAR const char *devname, FAR struct ppd42ns_config_s *config)
{
	int ret;
	struct ppd42ns_dev_s *priv = &g_ppd42ns_priv;

	/* validate ppd42ns config */
	if (config == NULL || config->read_gpio == NULL || config->attach == NULL || config->enable == NULL) {
		ret = EINVAL;
		lldbg("ERROR: invalid ppd42ns config\n");
		return ret;
	}

	/* set ppd42ns config */
	priv->config = config;

	/* reference count is set to 0 */
	priv->crefs = 0;

	/* init semaphore */
	sem_init(&priv->exclsem, 0, 1);
	sem_init(&priv->datasem, 0, 1);

	/* attach the interrupt handler */
	ret = priv->config->attach(priv->config, ppd42ns_interrupt_handler, (FAR void *)priv);
	if (ret < 0) {
		lldbg("ERROR: failed to attach the interrupt handler. (ret=%d)\n", ret);
		sem_destroy(&priv->exclsem);
		sem_destroy(&priv->datasem);
		return ret;
	}

	/* register the character device driver */
	ret = register_driver(devname, &g_ppd42ns_fops, 0666, priv);
	if (ret < 0) {
		lldbg("ERROR: failed to register driver %s. (ret=%d)\n", devname, ret);
		sem_destroy(&priv->exclsem);
		sem_destroy(&priv->datasem);
		return ret;
	}

	lldbg("Registered %s\n", devname);

	return 0;
}
