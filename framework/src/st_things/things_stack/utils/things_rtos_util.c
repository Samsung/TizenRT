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

#include <pthread.h>
#include <sched.h>

#include "things_rtos_util.h"

#define TAG "[things_rtos_util]"

static char thingsStackthreadName[THINGS_STACK_MAX_INDEX][40] = { {"THINGS_STACK_PING"},	/*THINGS_STACK_PING_THREAD */
	{"THINGS_STACK_RESETLOOP"},		/*THINGS_STACK_RESETLOOP_THREAD */
	{"THINGS_STACK_CICONN_INIT"},	/*THINGS_STACK_CICONNETION_INIT_THREAD */
	{"THINGS_STACK_CICONN_WAIT"},	/*THINGS_STACK_CICONNETION_WAIT_THREAD */
	{"THINGS_STACK_WIFIPROV_SET"},	/*THINGS_STACK_WIFI_PROV_SET_THREAD */
	{"THINGS_STACK_CLOUD_REFRESH"},	/*THINGS_STACK_CLOUD_REFRESH_THREAD */
	{"THINGS_STACK_PRESENCE_NOTI"},	/*THINGS_STACK_PRESENCE_NOTI_THREAD */
	{"THINGS_STACK_SERVEREXCE_LOOP"},	/*THINGS_STACK_SERVEREXCETUE_LOOP_THREAD */
	{"THINGS_STACK_BASE_TIME_OUT"},	/*THINGS_STACK_BASE_TIME_OUT_THREAD */
	{"THINGS_STACK_OICABORT"},		/*THINGS_STACK_OICABORT_THREAD */
	{"THINGS_STACK_WIFI_JOIN"},		/*THINGS_STACK_WIFI_JOIN_THREAD */
	{"THINGS_STACK_MAX_INDEX"}		/*THINGS_STACK_MAX_INDEX */
};

/**
 *
 * @param ethreadName
 * @param pstThread
 * @return
 */
char *GetPthreadAttrDetails(things_stack_thread_name ethreadName, pthread_attr_t *pstThread)
{
	if (ethreadName >= THINGS_STACK_MAX_INDEX) {
		ethreadName = THINGS_STACK_MAX_INDEX;
	}

	if (pstThread != NULL) {
		pthread_attr_init(pstThread);
		struct sched_param sparam;
		pthread_attr_init(pstThread);
		pthread_attr_setschedpolicy(pstThread, SCHED_RR);
		sparam.sched_priority = PTHREAD_DEFAULT_PRIORITY;
		pthread_attr_setschedparam(pstThread, &sparam);

		pthread_attr_setstacksize(pstThread, 8193);	/**Default Stack Size**/
		switch (ethreadName) {
		case THINGS_STACK_RESETLOOP_THREAD:
		case THINGS_STACK_PING_THREAD:
		case THINGS_STACK_CICONNETION_INIT_THREAD:
		case THINGS_STACK_CLOUD_REFRESH_THREAD:
		case THINGS_STACK_PRESENCE_NOTI_THREAD:
		case THINGS_STACK_BASE_TIME_OUT_THREAD:
			pthread_attr_setstacksize(pstThread, 4 * 1024);
			break;

		case THINGS_STACK_CICONNETION_WAIT_THREAD:
			pthread_attr_setstacksize(pstThread, 6 * 1024);
			break;
		case THINGS_STACK_SERVEREXCETUE_LOOP_THREAD:
			pthread_attr_setstacksize(pstThread, 6 * 1024);
			/* its the main process thread for all request */
			break;

		default:
			pthread_attr_setstacksize(pstThread, 8193);
			break;
		}
	}

	return thingsStackthreadName[ethreadName];

}

int pthread_create_rtos(FAR pthread_t *thread, FAR const pthread_attr_t *attr, pthread_startroutine_t start_routine, pthread_addr_t arg, things_stack_thread_name eThreadname)
{
	pthread_attr_t stPtr;
	char *pchTname = GetPthreadAttrDetails(eThreadname, &stPtr);
	int ret = pthread_create(thread, &stPtr, start_routine, (void *)arg);
	if (ret == 0) {
		pthread_setname_np(*thread, pchTname);
		pthread_detach(*thread);
	}
	return ret;
}
