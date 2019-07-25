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
#include <semaphore.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/otp.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define OTP_DEV_PATH "/dev/otp"

#define OU_LOCK(priv)										\
	do {													\
		int oulock_ret = sem_wait(&priv->ou_exclsem);		\
		if (oulock_ret < 0) {								\
			lldbg("ERROR: ou_lock failed: %d\n", oulock_ret);	\
			return oulock_ret;										\
		}													\
	} while (0)

#define OU_UNLOCK(priv)									\
	do {													\
		int oulock_ret = sem_post(&priv->ou_exclsem);		\
		if (oulock_ret < 0) {								\
			lldbg("ERROR: ou unlock failed: %d\n", oulock_ret);	\
			return oulock_ret;										\
		}													\
	} while (0)

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int     otp_open(struct file *filep);
static int     otp_close(struct file *filep);
static ssize_t otp_read(struct file *filep, char *buffer, size_t buflen);
static int     otp_ioctl(struct file *filep, int cmd, unsigned long arg);

static int otp_uh_read(struct otp_upperhalf_s *upper, uint32_t addr, uint8_t *data, uint32_t *length);
static int otp_uh_write(struct otp_upperhalf_s *upper, uint32_t addr, uint8_t *data, uint32_t length);
static int otp_uh_lock(struct otp_upperhalf_s *upper);


/****************************************************************************
 * Private Variables
 ****************************************************************************/
static const struct file_operations g_otp_ops = {
	otp_open,	/* open	 */
	otp_close,	/* close */
	otp_read,	/* read	 */
	NULL,		/* write */
	NULL,		/* seek	 */
	otp_ioctl,	/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	NULL	/* poll	 */
#endif
};

static const struct otp_upperhalf_ops_s g_uh_ops = {
	otp_uh_write,
	otp_uh_read,
	otp_uh_lock,
};

static struct otp_upperhalf_s *g_otp_dev = NULL;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

int otp_uh_read(struct otp_upperhalf_s *upper, uint32_t addr, uint8_t *data, uint32_t *length)
{
	FAR struct otp_lowerhalf_s *lower = upper->ou_lower;
	OU_LOCK(upper);

	// setting address should be updated
	int res = lower->ops->read(lower, 0, data, length);
	if (res < 0) {
		OU_UNLOCK(upper);
		return ENOTTY;
	}
	OU_UNLOCK(upper);

	return *length;
}

int otp_uh_write(struct otp_upperhalf_s *upper, uint32_t addr, uint8_t *data, uint32_t length)
{
	FAR struct otp_lowerhalf_s *lower = upper->ou_lower;

	// setting address should be updated
	OU_LOCK(upper);
	int res = lower->ops->write(lower, addr, data, length);
	if (res < 0) {
		OU_UNLOCK(upper);
		return ENOTTY;
	}
	OU_UNLOCK(upper);
	return res;
}

int otp_uh_lock(struct otp_upperhalf_s *upper)
{
	FAR struct otp_lowerhalf_s *lower = upper->ou_lower;
	OU_LOCK(upper);
	int res = lower->ops->lock(lower);
	if (res < 0) {
		OU_UNLOCK(upper);
		return -1;
	}
	OU_UNLOCK(upper);
	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/


/****************************************************************************
 * Name: otp_read
 *
 * Description:
 *    This function is automatically called when you handle the read() API in
 *    File system.
 *
 ****************************************************************************/
static ssize_t otp_read(struct file *filep, char *buffer,
						size_t buflen)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct otp_upperhalf_s *priv = inode->i_private;
	int res = OTP_READ(priv, 0, (uint8_t *)buffer, &buflen);
	if (res < 0) {
		lldbg("Fail to read\n");
		return ENOTTY;
	}
	return buflen;
}

/****************************************************************************
 * Name: otp_ioctl
 *
 * Description:
 *    This function is allow you to control the event.
 *
 ****************************************************************************/
static int otp_ioctl(struct file *filep, int cmd, unsigned long arg)
{
	return 0;
}


/****************************************************************************
 * Name: otp_close
 *
 * Description:
 *   This routine is called when the otp gets closed.
 *
 ****************************************************************************/
static int otp_close(struct file *filep)
{
	return 0;
}

/*****************************************************************************
 * Name: otp_open
 *
 * Description:
 *   This routine is called whenever otp is opened.
 *
 ****************************************************************************/
static int otp_open(struct file *filep)
{
	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: otp_register
 *
 * Description:
 *   Register OTP device.
 *
 ****************************************************************************/
int otp_register(struct otp_lowerhalf_s *lower)
{
	int ret;
	FAR struct otp_upperhalf_s *priv;

	DEBUGASSERT(lower);

	if (g_otp_dev) {
		return -1;
	}

	/* Allocate a new GPIO driver instance */
	priv = (FAR struct otp_upperhalf_s *)
		kmm_zalloc(sizeof(struct otp_upperhalf_s));
	if (!priv) {
		lldbg("ERROR: Failed to allocate device structure\n");
		return -ENOMEM;
	}

	sem_init(&priv->ou_exclsem, 0, 1);
	priv->ops = &g_uh_ops;
	/* Initiailize the new GPIO driver instance */
	priv->ou_lower = lower;
	lower->parent = priv;

	/* And register the GPIO driver */
	ret = register_driver(OTP_DEV_PATH, &g_otp_ops, 0444, priv);
	if (ret < 0) {
		lldbg("ERROR: register driver failed: %d\n", ret);
		goto errout_with_priv;
	}
	g_otp_dev = priv;

	return OK;

errout_with_priv:
	sem_destroy(&priv->ou_exclsem);
	kmm_free(priv);
	return ret;
}

/****************************************************************************
 * Name: otp_unregister
 *
 * Description:
 *   unregister OTP device.
 *
 ****************************************************************************/
int otp_unregister(void)
{
	if (!g_otp_dev) {
		lldbg("ERROR: no registered device\n");
		return -1;
	}

	unregister_driver(OTP_DEV_PATH);

	sem_destroy(&g_otp_dev->ou_exclsem);
	kmm_free(g_otp_dev);
	g_otp_dev = NULL;

	return 0;
}

/****************************************************************************
 * Name: get_otp_dev
 *
 * Description:
 *   Get otp lower device.
 *   Programs(SE) in a kernel space can access otp by this function
 *
 ****************************************************************************/
struct otp_upperhalf_s *get_otp_dev(void)
{
	if (!g_otp_dev) {
		lldbg("ERROR: no registered device\n");
		return NULL;
	}
	return g_otp_dev;
}
