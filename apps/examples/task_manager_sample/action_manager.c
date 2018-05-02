/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <task_manager/task_manager.h>

static int exit_flag;
static int tid_alarm;
static int tid_ledon;
static int tid_ledoff;

static void action(int signo, tm_msg_t *info)
{
	int ret_start_alarm;
	int ret_terminate_alarm;

	char *rec = (char *)malloc(strlen((char *)info->si_value.sival_ptr) + 1);
	strncpy(rec, info->si_value.sival_ptr, strlen((char *)info->si_value.sival_ptr));

	if (strncmp(rec, "alarm_on", strlen("alarm_on") + 1) == 0) {
		printf("\nAction Manager receives Alarm On request\n");
		ret_start_alarm = task_manager_start(tid_alarm, TM_RESPONSE_WAIT_INF);
		if (ret_start_alarm < 0) {
			printf("FAIL TO START Alarm On Action, %d\n", ret_start_alarm);
		} else if (ret_start_alarm == 0) {
			printf("Alarm On Action is successfully started\n");
		}
	} else if (strncmp(rec, "alarm_off", strlen("alarm_off") + 1) == 0) {
		printf("\nAction Manager receives Alarm Off request\n");
		ret_terminate_alarm = task_manager_terminate(tid_alarm, TM_RESPONSE_WAIT_INF);
		if (ret_terminate_alarm < 0) {
			printf("FAIL TO START Alarm Off Action, %d\n", ret_terminate_alarm);
		} else if (ret_terminate_alarm == 0) {
			printf("Alarm Off Action is successfully started\n");
		}
	} else {
		printf("Unregistered Action\n");
	}
	free(rec);
	exit_flag++;
}

/****************************************************************************
 * action_manager_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int action_manager_main(int argc, char *argv[])
#endif
{
	int ret_start_ledon;
	int ret_start_ledoff;

	printf("Action Manager is started\n");

	printf("\nRegister Alarm Action\n");

	exit_flag = 0;

	tid_alarm = task_manager_register("alarm", TM_TASK_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (tid_alarm < 0) {
		printf("FAIL TO REGISTER Alarm Action, %d\n", tid_alarm);
	} else if (tid_alarm > 0) {
		printf("Alarm Action is succeefully registered and its tid is %d\n", tid_alarm);
	}

	printf("\nRegister LED On Action\n");

	tid_ledon = task_manager_register("led_on", TM_TASK_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (tid_ledon < 0) {
		printf("FAIL TO REGISTER LED On Action, %d\n", tid_ledon);
	} else if (tid_ledon > 0) {
		printf("LED On Action is succeefully registered and its tid is %d\n", tid_ledon);
	}

	printf("\nRegister LED Off Action\n");

	tid_ledoff = task_manager_register("led_off", TM_TASK_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (tid_ledoff < 0) {
		printf("FAIL TO REGISTER LED Off Action, %d\n", tid_ledoff);
	} else if (tid_ledoff > 0) {
		printf("LED Off Action is succeefully registered and its tid is %d\n", tid_ledoff);
	}

	printf("\nLED On Action\n");
	ret_start_ledon = task_manager_start(tid_ledon, TM_RESPONSE_WAIT_INF);
	if (ret_start_ledon < 0) {
		printf("FAIL TO START LED On Action, %d\n", ret_start_ledon);
	} else if (ret_start_ledon > 0) {
		printf("LED On Action is successfully started\n");
	}

	sleep(1);
	printf("\nLED Off Action\n");
	ret_start_ledoff = task_manager_start(tid_ledoff, TM_RESPONSE_WAIT_INF);
	if (ret_start_ledoff < 0) {
		printf("FAIL TO START LED Off Action, %d\n", ret_start_ledoff);
	} else if (ret_start_ledoff > 0) {
		printf("LED Off Action is successfully started\n");
	}

	task_manager_set_handler(action);
	while (1) {
		usleep(10);
		if (exit_flag >= 2) {
			break;
		}
	}
	if (exit_flag >= 2) {
		printf("\n\nTask Manager Sample is successfully Done.\n");
	} else {
		printf("\n\nTask Manager Sample is failed.\n");
	}
	return 0;
}
