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
#include <tinyara/audio/alc1019.h>
#include <tinyara/math.h>
#include <math.h>
#include <tinyara/i2c.h>

#include "alc1019.h"
#include "alc1019scripts.h"

#define ALC1019_I2S_TIMEOUT_MS 200

/* Default configuration values */
#ifndef CONFIG_ALC1019_BUFFER_SIZE
#define CONFIG_ALC1019_BUFFER_SIZE       4096
#endif

#ifndef CONFIG_ALC1019_NUM_BUFFERS
#define CONFIG_ALC1019_NUM_BUFFERS       4
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct audio_ops_s g_audioops = {
        .getcaps = alc1019_getcaps,             /* getcaps        */
        .configure = alc1019_configure,         /* configure      */
        .shutdown = alc1019_shutdown,           /* shutdown       */
        .start = alc1019_start,                 /* start          */
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
        .stop = alc1019_stop,                   /* stop           */
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
        .pause = alc1019_pause,                 /* pause          */
        .resume = alc1019_resume,               /* resume         */
#endif
        .allocbuffer = NULL,                    /* allocbuffer    */
        .freebuffer = NULL,                     /* freebuffer     */
        .enqueuebuffer = alc1019_enqueuebuffer, /* enqueue_buffer */
        .cancelbuffer = alc1019_cancelbuffer,   /* cancel_buffer  */
        .ioctl = alc1019_ioctl,                 /* ioctl          */
        .read = NULL,                           /* read           */
        .read = NULL,                           /* write          */
        .reserve = alc1019_reserve,             /* reserve        */
        .release = alc1019_release,             /* release        */
};

static int alc1019_writereg_3byte(FAR struct alc1019_dev_s *priv, uint8_t regaddr_h, uint8_t regaddr_l , uint8_t regval)
{
        int ret;
        uint8_t reg[3];
        FAR struct i2c_dev_s *dev = priv->i2c;
        FAR struct i2c_config_s *alc1019_i2c_config = &(priv->lower->i2c_config);

        reg[0] = regaddr_h;
        reg[1] = regaddr_l;
        reg[2] = regval;

        ret = i2c_write(dev, alc1019_i2c_config, (uint8_t *)reg, 3);
        if (ret < 0) {
                auddbg("Error, cannot write reg %x\n", regaddr);
        }
        return ret;
}

/************************************************************************************
 * Name: alc1019_exec_i2c_script
 *
 * Description:
 *   Executes given script through i2c to configuure ALC1019 device.
 *
 ************************************************************************************/

static int alc1019_exec_i2c_script(FAR struct alc1019_dev_s *priv, t_codec_init_script_entry *script, uint32_t size)
{
	uint32_t i;
        uint16_t ret = 0;

        for (i = 0; i < size; i++) {
		ret = alc1019_writereg_3byte(priv, script[i].addr_h, script[i].addr_l, script[i].val);
                if (script[i].delay > 0) {
                	up_mdelay(script[i].delay);
                }
        }
        return ret;
}

/************************************************************************************
 * Name: alc1019_takesem
 *
 * Description:
 *  Take a semaphore count, handling the nasty EINTR return if we are interrupted
 *  by a signal.
 *
 ************************************************************************************/
static void alc1019_takesem(sem_t *sem)
{
        int ret;

        do {
                ret = sem_wait(sem);
                DEBUGASSERT(ret == 0 || errno == EINTR);
        } while (ret < 0);
}

/************************************************************************************
 * Name: alc1019_setvolume
 *
 * Description:
 *   Set the right and left volume values in the ALC1019 device based on the current
 *   volume and balance settings.
 *
 ************************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
static void alc1019_setvolume(FAR struct alc1019_dev_s *priv)
{
        /* if no audio device object return */
        if (!priv) {
                auddbg("Error, Device's private data Not available\n");
                return;
        }

        if (priv->running) {
                audvdbg("volume=%u mute=%u\n", priv->volume, priv->mute);
                if (priv->mute) {
			alc1019_exec_i2c_script(priv, codec_init_mute_on_script, sizeof(codec_init_mute_on_script) / sizeof(t_codec_init_script_entry));
                } else {
			if (priv->volume == 0) {
				codec_set_master_volume_script[0].val = 0;
			} else {
				codec_set_master_volume_script[0].val = ALC1019_HW_VOL_MAX - (ALC1019_SPK_VOL_MAX - priv->volume) * ALC1019_HW_VOL_STEP;
			}
			alc1019_exec_i2c_script(priv, codec_set_master_volume_script, sizeof(codec_set_master_volume_script) / sizeof(t_codec_init_script_entry));
		}
        } else {
                audvdbg("not running[volume=%u mute=%u]\n", priv->volume, priv->mute);
        }
}
#endif                                                  /* CONFIG_AUDIO_EXCLUDE_VOLUME */

/************************************************************************************
 * Name: alc1019_setbass
 *
 * Description:
 *   Set the bass level.
 *
 *   The level and range are in whole percentage levels (0-100).
 *
 ************************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_TONE
static void alc1019_setbass(FAR struct alc1019_dev_s *priv, uint8_t bass)
{
        audvdbg("bass=%u\n", bass);
}
#endif                                                  /* CONFIG_AUDIO_EXCLUDE_TONE */

/************************************************************************************
 * Name: alc1019_settreble
 *
 * Description:
 *   Set the treble level .
 *
 *   The level and range are in whole percentage levels (0-100).
 *
 ************************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_TONE
static void alc1019_settreble(FAR struct alc1019_dev_s *priv, uint8_t treble)
{
        audvdbg("treble=%u\n", treble);
}
#endif

/****************************************************************************
 * Name: alc1019_set_i2s_datawidth
 *
 * Description:
 *   Set the 8- 16- 24- bit data modes
 *
 ****************************************************************************/
static void alc1019_set_i2s_datawidth(FAR struct alc1019_dev_s *priv)
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
 * Name: alc1019_set_i2s_samplerate
 *
 * Description:
 *
 ****************************************************************************/
static void alc1019_set_i2s_samplerate(FAR struct alc1019_dev_s *priv)
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
 * Name: alc1019_getcaps
 *
 * Description:
 *   Get the audio device capabilities
 *
 ****************************************************************************/
static int alc1019_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps)
{
#if !defined(CONFIG_AUDIO_EXCLUDE_VOLUME) || !defined(CONFIG_AUDIO_EXCLUDE_TONE)
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)dev;
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

		caps->ac_channels = ALC1019_CHANNELS;	/* Stereo output */

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
		caps->ac_channels = ALC1019_CHANNELS;
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

			caps->ac_controls.b[0] = AUDIO_FU_VOLUME;
			caps->ac_controls.b[1] = AUDIO_FU_BALANCE >> 8;
		} else {
			/* TODO:  Do we need to provide specific info for the Feature Units,
			 * such as volume setting ranges, etc.?
			 */
		}

		caps->ac_controls.hw[0] = ALC1019_SPK_VOL_MAX;
		caps->ac_controls.hw[1] = priv->volume;

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
 * Name: alc1019_configure
 *
 * Description:
 *   Configure the audio device for the specified  mode of operation.
 *
 ****************************************************************************/
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps)
#else
static int alc1019_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps)
#endif
{
#if !defined(CONFIG_AUDIO_EXCLUDE_VOLUME) || !defined(CONFIG_AUDIO_EXCLUDE_TONE)
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)dev;
#endif
	int ret = OK;

	DEBUGASSERT(priv && caps);
	audvdbg("ac_type: %d\n", caps->ac_type);


	/* ALC1019 supports on the fly changes for almost all changes
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
			if (volume < ALC1019_SPK_VOL_MIN) {
				priv->volume = ALC1019_SPK_VOL_MIN;
			} else if (volume >= ALC1019_SPK_VOL_MAX) {
				priv->volume = ALC1019_SPK_VOL_MAX;
			} else {
				priv->volume = volume;
			}
			printf("set volume is called\n");
			alc1019_setvolume(priv);
		}
		break;
		case AUDIO_FU_MUTE: {
			/* Mute or unmute:	true(1) or false(0) */
			bool mute = caps->ac_controls.b[0];
			audvdbg("mute: 0x%x\n", mute);
			priv->mute = mute;
			alc1019_setvolume(priv);
		}
		break;
#endif							/* CONFIG_AUDIO_EXCLUDE_VOLUME */
		default:
			auddbg("    ERROR: Unrecognized feature unit\n");
			ret = -ENOSYS;
			break;
		}
		break;					/* Break for inner switch case */
	case AUDIO_TYPE_OUTPUT:
		audvdbg("  AUDIO_TYPE :%s\n", caps->ac_type == AUDIO_TYPE_INPUT ? "INPUT" : "OUTPUT");
		/* Verify that all of the requested values are supported */

		ret = -EDOM;
		if (caps->ac_channels != ALC1019_CHANNELS) {
			auddbg("ERROR: Unsupported number of channels: %d\n", caps->ac_channels);
			break;
		}

		if (caps->ac_controls.b[2] != ALC1019_BPSAMP) {
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
 * Name: alc1019_shutdown
 *
 * Description:
 *   Shutdown the ALC1019 chip and put it in the lowest power state possible.
 *
 ****************************************************************************/
static int alc1019_shutdown(FAR struct audio_lowerhalf_s *dev)
{
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)dev;
	struct alc1019_lower_s *lower = priv->lower;

	DEBUGASSERT(priv);

	if (!priv) {
		return -EINVAL;
	}

	/* First disable interrupts */
	alc1019_takesem(&priv->devsem);
	sq_entry_t *tmp = NULL;
	for (tmp = (sq_entry_t *)sq_peek(&priv->pendq); tmp; tmp = sq_next(tmp)) {
		sq_rem(tmp, &priv->pendq);
		audvdbg("(tasshutdown)removing tmp with addr 0x%x\n", tmp);
	}
	sq_init(&priv->pendq);

	if (priv->running) {
		I2S_STOP(priv->i2s, I2S_TX);
		alc1019_exec_i2c_script(priv, codec_stop_script, sizeof(codec_stop_script) / sizeof(t_codec_init_script_entry));
		priv->running = false;
	}
	priv->paused = false;

	alc1019_givesem(&priv->devsem);

	/* Now issue a software reset.  This puts all ALC1019 registers back in
	 * their default state.
	 */

	return OK;
}

/****************************************************************************
 * Name: alc1019_io_err_cb
 *
 * Description:
 *   Callback function for io error
 *
 ****************************************************************************/
static void alc1019_io_err_cb(FAR struct i2s_dev_s *dev, FAR void *arg, int flags)
{
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)arg;

	/* Call upper callback, let it post msg to user q
	 * apb is set NULL, okay? Rethink
	 */
	if (priv && priv->dev.upper) {
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_IOERR, NULL, flags);
	}

}

/****************************************************************************
 * Name: alc1019_start
 *
 * Description:
 *   Start the configured operation (audio streaming, volume enabled, etc.).
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_start(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int alc1019_start(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	audvdbg(" alc1019_start Entry\n");
	alc1019_takesem(&priv->devsem);
	if (priv->running) {
		goto ub_start_withsem;
	}

	/* Register cb for io error */
	I2S_ERR_CB_REG(priv->i2s, alc1019_io_err_cb, priv);

	/* Finally set alc1019 to be running */
	priv->running = true;
	priv->mute = false;
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	alc1019_setvolume(priv);
#endif

	sq_entry_t *tmp = NULL;
	sq_queue_t *q = &priv->pendq;
	for (tmp = sq_peek(q); tmp; tmp = sq_next(tmp)) {
		alc1019_enqueuebuffer(dev, (struct ap_buffer_s *)tmp);
	}

	/* Exit reduced power modes of operation */
	/* REVISIT */

ub_start_withsem:

	alc1019_givesem(&priv->devsem);
	return OK;
}

/****************************************************************************
 * Name: alc1019_stop
 *
 * Description: Stop the configured operation (audio streaming, volume
 *              disabled, etc.).
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int alc1019_stop(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}
	audvdbg(" alc1019_stop Entry\n");
	alc1019_takesem(&priv->devsem);
	I2S_STOP(priv->i2s, I2S_TX);
	/* Need to run the stop script here */
	alc1019_exec_i2c_script(priv, codec_stop_script, sizeof(codec_stop_script) / sizeof(t_codec_init_script_entry));

	priv->running = false;
	priv->mute = true;
	alc1019_givesem(&priv->devsem);

	/* Enter into a reduced power usage mode */
	/* REVISIT: */

	return OK;
}
#endif

/****************************************************************************
 * Name: alc1019_pause
 *
 * Description: Pauses the playback.
 *
 ****************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int alc1019_pause(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}
	audvdbg(" alc1019_pause Entry\n");
	alc1019_takesem(&priv->devsem);

	if (priv->running && !priv->paused) {
		/* Disable interrupts to prevent us from suppling any more data */

		priv->paused = true;
		priv->mute = true;
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
		alc1019_setvolume(priv);
#endif
		I2S_PAUSE(priv->i2s, I2S_TX);
	}

	alc1019_givesem(&priv->devsem);

	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: alc1019_resume
 *
 * Description: Resumes the playback.
 *
 ****************************************************************************/
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int alc1019_resume(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	audvdbg(" alc1019_resume Entry\n");
	alc1019_takesem(&priv->devsem);

	if (priv->running && priv->paused) {
		priv->paused = false;
		priv->mute = false;
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
		alc1019_setvolume(priv);
#endif

		I2S_RESUME(priv->i2s, I2S_TX);

	}

	alc1019_givesem(&priv->devsem);
	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: alc1019_rxcallback
 *
 * Description: Called when I2S filled a buffer. No recycling mechanism now.
 *
 ****************************************************************************/

static void alc1019_txcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)arg;

	DEBUGASSERT(priv && apb);

	alc1019_takesem(&priv->devsem);
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

	alc1019_givesem(&priv->devsem);
}

/****************************************************************************
 * Name: alc1019_enqueuebuffer
 *
 * Description: Enqueue an Audio Pipeline Buffer for playback/ processing.
 *
 ****************************************************************************/
static int alc1019_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)dev;
	int ret;

	if (!priv || !apb) {
		return -EINVAL;
	}

	if (!priv->running) {
		/* Add the new buffer to the tail of pending audio buffers */
		alc1019_takesem(&priv->devsem);
		sq_addlast((sq_entry_t *)&apb->dq_entry, &priv->pendq);
		alc1019_givesem(&priv->devsem);
		return OK;
	}
	
	ret = I2S_SEND(priv->i2s, apb, alc1019_txcallback, priv, ALC1019_I2S_TIMEOUT_MS);

	return ret;
}

/****************************************************************************
 * Name: alc1019_cancelbuffer
 *
 * Description: Called when an enqueued buffer is being cancelled.
 *
 ****************************************************************************/
static int alc1019_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	audvdbg("apb=%p\n", apb);
	/* Need to add logic here */
	return OK;
}

/****************************************************************************
 * Name: alc1019_ioctl
 *
 * Description: Perform a device ioctl
 *
 ****************************************************************************/
static int alc1019_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg)
{
	FAR struct ap_buffer_info_s *bufinfo;
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	/* Deal with ioctls passed from the upper-half driver */

	switch (cmd) {

	case AUDIOIOC_PREPARE: {
		audvdbg("AUDIOIOC_PREPARE: alc1019 prepare\n");
		uint8_t regval;
		struct alc1019_lower_s *lower = priv->lower;

		/* Take semaphore */
		alc1019_takesem(&priv->devsem);

		/* Pause i2s channel */
		I2S_PAUSE(priv->i2s, I2S_TX);

		alc1019_exec_i2c_script(priv, codec_initial_script, sizeof(codec_initial_script) / sizeof(t_codec_init_script_entry));
		
		/* Resume I2S */
		I2S_RESUME(priv->i2s, I2S_TX);

		/* Give semaphore */
		alc1019_givesem(&priv->devsem);
	}
	break;
	case AUDIOIOC_HWRESET: {
		/* REVISIT:  Should we completely re-initialize the chip?   We
		 * can't just issue a software reset; that would puts all ALC1019
		 * registers back in their default state.
		 */

		audvdbg("AUDIOIOC_HWRESET:\n");
		alc1019_hw_reset(priv);
	}
	break;

		/* Report our preferred buffer size and quantity */

	case AUDIOIOC_GETBUFFERINFO: {
		/* Report our preferred buffer size and quantity */
		audvdbg("AUDIOIOC_GETBUFFERINFO:\n");
		/* Take semaphore */
		alc1019_takesem(&priv->devsem);
		
		bufinfo = (FAR struct ap_buffer_info_s *)arg;
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
		bufinfo->buffer_size = CONFIG_ALC1019_BUFFER_SIZE;
		bufinfo->nbuffers = CONFIG_ALC1019_NUM_BUFFERS;
#else
		bufinfo->buffer_size = CONFIG_ALC1019_BUFFER_SIZE;
		bufinfo->nbuffers = CONFIG_ALC1019_NUM_BUFFERS;
#endif
		audvdbg("buffer_size : %d nbuffers : %d\n", bufinfo->buffer_size, bufinfo->nbuffers);

		/* Give semaphore */
		alc1019_givesem(&priv->devsem);
	}
	break;


	default:
		audvdbg("alc1019_ioctl received unkown cmd 0x%x\n", cmd);
		break;
	}

	return OK;
}

/****************************************************************************
 * Name: alc1019_reserve
 *
 * Description: Reserves a session (the only one we have).
 *
 ****************************************************************************/
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session)
#else
static int alc1019_reserve(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)dev;
	int ret = OK;

	if (!priv) {
		return -EINVAL;
	}

	/* Borrow the APBQ semaphore for thread sync */

	alc1019_takesem(&priv->devsem);
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

	alc1019_givesem(&priv->devsem);

	return ret;
}

/****************************************************************************
 * Name: alc1019_release
 *
 * Description: Releases the session (the only one we have).
 *
 ****************************************************************************/
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int alc1019_release(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int alc1019_release(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct alc1019_dev_s *priv = (FAR struct alc1019_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}
	alc1019_takesem(&priv->devsem);
	if (priv->running) {
		I2S_STOP(priv->i2s, I2S_TX);
		alc1019_exec_i2c_script(priv, codec_stop_script, sizeof(codec_stop_script) / sizeof(t_codec_init_script_entry));
		priv->running = false;
	}
	priv->reserved = false;
	alc1019_givesem(&priv->devsem);

	return OK;
}

/* Reset and reconfigure the ALC1019 hardwaqre */
/****************************************************************************
 * Name: alc1019_reset_config
 *
 * Description:
 *   re-initialize the ALC1019
 *
 * Input Parameters:
 *   priv - A reference to the driver state structure
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void alc1019_reset_config(FAR struct alc1019_dev_s *priv)
{
	priv->inout = false;
	/* Put audio output back to its initial configuration */
	priv->samprate = ALC1019_DEFAULT_SAMPRATE;
	priv->nchannels = ALC1019_DEFAULT_NCHANNELS;
	priv->bpsamp = ALC1019_DEFAULT_BPSAMP;
#if !defined(CONFIG_AUDIO_EXCLUDE_VOLUME) && !defined(CONFIG_AUDIO_EXCLUDE_BALANCE)
	priv->balance = b16HALF;	/* Center balance */
#endif

	/* Software reset.	This puts all ALC1019 registers back in their
	 * default state.
	 */
	int res = alc1019_exec_i2c_script(priv, codec_initial_script, sizeof(codec_initial_script) / sizeof(t_codec_init_script_entry));
	
	/*
	 * TODO : once setting i2s sample rate and data width is fixed by realtek, will uncomment the code,
	 * currently, the default config for i2s must be set to 48KHz and 16bit in amebasmart_i2s.c
	 *
	 *alc1019_set_i2s_samplerate(priv);
	 *alc1019_set_i2s_datawidth(priv);
	 */

	return;
}

/****************************************************************************
 * Name: alc1019_hw_reset
 *
 * Description:
 *   Reset and re-initialize the ALC1019
 *
 * Input Parameters:
 *   priv - A reference to the driver state structure
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void alc1019_hw_reset(FAR struct alc1019_dev_s *priv)
{
	if (!priv) {
		return;
	}

	struct alc1019_lower_s *lower = priv->lower;

	if (lower->control_hw_reset) {
		lower->control_hw_reset(true);
		up_mdelay(100);
		lower->control_hw_reset(false);
		up_mdelay(100);
	}
	alc1019_reset_config(priv);
	alc1019_exec_i2c_script(priv, codec_init_mute_on_script, sizeof(codec_init_mute_on_script) / sizeof(t_codec_init_script_entry));
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: alc1019_initialize
 *
 * Description:
 *   Initialize the ALC1019 device.
 *
 * Input Parameters:
 *   i2c     - An I2C driver instance
 *   i2s     - An I2S driver instance
 *   lower   - Persistent board configuration data
 *
 * Returned Value:
 *   A new lower half audio interface for the ALC1019 device is returned on
 *   success; NULL is returned on failure.
 *
 ****************************************************************************/
FAR struct audio_lowerhalf_s *alc1019_initialize(FAR struct i2c_dev_s *i2c, FAR struct i2s_dev_s *i2s, FAR struct alc1019_lower_s *lower)
{

	FAR struct alc1019_dev_s *priv;
	uint8_t regval;
	/* Sanity check */
	DEBUGASSERT(i2c && lower && i2s);

	auddbg("I2s dev addr is 0x%x\n", i2s);

	/* Allocate a ALC1019 device structure */
	priv = (FAR struct alc1019_dev_s *)kmm_zalloc(sizeof(struct alc1019_dev_s));

	if (!priv) {
		return NULL;
	}

	/* Initialize the ALC1019 device structure.  Since we used kmm_zalloc,
	 * only the non-zero elements of the structure need to be initialized.
	 */

	priv->dev.ops = &g_audioops;
	priv->lower = lower;
	priv->i2c = i2c;
	priv->i2s = i2s;

	sem_init(&priv->devsem, 0, 1);
	sq_init(&priv->pendq);

	/* Software reset.  This puts all ALC1019 registers back in their
	 * default state.
	 */

	/*reconfigure the ALC1019 hardwaqre */
	alc1019_reset_config(priv);
	alc1019_exec_i2c_script(priv, codec_init_mute_on_script, sizeof(codec_init_mute_on_script) / sizeof(t_codec_init_script_entry));

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	priv->volume = ALC1019_SPK_VOL_DEF;
	alc1019_setvolume(priv);
#endif

	return &priv->dev;
}

