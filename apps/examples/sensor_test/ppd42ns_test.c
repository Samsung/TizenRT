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
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <tinyara/sensors/ppd42ns.h>

#define DUST_SENSOR_DEVNAME		"/dev/dust0"

#if defined(CONFIG_EXAMPLES_SENSOR_TEST_PPD42NS_MQTT_TRANSMISSION)
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
char g_argv_buf[30][100];
char *g_argv_cur[30];

/******************
 * function prototype
 ******************/
int mqtt_client_pub_task(void *arg);

/******************
 * static function
 ******************/
static void create_arg(struct mqtt_pub_input *arg, char **argv_cur, char(*argv)[100], char *cmd)
{
	int i, len, idx1, idx2;

	len = (int)strlen(cmd);
	idx1 = idx2 = 0;
	for (i = 0; i < len; i++) {
		if (cmd[i] == ' ') {
			argv[idx1][idx2] = '\0';
			idx1++;
			idx2 = 0;
			i++;
		}
		argv[idx1][idx2++] = cmd[i];
	}
	argv[idx1][idx2] = '\0';

	arg->argc = idx1 + 1;
	for (i = 0; i < arg->argc; i++) {
		argv_cur[i] = argv[i];
	}
	arg->argv = argv_cur;
}

static int send_sensor_data(char *data)
{
	struct mqtt_pub_input arg;
	char mqtt_str[128];

	snprintf(mqtt_str, 128, "mqtt_pub -h %s -t %s -p %s -m %s", MQTT_BROKER_IP_ADDRESS, MQTT_TOPIC, MQTT_BROKER_PORT, data);

	create_arg(&arg, g_argv_cur, g_argv_buf, mqtt_str);
	mqtt_client_pub_task(&arg);

	return 0;
}
#endif							/* CONFIG_EXAMPLES_SENSOR_TEST_PPD42NS_MQTT_TRANSMISSION */

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
	int fd = -1;
	float sensor_data;
	char msg[64];
	int sampling_interval_ms;
	int test_cnt;
	int loop_cnt;

	if (argc != 3) {
		printf("USAGE: sensor_ppd42ns [sampling_interval_in_ms] [loop_count] \n");
		goto done;
	}

	sampling_interval_ms = atoi(argv[1]);
	test_cnt = atoi(argv[2]);
	loop_cnt = test_cnt;

	/* open ppd42ns sensor driver */
	fd = open(DUST_SENSOR_DEVNAME, O_RDONLY);
	if (fd < 0) {
		printf("ERROR: open() failed. devname=%s\n", DUST_SENSOR_DEVNAME);
		goto done;
	}

	printf(">>> Start ppd42ns dust sensor test\n");
	printf(" - interval: %d ms\n", sampling_interval_ms);
	printf(" - loop_count: %d\n", loop_cnt);
	printf(" - mqtt transmission: ");
#if defined(CONFIG_EXAMPLES_SENSOR_TEST_PPD42NS_MQTT_TRANSMISSION)
	printf("ON\n");
#else
	printf("OFF\n");
#endif

	while (loop_cnt) {
		usleep(sampling_interval_ms * 1000);
		if (read(fd, (void *)&sensor_data, sizeof(float)) == sizeof(float)) {
			/* make sensor data message with json style */
			snprintf(msg, sizeof(msg), "{\"dust\":%.2f}", sensor_data);
			printf("[%d] %s \n", (test_cnt - loop_cnt + 1), msg);

#if defined(CONFIG_EXAMPLES_SENSOR_TEST_PPD42NS_MQTT_TRANSMISSION)
			send_sensor_data(msg);
#endif
		} else {
			printf("ERROR: read() failed. devname=%s, remaining loop_cnt=%d\n", DUST_SENSOR_DEVNAME, loop_cnt - 1);
		}

		loop_cnt--;
	}
	printf("<<< End ppd42ns dust sensor test\n");

	/* close ppd42ns sensor driver */
	if (fd != -1) {
		close(fd);
		fd = -1;
	}

	result = 0;

done:
	return result;
}
