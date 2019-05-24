/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <iotbus/iotbus_adc.h>
#include <iotbus/iotbus_error.h>

iotbus_adc_context_h adc;

static void utc_systemio_adc_init_p(void)
{
	iotbus_adc_context_h m_adc = iotbus_adc_init(0, 1);
	TC_ASSERT_NEQ("iotbus_adc_init", m_adc, NULL);
	adc = m_adc;
	TC_SUCCESS_RESULT();
}

static void utc_systemio_adc_init_n(void)
{
	iotbus_adc_context_h m_adc = iotbus_adc_init(-1, -1);
	TC_ASSERT_EQ("iotbus_adc_init", m_adc, NULL);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_iotbus_adc_set_channel_p(void)
{
	TC_ASSERT_EQ("iotbus_adc_set_channel", iotbus_adc_set_channel(adc, 0), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_iotbus_adc_set_channel_n(void)
{
	TC_ASSERT_NEQ("iotbus_adc_set_channel", iotbus_adc_set_channel(NULL, -1), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_iotbus_adc_get_channel_p(void)
{
	TC_ASSERT_EQ("iotbus_adc_get_channel", iotbus_adc_get_channel(adc), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_iotbus_adc_get_channel_n(void)
{
	TC_ASSERT_NEQ("iotbus_adc_get_channel", iotbus_adc_get_channel(NULL), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_iotbus_adc_get_state_p(void)
{
	TC_ASSERT_GT("iotbus_adc_get_state", iotbus_adc_get_state(adc), IOTBUS_ADC_NONE);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_iotbus_adc_get_state_n(void)
{
	TC_ASSERT_LT("iotbus_adc_get_state", iotbus_adc_get_state(NULL), 0);
	TC_SUCCESS_RESULT();
}

int adc_callback_flag;
void adc_callback_event(int channel, uint32_t data)
{
	adc_callback_flag = 1;
}

static void utc_systemio_iotbus_adc_start_p(void)
{
	adc_callback_flag = 0;
	TC_ASSERT_EQ("iotbus_adc_start", iotbus_adc_start(adc, adc_callback_event), 0);
	TC_ASSERT_EQ("adc_callback_event", adc_callback_flag, 1);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_iotbus_adc_start_n(void)
{
	adc_callback_flag = 0;
	TC_ASSERT_NEQ("iotbus_adc_start", iotbus_adc_start(NULL, NULL), 0);
	TC_ASSERT_EQ("adc_callback_event", adc_callback_flag, 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_iotbus_adc_stop_p(void)
{
	adc_callback_flag = 0;
	TC_ASSERT_EQ("iotbus_adc_stop", iotbus_adc_stop(adc), 0);

	TC_SUCCESS_RESULT();
}

static void utc_systemio_iotbus_adc_stop_n(void)
{
	TC_ASSERT_NEQ("iotbus_adc_stop", iotbus_adc_stop(NULL), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_adc_deinit_n(void)
{
	TC_ASSERT_NEQ("iotbus_adc_deinit", iotbus_adc_deinit(NULL), 0);
	TC_SUCCESS_RESULT();
}

static void utc_systemio_adc_deinit_p(void)
{
	TC_ASSERT_EQ("iotbus_adc_deinit", iotbus_adc_deinit(adc));
	TC_SUCCESS_RESULT();
}

int utc_adc_main(void)
{
	utc_systemio_iotbus_adc_init_p();
	utc_systemio_iotbus_adc_init_n();

	utc_systemio_iotbus_adc_set_channel_p();
	utc_systemio_iotbus_adc_set_channel_n();

	utc_systemio_iotbus_adc_get_channel_p();
	utc_systemio_iotbus_adc_get_channel_n();

	utc_systemio_iotbus_adc_get_state_p();
	utc_systemio_iotbus_adc_get_state_n();

	utc_systemio_iotbus_adc_start_p();
	utc_systemio_iotbus_adc_start_n();

	utc_systemio_iotbus_adc_stop_p();
	utc_systemio_iotbus_adc_stop_n();

	utc_systemio_iotbus_adc_deinit_n();
	utc_systemio_iotbus_adc_deinit_p();

	return 0;
}
