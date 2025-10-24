// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#pragma once
#include <common/bk_include.h>
#include <driver/hal/hal_pwm_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief PWM APIs
 * @addtogroup bk_api_pwm PWM API group
 * @{
 */

/**
 * @brief PWM defines
 * @defgroup bk_api_pwm_defs macos
 * @ingroup bk_api_pwm
 * @{
 */

#define BK_ERR_PWM_CLOCK           (BK_ERR_PWM_BASE - 1) /**< PWM global clock type not supported */
#define BK_ERR_PWM_CHAN_CLOCK      (BK_ERR_PWM_BASE - 2) /**< PWM channel clock type not supported */
#define BK_ERR_PWM_CHAN_ID         (BK_ERR_PWM_BASE - 3) /**< PWM channel number is invalid */
#define BK_ERR_PWM_PERIOD_DUTY     (BK_ERR_PWM_BASE - 4) /**< PWM all duty cycle > period cycle */
#define BK_ERR_PWM_CAPTURE_EDGE    (BK_ERR_PWM_BASE - 5) /**< PWM capture edge is invalid */
#define BK_ERR_PWM_NOT_INIT        (BK_ERR_PWM_BASE - 6) /**< PWM driver not init */
#define BK_ERR_PWM_CHAN_NOT_INIT   (BK_ERR_PWM_BASE - 7) /**< PWM channel not init */
#define BK_ERR_PWM_CHAN_NOT_START  (BK_ERR_PWM_BASE - 8) /**< PWM channel not init */
#define BK_ERR_PWM_GROUP_ID        (BK_ERR_PWM_BASE - 9) /**< PWM group ID is invalid */
#define BK_ERR_PWM_GROUP_EXIST     (BK_ERR_PWM_BASE - 10) /**< PWM group already exists */
#define BK_ERR_PWM_GROUP_CHAN_USED (BK_ERR_PWM_BASE - 11) /**< PWM channel is used by other group */
#define BK_ERR_PWM_GROUP_NOT_EXIST (BK_ERR_PWM_BASE - 12) /**< PWM group doesn't exist */
#define BK_ERR_PWM_GROUP_DUTY      (BK_ERR_PWM_BASE - 13) /**< PWM group cycle invalid */
#define BK_ERR_PWM_GROUP_SAME_CHAN (BK_ERR_PWM_BASE - 14) /**< PWM group channel is same */
#define BK_ERR_PWM_INVALID_GPIO_MODE (BK_ERR_PWM_BASE - 15) /**< PWM invalid gpio mode */

/**
 * @brief default PWM configuration
 */
#define PWM_DEFAULT_PWM_CONFIG() {\
	.period_cycle = 0,\
	.duty_cycle = 0,\
	.duty2_cycle = 0,\
	.duty3_cycle = 0,\
}

/**
 * @brief default PWM capture configuration
 */
#define PWM_DEFAULT_PWM_CAPTURE_CONFIG() {\
	.isr = NULL,\
}

typedef struct {
	uint32_t period_cycle;  /**< PWM period cycle, unit is (1/clock_frequency) */
	uint32_t duty_cycle;    /**< PWM duty cycle, unit is (1/clock_frequency) */
	uint32_t duty2_cycle;   /**< PWM duty2 cycle, unit is (1/clock_frequency) */
	uint32_t duty3_cycle;   /**< PWM duty2 cycle, unit is (1/clock_frequency) */
	uint32_t reserved[4];   /**< Reserved for future extend, must set to 0 */
} pwm_init_config_t;

typedef struct {
	uint32_t period_cycle;  /**< PWM period cycle, unit is (1/clock_frequency) */
	uint32_t duty_cycle;    /**< PWM duty cycle, unit is (1/clock_frequency) */
	uint32_t duty2_cycle;   /**< PWM duty2 cycle, unit is (1/clock_frequency) */
	uint32_t duty3_cycle;   /**< PWM duty3 cycle, unit is (1/clock_frequency) */
} pwm_period_duty_config_t;

/**
 * @brief PWM interrupt service routine
 */
typedef void (*pwm_isr_t)(pwm_chan_t chan);

typedef struct {
	pwm_capture_edge_t edge; /**< PWM chan capture edge */
	pwm_isr_t isr;           /**< PWM capture interrupt routine */
} pwm_capture_init_config_t;

typedef struct {
	pwm_chan_t chan1;          /**< The first PWM channel ID in a group */
	pwm_chan_t chan2;          /**< The second PWM channel ID in a group */
	uint32_t period_cycle;     /**< PWM Group period cycle */
	uint32_t chan1_duty_cycle; /**< Duty cycle of chan1 */
	uint32_t chan2_duty_cycle; /**< Duty cycle of chan2 */
} pwm_group_init_config_t;

typedef struct {
	uint32_t period_cycle;     /**< PWM Group period cycle */
	uint32_t chan1_duty_cycle; /**< Duty cycle of channel 1 */
	uint32_t chan2_duty_cycle; /**< Duty cycle of channel 2 */
} pwm_group_config_t;

/**
 * @brief PWM group type
 */
typedef uint8_t pwm_group_t;

#define PWM_GROUP_ID_INVALID       (0xFF) /**< Invalid PWM group ID */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
