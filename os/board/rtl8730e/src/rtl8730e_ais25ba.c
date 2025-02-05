/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>
#include <tinyara/i2c.h>
#include <tinyara/gpio.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/sensors/sensor.h>
#include <tinyara/sensors/ais25ba.h>

/*#include "objects.h"
#include "gpio_irq_api.h"
#include "PinNames.h"
#include "gpio_api.h"*/
#include "amebasmart_i2s.h"
#define PIN_LOW 0
#define PIN_HIGH 1

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* i2c config */
#if CONFIG_RTL8730E_BOARD_REVISION >= 6
#define AIS25BA_I2C_PORT		0
#else
#define AIS25BA_I2C_PORT		2
#endif

#define AIS25BA_I2C_FREQ		100000
#define AIS25BA_I2C_ADDRLEN		7
#define AIS25BA_I2C_ADDR		0x19
#define AIS25BA_I2S_PORT                2

/*other pin config */
#define AIS25BA_GPIO_RESET_PIN		PA_5
#if CONFIG_RTL8730E_BOARD_REVISION >= 6
#define AIS25BA_GPIO_I2C_PIN PA_4
#else
#define AIS25BA_GPIO_I2C_PIN PA_2
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct rtl8730e_ais25ba_s {
	//struct lcd_touch_config touch_config;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct rtl8730e_ais25ba_s g_rtl8730e_ais25ba_priv0 = {
};

static struct ais25ba_dev_s g_ais25ba_dev0 = {
        .i2c = NULL,
        .i2c_config = {
                .frequency = AIS25BA_I2C_FREQ,
                .address = AIS25BA_I2C_ADDR,
                .addrlen = AIS25BA_I2C_ADDRLEN,
        },
        .priv = &g_rtl8730e_ais25ba_priv0,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void rtl8730e_ais25ba_initialize()
{
	FAR struct i2c_dev_s *i2c;
	struct i2s_dev_s *i2s;
	i2s = amebasmart_i2s_tdm_initialize(AIS25BA_I2S_PORT, 0);
	if (!i2s) {
		lldbg("ERROR: Failed to initialize I2S\n");
	}
	int ret = 0;

	i2c = up_i2cinitialize(AIS25BA_I2C_PORT);
	if (!i2c) {
		lldbg("ERROR: Failed to initialize I2C\n");
	}
	g_ais25ba_dev0.i2c = i2c;
	g_ais25ba_dev0.i2s = i2s;
	ret = ais25ba_initialize("/dev/sensor1", &g_ais25ba_dev0);
	if (ret < 0) {
		lldbg("ERROR: MEMS ais25ba driver register fail\n");
		return;
	}
	lldbg("MEMS ais25ba driver register success\n");
}
