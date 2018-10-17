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
 * board/sidk_s5jt200/src/s5jt200_led.c
 *
 *   Copyright (C) 2009, 2011, 2013, 2015 Gregory Nutt. All rights reserved.
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

#include <debug.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <tinyara/pwm.h>
#include <tinyara/kthread.h>

#include <tinyara/board.h>
#include <arch/board/board.h>

FAR struct pwm_lowerhalf_s    *ledr;
FAR struct pwm_lowerhalf_s    *ledg;
FAR struct pwm_lowerhalf_s    *ledb;

int s5jt200_rgbled_state_task(int argc, char *argv[])
{
	char freq;
	struct pwm_info_s info;
	UNUSED(argv);
	UNUSED(argc);
	info.duty = 30;

	while (1) {
		/* pwm pulse frequency will be set from 0KHz to 50Khz */
		for (freq = 0; freq < 50; freq++) {
			info.frequency = freq * 1000;

			switch (freq % 3) {
			case 0:
				ledb->ops->start(ledr, &info);
				break;
			case 1:
				ledg->ops->start(ledg, &info);
				break;
			case 2:
				ledr->ops->start(ledb, &info);
				break;
			default:
				break;
			}

			sleep(1);
		}
	}

	return OK;
}

/*
 * PWM1 to B1 Out
 * PWM2 to G1 Out
 * PWM3 to R1 Out
 */
int s5jt200_rgbled_setup(void)
{
	static bool initialized = false;
	struct pwm_info_s info;

	/* Have we already initialized? */
	if (!initialized) {
		lldbg("RGB on!!\n");

		/* Initialize LED B */
		/* Call s5j_pwminitialize() to get an instance of the PWM interface */
		ledb = (struct pwm_lowerhalf_s*)s5j_pwminitialize(1);
		if (!ledb) {
			lldbg("ERROR: Failed to get PWM lower half to LEDR\n");
			return -ENODEV;
		}

		/* Define frequency and duty cycle */
		info.frequency = 5000;
		info.duty = 20;

		ledb->ops->setup(ledb);
		ledb->ops->start(ledb, &info);

		/* Initialize LED G */
		/* Call s5j_pwminitialize() to get an instance of the PWM interface */
		ledg = (struct pwm_lowerhalf_s*)s5j_pwminitialize(2);
		if (!ledg) {
			lldbg("ERROR: Failed to get PWM lower half to LEDG\n");
			return -ENODEV;
		}

		ledg->ops->setup(ledg);
		ledg->ops->start(ledg, &info);

		/* Initialize LED R */
		/* Call s5j_pwminitialize() to get an instance of the PWM interface */
		ledr = (struct pwm_lowerhalf_s*)s5j_pwminitialize(3);
		if (!ledr) {
			lldbg("ERROR: Failed to get PWM lower half to LEDB\n");
			return -ENODEV;
		}

		ledr->ops->setup(ledr);
		ledr->ops->start(ledr, &info);

#ifdef CONFIG_RGBLED
		/* Register the RGB LED diver at "/dev/rgbled0" */
		ret = rgbled_register("/dev/rgbled0", ledr, ledg, ledb);
		if (ret < 0) {
			lldbg("ERROR: rgbled_register failed: %d\n", ret);
			return ret;
		}
#endif

		/* Now we are initialized */
		initialized = true;
	}

	return OK;
}

int s5j_ledinitialize(void)
{
	pid_t rgbled_tid;

	/* RGB LED controlled by S8300 LED DRIVER ID with PWM */
	s5jt200_rgbled_setup();

	lldbg("Creating rgbled task..\n");
	rgbled_tid = kernel_thread("s5jt200 rgbstate", 100, 1024, (main_t)s5jt200_rgbled_state_task, (FAR char *const *)NULL);

	if (!rgbled_tid) {
		lldbg("Failed to run rgbled task..\n");
		return -ESRCH;
	}

	return OK;
}
