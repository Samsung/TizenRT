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

/// @file clock.c

/// @brief Test Case Example for Clock API

#include <stdio.h>
#include <stdlib.h>
#include <iotbus/iotbus_pwm.h>
#include <iotbus/iotbus_error.h>
#include "utc_internal.h"

iotbus_pwm_context_h pwm;

static void utc_systemio_pwm_open_p(void)
{
	iotbus_pwm_context_h m_pwm = iotbus_pwm_open(0, 1);
	TC_ASSERT_NEQ("iotbus_pwm_open", m_pwm, NULL);
	pwm = m_pwm;
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_open_n(void)
{
	iotbus_pwm_context_h m_pwm = iotbus_pwm_open(-1, -1);
	TC_ASSERT_EQ("iotbus_pwm_open", m_pwm, NULL);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_set_duty_cycle_p(void)
{
	TC_ASSERT_GEQ("iotbus_pwm_set_duty_cycle", iotbus_pwm_set_duty_cycle(pwm, 20), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_set_duty_cycle_n(void)
{
	TC_ASSERT_LT("iotbus_pwm_set_duty_cycle", iotbus_pwm_set_duty_cycle(pwm, 101), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_get_duty_cycle_p(void)
{
	TC_ASSERT_EQ("iotbus_pwm_set_duty_cycle", iotbus_pwm_get_duty_cycle(pwm), 20);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_get_duty_cycle_n(void)
{
	TC_ASSERT_LT("iotbus_pwm_set_duty_cycle", iotbus_pwm_get_duty_cycle(0), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_set_period_p(void)
{
	TC_ASSERT_EQ("iotbus_pwm_set_period", iotbus_pwm_set_period(pwm, 1000), IOTBUS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_set_period_n(void)
{
	TC_ASSERT_LT("iotbus_pwm_set_period", iotbus_pwm_set_period(NULL, 1000), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_get_period_p(void)
{
	TC_ASSERT_EQ("iotbus_pwm_set_period", iotbus_pwm_get_period(pwm), 1000);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_get_period_n(void)
{
	TC_ASSERT_LT("iotbus_pwm_set_period", iotbus_pwm_get_period(NULL), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_set_enabled_p(void)
{
	TC_ASSERT_GEQ("iotbus_pwm_set_enabled", iotbus_pwm_set_enabled(pwm, IOTBUS_PWM_ENABLE), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_set_enabled_n(void)
{
	TC_ASSERT_LT("iotbus_pwm_set_enabled", iotbus_pwm_set_enabled(NULL, IOTBUS_PWM_ENABLE), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_is_enabled_p(void)
{
	TC_ASSERT_EQ("iotbus_pwm_is_enabled", iotbus_pwm_is_enabled(pwm), IOTBUS_PWM_ENABLE);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_is_enabled_n(void)
{
	TC_ASSERT_LT("iotbus_pwm_is_enabled", iotbus_pwm_is_enabled(NULL), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_close_n(void)
{
	TC_ASSERT_LT("iotbus_pwm_close", iotbus_pwm_close(NULL), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_pwm_close_p(void)
{
	TC_ASSERT_EQ("iotbus_pwm_close", iotbus_pwm_close(pwm), IOTBUS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

int utc_pwm_main(void)
{
	utc_systemio_pwm_open_p();
	utc_systemio_pwm_open_n();
	utc_systemio_pwm_set_duty_cycle_p();
	utc_systemio_pwm_set_duty_cycle_n();
	utc_systemio_pwm_get_duty_cycle_p();
	utc_systemio_pwm_get_duty_cycle_n();
	utc_systemio_pwm_set_period_p();
	utc_systemio_pwm_set_period_n();
	utc_systemio_pwm_get_period_p();
	utc_systemio_pwm_get_period_n();
	utc_systemio_pwm_set_enabled_p();
	utc_systemio_pwm_set_enabled_n();
	utc_systemio_pwm_is_enabled_p();
	utc_systemio_pwm_is_enabled_n();
	utc_systemio_pwm_close_n();
	utc_systemio_pwm_close_p();

	return 0;
}
