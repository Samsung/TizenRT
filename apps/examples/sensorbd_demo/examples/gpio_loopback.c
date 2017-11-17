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
 * examples/examples/sensorbd/examples/gpio_loopback.c
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
#include <tinyara/gpio.h>

#define XGPIO1 30
#define XGPIO2 31
#define XGPIO3 32
#define XGPIO8 37
#define XGPIO9 38
#define XGPIO11 40

static int gpio_read(int port)
{
	char buf[4];
	char devpath[16];
	snprintf(devpath, 16, "/dev/gpio%d", port);
	int fd = open(devpath, O_RDWR);
	if (fd < 0) {
		printf("fd open fail\n");
		return -1;
	}

	if (read(fd, buf, sizeof(buf)) < 0) {
		printf("read error\n");
		close(fd);
		return -1;
	}
	close(fd);

	return buf[0] == '1';
}

static void gpio_write(int port, int value)
{
	char buf[4];
	char devpath[16];
	snprintf(devpath, 16, "/dev/gpio%d", port);
	int fd = open(devpath, O_RDWR);
	if (fd < 0) {
		printf("fd open fail\n");
		return;
	}

	ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);
	if (write(fd, buf, snprintf(buf, sizeof(buf), "%d", !!value)) < 0) {
		printf("write error\n");
	}
	close(fd);
}

void gpioloopback_main(int argc, char *argv[])
{
	int tc;
	for (tc = 0; tc < 3; tc++) {
		int nA = 0, nB = 0;
		switch (tc) {
		case 0:
			nA = XGPIO9;
			nB = XGPIO11;
			break;
		case 1:
			nA = XGPIO8;
			nB = XGPIO2;
			break;
		case 2:
			nA = XGPIO1;
			nB = XGPIO3;
			break;
		}

		int readA, readB;
		readA = gpio_read(nA);

		if (readA == 0) {
			gpio_write(nA, 1);

			readA = gpio_read(nA);
			readB = gpio_read(nB);

			if (readA != readB) {
				printf("gpio%d and gpio%d is not connect!\n", nA, nB);
			} else {
				printf("gpio%d and gpio%d is connect!\n", nA, nB);
			}
		} else {
			gpio_write(nA, 0);

			readA = gpio_read(nA);
			readB = gpio_read(nB);

			if (readA != readB) {
				printf("gpio%d and gpio%d is not connect!\n", nA, nB);
			} else {
				printf("gpio%d and gpio%d is connect!\n", nA, nB);
			}
		}

		gpio_write(nA, 0);
	}
}
