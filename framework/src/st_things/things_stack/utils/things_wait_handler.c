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
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#include "utils/things_malloc.h"
#include "logging/things_logger.h"
#include "things_wait_handler.h"

#ifdef __ST_THINGS_RTOS__
#include "things_rtos_util.h"
#endif

#define TAG "[waithandler]"

#define MAX_RANDOM_NUM      2999

#define MAX_SENDHANDLES     25

typedef void *(*PthreadFunc)(void *param);

typedef struct TimeOutManager_s {
	things_timeout_s timeout;
	pthread_t gthreadId;
	OCDoHandle handleVal;
	things_check_time_out_call_func funcName;
} TimeOutManager_s;

static int iSendHandle = 0;
static OCDoHandle gSendHandle[MAX_SENDHANDLES] = { NULL, };

static pthread_mutex_t gREQmutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t gTimeOutmutex = PTHREAD_MUTEX_INITIALIZER;

OCDoHandle things_get_request_handle(void)
{
	int i = 0;
	OCDoHandle result = NULL;

	pthread_mutex_lock(&gREQmutex);
	for (i = iSendHandle - 1; i >= 0; i--) {	// latest gSendHandle Value return.
		if (gSendHandle[i] != NULL) {
			result = gSendHandle[i];
			break;
		}
	}
	pthread_mutex_unlock(&gREQmutex);

	return result;
}

int things_cas_request_handle(OCDoHandle EqualVal, OCDoHandle writeHandleVal)
{
	int i = 0;
	int res = 0;

	pthread_mutex_lock(&gREQmutex);
	for (i = iSendHandle - 1; i >= 0; i--) {	// latest gSendHandle Value check.
		if (gSendHandle[i] == EqualVal) {
			THINGS_LOG_D(TAG, "Found request handler.(%d th : 0x%X)", i, gSendHandle[i]);
			gSendHandle[i] = writeHandleVal;
			res = 1;
			break;
		}
	}
	pthread_mutex_unlock(&gREQmutex);

	return res;
}

bool things_is_empty_request_handle(void)
{
	bool isEmpty = false;

	pthread_mutex_lock(&gREQmutex);
	if (iSendHandle < MAX_SENDHANDLES) {
		isEmpty = true;
	}
	pthread_mutex_unlock(&gREQmutex);

	return isEmpty;
}

OCDoHandle things_add_request_handle(OCDoHandle HandleVal)
{
	OCDoHandle result = NULL;

	pthread_mutex_lock(&gREQmutex);
	if (iSendHandle >= MAX_SENDHANDLES) {
		THINGS_LOG_V(TAG, "gSendHandle array is Full.");
	} else {
		gSendHandle[iSendHandle] = HandleVal;
		result = gSendHandle[iSendHandle];
		iSendHandle++;
	}
	pthread_mutex_unlock(&gREQmutex);

	return result;
}

void things_del_all_request_handle(void)
{
	int i = 0;

	pthread_mutex_lock(&gREQmutex);
	for (i = 0; i < iSendHandle; i++) {
		gSendHandle[i] = NULL;
	}
	iSendHandle = 0;
	pthread_mutex_unlock(&gREQmutex);
}

/************************************************************
 *
 * Thread for Time-Out Process for Handle base
 *
 * Time-Out : DEFAULT_TIMEOUT sec.
 *
 ************************************************************/
static void *handle_base_timeout_loop(TimeOutManager_s *pTimeOutManager)
{
	THINGS_LOG_D(TAG, "Enter.");

	static int cntTimeOutThread = 0;

	pthread_mutex_lock(&gTimeOutmutex);
	cntTimeOutThread++;
	THINGS_LOG_D(TAG, "[HD Start] Count of Time-Out thread is %d.", cntTimeOutThread);
	pthread_mutex_unlock(&gTimeOutmutex);

	sleep(1);

	if (pTimeOutManager == NULL || pTimeOutManager->gthreadId < 0 || pTimeOutManager->funcName == NULL || pTimeOutManager->timeout.cur_counter <= 0) {
		THINGS_LOG_E(TAG, "Parameter is invalid.");
		if (pTimeOutManager) {
			things_free(pTimeOutManager);
		}

		return 0;
	}

	THINGS_LOG_D(TAG, "handleVal=0x%X, timeCounter=%d, funcName=0x%X", pTimeOutManager->handleVal, pTimeOutManager->timeout.cur_counter, pTimeOutManager->funcName);

	while (things_get_request_handle() != NULL && pTimeOutManager->handleVal == things_get_request_handle()) {	// things_get_request_handle() != NULL will guarantee for Connecting to AP.
		sleep(1);

		if (--pTimeOutManager->timeout.cur_counter == 0) {
			if (things_cas_request_handle(pTimeOutManager->handleVal, NULL) == 1) {	// things_cas_request_handle(handle, NULL) == 1 will guarantee that AP is connected and handle is latest important request.
				pTimeOutManager->funcName(&pTimeOutManager->timeout);
			}
			pTimeOutManager->handleVal = (void *)(-1);
		}
	}

	things_free(pTimeOutManager);
	pthread_mutex_lock(&gTimeOutmutex);
	cntTimeOutThread--;
	THINGS_LOG_D(TAG, "[HD End] Count of Time-Out thread is %d.", cntTimeOutThread);
	pthread_mutex_unlock(&gTimeOutmutex);

	THINGS_LOG_D(TAG, "Exit.");

	return 0;
}

long long things_next_time_out(const long long ori_num, const long long cur_num)
{
	long long x = 0;
	long long nextT = -1;

	if (ori_num <= 0 || cur_num <= 0) {
		THINGS_LOG_V(TAG, "Not Enabled PDF re-try.");
		return nextT;
	}

	while (cur_num >= nextT) {
		x = (long long)rand() * (long long)(MAX_RANDOM_NUM + 1);
// pkes        x = x / (long long)((unsigned int)MAX_RAND+(unsigned int)1);
		x = x / (long long)((unsigned int)RAND_MAX + (unsigned int)1);
		x = x + 500;

		if (x < 500 || x > MAX_RANDOM_NUM + 500) {
			THINGS_LOG_V(TAG, "X value invalid.");
			break;
		}

		nextT = ((long long)ori_num * ((long long)10000000000000 + x * x * x * x)) / (long long)10000000000000;
		THINGS_LOG_D(TAG, "Gaussian next Time-Out Number: nextT = %lld", nextT);
	}

	THINGS_LOG_D(TAG, "next Time-Out: %lld", nextT);
	return nextT;
}

unsigned long int things_create_time_out_process(OCDoHandle hadler, things_check_time_out_call_func CallFunc, things_timeout_s *timeOut)
{
	TimeOutManager_s *pTimeOutManager = NULL;

	if ((pTimeOutManager = (TimeOutManager_s *) things_malloc(sizeof(TimeOutManager_s))) == NULL || CallFunc == NULL) {
		THINGS_LOG_E(TAG, "Memory allocation is failed.");
		return 0;
	}

	pTimeOutManager->gthreadId = NULL;
	pTimeOutManager->handleVal = hadler;
	pTimeOutManager->funcName = CallFunc;
	if (timeOut == NULL) {
		pTimeOutManager->timeout.cur_counter = DEFAULT_TIMEOUT;	// default second.
		pTimeOutManager->timeout.ori_num = DEFAULT_TIMEOUT;	// default second.
		pTimeOutManager->timeout.cur_num = DEFAULT_TIMEOUT;	// default second.
	} else {
		pTimeOutManager->timeout.cur_counter = timeOut->cur_counter;
		pTimeOutManager->timeout.ori_num = timeOut->ori_num;
		pTimeOutManager->timeout.cur_num = timeOut->cur_num;
	}


	if (pthread_create_rtos(&pTimeOutManager->gthreadId, NULL, (PthreadFunc) handle_base_timeout_loop, (void *)pTimeOutManager, THINGS_STACK_TIME_OUT_THREAD) != 0)	{
		THINGS_LOG_E(TAG, "Create thread is failed.");
		things_free(pTimeOutManager);
		return 0;
	}

	return pTimeOutManager->gthreadId;
}
