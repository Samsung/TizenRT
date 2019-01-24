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
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>
#include "tiny_evtmgr.h"

#ifdef LINUX
#define getpid pthread_self
#endif
#define TS_ERR printf("%s:%d\t%s\n", __FILE__, __LINE__, __FUNCTION__);

static void *
delete_msg(tem_msg *msg)
{
	int *data = msg->data;
	printf("T%lu delete data (%d)\n", getpid(), *data);
	free(data);

	return 0;
}


static void *
event_handler(tem_msg *msg)
{
	printf("Event handler111111111111(%p)\n", msg);
	if (msg) {
		int *data = (int *)msg->data;
		printf(">>>>>Receive %d\n", *data);
		free(msg->data);
	}
	return 0;
}

static void *
event_handler2(tem_msg *msg)
{
	printf("Event handler22222222222\n");
	if (msg) {
		int *data = (int *)msg->data;
		printf(">>>>>Receive %d\n", *data);
		free(msg->data);
	}
	return 0;
}


int
test_evt_handler(void)
{
    printf("[EVT_TEST] start event handler\n");
	tem_hnd hnd = 0;
	tem_result tres = tiny_evtmgr_init(&hnd);
	if (tres != TINY_EVTMGR_SUCCESS) {
		TS_ERR;
		return -1;
	}
	printf("[EVT_TEST] start evtmgr\n");
	tiny_evtmgr_start(hnd, event_handler, delete_msg, 0);

	tem_msg msg1;
	int *data1 = (int *)malloc(sizeof(int));
	*data1 = 1;
	msg1.data = data1;
	printf("[EVT_TEST] create msg1(%p) (%p) (%d)\n", &msg1, data1, *data1);
	tiny_evtmgr_add_msg(hnd, &msg1, 0);

	printf("[EVT_TEST] create msg2\n");
	tem_msg msg2;
	int *data2 = (int *)malloc(sizeof(int));
	*data2 = 2;
	msg2.data = data2;
	printf("[EVT_TEST] create msg2(%p) (%p) (%d)\n", &msg2, data2, *data2);
	tiny_evtmgr_add_msg(hnd, &msg2, 0);

	printf("[EVT_TEST] init handler2\n");
	tem_hnd hnd2 = 0;
	tres = tiny_evtmgr_init(&hnd2);
	if (tres != TINY_EVTMGR_SUCCESS) {
		TS_ERR;
		return -1;
	}

	printf("[EVT_TEST] start handler2\n");
	tiny_evtmgr_start(hnd2, event_handler2, delete_msg, 0);

	printf("[EVT_TEST] make msg3\n");
	tem_msg msg3;
	int *data3 = (int *)malloc(sizeof(int));
	*data3 = 11;
	msg3.data = data3;
	printf("[EVT_TEST] create msg3(%p) (%p) (%d)\n", &msg3, data3, *data3);
	tiny_evtmgr_add_msg(hnd2, &msg3, 0);

	printf("[EVT_TEST] stop handler1\n");
	tiny_evtmgr_stop(hnd);

	printf("[EVT_TEST] add msg4\n");
	tem_msg msg4;
	int *data4 = (int *)malloc(sizeof(int));
	*data4 = 22;
	msg4.data = data4;
	printf("[EVT_TEST] create msg4(%p) (%p) (%d)\n", &msg4, data4, *data4);
	tiny_evtmgr_add_msg(hnd2, &msg4, 0);

	printf("[EVT_TEST] stop handler2\n");
	tiny_evtmgr_stop(hnd2);
}
