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
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <fixedmath.h>
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
#include "alc5658scripts.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

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
 * Name: delay
 *
 * Description
 *    Delay in ms.
 ****************************************************************************/
static void delay(unsigned int mS)
{
	volatile systime_t start = clock_systimer();
	mS = mS / MSEC_PER_TICK + 1;

	while (1) {
		if ((start + mS) < clock_systimer())
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

/************************************************************************************
 * Name: alc5658_modifyreg
 *
 * Description:
 *   Modify the specified 16-bit register to the ALC5658 device.
 *
 ************************************************************************************/
static uint16_t alc5658_modifyreg(FAR struct alc5658_dev_s *priv, uint16_t regaddr, uint16_t set, uint16_t clear)
{
	uint16_t data;

	data = alc5658_readreg(priv, regaddr);
	data &= ~clear;
	data |= set;

	alc5658_writereg(priv, regaddr, data);
	return alc5658_readreg(priv, regaddr);
}

/************************************************************************************
 * Name: alc5658_exec_i2c_script
 *
 * Description:
 *   Executes given script through i2c to configuure ALC5658 device.
 *
 ************************************************************************************/
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

	audvdbg(" alc5658_setvolume volume=%u mute=%u\n", volume, mute);
	/* ADD VOLUME CODE HERE */

	/* Remember the volume level and mute settings */
	alc5658_takesem(&priv->devsem);
	priv->volume = volume;
	priv->mute = mute;
	alc5658_givesem(&priv->devsem);
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
 * Name: alc5658_set_i2s_datawidth
 *
 * Description:
 *   Set the 8- 16- 24- bit data modes
 *
 ****************************************************************************/
static void alc5658_set_i2s_datawidth(FAR struct alc5658_dev_s *priv)
{
	if (priv->inout)
		I2S_RXDATAWIDTH(priv->i2s, priv->bpsamp);
	else
		I2S_TXDATAWIDTH(priv->i2s, priv->bpsamp);
}

/****************************************************************************
 * Name: alc5658_set_i2s_samplerate
 *
 * Description:
 *
 ****************************************************************************/
static void alc5658_set_i2s_samplerate(FAR struct alc5658_dev_s *priv)
{
	if (priv->inout)
		I2S_RXSAMPLERATE(priv->i2s, priv->samprate);
	else
		I2S_TXSAMPLERATE(priv->i2s, priv->samprate);
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
			caps->ac_controls.b[0] = AUDIO_TYPE_OUTPUT | AUDIO_TYPE_OUTPUT | AUDIO_TYPE_FEATURE | AUDIO_TYPE_PROCESSING;

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

	/* ALC5658 supports on the fly changes for almost all changes
	so no need to do anything. But if any issue, worth looking here */

	switch (caps->ac_type) {
	case AUDIO_TYPE_FEATURE:
		audvdbg("  AUDIO_TYPE_FEATURE\n");

		/* Inner swich case: Process based on Feature Unit */
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
		case AUDIO_FU_BASS:
		{	/* Set the bass.  The percentage level (0-100) is in the
			 * ac_controls.b[0] parameter. */

			uint8_t bass = caps->ac_controls.b[0];
			audvdbg("    Bass: %d\n", bass);
			if (bass <= 100)
				alc5658_setbass(priv, bass);
		}
		break;
		case AUDIO_FU_TREBLE:
		{	/* Set the treble.  The percentage level (0-100) is in the
			 * ac_controls.b[0] parameter. */

			uint8_t treble = caps->ac_controls.b[0];
			audvdbg("    Treble: %d\n", treble);
			if (treble <= 100)
				alc5658_settreble(priv, treble);
		}
		break;
#endif							/* CONFIG_AUDIO_EXCLUDE_TONE */
		default:
			auddbg("    ERROR: Unrecognized feature unit\n");
			break;
		}
		break; /* Break for inner switch case */
	case AUDIO_TYPE_INPUT: {
		audvdbg("  AUDIO_TYPE_INPUT:\n");
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

		audvdbg("    Number of channels: 0x%x\n", priv->nchannels);
		audvdbg("    Sample rate:        0x%x\n", priv->samprate);
		audvdbg("    Sample width:       0x%x\n", priv->bpsamp);

		/* Reconfigure the FLL to support the resulting number or channels,
		 * bits per sample, and bitrate.
		 */
#if 0
		alc5658_set_i2s_datawidth(priv);
		alc5658_set_i2s_samplerate(priv);

		alc5658_clock_analysis(&priv->dev, "AUDIO_TYPE_OUTPUT");
#endif
		priv->inout = true;
		ret = OK;
	}
	case AUDIO_TYPE_OUTPUT: {
		audvdbg("  AUDIO_TYPE_OUTPUT:\n");
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

		audvdbg("    Number of channels: 0x%x\n", priv->nchannels);
		audvdbg("    Sample rate:        0x%x\n", priv->samprate);
		audvdbg("    Sample width:       0x%x\n", priv->bpsamp);

		/* Reconfigure the FLL to support the resulting number or channels,
		 * bits per sample, and bitrate.
		 */
#if 0
		alc5658_set_i2s_datawidth(priv);
		alc5658_set_i2s_samplerate(priv);

		alc5658_clock_analysis(&priv->dev, "AUDIO_TYPE_OUTPUT");
#endif
		ret = OK;
		priv->inout = false;
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

	if (priv->running)
		return OK;

	audvdbg(" alc5658_start Entry\n");
	
	/* Fix me -- Need to support multiple samplerate */
	alc5658_exec_i2c_script(priv, codec_init_pll_16K, sizeof(codec_init_pll_16K) / sizeof(t_codec_init_script_entry));
	alc5658_exec_i2c_script(priv, codec_init_inout_script2, sizeof(codec_init_inout_script2) / sizeof(t_codec_init_script_entry));

	alc5658_takesem(&priv->devsem);
	
	priv->running = 1;
	dq_entry_t *tmp = NULL;
	dq_queue_t * q = &priv->pendq;

	for (tmp = dq_peek(q); tmp; tmp = dq_next(tmp)) {
		alc5658_enqueuebuffer(dev, (struct ap_buffer_s *) tmp);
	}

	alc5658_givesem(&priv->devsem);
	
	/* Exit reduced power modes of operation */
	/* REVISIT */

	return OK; /* Fix this -- always returns OK */
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

	/* Need to run the stop script here */
	alc5658_exec_i2c_script(priv, codec_stop_script, sizeof(codec_stop_script) / sizeof(t_codec_init_script_entry));

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
		ALC5658_DISABLE(priv->lower); /* Need inputs from REALTEK */
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
		/* Need resume logic later. Need to know if alc5658 dma can be paused and resumed */
#ifdef ALC5658_USE_FFLOCK_INT
		ALC5658_ENABLE(priv->lower);
#endif
	}

	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: alc5658_rxcallback
 *
 * Description: Called when I2S filled a buffer. No recycling mechanism now.
 *
 ****************************************************************************/

static void alc5658_rxtxcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)arg;

	DEBUGASSERT(priv &&  apb);
	audvdbg("alc5658_rxcallback, devaddr= 0x%x, apbaddr  =0x%x\n", dev, apb);
	
	/* Call upper callback, let it post msg to user q */
	priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, OK);

	alc5658_takesem(&priv->devsem);
	dq_entry_t *tmp;
	for (tmp = (dq_entry_t*)dq_peek(&priv->pendq); tmp; tmp = dq_next(tmp)) {
		if (tmp == (dq_entry_t*)apb) {
			dq_rem(tmp, &priv->pendq);
			apb_free(apb); /* let the reference gained in enqueue */
			audvdbg("found the apb to remove 0x%x\n", tmp);
			break;
		}
	}
	alc5658_givesem(&priv->devsem);
}

/****************************************************************************
 * Name: alc5658_enqueuebuffer
 *
 * Description: Enqueue an Audio Pipeline Buffer for playback/ processing.
 *
 ****************************************************************************/
static int alc5658_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	FAR struct alc5658_dev_s *priv = (FAR struct alc5658_dev_s *)dev;
	int ret;

	audvdbg("alc5658_enqueuebuffer: apbadr = 0x%x\n", apb);

	/* Need to fix later */
	if (!priv->running) {
		apb_reference(apb);

		/* Add the new buffer to the tail of pending audio buffers */
		alc5658_takesem(&priv->devsem);
		dq_addlast(&apb->dq_entry, &priv->pendq);
		audvdbg("enqueue added buf 0x%x\n", apb);
		alc5658_givesem(&priv->devsem);
		return OK;
	}
	
	if (priv->inout) /* record */
	ret = I2S_RECEIVE(priv->i2s, apb, alc5658_rxtxcallback, priv, 0);
	else			/* playback */
	ret = I2S_SEND(priv->i2s, apb, alc5658_rxtxcallback, priv, 0);

	audvdbg("I2s  returned 0x%x\n", ret);
	
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
	/* Need to add logic here */
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

	alc5658_takesem(&priv->devsem);
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

	alc5658_givesem(&priv->devsem);

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

	alc5658_takesem(&priv->devsem);
	priv->reserved = false;
	alc5658_givesem(&priv->devsem);

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
	alc5658_exec_i2c_script(priv, codec_init_out_script, sizeof(codec_init_out_script) / sizeof(t_codec_init_script_entry));
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
	alc5658_exec_i2c_script(priv, codec_init_in_script, sizeof(codec_init_in_script) / sizeof(t_codec_init_script_entry));
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
	/* Missing */
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

	alc5658_set_i2s_samplerate(priv);

	/* NOt sure if htis is needed */
	alc5658_writereg(priv, ALC5658_IN1_CTRL, (10 + 16) << 8);
	audvdbg("MIC GAIN 0x%x\n", (uint32_t) alc5658_readreg(priv, ALC5658_IN1_CTRL));

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

		sem_init(&priv->devsem, 0, 1);
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
	sem_destroy(&priv->devsem);
	kmm_free(priv);
	return NULL;
}
