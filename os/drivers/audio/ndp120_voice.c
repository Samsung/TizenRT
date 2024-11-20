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

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/gpio.h>
#include <tinyara/kmalloc.h>
#include <tinyara/audio/ndp120.h>
#include "ndp120_voice.h"

#ifdef CONFIG_PM
#include <tinyara/pm/pm.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NDP120_SAMPLE_RX_RATE	AUDIO_SAMP_RATE_16K

#define NDP120_BITS_PER_SAMPLE	16

#define NDP120_SPI_CHANNEL_NUM	1

/* currently unused */
#define NDP120_MIC_GAIN_MAX	10
#define NDP120_MIC_GAIN_DEFAULT	7

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int ndp120_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_configure(FAR struct audio_lowerhalf_s *dev, FAR void *session, FAR const struct audio_caps_s *caps);
#else
static int ndp120_configure(FAR struct audio_lowerhalf_s *dev, FAR const struct audio_caps_s *caps);
#endif
static int ndp120_shutdown(FAR struct audio_lowerhalf_s *dev);

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_start(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int ndp120_start(FAR struct audio_lowerhalf_s *dev);
#endif

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int ndp120_stop(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session);
static int ndp120_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int ndp120_pause(FAR struct audio_lowerhalf_s *dev);
static int ndp120_resume(FAR struct audio_lowerhalf_s *dev);
#endif
#endif
static int ndp120_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int ndp120_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb);
static int ndp120_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg);
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session);
#else
static int ndp120_reserve(FAR struct audio_lowerhalf_s *dev);
#endif
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_release(FAR struct audio_lowerhalf_s *dev, FAR void *session);
#else
static int ndp120_release(FAR struct audio_lowerhalf_s *dev);
#endif

#ifdef CONFIG_PM
static struct ndp120_dev_s *g_ndp120;
static int g_ndp120_pm_id;

static void ndp_pm_notify(struct pm_callback_s *cb, enum pm_state_e pmstate);
static int ndp_pm_prepare(struct pm_callback_s *cb, enum pm_state_e pmstate);

static struct pm_callback_s g_pmndpcb =
{
	.notify  = ndp_pm_notify,
	.prepare = ndp_pm_prepare,
};
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct audio_ops_s g_audioops = {
	ndp120_getcaps,           /* getcaps        */
	ndp120_configure,         /* configure      */
	ndp120_shutdown,          /* shutdown       */
	ndp120_start,             /* start          */
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	ndp120_stop,              /* stop           */
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
	ndp120_pause,             /* pause          */
	ndp120_resume,            /* resume         */
#endif
	NULL,                     /* allocbuffer    */
	NULL,                     /* freebuffer     */
	ndp120_enqueuebuffer,     /* enqueue_buffer */
	ndp120_cancelbuffer,      /* cancel_buffer  */
	ndp120_ioctl,             /* ioctl          */
	NULL,                     /* read           */
	NULL,                     /* write          */
	ndp120_reserve,           /* reserve        */
	ndp120_release,           /* release        */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * ndp120 semaphore functions
 ****************************************************************************/

static inline void ndp120_takesem(sem_t *sem)
{
	int ret;

	do {
		ret = sem_wait(sem);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

static inline int ndp120_givesem(sem_t *sem)
{
	return sem_post(sem);
}

static inline int ndp120_get_semvalue(sem_t *sem)
{
	int val;
	int ret;
	ret = sem_getvalue(sem, &val);
	if (ret < 0) {
		auddbg(" could not get semaphore value\n");
	}
	return val;
}

/****************************************************************************
 * ndp120 audio operations
 ****************************************************************************/

static int ndp120_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps)
{
	/* Validate the structure */
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;
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

			caps->ac_channels = 1; /* Mono output */
		
		switch (caps->ac_subtype) {
		case AUDIO_TYPE_QUERY:
			/* We don't decode any formats!  Only something above us in
			 * the audio stream can perform decoding on our behalf.
			 */

			/* The types of audio units we implement */

			caps->ac_controls.b[0] = AUDIO_TYPE_INPUT | AUDIO_TYPE_FEATURE | AUDIO_TYPE_PROCESSING;
			break;
		default:
			caps->ac_controls.b[0] = AUDIO_SUBFMT_END; /* what is this used for???? */
			break;
		}
		break;
	case AUDIO_TYPE_INPUT:
			caps->ac_channels = 1;
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
		switch (caps->ac_subtype) {
		case AUDIO_FU_INP_GAIN:
			caps->ac_controls.hw[0] = NDP120_MIC_GAIN_MAX;
#ifndef CONFIG_AUDIO_EXCLUDE_GAIN
			caps->ac_controls.hw[1] = priv->mic_gain;
#else
			caps->ac_controls.hw[1] = NDP120_MIC_GAIN_DEFAULT;
#endif
			break;

		default:
			break;
		}
		break;
	case AUDIO_TYPE_PROCESSING:
		audvdbg("\n\tAUDIO_TYPE_PROCESSING, type:%d, subtype:%d\n", caps->ac_type, caps->ac_subtype);

		switch (caps->ac_subtype) {
		case AUDIO_PU_UNDEF:
			caps->ac_controls.b[0] =
#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
				AUDIO_PU_SPEECH_DETECT |
#endif
				AUDIO_PU_UNDEF;
			break;

		case AUDIO_PU_SPEECH_DETECT:
			audvdbg("\n\tNDP120, AUDIO_PU_SPPECH_DETECT, ac_controls.b[0] = %d\n", caps->ac_controls.b[0]);
			/* Provide capabilities of our Speech Detect */
			caps->ac_controls.b[0] =
#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
				AUDIO_SD_KEYWORD_DETECT |
#ifdef CONFIG_NDP120_AEC_SUPPORT
				AUDIO_SD_AEC_ON | AUDIO_SD_AEC_OFF |
#endif
#endif
#endif
				AUDIO_SD_UNDEF;
			break;

		default:
			/* Other types of processing unit we don't support */
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

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_configure(FAR struct audio_lowerhalf_s *dev,
							FAR void *session,
							FAR const struct audio_caps_s *caps)
#else
static int ndp120_configure(FAR struct audio_lowerhalf_s *dev,
							FAR const struct audio_caps_s *caps)
#endif
{
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;
	int ret = 0;

	DEBUGASSERT(priv && caps);
	audvdbg("ac_type: %d\n", caps->ac_type);

	/* Process the configure operation */

	switch (caps->ac_type) {
	case AUDIO_TYPE_INPUT:
		audvdbg("AUDIO_TYPE_INPUT");
		break;
	case AUDIO_TYPE_FEATURE:
		audvdbg("AUDIO_TYPE_FEATURE");

		/* Process based on Feature Unit */
		switch (caps->ac_format.hw) {
		case AUDIO_FU_INP_GAIN: {
#ifndef CONFIG_AUDIO_EXCLUDE_GAIN
			uint16_t gain = caps->ac_controls.hw[0];
			if (gain <= NDP120_MIC_GAIN_MAX) {
				ndp120_takesem(&priv->devsem);
				priv->mic_gain = gain;
				/* No api to control gain as of now */
				ndp120_givesem(&priv->devsem);
			} else {
				ret = -EDOM;
			}
#else /* CONFIG_AUDIO_EXCLUDE_GAIN */
			ret = -EACCESS;
#endif
		} break;
		case AUDIO_FU_MUTE: {
#if !(defined(CONFIG_AUDIO_EXCLUDE_GAIN) && defined(CONFIG_AUDIO_EXCLUDE_VOLUME))
			/* Mute or unmute:  true(1) or false(0) */
			bool mute = caps->ac_controls.b[0];
			audvdbg("mute: 0x%x\n", mute);
			ndp120_takesem(&priv->devsem);
			priv->mute = mute;
			/* No api to control gain as of now */
			ndp120_givesem(&priv->devsem);
#else
			ret = -EACCESS;
#endif
		} break;
		default:
			audvdbg("ERROR: Unrecognized feature unit\n");
			break;
		}

		break;
	case AUDIO_TYPE_PROCESSING:
		auddbg("AUDIO_TYPE_PROCESSING");
		switch (caps->ac_subtype) {
		case AUDIO_PU_SPEECH_DETECT:
			switch (caps->ac_controls.w) {
#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
			case AUDIO_SD_KEYWORD_DETECT:
			case AUDIO_SD_LOCAL:
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
				/* DO Nothing for now */
#else
				ret = -EINVAL;
#endif
				break;
#endif
			}
			break;
		default:
			break;
		}
		break;
	default:
		audvdbg("ndp120_configure received unknown ac_type 0x%x\n", caps->ac_type);
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int ndp120_shutdown(FAR struct audio_lowerhalf_s *dev)
{
	/* TBD:disable interrupts */

	return 0;
}

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_start(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int ndp120_start(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;
	if (priv->running) {
		return 0;
	}

	audvdbg(" ndp120_start Entry\n");
	ndp120_takesem(&priv->devsem);

	ndp120_start_sample_ready(priv);
	priv->running = true;

	/* Enqueue buffers (enqueueed before the start of alc) to lower layer */
	sq_entry_t *tmp = NULL;
	sq_queue_t *q = &priv->pendq;
	for (tmp = sq_peek(q); tmp; tmp = sq_next(tmp)) {
		ndp120_enqueuebuffer(dev, (struct ap_buffer_s *)tmp);
	}
	/* Remove audio buffers from pending queue here */
	while ((tmp = sq_remfirst(&priv->pendq)) != NULL) {
		apb_free((struct ap_buffer_s *)tmp);
	}
	
	ndp120_givesem(&priv->devsem);
	return 0;
}

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_stop(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int ndp120_stop(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;
	if (!priv) {
		return -EINVAL;
	}

	audvdbg(" ndp120_i2s_stop Entry\n");
	ndp120_takesem(&priv->devsem);

	ndp120_stop_sample_ready(priv);

	priv->running = false;
	
	ndp120_givesem(&priv->devsem);
	return 0;
}
#endif

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_pause(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int ndp120_pause(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;
	ndp120_takesem(&priv->devsem);
	ndp120_stop_sample_ready(priv);
	ndp120_givesem(&priv->devsem);
	return 0;
}

#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_resume(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int ndp120_resume(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;
	ndp120_takesem(&priv->devsem);
	ndp120_start_sample_ready(priv);
	ndp120_givesem(&priv->devsem);
	return 0;
}
#endif

/****************************************************************************
 * Name: ndp120_enqueuebuffer
 *
 * Description: Enqueue an Audio Pipeline Buffer for playback/ processing.
 *
 ****************************************************************************/

static int ndp120_enqueuebuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;

	DEBUGASSERT(priv && priv->dev.upper && apb);

	audvdbg("ndp120_enqueuebuffer: apbadr = 0x%x\n", apb);

	/* TODO pendq should be handled by ndp120_api.c later. worker & interrupt logic need to be implemented there */
	if (!priv->running) {
		/* Add the new buffer to the tail of pending audio buffers */
		ndp120_takesem(&priv->devsem);
		sq_addlast((sq_entry_t *)&apb->dq_entry, &priv->pendq);
		audvdbg("enqueue added buf 0x%x\n", apb);
		ndp120_givesem(&priv->devsem);
		return 0;
	}

	sq_entry_t *tmp;

	int ret = ndp120_extract_audio(priv, apb);
	
	for (tmp = (sq_entry_t *)sq_peek(&priv->pendq); tmp; tmp = sq_next(tmp)) {
		if (tmp == (sq_entry_t *)apb) {
			sq_rem(tmp, &priv->pendq);
			audvdbg("found the apb to remove 0x%x\n", tmp);
			break;
		}
	}
	priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, ret);
	
	return 0;
}

static int ndp120_cancelbuffer(FAR struct audio_lowerhalf_s *dev, FAR struct ap_buffer_s *apb)
{
	/* TBD */
	return 0;
}

static int ndp120_spi_registerprocess(FAR struct audio_lowerhalf_s *dev, struct mq_des *mq)
{
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;

	if (priv->dev.process_mq == NULL) {
		priv->dev.process_mq = mq;
		return OK;
	}
	auddbg("already registered!!\n");
	return -EBUSY;
}

static int ndp120_spi_unregisterprocess(FAR struct audio_lowerhalf_s *dev)
{
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;
	int ret = OK;
	ndp120_takesem(&priv->devsem);
	if (priv->dev.process_mq != NULL) {
		priv->dev.process_mq = NULL;
	} else {
		auddbg("mq is null\n");
		ret = -ENOENT;
	}
	ndp120_givesem(&priv->devsem);
	return ret;
}

static int ndp120_ioctl(FAR struct audio_lowerhalf_s *dev, int cmd, unsigned long arg)
{
	FAR struct ap_buffer_info_s *bufinfo;
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	/* Deal with ioctls passed from the upper-half driver */

	int ret = 0;
	switch (cmd) {
	case AUDIOIOC_PREPARE: {
		/* nothing to prepare... */
	} break;
	case AUDIOIOC_GETBUFFERINFO: {
		/* Report our preferred buffer size and quantity */
		audvdbg("AUDIOIOC_GETBUFFERINFO:\n");
		/* Take semaphore */
		ndp120_takesem(&priv->devsem);

		bufinfo = (FAR struct ap_buffer_info_s *)arg;

		bufinfo->buffer_size = 4 * priv->sample_size;
		bufinfo->nbuffers = CONFIG_NDP120_NUM_BUFFERS;
		
		audvdbg("buffer_size : %d nbuffers : %d\n",
				bufinfo->buffer_size, bufinfo->nbuffers);

		/* Give semaphore */
		ndp120_givesem(&priv->devsem);
	} break;
	case AUDIOIOC_REGISTERPROCESS: {
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
		ret = ndp120_spi_registerprocess(dev, (mqd_t)arg);
		if (ret != 0) {
			auddbg("Process Start Failed ret : %d\n", ret);
			return ret;
		}
#else
		audvdbg("Register Process Failed - Device Doesn't support\n");
		ret = -EINVAL;
#endif
	} break;
	case AUDIOIOC_UNREGISTERPROCESS: {
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
		ret = ndp120_spi_unregisterprocess(dev);
		if (ret != 0) {
			auddbg("Process Start Failed ret : %d\n", ret);
			return ret;
		}
#else
		auddbg("UnRegister Process Failed - Device Doesn't support\n");
		ret = -EINVAL;
#endif
	} break;
	case AUDIOIOC_STARTPROCESS: {
		audvdbg("set start process!!\n");
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
		switch ((uint8_t)arg) {
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
		case AUDIO_SD_LOCAL:
		case AUDIO_SD_KEYWORD_DETECT: {
			if (priv->kd_enabled == false) {
				ndp120_kd_start_match_process(priv);
				priv->kd_enabled = true;
			}
		} break;
#endif
		case AUDIO_SD_AEC_ON: {
#ifdef CONFIG_NDP120_AEC_SUPPORT
			ndp120_aec_enable(priv);
#endif
		} break;
		case AUDIO_SD_AEC_OFF: {
#ifdef CONFIG_NDP120_AEC_SUPPORT
			ndp120_aec_disable(priv);
#endif
		} break;
		default: {
			/* DO Nothing for now */
		} break;
		}
#else
		audvdbg("start Process Failed - Device Doesn't support\n");
		ret = -EINVAL;
#endif	/* CONFIG_AUDIO_PROCESSING_FEATURES */
	} break;
	case AUDIOIOC_STOPPROCESS: {
		audvdbg("set stop process!!\n");
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
		case AUDIO_SD_KEYWORD_DETECT: {
			if (priv->kd_enabled == true) {
				ndp120_kd_stop_match_process(priv);
				priv->kd_enabled = false;
			}
		} break;
#endif
#else
		audvdbg("start Process Failed - Device Doesn't support\n");
		ret = -EINVAL;
#endif	/* CONFIG_AUDIO_PROCESSING_FEATURES */
	} break;
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
	case AUDIOIOC_GETKDBUFSIZE: {
		*(uint32_t *)arg = priv->keyword_bytes;
	} break;
	case AUDIOIOC_GETKDDATA: {
		memcpy((uint8_t *)arg, priv->keyword_buffer, priv->keyword_bytes);
		priv->keyword_bytes_left = 0;
	} break;
#endif  /* CONFIG_AUDIO_KEYWORD_DETECT */
#endif  /* CONFIG_AUDIO_PROCESSING_FEATURES */
	case AUDIOIOC_ENABLEDMIC: {
		if (priv->lower && priv->lower->set_dmic) {
			bool enable = (bool)arg;
			priv->lower->set_dmic(enable);
			ret = OK;
		} else {
			ret = -ENOSYS;
		}
		break;
	}
	default:
		audvdbg("ndp120_ioctl received unkown cmd 0x%x\n", cmd);
		ret = -EINVAL;
		break;
	}

	return ret;
}

#ifdef CONFIG_AUDIO_MULTI_SESSION
int ndp120_reserve(FAR struct audio_lowerhalf_s *dev, FAR void **session)
#else
int ndp120_reserve(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;
	int ret = 0;

	/* Borrow the APBQ semaphore for thread sync */

	ndp120_takesem(&priv->devsem);
	if (priv->reserved) {
		ret = -EBUSY;
	} else {
		/* Initialize the session context */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		*session = NULL;
#endif
		priv->running = false;
		priv->reserved = true;
	}

	ndp120_givesem(&priv->devsem);

	return ret;
}

/****************************************************************************
 * Name: ndp120_release
 *
 * Description: Releases the session (the only one we have).
 *
 ****************************************************************************/
#ifdef CONFIG_AUDIO_MULTI_SESSION
static int ndp120_release(FAR struct audio_lowerhalf_s *dev, FAR void *session)
#else
static int ndp120_release(FAR struct audio_lowerhalf_s *dev)
#endif
{
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;

	if (!priv) {
		return -EINVAL;
	}

	ndp120_takesem(&priv->devsem);
	if (priv->running) {
		priv->running = false;
	}
	priv->reserved = false;
	ndp120_givesem(&priv->devsem);

	return 0;
}

static void ndp120_interrupt_dispatch(int d)
{
	struct ndp120_dev_s *priv = (struct ndp120_dev_s *)d;
#ifdef CONFIG_PM
	pm_timedsuspend(g_ndp120_pm_id, 10000);
#endif
	ndp120_irq_handler(priv);
}

#ifdef CONFIG_PM
/****************************************************************************
 * Name: ndp_pm_notify
 *
 * Description:
 *   Notify the driver of new power state. This callback is called after
 *   all drivers have had the opportunity to prepare for the new power state.
 *
 ****************************************************************************/

static void ndp_pm_notify(struct pm_callback_s *cb, enum pm_state_e state)
{
	/* Currently PM follows the state changes as follows,
	 * On boot, we are in PM_NORMAL. After that we only use PM_STANDBY and PM_SLEEP
	 * on boot : PM_NORMAL -> PM_STANDBY -> PM_SLEEP, from there on
	 * PM_SLEEP -> PM_STANBY -> PM_SLEEP -> PM_STANBY........
	 */
	switch (state) {
	case(PM_SLEEP): {
		audvdbg("entering SLEEP\n");
#ifdef CONFIG_NDP120_AEC_SUPPORT
		ndp120_set_intclk(g_ndp120);
#endif
	}
	break;
	default: {
		/* Nothing to do */
		audvdbg("default case\n");
	}
	break;
	}
}

/****************************************************************************
 * Name: ndp_pm_prepare
 *
 * Description:
 *   Request the driver to prepare for a new power state. This is a warning
 *   that the system is about to enter into a new power state. The driver
 *   should begin whatever operations that may be required to enter power
 *   state. The driver may abort the state change mode by returning a
 *   non-zero value from the callback function.
 *
 ****************************************************************************/

static int ndp_pm_prepare(struct pm_callback_s *cb, enum pm_state_e state)
{
	audvdbg("entry\n");
	return OK;
}
#endif	/* End of CONFIG_PM */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ndp120_lowerhalf_initialize
 *
 * Description:
 *   Initialize the NDP120 device.
 *
 * Input Parameters:
 *
 * Returned Value:
 *   A new lower half audio interface for the ndp120 device is returned on
 *   success; NULL is returned on failure.
 *
 ****************************************************************************/
FAR struct audio_lowerhalf_s *ndp120_lowerhalf_initialize(FAR struct spi_dev_s *spi, FAR struct ndp120_lower_s *lower)
{
	FAR struct ndp120_dev_s *priv;
	int ret;

	/* Sanity check */

	DEBUGASSERT(spi);

	/* Allocate a NDP120 device structure */
	priv = (FAR struct ndp120_dev_s *)kmm_zalloc(sizeof(struct ndp120_dev_s));
	if (priv == NULL) {
		return NULL;
	}

	priv->dev.ops = &g_audioops;
	priv->spi = spi;
	priv->mic_gain = NDP120_MIC_GAIN_DEFAULT;
	sq_init(&priv->pendq);
	sem_init(&priv->devsem, 0, 1);

	priv->lower = lower;
	priv->recording = false;

	ret = ndp120_init(priv);
	if (ret != OK) {
		auddbg("ndp120 init failed\n");
		free(priv);
		return NULL;
	}

	priv->lower->attach(ndp120_interrupt_dispatch, priv);
#ifdef CONFIG_PM
	/* only used during pm callbacks */
	g_ndp120 = priv;

	g_ndp120_pm_id = pm_domain_register("NDP120");
	ret = pm_register(&g_pmndpcb);
	DEBUGASSERT(ret == OK);
#endif	
	return &priv->dev;
}
