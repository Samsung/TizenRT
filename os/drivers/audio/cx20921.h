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
 * drivers/audio/cx20921.h
 *
 * Audio device driver for REALTEK CX20921 Audio codec.
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

#ifndef __DRIVERS_AUDIO_CX20921_H
#define __DRIVERS_AUDIO_CX20921_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#ifdef CONFIG_AUDIO_CX20921
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <queue.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/audio/audio.h>
#include <tinyara/fs/ioctl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define cx20921_givesem(s) sem_post(s)

#define CX20921_SAMPLE_FREQUENCY_IN_HZ  (16000)
#define CX20921_CHANNELS                2
#define CX20921_BPSAMP                  16

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* This should be put under hammer to strip size
by for status variables */

struct cx20921_dev_s {
	/* We are an audio lower half driver (We are also the upper "half" of
	 * the CX20921 driver with respect to the board lower half driver).
	 *
	 * Terminology: Our "lower" half audio instances will be called dev for the
	 * publicly visible version and "priv" for the version that only this driver
	 * knows.  From the point of view of this driver, it is the board lower
	 * "half" that is referred to as "lower".
	 */

	struct audio_lowerhalf_s dev;	/* CX20921 audio lower half (this device) */

	/* Our specific driver data goes here */

	FAR struct cx20921_lower_s *lower;	/* Pointer to the board lower functions */
	FAR struct i2c_dev_s *i2c;		/* I2C driver to use */
	FAR struct i2s_dev_s *i2s;		/* I2S driver to use */
	struct sq_queue_s pendq;		/* Queue of pending buffers to be sent */
	sem_t devsem;				/* Protection for both pendq & dev */

	uint16_t samprate;			/* Configured samprate (samples/sec) */
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
#ifndef CONFIG_AUDIO_EXCLUDE_BALANCE
	uint16_t balance;			/* Current balance level (b16) */
#endif							/* CONFIG_AUDIO_EXCLUDE_BALANCE */
	uint8_t volume;				/* Current volume level {0..63} */
#endif							/* CONFIG_AUDIO_EXCLUDE_VOLUME */
	uint8_t nchannels;			/* Number of channels (1 or 2) */
	uint8_t bpsamp;				/* Bits per sample (8 or 16) */
	volatile uint8_t inflight;		/* Number of audio buffers in-flight */
	bool running;				/* True: Worker thread is running */
	bool paused;				/* True: Playing is paused */
	bool mute;				/* True: Output is muted */
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	bool terminating;			/* True: Stop requested */
#endif
	bool reserved;				/* True: Device is reserved */
	volatile int result;			/* The result of the last transfer */
	bool inout;				/* True: IN device */
};

/****************************************************************************
 * Private Declarations
 ****************************************************************************/
static void cx20921_takesem(sem_t *sem);

static void cx20921_set_i2s_datawidth(FAR struct cx20921_dev_s *priv);
static void cx20921_set_i2s_samplerate(FAR struct cx20921_dev_s *priv);

/* Audio lower half methods (and close friends) */

static int cx20921_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps);
#else
static int cx20921_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps);
#endif
static int cx20921_shutdown(FAR struct audio_lowerhalf_s *dev);

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_start(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int cx20921_start(FAR struct audio_lowerhalf_s *dev);
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int cx20921_stop(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session);
static int cx20921_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int cx20921_pause(FAR struct audio_lowerhalf_s *dev);
static int cx20921_resume(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
static int cx20921_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int cx20921_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int cx20921_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session);
#else
static int cx20921_reserve(FAR struct audio_lowerhalf_s *dev);
#endif
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int cx20921_release(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int cx20921_release(FAR struct audio_lowerhalf_s *dev);
#endif

/* Initialization */
static void cx20921_hw_reset(FAR struct cx20921_dev_s *priv);

#endif /* CONFIG_AUDIO_CX20921 */
#endif /* __DRIVERS_AUDIO_CX20921_H */
