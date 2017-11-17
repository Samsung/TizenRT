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
 * examples/examples/sensorbd/examples/gpio_ledonoff.c
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

#include <fcntl.h>
#include <tinyara/gpio.h>

static void gpio_write(int port, int value)
{
	char str[4];
	static char devpath[16];
	snprintf(devpath, 16, "/dev/gpio%d", port);
	int fd = open(devpath, O_RDWR);
	if (fd < 0) {
		printf("fd open fail\n");
		return;
	}

	ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);
	if (write(fd, str, snprintf(str, 4, "%d", value != 0) + 1) < 0) {
		printf("write error\n");
	}

	close(fd);
}

void ledonoff_main(int argc, char *argv[])
{
	int i, j, z, num, dim[7], result[7];
	srand(time(NULL));
	for (i = 0; i < 30; i++) {
		num = rand() % 127;
		printf("%03d: ", num);

		for (j = 0; num > 0; j++) {
			dim[j] = num % 2;
			num = num / 2;
		}
		for (z = 0; z < 7; z++) {
			result[z] = 0;
		}
		for (z = j - 1; z >= 0; z--) {
			result[6 - z] = dim[z];
		}

		for (j = 0; j < 7; j++) {
			printf("%d ", result[j]);

			switch (j) {
			case 0:
				gpio_write(51, result[j]);
				break;
			case 1:
				gpio_write(52, result[j]);
				break;
			case 2:
				gpio_write(53, result[j]);
				break;
			case 3:
				gpio_write(54, result[j]);
				break;
			case 4:
				gpio_write(55, result[j]);
				break;
			case 5:
				gpio_write(39, result[j]);
				break;
			case 6:
				gpio_write(41, result[j]);
				break;
			}
		}
		printf("\n");

		up_mdelay(500);
	}
	gpio_write(51, 0);
	gpio_write(52, 0);
	gpio_write(53, 0);
	gpio_write(54, 0);
	gpio_write(55, 0);
	gpio_write(39, 0);
	gpio_write(41, 0);
}
