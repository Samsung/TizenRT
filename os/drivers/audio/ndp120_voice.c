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

/*
 * SYNTIANT CONFIDENTIAL
 * _____________________
 *
 *   Copyright (c) 2018-2022 Syntiant Corporation
 *   All Rights Reserved.
 *
 *  NOTICE:  All information contained herein is, and remains the property of
 *  Syntiant Corporation and its suppliers, if any.  The intellectual and
 *  technical concepts contained herein are proprietary to Syntiant Corporation
 *  and its suppliers and may be covered by U.S. and Foreign Patents, patents in
 *  process, and are protected by trade secret or copyright law.  Dissemination
 *  of this information or reproduction of this material is strictly forbidden
 *  unless prior written permission is obtained from Syntiant Corporation.
*/

/*
 * Copyright (c) 2026 Syntiant Corp.  All rights reserved.
 * Contact at http://www.syntiant.com
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file LICENSE in the main
 * directory of this source tree, or the OpenIB.org BSD license below.  Any
 * code involving Linux software will require selection of the GNU General
 * Public License (GPL) Version 2.
 *
 * OPENIB.ORG BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
*/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <poll.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <tinyara/audio/i2s.h>
#include <tinyara/audio/ndp120.h>
#include <tinyara/gpio.h>
#include <tinyara/kmalloc.h>
#include <tinyara/pm/pm.h>
#include <tinyara/spi/spi.h>
#include <tinyara/wqueue.h>

#include <syntiant-firmware/ndp120_firmware.h>
#include <syntiant_ilib/ndp120_spi_regs.h>
#include <syntiant_ilib/syntiant_ndp.h>
#include <syntiant_ilib/syntiant_ndp120.h>
#include <syntiant_ilib/syntiant_ndp120_mailbox.h>
#include <syntiant_ilib/syntiant_ndp_error.h>
#include <syntiant_ilib/syntiant_ndp_ilib_version.h>

#include "ndp120_voice.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NDP120_SAMPLE_RX_RATE AUDIO_SAMP_RATE_16K
#define NDP120_BITS_PER_SAMPLE 16
#define NDP120_SPI_CHANNEL_NUM 1
#define NDP120_MIC_GAIN_MAX 10
#define NDP120_MIC_GAIN_DEFAULT 7
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define round_down(x, y) ((x) - ((x) % (y)))
#define STRING_LEN 256
#define AUDIO_BEFORE_MATCH_MS 2000
#define PDM_CLOCK_PDM_RATE 1536000
#define KEYWORD_BUFFER_LEN (SYNTIANT_NDP120_AUDIO_SAMPLE_RATE * SYNTIANT_NDP120_AUDIO_SAMPLES_PER_WORD * AUDIO_BEFORE_MATCH_MS / 1000)
#define NDP120_SPI_FREQ_HIGH 12000000
#define NDP120_SPI_FREQ_INIT 1000000
#define FF_ID NDP120_DSP_DATA_FLOW_FUNCTION_FULL_FF_49
#define SR_FE_POOLING_ID 227
#define KEYWORD_NETWORK_ID 0
#define NDP_ALIVENESS_CHECK_PERIOD_US (3 * 1000 * 1000)
#define COMBINED_FLOW_SET_ID 0
#define SHOW_DEBUG 0
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

/* Define CONFIG_DEBUG_AUDIO_INFO to print flow rules during initialization. */

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_NDP120_AEC_SUPPORT
enum ndp120_state_e {
	IS_RECORDING = 0,
	NOT_RECORDING = 1,
};

enum ndp120_state_e g_ndp120_state;
#endif

static const unsigned int NDP_NOTIFICATION_ERRORS =
	SYNTIANT_NDP_NOTIFICATION_ERROR
	| SYNTIANT_NDP_NOTIFICATION_ALGO_ERROR
	| SYNTIANT_NDP_NOTIFICATION_DNN_MEM_ERROR
	| SYNTIANT_NDP_NOTIFICATION_DSP_MEM_ERROR
	| SYNTIANT_NDP_NOTIFICATION_SPI_READ_FAILURE;

static char s_label_data[NDP120_MCU_LABELS_MAX_LEN];
static char *s_labels[MAX_LABELS];
static unsigned int s_num_labels;

static struct work_s ndp120_work;

/* only used for debugging purposes */
static struct ndp120_dev_s *_ndp_debug_handle = NULL;

typedef enum {
	DSP_FLOW_BIXBY = 0,
	DSP_CUSTOM_FLOW1 = 1,
	DSP_FLOW_MAX,
} dsp_flow_e;

dsp_flow_e idToFlow[MAX_NNETWORKS];

typedef struct {
	char *label;
	dsp_flow_e flow;
} dsp_flow_t;

static const dsp_flow_t g_flow_types[] = {
	{"hi-bixby", DSP_FLOW_BIXBY},
	{"bixby", DSP_FLOW_BIXBY},
	{"aod", DSP_CUSTOM_FLOW1}
};

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/
int ndp120_init(struct ndp120_dev_s *dev);
int ndp120_load_firmware(struct ndp120_dev_s *dev);
void ndp120_aec_enable(struct ndp120_dev_s *dev);
void ndp120_aec_disable(struct ndp120_dev_s *dev);
void ndp120_test_internal_passthrough_switch(struct ndp120_dev_s *dev, int internal);
int ndp120_irq_handler(struct ndp120_dev_s *dev);
int ndp120_extract_audio(struct ndp120_dev_s *dev, struct ap_buffer_s *apb);
int ndp120_kd_start(struct ndp120_dev_s *dev);
int ndp120_kd_stop(struct ndp120_dev_s *dev);
int ndp120_change_kd(struct ndp120_dev_s *dev, uint8_t kd_num);
int ndp120_kd_start_match_process(struct ndp120_dev_s *dev);
int ndp120_kd_stop_match_process(struct ndp120_dev_s *dev);
int ndp120_start_sample_ready(struct ndp120_dev_s *dev);
int ndp120_stop_sample_ready(struct ndp120_dev_s *dev);
int ndp120_kw_sensitivity_set(struct ndp120_dev_s *dev, uint16_t sensitivity);
int ndp120_kw_sensitivity_get(struct ndp120_dev_s *dev, uint16_t *sensitivity);
int ndp120_change_dsp_flow(struct ndp120_dev_s *dev, uint8_t dsp_flow_num);
#ifdef CONFIG_DUMP4CH_SUPPORT
int ndp120_utils_stream_init(struct ndp120_dev_s *dev, unsigned int duration, int verbose, int *dev_extract_size);
int ndp120_utils_stream_deinit(struct ndp120_dev_s *dev);
int ndp120_utils_stream_get_data(struct ndp120_dev_s *dev, uint8_t *data, uint32_t *extracted_size);
#endif

static void do_ndp120_i2s_setup(struct syntiant_ndp_device_s *ndp);
static void attach_algo_config_area(struct syntiant_ndp_device_s *ndp, int32_t algo_id, int32_t algo_config_index);

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

static int ndp120_setMute(FAR struct ndp120_dev_s *priv, bool mute)
{
	int ret = 0;
	audvdbg("mute : %d\n", mute);
	/* if NDP has not been initialized, return without doing anything */
	if (!priv->ndp) {
		return 0;
	}
	if (mute) {
		ret = ndp120_kd_stop(priv);
		if (ret != 0) {
			auddbg("ndp120_kd_stop failed ret : %d\n", ret);
			return ret;
		}
#ifdef CONFIG_AUDIO_MULTI_SESSION
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_MICMUTE, NULL, OK, NULL);
#else
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_MICMUTE, NULL, OK);
#endif
	} else {
		uint32_t notifications = 0;
		struct syntiant_ndp_device_s *ndp = priv->ndp;
		ret = syntiant_ndp120_poll(ndp, &notifications, 1);
		if (ret != 0) {
			auddbg("ndp120 poll failed ret: %d\n", ret);
			return ret;
		}
		ret = ndp120_kd_start(priv);
		if (ret != 0) {
			auddbg("ndp120_kd_start failed ret : %d\n", ret);
			return ret;
		}
#ifdef CONFIG_AUDIO_MULTI_SESSION
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_MICUNMUTE, NULL, OK, NULL);
#else
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_MICUNMUTE, NULL, OK);
#endif
	}
	return ret;
}

/****************************************************************************
 * ndp120 audio operations
 ****************************************************************************/

static int ndp120_getcaps(FAR struct audio_lowerhalf_s *dev, int type, FAR struct audio_caps_s *caps)
{
	/* Validate the structure */
	FAR struct ndp120_dev_s *priv = (FAR struct ndp120_dev_s *)dev;
	int ret = 0;

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
			return -ENOSYS;
		case AUDIO_FU_MUTE:
			ndp120_takesem(&priv->devsem);
			caps->ac_controls.b[0] = priv->mute;
			ndp120_givesem(&priv->devsem);
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
				AUDIO_SD_AEC |
#endif
#endif
#endif
				AUDIO_SD_UNDEF;
			break;
		case AUDIO_PU_KD_SENSITIVITY: {
			uint16_t sensitivity;
			ndp120_takesem(&priv->devsem);
			ret = ndp120_kw_sensitivity_get(priv, &sensitivity);
			if (ret != 0) {
				auddbg("ndp120_kw_sensitivity_get failed ret : %d\n", ret);
				ndp120_givesem(&priv->devsem);
				return -EIO;
			}
			caps->ac_controls.w = sensitivity;
			ndp120_givesem(&priv->devsem);
		}
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
			return -ENOSYS;
		}
		break;
		case AUDIO_FU_MUTE: {
			/* Mute or unmute:  true(1) or false(0) */
			bool mute = caps->ac_controls.b[0];
			audvdbg("mute: 0x%x\n", mute);
			ndp120_takesem(&priv->devsem);
			ret = ndp120_setMute(priv, mute);
			if (ret != 0) {
				auddbg("ndp120_setMute failed ret : %d\n", ret);
				return ret;
			}
			priv->mute = mute;
			/* No api to control gain as of now */
			ndp120_givesem(&priv->devsem);
		}
		break;
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
		case AUDIO_PU_KD_SENSITIVITY: {
			uint16_t sensitivity = caps->ac_controls.w;
			ndp120_takesem(&priv->devsem);
			ret = ndp120_kw_sensitivity_set(priv, sensitivity);
			if (ret != 0) {
				auddbg("ndp120_kw_sensitivity_set failed ret : %d\n", ret);
				ndp120_givesem(&priv->devsem);
				return -EIO;
			}
			ndp120_givesem(&priv->devsem);
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

	if (priv->mute) {
		return -ESTRPIPE;
	}

	audvdbg(" ndp120_start Entry\n");
	ndp120_takesem(&priv->devsem);

	ndp120_start_sample_ready(priv);
	priv->running = true;
	priv->total_size = 0;

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
	auddbg("Total record size : %lu\n", priv->total_size);
	ndp120_stop_sample_ready(priv);

	priv->running = false;
	priv->total_size = 0;
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

	/* TODO pendq should be handled by ndp120.c later. worker & interrupt logic need to be implemented there */
	if (!priv->running) {
		/* Add the new buffer to the tail of pending audio buffers */
		ndp120_takesem(&priv->devsem);
		sq_addlast((sq_entry_t *)&apb->dq_entry, &priv->pendq);
		audvdbg("enqueue added buf 0x%x\n", apb);
		ndp120_givesem(&priv->devsem);
		return OK;
	}

	sq_entry_t *tmp;

	int ret = ndp120_extract_audio(priv, apb);
	if (ret == SYNTIANT_NDP_ERROR_UNINIT) {
		// notify upper layer to stop capture, hence here just return OK.
		priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_UNREACHABLE, NULL, OK);
		return OK;
	}

	for (tmp = (sq_entry_t *)sq_peek(&priv->pendq); tmp; tmp = sq_next(tmp)) {
		if (tmp == (sq_entry_t *)apb) {
			sq_rem(tmp, &priv->pendq);
			audvdbg("found the apb to remove 0x%x\n", tmp);
			break;
		}
	}

	priv->dev.upper(priv->dev.priv, AUDIO_CALLBACK_DEQUEUE, apb, ret);
	return OK;
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

	int ret = OK;
	switch (cmd) {
	case AUDIOIOC_PREPARE: {
		/* nothing to prepare... */
	}
	break;
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
	}
	break;
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
	}
	break;
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
	}
	break;
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
		}
		break;
#endif
		case AUDIO_SD_AEC: {
#ifdef CONFIG_NDP120_AEC_SUPPORT
			ndp120_aec_enable(priv);
#endif
		}
		break;
		default: {
			/* DO Nothing for now */
		}
		break;
		}
#else
		audvdbg("start Process Failed - Device Doesn't support\n");
		ret = -EINVAL;
#endif	/* CONFIG_AUDIO_PROCESSING_FEATURES */
	}
	break;
	case AUDIOIOC_STOPPROCESS: {
		audvdbg("set stop process!!\n");
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
		case AUDIO_SD_KEYWORD_DETECT: {
			if (priv->kd_enabled == true) {
				ndp120_kd_stop_match_process(priv);
				priv->kd_enabled = false;
			}
		}
		break;
#endif
#else
		audvdbg("start Process Failed - Device Doesn't support\n");
		ret = -EINVAL;
#endif	/* CONFIG_AUDIO_PROCESSING_FEATURES */
	}
	break;
#ifdef CONFIG_AUDIO_PROCESSING_FEATURES
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
	case AUDIOIOC_GETKDBUFSIZE: {
		*(uint32_t *)arg = priv->keyword_bytes;
	}
	break;
	case AUDIOIOC_GETKDDATA: {
		memcpy((uint8_t *)arg, priv->keyword_buffer, priv->keyword_bytes);
		priv->keyword_bytes_left = 0;
	}
	break;
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
	case AUDIOIOC_CHANGEKD: {
		if (((arg & AUDIO_NN_MODEL_MASK) > AUDIO_NN_MODEL_MAX) ||
				((arg & AUDIO_NN_MODEL_LANG_MASK) > AUDIO_NN_MODEL_LANG_MAX)) {
			return -EINVAL;
		}
		if (priv->running) {
			return -EBUSY;
		}
		uint8_t kd_num = arg;
		if (ndp120_change_kd(priv, kd_num) != SYNTIANT_NDP_ERROR_NONE) {
			ret = -EIO;
		}
		break;
	}
	case AUDIOIOC_CHANGEDSPFLOW: {
		uint8_t dsp_flow_num = (uint8_t)arg;
		ret = ndp120_change_dsp_flow(priv, dsp_flow_num);
		if (ret != 0) {
			auddbg("ndp120_change_dsp_flow failed ret : %d\n", ret);
			return ret;
		}
		break;
	}
#ifdef CONFIG_DUMP4CH_SUPPORT
	case AUDIOIOC_MULTI_CH_STREAM_INIT: {
		/* stream init */
		struct audio_debug_dump_stream_init_s *stream_args;
		stream_args = (FAR struct audio_debug_dump_stream_init_s *)arg;
		int s = ndp120_utils_stream_init(priv, stream_args->duration, stream_args->verbose, stream_args->dev_extract_size);
		if (s) {
			ret = -EINVAL;
		}
		break;
	}
	case AUDIOIOC_MULTI_CH_STREAM_READ: {
		/* blocking read */
		struct audio_debug_dump_stream_read_s *read_args;
		read_args = (FAR struct audio_debug_dump_stream_read_s *)arg;
		int s = ndp120_utils_stream_get_data(priv, read_args->buffer, read_args->extracted_len);
		if (s) {
			ret = -EINTR;
		}
		break;
	}
	case AUDIOIOC_MULTI_CH_STREAM_DEINIT: {
		/* stream deinit */
		int s = ndp120_utils_stream_deinit(priv);
		if (s) {
			ret = -EINVAL;
		}
		break;
	}
#endif
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
	pm_timedsuspend(priv->pm_domain, 10000);
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
	 * On boot, we are in PM_NORMAL. After that we only use PM_NORMAL and PM_SLEEP
	 * on boot : PM_NORMAL -> PM_SLEEP, from there on
	 * PM_SLEEP -> PM_NORMAL -> PM_SLEEP -> PM_NORMAL........
	 */
	switch (state) {
	case(PM_SLEEP): {
		audvdbg("entering SLEEP\n");
#ifdef CONFIG_NDP120_AEC_SUPPORT
		ndp120_aec_disable(g_ndp120);
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
	priv->mute = false;
#ifdef CONFIG_PM
	/* only used during pm callbacks */
	g_ndp120 = priv;

	priv->pm_domain = pm_domain_register("NDP120");
	DEBUGASSERT(priv->pm_domain >= 0);
#endif

	int retry = NDP120_INIT_RETRY_COUNT;
	while (retry--) {
		lower->reset();
		ret = ndp120_init(priv);
		if (ret != SYNTIANT_NDP_ERROR_NONE) {
			auddbg("ndp120 init failed\n");
			ret = -EIO;
		} else {
			break;
		}
	}

	if (ret != OK) {
		pm_domain_unregister(priv->pm_domain);
		g_ndp120 = NULL;
		free(priv);
		return NULL;
	}

#ifdef CONFIG_PM
	/* register callbacks only if NDP init is done */
	ret = pm_register(&g_pmndpcb);
	DEBUGASSERT(ret == OK);
#endif

	priv->lower->attach(ndp120_interrupt_dispatch, priv);
	return &priv->dev;
}
void ndp120_semtake(struct ndp120_dev_s *dev)
{
	while (sem_wait(&dev->reset_sem) != 0) {
		/* The only case that an error should occur here is if
		 * the wait was awakened by a signal.
		 */

		ASSERT(*get_errno_ptr() == EINTR);
	}
}

void ndp120_semgive(struct ndp120_dev_s *dev)
{
	sem_post(&dev->reset_sem);
}

static int check_status(char *message, int s)
{
	if (s) {
		auddbg("%s failed: %s\n", message, syntiant_ndp_error_name(s));
	}
	return s;
}

static int check_io(char *message, int expected_len, int len)
{
	if (len < expected_len) {
		if (len < 0) {
			auddbg("%s failed\n", message);
		} else {
			auddbg("%s truncated\n", message);
		}
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	return SYNTIANT_NDP_ERROR_NONE;
}

static void timer_start(struct timespec *ts)
{
	int s;

	s = clock_gettime(CLOCK_REALTIME, ts);
}


static int timer_check(struct timespec *start, unsigned int timeout)
{
	int s, done;
	struct timespec ts;

	s = clock_gettime(CLOCK_REALTIME, &ts);

	done = start->tv_sec + timeout < ts.tv_sec
		|| (start->tv_sec + timeout == ts.tv_sec
			&& start->tv_nsec <= ts.tv_nsec);
	return done;
}

int tizenrt_io_spi_transfer(struct ndp120_dev_s *dev, int mcu, uint32_t addr, const void *out, void *in, int count)
{
	uint8_t spi_cmd[4];
	int s = SYNTIANT_NDP_ERROR_NONE;
	int read_delay = 4; /* check later */

	if (in && out) {
		s = SYNTIANT_NDP_ERROR_ARG;
		goto error;
	}

	SPI_LOCK(dev->spi, 1);

	/* reconfigure SPI parameters for NDP120 */
	SPI_SETMODE(dev->spi, dev->lower->spi_config.mode);
	SPI_SETFREQUENCY(dev->spi, dev->lower->spi_config.freq);
	SPI_SETBITS(dev->spi, dev->lower->spi_config.bpw);

	if (mcu) {
		if ((count & 0x3) != 0) {
			s = SYNTIANT_NDP_ERROR_ARG;
			goto error_with_lock;
		}

		uint8_t mcu_addr[10];

		mcu_addr[0] = NDP120_SPI_MADDR(0);
		memcpy(&mcu_addr[1], &addr, sizeof(addr));

		/* for reads, need to restart a read command packet */
		if (in) {
			uint8_t mcu_read_tx = 0x80 | (NDP120_SPI_MDATA(0) - read_delay);
			uint32_t read_delay_data;

			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, true);
			SPI_SNDBLOCK(dev->spi, mcu_addr, 5);
			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, false);

			/* restart a new transfer for read... */

			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, true);
			SPI_SNDBLOCK(dev->spi, &mcu_read_tx, 1);
			SPI_RECVBLOCK(dev->spi, &read_delay_data, 4);
			SPI_RECVBLOCK(dev->spi, in, count);
			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, false);

		} else {
			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, true);
			SPI_SNDBLOCK(dev->spi, mcu_addr, 5);
			SPI_SNDBLOCK(dev->spi, out, count);
			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, false);
		}
	} else {

		if (0xff < addr) {
				s = SYNTIANT_NDP_ERROR_ARG;
				goto error_with_lock;
		}
		spi_cmd[0] = (in ? 0x80 : 0) | addr;

		SPI_SELECT(dev->spi, dev->lower->spi_config.cs, true);
		SPI_SNDBLOCK(dev->spi, spi_cmd, 1);

		if (in) {
				SPI_RECVBLOCK(dev->spi, in, count);
		} else {
				SPI_SNDBLOCK(dev->spi, out, count);
		}
		SPI_SELECT(dev->spi, dev->lower->spi_config.cs, false);
	}
error_with_lock:
	SPI_LOCK(dev->spi, 0);
error:
	return s;
}

static int transfer(void *d, int mcu, uint32_t addr, const void *out, void *in, unsigned int count)
{
	return tizenrt_io_spi_transfer((struct ndp120_dev_s *) d, mcu, addr, out, in, count);
}

static int mbwait(void *d)
{
	struct ndp120_dev_s *dev = (struct ndp120_dev_s *) d;
	struct timespec ts_start;
	int s = SYNTIANT_NDP_ERROR_NONE;
	uint32_t notifications;

	if (dev->ndp_interrupts_enabled) {
		struct timespec abstime;
		clock_gettime(CLOCK_REALTIME, &abstime);
		abstime.tv_sec++;
		int err = pthread_cond_timedwait(&dev->ndp_cond_mcu_mb_in,
					&dev->ndp_mutex_mcu_mb_in, &abstime);
		if (err == ETIMEDOUT) {
			auddbg("NDP mbwait timeout\n");
			s = SYNTIANT_NDP_ERROR_TIMEOUT;
		} else if (err) {
			auddbg("Error waiting on ndp_cond_mcu_mb_in\n");
			s = SYNTIANT_NDP_ERROR_FAIL;
		}
	} else {
		timer_start(&ts_start);

		do {
			if (timer_check(&ts_start, 1)) {
				s = SYNTIANT_NDP_ERROR_TIMEOUT;
				auddbg("NDP mbwait timeout (polled)\n");
				break;
			}

			s = syntiant_ndp120_poll(dev->ndp, &notifications, 1);

			if (check_status("mbwait poll", s)) {
				return s;
			}

			if (notifications & NDP_NOTIFICATION_ERRORS) {
				auddbg("NDP error reported: 0x%x", notifications);
				return SYNTIANT_NDP_ERROR_FAIL;
			}

		} while (((notifications & SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN) == 0)
				&& ((notifications
					& SYNTIANT_NDP_NOTIFICATION_MAILBOX_OUT) == 0));
	}

	return s;
}

static int get_type(void *d, unsigned int *type)
{
	struct ndp120_dev_s *ndp120 = (struct ndp120_dev_s *) d;
	int s = SYNTIANT_NDP_ERROR_NONE;
	uint8_t data;

	s = transfer(ndp120, 0, NDP120_SPI_ID0, NULL, &data, 1);

	if (check_status("get id", s)) {
		return s;
	}

	*type = data;

	return s;
}

static int iif_sync(void *d)
{
	struct ndp120_dev_s *dev  = (struct ndp120_dev_s *) d;
	int s0, s = SYNTIANT_NDP_ERROR_NONE;

	/* here, the mutex lock can return EINTR */
	do {
		s0 = pthread_mutex_lock(&dev->ndp_mutex_mbsync);
	} while (s0 == EINTR);
	if (s0) {
		auddbg("error locking ndp_mutex_mbsync s0=%d\n", s0);
		s = SYNTIANT_NDP_ERROR_FAIL;
		goto error;
	}
	do {
		s0 = pthread_mutex_lock(&dev->ndp_mutex_mcu_mb_in);
	} while (s0 == EINTR);
	if (s0) {
		auddbg("error locking ndp_mutex_mcu_mb_in s0=%d\n", s0);
		s = SYNTIANT_NDP_ERROR_FAIL;
	}

error:
	return s;
}

static int iif_unsync(void *d)
{
	struct ndp120_dev_s *dev  = (struct ndp120_dev_s *) d;
	int s0, s = SYNTIANT_NDP_ERROR_NONE;

	s0 = pthread_mutex_unlock(&dev->ndp_mutex_mcu_mb_in);
	if (s0) {
		auddbg("error unlocking ndp_mutex_mcu_mb_in s0=%d\n", s0);
		s = SYNTIANT_NDP_ERROR_FAIL;
		goto error;
	}

	s0 = pthread_mutex_unlock(&dev->ndp_mutex_mbsync);
	if (s0) {
		auddbg("error unlocking ndp_mutex_mbsync s0=%d", s0);
		s = SYNTIANT_NDP_ERROR_FAIL;
	}

error:
	return s;
}

static int iif_udelay(unsigned int n)
{
	usleep(n);
	return 0;
}

static int initialize_ndp(struct ndp120_dev_s *dev)
{
	struct syntiant_ndp_integration_interfaces_s iif;
	struct syntiant_ndp_device_s *ndp = NULL;
	syntiant_ndp120_config_clk_pll_t config_clk_pll;
	syntiant_ndp120_config_clk_src_t config_clk_src;
	int s;

	memset(dev->labels_per_network, 0, sizeof(dev->labels_per_network));
	/* stuff the ILib integration interfaces */
	iif.d = dev;
	iif.malloc = (void * (*)(int)) kmm_malloc;
	iif.free = free;
	iif.mbwait = mbwait;
	iif.get_type = get_type;
	iif.sync = iif_sync;
	iif.unsync = iif_unsync;
	iif.transfer = transfer;
	iif.udelay = iif_udelay;

	dev->ndp_interrupts_enabled = false;

	dev->lower->spi_config.freq = NDP120_SPI_FREQ_INIT;

	/*
	 * initialize the ILib with the integration interface functions and
	 * reset the NDP device
	 */
	s = syntiant_ndp_init(&dev->ndp, &iif, SYNTIANT_NDP_INIT_MODE_RESET);
	if (check_status("ndp_init", s)) {
		return s;
	}

	/*
	 * syntiant_ndp_init() returns the malloced pointer to the NDP ILib
	 * internal state for use by future syntiant_ndp*() API calls
	 */
	ndp = dev->ndp;

	/* enable xtal */
	syntiant_ndp120_config_clk_xtal_t config_clk_xtal;
	memset(&config_clk_xtal, 0, sizeof(config_clk_xtal));
	config_clk_xtal.set = SYNTIANT_NDP120_CONFIG_SET_CLK_XTAL_OUT | SYNTIANT_NDP120_CONFIG_SET_CLK_XTAL_OSC;
	config_clk_xtal.out = true;
	config_clk_xtal.osc = true;
	s = syntiant_ndp120_config_clk_xtal(ndp, &config_clk_xtal);
	if (check_status("config_clk_xtal", s)) {
		return s;
	}

	/* lock then switch to pll without touching refclk field */
	memset(&config_clk_pll, 0, sizeof(config_clk_pll));
	config_clk_pll.set = SYNTIANT_NDP120_CONFIG_SET_CLK_PLL_PRESET;
	config_clk_pll.preset = SYNTIANT_NDP120_CLOCK_MODE_PLL_1P1V_32768_98P304MHZ;
	s = syntiant_ndp120_config_clk_pll(ndp, &config_clk_pll);
	if (check_status("config_clk_pll", s)) {
		return s;
	}

	/* supply the PLL-generated clock to the NDP */
	memset(&config_clk_src, 0, sizeof(config_clk_src));
	config_clk_src.set = SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_REFSEL;
	config_clk_src.refsel = true;
	s = syntiant_ndp120_config_clk_src(ndp, &config_clk_src);
	check_status("config_clk_src", s);
	auddbg("Changing SPI speed to %dHz\n", NDP120_SPI_FREQ_HIGH);
	dev->lower->spi_config.freq = NDP120_SPI_FREQ_HIGH;

	return s;
}

static int load_synpkg(struct syntiant_ndp_device_s *ndp, const char *p)
{
	struct stat st;
	int package_len;
	void *package;
	int s = 0;
	int pfd;
	int rl;
	int chunk_size = 1024;

	/*
	 * load synpkg file
	 */
	if (stat(p, &st) < 0) {
		auddbg("unable to stat synpkg file\n");
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	package_len = st.st_size;

	package = kmm_malloc(chunk_size);
	if (!package) {
		auddbg("no memory for package_load\n");
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	pfd = open(p, O_RDONLY);
	if (pfd < 0) {
		auddbg("unable to open synpkg file\n");
		free(package);
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	auddbg("Loading %d bytes of package data\n", package_len);

	/*
	 * reset the ILib synpkg parser state
	 */
	s = syntiant_ndp_load(ndp, NULL, 0);
	if (s != SYNTIANT_NDP_ERROR_MORE) {
		auddbg("Error resetting package load state\n");
		goto errorout_with_package;
	}

	/*
	 * load the synpkg object in chunks
	 */
	int data_left = package_len;
	while (s == SYNTIANT_NDP_ERROR_MORE) {

		int load_len = chunk_size < data_left ? chunk_size : data_left;

		rl = read(pfd, package, load_len);
		if (rl <= 0) {
			s = SYNTIANT_NDP_ERROR_FAIL;
			goto errorout_with_package;
		}

		load_len = rl;

		s = syntiant_ndp_load(ndp, package, load_len);
		if (s && s != SYNTIANT_NDP_ERROR_MORE) {
			if (check_status("load", s)) {
				goto errorout_with_package;
			}
		}

		data_left -= load_len;
	}

errorout_with_package:
	close(pfd);
	free(package);
	return s;
}

/* build an array that hold all labels based on network number */
int save_labels_per_network(struct syntiant_ndp_device_s *ndp,
						char **labels, int num_labels)
{
	int i, j, len, s = SYNTIANT_NDP_ERROR_NONE;
	int class_num = 0, nn_num = 0, prev_nn_num = 0;
	char *label_string;
	struct ndp120_dev_s *dev;
	for (j = 0; j < num_labels; j++) {
		label_string = labels[j];
		len = strlen(label_string);
		i = 0;
		/*Assuming lables are in the form NN<network_num>:<class_name>*/
		while (len) {
			if (isdigit(label_string[i])) {
				nn_num = label_string[i] - '0';
				if (nn_num < 0 || nn_num >= MAX_NNETWORKS) {
					s = SYNTIANT_NDP_ERROR_INVALID_NETWORK;
					auddbg("Invalid network type label\n");
					return s;
				}
				break;
			}
			i++;
			len--;
		}
		if (nn_num != prev_nn_num) {
			class_num = 0;
		}
		auddbg("  [%d][%d] label: %s\n", nn_num, class_num, label_string);
		dev = ndp->iif.d;
		dev->labels_per_network[nn_num][class_num++] = label_string;
		prev_nn_num = nn_num;
	}
	return s;
}

static int get_versions_and_labels(struct syntiant_ndp_device_s *ndp,
						char *label_data, unsigned int label_data_length,
						char **labels, unsigned int *num_labels)
{
	char fwver[NDP120_MCU_FW_VER_MAX_LEN] = "";
	char dspfwver[NDP120_MCU_DSP_FW_VER_MAX_LEN] = "";
	char pkgver[NDP120_MCU_PKG_VER_MAX_LEN] = "";
	struct syntiant_ndp_config_s config;
	unsigned int num_labels_;
	unsigned int i;
	int s;

	memset(&config, 0, sizeof(config));
	config.firmware_version = fwver;
	config.firmware_version_len = STRING_LEN;
	config.dsp_firmware_version = dspfwver;
	config.dsp_firmware_version_len = STRING_LEN;
	config.pkg_version = pkgver;
	config.pkg_version_len = STRING_LEN;
	config.labels = label_data;
	config.labels_len = label_data_length;

	s = syntiant_ndp_get_config(ndp, &config);
	if (check_status("get_config", s)) {
		return s;
	}

	if (STRING_LEN < config.pkg_version_len) {
		auddbg("package version string too long");
		return SYNTIANT_NDP_ERROR_FAIL;
	}
	if (STRING_LEN < config.firmware_version_len) {
		auddbg("firmware version string too long");
		return SYNTIANT_NDP_ERROR_FAIL;
	}
	if (STRING_LEN < config.dsp_firmware_version_len) {
		auddbg("dsp firmware version string too long");
		return SYNTIANT_NDP_ERROR_FAIL;
	}
	if (STRING_LEN < config.labels_len) {
		auddbg("labels strings too long");
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	/* get pointers to the labels */
	num_labels_ = 0;
	i = 0;

	/*
	 * labels_len is 4 byte aligned.  Continue processing labels until the
	 * running sum of label characters processed is within 3 bytes of
	 * labels_len
	 */
	while ((config.labels_len - i > 3) && (num_labels_ < MAX_LABELS)) {
		labels[num_labels_] = &label_data[i];
		num_labels_++;
		for (; label_data[i]; i++);
		i++;
	}
	/* print status information obtained from package */
	auddbg("chip: %s\n", config.device_type);
	auddbg("package version: %s\n", pkgver);
	auddbg("firmware version: %s\n", fwver);
	auddbg("dsp firmware version: %s\n", dspfwver);
	auddbg("labels: \n");

	save_labels_per_network(ndp, labels, num_labels_);
	*num_labels = num_labels_;
	return s;
}

static int configure_audio(struct ndp120_dev_s *dev, unsigned int pdm_in_shift)
{
	const unsigned int PDM_MAX_OUT_SHIFT = 7; /* always set to max */
	struct syntiant_ndp120_config_decimation_s decimation_config;
	struct syntiant_ndp120_config_pdm_s pdm_config;
	struct syntiant_ndp120_config_misc_s misc_config;
	ndp120_dsp_config_t dsp_config = {0};
	int s;

	/* configure PDM in shift for both mics */
	/* mic 0 */
	memset(&decimation_config, 0, sizeof(decimation_config));
	decimation_config.set =
		SYNTIANT_NDP120_CONFIG_SET_DECIMATION_INSHIFT
		| SYNTIANT_NDP120_CONFIG_SET_DECIMATION_OUTSHIFT;
	decimation_config.mic = 0;
	decimation_config.inshift = pdm_in_shift;
	decimation_config.outshift = PDM_MAX_OUT_SHIFT;
	s = syntiant_ndp120_config_decimation(dev->ndp, &decimation_config);
	if (check_status("config decimation", s)) {
		goto errout_configure_audio;
	}

	/* mic1 */
	decimation_config.mic = 1;
	s = syntiant_ndp120_config_decimation(dev->ndp, &decimation_config);
	check_status("syntiant_ndp120_config_decimation", s);


	/* Configure gain */
	syntiant_ndp120_config_gain_t config_gain;
	memset(&config_gain, 0, sizeof(config_gain));
	config_gain.mic = 0;
	config_gain.dcremovalmode = SYNTIANT_NDP120_CONFIG_VALUE_GAIN_DCREMOVALMODE_ON;
	config_gain.agcshiftdir = SYNTIANT_NDP120_CONFIG_VALUE_GAIN_AGCSHIFTDIR_LEFT;
	config_gain.agcshiftcnt = 0;
	config_gain.agcfinegrainmul = 0x2000;
	config_gain.set = SYNTIANT_NDP120_CONFIG_SET_GAIN_DCREMOVALMODE
					| SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTDIR
					| SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTCNT
					| SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCFINEGRAINMUL;
	s = syntiant_ndp120_config_gain(dev->ndp, &config_gain);

	/* mic 1 */
	config_gain.mic = 1;
	s = syntiant_ndp120_config_gain(dev->ndp, &config_gain);
	check_status("syntiant_ndp120_config_gain", s);

 	/* Note: this test code always sets up for internal clock and then adds intermediate test apis to either;
		1) switch to external clock - for use when buffer is in use (also allows for switching back again)
		or
		2) switch to using PCLK1 input as passthrough (also allows for switching back again)

		The assumption in this code is that at NDP initialization time, the buffer on i2s bclk is set to disabled (OE=0) so NDP can take control of the PDM clock

		To switch the clock in the two different cases:
		1) with "buffer approach":  Generally, this needs to be done in "break before make" fashion
		   A) Switching from internal -> external
		  	  I) Switch NDP to external using ndp120_test_internal_external_switch(dev, 0) API, this will stop the PDM clock from NDP without a hard reconfig of the rest of the PDM interface
			  II) Set buffer OE=1 to allow i2s clk to DMIC

			B) Switching from external -> internal, assuming buffer is on = set to NOT tri-state output
			  I) Set buffer to OE=0, to enable the tri-state of i2s clk to DMIC
			  II) Switch NDP to external using ndp120_test_internal_external_switch(dev, 1) API, this will start the PDM clock from NDP

		2) with "PDM passthrough" approach:
		   A) Switching from internal -> external:
		      Call ndp120_test_internal_passthrough_switch(dev, 0) API
		   A) Switching from external -> internal:
		      Call ndp120_test_internal_passthrough_switch(dev, 1) API

     */

	/* Configure mic sensitivity */
	syntiant_ndp120_config_mic_t mic_config;
	memset(&mic_config, 0, sizeof(mic_config));
	mic_config.mic = 0;
	mic_config.sensitivity = -37;
	mic_config.delay = 0;
	mic_config.pad = 0;
	mic_config.get = 0;
	mic_config.set = SYNTIANT_NDP120_CONFIG_MIC_SETTINGS;
	s = syntiant_ndp120_config_mic(dev->ndp, &mic_config);
	if (check_status("config mic0 sensitivity", s)) {
		goto errout_configure_audio;
	}

	mic_config.mic = 1;
	s = syntiant_ndp120_config_mic(dev->ndp, &mic_config);
	if (check_status("config mic1 sensitivity", s)) {
		goto errout_configure_audio;
	}

	/* don't enable AUD1 when using BT MIC */
#if BT_MIC_SUPPORT == 0
	do_ndp120_i2s_setup(dev->ndp);
#endif

	/* enable the PDM clock (for the default, pdm0 aka 'left' mic) */
	memset(&pdm_config, 0, sizeof(pdm_config));
	pdm_config.interface = 0;
	pdm_config.clk = SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_ON;
	pdm_config.sample_rate = 16000;
#ifdef CONFIG_NDP120_AEC_SUPPORT
	pdm_config.clk_mode = SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_MODE_INTERNAL;
#else
	pdm_config.clk_mode = SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_MODE_EXTERNAL;
#endif
	pdm_config.mode = SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_STEREO;
	pdm_config.pdm_rate = PDM_CLOCK_PDM_RATE;

	pdm_config.set = SYNTIANT_NDP120_CONFIG_SET_PDM_CLK
					| SYNTIANT_NDP120_CONFIG_SET_PDM_MODE
					| SYNTIANT_NDP120_CONFIG_SET_PDM_CLK_MODE
					| SYNTIANT_NDP120_CONFIG_SET_PDM_PDM_RATE
					| SYNTIANT_NDP120_CONFIG_SET_PDM_SAMPLE_RATE;

	s = syntiant_ndp120_config_pdm(dev->ndp, &pdm_config);
	if (check_status("config pdm clock on", s)) {
		goto errout_configure_audio;
	}


	/*
	 * get the current audio frame size which is governed by the audio
	 * & neural algorithms loaded into the NDP
	 */
	memset(&misc_config, 0, sizeof(misc_config));
	s = syntiant_ndp120_config_misc(dev->ndp, &misc_config);
	if (check_status("config_misc", s)) {
		goto errout_configure_audio;
	}

	dev->sample_size_orig_annot = misc_config.audio_sample_size_bytes;

	s = syntiant_ndp120_read_sample_config(dev->ndp, &dsp_config);
	if (s) {
		goto errout_configure_audio;
	}

	dev->sample_size = dsp_config.aud_samp_size_bytes;

	auddbg("Sample size: %d   annotated: %d\n", dev->sample_size, dev->sample_size_orig_annot);

	int channels_active;
	s = syntiant_ndp120_get_active_configurations(dev->ndp, &channels_active);
	if (check_status("syntiant_ndp120_get_active_configurations", s)) {
		goto errout_configure_audio;
	}
	s = syntiant_ndp120_get_extract_sample_size(dev->ndp, NDP120_SPI_FREQ_HIGH, dev->sample_size, channels_active, &dev->extract_size);
	if (check_status("syntiant_ndp120_get_extract_sample_size", s)) {
		goto errout_configure_audio;
	}

	auddbg("Channels active: 0x%x  extract_size = %d\n", channels_active, dev->extract_size);

errout_configure_audio:
	return s;

}

#ifdef CONFIG_DEBUG_AUDIO_INFO
void print_flow_rule(ndp120_dsp_data_flow_rule_t *flow, int type, size_t len)
{
	int i;
	for(i = 0; i < len; i++) {
		if (flow->dst_type) {
			auddbg("src_type:%s%d dst_type:%s{%d} algo_config_id:%d\n",
				NDP120_DSP_DATA_FLOW_SRC_TYPE_STR(type),
				flow->src_param,
				NDP120_DSP_DATA_FLOW_RULE_DST_STR(*flow),
				flow->dst_param,
				flow->algo_config_index);
		}
		flow++;
	}
}
void dsp_flow_show(struct syntiant_ndp_device_s *ndp)
{
	for(uint32_t v = 0; v < 1; v++) {
		ndp120_dsp_data_flow_setup_t filtered_flow_setup = {0};
		syntiant_ndp120_dsp_flow_setup_get_rules(ndp, v, &filtered_flow_setup);
		auddbg("\n flowset ID: %d \n", v);
		auddbg("----> PCM RULES\n");
		print_flow_rule(filtered_flow_setup.src_pcm_audio, NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO,
			ARRAY_SIZE(filtered_flow_setup.src_pcm_audio));
		auddbg("----> FUNC RULES\n");
		print_flow_rule(filtered_flow_setup.src_function, NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION,
			ARRAY_SIZE(filtered_flow_setup.src_function));
		auddbg("----> NN RULES\n");
		print_flow_rule(filtered_flow_setup.src_nn, NDP120_DSP_DATA_FLOW_SRC_TYPE_NN,
			ARRAY_SIZE(filtered_flow_setup.src_nn));
		auddbg("----> MCU RULES\n");
		print_flow_rule(filtered_flow_setup.src_mcu, NDP120_DSP_DATA_FLOW_SRC_TYPE_MCU,
			ARRAY_SIZE(filtered_flow_setup.src_mcu));
		auddbg("----> SENSOR RULES\n");
		print_flow_rule(filtered_flow_setup.src_sensor, NDP120_DSP_DATA_FLOW_SRC_TYPE_MCU,
			ARRAY_SIZE(filtered_flow_setup.src_sensor));
	}
	auddbg("\n======================\n");
}

#endif	/* CONFIG_DEBUG_AUDIO_INFO */

static
void do_ndp120_i2s_setup(struct syntiant_ndp_device_s *ndp)
{
    int s = 0;
    syntiant_ndp120_config_i2s_t ndp120_config_i2s, *config;

    config = &ndp120_config_i2s;

    auddbg("Configure I2S\n");
    memset(config, 0, sizeof(*config));

    config->set = SYNTIANT_NDP120_CONFIG_SET_I2S_MODE |
                  SYNTIANT_NDP120_CONFIG_SET_I2S_FRAMESIZE |
                  SYNTIANT_NDP120_CONFIG_SET_I2S_SAMPLESIZE |
                  SYNTIANT_NDP120_CONFIG_SET_I2S_MSB_INDEX |
                  SYNTIANT_NDP120_CONFIG_SET_I2S_PACKED |
                  SYNTIANT_NDP120_CONFIG_SET_I2S_DELAYED_FLOP_SENSITIVITY |
                  SYNTIANT_NDP120_CONFIG_SET_I2S_LEFTCHENABLE |
                  SYNTIANT_NDP120_CONFIG_SET_I2S_RIGHTCHENABLE;

    config->freq = 48000;
    config->mode = SYNTIANT_NDP120_CONFIG_VALUE_I2S_MODE_STANDARD;
    config->framesize = 32;
    config->samplesize = 16;
    config->msb_index = 14;
    config->delayed_flop_sensitivity = SYNTIANT_NDP120_CONFIG_VALUE_I2S_DELAYED_FLOP_SENSITIVITY_DELAYED;
    config->packed = 0;
    config->rightchenable = 0;
    config->leftchenable = 1;
    config->interface = NDP120_DSP_AUDIO_CHAN_AUD1;

    s = syntiant_ndp120_config_i2s(ndp, config);
    check_status("syntiant_ndp120_config_i2s", s);
};

static int
do_audio_sync(struct syntiant_ndp_device_s *ndp, int ref_chan, int adj_chan, int chan_delay_ticks)
{
    int s = 0;
    ndp120_dsp_audio_sync_config_t cfg;

    memset(&cfg, 0, sizeof(cfg));

    cfg.mode = NDP120_DSP_AUDIO_SYNC_MODE_ALIGN;
    cfg.ref_chan = (ndp120_dsp_audio_chan_t) ref_chan;
    cfg.adj_chan = (ndp120_dsp_audio_chan_t) adj_chan;
    cfg.chan_delay_ticks = chan_delay_ticks;

    s = syntiant_ndp120_write_audio_sync_config(ndp, &cfg);
    if (s) {
        auddbg("error in config for sync: %d\n", s);
    }
    return s;
}

dsp_flow_e get_dsp_flow_type(struct ndp120_dev_s *dev, int index)
{
	char* label;
	int count = 0;

	if (index >= MAX_NNETWORKS)
		return DSP_FLOW_MAX;

	while ((label = dev->labels_per_network[index][count++]) != NULL) {
		label = strchr(label, ':');
		if (label == NULL) continue;

		label += 1;
		int flow_type_num = sizeof(g_flow_types)/sizeof(dsp_flow_t);
		for (int i = 0; i < flow_type_num; i++) {
			char *str = g_flow_types[i].label;
			if (strncmp(label, str, strlen(str)) == 0)
				return g_flow_types[i].flow;
		}
	}

	return DSP_FLOW_MAX;
}

void add_common_flow(ndp120_dsp_data_flow_setup_t *setup, int *src_pcm, int *src_func, int* src_nn)
{
	// ----------
	// COMBINED NORMAL + AEC FLOW
	/* PCM7->FUNCx */
	setup->src_pcm_audio[*src_pcm].src_param = NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_STEREO;
#ifdef CONFIG_NDP120_AEC_SUPPORT
	setup->src_pcm_audio[*src_pcm].src_param |= NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD1_LEFT;
#endif
	setup->src_pcm_audio[*src_pcm].dst_param = FF_ID;
	setup->src_pcm_audio[*src_pcm].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_FUNCTION;
	setup->src_pcm_audio[*src_pcm].algo_config_index = 0;
	setup->src_pcm_audio[*src_pcm].set_id = COMBINED_FLOW_SET_ID;
	setup->src_pcm_audio[*src_pcm].algo_exec_property = 0;
	(*src_pcm)++;
}
void add_host_ext_flow(ndp120_dsp_data_flow_setup_t *setup, int *src_pcm, int *src_func, int* src_nn)
{
	/* FUNCx->HOST_EXT_AUDIO */
	setup->src_function[*src_func].src_param = FF_ID;
	setup->src_function[*src_func].dst_param = NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO;
	setup->src_function[*src_func].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_HOST_EXTRACT;
	setup->src_function[*src_func].algo_config_index = 0;
	setup->src_function[*src_func].set_id = COMBINED_FLOW_SET_ID;
	setup->src_function[*src_func].algo_exec_property = 0;
	(*src_func)++;
}

void add_host_ext_flow_factory(ndp120_dsp_data_flow_setup_t *setup, int *src_pcm, int *src_func, int* src_nn, int dsp_flow_num)
{
	/* PCMx->HOST_EXT_AUDIO */
	if (dsp_flow_num == 1) {
		setup->src_pcm_audio[*src_pcm].src_param = NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_LEFT;
	} else {
		setup->src_pcm_audio[*src_pcm].src_param = NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_RIGHT;
	}
	setup->src_pcm_audio[*src_pcm].dst_param = NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO;
	setup->src_pcm_audio[*src_pcm].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_HOST_EXTRACT;
	setup->src_pcm_audio[*src_pcm].algo_config_index = 0;
	setup->src_pcm_audio[*src_pcm].set_id = COMBINED_FLOW_SET_ID;
	setup->src_pcm_audio[*src_pcm].algo_exec_property = 0;
	(*src_pcm)++;
	auddbg("Added factory flow, dsp_flow_num = %d\n", dsp_flow_num);
}

void add_bixby_flow(ndp120_dsp_data_flow_setup_t *setup, int *src_pcm, int *src_func, int* src_nn, uint32_t network_id)
{
	/* FUNCx->NN */
	setup->src_function[*src_func].src_param = FF_ID;
	setup->src_function[*src_func].dst_param = network_id;
	setup->src_function[*src_func].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_NN;
	setup->src_function[*src_func].algo_config_index = -1;
	setup->src_function[*src_func].set_id = COMBINED_FLOW_SET_ID;
	setup->src_function[*src_func].algo_exec_property = 0;
	(*src_func)++;

	/* NN->MCU */
	setup->src_nn[*src_nn].src_param = network_id;
	setup->src_nn[*src_nn].dst_param = 0;
	setup->src_nn[*src_nn].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_MCU;
	setup->src_nn[*src_nn].algo_config_index = -1;
	setup->src_nn[*src_nn].set_id = COMBINED_FLOW_SET_ID;
	setup->src_nn[*src_nn].algo_exec_property = 0;
	(*src_nn)++;
	auddbg("Added Bixby/Hi-Bixby flow\n");
}

void add_custom_flow1(ndp120_dsp_data_flow_setup_t *setup, int *src_pcm, int *src_func, int* src_nn, uint32_t network_id)
{
	/* FUNCx->FUNC227 */
	setup->src_function[*src_func].src_param = FF_ID;
	setup->src_function[*src_func].dst_param = SR_FE_POOLING_ID;
	setup->src_function[*src_func].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_FUNCTION;
	setup->src_function[*src_func].algo_config_index = network_id;
	setup->src_function[*src_func].set_id = 0;
	setup->src_function[*src_func].algo_exec_property = 0;
	(*src_func)++;

	/* FUNC227->NN */
	setup->src_function[*src_func].src_param = SR_FE_POOLING_ID;
	setup->src_function[*src_func].dst_param =  network_id;
	setup->src_function[*src_func].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_NN;
	setup->src_function[*src_func].algo_config_index = -1;
	setup->src_function[*src_func].set_id = COMBINED_FLOW_SET_ID;
	setup->src_function[*src_func].algo_exec_property = 0;
	(*src_func)++;

	/* NN->MCU */
	setup->src_nn[*src_nn].src_param = network_id;
	setup->src_nn[*src_nn].dst_param = 0;
	setup->src_nn[*src_nn].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_MCU;
	setup->src_nn[*src_nn].algo_config_index = -1;
	setup->src_nn[*src_nn].set_id = COMBINED_FLOW_SET_ID;
	setup->src_nn[*src_nn].algo_exec_property = 0;
	(*src_nn)++;
	auddbg("Added custom flow1\n");
}

static
void add_dsp_flow_rules(struct syntiant_ndp_device_s *ndp)
{
	int s = 0;
	ndp120_dsp_data_flow_setup_t setup;
	struct ndp120_dev_s *dev = ndp->iif.d;

	int src_pcm = 0;
	int src_func = 0;
	int src_nn = 0;

	memset(&setup, 0, sizeof(setup));

	add_common_flow(&setup, &src_pcm, &src_func, &src_nn);
	// TODO Even if the order of dsp flow is changed, it will be modified in sdk 1.68 so that dsp is normally applied
	if (!dev->dsp_flow_num) {
		add_host_ext_flow(&setup, &src_pcm, &src_func, &src_nn);
	} else {
		add_host_ext_flow_factory(&setup, &src_pcm, &src_func, &src_nn, dev->dsp_flow_num);
	}

	dsp_flow_e flow;
	for (int i = 0;	i < MAX_NNETWORKS; i++) {
		flow = get_dsp_flow_type(dev, i);

		switch(flow){
		case DSP_FLOW_BIXBY:
			add_bixby_flow(&setup, &src_pcm, &src_func, &src_nn, i);
			break;
		case DSP_CUSTOM_FLOW1:
			attach_algo_config_area(dev->ndp, SR_FE_POOLING_ID, i);
			auddbg("Attached ALGO id = %d at index %d.\n", SR_FE_POOLING_ID, i);
			add_custom_flow1(&setup, &src_pcm, &src_func, &src_nn, i);
			break;
		case DSP_FLOW_MAX:
			auddbg("Wrong value : %d\n", flow);
			break;
		}
		if (flow == DSP_FLOW_MAX) {
			break;
		}
		idToFlow[i] = flow;
	}
	
	auddbg("Applied flow rules\n");
	s = syntiant_ndp120_dsp_flow_setup_apply(ndp, &setup);
	check_status("syntiant_ndp120_dsp_flow_setup_apply", s);
}

#if BT_MIC_SUPPORT == 1
static
void add_dsp_flow_rules_btmic(struct syntiant_ndp_device_s *ndp)
{
	int s = 0;
	ndp120_dsp_data_flow_setup_t setup;

	int src_pcm = 0;
	int src_func = 0;
	int src_nn = 0;

	memset(&setup, 0, sizeof(setup));

	// ----------
	/* PCM1->FUNC0 */
	setup.src_pcm_audio[src_pcm].src_param = NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_LEFT;
	setup.src_pcm_audio[src_pcm].dst_param = 0;
	setup.src_pcm_audio[src_pcm].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_FUNCTION;
	setup.src_pcm_audio[src_pcm].algo_config_index = 0;
	setup.src_pcm_audio[src_pcm].set_id = COMBINED_FLOW_SET_ID;
	setup.src_pcm_audio[src_pcm].algo_exec_property = 0;
	src_pcm++;

	/* FUNC0->NN0 */
	setup.src_function[src_func].src_param = 0;
	setup.src_function[src_func].dst_param = KEYWORD_NETWORK_ID;
	setup.src_function[src_func].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_NN;
	setup.src_function[src_func].algo_config_index = -1;
	setup.src_function[src_func].set_id = COMBINED_FLOW_SET_ID;
	setup.src_function[src_func].algo_exec_property = 0;
	src_func++;

	/* PCM1->HOST_EXT_AUDIO */
	setup.src_pcm_audio[src_pcm].src_param = NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_LEFT;
	setup.src_pcm_audio[src_pcm].dst_param = NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO;
	setup.src_pcm_audio[src_pcm].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_HOST_EXTRACT;
	setup.src_pcm_audio[src_pcm].algo_config_index = 0;
	setup.src_pcm_audio[src_pcm].set_id = COMBINED_FLOW_SET_ID;
	setup.src_pcm_audio[src_pcm].algo_exec_property = 0;
	src_pcm++;

	/* NN0->MCU */
	setup.src_nn[src_nn].src_param = 0;
	setup.src_nn[src_nn].dst_param = 0;
	setup.src_nn[src_nn].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_MCU;
	setup.src_nn[src_nn].algo_config_index = -1;
	setup.src_nn[src_nn].set_id = COMBINED_FLOW_SET_ID;
	setup.src_nn[src_nn].algo_exec_property = 0;
	src_nn++;

	auddbg("Applied flow rules\n");
	s = syntiant_ndp120_dsp_flow_setup_apply(ndp, &setup);
	check_status("syntiant_ndp120_dsp_flow_setup_apply", s);

}
#endif

static
void attach_algo_config_area(struct syntiant_ndp_device_s *ndp, int32_t algo_id, int32_t algo_config_index)
{
	int s = SYNTIANT_NDP_ERROR_NONE;
	ndp120_dsp_algo_t algos[NDP120_DSP_ALGO_MAX_COUNT];

	memset(algos, 0, sizeof(algos));

	/* read algo bindings */
	s = syntiant_ndp120_read_write_algo_bindings(ndp, algos,
												 sizeof(algos), 1);
	if (s == SYNTIANT_NDP_ERROR_NONE) {
		for (uint8_t algo = 0; algo < NDP120_DSP_ALGO_MAX_COUNT; algo++) {
			if (algos[algo].algo_id == algo_id) {
				/* write algo bindings for the given algo id */
				algos[algo].algo_config_index = algo_config_index;
				s = syntiant_ndp120_read_write_algo_bindings(ndp,
						algos, sizeof(algos), 0);
				if (s != SYNTIANT_NDP_ERROR_NONE) {
					auddbg("failed to write algo bindings\n");
				}
				check_status("write algo_bindings", s);
				auddbg("algo_id: %d, algo_config_index: %d, init status: %d\n",
					algos[algo].algo_id,
					algos[algo].algo_config_index,
					algos[algo].algo_init_status);
			}
		}
	}
	else {
		auddbg("failed to read algo bindings\n");
	}
}


/****************************************************************************
 * Public Functions
 ****************************************************************************/
/* debug function, useful for doing debugging via shell */
struct ndp120_dev_s * ndp120_get_debug_handle(void)
{
	return _ndp_debug_handle;
}

void check_mb(struct ndp120_dev_s *dev)
{
	uint32_t resp = 0;

	int s;
	s = syntiant_ndp120_do_mailbox_req(dev->ndp, NDP120_DSP_MB_H2D_REQUEST_NOP, &resp);
	auddbg("NDP120_DSP_MB_H2D_REQUEST_NOP => %d / 0x%x\n", s, resp);

	resp = 0;
	s = syntiant_ndp120_do_mailbox_req(dev->ndp, NDP_MBIN_REQUEST_NOP, &resp);
	auddbg("NDP_MBIN_REQUEST_NOP => %d / 0x%x\n", s, resp);
}

/* debug function, useful for doing debugging via shell */
void ndp120_show_debug(int do_check_mb)
{
	int i, s;
	ndp120_dsp_counters_t dsp_cnts;

	if (!_ndp_debug_handle) {
		return;
	}

	struct ndp120_dev_s * ndp_debug_handle = _ndp_debug_handle;
	struct syntiant_ndp_device_s *ndp = ndp_debug_handle->ndp;
    
	auddbg("FW Loaded: %d\n", ndp_debug_handle->fw_loaded);
	if (!ndp) {
		return;
	}

	auddbg("ilib: %s\n", SYNTIANT_NDP_ILIB_RELEASE_HASH);

	uint8_t ndp_spi_reg;
	int spi_regs_to_read[] = {0,1,2,3,4,5,0x10,0x11,0x12};
	auddbg("SPI regs\n");
	for (i = 0; i < sizeof(spi_regs_to_read)/sizeof(spi_regs_to_read[0]); i++) {
		syntiant_ndp120_read(ndp, 0, spi_regs_to_read[i], &ndp_spi_reg);
		printf("0x%02X = 0x%02X\n", spi_regs_to_read[i], ndp_spi_reg);
	}

	s = syntiant_ndp120_get_dsp_counters(ndp, &dsp_cnts);
	if (s) {
		auddbg("ERROR Getting DSP counters: %d\n", s);
	}
	auddbg("frame_cnt: %d\n", dsp_cnts.frame_cnt);
	auddbg("dnn_int_cnt: %d\n", dsp_cnts.dnn_int_cnt);
	auddbg("dnn_err_cnt: %d\n", dsp_cnts.dnn_err_cnt);
	auddbg("dnn_run_err_cnt: %d\n", dsp_cnts.dnn_run_err_cnt);
	auddbg("pcm_debug_cnt: %d\n", dsp_cnts.pcm_debug_cnt);
	auddbg("func_debug_cnt: %d\n", dsp_cnts.func_debug_cnt);
	auddbg("mem_alloc_err_cnt: %d\n", dsp_cnts.mem_alloc_err_cnt);

	syntiant_ndp120_config_tank_t tank_config;
	memset(&tank_config, 0, sizeof(tank_config));
	tank_config.get = 1;
	s = syntiant_ndp120_config_dsp_tank_memory(ndp, &tank_config);
	if (s) {
		auddbg("ERROR Getting Tank config: %d\n", s);
	}
	auddbg("NDP120 Tank memory type: %s\n", tank_config.sampletank_mem_type == 1? "HEAP": "DNN");
	auddbg("NDP120 Tank size (ms)  : %d\n", tank_config.sampletank_msec);

	auddbg("KD Enabled: %d\n",ndp_debug_handle->kd_enabled);
	auddbg("Sample ready count: %u\n", ndp_debug_handle->sample_ready_cnt);
	auddbg("Last Sample Ready at:\n");
	for (i = 0; i < sizeof(ndp_debug_handle->last_sample_int) / sizeof(ndp_debug_handle->last_sample_int[0]); i++) {
		printf("                      %u\n",ndp_debug_handle->last_sample_int[i]);
	}
	auddbg("Last Sample wait at:\n",ndp_debug_handle->last_sample_wait_start);
	for (i = 0; i < sizeof(ndp_debug_handle->last_sample_wait_start) / sizeof(ndp_debug_handle->last_sample_wait_start[0]); i++) {
		printf("                      %u\n",ndp_debug_handle->last_sample_wait_start[i]);
	}

	auddbg("Sample ready enable at : %u\n", ndp_debug_handle->last_sample_rdy_ena);
	auddbg("Sample ready disable at: %u\n", ndp_debug_handle->last_sample_rdy_dis);

	uint32_t bufwrptr1[NDP120_DSP_CONFIG_BUFCURWRPTR_COUNT];
	uint32_t bufwrptr2[NDP120_DSP_CONFIG_BUFCURWRPTR_COUNT];
	for (i = 0; i < 3; i++) {
		syntiant_ndp120_read(ndp, 1, NDP120_DSP_CONFIG_BUFCURWRPTR(i), &bufwrptr1[i]);
	}
	usleep(1000);

	for (i = 0; i < 3; i++) {
		syntiant_ndp120_read(ndp, 1, NDP120_DSP_CONFIG_BUFCURWRPTR(i), &bufwrptr2[i]);
	}

	for (i = 0; i < 3; i++) {
		printf("BUFCURWRPTR[%d]: 0x%X -> 0x%X\n", i, bufwrptr1[i], bufwrptr2[i]);
	}


	if (do_check_mb) {
		check_mb(_ndp_debug_handle);
	}
}

#ifdef CONFIG_NDP120_ALIVE_CHECK
static int
check_firmware_aliveness(struct ndp120_dev_s *dev, uint32_t wait_period_ms)
{
	int s;
	enum syntiant_ndp_fw_state state;

	/* calculate wait time again, multiply 3 times, then covert to us */
	s = syntiant_ndp120_check_fw(dev->ndp, &state, ((wait_period_ms * 3) * 1000));
	if (s) {
		auddbg("Error in getting the status of firmware: %d\n", s);
		/* force reload since we are unable to get the status from NDP */
		state = SYNTIANT_NDP_BOTH_FW_DEAD;
	}

	if (state != SYNTIANT_NDP_BOTH_FW_ALIVE) {
		auddbg("<device health>: %s\n",
			state == SYNTIANT_NDP_MCU_FW_ALIVE ?
				"DSP FW Dead and MCU FW Alive" :
			state == SYNTIANT_NDP_DSP_FW_ALIVE ?
				"MCU FW Dead and DSP FW Alive" :
				"MCU and DSP FW Dead");
#if SHOW_DEBUG
		ndp120_show_debug(1);
#endif
		/* At this point the device is dead, load firmware again */
		 ndp120_load_firmware(dev);
	}

	return s;
}

static void *
ndp120_app_device_health_check(void)
{
	int s;
	struct ndp120_dev_s *dev = (struct ndp120_dev_s *)_ndp_debug_handle;

	uint32_t wait_period_ms;


	while (1) {
		ndp120_semtake(dev);
		/* if thread was started prior to the FW being loaded, we can't rely on the initial value of dev->sample_size */
		wait_period_ms = dev->sample_size * 1000 /
			(SYNTIANT_NDP120_AUDIO_SAMPLE_RATE *
			SYNTIANT_NDP120_AUDIO_SAMPLES_PER_WORD);
		(void)pm_suspend(dev->pm_domain);
		/* If KD Change requested, then skip checking. Also skip checking if no FW is loaded */
		if (dev->fw_loaded) {
			s = check_firmware_aliveness(dev, wait_period_ms);
			if (s) {
				/* In the case of failure, try again after sometime */
				printf("Error: %d in check_firmware_aliveness\n", s);
			}
		}
		(void)pm_resume(dev->pm_domain);
		ndp120_semgive(dev);
		usleep(NDP_ALIVENESS_CHECK_PERIOD_US);
	}

	/* normally won't reach */
	return NULL;
}
#endif

int ndp120_init(struct ndp120_dev_s *dev)
{
	/* File names */
	int s;

	dev->keyword_correction = false;
	dev->sample_ready_cnt = 0;
	dev->last_sample_int_idx = 0;
	dev->last_sample_wait_idx = 0;

	/* save handle so we can use it from debug routine later, e.g. from other util/shell */
	_ndp_debug_handle = dev;

	dev->ndp = NULL;
	dev->fw_loaded = false;
	dev->kd_num = -1; // set invalid kd_num during initialization
	dev->dsp_flow_num = 0; // set dsp_flow_num to 0

	s = pthread_mutex_init(&dev->ndp_mutex_mbsync, NULL);
	if (s) {
		auddbg("failed to initialize mb sync mutex variable\n");
		return -SYNTIANT_NDP_ERROR_FAIL;
	}

	s = pthread_mutex_init(&dev->ndp_mutex_mcu_mb_in, NULL);
	if (s) {
		auddbg("failed to initialize mb in mutex variable\n");
		goto errout_mutex_mcu_mb_in;
	}

	s = pthread_mutex_init(&dev->ndp_mutex_notification_sample, NULL);
	if (s) {
		auddbg("failed to initialize mutex notification match\n");
		goto errout_mutex_notification_sample;
	}

	s = pthread_cond_init(&dev->ndp_cond_mcu_mb_in, NULL);
	if (s) {
		auddbg("failed to initialize ndp_cond_notification_sample\n");
		goto errout_cond_mcu_mb_in;
	}

	s = pthread_cond_init(&dev->ndp_cond_notification_sample, NULL);
	if (s) {
		auddbg("failed to initialize ndp_cond_notification_sample\n");
		goto errout_cond_notification_sample;
	}
	sem_init(&dev->reset_sem, 0, 1);

#ifdef CONFIG_NDP120_ALIVE_CHECK
	pid_t pid = kernel_thread("NDP_health_check", 100, 4096, ndp120_app_device_health_check, NULL);
	if (pid < 0) {
		auddbg("Device health check thread creation failed\n");
		s = SYNTIANT_NDP_ERROR_FAIL;
		goto errout_reset_sem;
	}
#endif
	return SYNTIANT_NDP_ERROR_NONE;

errout_reset_sem:
	sem_destroy(&dev->reset_sem);
errout_cond_notification_sample:
	pthread_cond_destroy(&dev->ndp_cond_mcu_mb_in);
errout_cond_mcu_mb_in:
	pthread_mutex_destroy(&dev->ndp_mutex_notification_sample);
errout_mutex_notification_sample:
	pthread_mutex_destroy(&dev->ndp_mutex_mcu_mb_in);
errout_mutex_mcu_mb_in:
	pthread_mutex_destroy(&dev->ndp_mutex_mbsync);
	return s;
}

int ndp120_load_firmware(struct ndp120_dev_s *dev)
{
	int s = SYNTIANT_NDP_ERROR_NONE;

	dev->fw_loaded = false;
	dev->lower->irq_enable(false);

	if (dev->ndp) {
		s = syntiant_ndp_uninit(dev->ndp, false, SYNTIANT_NDP_INIT_MODE_RESET);
		audvdbg("uninit : %d\n", s);
	}

	dev->lower->reset();
	
	const char *mcu_package = "/res/kernel/audio/mcu_fw";
#ifdef CONFIG_DUMP4CH_SUPPORT
	const char *dsp_package = "/res/kernel/audio/dsp_fw_4ch";
	auddbg("dsp_package is dsp_fw_4ch for dump4ch app\n");
#else
	const char *dsp_package = "/res/kernel/audio/dsp_fw";
#endif
	const char *neural_package;

	if ((dev->kd_num & AUDIO_NN_MODEL_MASK) == AUDIO_NN_MODEL_HI_BIXBY) {
		if ((dev->kd_num & AUDIO_NN_MODEL_LANG_MASK) == AUDIO_NN_MODEL_LANG_EN) {
			neural_package = "/res/kernel/audio/kd_local_en";
		} else {
			neural_package = "/res/kernel/audio/kd_local";
		}
	} else if ((dev->kd_num & AUDIO_NN_MODEL_MASK) == AUDIO_NN_MODEL_BIXBY) {
		if ((dev->kd_num & AUDIO_NN_MODEL_LANG_MASK) == AUDIO_NN_MODEL_LANG_EN) {
			neural_package = "/res/kernel/audio/kd_local2_en";
		} else {
			neural_package = "/res/kernel/audio/kd_local2";
		}
	} else {
		auddbg("Invalid ... kd_num : %d\n", dev->kd_num);
		return SYNTIANT_NDP_ERROR_ARG;
	}
#ifdef CONFIG_DUMP4CH_SUPPORT
	const unsigned int AUDIO_TANK_MS = 700;
#else
	const unsigned int AUDIO_TANK_MS =
		AUDIO_BEFORE_MATCH_MS  /* max word length + ~500 MS preroll */
		+ 300  /* posterior latency of <= 24 MS/frame * 12 frames == 288 MS */
		+ 100; /* generous allowance for RTL8730E match-to-extract time */
#endif

	// looks like there is some confusion around in-shift.
	// inshift=5 should be used with BT-mic, not with AFE running
	const unsigned int DMIC_1536KHZ_PDM_IN_SHIFT_FF = 5;

	/* initialize NDP */
	s = initialize_ndp(dev);
	if (s) {
		auddbg("initialize_ndp failed\n");
		goto errout_ndp120_load_firmware;
	}

	/*
	 * load M0, Xtensa and Syntiant Core 2 firmware neural network
	 * perform audio & neural agorithm and associated NDP configuration
	 */
	s = load_synpkg(dev->ndp, mcu_package);
	if (s) {
		auddbg("mcu package load failed\n");
		goto errout_ndp120_load_firmware;
	}

	s = load_synpkg(dev->ndp, dsp_package);
	if (s) {
		auddbg("dsp package load failed\n");
		goto errout_ndp120_load_firmware;
	}

	s = load_synpkg(dev->ndp, neural_package);
	if (s) {
		auddbg("neural package load failed\n");
		goto errout_ndp120_load_firmware;
	}

	s_num_labels = 16;
	s = get_versions_and_labels(dev->ndp, s_label_data, sizeof(s_label_data), s_labels, &s_num_labels);

	attach_algo_config_area(dev->ndp, FF_ID, 0);
	auddbg("Attached ALGO id = %d at index 0.\n", FF_ID);
#if BT_MIC_SUPPORT == 1
	// when using BT-mic, attach algo config to func0 as well
	attach_algo_config_area(dev->ndp, 0, 0);
#endif

#ifdef CONFIG_NDP120_AEC_SUPPORT
	do_audio_sync(dev->ndp, NDP120_DSP_AUDIO_CHAN_AUD1, NDP120_DSP_AUDIO_CHAN_AUD0, 0);
#endif

#if BT_MIC_SUPPORT == 1
	// add special rules for BT-mic
	add_dsp_flow_rules_btmic(dev->ndp);
#else
	add_dsp_flow_rules(dev->ndp);
#endif

	struct syntiant_ndp120_config_tank_s tank_config;
	memset(&tank_config, 0, sizeof(tank_config));
	tank_config.set = SYNTIANT_NDP120_CONFIG_SET_TANK_SAMPLETANK_MSEC;
	tank_config.sampletank_msec = AUDIO_TANK_MS;
	s = syntiant_ndp120_config_dsp_tank_memory(dev->ndp, &tank_config);

	int flowset_id = COMBINED_FLOW_SET_ID;
	s = syntiant_ndp120_dsp_flow_get_put_set_id(dev->ndp, &flowset_id);
	if (s) {
		auddbg("Error enabling flow id %d: %d\n", flowset_id, s);
		goto errout_ndp120_load_firmware;
	}

	s = configure_audio(dev, DMIC_1536KHZ_PDM_IN_SHIFT_FF);
	if (s) {
		auddbg("audio configure failed\n");
		goto errout_ndp120_load_firmware;
	}

	dev->keyword_bytes = round_down(KEYWORD_BUFFER_LEN, dev->sample_size);

	if (dev->keyword_buffer == NULL) {
		dev->keyword_buffer = (uint8_t *)kmm_malloc(dev->keyword_bytes);
	}

	if (dev->keyword_buffer == NULL) {
		auddbg("keyword buffer allocation failed\n");
		s = SYNTIANT_NDP_ERROR_NOMEM;
		return s;
	}

#ifdef CONFIG_NDP120_AEC_SUPPORT
	s = syntiant_ndp120_config_barge_in(dev->ndp, BARGE_IN_INIT);
	if (s) {
		auddbg("error enabling barge-in: %s\n", s);
		goto errout_ndp120_load_firmware;
	}
#endif

	s_num_labels = 16;
	s = get_versions_and_labels(dev->ndp, s_label_data, sizeof(s_label_data), s_labels, &s_num_labels);
#ifdef CONFIG_DEBUG_AUDIO_INFO
	dsp_flow_show(dev->ndp);
#endif
	ndp120_kd_stop_match_process(dev);

	if (!dev->mute) {
		/* turn on interrupts only if mic is unmute */
		ndp120_kd_start(dev);
	} else {
		ndp120_kd_stop(dev);
	}

#ifdef CONFIG_NDP120_AEC_SUPPORT
	g_ndp120_state = NOT_RECORDING;
	dev->extclk_inuse = false;
	ndp120_aec_enable(dev);
#endif

errout_ndp120_load_firmware:
	if (s) {
		/* For now do nothing, there may be some cases where init might
		 * have failed due to no memory, so retry after some time */
		auddbg("firmware load failed! s : %d\n", s);
	} else {
		dev->lower->irq_enable(true);
	}

	/* Regardless result, set true to run aliveness check after this */
	dev->fw_loaded = true;
	dev->keyword_correction = false;
	dev->sample_ready_cnt = 0;
	return s;
}

static inline int ndp120_poll_for_sample_ready(struct ndp120_dev_s *dev)
{
	uint32_t notifications;
	syntiant_ms_time start_time;
	syntiant_get_ms_time(&start_time);
	do {
		syntiant_ndp120_poll(dev->ndp, &notifications, 1);
		if (syntiant_get_ms_elapsed(&start_time) > 500) {
			audvdbg("Sample ready interrupt didn't arrive in time\n");
			return SYNTIANT_NDP_ERROR_TIMEOUT;
		}
	} while (!(notifications & SYNTIANT_NDP_NOTIFICATION_EXTRACT_READY));

	return SYNTIANT_NDP_ERROR_NONE;
}

static void extract_keyword(struct ndp120_dev_s *dev)
{
	int s;
	unsigned int extract_bytes;
	int extracted = 0;
	int total_len = dev->keyword_bytes;

	/* when the tank resides in DNN memory there is a risk of contention when extracting larger portions of data
	 * hence use a special approach */
	dev->ndp_interrupts_enabled = false;
	ndp120_set_sample_ready_int(dev, 1);

	extract_bytes = total_len;
	s = syntiant_ndp_extract_data(dev->ndp, SYNTIANT_NDP_EXTRACT_TYPE_INPUT,
				SYNTIANT_NDP_EXTRACT_FROM_MATCH, NULL,
				&extract_bytes);

	int retry = 0;

	while (extracted < total_len) {
		do {
			extract_bytes = dev->extract_size;
			if (extracted + extract_bytes > total_len) {
				extract_bytes = dev->keyword_bytes - extracted;
			}
			if (ndp120_poll_for_sample_ready(dev) == SYNTIANT_NDP_ERROR_NONE) {
				s = syntiant_ndp_extract_data(dev->ndp, SYNTIANT_NDP_EXTRACT_TYPE_INPUT,
					SYNTIANT_NDP_EXTRACT_FROM_UNREAD, &dev->keyword_buffer[extracted],
					&extract_bytes);
				retry = 0;
			} else {
				retry++;
				if (retry > 5) {
					return;
				}
			}
		} while (s == SYNTIANT_NDP_ERROR_DATA_REREAD);
		extracted += extract_bytes;
	}
	dev->keyword_bytes_left = dev->keyword_bytes;
	ndp120_set_sample_ready_int(dev, 0);
	dev->ndp_interrupts_enabled = true;
}

static void ndp120_signal_sample(struct ndp120_dev_s *dev)
{
	int err_no = pthread_mutex_lock(&dev->ndp_mutex_notification_sample);
	if (err_no) {
	  auddbg("Error obtaining lock: %d\n", err_no);
	  return;
	}

	syntiant_get_ms_time(&dev->last_sample_int[dev->last_sample_int_idx]);
	dev->last_sample_int_idx = (dev->last_sample_int_idx + 1) % (sizeof(dev->last_sample_int) / sizeof(dev->last_sample_int[0]));

	err_no = pthread_cond_signal(&dev->ndp_cond_notification_sample);
	if (err_no) {
	  	auddbg("Error signaling: %d\n", err_no);
		/* NB: fall through to unlock */
	}

	err_no = pthread_mutex_unlock(&dev->ndp_mutex_notification_sample);
	if (err_no) {
	  auddbg("Error unlock: %d\n", err_no);
	}
}

static void ndp120_signal_mb(struct ndp120_dev_s *dev)
{
	int err_no = pthread_mutex_lock(&dev->ndp_mutex_mcu_mb_in);
	if (err_no) {
		auddbg("Error obtaining lock: %d\n", err_no);
		return;
	}

	err_no = pthread_cond_signal(&dev->ndp_cond_mcu_mb_in);
	if (err_no) {
	  	auddbg("Error signaling: %d\n", err_no);
		/* NB: fall through to unlock */
	}
	err_no = pthread_mutex_unlock(&dev->ndp_mutex_mcu_mb_in);
	if (err_no) {
		auddbg("Error unlock: %d\n", err_no);
	}
}

int ndp120_irq_handler_work(struct ndp120_dev_s *dev)
{

	/* Note that currently this is only used to detect keyword match,
	 * but later will be expanded to AI related notifications */
	struct syntiant_ndp_device_s *ndp = dev->ndp;
	uint32_t notifications = 0; /*initialize this as this is checked if io wait fails and might return wrong results if it holds garbage value*/
	uint32_t summary;
	unsigned int winner;
	int s, serialno = 0;
	uint32_t network_id;
	int ret = SYNTIANT_NDP_ERROR_NONE;

	if (!ndp) {
		goto errout_with_irq;
	}

	s = syntiant_ndp120_poll(ndp, &notifications, 1);
	if (check_status("poll", s)) {
		ret = s;
		goto errout_with_irq;
	}

	/* SPI Read Failure */
	if (notifications & SYNTIANT_NDP_NOTIFICATION_SPI_READ_FAILURE) {
		auddbg("spi read failed\n");
		ndp120_signal_mb(dev);
	}

	if (notifications & SYNTIANT_NDP_NOTIFICATION_ERROR) {
		auddbg("SYNTIANT_NDP_NOTIFICATION_ERROR\n");
		if (notifications & SYNTIANT_NDP_NOTIFICATION_DSP_MEM_ERROR) {
			auddbg("(NO DSP MEM ERROR)\n");
		}

		if (notifications & SYNTIANT_NDP_NOTIFICATION_DNN_MEM_ERROR) {
			auddbg("(NO DNN MEM ERROR)\n");
		}

		if (notifications & SYNTIANT_NDP_NOTIFICATION_INVALID_ARGS) {
			auddbg("(INVALID ARGS)\n");
		}

		if (notifications & SYNTIANT_NDP_NOTIFICATION_DELAY_ERROR) {
			auddbg("(DELAY ERROR)\n");
		}
	}

	if (notifications & SYNTIANT_NDP_NOTIFICATION_EXTRACT_READY) {
		dev->sample_ready_cnt++;
		/* signal that we have data to extract */
		ndp120_signal_sample(dev);
	}

	if (notifications & (SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN | SYNTIANT_NDP_NOTIFICATION_MAILBOX_OUT)) {
		ndp120_signal_mb(dev);
	}

	if ((notifications & SYNTIANT_NDP_NOTIFICATION_MATCH) && (dev->dev.process_mq != NULL)) {
		s = syntiant_ndp_get_match_summary(ndp, &summary);
		if (check_status("get_match_summary", s)) {
			ret = s;
			goto errout_with_irq;
		}

		if (!(summary & NDP120_SPI_MATCH_MATCH_MASK)) {
			auddbg("NDP120_SPI_MATCH_MATCH_MASK - summary=0x%x\n", summary);
			goto errout_with_irq;
		}
		network_id = ndp->d.ndp120.last_network_id;
		winner = summary & NDP120_SPI_MATCH_WINNER_MASK;
		if (s_num_labels <= winner) {
			auddbg("winner %u exceeds label count %u\n", winner, s_num_labels - 1);
			ret = SYNTIANT_NDP_ERROR_FAIL;
			goto errout_with_irq;
		}
		auddbg("#################### winner : %d summary : %d network_id : %d kd enabled : %d\n", winner, summary, network_id, dev->kd_enabled);
		if (dev->kd_enabled) {
			/* TODO Local command also need to be handled here */
			struct audio_msg_s msg;
			msg.u.pPtr = NULL;
			msg.msgId = AUDIO_MSG_NONE;
			switch (idToFlow[network_id]) {
			case DSP_FLOW_BIXBY:
				serialno++;
				auddbg("[#%d Hi-Bixby] matched: %s\n", serialno, dev->labels_per_network[network_id][winner]);
				if (!dev->recording) {
#ifndef CONFIG_DUMP4CH_SUPPORT
					/* extract keyword immediately */
					extract_keyword(dev);
#endif
#ifdef CONFIG_NDP120_AEC_SUPPORT
					g_ndp120_state = IS_RECORDING;
#endif
					dev->keyword_correction = true;
					msg.msgId = AUDIO_MSG_KD;
				}
				break;
			case DSP_CUSTOM_FLOW1:
				auddbg("[#%d Voice Commands] matched: %s\n", serialno, dev->labels_per_network[network_id][winner]);
				extract_keyword(dev);
				switch (winner) {
				case 0:
					msg.msgId = AUDIO_MSG_LOCAL0;
					break;
				case 1:
					msg.msgId = AUDIO_MSG_LOCAL1;
					break;
				case 2:
					msg.msgId = AUDIO_MSG_LOCAL2;
					break;
				case 3:
					msg.msgId = AUDIO_MSG_LOCAL3;
					break;
				default:
					ret = SYNTIANT_NDP_ERROR_INVALID_NETWORK;
					goto errout_with_irq;
				}
				break;
			default:
				ret = SYNTIANT_NDP_ERROR_INVALID_NETWORK;
				goto errout_with_irq;
			}
			audvdbg("msgId = %d\n", msg.msgId);
			if (msg.msgId != AUDIO_MSG_NONE) {
				mq_send(dev->dev.process_mq, (FAR const char *)&msg, sizeof(msg), 100);
			}
		}
	} else if (dev->dev.process_mq == NULL) {
		/* consume the summary */
		(void)syntiant_ndp_get_match_summary(ndp, &summary);
	}

errout_with_irq:
	/* re-enable interrupts as we have finished the interrupt related work */
	dev->lower->irq_enable(true);
	return ret;
}

int ndp120_irq_handler(struct ndp120_dev_s *dev)
{
	/* add work to the work queue, dont enable interrupts until we handled this one compeltely */
	return work_queue(HPWORK, &ndp120_work, ndp120_irq_handler_work, (void *)dev, 0);
}

int ndp120_set_sample_ready_int(struct ndp120_dev_s *dev, int on)
{
	int s;
	int retry_count = 3;
	do {
		retry_count--;
		s = syntiant_ndp120_config_notify_on_sample_ready(dev->ndp, on);
		auddbg("sample ready state (%d), ret (%d)\n", on, s);
		if (retry_count == 0) {
			break;
		}
	} while (s != SYNTIANT_NDP_ERROR_NONE);
	return s;
}

/**
 * Adds milliseconds to a timespec structure (ignores negative values for ms)
 */
static void timespec_add_ms(struct timespec *ts, long ms) {
    ts->tv_sec += ms / 1000;
    ts->tv_nsec += (ms % 1000) * 1000000;

    if (ts->tv_nsec >= 1000000000) {
        ts->tv_sec += 1;
        ts->tv_nsec -= 1000000000;
    }
}

int ndp120_extract_audio(struct ndp120_dev_s *dev, struct ap_buffer_s *apb)
{
	int s;

	if (!dev->fw_loaded) {
		apb->nbytes = 0;
		return SYNTIANT_NDP_ERROR_UNINIT;
	}

	if (apb->nmaxbytes < dev->sample_size) {
		/* currently we always try to flush of sample size at each call to this function
		 * TODO : apply buffering in future if possible
		 */
		return SYNTIANT_NDP_ERROR_NOMEM;
	}

#ifdef CONFIG_DUMP4CH_SUPPORT
	memset(apb->samp, 0,dev->sample_size);
	apb->nbytes = dev->sample_size;
	return 0;
#endif

	/* buffer size (apb size) for ndp120 is set equal to the sample size.
	 * Hence, all the calls to extract_audio will have total bytes as sample size
	 * So, we can safely copy the extracted data without checking the limits on the
	 * apb size
	 */
	uint32_t sample_size = dev->sample_size;
	s = SYNTIANT_NDP_ERROR_NONE;
	if (dev->keyword_bytes_left != 0) {
		memcpy(apb->samp, &dev->keyword_buffer[dev->keyword_bytes - dev->keyword_bytes_left], sample_size);
		dev->keyword_bytes_left -= sample_size;
		apb->nbytes = dev->sample_size;
		return SYNTIANT_NDP_ERROR_NONE;
	}

	/* wait for sample interrupt for a given time */

	int err = pthread_mutex_lock(&dev->ndp_mutex_notification_sample);
	if (err) {
		auddbg("NDP sample mutex lock err: %d\n", err);
		return SYNTIANT_NDP_ERROR_FAIL;
	}
	/* Set timeout value to 2 sample sizes. Generally the time values are in milli seconds */
	/* 16KHz, 1 channel, 2bytes = 16000 * 1 * 2 = 32000 bytes per second, 32 bytes per millisecond, (sample size is 640 or 768) */
	uint32_t timeout_in_msec = (2 * dev->sample_size * 1000) / (SYNTIANT_NDP120_AUDIO_SAMPLE_RATE * SYNTIANT_NDP120_AUDIO_SAMPLES_PER_WORD);

	int retries = 3;
	int timedwait_err = 0;
	do {
		struct timespec abstime;
		clock_gettime(CLOCK_REALTIME, &abstime);
		timespec_add_ms(&abstime, timeout_in_msec);
		syntiant_get_ms_time(&dev->last_sample_wait_start[dev->last_sample_wait_idx]);
		dev->last_sample_wait_idx = (dev->last_sample_wait_idx + 1) % (sizeof(dev->last_sample_wait_start) / sizeof(dev->last_sample_wait_start[0]));
		timedwait_err = pthread_cond_timedwait(&dev->ndp_cond_notification_sample,
													&dev->ndp_mutex_notification_sample, &abstime);
		if (timedwait_err) {
			syntiant_ms_time tnow;
			syntiant_get_ms_time(&tnow);
			auddbg("NDP cond timed wait err: %d (at %u) Retries = %d\n", timedwait_err, tnow, retries);
			/* Fall through to unlock the mutex */
			// show debug info twice so we can see if things are moving
			ndp120_show_debug(1);
			usleep(20);
			ndp120_show_debug(1);
		}
	} while(--retries && timedwait_err);

	err = pthread_mutex_unlock(&dev->ndp_mutex_notification_sample);
	if (err) {
		auddbg("NDP sample mutex unlock err: %d\n", err);
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	if (timedwait_err) {
		syntiant_ms_time tnow;
		syntiant_get_ms_time(&tnow);
		auddbg("NDP sample wait err: %d (at %u)\n", timedwait_err, tnow);
#if SHOW_DEBUG
		ndp120_show_debug(1);
		usleep(25 * 1000);
		ndp120_show_debug(0);
#endif
		/* Underrun need to be handled here, however, as of now we are doing
		 * data extraction in sync. So, send back the apb with zero bytes
		 * TODO : apply async method of extraction, then handle the underrun
		 * case appropriately as we can add this buffer to pendq and use it
		 */
		apb->nbytes = 0;
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	/* since apb can hold 4 samples, set extraction size to 4 */
	uint32_t extract_size = apb->nmaxbytes;
	s = syntiant_ndp_extract_data(dev->ndp,
		SYNTIANT_NDP_EXTRACT_TYPE_INPUT,
		SYNTIANT_NDP_EXTRACT_FROM_UNREAD, apb->samp, &extract_size);

	/* To prevent overflow */
	if (s) {
		auddbg("error occured : %d\n", s);
		extract_size = 0;
	} else if (extract_size > apb->nmaxbytes) {
		auddbg("error, extract_size : %d\n", extract_size);
		extract_size = 0;
	}

	apb->nbytes = extract_size;
	dev->total_size += extract_size;

	return s;
}

int ndp120_kd_start(struct ndp120_dev_s *dev)
{
	int s = SYNTIANT_NDP_ERROR_NONE;

	/* Enable interrupts from ndp120 side */
	int on = SYNTIANT_NDP_INTERRUPT_DEFAULT;
	s = syntiant_ndp_interrupts(dev->ndp, &on);
	dev->ndp_interrupts_enabled = true;
	return s;
}

int ndp120_kd_stop(struct ndp120_dev_s *dev)
{
	int s = SYNTIANT_NDP_ERROR_NONE;

	/* Disable interrupts from ndp120 side */
	int off = 0;
	s = syntiant_ndp_interrupts(dev->ndp, &off);
	dev->ndp_interrupts_enabled = false;
	return s;
}

int ndp120_change_kd(struct ndp120_dev_s *dev, uint8_t kd_num)
{
	int s = SYNTIANT_NDP_ERROR_NONE;
	if (kd_num == dev->kd_num) {
		auddbg("Same kd, ignore kd_num : %d dev->kd_num : %d\n", kd_num, dev->kd_num);
		return SYNTIANT_NDP_ERROR_NONE;
	}
	auddbg("Change kd!! model : %d\n", kd_num);
	ndp120_semtake(dev);
	/* Set false to prevent aliveness check during changing kd */
	int temp = dev->kd_num;
	dev->kd_num = kd_num;
	
	int retry = NDP120_INIT_RETRY_COUNT;
	while (retry--) {
		s = ndp120_load_firmware(dev);
		if (!s) {
			break;
		}
	}

	if (s) {
		dev->kd_num = temp;
		auddbg("ndp120_load_firmware failed! s : %d, kd_num is restored to previous value(%d)\n", s, temp);
	}

	ndp120_semgive(dev);
	return s;
}

int ndp120_kd_stop_match_process(struct ndp120_dev_s *dev) {
	int s = SYNTIANT_NDP_ERROR_NONE;
	/* nothing here, we check priv->kd_enabled in the match callprocessing instead */
	/* alternative solution could be to configure the posterior to ignore
	 * matches, in which case we could let this function configure the PH action for the keyword to be "stay" */
	return s;
}

int ndp120_kd_start_match_process(struct ndp120_dev_s *dev) {
#ifdef CONFIG_NDP120_AEC_SUPPORT
	g_ndp120_state = NOT_RECORDING;
#endif
	int s = SYNTIANT_NDP_ERROR_NONE;
	/* nothing here, we check priv->kd_enabled in the match callprocessing instead */
	/* alternative solution could be to configure the posterior to ignore
	 * matches, in which case we could let this function configure the PH action for the keyword to be "match" */
	return s;
}

int ndp120_start_sample_ready(struct ndp120_dev_s *dev)
{
	int s;

#ifdef CONFIG_NDP120_AEC_SUPPORT
	g_ndp120_state = IS_RECORDING;
#endif

	dev->recording = true;
	syntiant_get_ms_time(&dev->last_sample_rdy_ena);

	s =  ndp120_set_sample_ready_int(dev, 1);
	if (s) {
		return s;
	}

	uint32_t bytes_before_match = 0;

	if (!dev->running) {
		if (dev->keyword_correction) {
			/* we need not do this if this is resume case, we only need to do it if its recorder start case after keyword detection */
			s = syntiant_ndp_extract_data(dev->ndp, SYNTIANT_NDP_EXTRACT_TYPE_INPUT,
																SYNTIANT_NDP_EXTRACT_FROM_MATCH, NULL,
																&bytes_before_match);
			dev->keyword_correction = false;
		}
	}

	return s;
}

int ndp120_stop_sample_ready(struct ndp120_dev_s *dev)
{
	int s;

	syntiant_get_ms_time(&dev->last_sample_rdy_dis);
	s = ndp120_set_sample_ready_int(dev, 0);

	dev->recording = false;
	dev->sample_ready_cnt = 0;
#ifdef CONFIG_NDP120_AEC_SUPPORT
	g_ndp120_state = NOT_RECORDING;
#endif
	return s;
}

#ifdef CONFIG_NDP120_AEC_SUPPORT
void ndp120_test_internal_external_switch(struct ndp120_dev_s *dev, int internal)
{
    uint32_t audctrl = 0;
    int intf = 0;

    syntiant_ndp120_read(dev->ndp, 1, NDP120_CHIP_CONFIG_AUDCTRL(intf), &audctrl);
    if (internal) {
		/* set to internal */
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTNEEDED_MASK_INSERT(audctrl, 1);
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_OE_MASK_INSERT(audctrl, 1);
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(audctrl, NDP120_CHIP_CONFIG_AUDCTRL_MODE_PDM_OUT);
    } else {
		/* set to external */
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTNEEDED_MASK_INSERT(audctrl, 0);
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_OE_MASK_INSERT(audctrl, 0);
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(audctrl, NDP120_CHIP_CONFIG_AUDCTRL_MODE_PDM_IN);
    }

    syntiant_ndp120_write(dev->ndp, 1, NDP120_CHIP_CONFIG_AUDCTRL(intf), audctrl);
}


void ndp120_test_internal_passthrough_switch(struct ndp120_dev_s *dev, int internal)
{
    uint32_t audctrl = 0;
    int intf = 0;

	if (!dev->ndp) {
		auddbg("NDP not initialized\n");
		return;
	}

    syntiant_ndp120_read(dev->ndp, 1, NDP120_CHIP_CONFIG_AUDCTRL(intf), &audctrl);
    if (internal) {
		/* set to internal */
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTNEEDED_MASK_INSERT(audctrl, 1);
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_IE_MASK_INSERT(audctrl, 5);   /* pclk0, pdat */
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(audctrl, NDP120_CHIP_CONFIG_AUDCTRL_MODE_PDM_OUT);
    } else {
		/* set to pass-thru */
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTNEEDED_MASK_INSERT(audctrl, 0);
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_IE_MASK_INSERT(audctrl, 7);   /* pclk0, pclk1, pdat */
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(audctrl, NDP120_CHIP_CONFIG_AUDCTRL_MODE_PDM_THRU); /* pdm-thru */
    }

    syntiant_ndp120_write(dev->ndp, 1, NDP120_CHIP_CONFIG_AUDCTRL(intf), audctrl);

	audvdbg("Set to %s\n", internal ? "internal" : "pdm-thru");
}

void ndp120_aec_enable(struct ndp120_dev_s *dev)
{
	if (g_ndp120_state == NOT_RECORDING && !dev->extclk_inuse) {
		ndp120_test_internal_passthrough_switch(dev, 0);
		dev->extclk_inuse = true;
	}
}

void ndp120_aec_disable(struct ndp120_dev_s *dev)
{
	ndp120_test_internal_passthrough_switch(dev, 1);
	dev->extclk_inuse = false;
}
#endif

int ndp120_kw_sensitivity_set(struct ndp120_dev_s *dev, uint16_t sensitivity)
{
	int s;
	uint32_t new_th;
	syntiant_ndp120_posterior_config_t ph_config;

	if (sensitivity > 1000) {
		return SYNTIANT_NDP_ERROR_ARG;
	}
	// map the incoming threshold (0-1000) to 0-0xffff range
	new_th = sensitivity * 0xffff / 1000;

	memset(&ph_config, 0, sizeof(ph_config));
	ph_config.set = SYNTIANT_NDP120_CONFIG_SET_POSTERIOR_CONFIG_SET_THRESHOLD;
	// kw is class index 0 of model 0
	ph_config.class_index = 0;
	ph_config.ph_idx = 0;
	ph_config.threshold = new_th;
	s = syntiant_ndp120_posterior_config(dev->ndp, &ph_config);
	check_status("Error setting KW sensitivity", s);
	return s;
}

int ndp120_kw_sensitivity_get(struct ndp120_dev_s *dev, uint16_t *sensitivity)
{
	int s;
	syntiant_ndp120_posterior_config_t ph_config;

	if (!sensitivity) {
		return SYNTIANT_NDP_ERROR_ARG;
	}
	memset(&ph_config, 0, sizeof(ph_config));
	// kw is class index 0 of model 0
	ph_config.class_index = 0;
	ph_config.ph_idx = 0;
	s = syntiant_ndp120_posterior_config(dev->ndp, &ph_config);
	check_status("Error getting KW sensitivity", s);
	double d = (((double)ph_config.threshold)*1000 / 0xffff);	
	*sensitivity = (uint16_t) (d + 0.5);
	return s;
}

/*
 * If no KD model has been selected yet, use the default Hi Bixby model so
 * that the neural package is loaded before applying the requested DSP flow.
 */
int ndp120_change_dsp_flow(struct ndp120_dev_s *dev, uint8_t dsp_flow_num)
{
#if BT_MIC_SUPPORT == 1
	auddbg("To change dsp flow is not spported when BT_MIC_SUPPORT.\n");
	return -ENOTSUP;
#endif

	int s = SYNTIANT_NDP_ERROR_NONE;
	if (dsp_flow_num == dev->dsp_flow_num && dev->kd_num != -1) {
		auddbg("Same dsp_flow_num, ignore dsp_flow_num : %d dev->dsp_flow_num : %d\n", dsp_flow_num, dev->dsp_flow_num);
		return SYNTIANT_NDP_ERROR_NONE;
	}
	auddbg("Change dsp_flow_num!! flow_num : %d\n", dsp_flow_num);
	ndp120_semtake(dev);
	/* Set false to prevent aliveness check during changing kd */
	int temp_flow_num = dev->dsp_flow_num;
	dev->dsp_flow_num = dsp_flow_num;

	int temp_kd_num = dev->kd_num;
	if (dev->kd_num == -1) {
		dev->kd_num = AUDIO_NN_MODEL_HI_BIXBY;
	}
	
	int retry = NDP120_INIT_RETRY_COUNT;
	while (retry--) {
		s = ndp120_load_firmware(dev);
		if (!s) {
			break;
		}
	}

	if (s) {
		dev->dsp_flow_num = temp_flow_num;
		dev->kd_num = temp_kd_num;
		auddbg("ndp120_load_firmware failed! s : %d, dsp_flow_num and kd_num are restored to their previous values(%d, %d)\n", s, temp_flow_num, temp_kd_num);
	}

	ndp120_semgive(dev);
	return s;
}

#ifdef CONFIG_DUMP4CH_SUPPORT

static int pdm_clk_en(struct syntiant_ndp_device_s *ndp, uint32_t clk)
{
	struct syntiant_ndp120_config_pdm_s pdm_config;
	int s;

	pdm_config.get = 0;
	pdm_config.set = 0;
	pdm_config.interface = 0;
	pdm_config.sample_rate = 0;
	pdm_config.pdm_rate = 0;
	pdm_config.clk_mode = 0;
	pdm_config.mode = 0;
	pdm_config.clk = clk;

	pdm_config.set = SYNTIANT_NDP120_CONFIG_SET_PDM_CLK;
	s = syntiant_ndp120_config_pdm(ndp, &pdm_config);
	if (s) {
		auddbg("ERROR: PDM clock set (%d) failed: %d\n", clk, s);
	}
	return s;
}

int ndp120_utils_stream_init(struct ndp120_dev_s *dev, unsigned int duration, int verbose, int* dev_extract_size)
{
	int num_channels = 4;
	int s;

	auddbg("Stream.... Duration: %d.  verbose: %d\n", duration, verbose);

	auddbg("num_channels: %d    Sample size: %d   Annotated: %d dev_extract_size: %d\n", num_channels, dev->sample_size, dev->sample_size_orig_annot, dev->extract_size);
	*dev_extract_size =  dev->extract_size;

	s = pdm_clk_en(dev->ndp, 0);
	if (s) {
		auddbg("ERROR: PDM clock disable failed: %d\n", s);
	}
	s = syntiant_ndp120_init_ring_buffer_pointers(dev->ndp, 0);
	if (s) {
		auddbg("ERROR: syntiant_ndp120_init_ring_buffer_pointers failed: %d\n", s);
	}
	s = syntiant_ndp120_dsp_restart(dev->ndp);
	if (s) {
		auddbg("ERROR: syntiant_ndp120_dsp_restart failed: %d\n", s);
	}
	s = pdm_clk_en(dev->ndp, 1);
	if (s) {
		auddbg("ERROR: PDM clock enable failed: %d\n", s);
	}

    auddbg("Turning on sample ready...\n");
    s = syntiant_ndp120_config_notify_on_sample_ready(dev->ndp, 1);
	if (s) {
		auddbg("ERROR: syntiant_ndp120_config_notify_on_sample_ready failed: %d\n", s);
	}
	return s;
}

int ndp120_utils_stream_deinit(struct ndp120_dev_s *dev)
{
	auddbg("Turning off sample ready...\n");
    int s = syntiant_ndp120_config_notify_on_sample_ready(dev->ndp, 0);
	if (s) {
		auddbg("ERROR: syntiant_ndp120_config_notify_on_sample_ready failed: %d\n", s);
	}
	return s;
}

int ndp120_utils_stream_get_data(struct ndp120_dev_s *dev, uint8_t *data, uint32_t *extracted_size)
{
	static int started = 0;
	static unsigned long n_extractions = 0;
	uint32_t extract_size;
	int s;

	/* wait for sample interrupt */
	int err = pthread_mutex_lock(&dev->ndp_mutex_notification_sample);
	if (err) {
		auddbg("NDP sample mutex lock err: %d\n", err);
		goto err_out;
	}

	/* wait with timeout if already started */
	if (started) {
		struct timespec abstime;
		clock_gettime(CLOCK_REALTIME, &abstime);

		{
			// Convert to nanoseconds
			long nanoseconds = 80000000;

			// Add nanoseconds to the current nanosecond value
			abstime.tv_nsec += nanoseconds;

			// Check for overflow and adjust seconds and nanoseconds accordingly
			while (abstime.tv_nsec >= 1000000000) {
				abstime.tv_nsec -= 1000000000;
				abstime.tv_sec++;
			}
		}
		err = pthread_cond_timedwait(&dev->ndp_cond_notification_sample,
								&dev->ndp_mutex_notification_sample, &abstime);
		if (err) {
			/*
			this is not a real error, we may just have read the data in an earlier round
			syntiant_ms_time tnow;
			syntiant_get_ms_time(&tnow);
			audvdbg("Sample ready notification timeout @ %u\n", tnow);
			*/
		}
	} else {
		err = pthread_cond_wait(&dev->ndp_cond_notification_sample,
								&dev->ndp_mutex_notification_sample);
		if (err) {
			auddbg("NDP sample wait err: %d\n", err);
			goto err_out;
		}
		started = 1;
		auddbg("1st sample ready arrived\n");
	}
	err = pthread_mutex_unlock(&dev->ndp_mutex_notification_sample);
	if (err) {
		auddbg("NDP sample mutex unlock err: %d\n", err);
		goto err_out;
	}

	do {
		extract_size = dev->extract_size;
		s = syntiant_ndp_extract_data(dev->ndp,
			SYNTIANT_NDP_EXTRACT_TYPE_INPUT,
			SYNTIANT_NDP_EXTRACT_FROM_UNREAD, data, &extract_size);
	} while (s == SYNTIANT_NDP_ERROR_DATA_REREAD);
	n_extractions++;
	*extracted_size = extract_size;

err_out:
	return err;
}

#endif /* CONFIG_DUMP4CH_SUPPORT */
