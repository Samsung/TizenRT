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
/************************************************************************************
 * configs/sama5d3-xplained/src/sam_i2schar.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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
 ************************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/clock.h>

#if defined(CONFIG_AUDIO_I2SCHAR) && defined(CONFIG_S5J_I2S) && defined(CONFIG_S5J_I2C)

#include <arch/board/s5jt200_alc5658_i2c.h>
#include <tinyara/audio/i2s.h>
#include "s5j_i2s.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

#ifndef CONFIG_S5JT200_I2SCHAR_MINOR
#define CONFIG_S5JT200_I2SCHAR_MINOR 0
#endif

/************************************************************************************
 * Private Types
 ************************************************************************************/
typedef struct {
	uint16_t addr;
	uint16_t val;
	unsigned int delay;
} t_codec_init_script_entry;

/************************************************************************************
 * Private Data
 ************************************************************************************/
/*This init script enables codec audio output */
/* Uncomment when it is needed
static t_codec_init_script_entry codec_reset_script[] = {
	{0x0000, 0x0000, 0},
};
*/

/*This init script enables codec audio output */
static t_codec_init_script_entry codec_init_script[] = {
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

/************************************************************************************
 * Private Functions
 ************************************************************************************/

static void delay(unsigned int mS)
{
	volatile systime_t start = clock_systimer();

	mS = mS / MSEC_PER_TICK + 1;

	while (1)
		if ((start + mS) < clock_systimer()) {
			return;
		}
}

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: i2schar_devinit
 *
 * Description:
 *   All architectures must provide the following interface in order to work with
 *   apps/examples/i2schar.
 *
 ************************************************************************************/

int i2schar_devinit(void)
{
	static bool initialized = false;
	i2c_alc *i2c;
	struct i2s_dev_s *i2s;
	int ret, i;

	/* Have we already initialized? */

	if (!initialized) {

		/* Get I2C interface for ALC5658 set default configuration */

		i2c = alc5658_i2c_initialize();
		if (!i2c) {
			auderr("ERROR: Failed to initialize CODEC I2C%d\n");
			return -ENODEV;
		}

		for (i = 0; i < sizeof(codec_init_script) / sizeof(t_codec_init_script_entry); i++) {
			ret = i2c->modify(codec_init_script[i].addr, codec_init_script[i].val, 0xFFFF);
			audinfo("ALC %x <- %x\n", codec_init_script[i].addr, ret);
			delay(codec_init_script[i].delay);
		}

		/* Call s5j_i2s_initialize() to get an instance of the I2S interface */

		i2s = s5j_i2s_initialize();
		if (!i2s) {
			auderr("ERROR: Failed to get the S5J I2S driver\n");
			return -ENODEV;
		}

		/* Register the I2S character driver at "/dev/i2schar0" */

		ret = i2schar_register(i2s, CONFIG_S5JT200_I2SCHAR_MINOR);
		if (ret < 0) {
			auderr("ERROR: i2schar_register failed: %d\n", ret);
			return ret;
		}

		/* Now we are initialized */

		initialized = true;
	}

	return OK;
}

#endif							/* CONFIG_AUDIO_I2SCHAR && CONFIG_S5J_I2S */
