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
#include <tinyara/audio/ndp120.h>
#include <tinyara/gpio.h>
#include "gpio_irq_api.h"
#include <tinyara/audio/ndp120.h>

#include "objects.h"
#include "PinNames.h"
#include "gpio_api.h"

#ifdef CONFIG_AUDIO_NDP120

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NDP120_AVAILABLE_MINOR_MIN	0
#define NDP120_AVAILABLE_MINOR_MAX	25

/* spi config */
#define NDP120_SPI_PORT			0
#define NDP120_SPI_FREQ			12000000
#define NDP120_SPI_BPW			8
#define NDP120_SPI_CS			0
#define NDP120_SPI_MODE			SPIDEV_MODE0

#define GPIO_DMIC_EN			PA_25

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct rtl8730e_ndp120_audioinfo_s {
	struct ndp120_lower_s lower;
	ndp120_handler_t handler;
	gpio_t dmic;
	gpio_irq_t data_ready;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct rtl8730e_ndp120_audioinfo_s g_ndp120info = {
	.lower = {
		.spi_config = {
                        .bpw = NDP120_SPI_BPW,
                        .freq = NDP120_SPI_FREQ,
                        .cs = NDP120_SPI_CS,
                        .mode = NDP120_SPI_MODE,
                },
	},

	.handler = NULL,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void rtl8730e_ndp120_irq_handler(uint32_t id, gpio_irq_event event)
{
	/* we are using level based interrupt trigger here,
	 * so, we have to disable this particular gpio interrupt
	 * until we finish this particular interrupt related work
	 * in the HPWORK thread
	 */
	gpio_irq_disable(&g_ndp120info.data_ready);

	if (g_ndp120info.handler != NULL) {
		g_ndp120info.handler(id);
	} else {
		gpio_irq_enable(&g_ndp120info.data_ready);
	}
}

static void rtl8730e_ndp120_enable_irq(bool enable)
{
	if (enable) {
		gpio_irq_enable(&g_ndp120info.data_ready);
	} else {
		gpio_irq_disable(&g_ndp120info.data_ready);
	}
}

static void rtl8730e_ndp120_irq_attach(ndp120_handler_t handler, FAR char *arg)
{
	g_ndp120info.handler = handler;
	gpio_irq_init(&g_ndp120info.data_ready, PA_23, rtl8730e_ndp120_irq_handler, arg);
	gpio_irq_set(&g_ndp120info.data_ready, IRQ_HIGH, 1);
	gpio_irq_enable(&g_ndp120info.data_ready);
}

static void rtl8730e_ndp120_set_dmic(bool enable)
{
	auddbg("Enable DMIC enable : %d\n", enable);
	gpio_dir(&g_ndp120info.dmic, PIN_OUTPUT);
	if (enable) {
		gpio_mode(&g_ndp120info.dmic, PullUp);
		gpio_write(&g_ndp120info.dmic, 1);
	} else {
		gpio_mode(&g_ndp120info.dmic, PullDown);
		gpio_write(&g_ndp120info.dmic, 0);
	}
}

#ifdef CONFIG_PM
static void rtl8730e_ndp120_pm(bool sleep)
{	
	/* h/w specific things to be done if required */
	if (sleep) {
		audvdbg("board entering sleep\n");
	} else {
		audvdbg("board wakeup\n");
	}
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rtl8730e_ndp120_initialize
 *
 * Description:
 *   This function is called by platform-specific, setup logic to configure
 *   and register the NDP120 device.  This function will register the driver
 *   as /dev/ndp120[x] where x is determined by the minor device number.
 *
 * Input Parameters:
 *   minor - The input device minor number
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/
int rtl8730e_ndp120_initialize(int minor)
{
	FAR struct audio_lowerhalf_s *ndp120;
	static bool initialized = false;
	char devname[12];
	int ret = OK;

	audvdbg("minor %d\n", minor);
	DEBUGASSERT(minor >= NDP120_AVAILABLE_MINOR_MIN && minor <= NDP120_AVAILABLE_MINOR_MAX);

	/* Have we already initialized?  Since we never uninitialize we must prevent
	 * multiple initializations.  This is necessary, for example, when the
	 * touchscreen example is used as a built-in application in NSH and can be
	 * called numerous time.  It will attempt to initialize each time.
	 */

	if (!initialized) {

		g_ndp120info.lower.attach = rtl8730e_ndp120_irq_attach;
		g_ndp120info.lower.irq_enable = rtl8730e_ndp120_enable_irq;
		g_ndp120info.lower.set_dmic = rtl8730e_ndp120_set_dmic;
		gpio_init(&g_ndp120info.dmic, GPIO_DMIC_EN);
		
		rtl8730e_ndp120_set_dmic(false);
#ifdef CONFIG_PM
		g_ndp120info.lower.set_pm_state = rtl8730e_ndp120_pm;
#endif
		/* currently spi 0 is only attached to AI SoC, so no
		 * need to change the spi config as we are dealing with
		 * only 1 slave, if we add more slaves, parameters below
		 * should be set always before the transfers and must be
		 * passed to lower level using spi_config in ndp120_lower_s.
		 */
		FAR struct spi_dev_s *spi = up_spiinitialize(NDP120_SPI_PORT);
		
		SPI_SETMODE(spi, NDP120_SPI_MODE);
		SPI_SETFREQUENCY(spi, NDP120_SPI_FREQ);
		SPI_SETBITS(spi, NDP120_SPI_BPW);
		
		ndp120 = (struct audio_lowerhalf_s *)ndp120_lowerhalf_initialize(spi, &g_ndp120info.lower);
		if (ndp120 == NULL) {
			return ERROR;
		}

		snprintf(devname, sizeof(devname), "pcmC%uD%u%c", minor, 0, 'c');
		ret = audio_register(devname, ndp120);
		
		initialized = true;
	}

	return ret;
}
#endif

