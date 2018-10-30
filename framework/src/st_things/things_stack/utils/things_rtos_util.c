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

#define _POSIX_C_SOURCE 200809L

#include <sys/types.h>
#include <pthread.h>
#include <sched.h>

#include "things_rtos_util.h"

#define TAG "[things_rtos_util]"

#define THINGS_STACK_MAX_THREAD_NAME_LEN 40

typedef const struct thread_info_s {
	char thread_name[THINGS_STACK_MAX_THREAD_NAME_LEN];
	int thread_size;
} thread_info_s;

static const thread_info_s things_stack_thread_name[THINGS_STACK_MAX_INDEX] = { {"THINGS_STACK_PING", 4 * 1024},	/*THINGS_STACK_PING_THREAD */
	{"THINGS_STACK_RESET", 4 * 1024},		/*THINGS_STACK_RESETLOOP_THREAD */
	{"THINGS_STACK_CLOUD_CONNECT", 4 * 1024},	/*THINGS_STACK_CICONNETION_INIT_THREAD */
	{"THINGS_STACK_CICONN_WAIT", 6 * 1024},	/*THINGS_STACK_CICONNETION_WAIT_THREAD */
	{"THINGS_STACK_CLOUD_TOKEN_CHECK", 4 * 1024},	/*THINGS_STACK_CLOUD_TOKEN_CHECK_THREAD */
	{"THINGS_STACK_PRESENCE_NOTI", 4 * 1024},	/*THINGS_STACK_PRESENCE_NOTI_THREAD */
	{"THINGS_STACK_SERVEREXCE_LOOP", 8 * 1024},	/*THINGS_STACK_SERVEREXCETUE_LOOP_THREAD */
	{"THINGS_STACK_TIME_OUT", 4 * 1024},	/*THINGS_STACK_TIME_OUT_THREAD */
	{"THINGS_STACK_ABORT", 4 * 1024},		/*THINGS_STACK_ABORT_THREAD */
	{"THINGS_STACK_WIFI_JOIN", 8 * 1024},		/*THINGS_STACK_WIFI_JOIN_THREAD */
	{"THINGS_STACK_FOTA_UPDATE", 6 * 1024},	/*THINGS_STACK_FOTA_THREAD */
	{"THINGS_STACK_AP_INFO_SET", 4 * 1024},/*THINGS_STACK_AP_INFO_SET_THREAD */
	{"THINGS_STACK_AP_SCAN_THREAD", 1024},/*THINGS_STACK_AP_SCAN_THREAD */
	{"THINGS_STACK_MAX_INDEX", 8 * 1024}		/*THINGS_STACK_MAX_INDEX */
};

char *get_pthread_attr_details(things_stack_thread_name_e e_thread_name, pthread_attr_t *pstThread)
{
	if (e_thread_name >= THINGS_STACK_MAX_INDEX) {
		e_thread_name = THINGS_STACK_MAX_INDEX;
	}

	if (pstThread != NULL) {
		pthread_attr_init(pstThread);
		struct sched_param sparam;
		pthread_attr_init(pstThread);
		pthread_attr_setschedpolicy(pstThread, SCHED_RR);
		sparam.sched_priority = PTHREAD_DEFAULT_PRIORITY;
		pthread_attr_setschedparam(pstThread, &sparam);

		pthread_attr_setstacksize(pstThread, things_stack_thread_name[e_thread_name].thread_size);
	}

	return things_stack_thread_name[e_thread_name].thread_name;
}

int pthread_create_rtos(FAR pthread_t *thread, FAR const pthread_attr_t *attr, pthread_startroutine_t start_routine, pthread_addr_t arg, things_stack_thread_name_e e_thread_name)
{
	pthread_attr_t st_ptr;
	char *pch_tname = get_pthread_attr_details(e_thread_name, &st_ptr);
	int ret = pthread_create(thread, &st_ptr, start_routine, (void *)arg);
	if (ret == 0) {
		pthread_setname_np(*thread, pch_tname);
		pthread_detach(*thread);
	}
	return ret;
}
