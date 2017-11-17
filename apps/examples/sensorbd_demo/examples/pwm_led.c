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
 * examples/examples/sensorbd/examples/pwm_buzzer.c
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
#include <tinyara/pwm.h>

void ledpwm_main(int argc, char *argv[])
{
	int i;
	int fd1;
	int fd2;
	int fd3;
	int fd4;
	struct pwm_info_s pwm_info;

	fd1 = open("/dev/pwm0", O_RDWR);
	if (fd1 < 0) {
		printf("fd open fail\n");
		return;
	}
	fd2 = open("/dev/pwm3", O_RDWR);
	if (fd2 < 0) {
		printf("fd open fail\n");
		close(fd1);
		return;
	}
	fd3 = open("/dev/pwm4", O_RDWR);
	if (fd3 < 0) {
		printf("fd open fail\n");
		close(fd1);
		close(fd2);
		return;
	}
	fd4 = open("/dev/pwm5", O_RDWR);
	if (fd4 < 0) {
		printf("fd open fail\n");
		close(fd1);
		close(fd2);
		close(fd3);
		return;
	}
	pwm_info.frequency = 1000;

	for (i = 0; i < 100; i = i + 3) {
		printf("brightness %d, %d\n", i, 99 - i);

		pwm_info.duty = i * 65536 / 100;
		ioctl(fd1, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)&pwm_info));
		ioctl(fd2, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)&pwm_info));
		ioctl(fd1, PWMIOC_START);
		ioctl(fd2, PWMIOC_START);

		pwm_info.duty = (99 - i) * 65536 / 100;
		ioctl(fd3, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)&pwm_info));
		ioctl(fd4, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)&pwm_info));
		ioctl(fd3, PWMIOC_START);
		ioctl(fd4, PWMIOC_START);

		up_mdelay(200);
	}

	up_mdelay(2000);
	ioctl(fd1, PWMIOC_STOP);
	ioctl(fd2, PWMIOC_STOP);
	ioctl(fd3, PWMIOC_STOP);
	ioctl(fd4, PWMIOC_STOP);
	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);
}
