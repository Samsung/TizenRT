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

/// @file iotbus_pwm.h
/// @brief Test Case of APIs for PWM

#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include "itc_internal.h"
#include <iotbus_pwm.h>
#include <iotbus_error.h>


/**
* @testcase         itc_systemio_pwm_open_close_p
* @brief            open and closes the pwm_context
* @scenario         open and closes the pwm_context
* @apicovered       iotbus_pwm_open, iotbus_pwm_close
* @precondition     none
* @postcondition    none
*/
void itc_systemio_pwm_open_close_p(void)
{
	int device = 0;
	int channel = 1;
	int ret = 0;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_set_get_duty_cycle_p
* @brief            sets and gets the pwm duty cycle
* @scenario         sets and gets the same pwm duty cycle
* @apicovered       iotbus_pwm_set_duty_cycle, iotbus_pwm_get_duty_cycle
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_set_get_duty_cycle_p(void)
{
	int device = 0;
	int channel = 1;
	int cycle = 20;
	int ret = 0;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	ret = iotbus_pwm_set_duty_cycle(h_pwm, cycle);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_set_duty_cycle", ret, OK, iotbus_pwm_close(h_pwm));

	ret = iotbus_pwm_get_duty_cycle(h_pwm);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_get_duty_cycle", ret, cycle, iotbus_pwm_close(h_pwm));

	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_set_get_period_p
* @brief            sets and gets the pwm period
* @scenario         sets and gets the same pwm period
* @apicovered       iotbus_pwm_set_period, iotbus_pwm_get_period
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_set_get_period_p(void)
{
	int device = 0;
	int channel = 1;
	int period = 1000;
	int ret = 0;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	ret = iotbus_pwm_set_period(h_pwm, period);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_set_period", ret, OK, iotbus_pwm_close(h_pwm));

	ret = iotbus_pwm_get_period(h_pwm);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_get_period", ret, period, iotbus_pwm_close(h_pwm));

	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_set_period_is_enabled_p
* @brief            checks if pwm state is enabled
* @scenario         sets and gets the same pwm period and checks if pwm state is enabled
* @apicovered       iotbus_pwm_is_enabled
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_set_period_is_enabled_p(void)
{
	int device = 0;
	int channel = 1;
	int period = 1000;
	int ret = 0;
	int default_state = 0;
	int get_state = 0;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	ret = iotbus_pwm_set_period(h_pwm, period);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_set_period", ret, OK, iotbus_pwm_close(h_pwm));

	ret = iotbus_pwm_get_period(h_pwm);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_get_period", ret, period, iotbus_pwm_close(h_pwm));

	default_state = iotbus_pwm_is_enabled(h_pwm);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_is_enabled", default_state < OK, false, iotbus_pwm_close(h_pwm));

	iotbus_pwm_state_e e_state = IOTBUS_PWM_ENABLE;
	if (default_state == IOTBUS_PWM_DISABLE) {
		ret = iotbus_pwm_set_enabled(h_pwm, e_state);
		TC_ASSERT_EQ_CLEANUP("iotbus_pwm_set_enabled", ret, OK, iotbus_pwm_close(h_pwm));

		get_state = iotbus_pwm_is_enabled(h_pwm);
		TC_ASSERT_EQ_CLEANUP("iotbus_pwm_is_enabled", get_state, e_state, iotbus_pwm_close(h_pwm); iotbus_pwm_set_enabled(h_pwm, default_state));

		// Restoring default state
		iotbus_pwm_set_enabled(h_pwm, default_state);
	} else {
		e_state = IOTBUS_PWM_DISABLE;
		ret = iotbus_pwm_set_enabled(h_pwm, e_state);
		TC_ASSERT_EQ_CLEANUP("iotbus_pwm_set_enabled", ret, OK, iotbus_pwm_close(h_pwm));

		get_state = iotbus_pwm_is_enabled(h_pwm);
		TC_ASSERT_EQ_CLEANUP("iotbus_pwm_is_enabled", get_state, e_state, iotbus_pwm_close(h_pwm); iotbus_pwm_set_enabled(h_pwm, default_state));

		// Restoring default state
		iotbus_pwm_set_enabled(h_pwm, default_state);
	}

	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_NEQ("iotbus_pwm_close", h_pwm, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_set_get_duty_cycle_p_different_cycle
* @brief            sets and gets the pwm duty cycle
* @scenario         sets and gets the same pwm duty cycle for all cycle values
* @apicovered       iotbus_pwm_set_duty_cycle, iotbus_pwm_get_duty_cycle
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_set_get_duty_cycle_p_different_cycle(void)
{
	int device = 0;
	int channel = 1;
	int cycle;
	int ret = 0;
	bool check = true;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	for (cycle = 0; cycle <= 100; cycle++) {
		ret = iotbus_pwm_set_duty_cycle(h_pwm, cycle);
		if (ret != OK) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_pwm_set_get_duty_cycle_p: iotbus_pwm_set_duty_cycle FAIL for cycle : %d\n", cycle);
			continue;
		}

		ret = iotbus_pwm_get_duty_cycle(h_pwm);
		if (ret != cycle) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_pwm_set_get_duty_cycle_p: iotbus_pwm_get_duty_cycle FAIL for cycle : %d\n", cycle);
			continue;
		}
	}
	TC_ASSERT_EQ_CLEANUP("itc_systemio_pwm_set_get_duty_cycle_p_different_cycle", check, true, iotbus_pwm_close(h_pwm));
	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_set_get_period_p_different_period
* @brief            sets and gets the pwm period
* @scenario         sets and gets the same pwm period
* @apicovered       iotbus_pwm_set_period, iotbus_pwm_get_period
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_set_get_period_p_different_period(void)
{
	int device = 0;
	int channel = 1;
	int period[] = { 10, 100, 1000, 10000, 100000, 1000000 };
	int count;
	int index;
	int ret = 0;
	bool check = true;
	count = sizeof(period) / sizeof(period[0]);
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	for (index = 0; index < count; index++) {
		ret = iotbus_pwm_set_period(h_pwm, period[index]);
		if (ret != OK) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_pwm_set_get_different_period_p: iotbus_pwm_set_period FAIL for period : %d\n", period[index]);
			continue;
		}
		ret = iotbus_pwm_get_period(h_pwm);
		if (ret != period[index]) {
			check = false;
			SYSIO_ITC_PRINT("\nitc_pwm_set_get_different_period_p: iotbus_pwm_get_period FAIL for period : %d\n", period[index]);
			continue;
		}
	}
	TC_ASSERT_EQ_CLEANUP("itc_systemio_pwm_set_get_period_p_different_period", check, true, iotbus_pwm_close(h_pwm));
	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_open_n
* @brief            open the pwm_context
* @scenario         open the pwm_context with invalid argument
* @apicovered       iotbus_pwm_open
* @precondition     none
* @postcondition    none
*/
void itc_systemio_pwm_open_n(void)
{
	int invalid_device = 1;
	int channel = 1;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(invalid_device, channel);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_open", h_pwm, NULL, iotbus_pwm_close(h_pwm));

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_close_n
* @brief            open and closes the pwm_context
* @scenario         open and closes the pwm_context
* @apicovered       iotbus_pwm_open, iotbus_pwm_close
* @precondition     none
* @postcondition    none
*/
void itc_systemio_pwm_close_n(void)
{
	int ret = iotbus_pwm_close(NULL);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_set_duty_cycle_n
* @brief            sets the pwm duty cycle
* @scenario         sets the same pwm duty cycle with invalid parameter
* @apicovered       iotbus_pwm_set_duty_cycle
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_set_duty_cycle_n(void)
{
	int device = 0;
	int channel = 1;
	int cycle = 100;
	int invalid_cycle = 101;
	int ret = 0;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	ret = iotbus_pwm_set_duty_cycle(NULL, cycle);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_set_duty_cycle", ret, IOTBUS_ERROR_INVALID_PARAMETER, iotbus_pwm_close(h_pwm));

	ret = iotbus_pwm_set_duty_cycle(h_pwm, invalid_cycle);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_set_duty_cycle", ret, IOTBUS_ERROR_INVALID_PARAMETER, iotbus_pwm_close(h_pwm));

	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_set_duty_cycle_n_after_close
* @brief            sets the pwm duty cycle
* @scenario         sets the pwm duty cycle after pwm close
* @apicovered       iotbus_pwm_set_duty_cycle
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_set_duty_cycle_n_after_close(void)
{
	int device = 0;
	int channel = 1;
	int cycle = 20;
	int ret = 0;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, OK);

	ret = iotbus_pwm_set_duty_cycle(h_pwm, cycle);
	TC_ASSERT_LT("iotbus_pwm_set_duty_cycle", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_get_duty_cycle_n
* @brief            gets the pwm duty cycle
* @scenario         gets the same pwm duty cycle with invalid parameter
* @apicovered       iotbus_pwm_get_duty_cycle
* @precondition     NA
* @postcondition    NA
*/
void itc_systemio_pwm_get_duty_cycle_n(void)
{
	int ret = iotbus_pwm_get_duty_cycle(NULL);
	TC_ASSERT_EQ("iotbus_pwm_get_duty_cycle", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_get_duty_cycle_n_after_close
* @brief            sets and gets the pwm duty cycle
* @scenario         sets and gets the same pwm duty cycle
* @apicovered       iotbus_pwm_set_duty_cycle, iotbus_pwm_get_duty_cycle
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_get_duty_cycle_n_after_close(void)
{
	int device = 0;
	int channel = 1;
	int cycle = 20;
	int ret = 0;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	ret = iotbus_pwm_set_duty_cycle(h_pwm, cycle);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_set_duty_cycle", ret, OK, iotbus_pwm_close(h_pwm));

	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, OK);

	ret = iotbus_pwm_get_duty_cycle(h_pwm);
	TC_ASSERT_LT("iotbus_pwm_get_duty_cycle", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_set_period_n
* @brief            sets the pwm period
* @scenario         sets the pwm period with invalid argument
* @apicovered       iotbus_pwm_set_period
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_set_period_n(void)
{
	int device = 0;
	int channel = 1;
	int period = 1000;
	int ret = 0;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	ret = iotbus_pwm_set_period(NULL, period);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_set_period", ret, IOTBUS_ERROR_INVALID_PARAMETER, iotbus_pwm_close(h_pwm));

	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_set_period_n_after_close
* @brief            sets the pwm period
* @scenario         sets the pwm period after close
* @apicovered       iotbus_pwm_set_period
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_set_period_n_after_close(void)
{
	int device = 0;
	int channel = 1;
	int period = 1000;
	int ret = 0;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, OK);

	ret = iotbus_pwm_set_period(h_pwm, period);
	TC_ASSERT_LT("iotbus_pwm_set_period", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_get_period_n
* @brief            gets the pwm period
* @scenario         gets the pwm period with invalid parameter
* @apicovered       iotbus_pwm_get_period
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_get_period_n(void)
{
	int ret = iotbus_pwm_get_period(NULL);
	TC_ASSERT_EQ("iotbus_pwm_get_period", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_get_period_n_after_close
* @brief            gets the pwm period
* @scenario         gets the pwm period after pwm close
* @apicovered       iotbus_pwm_get_period
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_get_period_n_after_close(void)
{
	int device = 0;
	int channel = 1;
	int period = 1000;
	int ret = 0;
	iotbus_pwm_context_h h_pwm = iotbus_pwm_open(device, channel);
	TC_ASSERT_NEQ("iotbus_pwm_open", h_pwm, NULL);

	ret = iotbus_pwm_set_period(h_pwm, period);
	TC_ASSERT_EQ_CLEANUP("iotbus_pwm_set_period", ret, OK, iotbus_pwm_close(h_pwm));

	ret = iotbus_pwm_close(h_pwm);
	TC_ASSERT_EQ("iotbus_pwm_close", ret, OK);

	ret = iotbus_pwm_get_period(h_pwm);
	TC_ASSERT_LT("iotbus_pwm_get_period", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_systemio_pwm_is_enabled_n
* @brief            checks if pwm state is enabled
* @scenario         checks if pwm state is enabled with invalid handle
* @apicovered       iotbus_pwm_is_enabled
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_systemio_pwm_is_enabled_n(void)
{
	int ret = iotbus_pwm_is_enabled(NULL);
	TC_ASSERT_EQ("iotbus_pwm_is_enabled", ret, IOTBUS_ERROR_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

int itc_pwm_main(void)
{
	itc_systemio_pwm_open_close_p();
	itc_systemio_pwm_set_get_duty_cycle_p();
	itc_systemio_pwm_set_get_period_p();
	itc_systemio_pwm_set_period_is_enabled_p();
	itc_systemio_pwm_set_get_duty_cycle_p_different_cycle();
	itc_systemio_pwm_set_get_period_p_different_period();
	itc_systemio_pwm_open_n();
	itc_systemio_pwm_close_n();
	itc_systemio_pwm_set_duty_cycle_n();
	itc_systemio_pwm_set_duty_cycle_n_after_close();
	itc_systemio_pwm_get_duty_cycle_n();
	itc_systemio_pwm_get_duty_cycle_n_after_close();
	itc_systemio_pwm_set_period_n();
	itc_systemio_pwm_set_period_n_after_close();
	itc_systemio_pwm_get_period_n();
	itc_systemio_pwm_get_period_n_after_close();
	itc_systemio_pwm_is_enabled_n();

	return 0;
}
