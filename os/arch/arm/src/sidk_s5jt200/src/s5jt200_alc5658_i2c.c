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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/clock.h>
#include <debug.h>
#include <assert.h>
#include <sys/types.h>

#include <tinyara/board.h>
#include <arch/board/board.h>
#include <tinyara/i2c.h>
#include <arch/board/s5jt200_alc5658_i2c.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define ALC5658_ADDR 0x1A
#define CONFIG_ALC5658_I2C_PORT 0
#define CONFIG_ALC5658_I2C_FREQ 100000
#define CONFIG_ALC5658_I2C_ADDRLEN 7
/****************************************************************************
 * Private Functions prototypes
 ****************************************************************************/
static int alc5658_write(uint16_t addr, uint16_t data);
static int alc5658_read(uint16_t addr, uint16_t *data);
static int alc5658_modify(uint16_t addr, uint16_t set, uint16_t clear);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct i2c_dev_s *i2c_dev;
static struct i2c_config_s configs;

static i2c_alc i2c_alc5658 = {
	.write = alc5658_write,
	.read = alc5658_read,
	.modify = alc5658_modify,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int alc5658_write(uint16_t addr, uint16_t data)
{
	int32_t ret;
	uint8_t reg[4];

	reg[0] = ((char *)&addr)[1];
	reg[1] = ((char *)&addr)[0];

	reg[2] = ((char *)&data)[1];
	reg[3] = ((char *)&data)[0];

	ret = i2c_write(i2c_dev, &configs, (uint8_t *) reg, 4);

	return ret;
}

static int alc5658_read(uint16_t addr, uint16_t *data)
{
	int32_t ret;
	uint8_t reg[2];
	reg[0] = ((char *)&addr)[1];
	reg[1] = ((char *)&addr)[0];

	ret = i2c_write(i2c_dev, &configs, reg, 2);
	ret = i2c_read(i2c_dev, &configs, reg, 2);

	((char *)data)[0] = reg[1];
	((char *)data)[1] = reg[0];

	return *data;
}

static int alc5658_modify(uint16_t addr, uint16_t set, uint16_t clear)
{
	uint16_t data;

	alc5658_read(addr, &data);
	data &= ~clear;
	data |= set;

	alc5658_write(addr, data);
	alc5658_read(addr, &data);
	return data;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: alc5658_i2c_initialize
 *
 * Description:
 *   Initialize i2c interface to access ALC5658 codec.
 *   Returns methods to communicate with codec
 *
 * Input Parameters:
 *  None
 *
 * Returned Value:
 *   pointer to i2c_alc operations structure, or NULL in case of errors
 *
 ****************************************************************************/

i2c_alc *alc5658_i2c_initialize(void)
{
	i2c_dev = up_i2cinitialize(CONFIG_ALC5658_I2C_PORT);
	if (i2c_dev == NULL) {
		i2cerr("ALC5658 up_i2cinitialize(port: %d) failed\n", CONFIG_ALC5658_I2C_PORT);
		return NULL;
	}

	configs.frequency = CONFIG_ALC5658_I2C_FREQ;
	configs.address = ALC5658_ADDR;
	configs.addrlen = CONFIG_ALC5658_I2C_ADDRLEN;

	return &i2c_alc5658;
}
