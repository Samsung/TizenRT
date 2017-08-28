/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * drivers/audio/alcchar.c
 *
 * This is a simple character driver for testing I2C.  It is not an audio
 * driver but does conform to some of the buffer management heuristics of an
 * audio driver.  It is not suitable for use in any real driver application
 * in its current form.
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/i2c.h>

#include <tinyara/clock.h>

#include "alcchar.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

#ifndef CONFIG_AUDIO_I2SCHAR_RXTIMEOUT
#define CONFIG_AUDIO_I2SCHAR_RXTIMEOUT 0
#endif

#ifndef CONFIG_AUDIO_I2SCHAR_TXTIMEOUT
#define CONFIG_AUDIO_I2SCHAR_TXTIMEOUT 0
#endif

#ifndef CONFIG_AUDIO_I2SCHAR_NRXBUF
#define CONFIG_AUDIO_I2SCHAR_NRXBUF 	8
#endif

#ifndef CONFIG_AUDIO_I2SCHAR_NTXBUF
#define CONFIG_AUDIO_I2SCHAR_NTXBUF 	8
#endif

#define alc_script(a, b) alc5658_exec_i2c_script(a, b, sizeof(b) / sizeof(t_codec_init_script_entry));

/* Device naming ************************************************************/
#define DEVNAME_FMT    "/dev/alc5658_char%d"
#define DEVNAME_FMTLEN (17 + 3 + 1)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct alcchar_dev_s {
	FAR struct i2s_dev_s *i2s;	/* The lower half i2s driver */
	FAR struct i2c_dev_s *i2c;	/* i2c bus access ops */
	FAR struct i2c_config_s *i2c_config;

#ifdef AUDIO_BUFFERS_REUSE
	/*If we decide to reuse bufferes use next queues*/
	struct dq_queue_s doneq;	/* Queue of sent buffers to be reused */
	sem_t donesem;				/* Protect dq */
#endif

	struct dq_queue_s rxedq;	/* Queue of received audio IN buffers */
	sem_t rxsem;				/* Protect RX Q and rx_cnt */
	sem_t cnt_rxsem;			/* Counting semaphore to block receiving thread if no data is received yet */
	int rx_cnt;					/* Count allocated RX buffers */

	sem_t alloc;				/* Counting semaphore for Alloc TX buffers, and block allocator if needed */

	sem_t exclsem;				/* Assures mutually exclusive access */

	uint32_t samprate;			/* 8000, 44100, ... */
	uint8_t bpsamp;				/* Bits per sample: 8 bits = 8, 16 bits = 16 */
	uint8_t nchannels;			/* Mono=1, Stereo=2 */

	uint8_t volume;
	int16_t balance;			/* Current balance level */
	int16_t micgain;			/* Current mic gain level */

	bool running;				/* True: Worker thread is running */
	bool paused;				/* True: Playing is paused */
	bool mute;					/* True: Output is muted */
	bool terminating;			/* True: Stop requested */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* I2S callback function */

static void alcchar_rxcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result);
static void alcchar_txcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result);

/* Character driver methods */

static ssize_t alcchar_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t alcchar_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);

static ssize_t alcchar_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations alcchar_fops = {
	NULL,						/* open  */
	NULL,						/* close */
	alcchar_read,				/* read  */
	alcchar_write,				/* write */
	NULL,						/* seek  */
	alcchar_ioctl,				/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	NULL,						/* poll  */
#endif
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
static uint16_t alc5658_readreg(FAR struct alcchar_dev_s *priv, uint16_t regaddr)
{

	int32_t ret;
	uint8_t reg[2];
	uint16_t regval;
	FAR struct i2c_dev_s *dev = priv->i2c;
	FAR struct i2c_config_s *alcchar_i2c_config = priv->i2c_config;

	reg[0] = ((char *)&regaddr)[1];
	reg[1] = ((char *)&regaddr)[0];

	ret = i2c_write(dev, alcchar_i2c_config, reg, 2);
	if (ret < 0) {
		auderr("Error, cannot read reg %x\n", regaddr);
		return 0;
	}

	ret = i2c_read(dev, alcchar_i2c_config, reg, 2);
	if (ret < 0) {
		auderr("Error, cannot read reg %x\n", regaddr);
		return 0;
	}

	regval = ((uint16_t) reg[0] << 8) | (uint16_t) reg[1];
//        audinfo("I2CRead: %04x -> %04x\n", regaddr, regval);

	return regval;
}

/************************************************************************************
 * Name: alc5658_writereg
 *
 * Description:
 *   Write the specified 16-bit register to the ALC5658 device.
 *
 ************************************************************************************/

static void alc5658_writereg(FAR struct alcchar_dev_s *priv, uint16_t regaddr, uint16_t regval)
{
	int32_t ret;
	uint8_t reg[4];
	FAR struct i2c_dev_s *dev = priv->i2c;
	FAR struct i2c_config_s *alcchar_i2c_config = priv->i2c_config;

	reg[0] = ((char *)&regaddr)[1];
	reg[1] = ((char *)&regaddr)[0];

	reg[2] = ((char *)&regval)[1];
	reg[3] = ((char *)&regval)[0];

	ret = i2c_write(dev, alcchar_i2c_config, (uint8_t *) reg, 4);
	if (ret < 0) {
		auderr("Error, cannot write reg %x\n", regaddr);
		return;
	}
//      audinfo("I2CWrite: %04x <- %04x\n", regaddr, regval);
	return;
}

static uint16_t alc5658_modifyreg(FAR struct alcchar_dev_s *priv, uint16_t regaddr, uint16_t set, uint16_t clear)
{
	uint16_t data;
//      audinfo("I2Cmodify ------\n");
	data = alc5658_readreg(priv, regaddr);
	data &= ~clear;
	data |= set;

	alc5658_writereg(priv, regaddr, data);
	return alc5658_readreg(priv, regaddr);
}

static void alc5658_exec_i2c_script(FAR struct alcchar_dev_s *priv, t_codec_init_script_entry *script, uint32_t size)
{
	uint32_t i;
	uint16_t ret;

	for (i = 0; i < size; i++) {
		ret = alc5658_modifyreg(priv, script[i].addr, script[i].val, 0xFFFF);
//        audinfo("ALC %04x := %04x\n", script[i].addr, ret);
		delay(script[i].delay);
	}
}

/****************************************************************************
 * Name: alcchar_setdatawidth
 *
 * Description:
 *   Set the 8- 16- 24- bit data modes
 *
 ****************************************************************************/

static void alcchar_setdatawidth(FAR struct alcchar_dev_s *priv)
{
	I2S_TXDATAWIDTH(priv->i2s, priv->bpsamp);
}

/****************************************************************************
 * Name: alccahr_setbitrate
 *
 * Description:
 *
 ****************************************************************************/

static void alcchar_setbitrate(FAR struct alcchar_dev_s *priv)
{
}

/************************************************************************************
 * Name: alcchar_setvolume
 *
 * Description:
 *   Set the right and left volume values in the ALC5658 device based on the current
 *   volume and balance settings.
 *
 ************************************************************************************/

static void alcchar_setvolume(FAR struct alcchar_dev_s *priv)
{
	uint16_t vol_l, vol_r;
	int16_t bal = priv->balance;

	/* ADD VOLUME CODE HERE */
	vol_l = priv->volume;
	vol_r = priv->volume;

	if (bal > 0) {
		vol_l = vol_l - vol_l * abs(bal) / 1000;
	}

	if (bal < 0) {
		vol_r = vol_r - vol_l * abs(bal) / 1000;
	}

	if (priv->mute) {
		alc5658_writereg(priv, ACL5658_HPOUT_MUTE, 0x8080);
	} else {
		alc5658_writereg(priv, ACL5658_HPOUT_MUTE, 0x0000);
	}

	alc5658_writereg(priv, ACL5658_HPOUT_VLML, vol_l << 8);
	alc5658_writereg(priv, ACL5658_HPOUT_VLMR, vol_r << 8);
	audinfo("MUTE %x, VOLL %x VOLR %x\n", (uint32_t) alc5658_readreg(priv, ACL5658_HPOUT_MUTE), (uint32_t) alc5658_readreg(priv, ACL5658_HPOUT_VLML), (uint32_t) alc5658_readreg(priv, ACL5658_HPOUT_VLMR));

}

/************************************************************************************
 * Name: alcchar_setmic
 *
 * Description:
 * 	Set Mic gain and some other related options if needed
 *
 ************************************************************************************/

static void alcchar_setmic(FAR struct alcchar_dev_s *priv)
{
	alc5658_writereg(priv, ACL5658_IN1_CTRL, (priv->micgain + 16) << 8);
	audinfo("MIC GAIN 0x%x\n", (uint32_t) alc5658_readreg(priv, ACL5658_IN1_CTRL));
}

/****************************************************************************
 * Name: alcchar_configure
 *
 * Description:
 *   Configure the audio device for the specified  mode of operation.
 *
 ****************************************************************************/

static int alcchar_configure(FAR struct alcchar_dev_s *priv, FAR const struct audio_caps_s *caps)
{
	int ret = OK;

	DEBUGASSERT(priv && caps);
	audinfo("ac_type: %d\n", caps->ac_type);

	/* Process the configure operation */

	switch (caps->ac_type) {
	case AUDIO_TYPE_FEATURE:
		audinfo("  AUDIO_TYPE_FEATURE\n");

		/* Process based on Feature Unit */

		switch (caps->ac_format.hw) {
		case AUDIO_FU_VOLUME: {
			/* Set the volume 0 ~ 1000 */
			if (caps->ac_controls.hw[0] <= 1000) {
				priv->volume = 63 - 63 * caps->ac_controls.hw[0] / 1000;
			}
			alcchar_setvolume(priv);
			audinfo("    Volume: %d\n", priv->volume);
		}
		break;
		case AUDIO_FU_MUTE: {
			/* Set the mute true/false */
			priv->mute = !!(caps->ac_controls.b[0]);
			audinfo("    Mute: %d\n", priv->mute);
			alcchar_setvolume(priv);
		}
		break;
		case AUDIO_FU_BALANCE: {
			/* Set the balamce -1000 ~ 0 ~ 1000 */
			int16_t bal = caps->ac_controls.hw[0];
			if (abs(bal) <= 1000) {
				priv->balance = bal;
			}
			audinfo("    Balance: %d\n", priv->balance);
			alcchar_setvolume(priv);
		}
		break;
		case AUDIO_FU_MICGAIN: {
			/* Set MIC gain */
			/* Values -16 ~ 0 ~ 53 equal to -12dB ~ 0dB ~ 39.75 */
			int16_t gain = caps->ac_controls.hw[0];
			if ((gain >= -16) && (gain <= 53)) {
				priv->micgain = gain;
				audinfo("    Mic Gain: %d\n", priv->micgain);
				alcchar_setmic(priv);
			} else {
				audinfo("    Mic Gain request is out of range, unchanged: %d\n", priv->micgain);
			}
		}
		break;

		default:
			auderr("    ERROR: Unrecognized feature unit\n");
			ret = -ENOTTY;
			break;
		}
		break;

	case AUDIO_TYPE_OUTPUT: {
		audinfo("  AUDIO_TYPE_OUTPUT:\n");
		audinfo("    Number of channels: %u\n", caps->ac_channels);
		audinfo("    Sample rate:        %u\n", caps->ac_controls.hw[0]);
		audinfo("    Sample width:       %u\n", caps->ac_controls.b[2]);

		/* Verify that all of the requested values are supported */

		ret = -ERANGE;
		if (caps->ac_channels != 1 && caps->ac_channels != 2) {
			auderr("ERROR: Unsupported number of channels: %d\n", caps->ac_channels);
			break;
		}

		if (caps->ac_controls.b[2] != 8 && caps->ac_controls.b[2] != 16) {
			auderr("ERROR: Unsupported bits per sample: %d\n", caps->ac_controls.b[2]);
			break;
		}

		/* Save the current stream configuration */

		priv->samprate = caps->ac_controls.hw[0];
		priv->nchannels = caps->ac_channels;
		priv->bpsamp = caps->ac_controls.b[2];

		/*
				alcchar_setdatawidth(priv);
				alcchar_setbitrate(priv);
		*/

		ret = OK;
	}
	break;

	case AUDIO_TYPE_PROCESSING:
		break;
	}

	return ret;
}

/****************************************************************************
 * Name: alcchar_configure
 *
 * Description:
 *   Configure the audio device for the specified  mode of operation.
 *
 ****************************************************************************/

static int alcchar_start(FAR struct alcchar_dev_s *priv, FAR const struct audio_caps_s *caps)
{
	int ret = OK;

	alcchar_setdatawidth(priv);

	/* Init codec here only for debugging purposes. Remove later. */
	alc5658_exec_i2c_script(priv, codec_init_inout_script1, sizeof(codec_init_inout_script1) / sizeof(t_codec_init_script_entry));

	switch (priv->samprate) {
	case AUDIO_SAMP_RATE_8K:
		alc5658_exec_i2c_script(priv, codec_init_pll_8K, sizeof(codec_init_pll_8K) / sizeof(t_codec_init_script_entry));
		break;
	case AUDIO_SAMP_RATE_11K:
		alc5658_exec_i2c_script(priv, codec_init_pll_11K, sizeof(codec_init_pll_11K) / sizeof(t_codec_init_script_entry));
		break;
	case AUDIO_SAMP_RATE_16K:
		alc5658_exec_i2c_script(priv, codec_init_pll_16K, sizeof(codec_init_pll_16K) / sizeof(t_codec_init_script_entry));
		break;
	case AUDIO_SAMP_RATE_22K:
		alc5658_exec_i2c_script(priv, codec_init_pll_22K, sizeof(codec_init_pll_22K) / sizeof(t_codec_init_script_entry));
		break;
	case AUDIO_SAMP_RATE_32K:
		alc5658_exec_i2c_script(priv, codec_init_pll_32K, sizeof(codec_init_pll_32K) / sizeof(t_codec_init_script_entry));
		break;
	case AUDIO_SAMP_RATE_44K:
		alc5658_exec_i2c_script(priv, codec_init_pll_44K, sizeof(codec_init_pll_44K) / sizeof(t_codec_init_script_entry));
		break;
	case AUDIO_SAMP_RATE_48K:
	default:
		alc5658_exec_i2c_script(priv, codec_init_pll_48K, sizeof(codec_init_pll_48K) / sizeof(t_codec_init_script_entry));

	};

	alc5658_exec_i2c_script(priv, codec_init_inout_script2, sizeof(codec_init_inout_script2) / sizeof(t_codec_init_script_entry));

	return ret;
}

static int alcchar_stop(FAR struct alcchar_dev_s *priv, FAR const struct audio_caps_s *caps)
{
	int ret = OK;
	/* Init codec here only for debugging purposes. Remove later. */
//  alc5658_exec_i2c_script(priv, codec_stop_script, sizeof(codec_stop_script) / sizeof(t_codec_init_script_entry));
	return ret;
}

/****************************************************************************
 * Name: alcchar_rxcallback
 *
 * Description:
 *   I2S RX transfer complete callback.
 *
 *   NOTE: In this test driver, the RX is simply dumped in the bit bucket.
 *   You would not do this in a real application. You would return the
 *   received data to the caller via some IPC.
 *
 *   Also, the test buffer is simply freed.  This will work if this driver
 *   has the sole reference to buffer; in that case the buffer will be freed.
 *   Otherwise -- memory leak!  A more efficient design would recyle the
 *   audio buffers.
 *
 ****************************************************************************/

static void alcchar_rxcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
	FAR struct alcchar_dev_s *priv = (FAR struct alcchar_dev_s *)arg;

	DEBUGASSERT(priv && apb);
	audinfo("apb=%p nbytes=%d result=%d crefs=%d\n", apb, apb->nbytes, result, apb->crefs);

	sem_wait(&priv->rxsem);
	/* we have some data, read ... */
	dq_addlast((FAR dq_entry_t *) apb, &priv->rxedq);
	sem_post(&priv->rxsem);

	/* Let the waiting thread to capture received data */
	sem_post(&priv->cnt_rxsem);
}

/****************************************************************************
 * Name: alcchar_txcallback
 *
 * Description:
 *   I2S TX transfer complete callback
 *
 *   NOTE: The test buffer is simply freed.  This will work if this driver
 *   has the sole reference to buffer; in that case the buffer will be freed.
 *   Otherwise -- memory leak!  A more efficient design would recyle the
 *   audio buffers.
 *
 ****************************************************************************/

static void alcchar_txcallback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
	FAR struct alcchar_dev_s *priv = (FAR struct alcchar_dev_s *)arg;

	DEBUGASSERT(priv && apb);
	audinfo("apb=%p nbytes=%d result=%d crefs=%d\n", apb, apb->nbytes, result, apb->crefs);

	/* Free buffer here ... completety, if not reused */
	apb_free(apb);

	/* Let to alloc next buffer if pull was full */
	sem_post(&priv->alloc);
}

/****************************************************************************
 * Name: alcchar_read
 *
 * Description:
 *   Standard character driver read method
 *
 ****************************************************************************/

static ssize_t alcchar_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	return 0;
}

/****************************************************************************
 * Name: alcchar_write
 *
 * Description:
 *   Standard character driver write method
 *
 ****************************************************************************/

static ssize_t alcchar_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	return 0;
}

/************************************************************************************
 * Name: alcchar_ioctl
 *
 * Description:
 *   The standard ioctl method.  This is where ALL of the Audio work is done.
 *
 ************************************************************************************/

static ssize_t alcchar_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	FAR struct alcchar_dev_s *priv;
	FAR struct audio_buf_desc_s *bufdesc;
	struct ap_buffer_s *apb;
	int ret;

	/* Get our private data structure */
	DEBUGASSERT(filep);

	inode = filep->f_inode;
	DEBUGASSERT(inode);

	priv = (FAR struct alcchar_dev_s *)inode->i_private;
	DEBUGASSERT(priv);

	audinfo("cmd: %d arg: %ld\n", cmd, arg);

	ret = 0;

	switch (cmd) {

	/* Alloc buffer for further TX operation. Will be filled by user APP. */
	case AUDIOIOC_ALLOCBUFFER: {
		audinfo("AUDIOIOC_ALLOCBUFFER, arg - %ld\n", arg);

		bufdesc = (FAR struct audio_buf_desc_s *)arg;
		/* We will be waiting here is if we reached max number of allocated TX buffers */
		sem_wait(&priv->alloc);
		ret = apb_alloc(bufdesc);
	}
	break;

	/* Free buffer we received at DEQUEUE with TXed data. */
	case AUDIOIOC_FREEBUFFER: {
		audinfo("AUDIOIOC_FREEBUFFER, arg - %ld\n", arg);

		bufdesc = (FAR struct audio_buf_desc_s *)arg;
		DEBUGASSERT(bufdesc->u.pBuffer != NULL);

		apb_free(bufdesc->u.pBuffer);

		/* decrement number of allocated rx buffers */
		DEBUGASSERT(priv->rx_cnt > 0);

		sem_wait(&priv->rxsem);
		priv->rx_cnt--;
		sem_post(&priv->rxsem);

		ret = sizeof(struct audio_buf_desc_s);
	}
	break;

	/* Put buffer into TX queue. callback will be called after buffer is transferred */
	case AUDIOIOC_ENQUEUEBUFFER: {
		audinfo("AUDIOIOC_ENQUEUEBUFFER, arg - %ld\n", arg);

		bufdesc = (FAR struct audio_buf_desc_s *)arg;
		ret = I2S_SEND(priv->i2s, bufdesc->u.pBuffer, alcchar_txcallback, priv, CONFIG_AUDIO_I2SCHAR_TXTIMEOUT);

	}
	break;

	/* Request RXed data. Before receive anything we allocate and enqueue empty RX buffer for receiving */
	case AUDIOIOC_DEQUEUEBUFFER: {
		audinfo("AUDIOIOC_DEQUEUEBUFFER, arg - %ld\n", arg);
		bufdesc = (FAR struct audio_buf_desc_s *)arg;

		/* Try to allocate needed buffers and enqueue in receive queue */
		while (priv->rx_cnt < CONFIG_AUDIO_I2SCHAR_NRXBUF) {

			if (bufdesc->numbytes == 0) {
				bufdesc->numbytes = 1024 * 16;
			}

			bufdesc->u.ppBuffer = &apb;
			ret = apb_alloc(bufdesc);

			if (ret < 0) {
				break;
			}

			ret = I2S_RECEIVE(priv->i2s, apb, alcchar_rxcallback, priv, CONFIG_AUDIO_I2SCHAR_RXTIMEOUT);

			if (ret < 0) {
				apb_free(apb);
				break;
			}
			/* Increment counter of allocated RX buffers */
			sem_wait(&priv->rxsem);
			priv->rx_cnt++;
			sem_post(&priv->rxsem);
		}

		/* Clean buffer pointer to prevent wrong accesses */
		bufdesc->u.ppBuffer = NULL;

		/* We didnt hit any error before, so try to read any data from rx queue */
		if (ret >= 0) {
			/* Will be blocked here if no data is available to read yet */
			sem_wait(&priv->cnt_rxsem);

			/* we have some data, read ... */
			sem_wait(&priv->rxsem);
			apb = (FAR struct ap_buffer_s *)dq_remfirst(&priv->rxedq);
			sem_post(&priv->rxsem);

			bufdesc->u.pBuffer = apb;
			ret = 0;
		}
		break;
	}
	break;

	case AUDIOIOC_CONFIGURE: {
		audinfo("AUDIOIOC_CONFIGURE, arg - %ld\n", arg);
		ret = alcchar_configure(priv, (FAR const struct audio_caps_s *)arg);
	}
	break;

	case AUDIOIOC_START: {
		audinfo("AUDIOIOC_START, arg - %ld\n", arg);
		ret = alcchar_start(priv, (FAR const struct audio_caps_s *)arg);
	}
	break;

	case AUDIOIOC_STOP: {
		audinfo("AUDIOIOC_STOP, arg - %ld\n", arg);
		ret = alcchar_stop(priv, (FAR const struct audio_caps_s *)arg);
	}
	break;

	default:
		audinfo("NOT SUPPORTED COMMAND %X, arg - %ld\n", cmd, arg);
	}

	return ret;

}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: alcchar_register
 *
 * Description:
 *   Create and register the I2S character driver.
 *
 *   The I2S character driver is a simple character driver that supports I2S
 *   transfers via a read() and write().  The intent of this driver is to
 *   support I2S testing.  It is not an audio driver but does conform to some
 *   of the buffer management heuristics of an audio driver.  It is not
 *   suitable for use in any real driver application in its current form.
 *
 * Input Parameters:
 *   i2s - An instance of the lower half I2S driver
 *   minor - The device minor number.  The I2S character device will be
 *     registers as /dev/alccharN where N is the minor number
 *
 * Returned Value:
 *   OK if the driver was successfully register; A negated errno value is
 *   returned on any failure.
 *
 ****************************************************************************/

int alcchar_register(FAR struct i2s_dev_s *i2s, FAR struct i2c_dev_s *i2c, FAR struct i2c_config_s *i2c_config, int minor)
{
	FAR struct alcchar_dev_s *priv;
	char devname[DEVNAME_FMTLEN];
	int ret;

	/* Sanity check */

	DEBUGASSERT(i2s != NULL && i2c != NULL && (unsigned)minor < 1000);

	/* Allocate a I2S character device structure */

	priv = (FAR struct alcchar_dev_s *)kmm_zalloc(sizeof(struct alcchar_dev_s));
	if (priv) {
		/* Initialize the I2S character device structure */

		priv->i2s = i2s;
		priv->i2c = i2c;
		priv->i2c_config = i2c_config;

		sem_init(&priv->exclsem, 0, 1);

		sem_init(&priv->alloc, 0, CONFIG_AUDIO_I2SCHAR_NTXBUF);

		dq_init(&priv->rxedq);	/* Queue of received audio IN buffers */
		sem_init(&priv->rxsem, 0, 1);	/* Protect dq */
		sem_init(&priv->cnt_rxsem, 0, 0);	/* Assume we did not receive anything yet!!! */
		priv->rx_cnt = 0;		/* Count allocated RX buffers */

		/* Create the character device name */
		snprintf(devname, DEVNAME_FMTLEN, DEVNAME_FMT, minor);
		ret = register_driver(devname, &alcchar_fops, 0666, priv);
		if (ret < 0) {
			/* Free the device structure if we failed to create the character
			 * device.
			 */

			kmm_free(priv);
		}

		/* Init codec here only once */
		alc5658_exec_i2c_script(priv, codec_init_script, sizeof(codec_init_script) / sizeof(t_codec_init_script_entry));

		/* Return the result of the registration */

		return OK;
	}

	return -ENOMEM;
}
