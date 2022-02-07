/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <errno.h>
#include <wifi_manager/wifi_manager.h>
#include "wm_test_utils.h"
#include "wm_test_log.h"

#define TAG "[WU]"
/* queue */
#define WO_QUEUE_SIZE 10
#define WO_QUEUE_LOCK(lock)						\
	do {										\
		sem_wait(lock);							\
	} while (0)
#define WO_QUEUE_UNLOCK(lock)					\
	do {										\
		sem_post(lock);							\
	} while (0)

int wo_is_empty(struct wo_queue *queue)
{
	if (queue->rear == queue->front) {
		return 1;
	}
	return 0;
}

int wo_is_full(struct wo_queue *queue)
{
	int tmp = (queue->front + 1) % WO_QUEUE_SIZE;
	if (tmp == queue->rear) {
		return 1;
	}
	return 0;
}

int wo_add_queue(int conn, struct wo_queue *queue)
{
	WO_QUEUE_LOCK(&queue->lock);
	if (wo_is_full(queue)) {
		WO_QUEUE_UNLOCK(&queue->lock);
		return -1;
	}
	queue->front = (queue->front + 1) % WO_QUEUE_SIZE;
	queue->queue[queue->front] = conn;
	WO_QUEUE_UNLOCK(&queue->lock);

	return 0;
}

int wo_dequeue(int *conn, struct wo_queue *queue)
{
	WO_QUEUE_LOCK(&queue->lock);
	if (wo_is_empty(queue)) {
		WO_QUEUE_UNLOCK(&queue->lock);
		return -1;
	}
	queue->rear = (queue->rear + 1) % WO_QUEUE_SIZE;
	*conn = queue->queue[queue->rear];
	WO_QUEUE_UNLOCK(&queue->lock);

	return 0;
}

struct wo_queue *wo_create_queue(void)
{
	struct wo_queue *queue = (struct wo_queue *)malloc(sizeof(struct wo_queue));
	if (!queue) {
		return NULL;
	}
	int res = sem_init(&queue->lock, 0, 1);
	if (res < 0) {
		WT_LOGE(TAG, "fail to initialize lock %d", errno);
		free(queue);
		return NULL;
	}
	res = sem_init(&queue->signal, 0, 0);
	if (res < 0) {
		WT_LOGE(TAG, "fail to intiailize signal", errno);
		sem_destroy(&queue->lock);
		free(queue);
	}
	queue->front = -1;
	queue->rear = -1;

	return queue;
}

void wo_destroy_queue(struct wo_queue *queue)
{
	if (!queue) {
		return;
	}
	sem_destroy(&queue->lock);
	sem_destroy(&queue->signal);

	free(queue);
}
