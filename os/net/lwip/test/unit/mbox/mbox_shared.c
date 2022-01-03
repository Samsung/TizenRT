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

#include <tinyara/config.h>
#include <assert.h>
#include <pthread.h>
#include <queue.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stress_tool/st_perf.h>
#include <unistd.h>

#include "mbox_util.h"
#include "port/port.h"
#include "sys_arch.h"

#define TCP_QUEUE_SIZE 50
#define QUEUE_MSG_CNT 100
static sys_mbox_t g_tcpmbox;
static sem_t *g_signal_atoc = NULL;
static sem_t *g_signal_ctoa = NULL;
static sem_t *g_signal_btoc = NULL;
static sem_t *g_signal_ctob = NULL;

void mbox_shared_event1(void)
{
	printf("[EVT] let task C know mbox is empty \t%s:%d\n", __FUNCTION__,
			 __LINE__);
	sem_post(g_signal_atoc);
}

void mbox_shared_event2(void)
{
	printf(
		"[EVT] task b post signal to task a that mbox is not empty \t%s:%d\n",
		__FUNCTION__, __LINE__);
}

void mbox_shared_event3(uint32_t front, uint32_t rear)
{
	if (front == rear) {
		printf(
			"[EVT] task b exit sem_wait even though mbox is full(%u/%u) because "
			"it "
			"receives signal that task b sent(not emtpy) \t%s:%d\n",
			front, rear, __FUNCTION__, __LINE__);
		assert(0);
	} else {
		printf(
			"[EVT] queue is not full(%u/%u) so it's ok to exit sem_wait "
			"\t%s:%d\n",
			front, rear, __FUNCTION__, __LINE__);
	}
}

static void *control_thread(void *arg)
{
	/*
	1) Task A called mbox_fetch. But there is no data in mbox, task A waits an
	event with mbox->mail
	2) Task B called mbox post. And call
	sys_arch_sem_signal(mbox->mail) to signal Task A
	3) If context switching
	didn't happen then Task B is doing mbox post repeatedly.
	4) mbox can be fulled
	because Task B posts data again and again. Then it calls sys_arch_sem_wait()
	to get a signal that mbox is not full
	5) sys_arch_sem_wait() from Task B return immediately because task A didn't
	receive a signal.
	6) Unexpected behavior will happen.
	*/

	/*	send signal to thread a to call sys_arch_mbox_fetch */
	printf("[COND] run task A \t%s:%d\n", __FUNCTION__, __LINE__);
	sem_post(g_signal_ctoa);
	/*	wait event that task a wait event with mbox->mail because it's empty */
	printf("[COND] wait until task A fetch event \t%s:%d\n", __FUNCTION__, __LINE__);
	sem_wait(g_signal_atoc);
	printf("[COND] task A wait not empty signal \t%s:%d\n", __FUNCTION__, __LINE__);
	/*	let task b to post data until g_tcpmbox is full */
	printf("[COND] run task B to post data \t%s:%d\n", __FUNCTION__, __LINE__);
	sem_post(g_signal_ctob);

	return NULL;
}

static void *a_thread(void *arg)
{
	uint32_t recv_cnt = 0;
	printf("[taskA] wait control signal \t%s:%d\n", __FUNCTION__, __LINE__);
	sem_wait(g_signal_ctoa);
	while (recv_cnt < QUEUE_MSG_CNT) {
		lwip_msg_s *msg = NULL;
		printf("[taskA] -->sys_arch_mbox_fetch \t%s:%d\n", __FUNCTION__,
				 __LINE__);
		uint32_t res = sys_arch_mbox_fetch(&g_tcpmbox, (void **)&msg, 0);
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

static int _init_resource(void)
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

static int _deinit_resource(void)
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

int shared_test(void)
{
	pthread_t contid, taid, tbid;

	(void)_init_resource();

	ST_ASSERT_EQ(0, pthread_create(&contid, NULL, control_thread, NULL));
	ST_ASSERT_EQ(0, pthread_create(&taid, NULL, a_thread, NULL));
	ST_ASSERT_EQ(0, pthread_create(&tbid, NULL, b_thread, NULL));

	pthread_join(contid, NULL);
	pthread_join(taid, NULL);
	pthread_join(tbid, NULL);

	_print_result();
	(void)_deinit_resource();
}
