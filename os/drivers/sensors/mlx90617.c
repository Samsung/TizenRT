/****************************************************************************
 * drivers/input/ist415.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
//#include <poll.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#include <tinyara/spi/spi.h>
#include <tinyara/i2c.h>
#include <tinyara/sensors/sensor.h>
#include <tinyara/sensors/mlx90617.h>

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static ssize_t mlx90617_read(struct sensor_upperhalf_s *dev, FAR char *buffer);

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct sensor_ops_s g_mlx90617_ops = {
	.sensor_read = mlx90617_read,
};
/****************************************************************************
 * Private Functions
 ****************************************************************************/
uint8_t Calculate_PEC(uint8_t initPEC, uint8_t newData)
{
	uint8_t data;
	uint8_t bitCheck;
	data = initPEC ^ newData;
	for (int i=0; i<8; i++ ) {
		bitCheck = data & 0x80;
		data = data << 1;
		if (bitCheck != 0) {data = data ^ 0x07;}
	}
	return data;
}

static void mlx90617_read_i2c(struct i2c_dev_s *i2c,struct i2c_config_s config, uint8_t reg[], int len, uint16_t *data)
{
	lldbg("\n%d %d %d\n", config.address, config.addrlen, config.frequency);
	uint8_t buffer[3];
	buffer[0] = 0;
	buffer[1] = 0;
	buffer[2] = 0;

	if (!i2c) {
		lldbg("ERROR: i2c not initialized\n");
		return;
	}

#ifdef CONFIG_I2C_WRITEREAD
		int ret = i2c_writeread(i2c, &config, reg, 1, buffer, len);
		lldbg("ret %d\n", ret);
#else
		int ret = i2c_write(i2c, &config, reg, 1);
		if (ret != 1) {
			lldbg("ERROR: i2c write not working\n");
			return;
		}
		ret = i2c_read(i2c, &config, buffer, len);
#endif

	//PEC error checking
	uint8_t pec;
	uint8_t sa;
	uint8_t cmd = 0;

	sa = config.address << 1;
	pec = sa;
	cmd = reg[0];

	pec = Calculate_PEC(0, pec);
	pec = Calculate_PEC(pec, cmd);
	pec = Calculate_PEC(pec, (sa|0x01));
	pec = Calculate_PEC(pec, buffer[0]);
	pec = Calculate_PEC(pec, buffer[1]);
	lldbg("\nreg value: %d, config.address: %x, pec value: %8x, r[2]: %8x\n", reg[0], config.address, pec, buffer[2]);
	if(pec != buffer[2]){
		lldbg("PEC error\n");
	} else {
		lldbg("PEC success\n");
	}

	data[0] = (uint16_t)(buffer[0] | (buffer[1] << 8));
	lldbg("\nvalue[0]: %8x | value[1]: %8x | value[2]: %8x ret: %d\n", buffer[0], buffer[1], buffer[2], ret);

	if (reg == 0x6 || reg == 0x7) {
		if (data[0] > 32767){
			lldbg("Above data should be ignored\n");
		}
	}
}

static ssize_t mlx90617_read(struct sensor_upperhalf_s *dev, FAR char *buffer)
{
	FAR struct mlx90617_dev_s *priv = (struct mlx90617_dev_s *)dev->priv;
	int ret;
	uint8_t reg[2] = {0, 0};
	uint16_t data[3] = {0, 0, 0};
	float Ta_N, To_N;

	struct i2c_dev_s *i2c = priv->i2c;
	struct i2c_config_s config = priv->i2c_config;
	/* Wait for semaphore to prevent concurrent reads */

	config.address = 0x5A;
	reg[0] = 0x06;
	mlx90617_read_i2c(i2c, config, reg, 3, data);
	Ta_N = (float)(data[0]) * 0.02 - 273.15;  // MLX90614 Ta
	lldbg("mlx90614 Ta_N   %f\n", Ta_N);
	
	reg[0] = 0x07;
	mlx90617_read_i2c(i2c, config, reg, 3, data);
	To_N = (float)(data[0]) * 0.02 - 273.15;  // MLX90614 To
	lldbg("mlx90614 To_N   %f\n", To_N);

	config.address = 0x5D;
	reg[0] = 0x06;
	mlx90617_read_i2c(i2c, config, reg, 3, data);
	Ta_N = (float)(data[0]) * 0.02 - 273.15;  // MLX90614 Ta
	lldbg("mlx90617 Ta_N    %f\n", Ta_N);

	reg[0] = 0x07;
	mlx90617_read_i2c(i2c, config, reg, 3, data);
	To_N = (float)(data[0]) * 0.02 - 273.15;  // MLX90614 To
	lldbg("mlx90617 To_N    %f\n", To_N);

	return 0;
}

int mlx90617_initialize(FAR const char *devpath, struct mlx90617_dev_s *priv)
{
	int ret = 0;

	/* Setup device structure. */
	struct sensor_upperhalf_s *upper = (struct sensor_upperhalf_s *)kmm_zalloc(sizeof(struct sensor_upperhalf_s));
	upper->ops = &g_mlx90617_ops;
	upper->priv = priv;
	priv->upper = upper;

	return sensor_register(devpath, upper);
}
