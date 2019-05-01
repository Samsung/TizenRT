/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 ******************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <stdio.h>

#include <tinyara/irq.h>
#include <tinyara/wqueue.h>
#include <tinyara/wdog.h>
#include <tinyara/analog/adc.h>
#include <tinyara/analog/ioctl.h>

#include "chip/esp32_rtc_adc.h"
#include "esp32_adc.h"

#ifdef CONFIG_ESP32_ADC

#define ESP32_ADC_MAX_CHANNELS ADC1_CHANNEL_MAX

//Depends on TICK; Default is 100ms(10ms*10)!
#define ESP32_ADC_DEFAULT_PEROID  10
#define ESP32_ADC_MIN_PEROID    1
#define ESP32_ADC_MAX_PEROID    1000000

#define DEBUG_ADC	0
#undef adcinfo
#if defined(DEBUG_ADC) && DEBUG_ADC > 0
#define adcinfo(format, ...)   printf(format, ##__VA_ARGS__)
#else
#define adcinfo(format, ...)
#endif

#define CONVERT_ONECHANNEL		1
/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This structure describes the state of one ADC block */
struct esp32_dev_s {
	FAR const struct adc_callback_s *cb;

	struct adc_dev_s *dev;		/* A reference to the outer (parent) */
	uint8_t nchannels;			/* Number of channels */
	uint8_t cchannels;			/* Number of configured channels */
	uint8_t current;			/* Current ADC channel being converted */

	WDOG_ID work;				/* Supports the IRQ handling */
	adc_channel_t chanlist[ESP32_ADC_MAX_CHANNELS];
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#if !defined(CONFIG_ESP32_ADC_PEROID) || (CONFIG_ESP32_ADC_PEROID<ESP32_ADC_MIN_PEROID) || (CONFIG_ESP32_ADC_PEROID>ESP32_ADC_MAX_PEROID)
const int PeroidPerChannel = ESP32_ADC_DEFAULT_PEROID;
#else
const int PeroidPerChannel = CONFIG_ESP32_ADC_PEROID;
#endif

/****************************************************************************
 * Name: adc_conversion
 *
 * Description:
 *   This function executes on the worker thread.  It is scheduled by
 *   adc_interrupt whenever any enabled end-of-conversion event occurs.
 *   All interrupts are disabled when this function runs.
 *   adc_conversion will re-enable the interrupt when it completes
 *   processing the pending interrupt.
 *
 * Input Parameters
 *   arg - The ADC private data structure cast to (void *)
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void adc_conversion(int argc, uint32_t arg)
{
	uint16_t sample = 0;
	struct esp32_dev_s *priv = (struct esp32_dev_s *)arg;

#if defined(CONVERT_ONECHANNEL) && (0 < CONVERT_ONECHANNEL)
	/* Read the ADC sample and pass it to the upper-half */
	sample = adc1_get_raw((adc1_channel_t)priv->chanlist[priv->current]);

	adcinfo("[ADC] conversion %d %d: %d\n", priv->current, priv->chanlist[priv->current], sample);
	if (priv->cb != NULL && priv->cb->au_receive != NULL) {
		priv->cb->au_receive(priv->dev, priv->chanlist[priv->current], sample);
	}

	/* Set the next channel to be sampled */
	priv->current++;

	if (priv->current >= priv->nchannels) {
		priv->current = 0;
	}

	/*next conversion */
	wd_start(priv->work, PeroidPerChannel, (wdentry_t)&adc_conversion, 1, (uint32_t)priv);
#else
	if (priv->current >= priv->nchannels) {
		priv->current = 0;
	}

	while (priv->current < priv->nchannels) {
		/* Read the ADC sample and pass it to the upper-half */
		sample = adc1_get_raw((adc1_channel_t)priv->chanlist[priv->current]);

		adcinfo("[ADC] conversion %d %d: %d\n", priv->current, priv->chanlist[priv->current], sample);
		if (priv->cb != NULL && priv->cb->au_receive != NULL) {
			priv->cb->au_receive(priv->dev, priv->chanlist[priv->current], sample);
		}
		/* Set the next channel to be sampled */
		priv->current++;
	}
	priv->current = 0;

	wd_start(priv->work, PeroidPerChannel, (wdentry_t)&adc_conversion, 1, (uint32_t)priv);
#endif
}

/****************************************************************************
 * Name: adc_startconv
 *
 * Description:
 *   Start (or stop) the ADC conversion process
 *
 * Input Parameters:
 *   priv   - A reference to the ADC block status
 *   enable - True: Start conversion
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void adc_startconv(FAR struct esp32_dev_s *priv, bool enable)
{
	if (enable) {
		wd_cancel(priv->work);
		wd_start(priv->work, ESP32_ADC_MIN_PEROID, (wdentry_t)&adc_conversion, 1, (uint32_t)priv);
	} else {
		wd_cancel(priv->work);
	}
	adcinfo("[ADC] conv %d: %d\n", enable, get_errno());
}

/****************************************************************************
 * Name: adc_set_ch
 *
 * Description
 *   Sets the ADC channel
 *
 * Input Parameters:
 *   dev - pointer to device structure used by the drvier
 *   ch  - ADC channel number + 1 (0 is reserved for all configured channels.)
 *
 * Returned Value:
 *   int - errno
 *
 ****************************************************************************/
static int adc_set_ch(FAR struct adc_dev_s *dev, uint8_t ch)
{
	int i;
	FAR struct esp32_dev_s *priv = (FAR struct esp32_dev_s *)dev->ad_priv;

	if (ch == 0) {
		/*
		 * 0 is reserved for special purpose which samples
		 * all channels
		 */
		priv->current = 0;
		priv->nchannels = priv->cchannels;
	} else {
		/* REVISIT: changing channel is not supported for now */
		for (i = 0; i < priv->cchannels && priv->chanlist[i] != ch - 1; i++) ;

		if (i >= priv->cchannels) {
			return -ENODEV;
		}

		priv->current = i;
		priv->nchannels = 1;
	}

	adc1_config_channel_atten(priv->chanlist[priv->current], ADC_ATTEN_0db);

	return OK;
}

/****************************************************************************
 * Name: adc_bind
 *
 * Description:
 *   Bind the upper-half driver callbacks to the lower-half implementation.
 *   This must be called early in order to receive ADC event notifications.
 *
 ****************************************************************************/
static int adc_bind(FAR struct adc_dev_s *dev, FAR const struct adc_callback_s *callback)
{
	FAR struct esp32_dev_s *priv = (FAR struct esp32_dev_s *)dev->ad_priv;

	DEBUGASSERT(priv != NULL);
	priv->cb = callback;

	return OK;
}

/****************************************************************************
 * Name: adc_reset
 *
 * Description:
 *   Reset the ADC device.  Called early to initialize the hardware. This
 *   is called, before adc_setup() and on error conditions.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/
static void adc_reset(FAR struct adc_dev_s *dev)
{
	irqstate_t flags;

	flags = irqsave();

	/* Reset ADC */

	/* Release ADC from reset state */

	/* Configuration of the channel conversions */
	adc_set_ch(dev, 0);

	irqrestore(flags);
}

/****************************************************************************
 * Name: adc_setup
 *
 * Description:
 *   Configure the ADC. This method is called the first time that the ADC
 *   device is opened.  This will occur when the port is first opened.
 *   This setup includes configuring and attaching ADC interrupts.
 *   Interrupts are all disabled upon return.
 *
 * Input Parameters:
 *
 * Returned Value:
 ****************************************************************************/
static int adc_setup(FAR struct adc_dev_s *dev)
{
	int ret;
	FAR struct esp32_dev_s *priv = (FAR struct esp32_dev_s *)dev->ad_priv;

	/* Attach the ADC interrupt */

	/* Make sure that the ADC device is in the powered up, reset state */
	adc_reset(dev);

	/* Enable the ADC interrupt */
	//up_enable_irq(IRQ_ADC);

	adcinfo("[ADC] adc_setup: %d, %d...\n\t", priv->cchannels, priv->nchannels);
	for (ret = 0; ret < priv->nchannels; ret++) {
		adcinfo("%d ", priv->chanlist[ret]);
	}
	adcinfo("\n");

	ret = adc1_config_width(ADC_WIDTH_12Bit);
	adcinfo("[ADC] adc1_config_width:%d\n\n", ret);

	return OK;
}

/****************************************************************************
 * Name: adc_shutdown
 *
 * Description:
 *   Disable the ADC.  This method is called when the ADC device is closed.
 *   This method reverses the operation the setup method.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/
static void adc_shutdown(FAR struct adc_dev_s *dev)
{
	FAR struct esp32_dev_s *priv = (FAR struct esp32_dev_s *)dev->ad_priv;

	wd_cancel(priv->work);
	adc_power_off();
}

/****************************************************************************
 * Name: adc_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts.
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/
static void adc_rxint(FAR struct adc_dev_s *dev, bool enable)
{
	/* Enable or Disable ADC interrupt */
}

/****************************************************************************
 * Name: adc_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method.
 *
 * Input Parameters:
 *   dev - pointer to device structure used by the driver
 *   cmd - command
 *   arg - arguments passed with command
 *
 * Returned Value:
 *
 ****************************************************************************/
static int adc_ioctl(FAR struct adc_dev_s *dev, int cmd, unsigned long arg)
{
	FAR struct esp32_dev_s *priv = (FAR struct esp32_dev_s *)dev->ad_priv;
	int ret = OK;

	switch (cmd) {
	case ANIOC_TRIGGER:
		adc_startconv(priv, true);
		break;

	default:
		ret = -ENOTTY;
		break;
	}

	return ret;
}

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct adc_ops_s g_adcops = {
	.ao_bind = adc_bind,
	.ao_reset = adc_reset,
	.ao_setup = adc_setup,
	.ao_shutdown = adc_shutdown,
	.ao_rxint = adc_rxint,
	.ao_ioctl = adc_ioctl,
};

static struct esp32_dev_s g_adcpriv = {
	.cb = NULL,
	.current = 0,
};

static struct adc_dev_s g_adcdev;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: esp32_adc_initialize
 *
 * Description:
 *   Initialize the ADC. As the pins of each ADC channel are exported through
 *   configurable GPIO and it is board-specific, information on available
 *   ADC channels should be passed to esp32_adc_initialize().
 *
 * Input Parameters:
 *   chanlist  - The list of channels
 *   cchannels - Number of channels
 *
 * Returned Value:
 *   Valid ADC device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/
struct adc_dev_s *esp32_adc_initialize(FAR const adc_channel_t *chanlist, int cchannels)
{
	FAR struct esp32_dev_s *priv = &g_adcpriv;

	/* Initialize the public ADC device data structure */
	g_adcdev.ad_ops = &g_adcops;
	g_adcdev.ad_priv = priv;

	/* Initialize the private ADC device data structure */
	priv->cb = NULL;
	priv->dev = &g_adcdev;
	priv->cchannels = cchannels;

	if (cchannels > ESP32_ADC_MAX_CHANNELS) {
		lldbg("ESP32 ADC1 has maximum %d ADC channels.\n", ESP32_ADC_MAX_CHANNELS);
		return NULL;
	}

	memcpy(priv->chanlist, chanlist, cchannels * sizeof(adc_channel_t));
	adcinfo("ESP32 ADC channel count %d\n", cchannels);

	adc1_config_width(ADC_WIDTH_12Bit);

	priv->work = wd_create();

	return &g_adcdev;
}
#endif							/* CONFIG_ESP32_ADC */
