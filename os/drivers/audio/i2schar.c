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
 * drivers/audio/i2schar.c
 *
 * This is a simple character driver for testing I2C.  It is not an audio
 * driver but does conform to some of the buffer management heuristics of an
 * audio driver.  It is not suitable for use in any real driver application
 * in its current form.
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/common_logs/common_logs.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

#ifndef CONFIG_AUDIO_I2SCHAR_RXTIMEOUT
#define CONFIG_AUDIO_I2SCHAR_RXTIMEOUT 0
#endif

#ifndef CONFIG_AUDIO_I2SCHAR_TXTIMEOUT
#define CONFIG_AUDIO_I2SCHAR_TXTIMEOUT 0
#endif

/* Device naming ************************************************************/
#define DEVNAME_FMT    "/dev/i2schar%d"
#define DEVNAME_FMTLEN (12 + 3 + 1)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct i2schar_dev_s {
	FAR struct i2s_dev_s *i2s;	/* The lower half i2s driver */
	sem_t exclsem;				/* Assures mutually exclusive access */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* I2S callback function */

static void i2schar_rxcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result);
static void i2schar_txcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result);

/* Character driver methods */

static ssize_t i2schar_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t i2schar_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations i2schar_fops = {
	NULL,						/* open  */
	NULL,						/* close */
	i2schar_read,				/* read  */
	i2schar_write,				/* write */
	NULL,						/* seek  */
	NULL,						/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	NULL,						/* poll  */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: i2schar_rxcallback
 *
 * Description:
 *   I2S RX transfer complete callback.
 *
 *   NOTE: In this test driver, the RX is simply dumped in the bit bucket.
 *   You would not do this in a real application. You would return the
 *   received data to the caller via some IPC.
 *
 *   Also, the test buffer is simply freed.  This will work if this driver
 *   has the sole reference to buffer; in that case the buffer will be freed.
 *   Otherwise -- memory leak!  A more efficient design would recyle the
 *   audio buffers.
 *
 ****************************************************************************/

static void i2schar_rxcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
	FAR struct i2schar_dev_s *priv = (FAR struct i2schar_dev_s *)arg;

	DEBUGASSERT(priv && apb);
	llvdbg("apb=%p nbytes=%d result=%d\n", apb, apb->nbytes, result);

	/* REVISIT: If you want this to actually do something other than
	 * test I2S data transfer, then this is the point where you would
	 * want to pass the received I2S to some application.
	 */

	/* Release our reference to the audio buffer. Hopefully it will be freed
	 * now.
	 */

	llvdbg("Freeing apb=%p crefs=%d\n", apb, apb->crefs);
	apb_free(apb);
}

/****************************************************************************
 * Name: i2schar_txcallback
 *
 * Description:
 *   I2S TX transfer complete callback
 *
 *   NOTE: The test buffer is simply freed.  This will work if this driver
 *   has the sole reference to buffer; in that case the buffer will be freed.
 *   Otherwise -- memory leak!  A more efficient design would recyle the
 *   audio buffers.
 *
 ****************************************************************************/

static void i2schar_txcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
	FAR struct i2schar_dev_s *priv = (FAR struct i2schar_dev_s *)arg;

	DEBUGASSERT(priv && apb);
	llvdbg("apb=%p nbytes=%d result=%d\n", apb, apb->nbytes, result);

	/* REVISIT: If you want this to actually do something other than
	 * test I2S data transfer, then this is the point where you would
	 * want to let some application know that the transfer has complete.
	 */

	/* Release our reference to the audio buffer.  Hopefully it will be freed
	 * now.
	 */

	llvdbg("Freeing apb=%p crefs=%d\n", apb, apb->crefs);
	apb_free(apb);
}

/****************************************************************************
 * Name: i2schar_read
 *
 * Description:
 *   Standard character driver read method
 *
 ****************************************************************************/

static ssize_t i2schar_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	FAR struct inode *inode;
	FAR struct i2schar_dev_s *priv;
	FAR struct ap_buffer_s *apb;
	size_t nbytes;
	int ret;

	llvdbg("buffer=%p buflen=%d\n", buffer, (int)buflen);

	/* Get our private data structure */

	DEBUGASSERT(filep && buffer);

	inode = filep->f_inode;
	DEBUGASSERT(inode);

	priv = (FAR struct i2schar_dev_s *)inode->i_private;
	DEBUGASSERT(priv);

	/* Verify that the buffer refers to one, correctly sized audio buffer */

	DEBUGASSERT(buflen >= sizeof(struct ap_buffer_s));

	apb = (FAR struct ap_buffer_s *)buffer;
	nbytes = apb->nmaxbytes;
	DEBUGASSERT(buflen >= (sizeof(struct ap_buffer_s) + nbytes));

	/* Add a reference to the audio buffer */

	apb_reference(apb);

	/* Exclusive access will be provided by I2S driver */

	/* Give the buffer to the I2S driver */

	ret = I2S_RECEIVE(priv->i2s, apb, i2schar_rxcallback, priv, CONFIG_AUDIO_I2SCHAR_RXTIMEOUT);
	if (ret < 0) {
		lldbg("ERROR: I2S_RECEIVE returned: %d\n", ret);
		goto errout_with_reference;
	}

	/* Lie to the caller and tell them that all of the bytes have been
	 * received. Actually it will be receiver in callback function.
	 */

	return sizeof(struct ap_buffer_s) + nbytes;

errout_with_reference:
	apb_free(apb);
	return ret;
}

/****************************************************************************
 * Name: i2schar_write
 *
 * Description:
 *   Standard character driver write method
 *
 ****************************************************************************/

static ssize_t i2schar_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	FAR struct inode *inode;
	FAR struct i2schar_dev_s *priv;
	FAR struct ap_buffer_s *apb;
	size_t nbytes;
	int ret;

	llvdbg("buffer=%p buflen=%d\n", buffer, (int)buflen);

	/* Get our private data structure */

	DEBUGASSERT(filep && buffer);

	inode = filep->f_inode;
	DEBUGASSERT(inode);

	priv = (FAR struct i2schar_dev_s *)inode->i_private;
	DEBUGASSERT(priv);

	/* Verify that the buffer refers to one, correctly sized audio buffer */

	DEBUGASSERT(buflen >= sizeof(struct ap_buffer_s));

	apb = (FAR struct ap_buffer_s *)buffer;
	nbytes = apb->nmaxbytes;
	DEBUGASSERT(buflen >= (sizeof(struct ap_buffer_s) + nbytes));

	/* Add a reference to the audio buffer */

	apb_reference(apb);

	/* Exclusive access will be provided by I2S driver */

	/* Give the audio buffer to the I2S driver */

	ret = I2S_SEND(priv->i2s, apb, i2schar_txcallback, priv, CONFIG_AUDIO_I2SCHAR_TXTIMEOUT);
	if (ret < 0) {
			lldbg("%s %d \n", clog_message_str[CMN_LOG_INVALID_VAL], ret);
		goto errout_with_reference;
	}

	/* Lie to the caller and tell them that all of the bytes have been
	 * sent. Actual transfer completion will be in callback function.
	 */

	return sizeof(struct ap_buffer_s) + nbytes;

errout_with_reference:
	apb_free(apb);
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: i2schar_register
 *
 * Description:
 *   Create and register the I2S character driver.
 *
 *   The I2S character driver is a simple character driver that supports I2S
 *   transfers via a read() and write().  The intent of this driver is to
 *   support I2S testing.  It is not an audio driver but does conform to some
 *   of the buffer management heuristics of an audio driver.  It is not
 *   suitable for use in any real driver application in its current form.
 *
 * Input Parameters:
 *   i2s - An instance of the lower half I2S driver
 *   minor - The device minor number.  The I2S character device will be
 *     registers as /dev/i2scharN where N is the minor number
 *
 * Returned Value:
 *   OK if the driver was successfully register; A negated errno value is
 *   returned on any failure.
 *
 ****************************************************************************/

int i2schar_register(FAR struct i2s_dev_s *i2s, int minor)
{
	FAR struct i2schar_dev_s *priv;
	char devname[DEVNAME_FMTLEN];
	int ret;

	/* Sanity check */

	DEBUGASSERT(i2s != NULL && (unsigned)minor < 1000);

	/* Allocate a I2S character device structure */

	priv = (FAR struct i2schar_dev_s *)kmm_zalloc(sizeof(struct i2schar_dev_s));
	if (priv == NULL) {
		return -ENOMEM;
	}

	/* Initialize the I2S character device structure */

	priv->i2s = i2s;

	/* Create the character device name */

	snprintf(devname, DEVNAME_FMTLEN, DEVNAME_FMT, minor);
	ret = register_driver(devname, &i2schar_fops, 0666, priv);
	if (ret < 0) {
		/* Free the device structure if we failed to create the character
		 * device.
		 */

		kmm_free(priv);
	}

	/* Return the result of the registration */

	return OK;
}
