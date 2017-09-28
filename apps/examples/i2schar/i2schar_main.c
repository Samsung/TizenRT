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
 * examples/i2schar/i2schar_main.c
 *
 *   Copyright (C) 2011-2012 Gregory Nutt. All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <debug.h>

#include "i2schar.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

struct i2schar_state_s g_i2schar;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: i2schar_devpath
 ****************************************************************************/

static void i2schar_devpath(FAR struct i2schar_state_s *i2schar, FAR const char *devpath)
{
	/* Get rid of any old device path */

	if (i2schar->devpath) {
		free(i2schar->devpath);
	}

	/* Then set-up the new device path by copying the string */

	i2schar->devpath = strdup(devpath);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: i2schar_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int i2schar_main(int argc, char *argv[])
#endif
{
#ifdef CONFIG_EXAMPLES_I2SCHAR_TX
	pthread_t transmitter;
#endif
#ifdef CONFIG_EXAMPLES_I2SCHAR_RX
	pthread_t receiver;
#endif
#if defined(CONFIG_EXAMPLES_I2SCHAR_RX) | defined(CONFIG_EXAMPLES_I2SCHAR_TX) | defined(CONFIG_EXAMPLES_I2SCHAR_DEVINIT)
	pthread_attr_t attr;
	pthread_addr_t result;
	int ret;
#endif
#if defined(CONFIG_EXAMPLES_I2SCHAR_RX) & defined(CONFIG_EXAMPLES_I2SCHAR_TX)
	struct sched_param param;
#endif

	/* Check if we have initialized */

	if (!g_i2schar.initialized) {
#ifdef CONFIG_EXAMPLES_I2SCHAR_DEVINIT
		/* Initialization of the I2S character device is performed by logic
		 * external to this test.
		 */

		printf("i2schar_main: Initializing external I2S character device\n");
		ret = i2schar_devinit();
		if (ret != OK) {
			printf("i2schar_main: i2schar_devinit failed: %d\n", ret);
			return EXIT_FAILURE;
		}
#endif

		/* Set the default values */

		i2schar_devpath(&g_i2schar, CONFIG_EXAMPLES_I2SCHAR_DEVPATH);

#ifdef CONFIG_EXAMPLES_I2SCHAR_TX
		g_i2schar.txcount = CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS;
#endif
#ifdef CONFIG_EXAMPLES_I2SCHAR_RX
		g_i2schar.rxcount = CONFIG_EXAMPLES_I2SCHAR_RXBUFFERS;
#endif

		g_i2schar.initialized = true;
	}

	sched_lock();
#ifdef CONFIG_EXAMPLES_I2SCHAR_RX
	/* Start the receiver thread */

	printf("i2schar_main: Start receiver thread\n");
	pthread_attr_init(&attr);
#if 1
#ifdef CONFIG_EXAMPLES_I2SCHAR_TX
	/* Bump the receiver priority from the default so that it will be above
	 * the priority of transmitter.  This is important if a loopback test is
	 * being performed; it improves the changes that a receiving audio buffer
	 * is in place for each transmission.
	 */

	(void)pthread_attr_getschedparam(&attr, &param);
	param.sched_priority++;
	(void)pthread_attr_setschedparam(&attr, &param);
#endif
#endif

	/* Set the receiver stack size */

	(void)pthread_attr_setstacksize(&attr, CONFIG_EXAMPLES_I2SCHAR_RXSTACKSIZE);

	/* Start the receiver */

	ret = pthread_create(&receiver, &attr, i2schar_receiver, NULL);
	if (ret != OK) {
		sched_unlock();
		printf("i2schar_main: ERROR: failed to Start receiver thread: %d\n", ret);
		return EXIT_FAILURE;
	}

	pthread_setname_np(receiver, "receiver");
#endif

#ifdef CONFIG_EXAMPLES_I2SCHAR_TX
	/* Start the transmitter thread */

	printf("i2schar_main: Start transmitter thread\n");
	pthread_attr_init(&attr);

	/* Set the transmitter stack size */

	(void)pthread_attr_setstacksize(&attr, CONFIG_EXAMPLES_I2SCHAR_TXSTACKSIZE);

	/* Start the transmitter */

	ret = pthread_create(&transmitter, &attr, i2schar_transmitter, NULL);
	if (ret != OK) {
		sched_unlock();
		printf("i2schar_main: ERROR: failed to Start transmitter thread: %d\n", ret);
#ifdef CONFIG_EXAMPLES_I2SCHAR_RX
		printf("i2schar_main: Waiting for the receiver thread\n");
		(void)pthread_join(receiver, &result);
#endif
		return EXIT_FAILURE;
	}

	pthread_setname_np(transmitter, "transmitter");
#endif

	sched_unlock();
#ifdef CONFIG_EXAMPLES_I2SCHAR_TX
	printf("i2schar_main: Waiting for the transmitter thread\n");
	ret = pthread_join(transmitter, &result);
	if (ret != OK) {
		printf("i2schar_main: ERROR: pthread_join failed: %d\n", ret);
	}
#endif

#ifdef CONFIG_EXAMPLES_I2SCHAR_RX
	printf("i2schar_main: Waiting for the receiver thread\n");
	ret = pthread_join(receiver, &result);
	if (ret != OK) {
		printf("i2schar_main: ERROR: pthread_join failed: %d\n", ret);
	}
#endif

	printf("END\n");

	return EXIT_SUCCESS;
}
