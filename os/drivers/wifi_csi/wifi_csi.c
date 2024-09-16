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
#include <sys/stat.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>

#include <tinyara/wifi_csi/wifi_csi.h>
#include <tinyara/wifi_csi/wifi_csi_struct.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Debug ********************************************************************/
/* Non-standard debug that may be enabled just for testing Wificsi */

#ifndef WIFI_CSI_MAX_DEVICE_PATH
#define WIFI_CSI_MAX_DEVICE_PATH 32
#endif
#ifndef CONFIG_WIFICSI_DATA_READY_PRIO
#define CONFIG_WIFICSI_DATA_READY_PRIO 1
#endif

/****************************************************************************
 * Private Type Definitions
 ****************************************************************************/

/* This structure describes the state of the upper half driver */

struct wifi_csi_upperhalf_s {
	uint8_t crefs;				/* The number of times the device has been opened */
	sem_t exclsem;				/* Supports mutual exclusion */
	FAR struct wifi_csi_lowerhalf_s *dev;	/* lower-half state */
	mqd_t usermq;				/* User mode app's message queue */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int wifi_csi_open(FAR struct file *filep);
static int wifi_csi_close(FAR struct file *filep);
static ssize_t wifi_csi_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static int wifi_csi_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

static void wifi_csi_callback(FAR void *handle, uint16_t reason, char *buff, uint32_t csi_data_len);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_wifi_csiops = {
	wifi_csi_open,					/* open */
	wifi_csi_close,					/* close */
	wifi_csi_read,					/* read */
	0,	       						/* write */
	0,								/* seek */
	wifi_csi_ioctl					/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0				    			/* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/************************************************************************************
 * Name: wifi_csi_open
 *
 * Description:
 *   This function is called whenever the Wificsi device is opened.
 *
 ************************************************************************************/

static int wifi_csi_open(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct wifi_csi_upperhalf_s *upper = inode->i_private;
	uint8_t tmp;
	int ret;

	csivdbg("crefs: %d\n", upper->crefs);

	/* Get exclusive access to the device structures */
	ret = sem_wait(&upper->exclsem);
	if (ret < 0) {
		ret = -errno;
		goto errout;
	}

	/* Increment the count of references to the device.  If this the first
	 * time that the driver has been opened for this device, then initialize
	 * the device.
	 */
	tmp = upper->crefs + 1;
	if (tmp == 0) {
		/* More than 255 opens; uint8_t overflows to zero */

		ret = -EMFILE;
		goto errout_with_sem;
	}

	/* Save the new open count on success */
	upper->crefs = tmp;

	/* Initialize usermq only when it is the first open */
	if (upper->crefs == 1) {
		upper->usermq = NULL;
	}
	ret = OK;

errout_with_sem:
	sem_post(&upper->exclsem);

errout:
	return ret;
}

/************************************************************************************
 * Name: wifi_csi_close
 *
 * Description:
 *   This function is called when the Wificsi device is closed.
 *
 ************************************************************************************/

static int wifi_csi_close(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct wifi_csi_upperhalf_s *upper = inode->i_private;
	int ret = OK;

	csivdbg("crefs: %d\n", upper->crefs);

	/* Get exclusive access to the device structures */
	ret = sem_wait(&upper->exclsem);
	if (ret < 0) {
		ret = -errno;
		goto errout;
	}

	/* Decrement the references to the driver.  If the reference count will
	 * decrement to 0, then uninitialize the driver.
	 */
	if (upper->crefs > 1) {
		upper->crefs--;
	} else {
		/* There are no more references to the port */
		upper->crefs = 0;
	}

//errout_with_sem:
	sem_post(&upper->exclsem);

errout:
	return ret;
}

/************************************************************************************
 * Name: wifi_csi_read
 *
 * Description:
 *   A dummy read method. Some hardwares might support read.
 *
 ************************************************************************************/
static ssize_t wifi_csi_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct wifi_csi_upperhalf_s *upper = inode->i_private;
	FAR struct wifi_csi_lowerhalf_s *lower = upper->dev;

	if (lower->ops->read != NULL) {
		return lower->ops->read(lower, buffer, buflen);
	}
	return 0;
}

/************************************************************************************
 * Name: wifi_csi_ioctl
 *
 * Description:
 *   The standard ioctl method.  This is where ALL of the Wificsi work is done.
 *
 ************************************************************************************/

static int wifi_csi_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct wifi_csi_upperhalf_s *upper = inode->i_private;
	FAR struct wifi_csi_lowerhalf_s *lower = upper->dev;
	int ret;
	csivdbg("cmd: %d arg: %ld\n", cmd, arg);
	
	/* Get exclusive access to the device structures */
	ret = sem_wait(&upper->exclsem);
	if (ret < 0) {
		return ret;
	}

	/* Handle built-in ioctl commands */
	switch (cmd) {

	case CSIIOC_REGISTERMQ: {
		csivdbg("CSIIOC_REGISTERMQ\n");
		if (!arg) {
			csidbg("ERROR: Invalid config arg\n");
			ret = -EINVAL;
			break;
		}
		upper->usermq = (mqd_t) arg;
		ret = OK;
	}
	break;

	case CSIIOC_UNREGISTERMQ: {
		csivdbg("CSIIOC_UNREGISTERMQ\n");
		ret = mq_close(upper->usermq);	/* Close the message queue */
		if (ret == -1) {
			csidbg("ERROR: mq_close failed\n");
			break;
		}
		upper->usermq = NULL;
	}
	break;

	case CSIIOC_GET_MAC_ADDR: {
		csivdbg("CSIIOC_GET_MAC_ADDR\n");
		if (!arg) {
			csidbg("ERROR: Invalid mac info arg\n");
			ret = -EINVAL;
			break;
		}
		csifw_mac_info *mac_info = (csifw_mac_info*)arg;
		if (lower->ops->getmacaddr != NULL) {
			ret = lower->ops->getmacaddr(lower, mac_info->mac_addr);
		} else {
			csidbg("ERROR: Unsupported operation: get mac\n");
			ret = -ENOSYS;
		}
	} 
	break;

	case CSIIOC_GET_DATA: {
		if (!arg) {
			csidbg("ERROR: invalid buffer arg\n");
			ret = -EINVAL;
			break;
		}
		csi_driver_buffer_args_t *buf_arg = (csi_driver_buffer_args_t*)arg;
		if (!buf_arg->buffer) {
			csidbg("ERROR: invalid buffer ptr\n");
			ret = -EINVAL;
			break;
		}
		if (lower->ops->getcsidata != NULL) {
			ret = lower->ops->getcsidata(lower, buf_arg->buffer, buf_arg->buflen);
		} else {
			csidbg("ERROR: Unsupported operation: getcsidata\n");
			ret = -ENOSYS;
		}
	}
	break;

	/* Any unrecognized IOCTL commands might be platform-specific ioctl commands */
	default: {
		if (lower->ops->ioctl != NULL) {
			csivdbg("Forwarding ioctl cmd: %d arg: %ld\n", cmd, arg);
			ret = lower->ops->ioctl(lower, cmd, arg);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	}
	sem_post(&upper->exclsem);
	return ret;
}

/****************************************************************************
 * Name: wifi_csi_error_handler
 *
 * Description:
 *   Send an CSI_CALLBACK_ERROR message to the client to indicate that the
 *   an error has occured.  The lower-half driver initiates this
 *   call via its callback pointer to our upper-half driver.
 *
 ****************************************************************************/

static inline void wifi_csi_error_handler(FAR struct wifi_csi_upperhalf_s *upper, uint16_t reason)
{
	struct wifi_csi_msg_s msg;

	/* Send a error message to the user if a message queue is registered */
	if (upper->usermq != NULL) {
		/* We are always sending CSI_MSG_ERROR error msg since it is the
		only type of error that we handle now. If new error scenario
		is required, then we need to handle here */
		msg.msgId = CSI_MSG_ERROR;
		mq_send(upper->usermq, (FAR const char *)&msg, sizeof(msg), MQ_PRIO_MAX);
	}
}

/****************************************************************************
 * Name: wifi_csi_data_ready
 *
 * Description:
 *   Send an CSI_CALLBACK_DATA_READY message to the client to indicate that the
 *   wificsi data is ready.  The lower-half driver initiates this
 *   call via its callback pointer to our upper-half driver.
 *
 ****************************************************************************/
static inline void wifi_csi_data_ready(FAR struct wifi_csi_upperhalf_s *upper, char *buff, uint32_t csi_data_len)
{
	struct wifi_csi_msg_s msg;
	/* Send a data ready message to the user if a message queue is registered */
	if (upper->usermq != NULL) {
		msg.msgId = CSI_MSG_DATA_READY_CB;
		msg.data_len = csi_data_len;
		int ret = mq_send(upper->usermq, (FAR const char *)&msg, sizeof(msg), CONFIG_WIFICSI_DATA_READY_PRIO);
		if (ret != 0) {
			csidbg("ERROR: mq_send failed, errno: %d, ret: %d\n", errno, ret);
		} 
	}
}

/****************************************************************************
 * Name: wifi_csi_callback
 *
 * Description:
 *   Provides a callback interface for lower-half drivers to call to the
 *   upper-half for buffer dequeueing, error reporting, etc.
 *
 * Input parameters:
 *   priv - Private context data owned by the upper-half
 *   reason - The reason code for the callback
 *   apb - A pointer to the previsously enqueued ap_buffer_s
 *   status - Status information associated with the callback
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

static void wifi_csi_callback(FAR void *handle, uint16_t reason, char *buff, uint32_t csi_data_len)
{
	FAR struct wifi_csi_upperhalf_s *upper = (FAR struct wifi_csi_upperhalf_s *)handle;

	/* Perform operation based on reason code */
	switch (reason) {

		/* Lower-half I/O error occurred */
		case CSI_CALLBACK_ERROR: {
			wifi_csi_error_handler(upper, reason);
		}
		break;

		/* Lower-half driver has csi data ready */
		case CSI_CALLBACK_DATA_READY: {
			/* Send a data ready message to the user if a message queue is registered */
			wifi_csi_data_ready(upper, buff, csi_data_len);
		}
		break;

		default: {
			csidbg("ERROR: Unknown callback reason code %d\n", reason);
			break;
		}
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: wifi_csi_register
 *
 * Description:
 *   This function binds an instance of a "lower half" wificsi driver with the
 *   "upper half" Wificsi device and registers that device so that can be used
 *   by application code.
 *
 *   When this function is called, the "lower half" driver should be in the
 *   reset state (as if the shutdown() method had already been called).
 *
 * Input parameters:
 *   path - The full path to the driver to be registers in the NuttX pseudo-
 *     filesystem.  The recommended convention is to name Wificsi drivers
 *     based on the function they provide, such as "/dev/pcm0", "/dev/mp31",
 *     etc.
 *   dev - A pointer to an instance of lower half wificsi driver.  This instance
 *     is bound to the Wificsi driver and must persists as long as the driver
 *     persists.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

int wifi_csi_register(FAR const char *name, FAR struct wifi_csi_lowerhalf_s *dev)
{
	FAR struct wifi_csi_upperhalf_s *upper;
	char* path;
	static bool dev_wifi_csi_created = false;
	int ret;
#ifndef CONFIG_WIFICSI_CUSTOM_DEV_PATH
	path = "/dev/wificsi";
#endif
	path = CONFIG_WIFICSI_CUSTOM_DEV_PATH;

	/* Allocate the upper-half data structure */
	upper = (FAR struct wifi_csi_upperhalf_s *)kmm_zalloc(sizeof(struct wifi_csi_upperhalf_s));
	if (!upper) {
		csidbg("ERROR: Allocation failed\n");
		return -ENOMEM;
	}

	/* Initialize the Wificsi device structure (it was already zeroed by kmm_zalloc()) */
	sem_init(&upper->exclsem, 0, 1);
	upper->dev = dev;
	
	/* Give the lower-half a context to the upper half */
	dev->upper = wifi_csi_callback;
	dev->priv = upper;

	csidbg("Registering %s\n", path);
	return register_driver(path, &g_wifi_csiops, 0666, upper);
}

