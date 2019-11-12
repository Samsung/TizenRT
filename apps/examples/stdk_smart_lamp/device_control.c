/* ***************************************************************************
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
#include "device_control.h"
#include <iotbus/iotbus_gpio.h>
#include "iot_os_util.h"

static iot_os_queue *button_event_queue = NULL;
static iotbus_gpio_context_h g_pir;
extern void iotapi_initialize(void);

int gpio_set_level(int port, int value)
{
	//the gpio value need to refer the specific lamp device
	value = (value == 0) ? NOTIFICATION_LED_GPIO_OFF : NOTIFICATION_LED_GPIO_ON;

	iotbus_gpio_context_h ibctx = iotbus_gpio_open(port);
	iotbus_gpio_write(ibctx, value);
	iotbus_gpio_close(ibctx);
	return OK;
}

static int gpio_get_level(int port)
{
	int result = 0;
	iotbus_gpio_context_h ibctx = iotbus_gpio_open(port);
	result = iotbus_gpio_read(ibctx);
	iotbus_gpio_close(ibctx);
	return result;
}

static float calculate_rgb(float v1, float v2, float vh)
{
	if (vh < 0) {
		vh += 1;
	}
	if (vh > 1) {
		vh -= 1;
	}

	if ((6 * vh) < 1) {
		return (v1 + (v2 - v1) * 6 * vh);
	}
	
	if ((2 * vh) < 1) {
		return v2;
	}

	if ((3 * vh) < 2) {
		return (v1 + (v2 - v1) * ((2.0f / 3) - vh) * 6);
	}
	
	return v1;
}

/* SmartThings manage color by using Hue-Saturation format,
   If you use LED by using RGB color format, you need to change color format */
void update_rgb_from_hsl(double hue, double saturation, int level,
		int *red, int *green, int *blue)
{
	if (saturation == 0) {
		*red = *green = *blue = 255;
		return;
	}

	float v1, v2;
	float h = ((float) hue) / 100;
	float s = ((float) saturation) / 100;
	float l = ((float) level) / 100;

	if (l < 0.5) {
		v2 = l * (1 + s);
	} else {
		v2 = l + s - l * s;
	}

	v1 = 2 * l - v2;

	*red   = (int)(255 * calculate_rgb(v1, v2, h + (1.0f / 3)));
	*green = (int)(255 * calculate_rgb(v1, v2, h));
	*blue  = (int)(255 * calculate_rgb(v1, v2, h - (1.0f / 3)));
}

void button_isr_handler(void *arg)
{
	static uint32_t last_time_ms = 0;
	uint32_t now_ms = TICK2MSEC(clock_systimer());

	/* check debounce time to ignore small ripple of currunt */
	if (now_ms - last_time_ms > BUTTON_DEBOUNCE_TIME_MS) {
		last_time_ms = now_ms;
		iot_os_queue_send(button_event_queue, &now_ms, 0xFFFFFFFF);
	}
}

bool get_button_event(int* button_event_type, int* button_event_count)
{
	static uint32_t button_count = 0;
	static uint32_t long_press_count = 0;
	static uint32_t button_last_time_ms = 0;

	uint32_t button_time_ms = 0;
	uint32_t now_ms = 0;
	uint32_t gpio_level = 0;

	now_ms = TICK2MSEC(clock_systimer());
	if (iot_os_queue_receive(button_event_queue, &button_time_ms, 0)) {
		vTaskDelay(BUTTON_DEBOUNCE_TIME_MS / portTICK_PERIOD_MS);
		gpio_level = iotbus_gpio_read(g_pir);
		printf("Button intr, val: %d, time: %d\n", gpio_level, button_time_ms);
		if (gpio_level == BUTTON_GPIO_PRESSED) {
			button_count++;
			if (button_time_ms - button_last_time_ms > BUTTON_LONG_THRESHOLD_MS) {
				long_press_count = 0;
			}
		}
		button_last_time_ms = button_time_ms;
	} else if (button_count > 0) {
		gpio_level = iotbus_gpio_read(g_pir);
		if ((gpio_level == BUTTON_GPIO_PRESSED)
				&& (now_ms - button_last_time_ms > BUTTON_LONG_THRESHOLD_MS)) {
			long_press_count++;
			*button_event_type = BUTTON_LONG_PRESS;
			*button_event_count = long_press_count;
			return true;
		} else if ((gpio_level == BUTTON_GPIO_RELEASED)
				&& (now_ms - button_last_time_ms > BUTTON_DELAY_MS)) {
			*button_event_type = BUTTON_SHORT_PRESS;
			*button_event_count = button_count;
			long_press_count = 0;
			button_count = 0;
			return true;
		}
	}

	return false;
}

void led_blink(int gpio, int delay, int count)
{
	uint32_t gpio_level;

	gpio_level =  gpio_get_level(gpio);
	for (int i = 0; i < count; i++) {
		gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED, 1 - gpio_level);
		vTaskDelay(delay / portTICK_PERIOD_MS);
		gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED, gpio_level);
		vTaskDelay(delay / portTICK_PERIOD_MS);
	}
}

void change_led_state(int noti_led_mode)
{
	static uint32_t led_last_time_ms = 0;
	uint32_t now_ms = 0;
	uint32_t gpio_level = 0;

	now_ms = TICK2MSEC(clock_systimer());
	switch (noti_led_mode) {
	case LED_ANIMATION_MODE_IDLE:
		break;
	case LED_ANIMATION_MODE_SLOW:
		gpio_level =  gpio_get_level(GPIO_OUTPUT_NOTIFICATION_LED);
		if ((gpio_level == NOTIFICATION_LED_GPIO_ON) && (now_ms - led_last_time_ms > 200)) {
			gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED, NOTIFICATION_LED_GPIO_OFF);
			led_last_time_ms = now_ms;
		}
		if ((gpio_level == NOTIFICATION_LED_GPIO_OFF) && (now_ms - led_last_time_ms > 1000)) {
			gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED, NOTIFICATION_LED_GPIO_ON);
			led_last_time_ms = now_ms;
		}
		break;
	case LED_ANIMATION_MODE_FAST:
		gpio_level =  gpio_get_level(GPIO_OUTPUT_NOTIFICATION_LED);
		if ((gpio_level == NOTIFICATION_LED_GPIO_ON) && (now_ms - led_last_time_ms > 100)) {
			gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED, NOTIFICATION_LED_GPIO_OFF);
			led_last_time_ms = now_ms;
		}
		if ((gpio_level == NOTIFICATION_LED_GPIO_OFF) && (now_ms - led_last_time_ms > 100)) {
			gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED, NOTIFICATION_LED_GPIO_ON);
			led_last_time_ms = now_ms;
		}
		break;
	default:
		break;
	}
}

void gpio_init(void)
{
	printf("gpio_init start\n");
	button_event_queue = iot_os_queue_create(10, sizeof(uint32_t));

	iotapi_initialize();
	g_pir = iotbus_gpio_open(GPIO_INPUT_BUTTON);
	iotbus_gpio_register_cb(g_pir, GPIO_INPUT_BUTTON_EDGE, button_isr_handler, NULL);
	
	//Set RGB LED GPIO to output mode
	iotbus_gpio_context_h n_handle;
	n_handle = iotbus_gpio_open(GPIO_OUTPUT_NOTIFICATION_LED);
	iotbus_gpio_set_direction(n_handle, GPIO_DIRECTION_OUT);
	iotbus_gpio_close(n_handle);

	n_handle = iotbus_gpio_open(GPIO_OUTPUT_COLORLED_R);
	iotbus_gpio_set_direction(n_handle, GPIO_DIRECTION_OUT);
	iotbus_gpio_close(n_handle);

	n_handle = iotbus_gpio_open(GPIO_OUTPUT_COLORLED_G);
	iotbus_gpio_set_direction(n_handle, GPIO_DIRECTION_OUT);
	iotbus_gpio_close(n_handle);

	n_handle = iotbus_gpio_open(GPIO_OUTPUT_COLORLED_B);
	iotbus_gpio_set_direction(n_handle, GPIO_DIRECTION_OUT);
	iotbus_gpio_close(n_handle);

	gpio_set_level(GPIO_OUTPUT_NOTIFICATION_LED, NOTIFICATION_LED_GPIO_ON);
	gpio_set_level(GPIO_OUTPUT_COLORLED_R, 0);
	gpio_set_level(GPIO_OUTPUT_COLORLED_G, 0);
	gpio_set_level(GPIO_OUTPUT_COLORLED_B, 0);
	printf("gpio_init end\n");
}
