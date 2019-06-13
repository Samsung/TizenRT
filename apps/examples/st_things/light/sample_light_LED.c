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

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <tinyara/gpio.h>
#include <tinyara/fs/fs.h>

#include "sample_light_LED.h"

const char *gpio_path_format = "/dev/gpio%d";

int key_get_status(int key, int *status)
{
	int fd_key = -1;
	int ret = ERROR;
	char path[16] = { 0 };

	if (key == KEY_RESET_IO_NUM) {
		sprintf(path, gpio_path_format, KEY_RESET_IO_NUM);
	} else if (key == KEY_SWITCH_IO_NUM) {
		sprintf(path, gpio_path_format, KEY_SWITCH_IO_NUM);
	} else {
		return ret;
	}

	fd_key = open(path, O_RDWR);
	if (fd_key < 0) {
		printf("open key fails\n");
		return ret;
	}

	ioctl(fd_key, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_IN);
	ioctl(fd_key, GPIOIOC_SET_DRIVE, BUTTON_PULL_SET);

	memset(path, 0, sizeof(path));
	ret = read(fd_key, path, 4);
	path[4] = 0;
	*status = atoi(path);

	close(fd_key);
	fd_key = -1;

	return OK;
}


static int g_switch_onoff = 0;
static const int64_t g_dimming_range[] = { 0, 100 };
static const int64_t g_color_range[] = { 0, 100 };

static int fd_led_red = -1;
static int fd_led_green = -1;
static int fd_led_blue = -1;

static int fd_ledex_red = -1;
static int fd_ledex_green = -1;
static int fd_ledex_blue = -1;

int led_open(void)
{
	char path[16] = { 0 };

	sprintf(path, gpio_path_format, LED_RED_IO_NUM);
	fd_led_red = open(path, O_RDWR);
	if (fd_led_red < 0) {
		return ERROR;
	}
	ioctl(fd_led_red, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);

	sprintf(path, gpio_path_format, LED_GREEN_IO_NUM);
	fd_led_green = open(path, O_RDWR);
	if (fd_led_green < 0) {
		return ERROR;
	}
	ioctl(fd_led_green, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);

	sprintf(path, gpio_path_format, LED_BLUE_IO_NUM);
	fd_led_blue = open(path, O_RDWR);
	if (fd_led_blue < 0) {
		return ERROR;
	}
	ioctl(fd_led_blue, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);


	sprintf(path, gpio_path_format, LED_EXT_RED_IO_NUM);
	fd_ledex_red = open(path, O_RDWR);
	if (fd_ledex_red < 0) {
		return ERROR;
	}
	ioctl(fd_ledex_red, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);

	sprintf(path, gpio_path_format, LED_EXT_GREEN_IO_NUM);
	fd_ledex_green = open(path, O_RDWR);
	if (fd_ledex_green < 0) {
		return ERROR;
	}
	ioctl(fd_ledex_green, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);

	sprintf(path, gpio_path_format, LED_EXT_BLUE_IO_NUM);
	fd_ledex_blue = open(path, O_RDWR);
	if (fd_ledex_blue < 0) {
		return ERROR;
	}
	ioctl(fd_ledex_blue, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);

	return OK;
}

static int led_set_status(int led, int status)
{
	char str[4] = { 0 };
	int ret = ERROR;
	int len = sprintf(str, "%d", status > 0);
	if (led == LED_GREEN_IO_NUM) {
		if (fd_led_green >= 0) {
			ret = write(fd_led_green, str, len);
		}
	} else if (led == LED_RED_IO_NUM) {
		if (fd_led_red >= 0) {
			ret = write(fd_led_red, str, len);
		}
	} else if (led == LED_BLUE_IO_NUM) {
		if (fd_led_blue >= 0) {
			ret = write(fd_led_blue, str, len);
		}
	} else if (led == LED_EXT_GREEN_IO_NUM) {
		if (fd_led_green >= 0) {
			ret = write(fd_ledex_green, str, len);
		}
	} else if (led == LED_EXT_RED_IO_NUM) {
		if (fd_led_red >= 0) {
			ret = write(fd_ledex_red, str, len);
		}
	} else if (led == LED_EXT_BLUE_IO_NUM) {
		if (fd_led_blue >= 0) {
			ret = write(fd_ledex_blue, str, len);
		}
	}
	return ret;
}


int led_set_switch(int status)
{
	g_switch_onoff = (status > 0);
	// no led for switch
	return g_switch_onoff;
}


int led_set_dimming(int dimming)
{
	if (g_dimming_range[0] > dimming || g_dimming_range[1] < dimming) {
		return - 1;
	}

	if (g_switch_onoff) { //on
		if (dimming >= 1) {
			led_set_status(LED_RED_IO_NUM, 1);
		} else {
			led_set_status(LED_RED_IO_NUM, 0);
		}

		if (dimming >= 34) {
			led_set_status(LED_GREEN_IO_NUM, 1);
		} else {
			led_set_status(LED_GREEN_IO_NUM, 0);
		}

		if (dimming >= 67) {
			led_set_status(LED_BLUE_IO_NUM, 1);
		} else {
			led_set_status(LED_BLUE_IO_NUM, 0);
		}
	} else { //off
		led_set_status(LED_RED_IO_NUM, 0);
		led_set_status(LED_GREEN_IO_NUM, 0);
		led_set_status(LED_BLUE_IO_NUM, 0);
	}

	return dimming;
}


int led_set_color_temprature(int color_temp)
{
	if (g_color_range[0] > color_temp || g_color_range[1] < color_temp) {
		return -1;
	}

	if (g_switch_onoff) { //on
		if (color_temp >= 1) {
			led_set_status(LED_EXT_RED_IO_NUM, 1);
		} else {
			led_set_status(LED_EXT_RED_IO_NUM, 0);
		}

		if (color_temp >= 34) {
			led_set_status(LED_EXT_GREEN_IO_NUM, 1);
		} else {
			led_set_status(LED_EXT_GREEN_IO_NUM, 0);
		}

		if (color_temp >= 67) {
			led_set_status(LED_EXT_BLUE_IO_NUM, 1);
		} else {
			led_set_status(LED_EXT_BLUE_IO_NUM, 0);
		}
	} else { //off
		led_set_status(LED_EXT_RED_IO_NUM, 0);
		led_set_status(LED_EXT_GREEN_IO_NUM, 0);
		led_set_status(LED_EXT_BLUE_IO_NUM, 0);
	}

	return color_temp;
}

int led_close(void)
{
	if (fd_led_green >= 0) {
		close(fd_led_green);
		fd_led_green = -1;
	}

	if (fd_led_red >= 0) {
		close(fd_led_red);
		fd_led_red = -1;
	}

	if (fd_led_blue >= 0) {
		close(fd_led_blue);
		fd_led_blue = -1;
	}

	if (fd_ledex_green >= 0) {
		close(fd_ledex_green);
		fd_ledex_green = -1;
	}

	if (fd_ledex_red >= 0) {
		close(fd_ledex_red);
		fd_ledex_red = -1;
	}

	if (fd_ledex_blue >= 0) {
		close(fd_ledex_blue);
		fd_ledex_blue = -1;
	}

	return OK;
}

#if 0 // for debugging
int led_utc_ledonoff(void)
{
	int ret = led_open();
	printf("\n\nLED open :%d \n", ret);

	while (OK == key_get_status(KEY_SWITCH_IO_NUM, &ret) &&
		ret != KEY_SWITCH_PRESSED) {
		usleep(10 * 1000);
	}
	printf("SWITCH pressed, start to open LED...\n");

	led_set_status(LED_RED_IO_NUM, 1);
	usleep(200 * 1000);
	led_set_status(LED_GREEN_IO_NUM, 1);
	usleep(200 * 1000);
	led_set_status(LED_BLUE_IO_NUM, 1);
	usleep(200 * 1000);

	led_set_status(LED_EXT_RED_IO_NUM, 1);
	usleep(200 * 1000);
	led_set_status(LED_EXT_GREEN_IO_NUM, 1);
	usleep(200 * 1000);
	led_set_status(LED_EXT_BLUE_IO_NUM, 1);

	printf("LED opened!\n");

	while (OK == key_get_status(KEY_RESET_IO_NUM, &ret) &&
		ret != KEY_RESET_PRESSED) {
		usleep(10 * 1000);
	}
	printf("RESET pressed, start to close LED...\n");

	led_set_status(LED_RED_IO_NUM, 0);
	usleep(200 * 1000);
	led_set_status(LED_GREEN_IO_NUM, 0);
	usleep(200 * 1000);
	led_set_status(LED_BLUE_IO_NUM, 0);
	usleep(200 * 1000);

	led_set_status(LED_EXT_RED_IO_NUM, 0);
	usleep(200 * 1000);
	led_set_status(LED_EXT_GREEN_IO_NUM, 0);
	usleep(200 * 1000);
	led_set_status(LED_EXT_BLUE_IO_NUM, 0);

	ret = led_close();
	printf("LED closed!\n\n\n\n");
	usleep(2000 * 1000);
}
#endif
