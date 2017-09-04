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
 *   arch/arm/src/artik053/src/artik053_alc5658.c
 *
 *   Copyright (C) 2014, 2016 Gregory Nutt. All rights reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/irq.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/audio/pcm.h>
#include <tinyara/audio/alc5658.h>

#include <arch/board/board.h>
#include "s5j_i2c.h"
#include "s5j_i2s.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifndef CONFIG_ALC5658_I2C_PORT
#define CONFIG_ALC5658_I2C_PORT 1
#endif

#define ALC5658_I2C_ADDR 0x1A
#define ALC5658_I2C_FREQ 100000
#define ALC5658_I2C_ADDRLEN 7

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct s5jt200_alcinfo_s {
	/* Standard MW8904 interface */

	struct alc5658_lower_s lower;

	/* Extensions for the s5jt200 board */

	alc5658_handler_t handler;

	FAR void *arg;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* IRQ/PIO access callbacks.  These operations all hidden behind
 * callbacks to isolate the ALC5658 driver from differences in PIO
 * interrupt handling by varying boards and MCUs.  If possible,
 * interrupts should be configured on both rising and falling edges
 * so that contact and loss-of-contact events can be detected.
 *
 *   attach  - Attach the ALC5658 interrupt handler to the PIO interrupt
 *   enable  - Enable or disable the PIO interrupt
 */

static int alc5658_attach(FAR const struct alc5658_lower_s *lower, alc5658_handler_t isr, FAR void *arg);
static bool alc5658_enable(FAR const struct alc5658_lower_s *lower, bool enable);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* A reference to a structure of this type must be passed to the ALC5658
 * driver.  This structure provides information about the configuration
 * of the ALC5658 and provides some board-specific hooks.
 *
 * Memory for this structure is provided by the caller.  It is not copied
 * by the driver and is presumed to persist while the driver is active.
 */

static struct s5jt200_alcinfo_s g_alc5658info = {
	.lower = {
		.i2c_config = {
			.frequency = ALC5658_I2C_FREQ,
			.address = ALC5658_I2C_ADDR,
			.addrlen = ALC5658_I2C_ADDRLEN,
		},

		.attach = alc5658_attach,
		.enable = alc5658_enable,
	},
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * IRQ/PIO access callbacks.  These operations all hidden behind
 * callbacks to isolate the ALC5658 driver from differences in PIO
 * interrupt handling by varying boards and MCUs.  If possible,
 * interrupts should be configured on both rising and falling edges
 * so that contact and loss-of-contact events can be detected.
 *
 *   attach  - Attach the ALC5658 interrupt handler to the PIO interrupt
 *   enable  - Enable or disable the PIO interrupt
 *   clear   - Acknowledge/clear any pending PIO interrupt
 *
 ****************************************************************************/

static int alc5658_attach(FAR const struct alc5658_lower_s *lower, alc5658_handler_t isr, FAR void *arg)
{
	if (isr) {
		/* Just save the address of the handler and its argument for now.  The
		 * new handler will called via alc5658_interrupt() when the interrupt occurs.
		 */

		audvdbg("Attaching %p\n", isr);
		g_alc5658info.handler = isr;
		g_alc5658info.arg = arg;
	} else {
		audvdbg("Detaching %p\n", g_alc5658info.handler);
		(void)alc5658_enable(lower, false);
		g_alc5658info.handler = NULL;
		g_alc5658info.arg = NULL;
	}

	return OK;
}

static bool alc5658_enable(FAR const struct alc5658_lower_s *lower, bool enable)
{
	static bool enabled;
	irqstate_t flags;
	bool ret;

	/* Has the interrupt state changed */

	flags = irqsave();
	if (enable != enabled) {
		/* Enable or disable interrupts */

		if (enable && g_alc5658info.handler) {
			audvdbg("Enabling\n");
			/* DO IRQ ENABLING HERE */
			enabled = true;
		} else {
			audvdbg("Disabling\n");
			/* DO IRQ DISABLING HERE */
			enabled = false;
		}
	}

	ret = enabled;
	irqrestore(flags);
	return ret;
}

static int alc5658_interrupt(int irq, FAR void *context)
{
	/* Just forward the interrupt to the ALC5658 driver */

	audvdbg("handler %p\n", g_alc5658info.handler);
	if (g_alc5658info.handler) {
		return g_alc5658info.handler(&g_alc5658info.lower, g_alc5658info.arg);
	}

	/* We got an interrupt with no handler.  This should not
	 * happen.
	 */

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: s5j_alc5658_initialize
 *
 * Description:
 *   This function is called by platform-specific, setup logic to configure
 *   and register the ALC5658 device.  This function will register the driver
 *   as /dev/alc5658[x] where x is determined by the minor device number.
 *
 * Input Parameters:
 *   minor - The input device minor number
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/
int s5j_alc5658_initialize(int minor)
{
	FAR struct audio_lowerhalf_s *alc5658;
	FAR struct audio_lowerhalf_s *pcm;
	FAR struct i2c_dev_s *i2c;
	FAR struct i2s_dev_s *i2s;
	static bool initialized;
	char devname[12];
	int ret;

	audvdbg("minor %d\n", minor);
	DEBUGASSERT(minor >= 0 && minor <= 25);

	/* Have we already initialized?  Since we never uninitialize we must prevent
	 * multiple initializations.  This is necessary, for example, when the
	 * touchscreen example is used as a built-in application in NSH and can be
	 * called numerous time.  It will attempt to initialize each time.
	 */

	if (!initialized) {
		/* Configure the ALC5658 interrupt pin */

		/* Do we have an interrupt PIN ? FIXME */

		/* Get an instance of the I2C interface for the ALC5658 chip select */

		i2c = up_i2cinitialize(CONFIG_ALC5658_I2C_PORT);
		if (!i2c) {
			auddbg("ERROR: Failed to initialize CODEC I2C%d\n");
			ret = -ENODEV;
			goto errout;
		}

		/* Get an instance of the I2S interface for the ALC5658 data channel */

		i2s = s5j_i2s_initialize();
		if (!i2s) {
			auddbg("ERROR: Failed to initialize I2S\n");
			ret = -ENODEV;
			goto errout_with_i2c;
		}

		/* Now we can use these I2C and I2S interfaces to initialize the
		 * ALC5658 which will return an audio interface.
		 */

		alc5658 = alc5658_initialize(i2c, i2s, &g_alc5658info.lower);
		if (!alc5658) {
			auddbg("ERROR: Failed to initialize the ALC5658\n");
			ret = -ENODEV;
			goto errout_with_irq;
		}

		/* No we can embed the ALC5658/I2C/I2S conglomerate into a PCM decoder
		 * instance so that we will have a PCM front end for the the ALC5658
		 * driver.
		 */

		pcm = pcm_decode_initialize(alc5658);
		if (!pcm) {
			auddbg("ERROR: Failed create the PCM decoder\n");
			ret = -ENODEV;
			goto errout_with_alc5658;
		}

		/* Create a device name */

		snprintf(devname, 12, "pcm%d", minor);

		/* Finally, we can register the PCM/ALC5658/I2C/I2S audio device.
		 *
		 * Is anyone young enough to remember Rube Goldberg?
		 */

		ret = audio_register(devname, pcm);
		if (ret < 0) {
			auddbg("ERROR: Failed to register /dev/%s device: %d\n", devname, ret);
			goto errout_with_pcm;
		}

		/* Now we are initialized */

		initialized = true;
	}

	return OK;

	/* Error exits.  Unfortunately there is no mechanism in place now to
	 * recover resources from most errors on initialization failures.
	 */

errout_with_pcm:
errout_with_alc5658:
errout_with_irq:
errout_with_i2c:
errout:
	return ret;
}
