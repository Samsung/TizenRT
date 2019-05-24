/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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

#include "imxrt_clock.h"
#include "imxrt_adc.h"

#ifdef CONFIG_IMXRT_ADC

#define IMXRT_ADC_MAX_CHANNELS 10

//Depends on TICK; Default is 100ms(10ms*10)!
#define IMXRT_ADC_DEFAULT_PEROID  10
#define IMXRT_ADC_MIN_PEROID    1
#define IMXRT_ADC_MAX_PEROID    1000000

#define CONVERT_ONECHANNEL		1
#define ADC_CHANNEL_GROUP       0
/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This structure describes the state of one ADC block */
struct imxrt_dev_s {
	FAR const struct adc_callback_s *cb;

	struct adc_dev_s *dev;		/* A reference to the outer (parent) */
	uint8_t nchannels;			/* Number of channels */
	uint8_t cchannels;			/* Number of configured channels */
	uint8_t current;			/* Current ADC channel being converted */

	WDOG_ID work;				/* Supports the IRQ handling */
	uint8_t chanlist[IMXRT_ADC_MAX_CHANNELS];
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get instance number for ADC module.
 *
 * @param base ADC peripheral base address
 */
static uint32_t imxrt_adc_getinstance(ADC_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to ADC bases for each instance. */
static ADC_Type *const s_adcBases[] = ADC_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to ADC clocks for each instance. */
static const clock_ip_name_t s_adcClocks[] = ADC_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#if !defined(CONFIG_IMXRT_ADC_PEROID) || (CONFIG_IMXRT_ADC_PEROID < IMXRT_ADC_MIN_PEROID) || (CONFIG_IMXRT_ADC_PEROID > IMXRT_ADC_MAX_PEROID)
const int PeroidPerChannel = IMXRT_ADC_DEFAULT_PEROID;
#else
const int PeroidPerChannel = CONFIG_IMXRT_ADC_PEROID;
#endif

static uint32_t imxrt_adc_getinstance(ADC_Type *base)
{
	uint32_t instance;

	/* Find the instance index from base address mappings. */
	for (instance = 0; instance < ARRAY_SIZE(s_adcBases); instance++) {
		if (s_adcBases[instance] == base) {
			break;
		}
	}

	assert(instance < ARRAY_SIZE(s_adcBases));

	return instance;
}

/*!
 * brief Initialize the ADC module.
 *
 * param base ADC peripheral base address.
 * param config Pointer to "adc_config_t" structure.
 */
void imxrt_adc_init(ADC_Type *base, const adc_config_t *config)
{
	assert(NULL != config);

	uint32_t tmp32;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Enable the clock. */
	imxrt_clock_enableclock(s_adcClocks[imxrt_adc_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
	/* ADCx_CFG */
	tmp32 = base->CFG & (ADC_CFG_AVGS_MASK | ADC_CFG_ADTRG_MASK); /* Reserve AVGS and ADTRG bits. */
	tmp32 |= ADC_CFG_REFSEL(config->referenceVoltageSource) | ADC_CFG_ADSTS(config->samplePeriodMode) |
				ADC_CFG_ADICLK(config->clockSource) | ADC_CFG_ADIV(config->clockDriver) | ADC_CFG_MODE(config->resolution);
	if (config->enableOverWrite) {
		tmp32 |= ADC_CFG_OVWREN_MASK;
	}
	if (config->enableLongSample) {
		tmp32 |= ADC_CFG_ADLSMP_MASK;
	}
	if (config->enableLowPower) {
		tmp32 |= ADC_CFG_ADLPC_MASK;
	}
	if (config->enableHighSpeed) {
		tmp32 |= ADC_CFG_ADHSC_MASK;
	}
	base->CFG = tmp32;

	/* ADCx_GC  */
	tmp32 = base->GC & ~(ADC_GC_ADCO_MASK | ADC_GC_ADACKEN_MASK);
	if (config->enableContinuousConversion) {
		tmp32 |= ADC_GC_ADCO_MASK;
	}
	if (config->enableAsynchronousClockOutput) {
		tmp32 |= ADC_GC_ADACKEN_MASK;
	}
	base->GC = tmp32;
}

/*!
 * brief De-initializes the ADC module.
 *
 * param base ADC peripheral base address.
 */
void imxrt_adc_deinit(ADC_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
	/* Disable the clock. */
	imxrt_clock_disableclock(s_adcClocks[imxrt_adc_getinstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/*!
 * brief Gets an available pre-defined settings for the converter's configuration.
 *
 * This function initializes the converter configuration structure with available settings. The default values are:
 * code
 *  config->enableAsynchronousClockOutput = true;
 *  config->enableOverWrite =               false;
 *  config->enableContinuousConversion =    false;
 *  config->enableHighSpeed =               false;
 *  config->enableLowPower =                false;
 *  config->enableLongSample =              false;
 *  config->referenceVoltageSource =        kADC_ReferenceVoltageSourceAlt0;
 *  config->samplePeriodMode =              kADC_SamplePeriod2or12Clocks;
 *  config->clockSource =                   kADC_ClockSourceAD;
 *  config->clockDriver =                   kADC_ClockDriver1;
 *  config->resolution =                    kADC_Resolution12Bit;
 * endcode
 * param base   ADC peripheral base address.
 * param config Pointer to the configuration structure.
 */
void imxrt_adc_getdefaultconfig(adc_config_t *config)
{
	assert(NULL != config);

	/* Initializes the configure structure to zero. */
	memset(config, 0, sizeof(*config));

	config->enableAsynchronousClockOutput = true;
	config->enableOverWrite = false;
	config->enableContinuousConversion = false;
	config->enableHighSpeed = false;
	config->enableLowPower = false;
	config->enableLongSample = false;
	config->referenceVoltageSource = kADC_ReferenceVoltageSourceAlt0;
	config->samplePeriodMode = kADC_SamplePeriod2or12Clocks;
	config->clockSource = kADC_ClockSourceAD;
	config->clockDriver = kADC_ClockDriver1;
	config->resolution = kADC_Resolution12Bit;
}

/*!
 * brief Configures the conversion channel.
 *
 * This operation triggers the conversion when in software trigger mode. When in hardware trigger mode, this API
 * configures the channel while the external trigger source helps to trigger the conversion.
 *
 * Note that the "Channel Group" has a detailed description.
 * To allow sequential conversions of the ADC to be triggered by internal peripherals, the ADC has more than one
 * group of status and control registers, one for each conversion. The channel group parameter indicates which group of
 * registers are used, for example channel group 0 is for Group A registers and channel group 1 is for Group B
 * registers. The
 * channel groups are used in a "ping-pong" approach to control the ADC operation.  At any point, only one of
 * the channel groups is actively controlling ADC conversions. The channel group 0 is used for both software and
 * hardware
 * trigger modes. Channel groups 1 and greater indicate potentially multiple channel group registers for
 * use only in hardware trigger mode. See the chip configuration information in the appropriate MCU reference manual
 * about the
 * number of SC1n registers (channel groups) specific to this device.  None of the channel groups 1 or greater are used
 * for software trigger operation. Therefore, writing to these channel groups does not initiate a new conversion.
 * Updating the channel group 0 while a different channel group is actively controlling a conversion is allowed and
 * vice versa. Writing any of the channel group registers while that specific channel group is actively controlling a
 * conversion aborts the current conversion.
 *
 * param base          ADC peripheral base address.
 * param channelGroup  Channel group index.
 * param config        Pointer to the "adc_channel_config_t" structure for the conversion channel.
 */
void imxrt_adc_setchannelconfig(ADC_Type *base, uint32_t channelGroup, const adc_channel_config_t *config)
{
	assert(NULL != config);
	assert(channelGroup < ADC_HC_COUNT);

	uint32_t tmp32;

	tmp32 = ADC_HC_ADCH(config->channelNumber);
	if (config->enableInterruptOnConversionCompleted) {
		tmp32 |= ADC_HC_AIEN_MASK;
	}
	base->HC[channelGroup] = tmp32;
}

/*
 *To complete calibration, the user must follow the below procedure:
 *  1. Configure ADC_CFG with actual operating values for maximum accuracy.
 *  2. Configure the ADC_GC values along with CAL bit.
 *  3. Check the status of CALF bit in ADC_GS and the CAL bit in ADC_GC.
 *  4. When CAL bit becomes '0' then check the CALF status and COCO[0] bit status.
 */
/*!
 * brief  Automates the hardware calibration.
 *
 * This auto calibration helps to adjust the plus/minus side gain automatically.
 * Execute the calibration before using the converter. Note that the software trigger should be used
 * during calibration.
 *
 * param  base ADC peripheral base address.
 *
 * return                 Execution status.
 * retval kStatus_Success Calibration is done successfully.
 * retval kStatus_Fail    Calibration has failed.
 */
status_t imxrt_adc_doautocalibration(ADC_Type *base)
{
	status_t status = kStatus_Success;
#if !(defined(FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE) && FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE)
	bool bHWTrigger = false;

	/* The calibration would be failed when in hardwar mode.
	 * Remember the hardware trigger state here and restore it later if the hardware trigger is enabled.*/
	if (0U != (ADC_CFG_ADTRG_MASK & base->CFG)) {
		bHWTrigger = true;
		imxrt_adc_enablehardwaretrigger(base, false);
	}
#endif

	/* Clear the CALF and launch the calibration. */
	base->GS = ADC_GS_CALF_MASK; /* Clear the CALF. */
	base->GC |= ADC_GC_CAL_MASK; /* Launch the calibration. */

	/* Check the status of CALF bit in ADC_GS and the CAL bit in ADC_GC. */
	while (0U != (base->GC & ADC_GC_CAL_MASK)) {
		/* Check the CALF when the calibration is active. */
		if (0U != (imxrt_adc_getstatusflags(base) & kADC_CalibrationFailedFlag)) {
			status = kStatus_Fail;
			break;
		}
	}

	/* When CAL bit becomes '0' then check the CALF status and COCO[0] bit status. */
	if (0U == imxrt_adc_getchannelstatusflags(base, 0U)) { /* Check the COCO[0] bit status. */
		status = kStatus_Fail;
	}
	if (0U != (imxrt_adc_getstatusflags(base) & kADC_CalibrationFailedFlag)) { /* Check the CALF status. */
		status = kStatus_Fail;
	}

	/* Clear conversion done flag. */
	imxrt_adc_getchannelconversionvalue(base, 0U);

#if !(defined(FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE) && FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE)
	/* Restore original trigger mode. */
	if (true == bHWTrigger) {
		imxrt_adc_enablehardwaretrigger(base, true);
	}
#endif

	return status;
}

/*!
 * brief Set user defined offset.
 *
 * param base   ADC peripheral base address.
 * param config Pointer to "adc_offest_config_t" structure.
 */
void imxrt_adc_setoffsetconfig(ADC_Type *base, const adc_offest_config_t *config)
{
	assert(NULL != config);

	uint32_t tmp32;

	tmp32 = ADC_OFS_OFS(config->offsetValue);
	if (config->enableSigned) {
		tmp32 |= ADC_OFS_SIGN_MASK;
	}
	base->OFS = tmp32;
}

/*!
 * brief Configures the hardware compare mode.
 *
 * The hardware compare mode provides a way to process the conversion result automatically by using hardware. Only the
 * result
 * in the compare range is available. To compare the range, see "adc_hardware_compare_mode_t" or the appopriate
 * reference
 * manual for more information.
 *
 * param base ADC peripheral base address.
 * param Pointer to "adc_hardware_compare_config_t" structure.
 *
 */
void imxrt_adc_sethardwarecompareconfig(ADC_Type *base, const adc_hardware_compare_config_t *config)
{
	uint32_t tmp32;

	tmp32 = base->GC & ~(ADC_GC_ACFE_MASK | ADC_GC_ACFGT_MASK | ADC_GC_ACREN_MASK);
	if (NULL == config) {/* Pass "NULL" to disable the feature. */
		base->GC = tmp32;
		return;
	}
	/* Enable the feature. */
	tmp32 |= ADC_GC_ACFE_MASK;

	/* Select the hardware compare working mode. */
	switch (config->hardwareCompareMode) {
	case kADC_HardwareCompareMode0:
		break;
	case kADC_HardwareCompareMode1:
		tmp32 |= ADC_GC_ACFGT_MASK;
		break;
	case kADC_HardwareCompareMode2:
		tmp32 |= ADC_GC_ACREN_MASK;
		break;
	case kADC_HardwareCompareMode3:
		tmp32 |= ADC_GC_ACFGT_MASK | ADC_GC_ACREN_MASK;
		break;
	default:
		break;
	}
	base->GC = tmp32;

	/* Load the compare values. */
	tmp32 = ADC_CV_CV1(config->value1) | ADC_CV_CV2(config->value2);
	base->CV = tmp32;
}

/*!
 * brief Configures the hardware average mode.
 *
 * The hardware average mode provides a way to process the conversion result automatically by using hardware. The
 * multiple
 * conversion results are accumulated and averaged internally making them easier to read.
 *
 * param base ADC peripheral base address.
 * param mode Setting the hardware average mode. See "adc_hardware_average_mode_t".
 */
void imxrt_adc_sethardwareaverageconfig(ADC_Type *base, adc_hardware_average_mode_t mode)
{
	uint32_t tmp32;

	if (mode == kADC_HardwareAverageDiasable) {
		base->GC &= ~ADC_GC_AVGE_MASK;
	} else {
		tmp32 = base->CFG & ~ADC_CFG_AVGS_MASK;
		tmp32 |= ADC_CFG_AVGS(mode);
		base->CFG = tmp32;
		base->GC |= ADC_GC_AVGE_MASK; /* Enable the hardware compare. */
	}
}

/*!
 * brief Clears the converter's status falgs.
 *
 * param base ADC peripheral base address.
 * param mask Mask value for the cleared flags. See "adc_status_flags_t".
 */
void imxrt_adc_clearstatusflags(ADC_Type *base, uint32_t mask)
{
	uint32_t tmp32 = 0;

	if (0U != (mask & kADC_CalibrationFailedFlag)) {
		tmp32 |= ADC_GS_CALF_MASK;
	}
	if (0U != (mask & kADC_ConversionActiveFlag)) {
		tmp32 |= ADC_GS_ADACT_MASK;
	}
	base->GS = tmp32;
}

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
	struct imxrt_dev_s *priv = (struct imxrt_dev_s *)arg;

#if defined(CONVERT_ONECHANNEL) && (0 < CONVERT_ONECHANNEL)
	/* Read the ADC sample and pass it to the upper-half */
	while (0U == imxrt_adc_getchannelstatusflags(ADC1, ADC_CHANNEL_GROUP)) {
	}
	sample = imxrt_adc_getchannelconversionvalue(ADC1, ADC_CHANNEL_GROUP);

	lldbg("[ADC] conversion %d %d: %d\n", priv->current, priv->chanlist[priv->current], sample);
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
		while (0U == imxrt_adc_getchannelstatusflags(ADC1, ADC_CHANNEL_GROUP)) {
		}
		sample = imxrt_adc_getchannelconversionvalue(ADC1, ADC_CHANNEL_GROUP);

		lldbg("[ADC] conversion %d %d: %d\n", priv->current, priv->chanlist[priv->current], sample);
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
static void adc_startconv(FAR struct imxrt_dev_s *priv, bool enable)
{
	if (enable) {
		#if !(defined(FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE) && FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE)
		imxrt_adc_enablehardwaretrigger(ADC1, false);
		#endif

		/* Do auto hardware calibration. */
		if (kStatus_Success == imxrt_adc_doautocalibration(ADC1)) {
			lldbg("imxrt_adc_doautocalibration() Done.\r\n");
		} else {
			lldbg("imxrt_adc_doautocalibration() Failed.\r\n");
		}

		wd_cancel(priv->work);
		wd_start(priv->work, IMXRT_ADC_MIN_PEROID, (wdentry_t)&adc_conversion, 1, (uint32_t)priv);
	} else {
		wd_cancel(priv->work);
	}
	lldbg("[ADC] conv %d: %d\n", enable, get_errno());
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
	adc_channel_config_t adcChannelConfigStruct;
	FAR struct imxrt_dev_s *priv = (FAR struct imxrt_dev_s *)dev->ad_priv;

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

		/* Configure the user channel and interrupt. */
	adcChannelConfigStruct.channelNumber = priv->current;
	adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;
	imxrt_adc_setchannelconfig(ADC1, 0, &adcChannelConfigStruct);

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
	FAR struct imxrt_dev_s *priv = (FAR struct imxrt_dev_s *)dev->ad_priv;

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
	adc_config_t adcConfigStrcut;
	adc_channel_config_t adcChannelConfigStruct;

	FAR struct imxrt_dev_s *priv = (FAR struct imxrt_dev_s *)dev->ad_priv;

	/* Attach the ADC interrupt */

	/* Make sure that the ADC device is in the powered up, reset state */
	adc_reset(dev);

	/* Enable the ADC interrupt */
	//up_enable_irq(IRQ_ADC);

	lldbg("[ADC] adc_setup: %d, %d...\n\t", priv->cchannels, priv->nchannels);
	for (ret = 0; ret < priv->nchannels; ret++) {
		lldbg("%d ", priv->chanlist[ret]);
	}
	lldbg("\n");

	imxrt_adc_getdefaultconfig(&adcConfigStrcut);
	imxrt_adc_init(ADC1, &adcConfigStrcut);

	lldbg("[ADC] adc1_config_width:%d\n\n", ret);

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
	FAR struct imxrt_dev_s *priv = (FAR struct imxrt_dev_s *)dev->ad_priv;

	wd_cancel(priv->work);
	imxrt_adc_deinit(ADC1);
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
	FAR struct imxrt_dev_s *priv = (FAR struct imxrt_dev_s *)dev->ad_priv;
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

static struct imxrt_dev_s g_adcpriv = {
	.cb = NULL,
	.current = 0,
};

static struct adc_dev_s g_adcdev;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: imxrt_adc_initialize
 *
 * Description:
 *   Initialize the ADC. As the pins of each ADC channel are exported through
 *   configurable GPIO and it is board-specific, information on available
 *   ADC channels should be passed to imxrt_adc_initialize().
 *
 * Input Parameters:
 *   chanlist  - The list of channels
 *   cchannels - Number of channels
 *
 * Returned Value:
 *   Valid ADC device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/
struct adc_dev_s *imxrt_adc_initialize(FAR const uint8_t *chanlist, int cchannels)
{
	FAR struct imxrt_dev_s *priv = &g_adcpriv;

	/* Initialize the public ADC device data structure */
	g_adcdev.ad_ops = &g_adcops;
	g_adcdev.ad_priv = priv;

	/* Initialize the private ADC device data structure */
	priv->cb = NULL;
	priv->dev = &g_adcdev;
	priv->cchannels = cchannels;

	if (cchannels > IMXRT_ADC_MAX_CHANNELS) {
		lldbg("IMXRT ADC1 has maximum %d ADC channels.\n", IMXRT_ADC_MAX_CHANNELS);
		return NULL;
	}

	memcpy(priv->chanlist, chanlist, cchannels * sizeof(uint8_t));
	lldbg("IMXRT ADC channel count %d\n", cchannels);

	priv->work = wd_create();

	return &g_adcdev;
}
#endif							/* CONFIG_IMXRT_ADC */
