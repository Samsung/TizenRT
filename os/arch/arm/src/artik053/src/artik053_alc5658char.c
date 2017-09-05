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
/************************************************************************************
 * arch/arm/src/artik053/src/artik053_alc5658char.c
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

#if defined(CONFIG_AUDIO_ALC5658CHAR) && defined(CONFIG_S5J_I2S) && defined(CONFIG_S5J_I2C)

#include <tinyara/i2c.h>

#include "s5j_i2s.h"
#include "s5j_i2c.h"
#include <tinyara/audio/i2s.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifndef CONFIG_ALC5658_I2C_PORT
#define CONFIG_ALC5658_I2C_PORT 1
#endif

#define ALC5658_I2C_ADDR 0x1A
#define ALC5658_I2C_FREQ 100000
#define ALC5658_I2C_ADDRLEN 7

int alc5658char_register(FAR struct i2s_dev_s *i2s, FAR struct i2c_dev_s *i2c, FAR struct i2c_config_s *i2c_config, int minor);

/************************************************************************************
 * Private Types
 ************************************************************************************/

/************************************************************************************
 * Private Data
 ************************************************************************************/
static struct i2c_config_s g_i2c_config = {
	.frequency = ALC5658_I2C_FREQ,
	.address = ALC5658_I2C_ADDR,
	.addrlen = ALC5658_I2C_ADDRLEN,
};

/************************************************************************************
 * Private Functions
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: s5j_alc5658char_initialize
 *
 * Description:
 *   All architectures must provide the following interface in order to work with
 *   apps/examples/alc5658char_test
 *
 ************************************************************************************/
int s5j_alc5658char_initialize(int minor)
{
	static bool initialized;
	struct i2c_dev_s *i2c;
	struct i2s_dev_s *i2s;
	int ret;

	/* Have we already initialized? */

	if (!initialized) {

		i2c = up_i2cinitialize(CONFIG_ALC5658_I2C_PORT);
		if (!i2c) {
			auddbg("ERROR: Failed to initialize CODEC I2C%d\n");
			return -ENODEV;
		}

		/* Call s5j_i2s_initialize() to get an instance of the I2S interface */
		i2s = s5j_i2s_initialize();
		if (!i2s) {
			auddbg("ERROR: Failed to get the S5J I2S driver\n");
			return -ENODEV;
		}

		/* Register the ALC5658 character driver at "/dev/alc5658charN" */

		ret = alc5658char_register(i2s, i2c, &g_i2c_config, minor);
		if (ret < 0) {
			auddbg("ERROR: i2schar_register failed: %d\n", ret);
			return ret;
		}

		/* Now we are initialized */

		initialized = true;
	}

	return OK;
}
#endif
