/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#ifndef __DRIVERS_AUDIO_ALC1019_H
#define __DRIVERS_AUDIO_ALC1019_H

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#ifdef CONFIG_AUDIO_ALC1019
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <queue.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/audio/audio.h>
#ifdef ALC1019_USE_FFLOCK_INT
#include <tinyara/wqueue.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define ALC1019_SAMPLE_FREQUENCY_IN_HZ  (48000) /* refer to Input data sample rate */
#define ALC1019_CHANNELS                2
#define ALC1019_BPSAMP                  16 /* refer to Digital Audio Interface Control registers */

#define ALC1019_DEFAULT_SAMPRATE    48000
#define ALC1019_DEFAULT_NCHANNELS   2
#define ALC1019_DEFAULT_BPSAMP      16
#define FAIL_8                      0xFF
#define FAIL_32                     0xFFFFFFFF
#define alc1019_givesem(s)          sem_post(s)

#define ALC1019_SPK_VOL_MIN         0
#define ALC1019_SPK_VOL_MAX         10
#define ALC1019_SPK_VOL_DEF         7

/*
 * TODO : tune the volume by adjusting the step size, max vol and so on
 */

#define ALC1019_HW_VOL_MAX		255
#define ALC1019_HW_VOL_STEP		7

/* ALC1019_HW_VOL_DEF = (ALC1019_HW_VOL_MAX - (ALC1019_SPK_VOL_MAX - ALC1019_SPK_VOL_DEF) * ALC1019_HW_VOL_STEP) */
#define ALC1019_HW_VOL_DEF		234

/* Commonly defined and redefined macros */

#ifndef MIN
#define MIN(a, b)                   (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b)                   (((a) > (b)) ? (a) : (b))
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct alc1019_dev_s {
	/* We are an audio lower half driver (We are also the upper "half" of
	 * the ALC1019 driver with respect to the board lower half driver).
	 *
	 * Terminology: Our "lower" half audio instances will be called dev for the
	 * publicly visible version and "priv" for the version that only this driver
	 * knows.  From the point of view of this driver, it is the board lower
	 * "half" that is referred to as "lower".
	 */

	struct audio_lowerhalf_s dev;	/* ALC1019 audio lower half (this device) */

	/* Our specific driver data goes here */

	FAR struct alc1019_lower_s *lower;	/* Pointer to the board lower functions */
	FAR struct i2c_dev_s *i2c;		/* I2C driver to use */
	FAR struct i2s_dev_s *i2s;		/* I2S driver to use */
	struct sq_queue_s pendq;		/* Queue of pending buffers to be sent */
	sem_t devsem;				/* Protection for both pendq & dev */

#ifdef ALC1019_USE_FFLOCK_INT
	struct work_s work;			/* Interrupt work */
#endif
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
#ifdef ALC1019_USE_FFLOCK_INT
	volatile bool locked;			/* FLL is locked */
#endif
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
static void alc1019_takesem(sem_t *sem);

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
static void alc1019_setvolume(FAR struct alc1019_dev_s *priv);
#endif

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
static void alc1019_setbass(FAR struct alc1019_dev_s *priv, uint8_t bass);
static void alc1019_settreble(FAR struct alc1019_dev_s *priv, uint8_t treble);
#endif
static void alc1019_set_i2s_datawidth(FAR struct alc1019_dev_s *priv);
static void alc1019_set_i2s_samplerate(FAR struct alc1019_dev_s *priv);

/* Audio lower half methods (and close friends) */

static int alc1019_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps);
#else
static int alc1019_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps);
#endif
static int alc1019_shutdown(FAR struct audio_lowerhalf_s *dev);

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_start(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int alc1019_start(FAR struct audio_lowerhalf_s *dev);
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int alc1019_stop(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session);
static int alc1019_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int alc1019_pause(FAR struct audio_lowerhalf_s *dev);
static int alc1019_resume(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
static int alc1019_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int alc1019_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int alc1019_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session);
#else
static int alc1019_reserve(FAR struct audio_lowerhalf_s *dev);
#endif
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_release(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int alc1019_release(FAR struct audio_lowerhalf_s *dev);
#endif

static void alc1019_reconfigure(FAR struct alc1019_dev_s *priv);
static void alc1019_hw_reset(FAR struct alc1019_dev_s *priv);
#endif							/* CONFIG_AUDIO_ALC1019 */
#endif							/* __DRIVERS_AUDIO_ALC1019_H */
