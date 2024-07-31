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

#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/irq.h>
#include <tinyara/audio/i2s.h>
#include <tinyara/audio/syu645b.h>
#include <tinyara/gpio.h>
#include "amebasmart_i2s.h"

#include "objects.h"
#include "PinNames.h"
#include "gpio_api.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* i2c config */
#define SYU645B_I2C_PORT		2

#define SYU645B_I2C_FREQ		100000
#define SYU645B_I2C_ADDRLEN		7

#define SYU645B_I2C_ADDR		0x2A

/* i2s config */
#define SYU645B_I2S_PORT		2
#define SYU645B_I2S_IS_MASTER		1
#define SYU645B_I2S_IS_OUTPUT		1

/*other pin config */
#define SYU645B_GPIO_RESET_PIN		PA_22

#define SYU645B_AVAILABLE_MINOR_MIN	0
#define SYU645B_AVAILABLE_MINOR_MAX	25

#ifdef CONFIG_AUDIO_SYU645B

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct rtl8730e_syu645b_audioinfo_s {
	struct syu645b_lower_s lower;
	syu645b_handler_t handler;
	FAR void *arg;
	gpio_t reset;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void syu645b_control_reset_pin(bool active);
static void syu645b_control_powerdown(bool powerdown);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct rtl8730e_syu645b_audioinfo_s g_syu645binfo = {
	.lower = {
		.i2c_config = {
			.frequency = SYU645B_I2C_FREQ,
			.address = SYU645B_I2C_ADDR,
			.addrlen = SYU645B_I2C_ADDRLEN,
		},
	},
	.reset.pin = SYU645B_GPIO_RESET_PIN,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void syu645b_control_reset_pin(bool active)
{
	gpio_write(&(g_syu645binfo.reset), !active);
}

static void syu645b_control_powerdown(bool enter_powerdown)
{
	/* currently does nothing */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

extern FAR struct i2s_dev_s *amebasmart_i2s_initialize(uint16_t port, bool is_reinit);

/****************************************************************************
 * Name: rtl8730e_syu645b_initialize
 *
 * Description:
 *   This function is called by platform-specific, setup logic to configure
 *   and register the SYU645B device.  This function will register the driver
 *   as /dev/pcmC[x]D0p where x is determined by the minor device number.
 *
 * Input Parameters:
 *   minor - The input device minor number
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/
int rtl8730e_syu645b_initialize(int minor)
{
	FAR struct audio_lowerhalf_s *syu645b = NULL;
	FAR struct i2c_dev_s *i2c = NULL;
	FAR struct i2s_dev_s *i2s = NULL;
	static bool initialized = false; // for compilation sake
	char devname[12];
	int ret;

	audvdbg("minor %d\n", minor);
	DEBUGASSERT(minor >= SYU645B_AVAILABLE_MINOR_MIN && minor <= SYU645B_AVAILABLE_MINOR_MAX);

	/* Have we already initialized?  Since we never uninitialize we must prevent
	 * multiple initializations.
	 */

	if (!initialized) {
		/* Reset pin initialization */
		gpio_init(&(g_syu645binfo.reset), SYU645B_GPIO_RESET_PIN);
		gpio_dir(&(g_syu645binfo.reset), PIN_OUTPUT);
		gpio_mode(&(g_syu645binfo.reset), PullNone);

		/* Get an instance of the I2C interface for the SYU645B chip select */
		i2c = up_i2cinitialize(SYU645B_I2C_PORT);
		if (!i2c) {
			auddbg("ERROR: Failed to initialize CODEC I2C%d\n");
			ret = -ENODEV;
			goto errout;
		}
		auddbg("i2c init done\n");
		auddbg("calling init i2s\n");
		/* Get an instance of the I2S interface for the SYU645B data channel */
		i2s = amebasmart_i2s_initialize(SYU645B_I2S_PORT, I2S_INIT);
		if (!i2s) {
			auddbg("ERROR: Failed to initialize I2S\n");
			ret = -ENODEV;
			goto errout;
		}
		
		auddbg("i2s init done\n");

#ifdef CONFIG_SYU645B_EXCLUDE_PDN_CONTROL
		g_syu645binfo.lower.control_powerdown = syu645b_control_powerdown;
#else
		g_syu645binfo.lower.control_powerdown = syu645b_control_powerdown;
		g_syu645binfo.lower.control_powerdown(false);
#endif

		g_syu645binfo.lower.control_hw_reset = syu645b_control_reset_pin;
		g_syu645binfo.lower.control_hw_reset(true); /*power on*/
		up_mdelay(10);
		g_syu645binfo.lower.control_hw_reset(false); /*does nothing*/
		up_mdelay(10);

		/* Now we can use these I2C and I2S interfaces to initialize the
		* SYU645B which will return an audio interface.
		*/
		/* Create a device name */
		snprintf(devname, sizeof(devname), "pcmC%uD%u%c", minor, 0, 'p');
		auddbg("calling init lower\n");
		syu645b = syu645b_initialize(i2c, i2s, &g_syu645binfo.lower);
		if (!syu645b) {
			auddbg("ERROR: Failed to initialize the SYU645B\n");
			ret = -ENODEV;
			goto errout;
		}
		auddbg("calling init lower done\n");
		/* Finally, we can register the PCM/SYU645B/I2C/I2S audio device.
		 */
		auddbg("registering audio device here\n");
		ret = audio_register(devname, syu645b);
		auddbg("registered the device\n");
		if (ret < 0) {
			auddbg("ERROR: Failed to register /dev/%s device: %d\n", devname, ret);
			goto errout;
		}

		/* Now we are initialized */

		initialized = true;
	}

	return OK;

	/* Error exits.  Unfortunately there is no mechanism in place now to
	 * recover resources from most errors on initialization failures.
	 */
errout:
	return ret;
}
#endif
