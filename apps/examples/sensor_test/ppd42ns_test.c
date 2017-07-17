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
/**
 * @file ppd42ns_test.c
 * @brief the program for testing ppd42ns dust sensor
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tinyara/sensors/sensor.h>
#include <tinyara/sensors/ppd42ns.h>

#define SUPPORT_MQTT_TRANSMISSION			0

#if SUPPORT_MQTT_TRANSMISSION==1
/**************
 * Definitions
 **************/
#define MQTT_BROKER_IP_ADDRESS		"192.168.1.14"
#define MQTT_BROKER_PORT			"1883"
#define MQTT_TOPIC					"/test/sensor"

/**************
 * Structure
 **************/
struct mqtt_pub_input {
	int argc;
	char **argv;
};

/**************
 * Private Data
 **************/
char argv_buf[30][100];
char *argv_cur[30];

/******************
 * function prototype
 ******************/
void create_arg(struct mqtt_pub_input *arg, char **argv_cur, char(*argv)[100], char *cmd);
int mqtt_client_pub_main(int argc, char *argv[]);

/******************
 * static function
 ******************/
static int send_sensor_data(char *data)
{
	struct mqtt_pub_input arg;
	char mqtt_str[128];

	snprintf(mqtt_str, 128, "mqtt_pub -h %s -t %s -p %s -m %s", MQTT_BROKER_IP_ADDRESS, MQTT_TOPIC, MQTT_BROKER_PORT, data);

	create_arg(&arg, argv_cur, argv_buf, mqtt_str);
	mqtt_client_pub_cb(&arg);

	return 0;
}
#endif							/* SUPPORT_MQTT_TRANSMISSION */

/****************************************************************************
 * main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ppd42ns_test_main(int argc, char *argv[])
#endif
{
	int result = -1;

	sensor_data_t data;
	sensor_device_t *dust;
	sensor_ioctl_value_t val;
	char msg[64];
	int sampling_interval_ms;
	int test_cnt;
	int loop_cnt;

	if (argc != 3) {
		printf("USAGE: sensor_ppd42ns [sampling_interval_in_ms] [loop_count] \n");
		goto done_without_init;
	}
	sampling_interval_ms = atoi(argv[1]);
	test_cnt = atoi(argv[2]);
	loop_cnt = test_cnt;

	dust = SENSOR_GET_HANDLE(SENSOR_NAME_PPD42NS);
	if (dust == NULL) {
		printf("ERROR: dust sensor handle is NULL.\n");
		goto done_without_init;
	}

	SENSOR_INIT(dust);

	printf(">>> Start \"%s\" dust sensor test\n", SENSOR_GET_NAME_STRING(SENSOR_NAME_PPD42NS));
	printf(" - interval: %d ms\n", sampling_interval_ms);
	printf(" - loop_count: %d\n", loop_cnt);
	printf(" - mqtt transmission: ");
#if SUPPORT_MQTT_TRANSMISSION==1
	printf("ON\n");
#else
	printf("OFF\n");
#endif
	printf(" - sensor type: %d\n", SENSOR_GET_DEVICE_TYPE(dust));
	if (SENSOR_IOCTL(dust, PPD42NS_IOCTL_ID_GET_GPIO_DEVPATH, &val) != 0) {
		printf("ERROR: SENSOR_IOCTL() failed.\n");
		goto done;
	}
	printf(" - sensor gpio devpath: %s\n", val.p_str);

	SENSOR_ACTIVATE(dust);
	while (loop_cnt) {
		usleep(sampling_interval_ms * 1000);

		if (SENSOR_GET_DATA(dust, &data) == 0) {
			/* make sensor data message with json style */
			sprintf(msg, "{\"dust\":%.2f}", data.fval);
			printf("[%d] %s \n", (test_cnt - loop_cnt + 1), msg);

#if SUPPORT_MQTT_TRANSMISSION==1
			send_sensor_data(msg);
#endif
		}

		loop_cnt--;
	}
	SENSOR_DEACTIVATE(dust);
	printf("<<< End \"%s\" dust sensor test\n", SENSOR_GET_NAME_STRING(SENSOR_NAME_PPD42NS));

	result = 0;

done:
	SENSOR_DEINIT(dust);

done_without_init:
	return result;
}
