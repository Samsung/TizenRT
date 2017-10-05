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
 * examples/examples/sensorbd/examples/uart_loopback.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
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

#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>

void uartloopback_main(int argc, char *argv[])
{
	static char buf[16];
	int port;
	int ret = 0;

	for (port = 1; port < 5; port++) {
		snprintf(buf, 16, "/dev/ttyS%d", port);
		int fd = open(buf, O_RDWR | O_NOCTTY);
		if (fd < 0) {
			ret = 1;
		} else {
			// tx
			char buf_tx[255] = "Hello";
			if (write(fd, buf_tx, 5) < 0) {
				ret = 1;
			}

			// rx
			char buf_rx[255] = { 0, };
			struct timeval tv;
			fd_set readfds;
			FD_ZERO(&readfds);
			FD_SET(fd, &readfds);

			tv.tv_sec = 1;
			tv.tv_usec = 0;

			if (select(fd + 1, &readfds, NULL, NULL, &tv) < 0) {
				ret = 1;
			} else if (FD_ISSET(fd, &readfds)) {
				if (read(fd, buf_rx, 5) < 0) {
					ret = 1;
				}
			}

			int i;
			for (i = 0; i < 5; i++) {
				if (buf_tx[i] != buf_rx[i]) {
					ret = 1;
				}
			}

			if (ret) {
				printf("UART%d LOOPBACK FAIL!!!\n", port);
				if (fd > 0) {
					close(fd);
				}
				ret = 0;
			} else {
				printf("UART%d LOOPBACK PASS!!!\n", port);
				close(fd);
			}
		}
	}
}
