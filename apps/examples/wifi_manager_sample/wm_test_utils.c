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

void wt_print_conninfo(wifi_manager_info_s *info)
{
	WT_LOGP(TAG, "==============================================\n");
	if (info->mode == SOFTAP_MODE) {
		WT_LOGP(TAG, "MODE: softap\n");
		WT_LOGP(TAG, "SSID: %s\n", info->ssid);
	} else if (info->mode == STA_MODE) {
		if (info->status == AP_CONNECTED) {
			WT_LOGP(TAG, "MODE: station (connected)\n");
			WT_LOGP(TAG, "SSID: %s\n", info->ssid);
			WT_LOGP(TAG, "rssi: %d\n", info->rssi);
		} else if (info->status == AP_DISCONNECTED) {
			WT_LOGP(TAG, "MODE: station (disconnected)\n");
		}
	} else {
		WT_LOGP(TAG, "STATE: NONE\n");
	}
	WT_LOGP(TAG, "==============================================\n");
}

void wt_print_stats(wifi_manager_stats_s *stats)
{
	WT_LOGP(TAG, "=======================================================================\n");
	WT_LOGP(TAG, "CONN    CONNFAIL    DISCONN    RECONN    SCAN    SOFTAP    JOIN    LEFT\n");
	WT_LOGP(TAG, "%-8d%-12d%-11d%-10d\n", stats->connect, stats->connectfail, stats->disconnect, stats->reconnect);
	WT_LOGP(TAG, "%-8d%-10d%-8d%-8d\n", stats->scan, stats->softap, stats->joined, stats->left);
	WT_LOGP(TAG, "Period %d:%d ~ %d:%d\n", stats->start.tv_sec, stats->start.tv_usec,
			stats->end.tv_sec, stats->end.tv_usec);
	WT_LOGP(TAG, "retransmit %d\n", stats->tx_retransmit);
	WT_LOGP(TAG, "TX drop %d\n", stats->tx_drop);
	WT_LOGP(TAG, "RX drop %d\n", stats->rx_drop);
	WT_LOGP(TAG, "TX success(count) %d\n", stats->tx_success_cnt);
	WT_LOGP(TAG, "TX success(bytes) %d\n", stats->tx_success_bytes);
	WT_LOGP(TAG, "RX counts %d\n", stats->rx_cnt);
	WT_LOGP(TAG, "RX bytes %d\n", stats->rx_bytes);
	WT_LOGP(TAG, "TX tries %d\n", stats->tx_try);
	WT_LOGP(TAG, "RSSI avg %d\n", stats->rssi_avg);
	WT_LOGP(TAG, "RSSI min %d\n", stats->rssi_min);
	WT_LOGP(TAG, "RSSI max %d\n", stats->rssi_max);
	WT_LOGP(TAG, "BEACON missed(counts) %d\n", stats->beacon_miss_cnt);
	WT_LOGP(TAG, "=======================================================================\n");
}
