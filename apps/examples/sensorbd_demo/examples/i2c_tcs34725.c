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
 * examples/examples/sensorbd/examples/i2c_tcs34725.c
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

#include <stdio.h>
#include <sys/types.h>
#include <tinyara/i2c.h>
#include <tinyara/math.h>

#define TCS34725_ADDRESS          (0x29)
#define TCS34725_COMMAND_BIT      (0x80)

#define TCS34725_ENABLE           (0x00)
#define TCS34725_ENABLE_AEN       (0x02)	/* RGBC Enable - Writing 1 actives the ADC, 0 disables it */
#define TCS34725_ENABLE_PON       (0x01)	/* Power on - Writing 1 activates the internal oscillator, 0 disables it */
#define TCS34725_ATIME            (0x01)	/* Integration time */
#define TCS34725_CONTROL          (0x0F)	/* Set the gain level for the sensor */
#define TCS34725_ID               (0x12)	/* 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727 */
#define TCS34725_CDATAL           (0x14)	/* Clear channel data */
#define TCS34725_RDATAL           (0x16)	/* Red channel data */
#define TCS34725_GDATAL           (0x18)	/* Green channel data */
#define TCS34725_BDATAL           (0x1A)	/* Blue channel data */

#define TCS34725_INTEGRATIONTIME_2_4MS  (0xFF)	/*  2.4ms - 1 cycle    */
#define TCS34725_INTEGRATIONTIME_24MS   (0xF6)	/*  24ms  - 10 cycles  */
#define TCS34725_INTEGRATIONTIME_50MS   (0xEB)	/*  50ms  - 20 cycles  */
#define TCS34725_INTEGRATIONTIME_101MS  (0xD5)	/*  101ms - 42 cycles  */
#define TCS34725_INTEGRATIONTIME_154MS  (0xC0)	/*  154ms - 64 cycles  */
#define TCS34725_INTEGRATIONTIME_700MS  (0x00)	/*  700ms - 256 cycles */

#define TCS34725_GAIN_1X      (0x00)	/*  No gain  */
#define TCS34725_GAIN_4X      (0x01)	/*  4x gain  */
#define TCS34725_GAIN_16X     (0x02)	/*  16x gain */
#define TCS34725_GAIN_60X     (0x03)	/*  60x gain */

static struct i2c_dev_s *i2c_dev;
static struct i2c_config_s configs;

uint32_t tcs34725IntegrationTime;

static void tcs34725_write(uint8_t addr, uint8_t data)
{
	uint8_t reg[2];
	reg[0] = TCS34725_COMMAND_BIT | addr;
	reg[1] = data;

	i2c_write(i2c_dev, &configs, reg, 2);
}

static void tcs34725_read(uint8_t addr, uint8_t *data, uint8_t bytecnt)
{
	uint8_t reg;
	reg = TCS34725_COMMAND_BIT | addr;
	i2c_write(i2c_dev, &configs, &reg, 1);
	i2c_read(i2c_dev, &configs, data, bytecnt);
}

static void tcs34725_getdata(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
	tcs34725_read(TCS34725_CDATAL, (uint8_t *)c, 2);
	tcs34725_read(TCS34725_RDATAL, (uint8_t *)r, 2);
	tcs34725_read(TCS34725_GDATAL, (uint8_t *)g, 2);
	tcs34725_read(TCS34725_BDATAL, (uint8_t *)b, 2);

	switch (tcs34725IntegrationTime) {
	case TCS34725_INTEGRATIONTIME_2_4MS:
		up_mdelay(3);
		break;
	case TCS34725_INTEGRATIONTIME_24MS:
		up_mdelay(24);
		break;
	case TCS34725_INTEGRATIONTIME_50MS:
		up_mdelay(50);
		break;
	case TCS34725_INTEGRATIONTIME_101MS:
		up_mdelay(101);
		break;
	case TCS34725_INTEGRATIONTIME_154MS:
		up_mdelay(154);
		break;
	case TCS34725_INTEGRATIONTIME_700MS:
		up_mdelay(700);
		break;
	}
}

static inline float powfl(const float x, const float y)
{
	return (float)(pow((double)x, (double)y));
}

static uint16_t tcs34725_calclux(uint16_t r, uint16_t g, uint16_t b)
{
	float illuminance;

	return (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

	return (uint16_t)illuminance;
}

/****************************************************************************
* Name: tcs34725_calctemperature
****************************************************************************/

static uint16_t tcs34725_calctemperature(uint16_t r, uint16_t g, uint16_t b)
{
	float X, Y, Z;
	float xc, yc;
	float n;
	float cct;

	X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
	Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
	Z = (-0.68202F * r) + (0.77073F * g) + (0.56332F * b);

	xc = (X) / (X + Y + Z);
	yc = (Y) / (X + Y + Z);

	n = (xc - 0.3320F) / (0.1858F - yc);

	cct = (449.0F * powfl(n, 3)) + (3525.0F * powfl(n, 2)) + (6823.3F * n) + 5520.33F;

	return (uint16_t)cct;
}

static void tcs34725_initialize(void)
{
	uint8_t reg = 0;
	tcs34725IntegrationTime = TCS34725_INTEGRATIONTIME_50MS;

	tcs34725_read(TCS34725_ID, &reg, 1);
	if ((reg != 0x44) && (reg != 0x10)) {
		printf("Bad devie id(0x%02x)\n", reg);
		return;
	}
	printf("Successfully read device id(0x%02x)\n", reg);

	tcs34725_write(TCS34725_ATIME, tcs34725IntegrationTime);
	tcs34725_write(TCS34725_CONTROL, TCS34725_GAIN_4X);
	tcs34725_write(TCS34725_ENABLE, TCS34725_ENABLE_PON);
	up_mdelay(10);

	tcs34725_write(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
	up_mdelay(10);
}

void tcs34725_main(int argc, char *argv[])
{
	int i, port = 1;
	uint16_t r, g, b, c, temp, lux;

	i2c_dev = up_i2cinitialize(port);
	if (i2c_dev == NULL) {
		printf("up_i2cinitialize failed(i2c:%d)\n", port);
		fflush(stdout);
	}

	configs.frequency = 400000;
	configs.address = TCS34725_ADDRESS;
	configs.addrlen = 7;

	tcs34725_initialize();

	r = g = b = c = temp = lux = 0;
	for (i = 0; i < 30; i++) {
		tcs34725_getdata(&r, &g, &b, &c);

		temp = tcs34725_calctemperature(r, g, b);
		lux = tcs34725_calclux(r, g, b);

		printf("(R:%d), (G:%d), (B:%d), (C:%d), (Color Temp:%d), (Lux:%d)\n", r, g, b, c, temp, lux);
		up_mdelay(500);
	}
}
