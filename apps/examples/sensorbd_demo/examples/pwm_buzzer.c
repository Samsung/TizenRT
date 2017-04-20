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

#define c3   3822				/* PWM: 130.82 Hz, note freq: 130.81 Hz, error 0.01% */
#define e3   3034				/* PWM: 164.80 Hz, note freq: 164.81 Hz, error 0.01% */
#define g3x  2408				/* PWM: 207.64 Hz, note freq: 207.65 Hz, error 0.01% */
#define a3   2273				/* PWM: 219.97 Hz, note freq: 220.00 Hz, error 0.01% */
#define b3   2025				/* PWM: 246.91 Hz, note freq: 246.94 Hz, error 0.01% */
#define c4   1911				/* PWM: 261.64 Hz, note freq: 261.63 Hz, error 0.01% */
#define d4   1703				/* PWM: 293.60 Hz, note freq: 293.66 Hz, error 0.02% */
#define d4x  1607				/* PWM: 311.14 Hz, note freq: 311.13 Hz, error 0.00% */
#define e4   1517				/* PWM: 329.60 Hz, note freq: 329.63 Hz, error 0.01% */
#define f4   1432				/* PWM: 349.16 Hz, note freq: 349.23 Hz, error 0.02% */
#define g4   1276				/* PWM: 391.85 Hz, note freq: 392.00 Hz, error 0.04% */
#define a4   1136				/* PWM: 440.14 Hz, note freq: 440.00 Hz, error 0.03% */
#define h4   1012				/* PWM: 494.07 Hz, note freq: 493.88 Hz, error 0.04% */
#define c5   956				/* PWM: 523.01 Hz, note freq: 523.25 Hz, error 0.05% */
#define p    1

int octavef[] = { c4, d4, e4, f4, g4, a4, h4, c5 };
int octaved[] = { 8, 8, 8, 8, 8, 8, 8, 8 };

int furelisef[] = {
	e4, d4x, e4, d4x, e4, b3, d4, c4, a3, p, c3, e3, a3, b3, p, e3, g3x, b3, c4, p, e3,
	e3, d4x, e4, d4x, e4, b3, d4, c4, a3, p, c3, e3, a3, b3, p, e3, c4, b3, a3
};

int furelised[] = {
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 4, 8, 8, 8, 8, 4, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 4, 8, 8, 8, 8, 4
};

void pwmbuzzer_main(int argc, char *argv[])
{
	int i, fd1, fd2;
	struct pwm_info_s pwm_info;

	fd1 = open("/dev/pwm1", O_RDWR);
	fd2 = open("/dev/pwm2", O_RDWR);

	for (i = 0; i < 8; i++) {
		pwm_info.frequency = octavef[i];
		pwm_info.duty = (octaved[i] * 65536) / 100;
		ioctl(fd1, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)&pwm_info));
		ioctl(fd1, PWMIOC_START);

		up_mdelay(400);
	}

	for (i = 0; i < 40; i++) {
		pwm_info.frequency = furelisef[i];
		pwm_info.duty = (furelised[i] * 65536) / 100;
		ioctl(fd2, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)&pwm_info));
		ioctl(fd2, PWMIOC_START);

		up_mdelay(400);
	}

	ioctl(fd1, PWMIOC_STOP);
	close(fd1);
	ioctl(fd2, PWMIOC_STOP);
	close(fd2);
}
