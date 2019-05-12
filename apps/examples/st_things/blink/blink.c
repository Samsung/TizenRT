/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include "blink.h"

static int g_power = IOTBUS_GPIO_LOW;
static int is_led_chaged = 0;
static int g_blink_led = 0;

static pthread_t led_thread_handler;
iotbus_gpio_context_h g_led_gpio;

char *power_status[] = {"off", "on"};

const char* get_led_power(void)
{
	return power_status[g_power];
}

static void* blink_loop(void* data)
{
	g_blink_led = 1;

	printf("Blink Start!\n");

	while (g_blink_led) {
		if (is_led_chaged) {
			iotbus_gpio_write(g_led_gpio, g_power);
			is_led_chaged = 0;
		}
		usleep(100 * 1000);
	}
	printf("Blink End!\n");

	return NULL;
}

int set_led_power(char *power)
{
	if (strncmp(power, "off" , strlen("off")) == 0) {
		g_power = IOTBUS_GPIO_LOW;
	} else if (strncmp(power, "on", strlen("on")) == 0) {
		g_power = IOTBUS_GPIO_HIGH;
	} else {
		printf("input Error");
		return -1;
	}
	is_led_chaged = 1;

	return g_power;
}

void blink_init()
{
	iotapi_initialize();
	g_led_gpio = iotbus_gpio_open(LED_POWER_PORT);

	if (g_led_gpio == NULL) {
		printf("Open GPIO Error\n");
		return -1;
	}
	
	iotbus_gpio_set_direction(g_led_gpio, IOTBUS_GPIO_DIRECTION_OUT);
	g_power = iotbus_gpio_read(g_led_gpio);
	return NULL;
}

int blink_start() 
{
	int ret;

	if (g_blink_led) {
		printf("Already Initialized!\n");
		return -1;
	}

	pthread_attr_t thread_attr;
	ret = pthread_attr_init(&thread_attr);
	if (ret != 0) {
		printf("[BLINK]pthread_attr_init error!\n");
		return -1;
	}
	ret = pthread_attr_setstacksize(&thread_attr, 2048);
	if (ret != 0) {
		printf("[BLINK]pthread_attr_setstacksize error!\n");
		return -1;
	}
	
	ret = pthread_create(&led_thread_handler, &thread_attr, blink_loop, (void *)NULL);
	if (ret != 0) {
		printf("[BLINK]pthread_create error!\n");
		return -1;
	}

	ret = pthread_setname_np(led_thread_handler, "blink_loop");
	if (ret != 0) {
		printf("[BLINK]pthread_setname_np error!\n");
		return -1;
	}

	return ret;
}

int blink_end()
{
	g_blink_led = 0;
	pthread_join(led_thread_handler, NULL);

	iotbus_gpio_close(g_led_gpio);
	g_led_gpio = NULL;
	printf("[BLINK]Blink LED Loop Done!\n");
	return 1;
}
