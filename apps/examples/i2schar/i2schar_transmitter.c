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
 * examples/i2schar/i2schar_transmitter.c
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
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/audio/audio.h>

#include "i2schar.h"

#ifdef CONFIG_EXAMPLES_I2SCHAR_TX

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

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: i2schar_transmitter()
 *
 * Description:
 *   This is the entry point for the transmitter thread.
 *
 ****************************************************************************/

pthread_addr_t i2schar_transmitter(pthread_addr_t arg)
{
	FAR struct ap_buffer_s *apb;
	struct audio_buf_desc_s desc;
	uint8_t crap;
	uint8_t *ptr;
	int bufsize;
	int nwritten;
	int ret;
	int fd;
	int i;
	int j = 0;

	/* Open the I2S character device */

	fd = open(g_i2schar.devpath, O_WRONLY);
	if (fd < 0) {
		int errcode = errno;

		printf("i2schar_transmitter: ERROR: failed to open %s: %d\n", g_i2schar.devpath, errcode);
		pthread_exit(NULL);
	}

	/* Loop for the requested number of times */

	for (i = 0, crap = 0; i < CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS; i++) {
		/* Allocate an audio buffer of the configured size */

		desc.numbytes = CONFIG_EXAMPLES_I2SCHAR_BUFSIZE;
		desc.u.ppBuffer = &apb;

		ret = apb_alloc(&desc);
		if (ret < 0) {
			printf("i2schar_transmitter: ERROR: failed to allocate buffer %d: %d\n", i + 1, ret);
			close(fd);
			pthread_exit(NULL);
		}

		bufsize = sizeof(struct ap_buffer_s) + CONFIG_EXAMPLES_I2SCHAR_BUFSIZE;

		/* Fill the audio buffer with crap */

		j = 0;
		for (ptr = apb->samp; j < CONFIG_EXAMPLES_I2SCHAR_BUFSIZE / 2;) {
			((uint16_t *) ptr)[j] = j;
			j++;
			((uint16_t *) ptr)[j] = -j;
			j++;
		}

		apb->nbytes = CONFIG_EXAMPLES_I2SCHAR_BUFSIZE;

		/* Then send the buffer */

		do {
			/* Flush any output before writing */

			fflush(stdout);

			/* Write the buffer to the I2S character driver */

			nwritten = write(fd, apb, bufsize);
			if (nwritten < 0) {
				int errcode = errno;

				if (errcode != EINTR) {
					printf("i2schar_transmitter: ERROR: write failed: %d\n", errcode);
					close(fd);
					pthread_exit(NULL);
				}
			} else if (nwritten != bufsize) {
				printf("i2schar_transmitter: ERROR: partial write: %d\n", nwritten);
				close(fd);
				pthread_exit(NULL);
			} else {
				printf("i2schar_transmitter: Send buffer %d\n", i + 1);
				apb_free(apb);
			}
		} while (nwritten != bufsize);

		/* Make sure that the receiver thread has a chance to run */

		pthread_yield();
	}

	close(fd);
	return NULL;
}

#endif							/* CONFIG_EXAMPLES_I2SCHAR_TX */
