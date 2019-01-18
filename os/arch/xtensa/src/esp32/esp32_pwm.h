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

// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _DRIVER_MCPWM_H_
#define _DRIVER_MCPWM_H_

//#include "soc/soc.h"
//#include "driver/gpio.h"
//#include "driver/periph_ctrl.h"
//#include "esp_intr.h"
//#include "esp_intr_alloc.h"

#ifdef __cplusplus
extern "C" {


#endif	/* 
 */

/**
 * @brief IO signals for MCPWM
 *        6 MCPWM output pins that generate PWM signals
 *        3 MCPWM fault input pins to detect faults like overcurrent, overvoltage, etc
 *        3 MCPWM sync input pins to synchronize MCPWM outputs signals
 *        3 MCPWM capture input pin to capture hall sell signal to measure time
 */
typedef enum {
	MCPWM0A = 0, /*!<PWM0A output pin */
	MCPWM0B, /*!<PWM0B output pin */
	MCPWM1A, /*!<PWM1A output pin */
	MCPWM1B, /*!<PWM1B output pin */
	MCPWM2A, /*!<PWM2A output pin */
	MCPWM2B, /*!<PWM2B output pin */
	MCPWM_SYNC_0, /*!<SYNC0  input pin */
	MCPWM_SYNC_1, /*!<SYNC1  input pin */
	MCPWM_SYNC_2, /*!<SYNC2  input pin */
	MCPWM_FAULT_0, /*!<FAULT0 input pin */
	MCPWM_FAULT_1, /*!<FAULT1 input pin */
	MCPWM_FAULT_2, /*!<FAULT2 input pin */
	MCPWM_CAP_0 = 84, /*!<CAP0   input pin */
	MCPWM_CAP_1, /*!<CAP1   input pin */
	MCPWM_CAP_2, /*!<CAP2   input pin */
}
mcpwm_io_signals_t;



/**
 * @brief MCPWM pin number for
 *
 */
typedef struct {
	int mcpwm0a_out_num;	/*!<MCPWM0A out pin */
	int mcpwm0b_out_num;	/*!<MCPWM0A out pin */
	int mcpwm1a_out_num;	/*!<MCPWM0A out pin */
	int mcpwm1b_out_num;	/*!<MCPWM0A out pin */
	int mcpwm2a_out_num;	/*!<MCPWM0A out pin */
	int mcpwm2b_out_num;	/*!<MCPWM0A out pin */
	int mcpwm_sync0_in_num;	/*!<SYNC0  in pin */
	int mcpwm_sync1_in_num;	/*!<SYNC1  in pin */
	int mcpwm_sync2_in_num;	/*!<SYNC2  in pin */
	int mcpwm_fault0_in_num;	/*!<FAULT0 in pin */
	int mcpwm_fault1_in_num;	/*!<FAULT1 in pin */
	int mcpwm_fault2_in_num;	/*!<FAULT2 in pin */
	int mcpwm_cap0_in_num;	/*!<CAP0   in pin */
	int mcpwm_cap1_in_num;	/*!<CAP1   in pin */
	int mcpwm_cap2_in_num;	/*!<CAP2   in pin */
} mcpwm_pin_config_t;



/**
 * @brief Select MCPWM unit
 */
typedef enum {
	MCPWM_UNIT_0 = 0, /*!<MCPWM unit0 selected */
	MCPWM_UNIT_1, /*!<MCPWM unit1 selected */
	MCPWM_UNIT_MAX, /*!<Num of MCPWM units on ESP32 */
} mcpwm_unit_t;



/**
 * @brief Select MCPWM timer
 */
typedef enum {
	MCPWM_TIMER_0 = 0, /*!<Select MCPWM timer0 */
	MCPWM_TIMER_1, /*!<Select MCPWM timer1 */
	MCPWM_TIMER_2, /*!<Select MCPWM timer2 */
	MCPWM_TIMER_MAX, /*!<Num of MCPWM timers on ESP32 */
} mcpwm_timer_t;



/**
 * @brief Select MCPWM operator
 */
typedef enum {
	MCPWM_OPR_A = 0, /*!<Select MCPWMXA, where 'X' is timer number */
	MCPWM_OPR_B, /*!<Select MCPWMXB, where 'X' is timer number */
	MCPWM_OPR_MAX, /*!<Num of operators to each timer of MCPWM */
} mcpwm_operator_t;



/**
 * @brief Select type of MCPWM counter
 */
typedef enum {
	MCPWM_UP_COUNTER = 1, /*!<For asymmetric MCPWM */
	MCPWM_DOWN_COUNTER, /*!<For asymmetric MCPWM */
	MCPWM_UP_DOWN_COUNTER, /*!<For symmetric MCPWM, frequency is half of MCPWM frequency set */
	MCPWM_COUNTER_MAX, /*!<Maximum counter mode */
} mcpwm_counter_type_t;



/**
 * @brief Select type of MCPWM duty cycle mode
 */
typedef enum {
	MCPWM_DUTY_MODE_0 = 0, /*!<Active high duty, i.e. duty cycle proportional to high time for asymmetric MCPWM */
	MCPWM_DUTY_MODE_1, /*!<Active low duty,  i.e. duty cycle proportional to low  time for asymmetric MCPWM, out of phase(inverted) MCPWM */
	MCPWM_DUTY_MODE_MAX, /*!<Num of duty cycle modes */
} mcpwm_duty_type_t;



/**
 * @brief MCPWM config structure
 */
typedef struct {
	uint32_t frequency;	/*!<Set frequency of MCPWM in Hz */
	ub16_t cmpr_a;		/*!<Set % duty cycle for operator a(MCPWMXA), i.e for 62.3% duty cycle, duty_a = 62.3 */
	ub16_t cmpr_b;		/*!<Set % duty cycle for operator b(MCPWMXB), i.e for 48% duty cycle, duty_b = 48.0 */
	mcpwm_duty_type_t duty_mode;	/*!<Set type of duty cycle */
	mcpwm_counter_type_t counter_mode;	/*!<Set  type of MCPWM counter */
} mcpwm_config_t;




/**
 * @brief This function initializes each gpio signal for MCPWM
 *        @note
 *        This function initializes one gpio at a time.
 *
 * @param mcpwm_num set MCPWM Channel(0-1)
 * @param io_signal set MCPWM signals, each MCPWM unit has 6 output(MCPWMXA, MCPWMXB) and 9 input(SYNC_X, FAULT_X, CAP_X)
 *                  'X' is timer_num(0-2)
 * @param gpio_num set this to configure gpio for MCPWM, if you want to use gpio16, gpio_num = 16
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
int mcpwm_gpio_init(mcpwm_unit_t mcpwm_num, mcpwm_io_signals_t io_signal, int gpio_num);



/**
 * @brief Initialize MCPWM gpio structure
 *        @note
 *        This function can be used to initialize more then one gpio at a time.
 *
 * @param mcpwm_num set MCPWM Channel(0-1)
 * @param mcpwm_pin MCPWM pin structure
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
int mcpwm_set_pin(mcpwm_unit_t mcpwm_num, const mcpwm_pin_config_t *mcpwm_pin);



/**
 * @brief Initialize MCPWM parameters
 *
 * @param mcpwm_num set MCPWM Channel(0-1)
 * @param timer_num set timer number(0-2) of MCPWM, each MCPWM unit has 3 timers
 * @param mcpwm_conf configure structure mcpwm_config_t
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
int mcpwm_init(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, const mcpwm_config_t *mcpwm_conf);



/**
 * @brief Set frequency(in Hz) of MCPWM timer
 *
 * @param mcpwm_num set MCPWM unit(0-1)
 * @param timer_num set timer number(0-2) of MCPWM, each MCPWM unit has 3 timers
 * @param frequency set the frequency in Hz of each timer
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
int mcpwm_set_frequency(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, uint32_t frequency);



/**
 * @brief Set duty cycle of each operator(MCPWMXA/MCPWMXB)
 *
 * @param mcpwm_num set MCPWM unit(0-1)
 * @param timer_num set timer number(0-2) of MCPWM, each MCPWM unit has 3 timers
 * @param op_num set the operator(MCPWMXA/MCPWMXB), 'X' is timer number selected
 * @param duty set duty cycle in %(i.e for 62.3% duty cycle, duty = 62.3) of each operator
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
int mcpwm_set_duty(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, mcpwm_operator_t op_num, ub16_t duty);




/**
 * @brief Set duty either active high or active low(out of phase/inverted)
 *        @note
 *        Call this function every time after mcpwm_set_signal_high or mcpwm_set_signal_low to resume with previously set duty cycle
 *
 * @param mcpwm_num set MCPWM unit(0-1)
 * @param timer_num set timer number(0-2) of MCPWM, each MCPWM unit has 3 timers
 * @param op_num set the operator(MCPWMXA/MCPWMXB), 'x' is timer number selected
 * @param duty_num set active low or active high duty type
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
int mcpwm_set_duty_type(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, mcpwm_operator_t op_num, mcpwm_duty_type_t duty_num);




/**
 * @brief Get duty cycle of each operator
 *
 * @param mcpwm_num set MCPWM unit(0-1)
 * @param timer_num set timer number(0-2) of MCPWM, each MCPWM unit has 3 timers
 * @param op_num set the operator(MCPWMXA/MCPWMXB), 'x' is timer number selected
 *
 * @return
 *     - duty cycle in % of each operator(56.7 means duty is 56.7%)
 */
int mcpwm_start(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num);



/**
 * @brief Start MCPWM signal on timer 'x'
 *
 * @param mcpwm_num set MCPWM unit(0-1)
 * @param timer_num set timer number(0-2) of MCPWM, each MCPWM unit has 3 timers
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
int mcpwm_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num);



FAR struct pwm_lowerhalf_s *esp32_pwminitialize(int id);



#ifdef __cplusplus
}
#endif	/* 
 */

#endif	/*_DRIVER_MCPWM_H_*/
