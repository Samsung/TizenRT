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
#include <arch/board/artik055_alc5658_i2c.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define ALC5658_ADDR 0x1A
#define CONFIG_ALC5658_I2C_PORT 1
#define CONFIG_ALC5658_I2C_FREQ 100000
#define CONFIG_ALC5658_I2C_ADDRLEN 7
/****************************************************************************
 * Private Functions prototypes
 ****************************************************************************/
static int alc5658_write(uint16_t addr, uint16_t data);
static int alc5658_read(uint16_t addr, uint16_t *data);
static int alc5658_modify(uint16_t addr, uint16_t set, uint16_t clear);

/************************************************************************************
 * Private Types
 ************************************************************************************/
typedef struct {
	uint16_t addr;
	uint16_t val;
	unsigned int delay;
} t_codec_init_script_entry;

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct i2c_dev_s *i2c_dev;
static struct i2c_config_s configs;

static alc_i2c_t alc5658_i2c = {
	.write = alc5658_write,
	.read = alc5658_read,
	.modify = alc5658_modify,
};

/*This init script enables codec audio output */
/* Uncomment when it is needed
static t_codec_init_script_entry codec_reset_script[] = {
	{0x0000, 0x0000, 0},
};
*/

/*This init script enables codec audio output */
static t_codec_init_script_entry codec_init_script[] = {
	{0x0000, 0x0000, 0},
	{0x006E, 0xFFFF, 0},
	{0x006F, 0xFFFF, 0},
	{0x0080, 0x8000, 0},
	{0x0094, 0x0280, 0},
	{0x0111, 0xA502, 0},
	{0x0125, 0x0430, 0},
	{0x013A, 0x3020, 0},
	{0x0073, 0x1770, 0},

	//{0x0070, 0x8020, 0},
	{0x0070, 0x0000, 0},
	{0x007B, 0x0003, 0},

	{0x00FA, 0x0001, 0},
	{0x0091, 0x0C16, 0},
	{0x0063, 0xA23E, 60},
	{0x0063, 0xF23E, 50},
	{0x0062, 0x0400, 50},
	{0x0061, 0x8080, 10},
	{0x0029, 0x8080, 0},
	{0x002A, 0xAAAA, 0},
	{0x002D, 0x0000, 0},
	{0x008E, 0x0009, 50},
	{0x0061, 0x8C80, 50},
	{0x0091, 0x0E16, 50},
	{0x0040, 0x0505, 0},
	{0x0065, 0x0180, 0},
	{0x013C, 0x3C05, 0},
	{0x01DF, 0x02C1, 0},
	{0x01DF, 0x2CC1, 0},
	{0x01DE, 0x5100, 0},
	{0x01E4, 0x0014, 0},
	{0x01DE, 0xD100, 30},
	{0x01DF, 0x2CC1, 0},
	{0x01DE, 0x4900, 0},
	{0x01E4, 0x0016, 0},
	{0x01DE, 0xC900, 250},
	{0x01DF, 0x2CC1, 0},
	{0x0002, 0x0000, 0},
	{0x01DE, 0x4500, 0},
	{0x01E4, 0x001F, 0},
	{0x01DE, 0xC500, 800},
	{0x0040, 0x0808, 0},
	{0x0065, 0x0000, 0},
	{0x013C, 0x2005, 0},
	{0x01E4, 0x0000, 0},
	{0x01DF, 0x20C0, 0},
	{0x0073, 0x0770, 0},
	{0x0080, 0x0000, 0},
	{0x0160, 0x8EC0, 0},
	{0x008E, 0x0019, 0},
	{0x0015, 0xC0F0, 0},
	{0x0015, 0x87F9, 0},
	{0x0094, 0x0180, 0},
	{0x00FB, 0x3000, 0},
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

static void delay(unsigned int mS)
{
	volatile clock_t start = clock_systimer();

	mS = mS / MSEC_PER_TICK + 1;

	while (1)
		if ((start + mS) < clock_systimer()) {
			return;
		}
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
 *   pointer to alc_i2c_t operations structure, or NULL in case of errors
 *
 ****************************************************************************/

alc_i2c_t *alc5658_i2c_initialize(void)
{
	alc_i2c_t *i2c;
	int i;
	int ret;

	i2c_dev = up_i2cinitialize(CONFIG_ALC5658_I2C_PORT);
	if (i2c_dev == NULL) {
		i2cerr("ALC5658 up_i2cinitialize(port: %d) failed\n", CONFIG_ALC5658_I2C_PORT);
		return NULL;
	}

	configs.frequency = CONFIG_ALC5658_I2C_FREQ;
	configs.address = ALC5658_ADDR;
	configs.addrlen = CONFIG_ALC5658_I2C_ADDRLEN;

	i2c = &alc5658_i2c;

	for (i = 0; i < sizeof(codec_init_script) / sizeof(t_codec_init_script_entry); i++) {
		ret = i2c->modify(codec_init_script[i].addr, codec_init_script[i].val, 0xFFFF);
		audvdbg("ALC %x <- %x\n", codec_init_script[i].addr, ret);
		delay(codec_init_script[i].delay);
	}

	return i2c;
}
