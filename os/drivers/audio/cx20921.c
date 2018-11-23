/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
 * drivers/audio/cx20921.c
 *
 * Audio device driver for CONEXANT CX20921 Audio codec.
 *
 *   Copyright (C) 2014, 2016 Gregory Nutt. All rights reserved.
 *   Author:  Gregory Nutt <gnutt@nuttx.org>
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
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <queue.h>
#include <errno.h>
#include <fixedmath.h>
#include <debug.h>
#include <tinyara/kmalloc.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/cx20921.h>
#include <tinyara/math.h>
#include <math.h>

#include "cx20921.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef CONFIG_CX20921_I2S_TIMEOUT
#define CX20921_I2S_TIMEOUT_MS CONFIG_CX20921_I2S_TIMEOUT
#else
#define CX20921_I2S_TIMEOUT_MS 100
#endif

#define CX20921_GAIN_DEFAULT 0x10	//ToDo: Change the value into a proper one by referencing cx20921 datasheet.

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct audio_ops_s g_audioops = {
	.getcaps = cx20921_getcaps,             /* getcaps        */
	.configure = cx20921_configure,         /* configure      */
	.shutdown = cx20921_shutdown,           /* shutdown       */
	.start = cx20921_start,                 /* start          */
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	.stop = cx20921_stop,                   /* stop           */
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
	.pause = cx20921_pause,                 /* pause          */
	.resume = cx20921_resume,               /* resume         */
#endif
	.allocbuffer = NULL,                    /* allocbuffer    */
	.freebuffer = NULL,                     /* freebuffer     */
	.enqueuebuffer = cx20921_enqueuebuffer, /* enqueue_buffer */
	.cancelbuffer = cx20921_cancelbuffer,   /* cancel_buffer  */
	.ioctl = cx20921_ioctl,                 /* ioctl          */
	.read = NULL,                           /* read           */
	.read = NULL,                           /* write          */
	.reserve = cx20921_reserve,             /* reserve        */
	.release = cx20921_release,             /* release        */
};

/************************************************************************************
 * Name: cx20921_takesem
 *
 * Description:
 *  Take a semaphore count, handling the nasty EINTR return if we are interrupted
 *  by a signal.
 *
 ************************************************************************************/
static void cx20921_takesem(sem_t *sem)
{
	int ret;

	do {
		ret = sem_wait(sem);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

/****************************************************************************
 * Name: cx20921_set_i2s_datawidth
 *
 * Description:
 *   Set the 8- 16- 24- bit data modes
 *
 ****************************************************************************/
static void cx20921_set_i2s_datawidth(FAR struct cx20921_dev_s *priv)
{
	/* if no audio device object return */
	if (!priv) {
		auddbg("Error, Device's private data Not available\n");
		return;
	}

	I2S_RXDATAWIDTH(priv->i2s, priv->bpsamp);
}

/****************************************************************************
 * Name: cx20921_set_i2s_samplerate
 *
 * Description:
 *
 ****************************************************************************/
static void cx20921_set_i2s_samplerate(FAR struct cx20921_dev_s *priv)
{
	/* if no audio device object return */
	if (!priv) {
		auddbg("Error, Device's private data Not available\n");
		return;
	}

	I2S_RXSAMPLERATE(priv->i2s, priv->samprate);
}

/****************************************************************************
 * Name: cx20921_getcaps
 *
 * Description:
 *   Get the audio device capabilities
 *
 ****************************************************************************/
static int cx20921_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps)
{
	/* Validate the structure */

	DEBUGASSERT(caps && caps->ac_len >= sizeof(struct audio_caps_s));
	audvdbg("type=%d ac_type=%d\n", type, caps->ac_type);

	/* Fill in the caller's structure based on requested info */

	caps->ac_controls.w = 0;

	switch (caps->ac_type) {
	/* Caller is querying for the types of units we support */

	case AUDIO_TYPE_QUERY:

		/* Provide our overall capabilities.  The interfacing software
		 * must then call us back for specific info for each capability.
		 */
		caps->ac_channels = CX20921_CHANNELS;  /* Stereo output */
		switch (caps->ac_subtype) {
		case AUDIO_TYPE_QUERY:
			/* We don't decode any formats!  Only something above us in
			 * the audio stream can perform decoding on our behalf.
			 */

			/* The types of audio units we implement */
			caps->ac_controls.b[0] = AUDIO_TYPE_INPUT;
			break;

		default:
			caps->ac_controls.b[0] = AUDIO_SUBFMT_END;
			break;
		}
		break;

	/* Provide capabilities of our INPUT & OUTPUT unit */
	case AUDIO_TYPE_INPUT:
		caps->ac_channels = CX20921_CHANNELS;
		switch (caps->ac_subtype) {
		case AUDIO_TYPE_QUERY:
			/* Report the Sample rates we support */
			caps->ac_controls.b[0] = AUDIO_SAMP_RATE_TYPE_16K;
			break;

		default:
			break;
		}

		break;

	case AUDIO_TYPE_FEATURE:

		switch (caps->ac_format.hw) {
		case AUDIO_FU_INP_GAIN:
			// ToDo: Need to return a more proper value.
			caps->ac_controls.hw[0] = CX20921_GAIN_DEFAULT;
			caps->ac_controls.hw[1] = CX20921_GAIN_DEFAULT;
			audvdbg("[getcaps] gain = %d, %x\n", caps->ac_controls.hw[1], caps->ac_controls.hw[1]);
			break;

		default:
			break;
		}

		break;

	/* All others we don't support */

	default:

		/* Zero out the fields to indicate no support */

		caps->ac_subtype = 0;
		caps->ac_channels = 0;

		break;
	}

	/* Return the length of the audio_caps_s struct for validation of
	 * proper Audio device type.
	 */

	return caps->ac_len;
}

/****************************************************************************
 * Name: cx20921_configure
 *
 * Description:
 *   Configure the audio device for the specified  mode of operation.
 *
 ****************************************************************************/
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps)
#else
static int cx20921_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps)
#endif
{
#if !defined(CONFIG_AUDIO_EXCLUDE_VOLUME) || !defined(CONFIG_AUDIO_EXCLUDE_TONE)
	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)dev;
#endif
	int ret = OK;

	DEBUGASSERT(priv && caps);
	audvdbg("ac_type: %d\n", caps->ac_type);

	/* CX20921 supports on the fly changes for almost all changes
	   so no need to do anything. But if any issue, worth looking here */

	switch (caps->ac_type) {
	case AUDIO_TYPE_FEATURE:
		audvdbg("  AUDIO_TYPE_FEATURE\n");
		/* Inner swich case: Process based on Feature Unit */
		switch (caps->ac_format.hw) {
		case AUDIO_FU_INP_GAIN: {
			ret = -EACCES;
			break;
		}
		default:
			auddbg("    ERROR: Unrecognized feature unit\n");
			break;
		}
		break;					/* Break for inner switch case */
	case AUDIO_TYPE_INPUT:
		audvdbg("  AUDIO_TYPE :%s\n", caps->ac_type == AUDIO_TYPE_INPUT ? "INPUT" : "OUTPUT");
		/* Verify that all of the requested values are supported */

		ret = -EDOM;
		if (caps->ac_channels != CX20921_CHANNELS) {
			auddbg("ERROR: Unsupported number of channels: %d\n", caps->ac_channels);
			break;
		}

		if (caps->ac_controls.b[2] != CX20921_BPSAMP) {
			auddbg("ERROR: Unsupported bits per sample: %d\n", caps->ac_controls.b[2]);
			break;
		}

		if (caps->ac_controls.hw[0] != CX20921_SAMPLE_FREQUENCY_IN_HZ) {
			auddbg("ERROR: Not supported Format: %lld\n", caps->ac_controls.hw[0]);
			return -EINVAL;
		}

		/* Save the current stream configuration */

		priv->samprate = caps->ac_controls.hw[0];
		priv->nchannels = caps->ac_channels;
		priv->bpsamp = caps->ac_controls.b[2];

		audvdbg("    Number of channels: 0x%x\n", priv->nchannels);
		audvdbg("    Sample rate:        0x%x\n", priv->samprate);
		audvdbg("    Sample width:       0x%x\n", priv->bpsamp);

		/* Reconfigure the FLL to support the resulting number or channels,
		 * bits per sample, and bitrate.
		 */
		ret = OK;
		if (caps->ac_type == AUDIO_TYPE_INPUT) {
			priv->inout = true;
		} else {
			ret = -ERANGE;
		}
		break;

	case AUDIO_TYPE_PROCESSING:
		break;
	}
	return ret;
}

/****************************************************************************
 * Name: cx20921_shutdown
 *
 * Description:
 *   Shutdown the CX20921 chip and put it in the lowest power state possible.
 *
 ****************************************************************************/
static int cx20921_shutdown(FAR struct audio_lowerhalf_s *dev)
{
	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)dev;

	DEBUGASSERT(priv);

	if (!priv) {
		return -EINVAL;
	}

	cx20921_takesem(&priv->devsem);
	sq_entry_t *tmp = NULL;
	for (tmp = (sq_entry_t *)sq_peek(&priv->pendq); tmp; tmp = sq_next(tmp)) {
		sq_rem(tmp, &priv->pendq);
		audvdbg("(cxshutdown)removing tmp with addr 0x%x\n", tmp);
	}
	sq_init(&priv->pendq);
	if (priv->running) {
		I2S_STOP(priv->i2s, I2S_RX);
		priv->running = false;
	}
	priv->paused = false;
	cx20921_givesem(&priv->devsem);
	
	/* Now issue a software reset.  This puts all CX20921 registers back in
	 * their default state.
	 */

	cx20921_hw_reset(priv);
	return OK;
}

/****************************************************************************
 * Name: cx20921_io_err_cb
 *
 * Description:
 *   Callback function for io error
 *
 ****************************************************************************/
static void cx20921_io_err_cb(FAR struct i2s_dev_s *dev, FAR void *arg, int flags)
{
	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)arg;

	/* Call upper callback, let it post msg to user q
	 * apb is set NULL, okay? Rethink
	 */
	if (priv && priv->dev.upper) {
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_IOERR, NULL, flags);
	}

}

/****************************************************************************
 * Name: cx20921_start
 *
 * Description:
 *   Start the configured operation (audio streaming, volume enabled, etc.).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_start(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int cx20921_start(FAR struct audio_lowerhalf_s *dev)
#endif
{

	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	audvdbg("cx20921_start Entry\n");
	cx20921_takesem(&priv->devsem);

	if (priv->running) {
		goto cxstart_withsem;
	}

	/* Register cb for io error */
	I2S_ERR_CB_REG(priv->i2s, cx20921_io_err_cb, priv);

	/* Finally set cx20921 to be running */
	priv->running = true;
	
	/* Enqueue buffers (enqueueed before the start of cx20921) to lower layer */
	sq_entry_t *tmp = NULL;
	sq_queue_t *q = &priv->pendq;
	for (tmp = sq_peek(q); tmp; tmp = sq_next(tmp)) {
		cx20921_enqueuebuffer(dev, (struct ap_buffer_s *)tmp);
	}

	/* Exit reduced power modes of operation */
	/* REVISIT */

cxstart_withsem:

	cx20921_givesem(&priv->devsem);
	return OK;
}

/****************************************************************************
 * Name: cx20921_stop
 *
 * Description: Stop the configured operation (audio streaming, volume
 *              disabled, etc.).
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int cx20921_stop(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}
	cx20921_takesem(&priv->devsem);

	I2S_STOP(priv->i2s, I2S_RX);

	/* Need to run the stop script here */

	priv->running = false;
	cx20921_givesem(&priv->devsem);

	/* Enter into a reduced power usage mode */
	/* REVISIT: */

	return OK;
}
#endif

/****************************************************************************
 * Name: cx20921_pause
 *
 * Description: Pauses the playback.
 *
 ****************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int cx20921_pause(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	cx20921_takesem(&priv->devsem);

	if (priv->running && !priv->paused) {
		/* Disable interrupts to prevent us from suppling any more data */

		priv->paused = true;
		I2S_PAUSE(priv->i2s, I2S_RX);
	}

	cx20921_givesem(&priv->devsem);

	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: cx20921_resume
 *
 * Description: Resumes the playback.
 *
 ****************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int cx20921_resume(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	cx20921_takesem(&priv->devsem);

	if (priv->running && priv->paused) {
		priv->paused = false;

		I2S_RESUME(priv->i2s, I2S_RX);
	}

	cx20921_givesem(&priv->devsem);
	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: cx20921_rxcallback
 *
 * Description: Called when I2S filled a buffer. No recycling mechanism now.
 *
 ****************************************************************************/

static void cx20921_rxcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)arg;

	DEBUGASSERT(priv && apb);
	if (!priv) {
		audvdbg("cx20921_rxcallback, no private data available\n");
		return;
	}

	cx20921_takesem(&priv->devsem);
	sq_entry_t *tmp;
	for (tmp = (sq_entry_t *)sq_peek(&priv->pendq); tmp; tmp = sq_next(tmp)) {
		if (tmp == (sq_entry_t *)apb) {
			sq_rem(tmp, &priv->pendq);
			audvdbg("found the apb to remove 0x%x\n", tmp);
			break;
		}
	}

	/* Call upper callback, let it post msg to user q */
	priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK);

	cx20921_givesem(&priv->devsem);
}

/****************************************************************************
 * Name: cx20921_enqueuebuffer
 *
 * Description: Enqueue an Audio Pipeline Buffer for playback/ processing.
 *
 ****************************************************************************/
static int cx20921_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)dev;
	int ret;

	if (!priv || !apb) {
		return -EINVAL;
	}

	if (!priv->running) {
		/* Add the new buffer to the tail of pending audio buffers */
		cx20921_takesem(&priv->devsem);
		sq_addlast((sq_entry_t*)&apb->dq_entry, &priv->pendq);
		audvdbg("enqueue added buf 0x%x\n", apb);
		cx20921_givesem(&priv->devsem);
		return OK;
	}

	ret = I2S_RECEIVE(priv->i2s, apb, cx20921_rxcallback, priv, CX20921_I2S_TIMEOUT_MS);
	return ret;
}

/****************************************************************************
 * Name: cx20921_cancelbuffer
 *
 * Description: Called when an enqueued buffer is being cancelled.
 *
 ****************************************************************************/
static int cx20921_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	audvdbg("apb=%p\n", apb);
	/* Need to add logic here */
	return OK;
}

/****************************************************************************
 * Name: cx20921_ioctl
 *
 * Description: Perform a device ioctl
 *
 ****************************************************************************/
static int cx20921_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg)
{
	FAR struct ap_buffer_info_s *bufinfo;
	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)dev;


	if (!priv) {
		return -EINVAL;
	}

	/* Deal with ioctls passed from the upper-half driver */

	switch (cmd) {
		case AUDIOIOC_PREPARE: {
			audvdbg("AUDIOIOC_PREPARE: cx20921 prepare\n");
		
			/* Take semaphore */
			cx20921_takesem(&priv->devsem);

			/* Pause i2s channel */
			I2S_PAUSE(priv->i2s, I2S_RX);

			/*Reconfigure cx20921 */

			/* Resume I2S */
			I2S_RESUME(priv->i2s, I2S_RX);
			/* Give semaphore */
			cx20921_givesem(&priv->devsem);
		}
		break;

		case AUDIOIOC_HWRESET: {
			/* This should put CX20921 in default state, reconfiguration needed */
			audvdbg("AUDIOIOC_HWRESET:\n");
			cx20921_hw_reset(priv);
		}
		break;

		case AUDIOIOC_GETBUFFERINFO: {
			/* Report our preferred buffer size and quantity */
			audvdbg("AUDIOIOC_GETBUFFERINFO:\n");
			/* Take semaphore */
			cx20921_takesem(&priv->devsem);

			bufinfo = (FAR struct ap_buffer_info_s *)arg;
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
			bufinfo->buffer_size = CONFIG_CX20921_BUFFER_SIZE;
			bufinfo->nbuffers = CONFIG_CX20921_NUM_BUFFERS;
#else
			bufinfo->buffer_size = CONFIG_CX20921_BUFFER_SIZE;
			bufinfo->nbuffers = CONFIG_CX20921_NUM_BUFFERS;
#endif
			audvdbg("buffer_size : %d nbuffers : %d\n", bufinfo->buffer_size, bufinfo->nbuffers);

			/* Give semaphore */
			cx20921_givesem(&priv->devsem);
		}
		break;
		default:
			audvdbg("cx20921_ioctl received unkown cmd 0x%x\n", cmd);
			break;
	}

	return OK;
}

/****************************************************************************
 * Name: cx20921_reserve
 *
 * Description: Reserves a session (the only one we have).
 *
 ****************************************************************************/
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session)
#else
static int cx20921_reserve(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)dev;
	int ret = OK;

	if (!priv) {
		return -EINVAL;
	}

	/* Borrow the APBQ semaphore for thread sync */

	cx20921_takesem(&priv->devsem);
	if (priv->reserved) {
		ret = -EBUSY;
	} else {
		/* Initialize the session context */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		*session = NULL;
#endif
		priv->inflight = 0;
		priv->running = false;
		priv->paused = false;
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
		priv->terminating = false;
#endif
		priv->reserved = true;
	}

	cx20921_givesem(&priv->devsem);

	return ret;
}

/****************************************************************************
 * Name: cx20921_release
 *
 * Description: Releases the session (the only one we have).
 *
 ****************************************************************************/
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_release(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int cx20921_release(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct cx20921_dev_s *priv = (FAR struct cx20921_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	cx20921_takesem(&priv->devsem);
	if (priv->running) {
		I2S_STOP(priv->i2s, I2S_RX);
		priv->running = false;
	}
	priv->reserved = false;
	cx20921_givesem(&priv->devsem);

	return OK;
}

/****************************************************************************
 * Name: cx20921_hw_reset
 *
 * Description:
 *   Reset and re-initialize the CX20921
 *
 * Input Parameters:
 *   priv - A reference to the driver state structure
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void cx20921_hw_reset(FAR struct cx20921_dev_s *priv)
{
	struct cx20921_lower_s *lower = priv->lower;

	if (lower->control_hw_reset) {
		lower->control_hw_reset(true);
		usleep(100 * 1000);
		lower->control_hw_reset(false);
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cx20921_initialize
 *
 * Description:
 *   Initialize the CX20921 device.
 *
 * Input Parameters:
 *   i2c     - An I2C driver instance
 *   i2s     - An I2S driver instance
 *   lower   - Persistent board configuration data
 *
 * Returned Value:
 *   A new lower half audio interface for the CX20921 device is returned on
 *   success; NULL is returned on failure.
 *
 ****************************************************************************/
FAR struct audio_lowerhalf_s *cx20921_initialize(FAR struct i2c_dev_s *i2c, FAR struct i2s_dev_s *i2s, FAR struct cx20921_lower_s *lower)
{

	FAR struct cx20921_dev_s *priv;
	/* Sanity check */
	DEBUGASSERT(i2c && i2s && lower);

	auddbg("I2s dev addr is 0x%x\n", i2s);

	/* Allocate a CX20921 device structure */
	priv = (FAR struct cx20921_dev_s *)kmm_zalloc(sizeof(struct cx20921_dev_s));

	if (!priv) {
		return NULL;
	}
	/* Initialize the CX20921 device structure.  Since we used kmm_zalloc,
	 * only the non-zero elements of the structure need to be initialized.
	 */

	priv->dev.ops = &g_audioops;
	priv->lower = lower;
	priv->i2c = i2c;
	priv->i2s = i2s;

	sem_init(&priv->devsem, 0, 1);
	sq_init(&priv->pendq);

	priv->inout = true;
	priv->nchannels = CX20921_CHANNELS;
	priv->samprate = CX20921_SAMPLE_FREQUENCY_IN_HZ;
	priv->bpsamp = CX20921_BPSAMP;
	cx20921_set_i2s_samplerate(priv);
	cx20921_set_i2s_datawidth(priv);
	return &priv->dev;
}

