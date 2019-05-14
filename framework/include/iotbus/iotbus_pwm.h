/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/**
 * @defgroup PWM PWM
 * @brief Provides APIs for PWM (Pulse Width Modulation)
 * @ingroup IOTBUS
 * @{
 */

/**
 * @file iotbus/iotbus_pwm.h
 * @brief Iotbus APIs for PWM
 */

#ifndef IOTBUS_PWM_H_
#define IOTBUS_PWM_H_

/*
 * public types
 */
struct _iotbus_pwm_s;

/**
 * @brief Pointer definition to the internal struct _iotbus_pwm_wrapper_s
 */
typedef struct _iotbus_pwm_wrapper_s *iotbus_pwm_context_h;

/**
 * @brief Enumeration of PWM state
 * @details
 * Enumeration Details:\n
 * IOTBUS_PWM_DISABLE\n
 * IOTBUS_PWM_ENABLE\n
 */
typedef enum {
	IOTBUS_PWM_DISABLE = 0,
	IOTBUS_PWM_ENABLE,
} iotbus_pwm_state_e;

/**
 * @brief Enumeration of pwm value type
 * @details
 * Enumeration Details:\n
 * IOTBUS_PWM_LOW = 0,
 * IOTBUS_PWM_HIGH = 1,
 */
typedef enum {
	IOTBUS_PWM_LOW    = 0, /* Low value on Gpio */
	IOTBUS_PWM_HIGH    = 1, /* High value on Gpio */
} iotbus_pwm_val_e;

/**
 *@brief Type of pwm duty cycle
 *@details Duty cycle is measured in terms of percentage
 *so uint8_t is enough to hold percentage values.
 */
typedef uint8_t percent_t;

/*
 * Public Function
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief closes the pwm_context.
 *
 * @details @b #include <iotbus/iotbus_pwm.h>
 * @param[in] device N/A
 * @param[in] channel pwm channel number
 * @return On success, handle of pwm_context is returned. On failure, NULL is returned.
 * @since TizenRT v1.0
 */
iotbus_pwm_context_h iotbus_pwm_open(int device, int channel);

/**
 * @brief closes the pwm_context.
 *
 * @details @b #include <iotbus/iotbus_pwm.h>
 * @param[in] pwm handle of pwm_context
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_pwm_close(iotbus_pwm_context_h pwm);

/**
 * @brief sets the pwm duty cycle.
 *
 * @details @b #include <iotbus/iotbus_pwm.h>
 * @param[in] pwm handle of pwm_context
 * @param[in] duty_cycle pwm duty cycle in percent
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_pwm_set_duty_cycle(iotbus_pwm_context_h pwm, percent_t duty_cycle);

/**
 * @brief sets the pwm duty cycle with integer value.
 *
 * @details @b #include <iotbus/iotbus_pwm.h>
 * @param[in] pwm handle of pwm_context
 * @param[in] duty_cycle pwm duty cycle in integer value (0 - 65535)
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_pwm_set_duty_cycle_in_integer(iotbus_pwm_context_h pwm, uint16_t duty_cycle);

/**
 * @brief sets the pwm period.
 *
 * @details @b #include <iotbus/iotbus_pwm.h>
 * @param[in] pwm handle of pwm_context
 * @param[in] period pwm duty period
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_pwm_set_period(iotbus_pwm_context_h pwm, uint32_t period);

/**
 * @brief sets the pwm idle value when pwm disabled.
 *
 * @details @b #include <iotbus/iotbus_pwm.h>
 * @param[in] pwm handle of pwm_context
 * @param[in] val pwm idle value
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_pwm_set_idle(iotbus_pwm_context_h pwm, iotbus_pwm_val_e val);

/**
 * @brief sets the pwm state.
 *
 * @details @b #include <iotbus/iotbus_pwm.h>
 * @param[in] pwm handle of pwm_context
 * @param[in] enable pwm enable/disabled state value
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_pwm_set_enabled(iotbus_pwm_context_h pwm, iotbus_pwm_state_e enable);

/**
 * @brief checks if pwm state is enabled.
 *
 * @details @b #include <iotbus/iotbus_pwm.h>
 * @param[in] pwm handle of pwm_context
 * @return On success, current pwm state value is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_pwm_is_enabled(iotbus_pwm_context_h pwm);

/**
 * @brief gets the pwm duty cycle.
 *
 * @details @b #include <iotbus/iotbus_pwm.h>
 * @param[in] pwm handle of pwm_context
 * @return On success, current pwm duty cycle is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_pwm_get_duty_cycle(iotbus_pwm_context_h pwm);

/**
 * @brief gets the pwm period.
 *
 * @details @b #include <iotbus/iotbus_pwm.h>
 * @param[in] pwm handle of pwm_context
 * @return On success, current pwm period is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int iotbus_pwm_get_period(iotbus_pwm_context_h pwm);

#ifdef __cplusplus
}
#endif

#endif /* IOTBUS_PWM_H_ */

/** @} */ // end of PWM group
