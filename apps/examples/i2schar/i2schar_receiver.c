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
 * examples/i2schar/i2schar_receiver.c
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

#ifdef CONFIG_EXAMPLES_I2SCHAR_RX

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: i2schar_receiver()
 *
 * Description:
 *   This is the entry point for the receiver thread.
 *
 ****************************************************************************/
FAR struct ap_buffer_s *g_apb[CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS];

pthread_addr_t i2schar_receiver(pthread_addr_t arg)
{
	FAR struct ap_buffer_s *apb[CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS];
	struct audio_buf_desc_s desc;
	int bufsize;
	int nread;
	int nwritten;
	int ret;
	int fd;
	int i;

	/* Open the I2S character device */

	fd = open(g_i2schar.devpath, O_RDWR);
	if (fd < 0) {
		int errcode = errno;

		printf("i2schar_receiver: ERROR: failed to open %s: %d\n", g_i2schar.devpath, errcode);
		pthread_exit(NULL);
	}


	bufsize = sizeof(struct ap_buffer_s) + CONFIG_EXAMPLES_I2SCHAR_BUFSIZE;

	/* Loop for the requested number of times */

	for (i = 0; i < CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS; i++) {
		/* Allocate an audio buffer of the configured size */

		desc.numbytes = CONFIG_EXAMPLES_I2SCHAR_BUFSIZE;
		desc.u.ppBuffer = &apb[i];

		ret = apb_alloc(&desc);
		if (ret < 0) {
			printf("i2schar_receiver: ERROR: failed to allocate buffer %d: %d\n", i + 1, ret);
			close(fd);
			pthread_exit(NULL);
		}

		/* Then receifve into the buffer */

		do {
			/* Flush any output before reading */

			fflush(stdout);

			/* Read the buffer to the I2S character driver */

			nread = read(fd, apb[i], bufsize);
			if (nread < 0) {
				int errcode = errno;

				if (errcode != EINTR) {
					printf("i2schar_receiver: ERROR: read failed: %d\n", errcode);
					close(fd);
					pthread_exit(NULL);
				}
			} else if (nread != bufsize) {
				printf("i2schar_receiver: ERROR: partial read: %d\n", nread);
				close(fd);
				pthread_exit(NULL);
			} else {
				printf("i2schar_receiver: Received buffer %d\n", i + 1);
			}
		} while (nread != bufsize);

		/* Make sure that the transmitter thread has a chance to run */

		pthread_yield();
	}

	sleep(2);

	for (i = 0; i < CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS; i++) {
		do {
			/* Flush any output before writing */

			fflush(stdout);

			/* Write the buffer to the I2S character driver */
			nwritten = write(fd, apb[i], bufsize);
			if (nwritten < 0) {
				int errcode = errno;

				if (errcode != EINTR) {
					printf("i2schar_transmitter: ERROR: write failed: %d\n", errcode);
					goto exit;
				}
			} else if (nwritten != bufsize) {
				printf("i2schar_transmitter: ERROR: partial write: %d\n", nwritten);
				goto exit;
			} else {
				printf("i2schar_transmitter: Send buffer %d\n", i + 1);
			}
		} while (nwritten != bufsize);

	}

exit:

	for (i = 0; i < CONFIG_EXAMPLES_I2SCHAR_TXBUFFERS; i++) {
		apb_free(apb[i]);
	}

	close(fd);
	return NULL;
}

#endif							/* CONFIG_EXAMPLES_I2SCHAR_RX */
