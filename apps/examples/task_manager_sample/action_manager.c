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
#include <semaphore.h>
#include <task_manager/task_manager.h>

#include "task_manager_sample.h"

static int exit_flag;
static int handle_alarm;

static void action(void *info)
{
	int ret;

	char *rec = (char *)malloc(strlen((char *)info) + 1);
	strncpy(rec, info, strlen((char *)info) + 1);

	if (strncmp(rec, "alarm_on", strlen("alarm_on") + 1) == 0) {
		printf("\nAction Manager receives Alarm On request\n");
		ret = task_manager_start(handle_alarm, TM_RESPONSE_WAIT_INF);
		if (ret < 0) {
			printf("FAIL TO START ALARM ACTION, %d\n", ret);
		} else if (ret == OK) {
			printf("Alarm is successfully on\n");
		}
	} else if (strncmp(rec, "alarm_pause", strlen("alarm_pause") + 1) == 0) {
		printf("\nAction Manager receives Alarm Pause request\n");
		ret = task_manager_pause(handle_alarm, TM_RESPONSE_WAIT_INF);
		if (ret < 0) {
			printf("FAIL TO PAUSE ALARM, %d\n", ret);
		} else if (ret == OK) {
			printf("Alarm is successfully paused\n");
		}
	} else if (strncmp(rec, "alarm_resume", strlen("alarm_resume") + 1) == 0) {
		printf("\nAction Manager receives Alarm Resume request\n");
		ret = task_manager_resume(handle_alarm, TM_RESPONSE_WAIT_INF);
		if (ret < 0) {
			printf("FAIL TO RESUME ALARM, %d\n", ret);
		} else if (ret == OK) {
			printf("Alarm is successfully resumed\n");
		}
	} else if (strncmp(rec, "alarm_restart", strlen("alarm_restart") + 1) == 0) {
		printf("\nAction Manager receives Alarm Restart request\n");
		ret = task_manager_restart(handle_alarm, TM_RESPONSE_WAIT_INF);
		if (ret != OK) {
			printf("FAIL TO RESTART ALARM, %d\n", ret);
		} else {
			printf("Alarm is successfully restarted\n");
		}
	} else if (strncmp(rec, "alarm_off", strlen("alarm_off") + 1) == 0) {
		printf("\nAction Manager receives Alarm Off request\n");
		ret = task_manager_stop(handle_alarm, TM_RESPONSE_WAIT_INF);
		if (ret < 0) {
			printf("FAIL TO STOP ALARM ACTION, %d\n", ret);
		} else if (ret == OK) {
			printf("Alarm is successfully off\n");
		}
	} else {
		printf("Unregistered Action\n");
	}
	free(rec);
	exit_flag++;
}

static void action_broad(int info)
{
	switch (info) {
	case TM_BROADCAST_WIFI_ON:
		printf("\nWIFI is On!\n");
		break;

	case TM_BROADCAST_WIFI_OFF:
		printf("\nWIFI is Off!\n");
		break;

	default:
		printf("Unsubscribed Action\n");
		break;
	}
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
	int handle_ledon;
	int handle_ledoff;
	int ret;
	int ret_unregister_alarm;
	int ret_unregister_ledon;
	int ret_unregister_ledoff;

	printf("Action Manager is started\n\nRegister Alarm Action\n");

	exit_flag = 0;

	handle_alarm = task_manager_register("alarm", TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (handle_alarm < 0) {
		printf("FAIL TO REGISTER ALARM ACTION, %d\n", handle_alarm);
	} else if (handle_alarm >= 0) {
		printf("Alarm Action is succeefully registered and its handle is %d\n", handle_alarm);
	}

	printf("\nRegister LED On Action\n");

	handle_ledon = task_manager_register("led_on", TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (handle_ledon < 0) {
		printf("FAIL TO REGISTER LED ON ACTION, %d\n", handle_ledon);
	} else if (handle_ledon >= 0) {
		printf("LED On Action is succeefully registered and its handle is %d\n", handle_ledon);
	}

	printf("\nRegister LED Off Action\n");

	handle_ledoff = task_manager_register("led_off", TM_APP_PERMISSION_ALL, TM_RESPONSE_WAIT_INF);
	if (handle_ledoff < 0) {
		printf("FAIL TO REGISTER LED OFF ACTION, %d\n", handle_ledoff);
	} else if (handle_ledoff >= 0) {
		printf("LED Off Action is succeefully registered and its handle is %d\n", handle_ledoff);
	}

	printf("\nLED On Action Start\n");
	ret = task_manager_start(handle_ledon, TM_RESPONSE_WAIT_INF);
	if (ret < 0) {
		printf("FAIL TO START LED ON ACTION, %d\n", ret);
	} else if (ret == OK) {
		printf("LED On Action is successfully started\n");
	}

	printf("\nLED Off Action Start\n");
	ret = task_manager_start(handle_ledoff, TM_RESPONSE_WAIT_INF);
	if (ret < 0) {
		printf("FAIL TO START LED Off ACTION, %d\n", ret);
	} else if (ret == OK) {
		printf("LED Off Action is successfully started\n");
	}

	task_manager_set_unicast_cb(action);
	task_manager_set_broadcast_cb((TM_BROADCAST_WIFI_ON | TM_BROADCAST_WIFI_OFF), action_broad);
	sem_post(&tm_sem);

	while (exit_flag < 7) {
		usleep(10);
	}

	if (exit_flag >= 7) {
		printf("\nUnregister Alarm Action\n");
		ret_unregister_alarm = task_manager_unregister(handle_alarm, TM_RESPONSE_WAIT_INF);
		if (ret_unregister_alarm < 0) {
			printf("FAIL TO UNREGISTER ALARM ACTION, %d\n", handle_alarm);
		} else if (ret_unregister_alarm == OK) {
			printf("Alarm Action is succeefully unregistered\n");
		}

		printf("\nUnregister LED On Action\n");
		ret_unregister_ledon = task_manager_unregister(handle_ledon, TM_RESPONSE_WAIT_INF);
		if (ret_unregister_ledon < 0) {
			printf("FAIL TO UNREGISTER LED ON ACTION, %d\n", handle_ledon);
		} else if (ret_unregister_ledon == OK) {
			printf("LED On Action is succeefully unregistered\n");
		}

		printf("\nUnregister LED Off Action\n");
		ret_unregister_ledoff = task_manager_unregister(handle_ledoff, TM_RESPONSE_WAIT_INF);
		if (ret_unregister_ledoff < 0) {
			printf("FAIL TO UNREGISTER LED OFF ACTION, %d\n", handle_ledoff);
		} else if (ret_unregister_ledoff == OK) {
			printf("LED Off Action is succeefully unregistered\n");
		}

		if ((ret_unregister_alarm == OK) && (ret_unregister_ledon == OK) && (ret_unregister_ledoff == OK)) {
			printf("\n\nAction Manager is Ended, and Unregister Procedure is Completed!\n");
		} else {
			printf("\n\nAction Manager is Ended, and Unregister Procedure is Failed.\n");
		}
	}

	sem_post(&tm_sem);
	return 0;
}
