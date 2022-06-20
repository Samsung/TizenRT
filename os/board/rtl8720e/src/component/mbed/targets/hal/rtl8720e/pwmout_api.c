/** mbed Microcontroller Library
 ******************************************************************************
 * @file	 pwmout_api.c
 * @author
 * @version V1.0.0
 * @date	 2016-08-01
 * @brief	 This file provides mbed API for PWM.
 ******************************************************************************
 * @attention
 *
 * This module is a confidential and proprietary property of RealTek and
 * possession or use of this module requires written permission of RealTek.
 *
 * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
 ******************************************************************************
 */

#include "device.h"
#include "objects.h"
#include "pinmap.h"
#include "pwmout_api.h"

#define PWM_TIMER			8
#define PWM_CHANNEL_MAX		8

/** @addtogroup Ameba_Mbed_API
  * @{
  */

/** @defgroup MBED_PWM
 *  @brief      MBED_PWM driver modules.
 *  @{
 */

/** @defgroup MBED_PWM_Exported_Types MBED_PWM Exported Types
 * @{
 */

u8 timer8_start = 0;
u8 km4_ch_start[PWM_CHANNEL_MAX] = {0};
u8 prescaler = 0;
RTIM_TypeDef *PWM_TIM = TIM8;
IRQn_Type PWM_IrqNum = TIMER8_IRQ;

static const u32 PinMap_PWM[PWM_CHANNEL_MAX] = {
	PINMUX_FUNCTION_PWM0,
	PINMUX_FUNCTION_PWM1,
	PINMUX_FUNCTION_PWM2,
	PINMUX_FUNCTION_PWM3,
	PINMUX_FUNCTION_PWM4,
	PINMUX_FUNCTION_PWM5,
	PINMUX_FUNCTION_PWM6,
	PINMUX_FUNCTION_PWM7
};

/**
  * @}
  */

/** @defgroup MBED_PWM_Exported_Functions MBED_PWM Exported Functions
 * @{
 */

/**
  * @brief  Check whether the pin has PWM out functionality and return corresponding channel number.
  * @param  pin: PWM output pin.
  * @return Check result. It can be one of the following values:
  * @retval 0: The pin is available for all PWM channels.
  * @retval 1: The pin is available for PWM channel 0/1/2/3.
  * @retval 2: The pin is available for PWM channel 4/5/6/7.
  * @retval NC: The pin does not have PWM out functionality.
  */
u32 pwmout_pin2chan(PinName pin)
{
	if ((pin >= PA_8) && (pin <= PA_14)) {
		return 0;
	} else if ((pin >= PA_15) && (pin <= PA_27)) {
		if ((pin == PA_19) || (pin == PA_20)) {
			return NC;
		}
		return 2; // pwm4/5/6/7
	} else if ((pin >= PA_28) && (pin <= PB_10)) {
		return 1; // pwm0/1/2/3
	} else	{
		return NC;
	}
}

/**
  * @brief  Initialize the TIM8 device.
  * @param  obj: PWM object defined in application software.
  * @retval none
  */
static void pwmout_timer8_init(pwmout_t *obj)
{
	(void) obj;
	RTIM_TimeBaseInitTypeDef TIM_InitStruct;
	RTIM_TimeBaseStructInit(&TIM_InitStruct);
	TIM_InitStruct.TIM_Idx = PWM_TIMER;

	RTIM_TimeBaseInit(PWM_TIM, &TIM_InitStruct, PWM_IrqNum, NULL, (u32)&TIM_InitStruct);
	RTIM_Cmd(PWM_TIM, ENABLE);

	timer8_start = 1;
}

/**
  * @brief  Initialize PWM channel output according to the specified pin.
  * @param  obj: PWM object defined in application software.
  * @param  pin: Pinname of corresponding PWM channel to be set.
  * @retval none
  * @note
  *             - default period: 1638us
  *             - default pulse width: 102us
  *             - default duty cycle: 6.227%
  */
void pwmout_init(pwmout_t *obj, PinName pin)
{
	u32 pwm_chan, pwm_chcheck;
	TIM_CCInitTypeDef TIM_CCInitStruct;

	assert_param(obj->pwm_idx < PWM_CHANNEL_MAX);

	pwm_chcheck = pwmout_pin2chan(pin);
	if (pwm_chcheck == NC) {
		DBG_8195A("PinName error: pwm channel of PinName doesn't exist!\n");
	} else if (pwm_chcheck == 1) {
		assert_param(obj->pwm_idx <= 3);
	} else if (pwm_chcheck == 2) {
		assert_param(obj->pwm_idx > 3);
	}

	/* Enable TIM_PWM function & clock */
	RCC_PeriphClockCmd(APBPeriph_TIM_PWM, APBPeriph_TIM_PWM_CLOCK, ENABLE);

	pwm_chan = obj->pwm_idx;
	obj->period = 0x10000 * (prescaler + 1) / 40;
	obj->pulse = 0x1000 * (prescaler + 1) / 40;

	if (!timer8_start) {
		pwmout_timer8_init(obj);
	}

	RTIM_CCStructInit(&TIM_CCInitStruct);
	RTIM_CCxInit(PWM_TIM, &TIM_CCInitStruct, pwm_chan);
	RTIM_CCxCmd(PWM_TIM, pwm_chan, TIM_CCx_Enable);

	Pinmux_Config(pin, PinMap_PWM[pwm_chan]);
	km4_ch_start[pwm_chan] = 1;
}


/**
  * @brief  Deinitialize the PWM device of the specified channel.
  * @param  obj: PWM object defined in application software.
  * @retval none
  * @note  If all channels are released, TIM8/TIMM08 will also be disabled.
  */
void pwmout_free(pwmout_t *obj)
{
	/* disable pwm channel */
	uint8_t pwm_chan = obj->pwm_idx;

	if (km4_ch_start[pwm_chan]) {
		km4_ch_start[pwm_chan] = 0;
		RTIM_CCxCmd(PWM_TIM, pwm_chan, TIM_CCx_Disable);

		/* stop timer8 if no pwm channels starts */
		for (pwm_chan = 0; pwm_chan < PWM_CHANNEL_MAX; pwm_chan++) {
			if (km4_ch_start[pwm_chan]) {
				return;
			}
		}

		RTIM_Cmd(PWM_TIM, DISABLE);
		timer8_start = 0;
	}

	return;
}

/**
  * @brief  Set the duty cycle of the specified channel.
  * @param  obj: PWM object defined in application software.
  * @param  percent: The duty cycle value to be set.
  * @retval none
  */
void pwmout_write(pwmout_t *obj, float percent) //write duty-cycle
{
	u32 ccrx;

	if (percent < (float)0.0) {
		percent = 0.0;
	} else if (percent > (float)1.0) {
		percent = 1.0;
	}

	obj->pulse = (percent * obj->period);

	ccrx = (u32)(obj->pulse  * 40 / (prescaler + 1)) & 0x0000ffff;

	RTIM_CCRxSet(PWM_TIM, ccrx, obj->pwm_idx);
}

/**
  * @brief  Get the duty cycle of the specified channel.
  * @param  obj: PWM object defined in application software.
  * @return Duty cycle of the specified channel.
  * @retval 0~1.0
  */
float pwmout_read(pwmout_t *obj) //read duty-cycle
{
	float value = 0;
	if (obj->period > 0) {
		value = (float)obj->pulse / (float)obj->period;
	}
	return ((value > 1.0) ? (1.0) : (value));
}

/**
  * @brief  Set the period of the specified channel in seconds.
  * @param  obj: PWM object defined in application software.
  * @param  seconds: The period value to be set in seconds(s).
  * @retval none
  */
void pwmout_period(pwmout_t *obj, float seconds)
{
	pwmout_period_us(obj, (int)(seconds * 1000000.0f));
}

/**
  * @brief  Set the period of the specified channel in millseconds.
  * @param  obj: PWM object defined in application software.
  * @param  ms: The period value to be set in millseconds(ms).
  * @retval none
  */
void pwmout_period_ms(pwmout_t *obj, int ms)
{
	pwmout_period_us(obj, (int)(ms * 1000));
}

/**
  * @brief  Set the period of the specified channel in microseconds.
  * @param  obj: PWM object defined in application software.
  * @param  us: The period value to be set in microseconds(us).
  * @retval none
  */
void pwmout_period_us(pwmout_t *obj, int us)
{
	u32 arr;
	float dc = pwmout_read(obj);
	u32 tmp = us * 40 / (prescaler + 1);

	if (tmp > 0x10000) {
		prescaler = us * 40 / 0x10000;
		RTIM_PrescalerConfig(PWM_TIM, prescaler, TIM_PSCReloadMode_Update);
	}

	obj->period = us;
	arr = us * 40 / (prescaler + 1) - 1;

	RTIM_ChangePeriod(PWM_TIM, arr);
	pwmout_write(obj, dc);
}

/**
  * @brief  Set the pulse width of the specified channel in seconds.
  * @param  obj: PWM object defined in application software.
  * @param  seconds: The pulse width value to be set in seconds(s).
  * @retval none
  */
void pwmout_pulsewidth(pwmout_t *obj, float seconds)
{
	pwmout_pulsewidth_us(obj, (int)(seconds * 1000000.0f));
}

/**
  * @brief  Set the pulse width of the specified channel in milliseconds.
  * @param  obj: PWM object defined in application software.
  * @param  ms: The pulse width value to be set in milliseconds(ms).
  * @retval none
  */
void pwmout_pulsewidth_ms(pwmout_t *obj, int ms)
{
	pwmout_pulsewidth_us(obj, ms * 1000);
}

/**
  * @brief  Set the pulse width of the specified channel in microseconds.
  * @param  obj: PWM object defined in application software.
  * @param  us: The pulse width value to be set in microseconds(us).
  * @retval none
  */
void pwmout_pulsewidth_us(pwmout_t *obj, int us)
{
	u32 ccrx;

	obj->pulse = (float)us;
	ccrx = (u32)(obj->pulse  * 40 / (prescaler + 1)) & 0x0000ffff;
	RTIM_CCRxSet(PWM_TIM, ccrx, obj->pwm_idx);
}

/**
  * @brief  Enable the specified channel to output PWM.
  * @param  obj: PWM object defined in application software.
  * @retval none
  */
void pwmout_start(pwmout_t *obj)
{
	RTIM_CCxCmd(PWM_TIM, obj->pwm_idx, TIM_CCx_Enable);
}

/**
  * @brief  Disable the specified channel to output PWM.
  * @param  obj: PWM object defined in application software.
  * @retval none
  */
void pwmout_stop(pwmout_t *obj)
{
	RTIM_CCxCmd(PWM_TIM, obj->pwm_idx, TIM_CCx_Disable);
}

/**
  * @brief  Set the polarity of the specified PWM channel.
  * @param  obj: PWM object defined in application software.
  * @param  polarity: This parameter can be one of the following values:
  *	  @arg  0: Output is LOW when timer count < setvalue.
  *	  @arg  1: Output is HIGH when timer count < setvalue.(default)
  * @attention Configure polarity after setting duty cycle or pulse width.
  * @retval none
  */
void pwmout_set_polarity(pwmout_t *obj, int polarity)
{
	if (0 == polarity) {
		RTIM_CCxPolarityConfig(PWM_TIM, TIM_CCPolarity_Low, obj->pwm_idx);
	} else {
		RTIM_CCxPolarityConfig(PWM_TIM, TIM_CCPolarity_High, obj->pwm_idx);
	}
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

