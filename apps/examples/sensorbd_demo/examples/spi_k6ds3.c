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
 * examples/examples/sensorbd/examples/spi_k6ds3.c
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

#include <tinyara/spi/spi.h>

#define K6DS3_FACTORY_ID		0x69
#define K6DS3_REG_WHO_AM_I		0x0F
#define K6DS3_REG_CTRL1_XL		0x10
#define K6DS3_REG_CTRL2_G		0x11

/* Gyro */
#define K6DS3_REG_OUTX_L_G		0x22
#define K6DS3_REG_OUTX_H_G		0x23
#define K6DS3_REG_OUTY_L_G		0x24
#define K6DS3_REG_OUTY_H_G		0x25
#define K6DS3_REG_OUTZ_L_G		0x26
#define K6DS3_REG_OUTZ_H_G		0x27

/* Accel */
#define K6DS3_REG_OUTX_L_XL		0x28
#define K6DS3_REG_OUTX_H_XL		0x29
#define K6DS3_REG_OUTY_L_XL		0x2A
#define K6DS3_REG_OUTY_H_XL		0x2B
#define K6DS3_REG_OUTZ_L_XL		0x2C
#define K6DS3_REG_OUTZ_H_XL		0x2D

static struct spi_dev_s *spi_dev;

static char spi_read(int port, int addr, int frequency, int bits, int conf)
{
	unsigned char buf[2];
	buf[0] = addr | 0x80;

	if (spi_dev != 0x0) {
		SPI_LOCK(spi_dev, true);

		SPI_SETFREQUENCY(spi_dev, frequency);
		SPI_SETBITS(spi_dev, bits);
		SPI_SETMODE(spi_dev, conf);

		SPI_SELECT(spi_dev, port, true);
		SPI_RECVBLOCK(spi_dev, buf, 2);
		SPI_SELECT(spi_dev, port, false);

		SPI_LOCK(spi_dev, false);
	}

	return buf[1];
}

static void spi_write(int port, int addr, int frequency, int bits, int conf, char value)
{
	unsigned char buf[2];
	buf[0] = addr;
	buf[1] = value;

	if (spi_dev != 0x0) {
		SPI_LOCK(spi_dev, true);

		SPI_SETFREQUENCY(spi_dev, frequency);
		SPI_SETBITS(spi_dev, bits);
		SPI_SETMODE(spi_dev, conf);

		SPI_SELECT(spi_dev, port, true);
		SPI_SNDBLOCK(spi_dev, buf, 2);
		SPI_SELECT(spi_dev, port, false);

		SPI_LOCK(spi_dev, false);
	}
}

void k6ds3_main(int argc, char *argv[])
{
	int port = 0;
	int freq = 1000000;
	int bits = 8;
	int conf = 0;

	spi_dev = up_spiinitialize(port);

	if (spi_read(port, K6DS3_REG_WHO_AM_I, freq, bits, conf) != K6DS3_FACTORY_ID) {
		printf("It is not K6DS3 \n");
		return;
	}

	/* initialize */

	spi_write(port, K6DS3_REG_CTRL1_XL, freq, bits, conf, 0x80);
	spi_write(port, K6DS3_REG_CTRL2_G, freq, bits, conf, 0x80);

	/* read */
	uint16_t axl, axh, ayl, ayh, azl, azh, gxl, gxh, gyl, gyh, gzl, gzh;
	uint32_t ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;

	int i;
	for (i = 0; i < 30; i++) {
		/* accel */
		axl = spi_read(port, K6DS3_REG_OUTX_L_XL, freq, bits, conf);
		axh = spi_read(port, K6DS3_REG_OUTX_H_XL, freq, bits, conf);
		ayl = spi_read(port, K6DS3_REG_OUTY_L_XL, freq, bits, conf);
		ayh = spi_read(port, K6DS3_REG_OUTY_H_XL, freq, bits, conf);
		azl = spi_read(port, K6DS3_REG_OUTZ_L_XL, freq, bits, conf);
		azh = spi_read(port, K6DS3_REG_OUTZ_H_XL, freq, bits, conf);

		ax = (axl | (axh << 8));
		ay = (ayl | (ayh << 8));
		az = (azl | (azh << 8));

		/* gyro */
		gxl = spi_read(port, K6DS3_REG_OUTX_L_G, freq, bits, conf);
		gxh = spi_read(port, K6DS3_REG_OUTX_H_G, freq, bits, conf);
		gyl = spi_read(port, K6DS3_REG_OUTY_L_G, freq, bits, conf);
		gyh = spi_read(port, K6DS3_REG_OUTY_H_G, freq, bits, conf);
		gzl = spi_read(port, K6DS3_REG_OUTZ_L_G, freq, bits, conf);
		gzh = spi_read(port, K6DS3_REG_OUTZ_H_G, freq, bits, conf);

		gx = (gxl | (gxh << 8));
		gy = (gyl | (gyh << 8));
		gz = (gzl | (gzh << 8));

		printf("ACCEL:x(0x%04X) y(0x%04X) z(0x%04X) GYRO:x(0x%04X) y(0x%04X) z(0x%04X)\n", ax, ay, az, gx, gy, gz);

		up_mdelay(500);
	}
}
