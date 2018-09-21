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
 * drivers/audio/tas5749.c
 *
 * Audio device driver for TI TAS5749 Audio codec.
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
#include <stdbool.h>
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
#include <tinyara/audio/tas5749.h>
#include <tinyara/math.h>
#include <math.h>
#include <tinyara/i2c.h>

#include "tas5749.h"
#include "tas5749scripts.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef CONFIG_TAS5749_I2S_TIMEOUT
#define TAS5749_I2S_TIMEOUT_MS CONFIG_TAS5749_I2S_TIMEOUT
#else
#define TAS5749_I2S_TIMEOUT_MS 100
#endif

/* Default configuration values */
#ifndef CONFIG_TAS5749_BUFFER_SIZE
#define CONFIG_TAS5749_BUFFER_SIZE       4096
#endif

#ifndef CONFIG_TAS5749_NUM_BUFFERS
#define CONFIG_TAS5749_NUM_BUFFERS       4
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/
static TAS5749_REG_DATA_TYPE g_tas5749_i2c_mode = TAS5749_REG_D_1BYTE;

static const struct audio_ops_s g_audioops = {
	.getcaps = tas5749_getcaps,             /* getcaps        */
	.configure = tas5749_configure,         /* configure      */
	.shutdown = tas5749_shutdown,           /* shutdown       */
	.start = tas5749_start,                 /* start          */
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	.stop = tas5749_stop,                   /* stop           */
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
	.pause = tas5749_pause,                 /* pause          */
	.resume = tas5749_resume,               /* resume         */
#endif
	.allocbuffer = NULL,                    /* allocbuffer    */
	.freebuffer = NULL,                     /* freebuffer     */
	.enqueuebuffer = tas5749_enqueuebuffer, /* enqueue_buffer */
	.cancelbuffer = tas5749_cancelbuffer,   /* cancel_buffer  */
	.ioctl = tas5749_ioctl,                 /* ioctl          */
	.read = NULL,                           /* read           */
	.read = NULL,                           /* write          */
	.reserve = tas5749_reserve,             /* reserve        */
	.release = tas5749_release,             /* release        */
};

struct sample_rate_entry_s {
	uint16_t samprate;
	t_codec_init_script_entry *script;
	int size;
};

/****************************************************************************
 * Name: delay
 *
 * Description
 *    Delay in ms.
 ****************************************************************************/
static void delay(unsigned int mS)
{
	usleep(mS * 1000);
}

/****************************************************************************
 * Name: tas5749_readreg
 *
 * Description
 *    Read the specified 8-bit register from the TAS5749 device.
 *
 ****************************************************************************/
uint8_t tas5749_readreg_1byte(FAR struct tas5749_dev_s *priv, uint8_t regaddr)
{
	uint8_t reg[4];
	FAR struct i2c_dev_s *dev = priv->i2c;
	FAR struct i2c_config_s *tas5749_i2c_config = &(priv->lower->i2c_config);
	uint8_t reg_w[4];
	reg_w[0] = regaddr;
	if (i2c_writeread(dev, tas5749_i2c_config, reg_w, 1, reg, 1) < 0) {
		auddbg("Error, cannot read reg %x\n", regaddr);
		return FAIL_8;
	}
	return reg[0];
}

/****************************************************************************
 * Name: tas5749_readreg
 *
 * Description
 *    Read the specified 32-bit register from the TAS5749 device. (big endian)
 *
 ****************************************************************************/
uint32_t tas5749_readreg_4byte(FAR struct tas5749_dev_s *priv, uint8_t regaddr)
{
	int32_t ret;
	uint8_t reg[4];
	uint32_t regval;
	FAR struct i2c_dev_s *dev = priv->i2c;
	FAR struct i2c_config_s *tas5749_i2c_config = &(priv->lower->i2c_config);

	reg[0] = regaddr;

	ret = i2c_write(dev, tas5749_i2c_config, reg, 1);
	if (ret < 0) {
		auddbg("Error, cannot read reg %x\n", regaddr);
		return FAIL_32;
	}
	ret = i2c_read(dev, tas5749_i2c_config, reg, 4);
	if (ret < 0) {
		auddbg("Error, cannot read reg %x\n", regaddr);
		return FAIL_32;
	}
	regval = ((reg[0] << 24) | (reg[1] << 16) | (reg[2] << 8) | (reg[3] << 24));
	return regval;
}


/************************************************************************************
 * Name: tas5749_writereg_1byte
 *
 * Description:
 *   Write the specified 8-bit register to the TAS5749 device.
 *
 ************************************************************************************/
static int tas5749_writereg_1byte(FAR struct tas5749_dev_s *priv, uint8_t regaddr, uint8_t regval)
{
	int ret;
	uint8_t reg[2];
	FAR struct i2c_dev_s *dev = priv->i2c;
	FAR struct i2c_config_s *tas5749_i2c_config = &(priv->lower->i2c_config);

	if (regaddr == TAS5749_I2C_MODE) {
		if (regval == 0x00) {
			g_tas5749_i2c_mode = TAS5749_REG_D_1BYTE;
		} else if (regval == 0x01) {
			g_tas5749_i2c_mode = TAS5749_REG_D_4BYTE;
		}
	} else if ((regaddr == TAS5749_SOFT_RESET) && (regval == 0x01)) {
		g_tas5749_i2c_mode = TAS5749_REG_D_1BYTE;
	}
	
	reg[0] = regaddr;
	reg[1] = regval;

	ret = i2c_write(dev, tas5749_i2c_config, (uint8_t *)reg, 2);
	if (ret < 0) {
		auddbg("Error, cannot write reg %x\n", regaddr);
	}
	return ret;
}

/************************************************************************************
 * Name: tas5749_writereg_4byte
 *
 * Description:
 *   Write the specified 32-bit register to the TAS5749 device.
 *
 ************************************************************************************/
static int tas5749_writereg_4byte(FAR struct tas5749_dev_s *priv, uint8_t regaddr, uint8_t regval[4])
{
	int ret;
	uint8_t reg[5];
	FAR struct i2c_dev_s *dev = priv->i2c;
	FAR struct i2c_config_s *tas5749_i2c_config = &(priv->lower->i2c_config);

	reg[0] = regaddr;
	reg[1] = regval[0];
	reg[2] = regval[1];
	reg[3] = regval[2];
	reg[4] = regval[3];

	ret = i2c_write(dev, tas5749_i2c_config, (uint8_t *)reg, 5);
	if (ret < 0) {
		auddbg("Error, cannot write reg %x\n", regaddr);
	}
	return ret;
}

/************************************************************************************
 * Name: tas5749_exec_i2c_script
 *
 * Description:
 *   Executes given script through i2c to configuure TAS5749 device.
 *
 ************************************************************************************/
static int tas5749_exec_i2c_script(FAR struct tas5749_dev_s *priv, t_codec_init_script_entry *script, uint32_t size)
{
	uint32_t i;
	uint16_t ret = 0;
	TAS5749_REG_DATA_TYPE reg_acc;

	for (i = 0; i < size; i++) {
		reg_acc = (TAS5749_REG_DATA_TYPE)script[i].type;
		if (reg_acc == TAS5749_REG_D_1BYTE) {
			ret = tas5749_writereg_1byte(priv, (uint8_t)script[i].addr, script[i].val[0]);
		} else if (reg_acc == TAS5749_REG_D_4BYTE) {
			ret = tas5749_writereg_4byte(priv, (uint8_t)script[i].addr, script[i].val);
		} if (script[i].delay > 0) {
			delay(script[i].delay);
		}
	}
	return ret;
}

/************************************************************************************
 * Name: tas5749_takesem
 *
 * Description:
 *  Take a semaphore count, handling the nasty EINTR return if we are interrupted
 *  by a signal.
 *
 ************************************************************************************/
static void tas5749_takesem(sem_t *sem)
{
	int ret;

	do {
		ret = sem_wait(sem);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

/************************************************************************************
 * Name: tas5749_setvolume
 *
 * Description:
 *   Set the right and left volume values in the TAS5749 device based on the current
 *   volume and balance settings.
 *
 ************************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
static void tas5749_setvolume(FAR struct tas5749_dev_s *priv)
{
	/* if no audio device object return */
	if (!priv) {
		auddbg("Error, Device's private data Not available\n");
		return;
	}

	if (priv->running) {
		audvdbg("volume=%u mute=%u\n", priv->volume, priv->mute);
		if (priv->mute) {
			tas5749_exec_i2c_script(priv, codec_init_mute_on_script, sizeof(codec_init_mute_on_script) / sizeof(t_codec_init_script_entry));
		} else {
			codec_set_master_volume_script[3].val[0] = (uint8_t)priv->volume;
			tas5749_exec_i2c_script(priv, codec_set_master_volume_script, sizeof(codec_set_master_volume_script) / sizeof(t_codec_init_script_entry));
		}
	} else {
		audvdbg("not running[volume=%u mute=%u]\n", priv->volume, priv->mute);
	}
}
#endif							/* CONFIG_AUDIO_EXCLUDE_VOLUME */

/************************************************************************************
 * Name: tas5749_setbass
 *
 * Description:
 *   Set the bass level.
 *
 *   The level and range are in whole percentage levels (0-100).
 *
 ************************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_TONE
static void tas5749_setbass(FAR struct tas5749_dev_s *priv, uint8_t bass)
{
	audvdbg("bass=%u\n", bass);
}
#endif							/* CONFIG_AUDIO_EXCLUDE_TONE */

/************************************************************************************
 * Name: tas5749_settreble
 *
 * Description:
 *   Set the treble level .
 *
 *   The level and range are in whole percentage levels (0-100).
 *
 ************************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_TONE
static void tas5749_settreble(FAR struct tas5749_dev_s *priv, uint8_t treble)
{
	audvdbg("treble=%u\n", treble);
}
#endif							/* CONFIG_AUDIO_EXCLUDE_TONE */

/****************************************************************************
 * Name: tas5749_set_i2s_datawidth
 *
 * Description:
 *   Set the 8- 16- 24- bit data modes
 *
 ****************************************************************************/
static void tas5749_set_i2s_datawidth(FAR struct tas5749_dev_s *priv)
{
	/* if no audio device object return */
	if (!priv) {
		auddbg("Error, Device's private data Not available\n");
		return;
	}

	if (priv->inout) {
		I2S_RXDATAWIDTH(priv->i2s, priv->bpsamp);
	} else {
		I2S_TXDATAWIDTH(priv->i2s, priv->bpsamp);
	}
}

/****************************************************************************
 * Name: tas5749_set_i2s_samplerate
 *
 * Description:
 *
 ****************************************************************************/
static void tas5749_set_i2s_samplerate(FAR struct tas5749_dev_s *priv)
{
	/* if no audio device object return */
	if (!priv) {
		auddbg("Error, Device's private data Not available\n");
		return;
	}

	if (priv->inout) {
		I2S_RXSAMPLERATE(priv->i2s, priv->samprate);
	} else {
		I2S_TXSAMPLERATE(priv->i2s, priv->samprate);
	}
}

/****************************************************************************
 * Name: tas5749_getcaps
 *
 * Description:
 *   Get the audio device capabilities
 *
 ****************************************************************************/
static int tas5749_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps)
{
#if !defined(CONFIG_AUDIO_EXCLUDE_VOLUME) || !defined(CONFIG_AUDIO_EXCLUDE_TONE)
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)dev;
#endif
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

		caps->ac_channels = TAS5749_CHANNELS;	/* Stereo output */

		switch (caps->ac_subtype) {
		case AUDIO_TYPE_QUERY:
			/* We don't decode any formats!  Only something above us in
			 * the audio stream can perform decoding on our behalf.
			 */

			/* The types of audio units we implement */
			caps->ac_controls.b[0] = AUDIO_TYPE_OUTPUT | AUDIO_TYPE_FEATURE;

			break;

		default:
			caps->ac_controls.b[0] = AUDIO_SUBFMT_END;
			break;
		}

		break;

	/* Provide capabilities of our INPUT & OUTPUT unit */
	case AUDIO_TYPE_OUTPUT:
		caps->ac_channels = TAS5749_CHANNELS;
		switch (caps->ac_subtype) {
		case AUDIO_TYPE_QUERY:
			/* Report the Sample rates we support */
			caps->ac_controls.b[0] = AUDIO_SAMP_RATE_TYPE_48K;
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

		switch (caps->ac_format.hw) {
		case AUDIO_FU_VOLUME:
			caps->ac_controls.hw[0] = TAS5749_SPK_VOL_MAX;
			caps->ac_controls.hw[1] = priv->volume;
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
 * Name: tas5749_configure
 *
 * Description:
 *   Configure the audio device for the specified  mode of operation.
 *
 ****************************************************************************/
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int tas5749_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps)
#else
static int tas5749_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps)
#endif
{
#if !defined(CONFIG_AUDIO_EXCLUDE_VOLUME) || !defined(CONFIG_AUDIO_EXCLUDE_TONE)
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)dev;
#endif
	int ret = OK;

	DEBUGASSERT(priv && caps);
	audvdbg("ac_type: %d\n", caps->ac_type);


	/* TAS5749 supports on the fly changes for almost all changes
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
			if (volume < TAS5749_SPK_VOL_MIN) {
				priv->volume = TAS5749_SPK_VOL_MIN;
			} else if (volume >= TAS5749_SPK_VOL_MAX) {
				priv->volume = TAS5749_SPK_VOL_MAX;
			} else {
				priv->volume = volume;
			}

			tas5749_setvolume(priv);
		}
		break;
		case AUDIO_FU_MUTE: {
			/* Mute or unmute:	true(1) or false(0) */
			bool mute = caps->ac_controls.b[0];
			audvdbg("mute: 0x%x\n", mute);
			priv->mute = mute;
			tas5749_setvolume(priv);
		}
		break;
#endif							/* CONFIG_AUDIO_EXCLUDE_VOLUME */

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
		case AUDIO_FU_BASS: {
			/* Set the bass.  The percentage level (0-100) is in the
			 * ac_controls.b[0] parameter. */

			uint8_t bass = caps->ac_controls.b[0];
			audvdbg("    Bass: %d\n", bass);
			if (bass <= TAS5749_BASS_MAX) {
				tas5749_setbass(priv, bass);
			}
		}
		break;
		case AUDIO_FU_TREBLE: {
			/* Set the treble.  The percentage level (0-100) is in the
			 * ac_controls.b[0] parameter. */

			uint8_t treble = caps->ac_controls.b[0];
			audvdbg("    Treble: %d\n", treble);
			if (treble <= TAS5749_TREBLE_MAX) {
				tas5749_settreble(priv, treble);
			}
		}
		break;
#endif							/* CONFIG_AUDIO_EXCLUDE_TONE */
		default:
			auddbg("    ERROR: Unrecognized feature unit\n");
			break;
		}
		break;					/* Break for inner switch case */
	case AUDIO_TYPE_OUTPUT:
		audvdbg("  AUDIO_TYPE :%s\n", caps->ac_type == AUDIO_TYPE_INPUT ? "INPUT" : "OUTPUT");
		/* Verify that all of the requested values are supported */

		ret = -EDOM;
		if (caps->ac_channels != TAS5749_CHANNELS) {
			auddbg("ERROR: Unsupported number of channels: %d\n", caps->ac_channels);
			break;
		}

		if (caps->ac_controls.b[2] != TAS5749_BPSAMP) {
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
		ret = OK;
		priv->inout = false;
		break;

	case AUDIO_TYPE_PROCESSING:
		break;

	default:
		break;
	}
	return ret;
}

/****************************************************************************
 * Name: tas5749_shutdown
 *
 * Description:
 *   Shutdown the TAS5749 chip and put it in the lowest power state possible.
 *
 ****************************************************************************/
static int tas5749_shutdown(FAR struct audio_lowerhalf_s *dev)
{
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)dev;
	struct tas5749_lower_s *lower = priv->lower;

	DEBUGASSERT(priv);

	if (!priv) {
		return -EINVAL;
	}

	/* First disable interrupts */
	tas5749_takesem(&priv->devsem);
	sq_entry_t *tmp = NULL;
	for (tmp = (sq_entry_t *)sq_peek(&priv->pendq); tmp; tmp = sq_next(tmp)) {
		sq_rem(tmp, &priv->pendq);
		audvdbg("(tasshutdown)removing tmp with addr 0x%x\n", tmp);
	}
	sq_init(&priv->pendq);

	if (priv->running) {
		I2S_STOP(priv->i2s, I2S_TX);
		tas5749_exec_i2c_script(priv, codec_stop_script, sizeof(codec_stop_script) / sizeof(t_codec_init_script_entry));
		priv->running = false;
	}
	priv->paused = false;

	tas5749_givesem(&priv->devsem);

	/* Now issue a software reset.  This puts all TAS5749 registers back in
	 * their default state.
	 */

	if (lower->control_powerdown) {
		lower->control_powerdown(1);
	}
	return OK;
}

/****************************************************************************
 * Name: tas5749_io_err_cb
 *
 * Description:
 *   Callback function for io error
 *
 ****************************************************************************/
static void tas5749_io_err_cb(FAR struct i2s_dev_s *dev, FAR void *arg, int flags)
{
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)arg;

	/* Call upper callback, let it post msg to user q
	 * apb is set NULL, okay? Rethink
	 */
	if (priv && priv->dev.upper) {
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_IOERR, NULL, flags);
	}

}

/****************************************************************************
 * Name: tas5749_start
 *
 * Description:
 *   Start the configured operation (audio streaming, volume enabled, etc.).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int tas5749_start(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int tas5749_start(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	audvdbg(" tas5749_start Entry\n");
	tas5749_takesem(&priv->devsem);
	if (priv->running) {
		goto tasstart_withsem;
	}

	/* Register cb for io error */
	I2S_ERR_CB_REG(priv->i2s, tas5749_io_err_cb, priv);

	/* Finally set tas5749 to be running */
	priv->running = true;
	priv->mute = false;
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	tas5749_setvolume(priv);
#endif

	sq_entry_t *tmp = NULL;
	sq_queue_t *q = &priv->pendq;
	for (tmp = sq_peek(q); tmp; tmp = sq_next(tmp)) {
		tas5749_enqueuebuffer(dev, (struct ap_buffer_s *)tmp);
	}

	/* Exit reduced power modes of operation */
	/* REVISIT */

tasstart_withsem:

	tas5749_givesem(&priv->devsem);
	return OK;
}

/****************************************************************************
 * Name: tas5749_stop
 *
 * Description: Stop the configured operation (audio streaming, volume
 *              disabled, etc.).
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int tas5749_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int tas5749_stop(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}
	audvdbg(" tas5749_stop Entry\n");
	tas5749_takesem(&priv->devsem);
	I2S_STOP(priv->i2s, I2S_TX);
	/* Need to run the stop script here */
	tas5749_exec_i2c_script(priv, codec_stop_script, sizeof(codec_stop_script) / sizeof(t_codec_init_script_entry));

	priv->running = false;
	priv->mute = true;
	tas5749_givesem(&priv->devsem);

	/* Enter into a reduced power usage mode */
	/* REVISIT: */

	return OK;
}
#endif

/****************************************************************************
 * Name: tas5749_pause
 *
 * Description: Pauses the playback.
 *
 ****************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int tas5749_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int tas5749_pause(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}
	audvdbg(" tas5749_pause Entry\n");
	tas5749_takesem(&priv->devsem);

	if (priv->running && !priv->paused) {
		/* Disable interrupts to prevent us from suppling any more data */

		priv->paused = true;
		priv->mute = true;
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
		tas5749_setvolume(priv);
#endif
		I2S_PAUSE(priv->i2s, I2S_TX);
	}

	tas5749_givesem(&priv->devsem);

	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: tas5749_resume
 *
 * Description: Resumes the playback.
 *
 ****************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int tas5749_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int tas5749_resume(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}
	
	audvdbg(" tas5749_resume Entry\n");
	tas5749_takesem(&priv->devsem);

	if (priv->running && priv->paused) {
		priv->paused = false;
		priv->mute = false;
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
		tas5749_setvolume(priv);
#endif

		I2S_RESUME(priv->i2s, I2S_TX);

	}

	tas5749_givesem(&priv->devsem);
	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: tas5749_rxcallback
 *
 * Description: Called when I2S filled a buffer. No recycling mechanism now.
 *
 ****************************************************************************/

static void tas5749_txcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)arg;

	DEBUGASSERT(priv && apb);

	tas5749_takesem(&priv->devsem);
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

	tas5749_givesem(&priv->devsem);
}

/****************************************************************************
 * Name: tas5749_enqueuebuffer
 *
 * Description: Enqueue an Audio Pipeline Buffer for playback/ processing.
 *
 ****************************************************************************/
static int tas5749_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)dev;
	int ret;

	if (!priv || !apb) {
		return -EINVAL;
	}

	if (!priv->running) {
		/* Add the new buffer to the tail of pending audio buffers */
		tas5749_takesem(&priv->devsem);
		sq_addlast((sq_entry_t *)&apb->dq_entry, &priv->pendq);
		audvdbg("enqueue added buf 0x%x\n", apb);
		tas5749_givesem(&priv->devsem);
		return OK;
	}
	ret = I2S_SEND(priv->i2s, apb, tas5749_txcallback, priv, TAS5749_I2S_TIMEOUT_MS);

	return ret;
}

/****************************************************************************
 * Name: tas5749_cancelbuffer
 *
 * Description: Called when an enqueued buffer is being cancelled.
 *
 ****************************************************************************/
static int tas5749_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	audvdbg("apb=%p\n", apb);
	/* Need to add logic here */
	return OK;
}

/****************************************************************************
 * Name: tas5749_ioctl
 *
 * Description: Perform a device ioctl
 *
 ****************************************************************************/
static int tas5749_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg)
{
	FAR struct ap_buffer_info_s *bufinfo;
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	/* Deal with ioctls passed from the upper-half driver */

	switch (cmd) {

	case AUDIOIOC_PREPARE: {
		audvdbg("AUDIOIOC_PREPARE: tas5749 prepare\n");
		uint8_t regval;
		struct tas5749_lower_s *lower = priv->lower;

		/* Take semaphore */
		tas5749_takesem(&priv->devsem);

		/* Pause i2s channel */
		I2S_PAUSE(priv->i2s, I2S_TX);

		if (lower->control_powerdown) {
			lower->control_powerdown(0);
			delay(10);  //spec is 100us
		}
		tas5749_exec_i2c_script(priv, codec_initial_script, sizeof(codec_initial_script) / sizeof(t_codec_init_script_entry));

		/* Amp Error Check */
		tas5749_writereg_1byte(priv, (uint8_t)0x01, 0x00);
		regval = tas5749_readreg_1byte(priv, 0x01);
		if (regval != 0x00) {
			auddbg("ERROR: Amp has some error! 0x%02x \n", regval);
		}

		/* Resume I2S */
		I2S_RESUME(priv->i2s, I2S_TX);

		/* Give semaphore */
		tas5749_givesem(&priv->devsem);
	}
	break;
	case AUDIOIOC_HWRESET: {
		/* REVISIT:  Should we completely re-initialize the chip?   We
		 * can't just issue a software reset; that would puts all TAS5749
		 * registers back in their default state.
		 */

		audvdbg("AUDIOIOC_HWRESET:\n");
		tas5749_hw_reset(priv);
	}
	break;

		/* Report our preferred buffer size and quantity */

	case AUDIOIOC_GETBUFFERINFO: {
		/* Report our preferred buffer size and quantity */
		audvdbg("AUDIOIOC_GETBUFFERINFO:\n");
		/* Take semaphore */
		tas5749_takesem(&priv->devsem);
		
		bufinfo = (FAR struct ap_buffer_info_s *)arg;
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
		bufinfo->buffer_size = CONFIG_TAS5749_BUFFER_SIZE;
		bufinfo->nbuffers = CONFIG_TAS5749_NUM_BUFFERS;
#else
		bufinfo->buffer_size = CONFIG_TAS5749_BUFFER_SIZE;
		bufinfo->nbuffers = CONFIG_TAS5749_NUM_BUFFERS;
#endif
		audvdbg("buffer_size : %d nbuffers : %d\n", bufinfo->buffer_size, bufinfo->nbuffers);

		/* Give semaphore */
		tas5749_givesem(&priv->devsem);
	}
	break;


	default:
		audvdbg("tas5749_ioctl received unkown cmd 0x%x\n", cmd);
		break;
	}

	return OK;
}

/****************************************************************************
 * Name: tas5749_reserve
 *
 * Description: Reserves a session (the only one we have).
 *
 ****************************************************************************/
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int tas5749_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session)
#else
static int tas5749_reserve(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)dev;
	int ret = OK;

	if (!priv) {
		return -EINVAL;
	}

	/* Borrow the APBQ semaphore for thread sync */

	tas5749_takesem(&priv->devsem);
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

	tas5749_givesem(&priv->devsem);

	return ret;
}

/****************************************************************************
 * Name: tas5749_release
 *
 * Description: Releases the session (the only one we have).
 *
 ****************************************************************************/
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int tas5749_release(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int tas5749_release(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct tas5749_dev_s *priv = (FAR struct tas5749_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}
	tas5749_takesem(&priv->devsem);
	if (priv->running) {
		I2S_STOP(priv->i2s, I2S_TX);
		tas5749_exec_i2c_script(priv, codec_stop_script, sizeof(codec_stop_script) / sizeof(t_codec_init_script_entry));
		priv->running = false;
	}
	priv->reserved = false;
	tas5749_givesem(&priv->devsem);

	return OK;
}

/* Reset and reconfigure the TAS5749 hardwaqre */
/****************************************************************************
 * Name: tas5749_reconfigure
 *
 * Description:
 *   re-initialize the TAS5749
 *
 * Input Parameters:
 *   priv - A reference to the driver state structure
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void tas5749_reconfigure(FAR struct tas5749_dev_s *priv)
{
	priv->inout = false;
	/* Put audio output back to its initial configuration */
	priv->samprate = TAS5749_DEFAULT_SAMPRATE;
	priv->nchannels = TAS5749_DEFAULT_NCHANNELS;
	priv->bpsamp = TAS5749_DEFAULT_BPSAMP;
#if !defined(CONFIG_AUDIO_EXCLUDE_VOLUME) && !defined(CONFIG_AUDIO_EXCLUDE_BALANCE)
	priv->balance = b16HALF;	/* Center balance */
#endif

	/* Software reset.	This puts all TAS5749 registers back in their
	 * default state.
	 */
	tas5749_exec_i2c_script(priv, codec_initial_script, sizeof(codec_initial_script) / sizeof(t_codec_init_script_entry));
	tas5749_set_i2s_samplerate(priv);
	tas5749_set_i2s_datawidth(priv);
}

/****************************************************************************
 * Name: tas5749_hw_reset
 *
 * Description:
 *   Reset and re-initialize the TAS5749
 *
 * Input Parameters:
 *   priv - A reference to the driver state structure
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void tas5749_hw_reset(FAR struct tas5749_dev_s *priv)
{
	if (!priv) {
		return;
	}

	struct tas5749_lower_s *lower = priv->lower;

	if (lower->control_hw_reset) {
		lower->control_hw_reset(true);
		delay(100);
		lower->control_hw_reset(false);
		delay(100);  //spec is 63.5ms
	}
	tas5749_reconfigure(priv);
	tas5749_exec_i2c_script(priv, codec_init_mute_on_script, sizeof(codec_init_mute_on_script) / sizeof(t_codec_init_script_entry));
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tas5749_initialize
 *
 * Description:
 *   Initialize the TAS5749 device.
 *
 * Input Parameters:
 *   i2c     - An I2C driver instance
 *   i2s     - An I2S driver instance
 *   lower   - Persistent board configuration data
 *
 * Returned Value:
 *   A new lower half audio interface for the TAS5749 device is returned on
 *   success; NULL is returned on failure.
 *
 ****************************************************************************/
FAR struct audio_lowerhalf_s *tas5749_initialize(FAR struct i2c_dev_s *i2c, FAR struct i2s_dev_s *i2s, FAR struct tas5749_lower_s *lower)
{

	FAR struct tas5749_dev_s *priv;
	uint8_t regval;
	/* Sanity check */
	DEBUGASSERT(i2c && i2s && lower);

	auddbg("I2s dev addr is 0x%x\n", i2s);

	/* Allocate a TAS5749 device structure */
	priv = (FAR struct tas5749_dev_s *)kmm_zalloc(sizeof(struct tas5749_dev_s));

	if (!priv) {
		return NULL;
	}

	/* Initialize the TAS5749 device structure.  Since we used kmm_zalloc,
	 * only the non-zero elements of the structure need to be initialized.
	 */

	priv->dev.ops = &g_audioops;
	priv->lower = lower;
	priv->i2c = i2c;
	priv->i2s = i2s;

	sem_init(&priv->devsem, 0, 1);
	sq_init(&priv->pendq);

	/* Software reset.  This puts all TAS5749 registers back in their
	 * default state.
	 */

	/* Verify that TAS5749 is present and available on this I2C */
	regval = tas5749_readreg_1byte(priv, TAS5749_TI_DEVICE_ID);
	if (regval != TAS5749_TI_DEVICE_ID_VAL) {
		auddbg("ERROR: TAS5749 not found: ID=%04x\n", regval);
		goto errout_with_dev;
	}

	/*reconfigure the TAS5749 hardwaqre */
	tas5749_reconfigure(priv);
	tas5749_exec_i2c_script(priv, codec_init_mute_on_script, sizeof(codec_init_mute_on_script) / sizeof(t_codec_init_script_entry));

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	priv->volume = TAS5749_SPK_VOL_DEF;
	tas5749_setvolume(priv);
#endif
	return &priv->dev;

errout_with_dev:
	sem_destroy(&priv->devsem);
	kmm_free(priv);
	return NULL;
}

