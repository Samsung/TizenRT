/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
 * examples/iotbus_test/iotbus_test_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>

#include <tinyara/gpio.h>
#include <iotbus/iotbus_gpio.h>
#include <iotbus/iotbus_error.h>


/* To work this sample, below GPIO numbers should be provided.
   Refer Kconfig of this sample.
 */
#if !defined(CONFIG_EXAMPLES_IOTBUS_TEST_RED_LED_GPIO_NUM) ||\
	!defined(CONFIG_EXAMPLES_IOTBUS_TEST_BLUE_LED_GPIO_NUM) ||\
	!defined(CONFIG_EXAMPLES_IOTBUS_TEST_LEFT_BUTTON_GPIO_NUM) ||\
	!defined(CONFIG_EXAMPLES_IOTBUS_TEST_RIGHT_BUTTON_GPIO_NUM)
#error To run this sample, input GPIO numbers on menuconfig.
#endif

#define RED_LED_GPIO      CONFIG_EXAMPLES_IOTBUS_TEST_RED_LED_GPIO_NUM
#define BLUE_LED_GPIO     CONFIG_EXAMPLES_IOTBUS_TEST_BLUE_LED_GPIO_NUM
#define LEFT_BUTTON_GPIO  CONFIG_EXAMPLES_IOTBUS_TEST_LEFT_BUTTON_GPIO_NUM
#define RIGHT_BUTTON_GPIO CONFIG_EXAMPLES_IOTBUS_TEST_RIGHT_BUTTON_GPIO_NUM

#define BUTTON_RELEASE 0
#define BUTTON_PRESS   1

#define LED_ON  IOTBUS_GPIO_LOW
#define LED_OFF IOTBUS_GPIO_HIGH

/****************************************************************************
 * iotbus_main
 ****************************************************************************/
iotbus_gpio_context_h r_led;
iotbus_gpio_context_h b_led;
iotbus_gpio_context_h left_btn;
iotbus_gpio_context_h right_btn;

static void gpio_event_callback(void *user_data)
{
	iotbus_gpio_context_h hnd = (iotbus_gpio_context_h)user_data;

	if (hnd == left_btn) {
		int left_btn_read = iotbus_gpio_read(left_btn);

		if (left_btn_read == BUTTON_PRESS) {
			iotbus_gpio_write(r_led, LED_ON);
		} else if (left_btn_read == BUTTON_RELEASE) {
			iotbus_gpio_write(r_led, LED_OFF);
		}
	} else if (hnd == right_btn) {
		int right_btn_read = iotbus_gpio_read(right_btn);

		if (right_btn_read == BUTTON_PRESS) {
			iotbus_gpio_write(b_led, LED_ON);
		} else if (right_btn_read == BUTTON_RELEASE) {
			iotbus_gpio_write(b_led, LED_OFF);
		}
	}
	return;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int iotbus_test_main(int argc, char *argv[])
#endif
{
	int ret;

	printf("IOTBUS EXAMPLE!\n");

	iotapi_initialize();

	r_led = iotbus_gpio_open(RED_LED_GPIO);
	iotbus_gpio_set_direction(r_led, GPIO_DIRECTION_OUT);
	b_led = iotbus_gpio_open(BLUE_LED_GPIO);
	iotbus_gpio_set_direction(b_led, GPIO_DIRECTION_OUT);
	left_btn = iotbus_gpio_open(LEFT_BUTTON_GPIO);
	iotbus_gpio_set_direction(left_btn, GPIO_DIRECTION_IN);
	right_btn = iotbus_gpio_open(RIGHT_BUTTON_GPIO);
	iotbus_gpio_set_direction(right_btn, GPIO_DIRECTION_IN);

	printf("Press R/B Test Button!!!\n");

	ret = iotbus_gpio_register_cb(left_btn, IOTBUS_GPIO_EDGE_BOTH, gpio_event_callback, (void *)left_btn);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("Registering LeftBtn Callback Error!\n");

		goto iotbus_err;
	}

	ret = iotbus_gpio_register_cb(right_btn, IOTBUS_GPIO_EDGE_BOTH, gpio_event_callback, (void *)right_btn);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("Registering RightBtn Callback Error!\n");

		goto iotbus_err;
	}

	return 0;

iotbus_err:
	ret = iotbus_gpio_unregister_cb(left_btn);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("Unregistering LefttBtn Callback Error!\n");
	}
	ret = iotbus_gpio_unregister_cb(right_btn);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("Unregistering RightBtn Callback Error!\n");
	}

	iotbus_gpio_close(r_led);
	iotbus_gpio_close(b_led);
	iotbus_gpio_close(left_btn);
	iotbus_gpio_close(right_btn);

	return -1;
}
