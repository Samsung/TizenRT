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
 * audio/audio.c
 *
 *   Copyright (C) 2013 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
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
#include <tinyara/arch.h>
#include <tinyara/audio/audio.h>

#include <arch/irq.h>

#ifdef CONFIG_AUDIO

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Debug ********************************************************************/
/* Non-standard debug that may be enabled just for testing Audio */

#ifndef AUDIO_MAX_DEVICE_PATH
#define AUDIO_MAX_DEVICE_PATH 32
#endif

#ifndef CONFIG_AUDIO_BUFFER_DEQUEUE_PRIO
#define CONFIG_AUDIO_BUFFER_DEQUEUE_PRIO  1
#endif

/****************************************************************************
 * Private Type Definitions
 ****************************************************************************/

/* This structure describes the state of the upper half driver */

struct audio_upperhalf_s {
	uint8_t crefs;				/* The number of times the device has been opened */
	volatile bool started;		/* True: playback is active */
	sem_t exclsem;				/* Supports mutual exclusion */
	FAR struct audio_lowerhalf_s *dev;	/* lower-half state */
	mqd_t usermq;				/* User mode app's message queue */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int audio_open(FAR struct file *filep);
static int audio_close(FAR struct file *filep);
static ssize_t audio_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t audio_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static int audio_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int audio_start(FAR struct audio_upperhalf_s *upper, FAR void *session);
static void audio_callback(FAR void *priv, uint16_t reason, FAR struct ap_buffer_s *apb, uint16_t status, FAR void *session);
#else
static int audio_start(FAR struct audio_upperhalf_s *upper);
static void audio_callback(FAR void *priv, uint16_t reason, FAR struct ap_buffer_s *apb, uint16_t status);
#endif							/* CONFIG_AUDIO_MULTI_SESSION */

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_audioops = {
	audio_open,					/* open */
	audio_close,				/* close */
	audio_read,					/* read */
	audio_write,				/* write */
	0,							/* seek */
	audio_ioctl					/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0						/* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/************************************************************************************
 * Name: audio_open
 *
 * Description:
 *   This function is called whenever the Audio device is opened.
 *
 ************************************************************************************/

static int audio_open(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct audio_upperhalf_s *upper = inode->i_private;
	uint8_t tmp;
	int ret;

	audvdbg("crefs: %d\n", upper->crefs);

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
 * Name: audio_close
 *
 * Description:
 *   This function is called when the Audio device is closed.
 *
 ************************************************************************************/

static int audio_close(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct audio_upperhalf_s *upper = inode->i_private;
	int ret = OK;

	audvdbg("crefs: %d\n", upper->crefs);

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
		FAR struct audio_lowerhalf_s *lower = upper->dev;

		/* There are no more references to the port */

		upper->crefs = 0;

		/* Disable the Audio device */

		DEBUGASSERT(lower->ops->shutdown != NULL);
		audvdbg("calling shutdown: %d\n");

		ret = lower->ops->shutdown(lower);
	}

//errout_with_sem:
	sem_post(&upper->exclsem);

errout:
	return ret;
}

/************************************************************************************
 * Name: audio_read
 *
 * Description:
 *   A dummy read method.  This is provided only to satsify the VFS layer.
 *
 ************************************************************************************/

static ssize_t audio_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct audio_upperhalf_s *upper = inode->i_private;
	FAR struct audio_lowerhalf_s *lower = upper->dev;

	/* TODO: Should we check permissions here? */

	/* Audio read operations get passed directly to the lower-level */

	if (lower->ops->read != NULL) {
		return lower->ops->read(lower, buffer, buflen);
	}

	return 0;
}

/************************************************************************************
 * Name: audio_write
 *
 * Description:
 *   A dummy write method.  This is provided only to satsify the VFS layer.
 *
 ************************************************************************************/

static ssize_t audio_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct audio_upperhalf_s *upper = inode->i_private;
	FAR struct audio_lowerhalf_s *lower = upper->dev;
	/* TODO: Should we check permissions here? */

	/* Audio write operations get passed directly to the lower-level */

	if (lower->ops->write != NULL) {
		return lower->ops->write(lower, buffer, buflen);
	}

	return 0;
}

/************************************************************************************
 * Name: audio_start
 *
 * Description:
 *   Handle the AUDIOIOC_START ioctl command
 *
 ************************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int audio_start(FAR struct audio_upperhalf_s *upper, FAR void *session)
#else
static int audio_start(FAR struct audio_upperhalf_s *upper)
#endif
{

	if (!upper) {
		return -EINVAL;
	}

	FAR struct audio_lowerhalf_s *lower = upper->dev;

	if (lower->ops->start == NULL) {
		return -ENOSYS;
	}

	int ret = OK;

	/* Verify that the Audio is not already running */

	if (!upper->started) {
		/* Invoke the bottom half method to start the audio stream */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		ret = lower->ops->start(lower, session);
#else
		ret = lower->ops->start(lower);
#endif

		/* A return value of zero means that the audio stream was started
		 * successfully.
		 */

		if (ret == OK) {
			/* Indicate that the audio stream has started */

			upper->started = true;
		}
	}

	return ret;
}

/************************************************************************************
 * Name: audio_ioctl
 *
 * Description:
 *   The standard ioctl method.  This is where ALL of the Audio work is done.
 *
 ************************************************************************************/

static int audio_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct audio_upperhalf_s *upper = inode->i_private;
	FAR struct audio_lowerhalf_s *lower = upper->dev;
	FAR struct audio_buf_desc_s *bufdesc;
#ifdef CONFIG_AUDIO_MULTI_SESSION
	FAR void *session;
#endif
	int ret;

	audvdbg("cmd: %d arg: %ld\n", cmd, arg);

	/* Get exclusive access to the device structures */

	ret = sem_wait(&upper->exclsem);
	if (ret < 0) {
		return ret;
	}

	/* Handle built-in ioctl commands */

	switch (cmd) {
	/* AUDIOIOC_GETCAPS - Get the audio device capabilities.
	 *
	 *   ioctl argument:  A pointer to the audio_caps_s structure.
	 */

	case AUDIOIOC_GETCAPS: {

		if (lower->ops->getcaps != NULL) {

			FAR struct audio_caps_s *caps = (FAR struct audio_caps_s *)((uintptr_t) arg);

			audvdbg("AUDIOIOC_GETCAPS: Device=%d\n", caps->ac_type);


			/* Call the lower-half driver capabilities handler */

			ret = lower->ops->getcaps(lower, caps->ac_type, caps);
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	case AUDIOIOC_CONFIGURE: {

		if (lower->ops->configure != NULL) {

			FAR const struct audio_caps_desc_s *caps = (FAR const struct audio_caps_desc_s *)((uintptr_t) arg);

			audvdbg("AUDIOIOC_CONFIGURE: Device=%d\n", caps->caps.ac_type);

			/* Call the lower-half driver configure handler */

#ifdef CONFIG_AUDIO_MULTI_SESSION
			ret = lower->ops->configure(lower, caps->session, &caps->caps);
#else
			ret = lower->ops->configure(lower, &caps->caps);
#endif
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	case AUDIOIOC_SHUTDOWN: {

		if (lower->ops->shutdown != NULL) {

			audvdbg("AUDIOIOC_SHUTDOWN\n");

			/* Call the lower-half driver initialize handler */
			ret = lower->ops->shutdown(lower);
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	/* AUDIOIOC_START - Start the audio stream.  The AUDIOIOC_SETCHARACTERISTICS
	 *   command must have previously been sent.
	 *
	 *   ioctl argument:  Audio session
	 */

	case AUDIOIOC_START: {

		if (lower->ops->start != NULL) {
			audvdbg("AUDIOIOC_START\n");

			/* Start the audio stream */

#ifdef CONFIG_AUDIO_MULTI_SESSION
			session = (FAR void *)arg;
			ret = audio_start(upper, session);
#else
			ret = audio_start(upper);
#endif
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	/* AUDIOIOC_STOP - Stop the audio stream.
	 *
	 *   ioctl argument:  Audio session
	 */

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	case AUDIOIOC_STOP: {

		if (lower->ops->stop != NULL) {
			audvdbg("AUDIOIOC_STOP\n");

			if (upper->started) {
#ifdef CONFIG_AUDIO_MULTI_SESSION
				session = (FAR void *)arg;
				ret = lower->ops->stop(lower, session);
#else
				ret = lower->ops->stop(lower);
#endif
				upper->started = false;
			}
		} else {
			ret = -ENOSYS;
		}
	}
	break;
#endif							/* CONFIG_AUDIO_EXCLUDE_STOP */

	/* AUDIOIOC_PAUSE - Pause the audio stream.
	 *
	 *   ioctl argument:  Audio session
	 */

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME

	case AUDIOIOC_PAUSE: {
		if (lower->ops->pause != NULL) {

			audvdbg("AUDIOIOC_PAUSE\n");

			if (upper->started) {
#ifdef CONFIG_AUDIO_MULTI_SESSION
				session = (FAR void *)arg;
				ret = lower->ops->pause(lower, session);
#else
				ret = lower->ops->pause(lower);
#endif
			}
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	/* AUDIOIOC_RESUME - Resume the audio stream.
	 *
	 *   ioctl argument:  Audio session
	 */

	case AUDIOIOC_RESUME: {

		if (lower->ops->resume != NULL) {
			audvdbg("AUDIOIOC_RESUME\n");

			if (upper->started) {
#ifdef CONFIG_AUDIO_MULTI_SESSION
				session = (FAR void *)arg;
				ret = lower->ops->resume(lower, session);
#else
				ret = lower->ops->resume(lower);
#endif
			}
		} else {
			ret = -ENOSYS;
		}
	}
	break;

#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

	/* AUDIOIOC_ALLOCBUFFER - Allocate an audio buffer
	 *
	 *   ioctl argument:  pointer to an audio_buf_desc_s structure
	 */

	case AUDIOIOC_ALLOCBUFFER: {
		audvdbg("AUDIOIOC_ALLOCBUFFER\n");

		bufdesc = (FAR struct audio_buf_desc_s *)arg;
		if (lower->ops->allocbuffer) {
			ret = lower->ops->allocbuffer(lower, bufdesc);
		} else {
			/* Perform a simple kumm_malloc operation assuming 1 session */

			ret = apb_alloc(bufdesc);
		}
	}
	break;

	/* AUDIOIOC_FREEBUFFER - Free an audio buffer
	 *
	 *   ioctl argument:  pointer to an audio_buf_desc_s structure
	 */

	case AUDIOIOC_FREEBUFFER: {
		audvdbg("AUDIOIOC_FREEBUFFER\n");

		bufdesc = (FAR struct audio_buf_desc_s *)arg;
		if (lower->ops->freebuffer) {
			ret = lower->ops->freebuffer(lower, bufdesc);
		} else {
			/* Perform a simple apb_free operation */

			DEBUGASSERT(bufdesc->u.pBuffer != NULL);
			apb_free(bufdesc->u.pBuffer);
			ret = sizeof(struct audio_buf_desc_s);
		}
	}
	break;

	/* AUDIOIOC_ENQUEUEBUFFER - Enqueue an audio buffer
	 *
	 *   ioctl argument:  pointer to an audio_buf_desc_s structure
	 */

	case AUDIOIOC_ENQUEUEBUFFER: {

		if (lower->ops->enqueuebuffer != NULL) {
			audvdbg("AUDIOIOC_ENQUEUEBUFFER\n");
			bufdesc = (FAR struct audio_buf_desc_s *)arg;
			bufdesc->u.pBuffer->flags |= AUDIO_APB_OUTPUT_ENQUEUED;
			ret = lower->ops->enqueuebuffer(lower, bufdesc->u.pBuffer);
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	/* AUDIOIOC_REGISTERMQ - Register a client Message Queue
	 *
	 * TODO:  This needs to have multi session support.
	 */

	case AUDIOIOC_REGISTERMQ: {
		audvdbg("AUDIOIOC_REGISTERMQ\n");

		upper->usermq = (mqd_t) arg;
		ret = OK;
	}
	break;

	/* AUDIOIOC_UNREGISTERMQ - Register a client Message Queue
	 *
	 * TODO:  This needs to have multi session support.
	 */

	case AUDIOIOC_UNREGISTERMQ: {
		audvdbg("AUDIOIOC_UNREGISTERMQ\n");
		mq_close(upper->usermq);	/* Close the message queue */

		upper->usermq = NULL;
		ret = OK;
	}
	break;

	/* AUDIOIOC_RESERVE - Reserve a session with the driver
	 *
	 *   ioctl argument - pointer to receive the session context
	 */

	case AUDIOIOC_RESERVE: {

		if (lower->ops->reserve != NULL) {
			audvdbg("AUDIOIOC_RESERVE\n");

			/* Call lower-half to perform the reservation */

#ifdef CONFIG_AUDIO_MULTI_SESSION
			ret = lower->ops->reserve(lower, (FAR void **)arg);
#else
			ret = lower->ops->reserve(lower);
#endif
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	/* AUDIOIOC_RESERVE - Reserve a session with the driver
	 *
	 *   ioctl argument - pointer to receive the session context
	 */

	case AUDIOIOC_RELEASE: {
		if (lower->ops->release != NULL) {
			audvdbg("AUDIOIOC_RELEASE\n");

			/* Call lower-half to perform the release */

#ifdef CONFIG_AUDIO_MULTI_SESSION
			ret = lower->ops->release(lower, (FAR void *)arg);
#else
			ret = lower->ops->release(lower);
#endif
		} else {
			ret = -ENOSYS;
		}
	}
	break;

	/* Any unrecognized IOCTL commands might be platform-specific ioctl commands */

	default: {
		if (lower->ops->ioctl != NULL) {
			audvdbg("Forwarding unrecognized cmd: %d arg: %ld\n", cmd, arg);
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
 * Name: audio_dequeuebuffer
 *
 * Description:
 *   Dequeues a previously enqueued Audio Pipeline Buffer.
 *
 *   1. The upper half driver calls the enqueuebuffer method, providing the
 *      lower half driver with the ab_buffer to process.
 *   2. The lower half driver's enqueuebuffer will either processes the
 *      buffer directly, or more likely add it to a queue for processing
 *      by a background thread or worker task.
 *   3. When the lower half driver has completed processing of the enqueued
 *      ab_buffer, it will call this routine to indicate processing of the
 *      buffer is complete.
 *   4. When this routine is called, it will check if any threads are waiting
 *      to enqueue additional buffers and "wake them up" for further
 *      processing.
 *
 * Input parameters:
 *   handle - This is the handle that was provided to the lower-half
 *     start() method.
 *   apb - A pointer to the previsously enqueued ap_buffer_s
 *   status - Status of the dequeue operation
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static inline void audio_dequeuebuffer(FAR struct audio_upperhalf_s *upper, FAR struct ap_buffer_s *apb, uint16_t status, FAR void *session)
#else
static inline void audio_dequeuebuffer(FAR struct audio_upperhalf_s *upper, FAR struct ap_buffer_s *apb, uint16_t status)
#endif
{
	struct audio_msg_s msg;

	audvdbg("Entry\n");

	/* Send a dequeue message to the user if a message queue is registered */

	if (upper->usermq != NULL) {
		msg.msgId = AUDIO_MSG_DEQUEUE;
		msg.u.pPtr = apb;
#ifdef CONFIG_AUDIO_MULTI_SESSION
		msg.session = session;
#endif
		apb->flags |= AUDIO_APB_DEQUEUED;
		mq_send(upper->usermq, (FAR const char *)&msg, sizeof(msg), CONFIG_AUDIO_BUFFER_DEQUEUE_PRIO);
	}
}

/****************************************************************************
 * Name: audio_error_handler
 *
 * Description:
 *   Send an AUDIO_MSG_IOERR message to the client to indicate that the
 *   an error has occured.  The lower-half driver initiates this
 *   call via its callback pointer to our upper-half driver.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static inline void audio_error_handler(FAR struct audio_upperhalf_s *upper, FAR struct ap_buffer_s *apb, uint16_t status, FAR void *session)
#else
static inline void audio_error_handler(FAR struct audio_upperhalf_s *upper, FAR struct ap_buffer_s *apb, uint16_t status)
#endif
{
	struct audio_msg_s msg;

	/* Send a error message to the user if a message queue is registered */

	upper->started = false;
	if (upper->usermq != NULL) {
		/* We are always sending XRUN error msg since it is the
		only type of error that we handle now. If new error scenario
		is required, then we need to handle here */
		msg.msgId = AUDIO_MSG_XRUN;
		msg.u.pPtr = NULL;
#ifdef CONFIG_AUDIO_MULTI_SESSION
		msg.session = session;
#endif
		mq_send(upper->usermq, (FAR const char *)&msg, sizeof(msg), MQ_PRIO_MAX);
	}
}

/****************************************************************************
 * Name: audio_complete
 *
 * Description:
 *   Send an AUDIO_MSG_COMPLETE message to the client to indicate that the
 *   active playback has completed.  The lower-half driver initiates this
 *   call via its callback pointer to our upper-half driver.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static inline void audio_complete(FAR struct audio_upperhalf_s *upper, FAR struct ap_buffer_s *apb, uint16_t status, FAR void *session)
#else
static inline void audio_complete(FAR struct audio_upperhalf_s *upper, FAR struct ap_buffer_s *apb, uint16_t status)
#endif
{
	struct audio_msg_s msg;

	audvdbg("Entry\n");

	/* Send a dequeue message to the user if a message queue is registered */

	upper->started = false;
	if (upper->usermq != NULL) {
		msg.msgId = AUDIO_MSG_COMPLETE;
		msg.u.pPtr = NULL;
#ifdef CONFIG_AUDIO_MULTI_SESSION
		msg.session = session;
#endif
		mq_send(upper->usermq, (FAR const char *)&msg, sizeof(msg), CONFIG_AUDIO_BUFFER_DEQUEUE_PRIO);
	}
}

/****************************************************************************
 * Name: audio_callback
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

#ifdef CONFIG_AUDIO_MULTI_SESSION
static void audio_callback(FAR void *handle, uint16_t reason, FAR struct ap_buffer_s *apb, uint16_t status, FAR void *session)
#else
static void audio_callback(FAR void *handle, uint16_t reason, FAR struct ap_buffer_s *apb, uint16_t status)
#endif
{
	FAR struct audio_upperhalf_s *upper = (FAR struct audio_upperhalf_s *)handle;

	audvdbg("Entry\n");

	/* Perform operation based on reason code */

	switch (reason) {
	case AUDIO_CALLBACK_DEQUEUE: {
		/* Call the dequeue routine */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		audio_dequeuebuffer(upper, apb, status, session);
#else
		audio_dequeuebuffer(upper, apb, status);
#endif
		break;
	}

	/* Lower-half I/O error occurred */

	case AUDIO_CALLBACK_IOERR: {
#ifdef CONFIG_AUDIO_MULTI_SESSION
		audio_error_handler(upper, apb, status, session);
#else
		audio_error_handler(upper, apb, status);
#endif
	}
	break;

	/* Lower-half driver has completed a playback */

	case AUDIO_CALLBACK_COMPLETE: {
		/* Send a complete message to the user if a message queue is registered */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		audio_complete(upper, apb, status, session);
#else
		audio_complete(upper, apb, status);
#endif
	}
	break;

	default: {
		auddbg("ERROR: Unknown callback reason code %d\n", reason);
		break;
	}
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: audio_register
 *
 * Description:
 *   This function binds an instance of a "lower half" audio driver with the
 *   "upper half" Audio device and registers that device so that can be used
 *   by application code.
 *
 *   When this function is called, the "lower half" driver should be in the
 *   reset state (as if the shutdown() method had already been called).
 *
 * Input parameters:
 *   path - The full path to the driver to be registers in the NuttX pseudo-
 *     filesystem.  The recommended convention is to name Audio drivers
 *     based on the function they provide, such as "/dev/pcm0", "/dev/mp31",
 *     etc.
 *   dev - A pointer to an instance of lower half audio driver.  This instance
 *     is bound to the Audio driver and must persists as long as the driver
 *     persists.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

int audio_register(FAR const char *name, FAR struct audio_lowerhalf_s *dev)
{
	FAR struct audio_upperhalf_s *upper;
	char path[AUDIO_MAX_DEVICE_PATH];
	static bool dev_audio_created = false;
#ifndef CONFIG_AUDIO_CUSTOM_DEV_PATH
	FAR const char *devname = "/dev/audio";
	int ret;
#elif !defined(CONFIG_AUDIO_DEV_ROOT)
	FAR const char *devname = CONFIG_AUDIO_DEV_PATH;
	FAR const char *ptr;
	FAR char *pathptr;
	int ret;
#endif

	/* Allocate the upper-half data structure */

	upper = (FAR struct audio_upperhalf_s *)kmm_zalloc(sizeof(struct audio_upperhalf_s));
	if (!upper) {
		auddbg("ERROR: Allocation failed\n");
		return -ENOMEM;
	}

	/* Initialize the Audio device structure (it was already zeroed by kmm_zalloc()) */

	sem_init(&upper->exclsem, 0, 1);
	upper->dev = dev;

#ifdef CONFIG_AUDIO_CUSTOM_DEV_PATH

#ifdef CONFIG_AUDIO_DEV_ROOT

	/* This is the simple case ... No need to make a directory */

	strcpy(path, "/dev/");
	strcat(path, name);

#else
	/* Ensure the path begins with "/dev" as we don't support placing device
	 * anywhere but in the /dev directory
	 */

	DEBUGASSERT(strncmp(devname, "/dev", 4) == 0);

	/* Create a /dev/audio directory. */

	if (!dev_audio_created) {
		/* Get path name after "/dev" */

		ptr = &devname[4];
		if (*ptr == '/') {
			ptr++;
		}

		strcpy(path, "/dev/");
		pathptr = &path[5];

		/* Do mkdir for each segment of the path */

		while (*ptr != '\0') {
			/* Build next path segment into path variable */

			while (*ptr != '/' && *ptr != '\0') {
				*pathptr++ = *ptr++;
			}
			*pathptr = '\0';

			/* Make this level of directory */

			ret = mkdir(path, 0644);
			if (ret < 0) {
				auddbg("ERROR: mkdir failed\n");
				kmm_free(upper);
				return ret;
			}

			/* Check for another level */

			*pathptr++ = '/';
			if (*ptr == '/') {
				ptr++;
			}
		}

		/* Indicate we have created the audio dev path */

		dev_audio_created = true;
	}

	/* Now build the path for registration */

	strcpy(path, devname);
	if (devname[sizeof(devname) - 1] != '/') {
		strcat(path, "/");
	}

	strcat(path, name);

#endif							/* CONFIG_AUDIO_DEV_PATH=="/dev" */

#else							/* CONFIG_AUDIO_CUSTOM_DEV_PATH */

	/* Create a /dev/audio directory. */

	if (!dev_audio_created) {
		/* We don't check for error here because even if it fails, then
		 * the register_driver call below will return an error condition
		 * for us.
		 */

		ret = mkdir(devname, 0644);
		if (ret < 0) {
			auddbg("ERROR: mkdir failed\n");
			kmm_free(upper);
			return ret;
		}
		dev_audio_created = true;
	}

	/* Register the Audio device */

	memset(path, 0, AUDIO_MAX_DEVICE_PATH);
	strncpy(path, devname, sizeof(path));
	strncat(path, "/", strlen("/"));
	strncat(path, name, AUDIO_MAX_DEVICE_PATH - 11);
#endif

	/* Give the lower-half a context to the upper half */

	dev->upper = audio_callback;
	dev->priv = upper;

	audvdbg("Registering %s\n", path);
	return register_driver(path, &g_audioops, 0666, upper);
}

#endif							/* CONFIG_AUDIO */
