/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include <tinyara/sensors/ais25ba.h>

#include "amebasmart_i2s.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* i2c config */
#define AIS25BA_I2C_PORT		0
#define AIS25BA_I2C_FREQ		100000
#define AIS25BA_I2C_ADDRLEN		7
#define AIS25BA_I2C_ADDR		0x19
/* i2s config */
#define AIS25BA_I2S_PORT		2

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct ais25ba_dev_s g_ais25ba_dev = {
	.i2c = NULL,
	.i2c_config = {
		.frequency = AIS25BA_I2C_FREQ,
		.address = AIS25BA_I2C_ADDR,
		.addrlen = AIS25BA_I2C_ADDRLEN,
	},
	.i2s = NULL,
	.upper = NULL,
	.priv = NULL,
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void rtl8730e_ais25ba_initialize()
{
	FAR struct i2c_dev_s *i2c;
	struct i2s_dev_s *i2s;
	int ret = 0;
	i2s = amebasmart_i2s_tdm_initialize(AIS25BA_I2S_PORT, 0);
	if (!i2s) {
		sndbg("ERROR: Failed to initialize I2S\n");
	}

	i2c = up_i2cinitialize(AIS25BA_I2C_PORT);
	if (!i2c) {
		sndbg("ERROR: Failed to initialize I2C\n");
	}

	g_ais25ba_dev.i2c = i2c;
	g_ais25ba_dev.i2s = i2s;
	ret = ais25ba_initialize("/dev/sensor-mems", &g_ais25ba_dev);
	if (ret != OK) {
		sndbg("ERROR: MEMS ais25ba driver register fail\n");
		return;
	}
	snvdbg("MEMS ais25ba driver register success\n");
}
