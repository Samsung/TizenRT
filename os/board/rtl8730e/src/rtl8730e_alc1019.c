/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/irq.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/audio/alc1019.h>
#include <tinyara/gpio.h>
#include "amebasmart_i2s.h"

#include "objects.h"
#include "PinNames.h"
#include "gpio_api.h"

extern FAR struct i2s_dev_s *amebasmart_i2s_initialize(uint16_t port);

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* i2c config */
#define ALC1019_I2C_PORT		1
#define ALC1019_I2C_FREQ		100000
#define ALC1019_I2C_ADDRLEN		7

#define ALC1019_I2C_ADDR		0x28 

/* i2s config */
#define ALC1019_I2S_PORT		2
#define ALC1019_I2S_IS_MASTER		1
#define ALC1019_I2S_IS_OUTPUT		1

/*other pin config */
#define ALC1019_GPIO_RESET_PIN		PA_22

#define ALC1019_AVAILABLE_MINOR_MIN	0
#define ALC1019_AVAILABLE_MINOR_MAX	25

#ifdef CONFIG_AUDIO_ALC1019

extern struct i2s_dev_s *amebad_i2s_initialize(uint16_t port);

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct rtl8730e_alc1019_audioinfo_s {
	struct alc1019_lower_s lower;
	alc1019_handler_t handler;
	FAR void *arg;
	gpio_t reset;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void alc1019_control_reset_pin(bool active);
static void alc1019_control_powerdown(bool powerdown);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct rtl8730e_alc1019_audioinfo_s g_alc1019info = {
	.lower = {
		.i2c_config = {
			.frequency = ALC1019_I2C_FREQ,
			.address = ALC1019_I2C_ADDR,
			.addrlen = ALC1019_I2C_ADDRLEN,
		},
	},
	.reset.pin = ALC1019_GPIO_RESET_PIN,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void alc1019_control_reset_pin(bool active)
{
	gpio_write(&(g_alc1019info.reset), !active);
}

static void alc1019_control_powerdown(bool enter_powerdown)
{
	/* currently does nothing */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rtl8730e_alc1019_initialize
 *
 * Description:
 *   This function is called by platform-specific, setup logic to configure
 *   and register the ALC1019 device.  This function will register the driver
 *   as /dev/alc1019[x] where x is determined by the minor device number.
 *
 * Input Parameters:
 *   minor - The input device minor number
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/
int rtl8730e_alc1019_initialize(int minor)
{
	FAR struct audio_lowerhalf_s *alc1019;
	FAR struct i2c_dev_s *i2c;
	FAR struct i2s_dev_s *i2s = NULL;
	static bool initialized = false; // for compilation sake
	char devname[12];
	int ret;

	audvdbg("minor %d\n", minor);
	DEBUGASSERT(minor >= ALC1019_AVAILABLE_MINOR_MIN && minor <= ALC1019_AVAILABLE_MINOR_MAX);

	/* Have we already initialized?  Since we never uninitialize we must prevent
	 * multiple initializations.  This is necessary, for example, when the
	 * touchscreen example is used as a built-in application in NSH and can be
	 * called numerous time.  It will attempt to initialize each time.
	 */

	if (!initialized) {
		/* Reset pin initialization */
		gpio_init(&(g_alc1019info.reset), ALC1019_GPIO_RESET_PIN);
		gpio_dir(&(g_alc1019info.reset), PIN_OUTPUT);
		gpio_mode(&(g_alc1019info.reset), PullNone);

		/* Get an instance of the I2C interface for the ALC1019 chip select */
		i2c = up_i2cinitialize(ALC1019_I2C_PORT);
		if (!i2c) {
			auddbg("ERROR: Failed to initialize CODEC I2C%d\n");
			ret = -ENODEV;
			goto errout_with_i2c;
		}
		auddbg("i2c init done\n");
		auddbg("calling init i2s\n");
		/* Get an instance of the I2S interface for the ALC1019 data channel */
		i2s = amebasmart_i2s_initialize(ALC1019_I2S_PORT);
		if (!i2s) {
			auddbg("ERROR: Failed to initialize I2S\n");
			ret = -ENODEV;
			goto errout_with_i2s;
		}
		
		auddbg("calling init i2s done\n");

#ifdef CONFIG_ALC1019_EXCLUDE_PDN_CONTROL
		g_alc1019info.lower.control_powerdown = alc1019_control_powerdown;
#else
		g_alc1019info.lower.control_powerdown = alc1019_control_powerdown;
		g_alc1019info.lower.control_powerdown(false);
#endif

		g_alc1019info.lower.control_hw_reset = alc1019_control_reset_pin;
		g_alc1019info.lower.control_hw_reset(true); /*power on*/
		up_mdelay(10);
		g_alc1019info.lower.control_hw_reset(false); /*does nothing*/
		up_mdelay(10);

		/* Now we can use these I2C and I2S interfaces to initialize the
		* ALC1019 which will return an audio interface.
		*/
		/* Create a device name */
		snprintf(devname, sizeof(devname), "pcmC%uD%u%c", minor, 0, 'p');
		auddbg("calling init lower\n");
		alc1019 = alc1019_initialize(i2c, i2s, &g_alc1019info.lower);
		if (!alc1019) {
			auddbg("ERROR: Failed to initialize the ALC1019\n");
			ret = -ENODEV;
			goto errout_with_alc1019;
		}
		auddbg("calling init lower done\n");
		/* Finally, we can register the PCM/ALC1019/I2C/I2S audio device.
		 */
		auddbg("registering audio device here\n");
		ret = audio_register(devname, alc1019);
		auddbg("registrred the device\n");
		if (ret < 0) {
			auddbg("ERROR: Failed to register /dev/%s device: %d\n", devname, ret);
			goto errout_with_pcm;
		}

		/* Now we are initialized */

		initialized = true;
	}

	return OK;

	/* Error exits.  Unfortunately there is no mechanism in place now to
	 * recover resources from most errors on initialization failures.
	 */

errout_with_pcm:
errout_with_i2c:
errout_with_alc1019:
errout_with_i2s:
	return ret;
}
#endif
