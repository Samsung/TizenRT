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
 * drivers/audio/alc5658.c
 *
 * Audio device driver for Wolfson Microelectronics ALC5658 Audio codec.
 *
 *   Copyright (C) 2014, 2016 Gregory Nutt. All rights reserved.
 *   Author:  Gregory Nutt <gnutt@nuttx.org>
 *
 * References:
 * - "ALC5658 Ultra Low Power CODEC for Portable Audio Applications, Pre-
 *    Production", September 2012, Rev 3.3, Wolfson Microelectronics
 *
 * -  The framework for this driver is based on Ken Pettit's VS1053 driver.
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
#include <string.h>
#include <errno.h>
#include <fixedmath.h>
#include <queue.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/kmalloc.h>
#include <tinyara/clock.h>
#include <tinyara/wqueue.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/alc5658.h>
#include <tinyara/math.h>
#include <math.h>
#include <tinyara/i2c.h>

#include "alc5658.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define ALC5658_DEFAULT_SAMPRATE 	48000
#define ALC5658_DEFAULT_NCHANNELS 	2
#define ALC5658_DEFAULT_BPSAMP 		16
#define FAIL	0xFFFF

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#if !defined(CONFIG_ALC5658_REGDUMP) && !defined(CONFIG_ALC5658_CLKDEBUG)
static
#endif
uint16_t alc5658_readreg(FAR struct alc5658_dev_s *priv, uint16_t regaddr);
static void alc5658_writereg(FAR struct alc5658_dev_s *priv, uint16_t regaddr, uint16_t regval);
static void alc5658_takesem(sem_t *sem);
static uint16_t alc5658_modifyreg(FAR struct alc5658_dev_s *priv, uint16_t regaddr, uint16_t set, uint16_t clear);

#define         alc5658_givesem(s) sem_post(s)

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
static inline uint16_t alc5658_scalevolume(uint16_t volume, b16_t scale);
static void alc5658_setvolume(FAR struct alc5658_dev_s *priv, uint16_t volume, bool mute);
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_TONE
static void alc5658_setbass(FAR struct alc5658_dev_s *priv, uint8_t bass);
static void alc5658_settreble(FAR struct alc5658_dev_s *priv, uint8_t treble);
#endif

static void alc5658_setdatawidth(FAR struct alc5658_dev_s *priv);
static void alc5658_setbitrate(FAR struct alc5658_dev_s *priv);

/* Audio lower half methods (and close friends) */

static int alc5658_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps);
#else
static int alc5658_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps);
#endif
static int alc5658_shutdown(FAR struct audio_lowerhalf_s *dev);
static void alc5658_senddone(FAR struct i2s_dev_s *i2s, FAR struct ap_buffer_s *apb, FAR void *arg, int result);
static void alc5658_returnbuffers(FAR struct alc5658_dev_s *priv);
static int alc5658_sendbuffer(FAR struct alc5658_dev_s *priv);

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_start(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int alc5658_start(FAR struct audio_lowerhalf_s *dev);
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int alc5658_stop(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session);
static int alc5658_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int alc5658_pause(FAR struct audio_lowerhalf_s *dev);
static int alc5658_resume(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
static int alc5658_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int alc5658_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int alc5658_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session);
#else
static int alc5658_reserve(FAR struct audio_lowerhalf_s *dev);
#endif
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_release(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int alc5658_release(FAR struct audio_lowerhalf_s *dev);
#endif

/* Interrupt handling an worker thread */

#ifdef ALC5658_USE_FFLOCK_INT
static void alc5658_interrupt_work(FAR void *arg);
static int alc5658_interrupt(FAR const struct alc5658_lower_s *lower, FAR void *arg);
#endif

static void *alc5658_workerthread(pthread_addr_t pvarg);

/* Initialization */

static void alc5658_audio_output(FAR struct alc5658_dev_s *priv);
static void alc5658_audio_input(FAR struct alc5658_dev_s *priv);
#ifdef ALC5658_USE_FFLOCK_INT
static void alc5658_configure_ints(FAR struct alc5658_dev_s *priv);
#else
#define       alc5658_configure_ints(p)
#endif
static void alc5658_hw_reset(FAR struct alc5658_dev_s *priv);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct audio_ops_s g_audioops = {
	alc5658_getcaps,			/* getcaps        */
	alc5658_configure,			/* configure      */
	alc5658_shutdown,			/* shutdown       */
	alc5658_start,				/* start          */
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	alc5658_stop,				/* stop           */
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
	alc5658_pause,				/* pause          */
	alc5658_resume,				/* resume         */
#endif
	NULL,						/* allocbuffer    */
	NULL,						/* freebuffer     */
	alc5658_enqueuebuffer,		/* enqueue_buffer */
	alc5658_cancelbuffer,		/* cancel_buffer  */
	alc5658_ioctl,				/* ioctl          */
	NULL,						/* read           */
	NULL,						/* write          */
	alc5658_reserve,			/* reserve        */
	alc5658_release				/* release        */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void delay(unsigned int mS)
{
	volatile systime_t start = clock_systimer();

	mS = mS / MSEC_PER_TICK + 1;

	while (1)
		if ((start + mS) < clock_systimer()) {
			return;
		}
}

/****************************************************************************
 * Name: alc5658_readreg
 *
 * Description
 *    Read the specified 16-bit register from the ALC5658 device.
 *
 ****************************************************************************/

#if !defined(CONFIG_ALC5658_REGDUMP) && !defined(CONFIG_ALC5658_CLKDEBUG)
static
#endif
uint16_t alc5658_readreg(FAR struct alc5658_dev_s *priv, uint16_t regaddr)
{

	int32_t ret;
	uint8_t reg[2];
	uint16_t regval;
	FAR struct i2c_dev_s *dev = priv->i2c;
	FAR struct i2c_config_s *alc5658_i2c_config = &(priv->lower->i2c_config);

	reg[0] = ((char *)&regaddr)[1];
	reg[1] = ((char *)&regaddr)[0];

	ret = i2c_write(dev, alc5658_i2c_config, reg, 2);
	if (ret < 0) {
		auddbg("Error, cannot read reg %x\n", regaddr);
		return FAIL;
	}

	ret = i2c_read(dev, alc5658_i2c_config, reg, 2);
	if (ret < 0) {
		auddbg("Error, cannot read reg %x\n", regaddr);
		return FAIL;
	}

	regval = ((uint16_t) reg[0] << 8) | (uint16_t) reg[1];

	return regval;
}

/************************************************************************************
 * Name: alc5658_writereg
 *
 * Description:
 *   Write the specified 16-bit register to the ALC5658 device.
 *
 ************************************************************************************/

static void alc5658_writereg(FAR struct alc5658_dev_s *priv, uint16_t regaddr, uint16_t regval)
{
	int32_t ret;
	uint8_t reg[4];
	FAR struct i2c_dev_s *dev = priv->i2c;
	FAR struct i2c_config_s *alc5658_i2c_config = &(priv->lower->i2c_config);

	reg[0] = ((char *)&regaddr)[1];
	reg[1] = ((char *)&regaddr)[0];

	reg[2] = ((char *)&regval)[1];
	reg[3] = ((char *)&regval)[0];

	ret = i2c_write(dev, alc5658_i2c_config, (uint8_t *) reg, 4);
	if (ret < 0) {
		auddbg("Error, cannot write reg %x\n", regaddr);
		return;
	}

	return;
}

static uint16_t alc5658_modifyreg(FAR struct alc5658_dev_s *priv, uint16_t regaddr, uint16_t set, uint16_t clear)
{
	uint16_t data;

	data = alc5658_readreg(priv, regaddr);
	data &= ~clear;
	data |= set;

	alc5658_writereg(priv, regaddr, data);
	return alc5658_readreg(priv, regaddr);
}

static void alc5658_exec_i2c_script(FAR struct alc5658_dev_s *priv, t_codec_init_script_entry *script, uint32_t size)
{
	uint32_t i;
	uint16_t ret;

	for (i = 0; i < size; i++) {
		ret = alc5658_modifyreg(priv, script[i].addr, script[i].val, 0xFFFF);
		delay(script[i].delay);
	}
}

/************************************************************************************
 * Name: alc5658_takesem
 *
 * Description:
 *  Take a semaphore count, handling the nasty EINTR return if we are interrupted
 *  by a signal.
 *
 ************************************************************************************/

static void alc5658_takesem(sem_t *sem)
{
	int ret;

	do {
		ret = sem_wait(sem);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

/************************************************************************************
 * Name: alc5658_scalevolume
 *
 * Description:
 *   Set the right and left volume values in the ALC5658 device based on the current
 *   volume and balance settings.
 *
 ************************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
static inline uint16_t alc5658_scalevolume(uint16_t volume, b16_t scale)
{
	return b16toi((b16_t) volume * scale);
}
#endif

/************************************************************************************
 * Name: alc5658_setvolume
 *
 * Description:
 *   Set the right and left volume values in the ALC5658 device based on the current
 *   volume and balance settings.
 *
 ************************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
static void alc5658_setvolume(FAR struct alc5658_dev_s *priv, uint16_t volume, bool mute)
{

	audvdbg("volume=%u mute=%u\n", volume, mute);

	/* ADD VOLUME CODE HERE */

	/* Remember the volume level and mute settings */

	priv->volume = volume;
	priv->mute = mute;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_VOLUME */

/************************************************************************************
 * Name: alc5658_setbass
 *
 * Description:
 *   Set the bass level.
 *
 *   The level and range are in whole percentage levels (0-100).
 *
 ************************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
static void alc5658_setbass(FAR struct alc5658_dev_s *priv, uint8_t bass)
{
	audvdbg("bass=%u\n", bass);
}
#endif							/* CONFIG_AUDIO_EXCLUDE_TONE */

/************************************************************************************
 * Name: alc5658_settreble
 *
 * Description:
 *   Set the treble level .
 *
 *   The level and range are in whole percentage levels (0-100).
 *
 ************************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
static void alc5658_settreble(FAR struct alc5658_dev_s *priv, uint8_t treble)
{
	audvdbg("treble=%u\n", treble);
}
#endif							/* CONFIG_AUDIO_EXCLUDE_TONE */

/****************************************************************************
 * Name: alc5658_setdatawidth
 *
 * Description:
 *   Set the 8- 16- 24- bit data modes
 *
 ****************************************************************************/

static void alc5658_setdatawidth(FAR struct alc5658_dev_s *priv)
{
}

/****************************************************************************
 * Name: alc5658_setbitrate
 *
 * Description:
 *
 ****************************************************************************/

static void alc5658_setbitrate(FAR struct alc5658_dev_s *priv)
{

}

/****************************************************************************
 * Name: alc5658_getcaps
 *
 * Description:
 *   Get the audio device capabilities
 *
 ****************************************************************************/

static int alc5658_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps)
{
	/* Validate the structure */

	DEBUGASSERT(caps && caps->ac_len >= sizeof(struct audio_caps_s));
	audvdbg("type=%d ac_type=%d\n", type, caps->ac_type);

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

		default:
			caps->ac_controls.b[0] = AUDIO_SUBFMT_END;
			break;
		}

		break;

	/* Provide capabilities of our OUTPUT unit */

	case AUDIO_TYPE_OUTPUT:

		caps->ac_channels = 2;

		switch (caps->ac_subtype) {
		case AUDIO_TYPE_QUERY:

			/* Report the Sample rates we support */

			caps->ac_controls.b[0] = AUDIO_SAMP_RATE_8K | AUDIO_SAMP_RATE_11K | AUDIO_SAMP_RATE_16K | AUDIO_SAMP_RATE_22K | AUDIO_SAMP_RATE_32K | AUDIO_SAMP_RATE_44K | AUDIO_SAMP_RATE_48K;
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
			break;

		case AUDIO_PU_STEREO_EXTENDER:

			/* Provide capabilities of our Stereo Extender */

			caps->ac_controls.b[0] = AUDIO_STEXT_ENABLE | AUDIO_STEXT_WIDTH;
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

	return caps->ac_len;
}

/****************************************************************************
 * Name: alc5658_configure
 *
 * Description:
 *   Configure the audio device for the specified  mode of operation.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps)
#else
static int alc5658_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps)
#endif
{
#if !defined(CONFIG_AUDIO_EXCLUDE_VOLUME) || !defined(CONFIG_AUDIO_EXCLUDE_TONE)
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)dev;
#endif
	int ret = OK;

	DEBUGASSERT(priv && caps);
	audvdbg("ac_type: %d\n", caps->ac_type);

	/* Process the configure operation */

	switch (caps->ac_type) {
	case AUDIO_TYPE_FEATURE:
		audvdbg("  AUDIO_TYPE_FEATURE\n");

		/* Process based on Feature Unit */

		switch (caps->ac_format.hw) {
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
		case AUDIO_FU_VOLUME: {
			/* Set the volume */

			uint16_t volume = caps->ac_controls.hw[0];
			audvdbg("    Volume: %d\n", volume);

			if (volume >= 0 && volume <= 1000) {
				/* Scale the volume setting to the range {0.. 63} */

				alc5658_setvolume(priv, (63 * volume / 1000), priv->mute);
			} else {
				ret = -EDOM;
			}
		}
		break;
#endif							/* CONFIG_AUDIO_EXCLUDE_VOLUME */

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
		case AUDIO_FU_BASS: {
			/* Set the bass.  The percentage level (0-100) is in the
			 * ac_controls.b[0] parameter.
			 */

			uint8_t bass = caps->ac_controls.b[0];
			audvdbg("    Bass: %d\n", bass);

			if (bass <= 100) {
				alc5658_setbass(priv, bass);
			} else {
				ret = -EDOM;
			}
		}
		break;

		case AUDIO_FU_TREBLE: {
			/* Set the treble.  The percentage level (0-100) is in the
			 * ac_controls.b[0] parameter.
			 */

			uint8_t treble = caps->ac_controls.b[0];
			audvdbg("    Treble: %d\n", treble);

			if (treble <= 100) {
				alc5658_settreble(priv, treble);
			} else {
				ret = -EDOM;
			}
		}
		break;
#endif							/* CONFIG_AUDIO_EXCLUDE_TONE */

		default:
			auddbg("    ERROR: Unrecognized feature unit\n");
			ret = -ENOTTY;
			break;
		}
		break;

	case AUDIO_TYPE_OUTPUT: {
		audvdbg("  AUDIO_TYPE_OUTPUT:\n");
		audvdbg("    Number of channels: %u\n", caps->ac_channels);
		audvdbg("    Sample rate:        %u\n", caps->ac_controls.hw[0]);
		audvdbg("    Sample width:       %u\n", caps->ac_controls.b[2]);

		/* Verify that all of the requested values are supported */

		ret = -ERANGE;
		if (caps->ac_channels != 1 && caps->ac_channels != 2) {
			auddbg("ERROR: Unsupported number of channels: %d\n", caps->ac_channels);
			break;
		}

		if (caps->ac_controls.b[2] != 8 && caps->ac_controls.b[2] != 16) {
			auddbg("ERROR: Unsupported bits per sample: %d\n", caps->ac_controls.b[2]);
			break;
		}

		/* Save the current stream configuration */

		priv->samprate = caps->ac_controls.hw[0];
		priv->nchannels = caps->ac_channels;
		priv->bpsamp = caps->ac_controls.b[2];

		/* Reconfigure the FLL to support the resulting number or channels,
		 * bits per sample, and bitrate.
		 */

		alc5658_setdatawidth(priv);
		alc5658_setbitrate(priv);

		alc5658_clock_analysis(&priv->dev, "AUDIO_TYPE_OUTPUT");
		ret = OK;
	}
	break;

	case AUDIO_TYPE_PROCESSING:
		break;
	}

	return ret;
}

/****************************************************************************
 * Name: alc5658_shutdown
 *
 * Description:
 *   Shutdown the ALC5658 chip and put it in the lowest power state possible.
 *
 ****************************************************************************/

static int alc5658_shutdown(FAR struct audio_lowerhalf_s *dev)
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)dev;

	DEBUGASSERT(priv);

	/* First disable interrupts */

	ALC5658_DISABLE(priv->lower);

	/* Now issue a software reset.  This puts all ALC5658 registers back in
	 * their default state.
	 */

	alc5658_hw_reset(priv);
	return OK;
}

/****************************************************************************
 * Name: alc5658_senddone
 *
 * Description:
 *   This is the I2S callback function that is invoked when the transfer
 *   completes.
 *
 ****************************************************************************/

static void alc5658_senddone(FAR struct i2s_dev_s *i2s, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)arg;
	struct audio_msg_s msg;
	irqstate_t flags;
	int ret;

	DEBUGASSERT(i2s && priv && priv->running && apb);
	audvdbg("apb=%p inflight=%d result=%d\n", apb, priv->inflight, result);

	/* We do not place any restriction on the context in which this function
	 * is called.  It may be called from an interrupt handler.  Therefore, the
	 * doneq and in-flight values might be accessed from the interrupt level.
	 * Not the best design.  But we will use interrupt controls to protect
	 * against that possibility.
	 */

	flags = irqsave();

	/* Add the completed buffer to the end of our doneq.  We do not yet
	 * decrement the reference count.
	 */

	dq_addlast((FAR dq_entry_t *) apb, &priv->doneq);

	/* And decrement the number of buffers in-flight */

	DEBUGASSERT(priv->inflight > 0);
	priv->inflight--;

	/* Save the result of the transfer */
	/* REVISIT:  This can be overwritten */

	priv->result = result;
	irqrestore(flags);

	/* Now send a message to the worker thread, informing it that there are
	 * buffers in the done queue that need to be cleaned up.
	 */

	msg.msgId = AUDIO_MSG_COMPLETE;
	ret = mq_send(priv->mq, (FAR const char *)&msg, sizeof(msg), CONFIG_ALC5658_MSG_PRIO);
	if (ret < 0) {
		auddbg("ERROR: mq_send failed: %d\n", errno);
	}
}

/****************************************************************************
 * Name: alc5658_returnbuffers
 *
 * Description:
 *   This function is called after the complete of one or more data
 *   transfers.  This function will empty the done queue and release our
 *   reference to each buffer.
 *
 ****************************************************************************/

static void alc5658_returnbuffers(FAR struct alc5658_dev_s *priv)
{
	FAR struct ap_buffer_s *apb;
	irqstate_t flags;

	/* The doneq and in-flight values might be accessed from the interrupt
	 * level in some implementations.  Not the best design.  But we will
	 * use interrupt controls to protect against that possibility.
	 */

	flags = irqsave();
	while (dq_peek(&priv->doneq) != NULL) {
		/* Take the next buffer from the queue of completed transfers */

		apb = (FAR struct ap_buffer_s *)dq_remfirst(&priv->doneq);
		irqrestore(flags);

		audvdbg("Returning: apb=%p curbyte=%d nbytes=%d flags=%04x\n", apb, apb->curbyte, apb->nbytes, apb->flags);

		/* Are we returning the final buffer in the stream? */

		if ((apb->flags & AUDIO_APB_FINAL) != 0) {
			/* Both the pending and the done queues should be empty and there
			 * should be no buffers in-flight.
			 */

			DEBUGASSERT(dq_empty(&priv->doneq) && dq_empty(&priv->pendq) && priv->inflight == 0);

			/* Set the terminating flag.  This will, eventually, cause the
			 * worker thread to exit (if it is not already terminating).
			 */

			audvdbg("Terminating\n");
			priv->terminating = true;
		}

		/* Release our reference to the audio buffer */

		apb_free(apb);

		/* Send the buffer back up to the previous level. */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK, NULL);
#else
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK);
#endif
		flags = irqsave();
	}

	irqrestore(flags);
}

/****************************************************************************
 * Name: alc5658_sendbuffer
 *
 * Description:
 *   Start the transfer an audio buffer to the ALC5658 via I2S.  This
 *   will not wait for the transfer to complete but will return immediately.
 *   the alc5658 called will be invoked when the transfer
 *   completes, stimulating the worker thread to call this function again.
 *
 ****************************************************************************/

static int alc5658_sendbuffer(FAR struct alc5658_dev_s *priv)
{
	FAR struct ap_buffer_s *apb;
	irqstate_t flags;
	uint32_t timeout;
	int shift;
	int ret = OK;

	/* Loop while there are audio buffers to be sent and we have few than
	 * CONFIG_ALC5658_INFLIGHT then "in-flight"
	 *
	 * The 'inflight' value might be modified from the interrupt level in some
	 * implementations.  We will use interrupt controls to protect against
	 * that possibility.
	 *
	 * The 'pendq', on the other hand, is protected via a semaphore.  Let's
	 * hold the semaphore while we are busy here and disable the interrupts
	 * only while accessing 'inflight'.
	 */

	alc5658_takesem(&priv->pendsem);
	while (priv->inflight < CONFIG_ALC5658_INFLIGHT && dq_peek(&priv->pendq) != NULL && !priv->paused) {
		/* Take next buffer from the queue of pending transfers */

		apb = (FAR struct ap_buffer_s *)dq_remfirst(&priv->pendq);
		audvdbg("Sending apb=%p, size=%d inflight=%d\n", apb, apb->nbytes, priv->inflight);

		/* Increment the number of buffers in-flight before sending in order
		 * to avoid a possible race condition.
		 */

		flags = irqsave();
		priv->inflight++;
		irqrestore(flags);

		/* Send the entire audio buffer via I2S.  What is a reasonable timeout
		 * to use?  This would depend on the bit rate and size of the buffer.
		 *
		 * Samples in the buffer (samples):
		 *   = buffer_size * 8 / bpsamp                           samples
		 * Sample rate (samples/second):
		 *   = samplerate * nchannels
		 * Expected transfer time (seconds):
		 *   = (buffer_size * 8) / bpsamp / samplerate / nchannels
		 *
		 * We will set the timeout about twice that.
		 *
		 * NOTES:
		 * - The multiplier of 8 becomes 16000 for 2x and units of
		 *   milliseconds.
		 * - 16000 is a approximately 16384 (1 << 14), bpsamp is either
		 *   (1 << 3) or (1 << 4), and nchannels is either (1 << 0) or
		 *   (1 << 1).  So this can be simplifies to (milliseconds):
		 *
		 *   = (buffer_size << shift) / samplerate
		 */

		shift = (priv->bpsamp == 8) ? 14 - 3 : 14 - 4;
		shift -= (priv->nchannels > 1) ? 1 : 0;

		timeout = MSEC2TICK(((uint32_t)(apb->nbytes - apb->curbyte) << shift) / (uint32_t) priv->samprate);

		ret = I2S_SEND(priv->i2s, apb, alc5658_senddone, priv, timeout);
		if (ret < 0) {
			auddbg("ERROR: I2S_SEND failed: %d\n", ret);
			break;
		}
	}

	alc5658_givesem(&priv->pendsem);
	return ret;
}

/****************************************************************************
 * Name: alc5658_start
 *
 * Description:
 *   Start the configured operation (audio streaming, volume enabled, etc.).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_start(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int alc5658_start(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)dev;
	struct sched_param sparam;
	struct mq_attr attr;
	pthread_attr_t tattr;
	FAR void *value;
	int ret;

	audvdbg("Entry\n");

	/* Exit reduced power modes of operation */
	/* REVISIT */

	/* Create a message queue for the worker thread */

	snprintf(priv->mqname, sizeof(priv->mqname), "/tmp/%X", priv);

	attr.mq_maxmsg = 16;
	attr.mq_msgsize = sizeof(struct audio_msg_s);
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;

	priv->mq = mq_open(priv->mqname, O_RDWR | O_CREAT, 0644, &attr);
	if (priv->mq == NULL) {
		/* Error creating message queue! */

		auddbg("ERROR: Couldn't allocate message queue\n");
		return -ENOMEM;
	}

	/* Join any old worker thread we had created to prevent a memory leak */

	if (priv->threadid != 0) {
		audvdbg("Joining old thread\n");
		pthread_join(priv->threadid, &value);
	}

	/* Start our thread for sending data to the device */

	pthread_attr_init(&tattr);
	sparam.sched_priority = sched_get_priority_max(SCHED_FIFO) - 3;
	(void)pthread_attr_setschedparam(&tattr, &sparam);
	(void)pthread_attr_setstacksize(&tattr, CONFIG_ALC5658_WORKER_STACKSIZE);

	audvdbg("Starting worker thread\n");
	ret = pthread_create(&priv->threadid, &tattr, alc5658_workerthread, (pthread_addr_t) priv);
	if (ret != OK) {
		auddbg("ERROR: pthread_create failed: %d\n", ret);
	} else {
		pthread_setname_np(priv->threadid, "alc5658");
		audvdbg("Created worker thread\n");
	}

	return ret;
}

/****************************************************************************
 * Name: alc5658_stop
 *
 * Description: Stop the configured operation (audio streaming, volume
 *              disabled, etc.).
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int alc5658_stop(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)dev;
	struct audio_msg_s term_msg;
	FAR void *value;

	/* Send a message to stop all audio streaming */

	term_msg.msgId = AUDIO_MSG_STOP;
	term_msg.u.data = 0;
	mq_send(priv->mq, (FAR const char *)&term_msg, sizeof(term_msg), CONFIG_ALC5658_MSG_PRIO);

	/* Join the worker thread */

	pthread_join(priv->threadid, &value);
	priv->threadid = 0;

	/* Enter into a reduced power usage mode */
	/* REVISIT: */

	return OK;
}
#endif

/****************************************************************************
 * Name: alc5658_pause
 *
 * Description: Pauses the playback.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int alc5658_pause(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)dev;

	if (priv->running && !priv->paused) {
		/* Disable interrupts to prevent us from suppling any more data */

		priv->paused = true;
		alc5658_setvolume(priv, priv->volume, true);
		ALC5658_DISABLE(priv->lower);
	}

	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: alc5658_resume
 *
 * Description: Resumes the playback.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int alc5658_resume(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)dev;

	if (priv->running && priv->paused) {
		priv->paused = false;
		alc5658_setvolume(priv, priv->volume, false);

		/* Enable interrupts to allow sampling data */

		alc5658_sendbuffer(priv);
#ifdef ALC5658_USE_FFLOCK_INT
		ALC5658_ENABLE(priv->lower);
#endif
	}

	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: alc5658_enqueuebuffer
 *
 * Description: Enqueue an Audio Pipeline Buffer for playback/ processing.
 *
 ****************************************************************************/

static int alc5658_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)dev;
	struct audio_msg_s term_msg;
	int ret;

	audvdbg("Enqueueing: apb=%p curbyte=%d nbytes=%d flags=%04x\n", apb, apb->curbyte, apb->nbytes, apb->flags);

	/* Take a reference on the new audio buffer */

	apb_reference(apb);

	/* Add the new buffer to the tail of pending audio buffers */

	alc5658_takesem(&priv->pendsem);
	apb->flags |= AUDIO_APB_OUTPUT_ENQUEUED;
	dq_addlast(&apb->dq_entry, &priv->pendq);
	alc5658_givesem(&priv->pendsem);

	/* Send a message to the worker thread indicating that a new buffer has been
	 * enqueued.  If mq is NULL, then the playing has not yet started.  In that
	 * case we are just "priming the pump" and we don't need to send any message.
	 */

	ret = OK;
	if (priv->mq != NULL) {
		term_msg.msgId = AUDIO_MSG_ENQUEUE;
		term_msg.u.data = 0;

		ret = mq_send(priv->mq, (FAR const char *)&term_msg, sizeof(term_msg), CONFIG_ALC5658_MSG_PRIO);
		if (ret < 0) {
			int errcode = errno;
			DEBUGASSERT(errcode > 0);

			auddbg("ERROR: mq_send failed: %d\n", errcode);
			UNUSED(errcode);
		}
	}

	return ret;
}

/****************************************************************************
 * Name: alc5658_cancelbuffer
 *
 * Description: Called when an enqueued buffer is being cancelled.
 *
 ****************************************************************************/

static int alc5658_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	audvdbg("apb=%p\n", apb);
	return OK;
}

/****************************************************************************
 * Name: alc5658_ioctl
 *
 * Description: Perform a device ioctl
 *
 ****************************************************************************/

static int alc5658_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg)
{
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	FAR struct ap_buffer_info_s *bufinfo;
#endif

	/* Deal with ioctls passed from the upper-half driver */

	switch (cmd) {
	/* Check for AUDIOIOC_HWRESET ioctl.  This ioctl is passed straight
	 * through from the upper-half audio driver.
	 */

	case AUDIOIOC_HWRESET: {
		/* REVISIT:  Should we completely re-initialize the chip?   We
		 * can't just issue a software reset; that would puts all ALC5658
		 * registers back in their default state.
		 */

		audvdbg("AUDIOIOC_HWRESET:\n");
	}
	break;

		/* Report our preferred buffer size and quantity */

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	case AUDIOIOC_GETBUFFERINFO: {
		audvdbg("AUDIOIOC_GETBUFFERINFO:\n");
		bufinfo = (FAR struct ap_buffer_info_s *)arg;
		bufinfo->buffer_size = CONFIG_ALC5658_BUFFER_SIZE;
		bufinfo->nbuffers = CONFIG_ALC5658_NUM_BUFFERS;
	}
	break;
#endif

	default:
		audvdbg("Ignored\n");
		break;
	}

	return OK;
}

/****************************************************************************
 * Name: alc5658_reserve
 *
 * Description: Reserves a session (the only one we have).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session)
#else
static int alc5658_reserve(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)dev;
	int ret = OK;

	/* Borrow the APBQ semaphore for thread sync */

	alc5658_takesem(&priv->pendsem);
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

	alc5658_givesem(&priv->pendsem);

	return ret;
}

/****************************************************************************
 * Name: alc5658_release
 *
 * Description: Releases the session (the only one we have).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc5658_release(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int alc5658_release(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)dev;
	void *value;

	/* Join any old worker thread we had created to prevent a memory leak */

	if (priv->threadid != 0) {
		pthread_join(priv->threadid, &value);
		priv->threadid = 0;
	}

	/* Borrow the APBQ semaphore for thread sync */

	alc5658_takesem(&priv->pendsem);

	/* Really we should free any queued buffers here */

	priv->reserved = false;
	alc5658_givesem(&priv->pendsem);

	return OK;
}

/****************************************************************************
 * Name: alc5658_interrupt_work
 *
 * Description:
 *   ALC5658 interrupt actions cannot be performed in the interrupt handler
 *   because I2C access is not possible in that context.  Instead, all I2C
 *   operations are deferred to the work queue.
 *
 * Assumptions:
 *   ALC5658 interrupts were disabled in the interrupt handler.
 *
 ****************************************************************************/

#ifdef ALC5658_USE_FFLOCK_INT
static void alc5658_interrupt_work(FAR void *arg)
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)arg;
	uint16_t regval;

	DEBUGASSERT(priv && priv->lower);

	/* Sample the interrupt status */

	regval = alc5658_readreg(priv, ALC5658_INT_STATUS);
	audvdbg("INT_STATUS: %04x\n", regval);

	/* Check for the FLL lock interrupt.  We are sloppy here since at
	 * present, only the FLL lock interrupt is used.
	 */

	DEBUGASSERT((regval & ALC5658_FLL_LOCK_INT) != 0 && !priv->locked);
	UNUSED(regval);

	priv->locked = true;

	/* Clear all pending interrupts by write 1's to the interrupt status
	 * register.
	 *
	 * REVISIT: Since I2C is slow and not atomic with respect to ALC5658 event,
	 * could this not cause the lost of interrupts?
	 */

	alc5658_writereg(priv, ALC5658_INT_STATUS, ALC5658_ALL_INTS);

	/* Disable further FLL lock interrupts.  We are sloppy here since at
	 * present, only the FLL lock interrupt is used.
	 */

	alc5658_writereg(priv, ALC5658_INT_MASK, ALC5658_ALL_INTS);

#ifdef ALC5658_USE_FFLOCK_INT
	/* Re-enable ALC5658 interrupts */

	ALC5658_ENABLE(priv->lower);
#endif
}
#endif

/****************************************************************************
 * Name: alc5658_interrupt
 *
 * Description:
 *   This is the ISR that services the GPIO1/IRQ pin from the ALC5658.  It
 *   signals ALC5658 events such FLL lock.
 *
 ****************************************************************************/

#ifdef ALC5658_USE_FFLOCK_INT
static int alc5658_interrupt(FAR const struct alc5658_lower_s *lower, FAR void *arg)
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)arg;
	int ret;

	DEBUGASSERT(lower && priv);

	/* Disable further interrupts and perform all interrupt related activities
	 * on the work thread.  There is nothing that we can do from the interrupt
	 * handler because we cannot perform I2C operations here.
	 */

	ALC5658_DISABLE(priv->lower);

	DEBUGASSERT(work_available(&priv->work));
	ret = work_queue(LPWORK, &priv->work, alc5658_interrupt_work, priv, 0);
	if (ret < 0) {
		auddbg("ERROR: Failed to schedule work\n");
	}

	return OK;
}
#endif

/****************************************************************************
 * Name: alc5658_workerthread
 *
 *  This is the thread that feeds data to the chip and keeps the audio
 *  stream going.
 *
 ****************************************************************************/

static void *alc5658_workerthread(pthread_addr_t pvarg)
{
	FAR struct alc5658_dev_s *priv = (struct alc5658_dev_s *)pvarg;
	struct audio_msg_s msg;
	FAR struct ap_buffer_s *apb;
	int msglen;
	int prio;

	audvdbg("Entry\n");

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	priv->terminating = false;
#endif

	/* Mark ourself as running and make sure that ALC5658 interrupts are
	 * enabled.
	 */

	priv->running = true;
#ifdef ALC5658_USE_FFLOCK_INT
	ALC5658_ENABLE(priv->lower);
#endif
	alc5658_setvolume(priv, priv->volume, false);

	/* Loop as long as we are supposed to be running and as long as we have
	 * buffers in-flight.
	 */

	while (priv->running || priv->inflight > 0) {
		/* Check if we have been asked to terminate.  We have to check if we
		 * still have buffers in-flight.  If we do, then we can't stop until
		 * birds come back to roost.
		 */

		if (priv->terminating && priv->inflight <= 0) {
			/* We are IDLE.  Break out of the loop and exit. */

			break;
		} else {
			/* Check if we can send more audio buffers to the ALC5658 */

			alc5658_sendbuffer(priv);
		}

		/* Wait for messages from our message queue */

		msglen = mq_receive(priv->mq, (FAR char *)&msg, sizeof(msg), &prio);

		/* Handle the case when we return with no message */

		if (msglen < sizeof(struct audio_msg_s)) {
			auddbg("ERROR: Message too small: %d\n", msglen);
			continue;
		}

		/* Process the message */

		switch (msg.msgId) {
		/* The ISR has requested more data.  We will catch this case at
		 * the top of the loop.
		 */

		case AUDIO_MSG_DATA_REQUEST:
			audvdbg("AUDIO_MSG_DATA_REQUEST\n");
			break;

			/* Stop the playback */

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
		case AUDIO_MSG_STOP:
			/* Indicate that we are terminating */

			audvdbg("AUDIO_MSG_STOP: Terminating\n");
			priv->terminating = true;
			break;
#endif

		/* We have a new buffer to send.  We will catch this case at
		 * the top of the loop.
		 */

		case AUDIO_MSG_ENQUEUE:
			audvdbg("AUDIO_MSG_ENQUEUE\n");
			break;

		/* We will wake up from the I2S callback with this message */

		case AUDIO_MSG_COMPLETE:
			audvdbg("AUDIO_MSG_COMPLETE\n");
			alc5658_returnbuffers(priv);
			break;

		default:
			auddbg("ERROR: Ignoring message ID %d\n", msg.msgId);
			break;
		}
	}

	/* Reset the ALC5658 hardware */

	alc5658_hw_reset(priv);

	/* Return any pending buffers in our pending queue */

	alc5658_takesem(&priv->pendsem);
	while ((apb = (FAR struct ap_buffer_s *)dq_remfirst(&priv->pendq)) != NULL) {
		/* Release our reference to the buffer */

		apb_free(apb);

		/* Send the buffer back up to the previous level. */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK, NULL);
#else
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK);
#endif
	}

	alc5658_givesem(&priv->pendsem);

	/* Return any pending buffers in our done queue */

	alc5658_returnbuffers(priv);

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
 * Name: alc5658_audio_output
 *
 * Description:
 *   Initialize and configure the ALC5658 device as an audio output device.
 *
 * Input Parameters:
 *   priv - A reference to the driver state structure
 *
 * Returned Value:
 *   None.  No failures are detected.
 *
 ****************************************************************************/

static void alc5658_audio_output(FAR struct alc5658_dev_s *priv)
{
	alc5658_exec_i2c_script(priv, codec_init_script, sizeof(codec_init_script) / sizeof(t_codec_init_script_entry));
}

/****************************************************************************
 * Name: alc5658_audio_input
 *
 * Description:
 *   Initialize and configure the ALC5658 device as an audio input device
 *
 * Input Parameters:
 *   priv - A reference to the driver state structure
 *
 * Returned Value:
 *   None.  No failures are detected.
 *
 ****************************************************************************/

static void alc5658_audio_input(FAR struct alc5658_dev_s *priv)
{
}

/****************************************************************************
 * Name: alc5658_configure_ints
 *
 * Description:
 *   Configure the GPIO/IRQ interrupt
 *
 * Input Parameters:
 *   priv - A reference to the driver state structure
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef ALC5658_USE_FFLOCK_INT
static void alc5658_configure_ints(FAR struct alc5658_dev_s *priv)
{
}
#endif

/****************************************************************************
 * Name: alc5658_hw_reset
 *
 * Description:
 *   Reset and re-initialize the ALC5658
 *
 * Input Parameters:
 *   priv - A reference to the driver state structure
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void alc5658_hw_reset(FAR struct alc5658_dev_s *priv)
{

	/* Put audio output back to its initial configuration */

	priv->samprate = ALC5658_DEFAULT_SAMPRATE;
	priv->nchannels = ALC5658_DEFAULT_NCHANNELS;
	priv->bpsamp = ALC5658_DEFAULT_BPSAMP;
#if !defined(CONFIG_AUDIO_EXCLUDE_VOLUME) && !defined(CONFIG_AUDIO_EXCLUDE_BALANCE)
	priv->balance = b16HALF;	/* Center balance */
#endif

	/* Software reset.  This puts all ALC5658 registers back in their
	 * default state.
	 */

	alc5658_exec_i2c_script(priv, codec_reset_script, sizeof(codec_reset_script) / sizeof(t_codec_init_script_entry));

	alc5658_audio_output(priv);

	/* Configure interrupts */

	/* Configure the FLL and the LRCLK */

	alc5658_setbitrate(priv);

	/* Dump some information and return the device instance */

	alc5658_dump_registers(&priv->dev, "After configuration");
	alc5658_clock_analysis(&priv->dev, "After configuration");
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: alc5658_initialize
 *
 * Description:
 *   Initialize the ALC5658 device.
 *
 * Input Parameters:
 *   i2c     - An I2C driver instance
 *   i2s     - An I2S driver instance
 *   lower   - Persistent board configuration data
 *
 * Returned Value:
 *   A new lower half audio interface for the ALC5658 device is returned on
 *   success; NULL is returned on failure.
 *
 ****************************************************************************/

FAR struct audio_lowerhalf_s *alc5658_initialize(FAR struct i2c_dev_s *i2c, FAR struct i2s_dev_s *i2s, FAR struct alc5658_lower_s *lower)
{

	FAR struct alc5658_dev_s *priv;
	uint16_t regval;
	/* Sanity check */
	DEBUGASSERT(i2c && i2s && lower);

	/* Allocate a ALC5658 device structure */
	priv = (FAR struct alc5658_dev_s *)kmm_zalloc(sizeof(struct alc5658_dev_s));
	if (priv) {
		/* Initialize the ALC5658 device structure.  Since we used kmm_zalloc,
		 * only the non-zero elements of the structure need to be initialized.
		 */

		priv->dev.ops = &g_audioops;
		priv->lower = lower;
		priv->i2c = i2c;
		priv->i2s = i2s;

		sem_init(&priv->pendsem, 0, 1);
		dq_init(&priv->pendq);
		dq_init(&priv->doneq);

		/* Software reset.  This puts all ALC5658 registers back in their
		 * default state.
		 */

		alc5658_writereg(priv, ALC5658_RESET, 0);
		alc5658_dump_registers(&priv->dev, "After reset");

		/* Verify that ALC5658 is present and available on this I2C */

		regval = alc5658_readreg(priv, ALC5658_RESET);
		if (regval != 0) {
			auddbg("ERROR: ALC5658 not found: ID=%04x\n", regval);
			goto errout_with_dev;
		}

		/* Reset and reconfigure the ALC5658 hardwaqre */

		alc5658_hw_reset(priv);
		return &priv->dev;
	}

	return NULL;

errout_with_dev:
	sem_destroy(&priv->pendsem);
	kmm_free(priv);
	return NULL;
}
