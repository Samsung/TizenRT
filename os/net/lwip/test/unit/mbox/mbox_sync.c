/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <assert.h>
#include <pthread.h>
#include <queue.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stress_tool/st_perf.h>
#include <sys/types.h>
#include <unistd.h>

#include "mbox_util.h"
#include "port/port.h"
#include "sys_arch.h"

#define TCP_QUEUE_SIZE 50
#define QUEUE_MSG_CNT 100
#define FETCH_TIMEOUT 30000 // 30ms

static sys_mbox_t g_tcpmbox;
static sem_t *g_signal_atoc = NULL;
static sem_t *g_signal_ctoa = NULL;
static sem_t *g_signal_btoc = NULL;
static sem_t *g_signal_ctob = NULL;

void mbox_sync_event1(void)
{
	printf("[EVT] task A call wait not_empty signal \t%s:%d\n", __FUNCTION__,
			 __LINE__);
}

void mbox_sync_event2(uint32_t res)
{
	printf("[EVT] res %x %x \t%s:%d\n", res, SYS_ARCH_TIMEOUT, __FUNCTION__,
			 __LINE__);
	if (res != SYS_ARCH_TIMEOUT && (g_tcpmbox.front == g_tcpmbox.rear)) {
		/*	if result is not timeout while mbox is empty then it receives invalid
		 * event */
		printf("[EVT] receive invalid signal %d %d\t%s:%d\n", res, SYS_ARCH_TIMEOUT,
				 __FUNCTION__, __LINE__);
		assert(0);
	}

	printf(
		"[EVT] let control task know that timeout happened int task A "
		"\t%s:%d\n",
		__FUNCTION__, __LINE__);
	printf("[EVT] wait_fetch %d \t%s:%d\n", g_tcpmbox.wait_fetch, __FUNCTION__,
			 __LINE__);
	sem_post(g_signal_atoc);
	sem_wait(g_signal_ctoa);
}

void mbox_sync_event3(void)
{
	printf(
		"[EVT] there is no task waiting not_empty signal. \t%s:%d\n",
		__FUNCTION__, __LINE__);
	printf(
		"[EVT] However task b "
		"generates additional signal which mess up event handler \t%s:%d\n",
		__FUNCTION__, __LINE__);
	sem_post(g_signal_btoc);
}

void mbox_sync_event4(void)
{
	if (g_tcpmbox.front == ((g_tcpmbox.rear + 1) % g_tcpmbox.queue_size)) {
		printf("[EVT] receive invalid signal in posting\t%s:%d\n",
				 __FUNCTION__, __LINE__);
		printf("[EVT] (queue is still full \t%s:%d\n", __FUNCTION__, __LINE__);
		assert(0);
	}
}

static void *control_thread(void *arg)
{
	/* 1) Task A tried to fetch an data from mbox with timeout
2) If mbox is empty then Task A increase wait_fetch with mutex protection
3) Task A releases mutex and sys_arch_sem_wait(mail, timeout) to get an event.
4) Time expires and task A exit from sys_arch_sem_wait()
5) Context switching occurs, Task B post event with sys_arch_mbox_post()
6) After task B enque data to mbox, it'll check whether send an event or not
7) Task B sends an event because mbox was empty(first_msg == 1) and wait_fetch
is not 0.
8) After context switching task A lock mutex and decrease wait_fetch
and exit the function.
9) No task receive the event task B sent, so unexpected
behavior can be happened. */

	/*	send signal to task a to call sys_arch_mbox_fetch */
	printf("[COND] let task A fetch the event \t%s:%d\n", __FUNCTION__, __LINE__);
	sem_post(g_signal_ctoa);
	/*	wait event that task a wait event with mbox->mail because it's empty */
	printf("[COND] wait timeout signal \t%s:%d\n", __FUNCTION__, __LINE__);
	sem_wait(g_signal_atoc);
	printf("[COND] task A timeout happened \t%s:%d\n", __FUNCTION__, __LINE__);

	/*	let task b to post data until g_tcpmbox is full */
	printf("[COND] let task B to generate invalid signal \t%s:%d\n", __FUNCTION__,
			 __LINE__);
	sem_post(g_signal_ctob);

	printf("[COND] after task B generates signal, wake up task A \t%s:%d\n",
			 __FUNCTION__, __LINE__);
	sem_wait(g_signal_btoc);

	/*	after invalid signal generated then wake up task a */
	sem_wait(g_signal_ctoa);
	return NULL;
}

static void *a_thread(void *arg)
{
	uint32_t recv_cnt = 0;
	printf("[taskA] wait control signal \t%s:%d\n", __FUNCTION__, __LINE__);
	sem_wait(g_signal_ctoa);
	while (recv_cnt < QUEUE_MSG_CNT) {
		lwip_msg_s *msg = NULL;
		printf("[taskA] -->sys_arch_mbox_fetch timeout %d\t%s:%d\n", FETCH_TIMEOUT,
				 __FUNCTION__, __LINE__);
		uint32_t res =
			sys_arch_mbox_fetch(&g_tcpmbox, (void **)&msg, FETCH_TIMEOUT);
		if (res != 0) {
			assert(0);
		}
		free(msg);
		recv_cnt++;
	}
	return NULL;
}

static void *b_thread(void *arg)
{
	printf("[taskB] wait control signal	 \t%s:%d\n", __FUNCTION__, __LINE__);
	sem_wait(g_signal_ctob);
	uint32_t send_cnt = 0;
	while (send_cnt < QUEUE_MSG_CNT) {
		lwip_msg_s *msg = (lwip_msg_s *)malloc(sizeof(lwip_msg_s));
		if (!msg) {
			assert(0);
		}
		msg->type = APP_MSG;
		msg->data = send_cnt + 1000;
		sys_mbox_post(&g_tcpmbox, (void *)msg);
		send_cnt++;
	}
	return NULL;
}

static void _init_resource(void)
{
	ST_ASSERT_EQ(ERR_OK, sys_mbox_new(&g_tcpmbox, TCP_QUEUE_SIZE));
	printf("[TEST] mbox created %p %d\n", &g_tcpmbox, TCP_QUEUE_SIZE);
	g_signal_atoc = (sem_t *)malloc(sizeof(sem_t));
	ST_ASSERT_NEQ(NULL, g_signal_atoc);
	g_signal_ctoa = (sem_t *)malloc(sizeof(sem_t));
	ST_ASSERT_NEQ(NULL, g_signal_ctoa);
	g_signal_btoc = (sem_t *)malloc(sizeof(sem_t));
	ST_ASSERT_NEQ(NULL, g_signal_btoc);
	g_signal_ctob = (sem_t *)malloc(sizeof(sem_t));
	ST_ASSERT_NEQ(NULL, g_signal_ctob);

	ST_ASSERT_EQ(0, sem_init(g_signal_atoc, 0, 0));
	ST_ASSERT_EQ(0, sem_init(g_signal_ctoa, 0, 0));
	ST_ASSERT_EQ(0, sem_init(g_signal_btoc, 0, 0));
	ST_ASSERT_EQ(0, sem_init(g_signal_ctob, 0, 0));
}

static void _deinit_resource(void)
{
	ST_ASSERT_EQ(0, sem_destroy(g_signal_atoc));
	ST_ASSERT_EQ(0, sem_destroy(g_signal_ctoa));
	ST_ASSERT_EQ(0, sem_destroy(g_signal_btoc));
	ST_ASSERT_EQ(0, sem_destroy(g_signal_ctob));

	free(g_signal_atoc);
	free(g_signal_ctoa);
	free(g_signal_btoc);
	free(g_signal_ctob);
}

static void _print_result(void)
{
	printf("[TEST] test done\n");
}

int sync_test(void)
{
	pthread_t contid;
	pthread_t taid;
	pthread_t tbid;

	_init_resource();

	ST_ASSERT_EQ(0, pthread_create(&contid, NULL, control_thread, NULL));
	ST_ASSERT_EQ(0, pthread_create(&taid, NULL, a_thread, NULL));
	ST_ASSERT_EQ(0, pthread_create(&tbid, NULL, b_thread, NULL));

	pthread_join(contid, NULL);
	pthread_join(taid, NULL);
	pthread_join(tbid, NULL);

	_print_result();
	_deinit_resource();
	return 0;
}
