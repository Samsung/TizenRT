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
#include <stdlib.h>
#include <errno.h>
#include <tinyara/time.h>
#include <sys/time.h>
#include "itc_internal.h"
#include <iotbus_pwm.h>
#include <iotbus_error.h>

/**
* @testcase         itc_pwm_open_close_p
* @brief            open and closes the pwm_context
* @scenario         open and closes the pwm_context
* @apicovered       iotbus_pwm_open, iotbus_pwm_close
* @precondition     none
* @postcondition    none
*/
void itc_pwm_open_close_p(void)
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
* @testcase         itc_pwm_set_get_duty_cycle_p
* @brief            sets and gets the pwm duty cycle
* @scenario         sets and gets the same pwm duty cycle
* @apicovered       iotbus_pwm_set_duty_cycle, iotbus_pwm_get_duty_cycle
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_pwm_set_get_duty_cycle_p(void)
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
* @testcase         itc_pwm_set_get_period_p
* @brief            sets and gets the pwm period
* @scenario         sets and gets the same pwm period
* @apicovered       iotbus_pwm_set_period, iotbus_pwm_get_period
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_pwm_set_get_period_p(void)
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
* @testcase         itc_pwm_set_enabled_is_enabled_p
* @brief            checks if pwm state is enabled
* @scenario         sets and gets the same pwm period and checks if pwm state is enabled
* @apicovered       iotbus_pwm_is_enabled
* @precondition     opens the pwm_context
* @postcondition    closes the pwm_context
*/
void itc_pwm_set_enabled_is_enabled_p(void)
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

int itc_pwm_main(void)
{
    itc_pwm_open_close_p();
    itc_pwm_set_get_duty_cycle_p();
    itc_pwm_set_get_period_p();
    itc_pwm_set_enabled_is_enabled_p();

    return 0;
}
