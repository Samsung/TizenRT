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
#include <errno.h>
#include <debug.h>

#if defined(CONFIG_AUDIO_I2SCHAR) && defined(CONFIG_ARMINO_I2S)

#include <tinyara/audio/i2s.h>
#include "armino_i2s.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_ARMINO_I2SCHAR_MINOR
#define CONFIG_ARMINO_I2SCHAR_MINOR 0
#endif
/*****************************************************************************
 * Private Functions
 ****************************************************************************/

static void err_cb(FAR struct i2s_dev_s *dev, FAR void *arg, int flags)
{
/* Do something here */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: i2schar_devinit
 *
 * Description:
 *   All architectures must provide the following interface in order to
 *   work with apps/examples/i2schar.
 *
 ****************************************************************************/

int i2schar_devinit(void)
{
	static bool initialized;
	struct i2s_dev_s *i2s;
	int ret;

	/* Have we already initialized? */

	if (!initialized) {
		/* Call armino_i2s_initialize() to get an instance of the I2S interface */

		i2s = armino_i2s_initialize(0);
		if (!i2s) {
			lldbg("ERROR: Failed to get the ARMINO I2S driver\n");
			return -ENODEV;
		}

		/* Register the I2S character driver at "/dev/i2schar0" */

		ret = i2schar_register(i2s, CONFIG_ARMINO_I2SCHAR_MINOR);
		if (ret < 0) {
			lldbg("ERROR: i2schar_register failed: %d\n", ret);
			return ret;
		}

		I2S_ERR_CB_REG(i2s, err_cb, "Error_Test_String");


		/* Now we are initialized */

		initialized = true;
	}

	return OK;
}

#endif							/* CONFIG_AUDIO_I2SCHAR && CONFIG_ARMINO_I2S */
