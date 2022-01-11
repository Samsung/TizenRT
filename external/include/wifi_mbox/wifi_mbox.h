/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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
#pragma once

#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <semaphore.h>

#define WT_SEM_TRY_WAIT_US 10000
#define WMBOX_LOG

#define WSEM_POST(sem)                  \
	do {                                \
		while (sem_post(&sem) != 0) {   \
			usleep(WT_SEM_TRY_WAIT_US); \
		}                               \
	} while (0)

#define WSEM_WAIT(sem)                            \
	do {                                          \
		while (sem_wait(&sem) != 0) {             \
			if (errno == EINTR) {                 \
				continue;                         \
			}                                     \
			WMBOX_LOG("sem wait fail %d", errno); \
		}                                         \
	} while (0)

#define WMBOX_SIGNAL(conn, queue)                                 \
	do {                                                          \
		WMBOX_LOG("send signal\t %s:%d", __FUNCTION__, __LINE__); \
		int ssres = wifi_mbox_post(conn, queue);                  \
		if (ssres != 0) {                                         \
			assert(0);                                            \
		}                                                         \
		WSEM_POST(queue->signal);                                 \
	} while (0)

#define WMBOX_WAIT(conn, queue)                                   \
	do {                                                          \
		WMBOX_LOG("wait signal\t %s:%d", __FUNCTION__, __LINE__); \
		WSEM_WAIT(queue->signal);                                 \
		int swres = wifi_mbox_fetch(&conn, queue);                \
		if (swres != 0) {                                         \
			assert(0);                                            \
		}                                                         \
	} while (0)

struct wifi_mbox {
	sem_t lock;
	sem_t signal;
	int queue[10];
	int front;
	int rear;
};

int wifi_mbox_post(int conn, struct wifi_mbox *queue);
int wifi_mbox_fetch(int *conn, struct wifi_mbox *queue);
struct wifi_mbox *wifi_mbox_init(void);
void wifi_mbox_deinit(struct wifi_mbox *queue);
