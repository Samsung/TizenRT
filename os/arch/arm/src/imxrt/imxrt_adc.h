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

#ifndef __ARCH_ARM_SRC_IMXRT_IMXRT_ADC_H__
#define __ARCH_ARM_SRC_IMXRT_IMXRT_ADC_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include "imxrt_config.h"
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#include "chip/imxrt102x_config.h"
#include "chip/imxrt102x_features.h"
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#include "chip/imxrt105x_config.h"
#include "chip/imxrt105x_features.h"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/*!
 *  @addtogroup adc_12b1msps_sar
 *  @{
 */

/*******************************************************************************
* Definitions
******************************************************************************/
#if defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT102x)
#define IMXRT_ADC1_IN0		IOMUXC_GPIO_AD_B0_12_GPIO1_IO12
#define IMXRT_ADC1_IN1		IOMUXC_GPIO_AD_B0_14_GPIO1_IO14
#define IMXRT_ADC1_IN2		IOMUXC_GPIO_AD_B0_15_GPIO1_IO15
#define IMXRT_ADC1_IN3		IOMUXC_GPIO_AD_B1_01_GPIO1_IO17
#define IMXRT_ADC1_IN4		IOMUXC_GPIO_AD_B1_03_GPIO1_IO19
#define IMXRT_ADC1_IN5		IOMUXC_GPIO_AD_B1_05_GPIO1_IO21
#define IMXRT_ADC1_IN6		IOMUXC_GPIO_AD_B1_06_GPIO1_IO22
#define IMXRT_ADC1_IN7		IOMUXC_GPIO_AD_B1_07_GPIO1_IO23
#define IMXRT_ADC1_IN8		IOMUXC_GPIO_AD_B1_08_GPIO1_IO24
#define IMXRT_ADC1_IN9		IOMUXC_GPIO_AD_B1_09_GPIO1_IO25
#define IMXRT_ADC1_IN10		IOMUXC_GPIO_AD_B1_10_GPIO1_IO26
#define IMXRT_ADC1_IN11		IOMUXC_GPIO_AD_B1_11_GPIO1_IO27
#define IMXRT_ADC1_IN12		IOMUXC_GPIO_AD_B1_12_GPIO1_IO28
#define IMXRT_ADC1_IN13		IOMUXC_GPIO_AD_B1_13_GPIO1_IO29
#define IMXRT_ADC1_IN14		IOMUXC_GPIO_AD_B1_14_GPIO1_IO30
#define IMXRT_ADC1_IN15		IOMUXC_GPIO_AD_B1_15_GPIO1_IO31
#elif defined(CONFIG_ARCH_CHIP_FAMILY_IMXRT105x)
#define IMXRT_ADC1_IN0		IOMUXC_GPIO_AD_B1_11_GPIO1_IO27
#define IMXRT_ADC1_IN1		IOMUXC_GPIO_AD_B0_12_GPIO1_IO12
#define IMXRT_ADC1_IN2		IOMUXC_GPIO_AD_B0_13_GPIO1_IO13
#define IMXRT_ADC1_IN3		IOMUXC_GPIO_AD_B0_14_GPIO1_IO14
#define IMXRT_ADC1_IN4		IOMUXC_GPIO_AD_B0_15_GPIO1_IO15
#define IMXRT_ADC1_IN5		IOMUXC_GPIO_AD_B1_00_GPIO1_IO16
#define IMXRT_ADC1_IN6		IOMUXC_GPIO_AD_B1_01_GPIO1_IO17
#define IMXRT_ADC1_IN7		IOMUXC_GPIO_AD_B1_02_GPIO1_IO18
#define IMXRT_ADC1_IN8		IOMUXC_GPIO_AD_B1_03_GPIO1_IO19
#define IMXRT_ADC1_IN9		IOMUXC_GPIO_AD_B1_04_GPIO1_IO20
#define IMXRT_ADC1_IN10		IOMUXC_GPIO_AD_B1_05_GPIO1_IO21
#define IMXRT_ADC1_IN11		IOMUXC_GPIO_AD_B1_06_GPIO1_IO22
#define IMXRT_ADC1_IN12		IOMUXC_GPIO_AD_B1_07_GPIO1_IO23
#define IMXRT_ADC1_IN13		IOMUXC_GPIO_AD_B1_08_GPIO1_IO24
#define IMXRT_ADC1_IN14		IOMUXC_GPIO_AD_B1_09_GPIO1_IO25
#define IMXRT_ADC1_IN15		IOMUXC_GPIO_AD_B1_10_GPIO1_IO26
#endif

/*! @brief ADC driver version */
#define FSL_ADC_DRIVER_VERSION (MAKE_VERSION(2, 0, 1)) /*!< Version 2.0.1. */

#if 0
typedef enum {
	ADC_CHANNEL_0 = 0,		/*!< ADC channel */
	ADC_CHANNEL_1,			/*!< ADC channel */
	ADC_CHANNEL_2,			/*!< ADC channel */
	ADC_CHANNEL_3,			/*!< ADC channel */
	ADC_CHANNEL_4,			/*!< ADC channel */
	ADC_CHANNEL_5,			/*!< ADC channel */
	ADC_CHANNEL_6,			/*!< ADC channel */
	ADC_CHANNEL_7,			/*!< ADC channel */
	ADC_CHANNEL_8,			/*!< ADC channel */
	ADC_CHANNEL_9,			/*!< ADC channel */
	ADC_CHANNEL_MAX,
} adc_channel_t;
#endif

/********************************************************************************************
 * Pre-processor Definitions
 ********************************************************************************************/
/* ADC Port Number
 *
 *               3322 2222 2222 1111  1111 1100 0000 0000
 *               1098 7654 3210 9876  5432 1098 7654 3210
 *   ADC       00.. ...G GGG. ....  .... .... .... ....
 */

#define ADC_BUS_SHIFT       (21)      /* Bits 21-23: GPIO port index */
#define ADC_BUS_MASK        (15 << ADC_BUS_SHIFT)

#define ADC_ADC1            (0 << ADC_BUS_SHIFT)	/* ADC Bus 0 */
#define ADC_ADC2            (1 << ADC_BUS_SHIFT)	/* ADC Bus 1 */
#define IMXRT_ADC_NBUS		2

/* ADC Channel Number:
 *
 *               3322 2222 2222 1111  1111 1100 0000 0000
 *               1098 7654 3210 9876  5432 1098 7654 3210
 *   ADC         00.. .... ...P PPPP  .... .... .... ....
 */

/////////////////////////////////////////////////////////////////////////////
// Chip/imxrt_adc.h ??
#define ADC1INDEX                     0	/* Channel 1 index */
#define ADC2INDEX                     1	/* Channel 2 index */
#define ADC3INDEX                     2	/* Channel 3 index */
#define ADC4INDEX                     3	/* Channel 4 index */
#define ADC5INDEX                     4	/* Channel 5 index */
#define ADC6INDEX                     5	/* Channel 6 index */
#define ADC7INDEX                     6	/* Channel 7 index */
#define ADC8INDEX                     7	/* Channel 8 index */
#define ADC9INDEX                     8	/* Channel 9 index */
#define ADC10INDEX                    9	/* Channel 9 index */
#define ADC11INDEX                    10	/* Channel 10 index */
#define ADC12INDEX                    11	/* Channel 11 index */
#define ADC13INDEX                    12	/* Channel 12 index */
#define ADC14INDEX                    13	/* Channel 13 index */
#define ADC15INDEX                    14	/* Channel 14 index */
#define IMXRT_ADC_NCHANEL		ADC15INDEX

/////////////////////////////////////////////////////////////////////////////


#define ADC_CHANNEL_SHIFT         (16)	/* Bits 16-20: GPIO pin number */
#define ADC_CHANNEL_MASK          (31 << GPIO_PIN_SHIFT)

#define ADC_CHANNEL1           (ADC1INDEX << ADC_CHANNEL_SHIFT) /* ADC1 */
#define ADC_CHANNEL2           (ADC2INDEX << ADC_CHANNEL_SHIFT) /* ADC2 */
#define ADC_CHANNEL3           (ADC3INDEX << ADC_CHANNEL_SHIFT) /* ADC3 */
#define ADC_CHANNEL4           (ADC4INDEX << ADC_CHANNEL_SHIFT) /* ADC4 */
#define ADC_CHANNEL5           (ADC5INDEX << ADC_CHANNEL_SHIFT) /* ADC5 */
#define ADC_CHANNEL6           (ADC6INDEX << ADC_CHANNEL_SHIFT) /* ADC6 */
#define ADC_CHANNEL7           (ADC7INDEX << ADC_CHANNEL_SHIFT) /* ADC7 */
#define ADC_CHANNEL8           (ADC8INDEX << ADC_CHANNEL_SHIFT) /* ADC8 */
#define ADC_CHANNEL9           (ADC9INDEX << ADC_CHANNEL_SHIFT) /* ADC9 */
#define ADC_CHANNEL10           (ADC10INDEX << ADC_CHANNEL_SHIFT) /* ADC10 */
#define ADC_CHANNEL11           (ADC11INDEX << ADC_CHANNEL_SHIFT) /* ADC11 */
#define ADC_CHANNEL12           (ADC12INDEX << ADC_CHANNEL_SHIFT) /* ADC12 */
#define ADC_CHANNEL13           (ADC13INDEX << ADC_CHANNEL_SHIFT) /* ADC13 */
#define ADC_CHANNEL14           (ADC14INDEX << ADC_CHANNEL_SHIFT) /* ADC14 */
#define ADC_CHANNEL15           (ADC15INDEX << ADC_CHANNEL_SHIFT) /* ADC15 */



/*!
 * @brief Converter's status flags.
 */
typedef enum _adc_status_flags {
	kADC_ConversionActiveFlag = ADC_GS_ADACT_MASK, /*!< Conversion is active,not support w1c. */
	kADC_CalibrationFailedFlag = ADC_GS_CALF_MASK, /*!< Calibration is failed,support w1c. */
	kADC_AsynchronousWakeupInterruptFlag =
		ADC_GS_AWKST_MASK, /*!< Asynchronous wakeup interrupt occured, support w1c. */
} adc_status_flags_t;

/*!
 * @brief Reference voltage source.
 */
typedef enum _adc_reference_voltage_source {
	kADC_ReferenceVoltageSourceAlt0 = 0U, /*!< For external pins pair of VrefH and VrefL. */
} adc_reference_voltage_source_t;

/*!
 * @brief Sample time duration.
 */
typedef enum _adc_sample_period_mode {
	/* This group of enumeration is for internal use which is related to register setting. */
	kADC_SamplePeriod2or12Clocks = 0U, /*!< Long sample 12 clocks or short sample 2 clocks. */
	kADC_SamplePeriod4or16Clocks = 1U, /*!< Long sample 16 clocks or short sample 4 clocks. */
	kADC_SamplePeriod6or20Clocks = 2U, /*!< Long sample 20 clocks or short sample 6 clocks. */
	kADC_SamplePeriod8or24Clocks = 3U, /*!< Long sample 24 clocks or short sample 8 clocks. */
	/* This group of enumeration is for a public user. */
	/* For long sample mode. */
	kADC_SamplePeriodLong12Clcoks = kADC_SamplePeriod2or12Clocks, /*!< Long sample 12 clocks. */
	kADC_SamplePeriodLong16Clcoks = kADC_SamplePeriod4or16Clocks, /*!< Long sample 16 clocks. */
	kADC_SamplePeriodLong20Clcoks = kADC_SamplePeriod6or20Clocks, /*!< Long sample 20 clocks. */
	kADC_SamplePeriodLong24Clcoks = kADC_SamplePeriod8or24Clocks, /*!< Long sample 24 clocks. */
	/* For short sample mode. */
	kADC_SamplePeriodShort2Clocks = kADC_SamplePeriod2or12Clocks, /*!< Short sample 2 clocks. */
	kADC_SamplePeriodShort4Clocks = kADC_SamplePeriod4or16Clocks, /*!< Short sample 4 clocks. */
	kADC_SamplePeriodShort6Clocks = kADC_SamplePeriod6or20Clocks, /*!< Short sample 6 clocks. */
	kADC_SamplePeriodShort8Clocks = kADC_SamplePeriod8or24Clocks, /*!< Short sample 8 clocks. */
} adc_sample_period_mode_t;

/*!
 * @brief Clock source.
 */
typedef enum _adc_clock_source {
	kADC_ClockSourceIPG = 0U,     /*!< Select IPG clock to generate ADCK. */
	kADC_ClockSourceIPGDiv2 = 1U, /*!< Select IPG clock divided by 2 to generate ADCK. */
#if !(defined(FSL_FEATURE_ADC_SUPPORT_ALTCLK_REMOVE) && FSL_FEATURE_ADC_SUPPORT_ALTCLK_REMOVE)
	kADC_ClockSourceALT = 2U, /*!< Select alternate clock to generate ADCK. */
#endif
	kADC_ClockSourceAD = 3U, /*!< Select Asynchronous clock to generate ADCK. */
} adc_clock_source_t;

/*!
 * @brief Clock divider for the converter.
 */
typedef enum _adc_clock_drvier {
	kADC_ClockDriver1 = 0U, /*!< For divider 1 from the input clock to the module. */
	kADC_ClockDriver2 = 1U, /*!< For divider 2 from the input clock to the module. */
	kADC_ClockDriver4 = 2U, /*!< For divider 4 from the input clock to the module. */
	kADC_ClockDriver8 = 3U, /*!< For divider 8 from the input clock to the module. */
} adc_clock_driver_t;

/*!
 * @brief Converter's resolution.
 */
typedef enum _adc_resolution {
	kADC_Resolution8Bit = 0U,  /*!< Single End 8-bit resolution. */
	kADC_Resolution10Bit = 1U, /*!< Single End 10-bit resolution. */
	kADC_Resolution12Bit = 2U, /*!< Single End 12-bit resolution. */
} adc_resolution_t;

/*!
 * @brief Converter hardware compare mode.
 */
typedef enum _adc_hardware_compare_mode {
	kADC_HardwareCompareMode0 = 0U, /*!< Compare true if the result is less than the value1. */
	kADC_HardwareCompareMode1 = 1U, /*!< Compare true if the result is greater than or equal to value1. */
	kADC_HardwareCompareMode2 = 2U, /*!< Value1 <= Value2, compare true if the result is less than value1 Or
															the result is Greater than value2.
											Value1 >  Value2, compare true if the result is less than value1 And the
															result is greater than value2*/
	kADC_HardwareCompareMode3 = 3U, /*!< Value1 <= Value2, compare true if the result is greater than or equal
															to value1 And the result is less than or equal to value2.
											Value1 >  Value2, compare true if the result is greater than or equal to
															value1 Or the result is less than or equal to value2. */
} adc_hardware_compare_mode_t;

/*!
 * @brief Converter hardware average mode.
 */
typedef enum _adc_hardware_average_mode {
	kADC_HardwareAverageCount4 = 0U,   /*!< For hardware average with 4 samples. */
	kADC_HardwareAverageCount8 = 1U,   /*!< For hardware average with 8 samples. */
	kADC_HardwareAverageCount16 = 2U,  /*!< For hardware average with 16 samples. */
	kADC_HardwareAverageCount32 = 3U,  /*!< For hardware average with 32 samples. */
	kADC_HardwareAverageDiasable = 4U, /*!< Disable the hardware average function. */
} adc_hardware_average_mode_t;

/*!
 * @brief Converter configuration.
 */
typedef struct _adc_config {
	bool enableOverWrite;                                  /*!< Enable the overwriting. */
	bool enableContinuousConversion;                       /*!< Enable the continuous conversion mode. */
	bool enableHighSpeed;                                  /*!< Enable the high-speed mode. */
	bool enableLowPower;                                   /*!< Enable the low power mode. */
	bool enableLongSample;                                 /*!< Enable the long sample mode. */
	bool enableAsynchronousClockOutput;                    /*!< Enable the asynchronous clock output. */
	adc_reference_voltage_source_t referenceVoltageSource; /*!< Select the reference voltage source. */
	adc_sample_period_mode_t samplePeriodMode; /*!< Select the sample period in long sample mode or short mode. */
	adc_clock_source_t clockSource; /*!< Select the input clock source to generate the internal clock ADCK. */
	adc_clock_driver_t clockDriver; /*!< Select the divide ratio used by the ADC to generate the internal clock ADCK. */
	adc_resolution_t resolution;    /*!< Select the ADC resolution mode. */
} adc_config_t;

/*!
 * @brief Converter Offset configuration.
 */
typedef struct _adc_offest_config {
	bool enableSigned;    /*!< if false,The offset value is added with the raw result.
								if true,The offset value is subtracted from the raw converted value. */
	uint32_t offsetValue; /*!< User configurable offset value(0-4095). */
} adc_offest_config_t;

/*!
 * @brief ADC hardware compare configuration.
 *
 * In kADC_HardwareCompareMode0, compare true if the result is less than the value1.
 * In kADC_HardwareCompareMode1, compare true if the result is greater than or equal to value1.
 * In kADC_HardwareCompareMode2, Value1 <= Value2, compare true if the result is less than value1 Or the result is
 * Greater than value2.
 *                               Value1 >  Value2, compare true if the result is less than value1 And the result is
 * Greater than value2.
 * In kADC_HardwareCompareMode3, Value1 <= Value2, compare true if the result is greater than or equal to value1 And the
 * result is less than or equal to value2.
 *                               Value1 >  Value2, compare true if the result is greater than or equal to value1 Or the
 * result is less than or equal to value2.
 */
typedef struct _adc_hardware_compare_config {
	adc_hardware_compare_mode_t hardwareCompareMode; /*!< Select the hardware compare mode.
															See "adc_hardware_compare_mode_t". */
	uint16_t value1;                                 /*!< Setting value1(0-4095) for hardware compare mode. */
	uint16_t value2;                                 /*!< Setting value2(0-4095) for hardware compare mode. */
} adc_hardware_compare_config_t;

/*!
 * @brief ADC channel conversion configuration.
 */
typedef struct _adc_channel_config {
	uint32_t channelNumber;                    /*!< Setting the conversion channel number. The available range is 0-31.
													See channel connection information for each chip in Reference
													Manual document. */
	bool enableInterruptOnConversionCompleted; /*!< Generate an interrupt request once the conversion is completed. */
} adc_channel_config_t;
/*******************************************************************************
* API
******************************************************************************/
/*!
 * @name Initialization
 * @{
 */

/*!
 * @brief Initialize the ADC module.
 *
 * @param base ADC peripheral base address.
 * @param config Pointer to "adc_config_t" structure.
 */
void imxrt_adc_init(ADC_Type *base, const adc_config_t *config);

/*!
 * @brief De-initializes the ADC module.
 *
 * @param base ADC peripheral base address.
 */
void imxrt_adc_deinit(ADC_Type *base);

/*!
 * @brief Gets an available pre-defined settings for the converter's configuration.
 *
 * This function initializes the converter configuration structure with available settings. The default values are:
 * @code
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
 * @endcode
 * @param base   ADC peripheral base address.
 * @param config Pointer to the configuration structure.
 */
void imxrt_adc_getdefaultconfig(adc_config_t *config);

/*!
 * @brief Configures the conversion channel.
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
 * @param base          ADC peripheral base address.
 * @param channelGroup  Channel group index.
 * @param config        Pointer to the "adc_channel_config_t" structure for the conversion channel.
 */
void imxrt_adc_setchannelconfig(ADC_Type *base, uint32_t channelGroup, const adc_channel_config_t *config);

/*!
 * @brief  Gets the conversion value.
 *
 * @param  base         ADC peripheral base address.
 * @param  channelGroup Channel group index.
 *
 * @return              Conversion value.
 */
static inline uint32_t imxrt_adc_getchannelconversionvalue(ADC_Type *base, uint32_t channelGroup)
{
	assert(channelGroup < ADC_R_COUNT);

	return base->R[channelGroup];
}

/*!
 * @brief Gets the status flags of channel.
 *
 * A conversion is completed when the result of the conversion is transferred into the data
 * result registers. (provided the compare function & hardware averaging is disabled), this is
 * indicated by the setting of COCOn. If hardware averaging is enabled, COCOn sets only,
 * if the last of the selected number of conversions is complete. If the compare function is
 * enabled, COCOn sets and conversion result data is transferred only if the compare
 * condition is true. If both hardware averaging and compare functions are enabled, then
 * COCOn sets only if the last of the selected number of conversions is complete and the
 * compare condition is true.
 *
 * @param base         ADC peripheral base address.
 * @param channelGroup Channel group index.
 *
 * @return             Status flags of channel.return 0 means COCO flag is 0,return 1 means COCOflag is 1.
 */
static inline uint32_t imxrt_adc_getchannelstatusflags(ADC_Type *base, uint32_t channelGroup)
{
	assert(channelGroup < ADC_HC_COUNT);

	/* If flag is set,return 1,otherwise, return 0. */
	return (((base->HS) & (1U << channelGroup)) >> channelGroup);
}

/*!
 * @brief  Automates the hardware calibration.
 *
 * This auto calibration helps to adjust the plus/minus side gain automatically.
 * Execute the calibration before using the converter. Note that the software trigger should be used
 * during calibration.
 *
 * @param  base ADC peripheral base address.
 *
 * @return                 Execution status.
 * @retval kStatus_Success Calibration is done successfully.
 * @retval kStatus_Fail    Calibration has failed.
 */
status_t imxrt_adc_doautocalibration(ADC_Type *base);

/*!
 * @brief Set user defined offset.
 *
 * @param base   ADC peripheral base address.
 * @param config Pointer to "adc_offest_config_t" structure.
 */
void imxrt_adc_setoffsetconfig(ADC_Type *base, const adc_offest_config_t *config);

/*!
 * @brief Enables generating the DMA trigger when the conversion is complete.
 *
 * @param base   ADC peripheral base address.
 * @param enable Switcher of the DMA feature. "true" means enabled, "false" means not enabled.
 */
static inline void imxrt_adc_enabledma(ADC_Type *base, bool enable)
{
	if (enable) {
		base->GC |= ADC_GC_DMAEN_MASK;
	} else {
		base->GC &= ~ADC_GC_DMAEN_MASK;
	}
}

/*!
 * @brief Enables the hardware trigger mode.
 *
 * @param base ADC peripheral base address.
 * @param enable Switcher of the trigger mode. "true" means hardware tirgger mode,"false" means software mode.
 */
#if !(defined(FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE) && FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE)
static inline void imxrt_adc_enablehardwaretrigger(ADC_Type *base, bool enable)
{
	if (enable) {
		base->CFG |= ADC_CFG_ADTRG_MASK;
	} else {
		base->CFG &= ~ADC_CFG_ADTRG_MASK;
	}
}
#endif

/*!
 * @brief Configures the hardware compare mode.
 *
 * The hardware compare mode provides a way to process the conversion result automatically by using hardware. Only the
 * result
 * in the compare range is available. To compare the range, see "adc_hardware_compare_mode_t" or the appopriate
 * reference
 * manual for more information.
 *
 * @param base ADC peripheral base address.
 * @param Pointer to "adc_hardware_compare_config_t" structure.
 *
 */
void imxrt_adc_sethardwarecompareconfig(ADC_Type *base, const adc_hardware_compare_config_t *config);

/*!
 * @brief Configures the hardware average mode.
 *
 * The hardware average mode provides a way to process the conversion result automatically by using hardware. The
 * multiple
 * conversion results are accumulated and averaged internally making them easier to read.
 *
 * @param base ADC peripheral base address.
 * @param mode Setting the hardware average mode. See "adc_hardware_average_mode_t".
 */
void imxrt_adc_sethardwareaverageconfig(ADC_Type *base, adc_hardware_average_mode_t mode);

/*!
 * @brief Gets the converter's status flags.
 *
 * @param base ADC peripheral base address.
 *
 * @return Flags' mask if indicated flags are asserted. See "adc_status_flags_t".
 */
static inline uint32_t imxrt_adc_getstatusflags(ADC_Type *base)
{
	return base->GS;
}

/*!
 * @brief Clears the converter's status falgs.
 *
 * @param base ADC peripheral base address.
 * @param mask Mask value for the cleared flags. See "adc_status_flags_t".
 */
void imxrt_adc_clearstatusflags(ADC_Type *base, uint32_t mask);

/****************************************************************************
 * Name: imxrt_adc_initialize
 *
 * Description:
 *   Initialize the ADC. As the pins of each ADC channel are exported through
 *   configurable GPIO and it is board-specific, information on available
 *   ADC channels should be passed to imxrt_adc_initialize().
 *
 * Input Parameters:
 *   bus - ADC module number
 *   cchannels - Number of channels
 *
 * Returned Value:
 *   Valid can device structure reference on success; a NULL on failure
 *
 ****************************************************************************/
struct adc_dev_s *imxrt_adc_initialize(uint32_t pinset);

void imxrt_adc_pins_init(int bus, int cchannels);
//static
int imxrt_adc_configinput(uint32_t pinset);
FAR struct adc_dev_s * imxrt_adc_lowerhalf(uint32_t pinset);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* __ARCH_ARM_SRC_IMXRT_IMXRT_ADC_H__ */
