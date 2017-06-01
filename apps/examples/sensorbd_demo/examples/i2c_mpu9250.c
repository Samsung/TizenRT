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
 * examples/examples/sensorbd/examples/i2c_mpu9250.c
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

#include "i2c_mpu9250.h"

enum mpu9250_axis_e {
	MPU9250_AXIS_X = 0,
	MPU9250_AXIS_Y = 1,
	MPU9250_AXIS_Z = 2,
};

static int mpu9250_initialize(void);

static struct i2c_dev_s *i2c_dev;
static struct i2c_config_s configs;

uint8_t mpu9250_disable_mask[3] = {
	MPU9250_DISABLE_XA_MASK,
	MPU9250_DISABLE_YA_MASK,
	MPU9250_DISABLE_ZA_MASK,
};

uint8_t mpu9250_accel_out[3] = {
	MPU9250_ACCEL_XOUT_H,
	MPU9250_ACCEL_YOUT_H,
	MPU9250_ACCEL_ZOUT_H,
};

static uint32_t mpu9250_get_axis(uint8_t i)
{
	uint8_t data[2];
	int ret;
	uint8_t reg;
	data[0] = 0;
	data[1] = 0;

	reg = MPU9250_PWR_MGMT_2;

	ret = i2c_write(i2c_dev, &configs, &reg, 1);
	if (ret < 0) {
		printf("i2c_write fail(%d)\n", ret);
		return -ret;
	}

	up_mdelay(1);

	ret = i2c_read(i2c_dev, &configs, data, 1);
	if (ret < 0) {
		printf("i2c_read fail(%d)\n", ret);
		return -ret;
	}

	if (0 != (data[0] & mpu9250_disable_mask[i])) {
		printf("Disabled (%d) axis(%d)\n", i, ret);
		return 0;
	}

	up_mdelay(1);

	reg = mpu9250_accel_out[i];

	ret = i2c_write(i2c_dev, &configs, &reg, 1);
	if (ret < 0) {
		printf("i2c_write fail(%d)\n", ret);
		return -ret;
	}

	up_mdelay(1);

	ret = i2c_read(i2c_dev, &configs, data, 2);

	if (ret < 0) {
		printf("i2c_read fail(%d)\n", ret);
		return -ret;
	}

	return (uint16_t)(data[0] << 8) | data[1];
}

/****************************************************************************
 * Name: mpu9250_initialize
 ****************************************************************************/

static int mpu9250_initialize(void)
{
	uint8_t data[2];
	int ret;
	data[0] = MPU9250_PWR_MGMT_1;
	data[1] = 0x80;

	ret = i2c_write(i2c_dev, &configs, data, 2);
	if (ret < 0) {
		printf("i2c_write fail(%d)\n", ret);
		return -ret;
	}

	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mpu9250_main
 ****************************************************************************/

int mpu9250_main(int argc, char *argv[])
{
	int i;
	uint16_t data[3];

	int port = 1;
	i2c_dev = up_i2cinitialize(port);
	if (i2c_dev == NULL) {
		printf("i2ctest_main: up_i2cinitialize(i2c:%d) failed\n", port);
		goto errout;
	}

	configs.frequency = 40000;
	configs.address = MPU9250_ADDR;
	configs.addrlen = 7;

	mpu9250_initialize();

	printf("ACC:      %-10s%-10s%-10s\n", "X", "Y", "Z");

	for (i = 0; i < 30; i++) {
		data[0] = mpu9250_get_axis(MPU9250_AXIS_X);
		if (data[0] < 0) {
			break;
		}

		data[1] = mpu9250_get_axis(MPU9250_AXIS_Y);
		if (data[1] < 0) {
			break;
		}

		data[2] = mpu9250_get_axis(MPU9250_AXIS_Z);
		if (data[2] < 0) {
			break;
		}

		printf("ACC:      %-10d%-10d%-10d\n", (int16_t)data[0], (int16_t)data[1], (int16_t)data[2]);

		up_mdelay(500);
	}

	return OK;

errout:
	fflush(stdout);
	return ERROR;
}
