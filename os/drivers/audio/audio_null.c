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
 * drivers/audio/audio_null.c
 *
 * A do-nothinig audio device driver to simplify testing of audio decoders.
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <debug.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/audio_null.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
/* Below are for testing, we will send message to upper layer if frames reaches below */
#define AUDIO_NULL_KEYWORD_DETECT_THRESHOLD 10

#define AUDIO_NULL_ENDPOINT_DETECT_THRESHOLD 45

enum speech_state_e {
	AUDIO_NULL_SPEECH_STATE_NONE = 0,
	AUDIO_NULL_SPEECH_STATE_IDLE = 1,
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
	AUDIO_NULL_SPEECH_STATE_KD = 2,
#endif
#ifdef CONFIG_AUDIO_ENDPOINT_DETECT
	AUDIO_NULL_SPEECH_STATE_EPD = 3,
#endif
	AUDIO_NULL_SPEECH_STATE_CLEAR = 4,
};

typedef enum speech_state_e speech_state_t;
#endif

struct null_dev_s {
	struct audio_lowerhalf_s dev;	/* Audio lower half (this device) */
	mqd_t mq;					/* Message queue for receiving messages */
	char mqname[16];			/* Our message queue name */
	pthread_t worker_threadid;	/* ID of our worker thread */
	volatile uint32_t frames;	/* Total frames */
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
	pthread_t process_threadid;	/* ID of our process thread */
	volatile bool process_terminate;	/* True : request to terminate processing */
	sem_t processing_sem;		/* Protection for processing start */
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	volatile bool terminate;	/* True: request to terminate audio operation */
#endif
#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
	volatile speech_state_t speech_state;
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
	volatile bool keyword_detect;
#endif
#ifdef CONFIG_AUDIO_ENDPOINT_DETECT
	volatile bool endpoint_detect;
#endif
#endif
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int null_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps);
#else
static int null_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps);
#endif
static int null_shutdown(FAR struct audio_lowerhalf_s *dev);
static void *null_workerthread(pthread_addr_t pvarg);
static void *null_processthread(pthread_addr_t pvarg);

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_start(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int null_start(FAR struct audio_lowerhalf_s *dev);
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int null_stop(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session);
static int null_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int null_pause(FAR struct audio_lowerhalf_s *dev);
static int null_resume(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
static int null_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int null_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int null_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session);
#else
static int null_reserve(FAR struct audio_lowerhalf_s *dev);
#endif
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_release(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int null_release(FAR struct audio_lowerhalf_s *dev);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct audio_ops_s g_audioops = {
	null_getcaps,				/* getcaps        */
	null_configure,				/* configure      */
	null_shutdown,				/* shutdown       */
	null_start,					/* start          */
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	null_stop,					/* stop           */
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
	null_pause,					/* pause          */
	null_resume,				/* resume         */
#endif
	NULL,						/* allocbuffer    */
	NULL,						/* freebuffer     */
	null_enqueuebuffer,			/* enqueue_buffer */
	null_cancelbuffer,			/* cancel_buffer  */
	null_ioctl,					/* ioctl          */
	NULL,						/* read           */
	NULL,						/* write          */
	null_reserve,				/* reserve        */
	null_release				/* release        */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: null_getcaps
 *
 * Description: Get the audio device capabilities
 *
 ****************************************************************************/

static int null_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps)
{
	audvdbg("type=%d\n", type);

	/* Validate the structure */

	DEBUGASSERT(caps->ac_len >= sizeof(struct audio_caps_s));

	/* Fill in the caller's structure based on requested info */

	caps->ac_format.hw = 0;
	caps->ac_controls.w = 0;

	switch (caps->ac_type) {
		/* Caller is querying for the types of units we support */

	case AUDIO_TYPE_QUERY:

		/* Provide our overall capabilities.  The interfacing software
		 * must then call us back for specific info for each capability.
		 */

		caps->ac_channels = 2;	/* Stereo output */

		switch (caps->ac_subtype) {
		case AUDIO_TYPE_QUERY:
			/* We don't decode any formats!  Only something above us in
			 * the audio stream can perform decoding on our behalf.
			 */

			/* The types of audio units we implement */

			caps->ac_controls.b[0] = AUDIO_TYPE_OUTPUT | AUDIO_TYPE_FEATURE | AUDIO_TYPE_PROCESSING;

			break;

		case AUDIO_FMT_MIDI:
			/* We only support Format 0 */

			caps->ac_controls.b[0] = AUDIO_SUBFMT_END;
			break;
		case AUDIO_FMT_OTHER:

		default:
			caps->ac_controls.b[0] = AUDIO_SUBFMT_END;
			break;
		}

		break;

		/* Provide capabilities of our INPUT & OUTPUT unit */

	case AUDIO_TYPE_INPUT:
	case AUDIO_TYPE_OUTPUT:

		caps->ac_channels = 2;

		switch (caps->ac_subtype) {
		case AUDIO_TYPE_QUERY:

			/* Report the Sample rates we support */

			caps->ac_controls.b[0] = AUDIO_SAMP_RATE_TYPE_16K;
			break;

		case AUDIO_FMT_MP3:
		case AUDIO_FMT_WMA:
		case AUDIO_FMT_PCM:
			break;

		default:
			break;
		}

		break;

		/* Provide capabilities of our FEATURE units */

	case AUDIO_TYPE_FEATURE:

		/* If the sub-type is UNDEF, then report the Feature Units we support */

		if (caps->ac_subtype == AUDIO_FU_UNDEF) {
			/* Fill in the ac_controls section with the Feature Units we have */

			caps->ac_controls.b[0] = AUDIO_FU_VOLUME | AUDIO_FU_BASS | AUDIO_FU_TREBLE;
			caps->ac_controls.b[1] = AUDIO_FU_BALANCE >> 8;
		} else {
			/* TODO:  Do we need to provide specific info for the Feature Units,
			 * such as volume setting ranges, etc.?
			 */
		}

		break;

		/* Provide capabilities of our PROCESSING unit */

	case AUDIO_TYPE_PROCESSING:

		switch (caps->ac_subtype) {
		case AUDIO_PU_UNDEF:

			/* Provide the type of Processing Units we support */
			caps->ac_controls.b[0] = AUDIO_PU_STEREO_EXTENDER;

#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
			caps->ac_controls.b[0] |= AUDIO_PU_SPEECH_DETECT;
#endif
			break;

		case AUDIO_PU_STEREO_EXTENDER:

			/* Provide capabilities of our Stereo Extender */

			caps->ac_controls.b[0] = AUDIO_STEXT_ENABLE | AUDIO_STEXT_WIDTH;
			break;

		case AUDIO_PU_SPEECH_DETECT:

			/* Provide capabilities of our Speech Detect */
#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
			caps->ac_controls.b[0] = 0;

#ifdef CONFIG_AUDIO_KEYWORD_DETECT
			caps->ac_controls.b[0] |= AUDIO_SD_KEYWORD_DETECT;
#endif

#ifdef CONFIG_AUDIO_ENDPOINT_DETECT
			caps->ac_controls.b[0] |= AUDIO_SD_ENDPOINT_DETECT;
#endif

#else
			caps->ac_controls.b[0] = AUDIO_SD_UNDEF;
#endif
			break;

		default:

			/* Other types of processing uint we don't support */
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

	audvdbg("Return %d\n", caps->ac_len);
	return caps->ac_len;
}

/****************************************************************************
 * Name: null_configure
 *
 * Description:
 *   Configure the audio device for the specified  mode of operation.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps)
#else
static int null_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps)
#endif
{
	int ret = OK;
#if defined(CONFIG_AUDIO_KEYWORD_DETECT) || defined(CONFIG_AUDIO_ENDPOINT_DETECT)
	FAR struct null_dev_s *priv = (FAR struct null_dev_s *)dev;
#endif
	audvdbg("ac_type: %d\n", caps->ac_type);

	/* Process the configure operation */

	switch (caps->ac_type) {
	case AUDIO_TYPE_FEATURE:
		audvdbg("  AUDIO_TYPE_FEATURE\n");

		/* Process based on Feature Unit */

		switch (caps->ac_format.hw) {
#ifndef CONFIG_AUDIO_EXCLUDE_TONE
		case AUDIO_FU_BASS:
			audvdbg("    Bass: %d\n", caps->ac_controls.b[0]);
			break;

		case AUDIO_FU_TREBLE:
			audvdbg("    Treble: %d\n", caps->ac_controls.b[0]);
			break;
#endif							/* CONFIG_AUDIO_EXCLUDE_TONE */

		default:
			auddbg("    ERROR: Unrecognized feature unit\n");
			break;
		}
		break;

	case AUDIO_TYPE_OUTPUT:
		audvdbg("  AUDIO_TYPE_OUTPUT:\n");
		audvdbg("    Number of channels: %u\n", caps->ac_channels);
		audvdbg("    Sample rate:        %u\n", caps->ac_controls.hw[0]);
		audvdbg("    Sample width:       %u\n", caps->ac_controls.b[2]);
		break;

	case AUDIO_PU_SPEECH_DETECT:
		audvdbg("  AUDIO_PU_SPEECH_DETECT\n");
		switch (caps->ac_subtype) {
#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
		case AUDIO_SD_KEYWORD_DETECT:
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
			priv->keyword_detect = true;
#else
			ret = -EINVAL;
#endif
			break;
		case AUDIO_SD_ENDPOINT_DETECT:
#ifdef CONFIG_AUDIO_ENDPOINT_DETECT
			priv->endpoint_detect = true;
#else
			ret = -EINVAL;
#endif
			break;
#endif
		default:
			auddbg("	ERROR: Unrecognized feature unit\n");
			ret = -EINVAL;
			break;
		}
		audvdbg("  AUDIO_TYPE_PROCESSING:\n");
		break;
	}

	audvdbg("Return OK\n");
	return ret;
}

/****************************************************************************
 * Name: null_shutdown
 *
 * Description:
 *   Shutdown the driver and put it in the lowest power state possible.
 *
 ****************************************************************************/

static int null_shutdown(FAR struct audio_lowerhalf_s *dev)
{
	audvdbg("Return OK\n");
	return OK;
}

/****************************************************************************
 * Name: null_processthread
 *
 *  This is the thread that feeds data to the chip and keeps the audio
 *  stream going.
 *
 ****************************************************************************/
#ifdef CONFIG_AUDIO_ENDPOINT_DETECT
#define AUDIO_MSG_EPD              10
#endif

#ifdef CONFIG_AUDIO_KEYWORD_DETECT
#define AUDIO_MSG_KD               11
#endif

static void *null_processthread(pthread_addr_t pvarg)
{
	FAR struct null_dev_s *priv = (struct null_dev_s *)pvarg;
	struct audio_msg_s msg;

	sem_wait(&priv->processing_sem);
	while (!priv->process_terminate) {
		audvdbg("count : %d state : %d kd : %d epd : %d\n", priv->frames, priv->speech_state, priv->keyword_detect, priv->endpoint_detect);
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
		if ((priv->keyword_detect) && (priv->speech_state == AUDIO_NULL_SPEECH_STATE_IDLE)) {
			if (priv->frames == AUDIO_NULL_KEYWORD_DETECT_THRESHOLD) {
				audvdbg("Keyword Detected!!\n");
				priv->speech_state = AUDIO_NULL_SPEECH_STATE_KD;
				msg.msgId = AUDIO_MSG_KD;
				msg.u.pPtr = NULL;
				mq_send(priv->dev.process_mq, (FAR const char *)&msg, sizeof(msg), CONFIG_AUDIO_NULL_MSG_PRIO);
			}
		}
#endif

#ifdef CONFIG_AUDIO_ENDPOINT_DETECT
		if ((priv->endpoint_detect) && (priv->speech_state == AUDIO_NULL_SPEECH_STATE_IDLE)) {
			audvdbg("EPD Frames: %d\n", priv->frames);
			if (priv->frames == AUDIO_NULL_ENDPOINT_DETECT_THRESHOLD) {
				audvdbg("EndPoint Detected!!\n");
				priv->speech_state = AUDIO_NULL_SPEECH_STATE_EPD;
				msg.msgId = AUDIO_MSG_EPD;
				msg.u.pPtr = NULL;
				mq_send(priv->dev.process_mq, (FAR const char *)&msg, sizeof(msg), CONFIG_AUDIO_NULL_MSG_PRIO);
			}
		}
#endif
		if (priv->speech_state == AUDIO_NULL_SPEECH_STATE_CLEAR) {
			priv->speech_state = AUDIO_NULL_SPEECH_STATE_IDLE;
			audvdbg("clear detect\n");
		}
		usleep(10 * 1000);
	}

	audvdbg("############## End of null_processthread ##############\n");

	return NULL;
}

/****************************************************************************
 * Name: null_workerthread
 *
 *  This is the thread that feeds data to the chip and keeps the audio
 *  stream going.
 *
 ****************************************************************************/

static void *null_workerthread(pthread_addr_t pvarg)
{
	FAR struct null_dev_s *priv = (struct null_dev_s *)pvarg;

	struct audio_msg_s msg;
	int msglen;
	int prio;
	struct timespec st_time;

	audvdbg("Entry\n");

	/* Loop as long as we are supposed to be running */

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	while (!priv->terminate)
#else
	for (;;)
#endif
	{
		/* Wait for messages from our message queue */
		clock_gettime(CLOCK_REALTIME, &st_time);
		msglen = mq_timedreceive(priv->mq, (FAR char *)&msg, sizeof(msg), &prio, &st_time);
		/* Handle the case when we return with no message */

		if (msglen < sizeof(struct audio_msg_s)) {
			auddbg("ERROR: Message too small: %d\n", msglen);
			continue;
		}

		/* Process the message */

		switch (msg.msgId) {
		case AUDIO_MSG_DATA_REQUEST:
			break;

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
		case AUDIO_MSG_STOP:
			priv->terminate = true;
			audvdbg("stop loop in thread\n");
			break;
#endif

		case AUDIO_MSG_ENQUEUE:
			break;

		case AUDIO_MSG_COMPLETE:
			break;

		default:
			audvdbg("ERROR: Ignoring message ID %d\n", msg.msgId);
			break;
		}
		usleep(10 * 1000);
	}

	/* Close the message queue */

	mq_close(priv->mq);
	mq_unlink(priv->mqname);
	priv->mq = NULL;

	/* Send an AUDIO_MSG_COMPLETE message to the client */

#ifdef CONFIG_AUDIO_MULTI_SESSION
	priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_COMPLETE, NULL, OK, NULL);
#else
	priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_COMPLETE, NULL, OK);
#endif

	audvdbg("Exit\n");
	return NULL;
}

/****************************************************************************
 * Name: null_unregisterprocess
 *
 * Description: Register the process (epd, wakeup etc.).
 *
 ****************************************************************************/
static int null_unregisterprocess(FAR struct audio_lowerhalf_s *dev)
{
	FAR struct null_dev_s *priv = (FAR struct null_dev_s *)dev;
	void *value;
	int ret;
	/* Join any old worker thread we had created to prevent a memory leak */
	ret = OK;
	if (priv->process_threadid != 0) {
		ret = pthread_join(priv->process_threadid, &value);
		if (ret != OK) {
			auddbg("join failed!! ret : %d\n", ret);
			return ret;
		}
		priv->process_threadid = 0;
	}

	sem_destroy(&priv->processing_sem);

	if (priv->dev.process_mq != NULL) {
		priv->dev.process_mq = NULL;
	} else {
		auddbg("mq is null\n");
		return -ENOENT;
	}
#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
	priv->speech_state = AUDIO_NULL_SPEECH_STATE_NONE;
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
	priv->keyword_detect = false;
#endif
#ifdef CONFIG_AUDIO_ENDPOINT_DETECT
	priv->endpoint_detect = false;
#endif

#endif
	return ret;
}

/****************************************************************************
 * Name: null_processstart
 *
 * Description:
 *   Register the process (epd, wakeup etc.).
 *
 ****************************************************************************/
static int null_registerprocess(FAR struct audio_lowerhalf_s *dev, mqd_t mq)
{
	FAR struct null_dev_s *priv = (FAR struct null_dev_s *)dev;
	pthread_attr_t tattr;
	FAR void *value;
	int ret;

	audvdbg("Entry\n");
	if (priv->dev.process_mq == NULL) {
		priv->dev.process_mq = mq;
	} else {
		auddbg("already registered!!\n");
		return -EBUSY;
	}

	sem_init(&priv->processing_sem, 0, 0);

	/* Join any old worker thread we had created to prevent a memory leak */

	if (priv->process_threadid != 0) {
		audvdbg("Joining old thread\n");
		pthread_join(priv->process_threadid, &value);
	}

	/* Start our thread for sending data to the device */

	pthread_attr_init(&tattr);
	(void)pthread_attr_setstacksize(&tattr, CONFIG_AUDIO_NULL_WORKER_STACKSIZE);

	audvdbg("#### Starting worker thread\n");
	priv->process_terminate = false;
	ret = pthread_create(&priv->process_threadid, &tattr, null_processthread, (pthread_addr_t) priv);
	if (ret != OK) {
		auddbg("ERROR: pthread_create failed: %d\n", ret);
	} else {
		pthread_setname_np(priv->process_threadid, "null audio process");
		auddbg("Created worker thread\n");
	}

	audvdbg("Return %d\n", ret);
	return ret;
}

/****************************************************************************
 * Name: null_start
 *
 * Description:
 *   Start the configured operation (audio streaming, volume enabled, etc.).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_start(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int null_start(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct null_dev_s *priv = (FAR struct null_dev_s *)dev;
	struct mq_attr attr;
	pthread_attr_t tattr;
	FAR void *value;
	int ret;

	audvdbg("Entry\n");

	/* Create a message queue for the worker thread */

	snprintf(priv->mqname, sizeof(priv->mqname), "/tmp/%X", priv);

	attr.mq_maxmsg = 16;
	attr.mq_msgsize = sizeof(struct audio_msg_s);
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;

	priv->mq = mq_open(priv->mqname, O_RDWR | O_CREAT, 0644, &attr);
	if (priv->mq == NULL) {
		/* Error creating message queue! */

		audvdbg("ERROR: Couldn't allocate message queue\n");
		return -ENOMEM;
	}

	/* Join any old worker thread we had created to prevent a memory leak */

	if (priv->worker_threadid != 0) {
		audvdbg("Joining old thread\n");
		pthread_join(priv->worker_threadid, &value);
	}

	/* Start our thread for sending data to the device */

	pthread_attr_init(&tattr);
	(void)pthread_attr_setstacksize(&tattr, CONFIG_AUDIO_NULL_WORKER_STACKSIZE);

	audvdbg("Starting worker thread\n");
	ret = pthread_create(&priv->worker_threadid, &tattr, null_workerthread, (pthread_addr_t) priv);
	if (ret != OK) {
		auddbg("ERROR: pthread_create failed: %d\n", ret);
	} else {
		pthread_setname_np(priv->worker_threadid, "null audio");
		auddbg("Created worker thread\n");
	}

	audvdbg("Return %d\n", ret);
	return ret;
}

/****************************************************************************
 * Name: null_stop
 *
 * Description: Stop the configured operation (audio streaming, volume
 *              disabled, etc.).
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int null_stop(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct null_dev_s *priv = (FAR struct null_dev_s *)dev;
	struct audio_msg_s term_msg;
	FAR void *value;

	/* Send a message to stop all audio streaming */
	/* REVISIT: There should be a check to see if the worker thread is still
	 * running.
	 */

	term_msg.msgId = AUDIO_MSG_STOP;
	term_msg.u.data = 0;
	mq_send(priv->mq, (FAR const char *)&term_msg, sizeof(term_msg), CONFIG_AUDIO_NULL_MSG_PRIO);

	/* Join the worker thread */

	pthread_join(priv->worker_threadid, &value);
	priv->worker_threadid = 0;
	priv->frames = 0;
	audvdbg("Return OK\n");
	return OK;
}
#endif

/****************************************************************************
 * Name: null_pause
 *
 * Description: Pauses the playback.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int null_pause(FAR struct audio_lowerhalf_s *dev)
#endif
{
	audvdbg("Return OK\n");
	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: null_resume
 *
 * Description: Resumes the playback.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int null_resume(FAR struct audio_lowerhalf_s *dev)
#endif
{
	audvdbg("Return OK\n");
	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: null_enqueuebuffer
 *
 * Description: Enqueue an Audio Pipeline Buffer for playback/ processing.
 *
 ****************************************************************************/

static int null_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	FAR struct null_dev_s *priv = (FAR struct null_dev_s *)dev;
	bool done;

	DEBUGASSERT(priv && apb && priv->dev.upper);

	audvdbg("apb=%p curbyte=%d nbytes=%d nmaxbytes %d\n", apb, apb->curbyte, apb->nbytes, apb->nmaxbytes);

	/* Say that we consumed all of the data */

	apb->curbyte = apb->nbytes;
	apb->nbytes = apb->nmaxbytes;
	/* Check if this was the last buffer in the stream */
	done = ((apb->flags & AUDIO_APB_FINAL) != 0);

	priv->frames++;

	/* The buffer belongs to to an upper level.  Just forward the event to
	 * the next level up.
	 */
#ifdef CONFIG_AUDIO_MULTI_SESSION
	priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK, NULL);
#else
	priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK);
#endif
	usleep(20 * 1000); //20ms as a delay, assuming that i2s operations

	/* Say we are done playing if this was the last buffer in the stream */

	if (done) {
#ifdef CONFIG_AUDIO_MULTI_SESSION
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_COMPLETE, NULL, OK, NULL);
#else
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_COMPLETE, NULL, OK);
#endif
	}

	audvdbg("Return OK\n");
	return OK;
}

/****************************************************************************
 * Name: null_cancelbuffer
 *
 * Description: Called when an enqueued buffer is being cancelled.
 *
 ****************************************************************************/

static int null_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	audvdbg("apb=%p curbyte=%d nbytes=%d, return OK\n", apb, apb->curbyte, apb->nbytes);

	return OK;
}

/****************************************************************************
 * Name: null_ioctl
 *
 * Description: Perform a device ioctl
 *
 ****************************************************************************/

static int null_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg)
{
	FAR struct null_dev_s *priv = (FAR struct null_dev_s *)dev;
	int ret = OK;

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	FAR struct ap_buffer_info_s *bufinfo;
#endif

	auddbg("cmd=%d arg=%ld\n");

	/* Deal with ioctls passed from the upper-half driver */

	switch (cmd) {
		/* Check for AUDIOIOC_HWRESET ioctl.  This ioctl is passed straight
		 * through from the upper-half audio driver.
		 */
	case AUDIOIOC_HWRESET: {
		audvdbg("AUDIOIOC_HWRESET:\n");
	}
	break;
	/* Report our preferred buffer size and quantity */

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	case AUDIOIOC_GETBUFFERINFO: {
		audvdbg("AUDIOIOC_GETBUFFERINFO:\n");
		bufinfo = (FAR struct ap_buffer_info_s *)arg;
		bufinfo->buffer_size = CONFIG_AUDIO_NULL_BUFFER_SIZE;
		bufinfo->nbuffers = CONFIG_AUDIO_NULL_NUM_BUFFERS;
	}
	break;
#endif
	case AUDIOIOC_REGISTERPROCESS: {
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
		priv->speech_state = AUDIO_NULL_SPEECH_STATE_NONE;
#endif
		ret = null_registerprocess(dev, (mqd_t) arg);
		if (ret != OK) {
			auddbg("Process Start Failed ret : %d\n", ret);
			return ret;
		}
		ret = OK;
#else
		auddbg("Register Process Failed - Device Doesn't support\n");
		ret = -EINVAL;
#endif
	}
	break;
	case AUDIOIOC_UNREGISTERPROCESS: {
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
		ret = null_unregisterprocess(dev);
		if (ret != OK) {
			auddbg("Process Start Failed ret : %d\n", ret);
			return ret;
		}
		ret = OK;

#else
		auddbg("UnRegister Process Failed - Device Doesn't support\n");
		ret = -EINVAL;
#endif
	}
	break;

	case AUDIOIOC_STARTPROCESS: {
		auddbg("set start process!!\n");
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
		priv->process_terminate = false;
		priv->speech_state = AUDIO_NULL_SPEECH_STATE_IDLE;
		sem_post(&priv->processing_sem);
		ret = OK;
#else
		auddbg("start Process Failed - Device Doesn't support\n");
		ret = -EINVAL;
#endif
	}
	break;

	case AUDIOIOC_STOPPROCESS: {
		auddbg("set stop process!!\n");
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
		priv->speech_state = AUDIO_NULL_SPEECH_STATE_NONE;
		priv->process_terminate = true;
		ret = OK;
#else
		auddbg("start Process Failed - Device Doesn't support\n");
		ret = -EINVAL;
#endif
	}
	break;

	default:
		break;
	}

	audvdbg("Return OK\n");
	return ret;
}

/****************************************************************************
 * Name: null_reserve
 *
 * Description: Reserves a session (the only one we have).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session)
#else
static int null_reserve(FAR struct audio_lowerhalf_s *dev)
#endif
{
	audvdbg("Return OK\n");
	return OK;
}

/****************************************************************************
 * Name: null_release
 *
 * Description: Releases the session (the only one we have).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int null_release(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int null_release(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct null_dev_s *priv = (FAR struct null_dev_s *)dev;
	void *value;

	/* Join any old worker thread we had created to prevent a memory leak */

	if (priv->worker_threadid != 0) {
		pthread_join(priv->worker_threadid, &value);
		priv->worker_threadid = 0;
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: audio_null_initialize
 *
 * Description:
 *   Initialize the null audio device.
 *
 * Input Parameters:
 *   i2c     - An I2C driver instance
 *   i2s     - An I2S driver instance
 *   lower   - Persistent board configuration data
 *
 * Returned Value:
 *   A new lower half audio interface for the NULL audio device is returned
 *   on success; NULL is returned on failure.
 *
 ****************************************************************************/

FAR struct audio_lowerhalf_s *audio_null_initialize(void)
{
	FAR struct null_dev_s *priv;

	/* Allocate the null audio device structure */

	priv = (FAR struct null_dev_s *)kmm_zalloc(sizeof(struct null_dev_s));
	if (priv) {
		/* Initialize the null audio device structure.  Since we used kmm_zalloc,
		 * only the non-zero elements of the structure need to be initialized.
		 */

		priv->dev.ops = &g_audioops;
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
		priv->process_terminate = false;
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
		priv->keyword_detect = false;
#endif
#ifdef CONFIG_AUDIO_ENDPOINT_DETECT
		priv->endpoint_detect = false;
#endif
#endif

		return &priv->dev;
	}

	auddbg("ERROR: Failed to allocate null audio device\n");
	return NULL;
}
