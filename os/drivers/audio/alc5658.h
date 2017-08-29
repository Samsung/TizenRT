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
 * drivers/audio/alc5658.h
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

#ifndef __DRIVERS_AUDIO_ALC5658_H
#define __DRIVERS_AUDIO_ALC5658_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <pthread.h>
#include <mqueue.h>

#include <tinyara/wqueue.h>
#include <tinyara/fs/ioctl.h>

#ifdef CONFIG_AUDIO

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Registers Addresses ******************************************************/

typedef enum {
	ALC5658_RESET = 0x0000,
	ALC5658_SPO_VOL = 0x0001,
	ALC5658_HP_VOL = 0x0002,
	ALC5658_LOUT_CTRL1 = 0x0003,
	ALC5658_LOUT_CTRL2 = 0x0004,
	ALC5658_HPOUT_VLML = 0x0005,
	ALC5658_HPOUT_VLMR = 0x0006,
	ALC5658_SPDIF_CTRL1 = 0x0008,
	ALC5658_SPDIF_CTRL2 = 0x0009,
	ALC5658_SPDIF_CTRL3 = 0x0036,
	ALC5658_IN1_CTRL = 0x000C,
	ALC5658_INL_VLM = 0x000F,

	ALC5658_SIDETONE = 0x0018,

	/* DIGITAL Volume*/
	ALC5658_DAC_L1R1_VLM = 0x0019,
	ALC5658_DAC_L2R2_VLM = 0x001A,
	ALC5658_DAC_L2R2_MUTE = 0x001B,

	/* DIGITAL Mixers*/
	ALC5658_ADC_2_DAC_MXR = 0x0029,
	ALC5658_DAC_STR_MXR = 0x002A,
	ALC5658_DAC_MN_MXR = 0x002B,
	ALC5658_DAC_LB_SDTONE = 0x002C,
	ALC5658_COPY_MODE = 0x002F,

	/* Analog DAC Source */
	ALC5658_DAC_SRC = 0x002D,

	/* ANALOG Mixers */
	ALC5658_SPKMIXL = 0x0046,
	ALC5658_SPKMIXR = 0x0047,
	ALC5658_SPOMIX = 0x0048,
	ALC5658_OUTMIXL1 = 0x004D,
	ALC5658_OUTMIXL2 = 0x004E,
	ALC5658_OUTMIXR1 = 0x004F,
	ALC5658_OUTMIXR2 = 0x0050,
	ALC5658_LOUTMIX = 0x0052,

	/* Power management */
	ALC5658_PWR_MNG1 = 0x0061,
	ALC5658_PWR_MNG2 = 0x0062,
	ALC5658_PWR_MNG3 = 0x0063,
	ALC5658_PWR_MNG4 = 0x0064,
	ALC5658_PWR_MNG5 = 0x0065,
	ALC5658_PWR_MNG6 = 0x0066,
	ALC5658_PWR_MNG7 = 0x0067,

	/* DIGITAL ports comtrol */
	ALC5658_IF_DTCT = 0x006B,
	ALC5658_I2S1_CTRL = 0x0070,
	ALC5658_I2S2_CTRL = 0x0071,
	ALC5658_ADDA_CLK = 0x0073,
	ALC5658_ADDA_HPF = 0x0074,

	/* TDM */
	ALC5658_TDM_CTRL1 = 0x0077,
	ALC5658_TDM_CTRL2 = 0x0078,
	ALC5658_TDM_CTRL3 = 0x0079,
	ALC5658_TDM_CTRL4 = 0x007A,

	/* Global Clock*/
	ALC5658_GLBL_CLK = 0x0080,
	ALC5658_GLBL_PLL1 = 0x0081,
	ALC5658_GLBL_PLL2 = 0x0082,
	ALC5658_GLBL_ASRC1 = 0x0083,
	ALC5658_GLBL_ASRC2 = 0x0084,
	ALC5658_GLBL_ASRC3 = 0x0085,
	ALC5658_GLBL_ASRC4 = 0x008A,

	/* Amplifiers */
	ALC5658_HP_AMP = 0x008E,
	ALC5658_SPK_AMP = 0x00A0,

} ALC5658_REG;

typedef struct {
	uint16_t addr;
	uint16_t val;
	unsigned int delay;
} t_codec_init_script_entry;

t_codec_init_script_entry codec_reset_script[] = {
	{0x0000, 0x0000, 0},
};

t_codec_init_script_entry codec_init_script[] = {
	{0x0000, 0x0000, 0},
	{0x006E, 0xFFFF, 0},
	{0x006F, 0xFFFF, 0},
	{0x0080, 0x8000, 0},
	{0x0094, 0x0280, 0},
	{0x0111, 0xA502, 0},
	{0x0125, 0x0430, 0},
	{0x013A, 0x3020, 0},
	{0x0073, 0x1770, 0},

//{0x0070, 0x8020, 0},
	{0x0070, 0x0000, 0},
	{0x007B, 0x0003, 0},

	{0x00FA, 0x0001, 0},
	{0x0091, 0x0C16, 0},
	{0x0063, 0xA23E, 60},
	{0x0063, 0xF23E, 50},
	{0x0062, 0x0400, 50},
	{0x0061, 0x8080, 10},
	{0x0029, 0x8080, 0},
	{0x002A, 0xAAAA, 0},
	{0x002D, 0x0000, 0},
	{0x008E, 0x0009, 50},
	{0x0061, 0x8C80, 50},
	{0x0091, 0x0E16, 50},
	{0x0040, 0x0505, 0},
	{0x0065, 0x0180, 0},
	{0x013C, 0x3C05, 0},
	{0x01DF, 0x02C1, 0},
	{0x01DF, 0x2CC1, 0},
	{0x01DE, 0x5100, 0},
	{0x01E4, 0x0014, 0},
	{0x01DE, 0xD100, 30},
	{0x01DF, 0x2CC1, 0},
	{0x01DE, 0x4900, 0},
	{0x01E4, 0x0016, 0},
	{0x01DE, 0xC900, 250},
	{0x01DF, 0x2CC1, 0},
	{0x0002, 0x0000, 0},
	{0x01DE, 0x4500, 0},
	{0x01E4, 0x001F, 0},
	{0x01DE, 0xC500, 800},
	{0x0040, 0x0808, 0},
	{0x0065, 0x0000, 0},
	{0x013C, 0x2005, 0},
	{0x01E4, 0x0000, 0},
	{0x01DF, 0x20C0, 0},
	{0x0073, 0x0770, 0},
	{0x0080, 0x0000, 0},
	{0x0160, 0x8EC0, 0},
	{0x008E, 0x0019, 0},
	{0x0015, 0xC0F0, 0},
	{0x0015, 0x87F9, 0},
	{0x0094, 0x0180, 0},
	{0x00FB, 0x3000, 0},
};

/* Commonly defined and redefined macros */

#ifndef MIN
#define MIN(a,b)                   (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)                   (((a) > (b)) ? (a) : (b))
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct alc5658_dev_s {
	/* We are an audio lower half driver (We are also the upper "half" of
	 * the ALC5658 driver with respect to the board lower half driver).
	 *
	 * Terminology: Our "lower" half audio instances will be called dev for the
	 * publicly visible version and "priv" for the version that only this driver
	 * knows.  From the point of view of this driver, it is the board lower
	 * "half" that is referred to as "lower".
	 */

	struct audio_lowerhalf_s dev;	/* ALC5658 audio lower half (this device) */

	/* Our specific driver data goes here */

	FAR struct alc5658_lower_s *lower;	/* Pointer to the board lower functions */
	FAR struct i2c_dev_s *i2c;	/* I2C driver to use */
	FAR struct i2s_dev_s *i2s;	/* I2S driver to use */
	struct dq_queue_s pendq;	/* Queue of pending buffers to be sent */
	struct dq_queue_s doneq;	/* Queue of sent buffers to be returned */
	mqd_t mq;					/* Message queue for receiving messages */
	char mqname[16];			/* Our message queue name */
	pthread_t threadid;			/* ID of our thread */
	uint32_t bitrate;			/* Actual programmed bit rate */
	sem_t pendsem;				/* Protect pendq */
#ifdef ALC5658_USE_FFLOCK_INT
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
	volatile uint8_t inflight;	/* Number of audio buffers in-flight */
#ifdef ALC5658_USE_FFLOCK_INT
	volatile bool locked;		/* FLL is locked */
#endif
	bool running;				/* True: Worker thread is running */
	bool paused;				/* True: Playing is paused */
	bool mute;					/* True: Output is muted */
#ifndef CONFIG_AUDIO_EXCLUDE_STOP
	bool terminating;			/* True: Stop requested */
#endif
	bool reserved;				/* True: Device is reserved */
	volatile int result;		/* The result of the last transfer */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef CONFIG_ALC5658_CLKDEBUG
extern const uint8_t g_sysclk_scaleb1[ALC5658_BCLK_MAXDIV + 1];
extern const uint8_t g_fllratio[ALC5658_NFLLRATIO];
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: alc5658_readreg
 *
 * Description
 *    Read the specified 16-bit register from the ALC5658 device.
 *
 ****************************************************************************/

#if defined(CONFIG_ALC5658_REGDUMP) || defined(CONFIG_ALC5658_CLKDEBUG)
struct alc5658_dev_s;
uint16_t alc5658_readreg(FAR struct alc5658_dev_s *priv, uint8_t regaddr);
#endif

#endif							/* CONFIG_AUDIO */
#endif							/* __DRIVERS_AUDIO_ALC5658_H */
