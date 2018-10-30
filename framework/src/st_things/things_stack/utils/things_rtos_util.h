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

#ifndef THINGS_RTOS_UTIL_H_
#define THINGS_RTOS_UTIL_H_
#include <sys/types.h>
#include <pthread.h>

typedef enum {
	THINGS_STACK_PING_THREAD = 0,
	THINGS_STACK_RESETLOOP_THREAD,
	THINGS_STACK_CICONNETION_INIT_THREAD,
	THINGS_STACK_CICONNETION_WAIT_THREAD,
	THINGS_STACK_CLOUD_TOKEN_CHECK_THREAD,
	THINGS_STACK_PRESENCE_NOTI_THREAD,
	THINGS_STACK_SERVEREXCETUE_LOOP_THREAD,
	THINGS_STACK_TIME_OUT_THREAD,
	THINGS_STACK_ABORT_THREAD,
	THINGS_STACK_WIFI_JOIN_THREAD,
	THGINS_STACK_FOTA_UPDATE_THREAD,
	THINGS_STACK_AP_INFO_SET_THREAD,
	THINGS_STACK_AP_SCAN_THREAD,
	THINGS_STACK_MAX_INDEX,
} things_stack_thread_name_e;

int pthread_create_rtos(FAR pthread_t *thread, FAR const pthread_attr_t *attr, pthread_startroutine_t start_routine, pthread_addr_t arg, things_stack_thread_name_e eThreadname);

#endif							/* THINGS_RTOS_UTIL_H_ */
