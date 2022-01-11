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
#include <wifi_mbox/wifi_mbox.h>

/* queue */
#define WMBOX_SIZE 10

#define WMBOX_LOCK(lock) \
	do {                    \
		sem_wait(lock);     \
	} while (0)

#define WMBOX_UNLOCK(lock) \
	do {                      \
		sem_post(lock);       \
	} while (0)

static int _is_empty(struct wifi_mbox *queue)
{
	if (queue->rear == queue->front) {
		return 1;
	}
	return 0;
}

static int _is_full(struct wifi_mbox *queue)
{
	int tmp = (queue->front + 1) % WMBOX_SIZE;
	if (tmp == queue->rear) {
		return 1;
	}
	return 0;
}

/*
 * API
 */
int wifi_mbox_post(int conn, struct wifi_mbox *queue)
{
	WMBOX_LOCK(&queue->lock);
	if (_is_full(queue)) {
		WMBOX_UNLOCK(&queue->lock);
		return -1;
	}
	queue->front = (queue->front + 1) % WMBOX_SIZE;
	queue->queue[queue->front] = conn;
	WMBOX_UNLOCK(&queue->lock);

	return 0;
}

int wifi_mbox_fetch(int *conn, struct wifi_mbox *queue)
{
	WMBOX_LOCK(&queue->lock);
	if (_is_empty(queue)) {
		WMBOX_UNLOCK(&queue->lock);
		return -1;
	}
	queue->rear = (queue->rear + 1) % WMBOX_SIZE;
	*conn = queue->queue[queue->rear];
	WMBOX_UNLOCK(&queue->lock);

	return 0;
}

struct wifi_mbox *wifi_mbox_init(void)
{
	struct wifi_mbox *queue = (struct wifi_mbox *)malloc(sizeof(struct wifi_mbox));
	if (!queue) {
		return NULL;
	}
	int res = sem_init(&queue->lock, 0, 1);
	if (res < 0) {
		printf("[ERR] fail to initialize lock %d", errno);
		free(queue);
		return NULL;
	}
	res = sem_init(&queue->signal, 0, 0);
	if (res < 0) {
		printf("[ERR] fail to initialize signal %d", errno);
		sem_destroy(&queue->lock);
		free(queue);
	}
	queue->front = -1;
	queue->rear = -1;

	return queue;
}

void wifi_mbox_deinit(struct wifi_mbox *queue)
{
	if (!queue) {
		return;
	}
	sem_destroy(&queue->lock);
	sem_destroy(&queue->signal);

	free(queue);
}
