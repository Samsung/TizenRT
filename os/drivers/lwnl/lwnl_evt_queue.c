/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <tinyara/kmalloc.h>
#include <stdlib.h>
#include <errno.h>
#include <debug.h>
#include <net/if.h>
#include <pthread.h>
#include <tinyara/net/if/wifi.h>
#include "lwnl_evt_queue.h"
#include "lwnl_log.h"

#define LWQ_LOCK									\
		do {										\
			sem_wait(&g_wm_sem);					\
		} while (0)

#define LWQ_UNLOCK									\
		do {										\
			sem_post(&g_wm_sem);					\
		} while (0)

struct lwnl_event {
	struct lwnl_event *flink;
	lwnl_cb_data data;
	int8_t refs;
};

struct lwnl_queue {
	struct file *filep; // file index
	// check_header 0: initial state. if there is data to send then assign 1
	// check_header 1: there is data to send.
	int8_t check_header;
	struct lwnl_event *front;
	struct lwnl_event *rear;
};

/* protect g_queue and g_connected*/
static sem_t g_wm_sem;
/* both data should be protected by LWQ_LOCK */
static struct lwnl_queue g_queue[LWNL_NPOLLWAITERS];
static int g_connected = 0;
#ifdef CONFIG_DEBUG_LWNL80211_INFO
static int g_totalevt = 0;
#endif

/*
 * private
 */
static int _lwnl_add_event(struct lwnl_event *event)
{
	int check = 0;
	LWQ_ENTRY;
#ifdef CONFIG_DEBUG_LWNL80211_INFO
	LWQ_LOG("[LWQ] _lwnl_add_event: %p\tg_connect %d %d \t%s \n",
			event, g_connected, g_totalevt, __FUNCTION__);
#endif
	event->flink = NULL;
	LWQ_LOCK;
	for (int i = 0; i < LWNL_NPOLLWAITERS; i++) {
		if (g_queue[i].filep) {
			check = 1;
			LWQ_LOG("[LWQ] add event %p %d %p %p \n",
				   g_queue[i].filep, i, g_queue[i].front, event);
			event->refs = g_connected;
			if (g_queue[i].front == NULL) {
				g_queue[i].front = event;
				g_queue[i].rear = event;
			} else {
				g_queue[i].rear->flink = event;
				g_queue[i].rear = event;
			}
		}
	}
#ifdef CONFIG_DEBUG_LWNL80211_INFO
	if (check == 1) {
		g_totalevt++;
	}
#endif
	LWQ_UNLOCK;
	return 0;
}

/* this function is protected by LWQ_LOCK */
static int _lwnl_remove_event(struct lwnl_event *evt)
{
	LWQ_ENTRY;
	evt->refs--;
	LWQ_LOG("[LWQ] _remove_event refs(%d) (%p) (%p) \n",
			evt->refs, evt, evt->data.data);
	if (evt->refs > 0) {
		return 0;
	}

	LWQ_LOG("[LWQ] _remove_event remove item \n");
	// it's not refered.
	if (evt->data.data) {
		kmm_free(evt->data.data);
		evt->data.data = NULL;
	}
	kmm_free(evt);
#ifdef CONFIG_DEBUG_LWNL80211_INFO
	g_totalevt--;
#endif

	return 0;
}

/**
 * API
 */
void lwnl_queue_initialize(void)
{
	LWQ_ENTRY;
	LWQ_LOCK;
	for (int i = 0; i < LWNL_NPOLLWAITERS; i++) {
		g_queue[i].filep = NULL;
		g_queue[i].front = NULL;
		g_queue[i].rear = NULL;
		g_queue[i].check_header = 0;
	}
	g_connected = 0;

	int res = sem_init(&g_wm_sem, 0, 1);
	if (res < 0) {
		LWQ_ERR;
	}
	LWQ_UNLOCK;
}

int lwnl_add_event(lwnl_cb_status type, void *buffer, uint32_t buf_len)
{
	LWQ_ENTRY;
	struct lwnl_event *evt = (struct lwnl_event *)kmm_malloc(sizeof(struct lwnl_event));
	if (!evt) {
		LWQ_ERR;
		return -1;
	}
	evt->flink = NULL;
	evt->refs = 0;

	evt->data.status = type;
	if (buffer) {
		char *output = kmm_malloc(buf_len);
		if (!output) {
			LWQ_ERR;
			return -3;
		}
		memcpy(output, buffer, buf_len);
		evt->data.data = output;
		evt->data.data_len = buf_len;
	} else {
		evt->data.data = NULL;
		evt->data.data_len = 0;
	}

	int res = _lwnl_add_event(evt);
	if (res < 0) {
		if (evt->data.data) {
			kmm_free(evt->data.data);
		}
		kmm_free(evt);

		LWQ_ERR;
		return -2;
	}
	return 0;
}

int lwnl_get_event(struct file *filep, char *buf, int len)
{
	LWQ_LOCK;
	LWQ_ENTRY;

	struct lwnl_queue *queue = filep->f_priv;
	if (!queue) {
		LWQ_UNLOCK;
		LWQ_ERR;
		return -1;
	}

	if (!queue->front) {
		LWQ_ERR;
		LWQ_UNLOCK;
		return 0;
	}
	struct lwnl_event *evt = queue->front;
	int written = 0;

	if (queue->check_header == 0) {
		if (len < sizeof(lwnl_cb_status) + sizeof(uint32_t)) {
			LWQ_ERR;
			LWQ_UNLOCK;
			return -1;
		}
		memcpy(buf, &evt->data.status, sizeof(lwnl_cb_status));
		buf += sizeof(lwnl_cb_status);
		memcpy(buf, &evt->data.data_len, sizeof(uint32_t));
		written = sizeof(lwnl_cb_status) + sizeof(uint32_t);
		if (evt->data.data_len > 0) {
			queue->check_header = 1;
			LWQ_UNLOCK;
			return written;
		}
	} else {
		if (len < evt->data.data_len) {
			LWQ_ERR;
			LWQ_UNLOCK;
			return -1;
		}
		memcpy(buf, evt->data.data, evt->data.data_len);
		queue->check_header = 0;
		written = evt->data.data_len;
	}

	queue->front = evt->flink;
	_lwnl_remove_event(evt);
	LWQ_UNLOCK;
	return written;
}

int lwnl_add_listener(struct file *filep)
{
	LWQ_LOCK;
	LWQ_ENTRY;
	for (int i = 0; i < LWNL_NPOLLWAITERS; i++) {
		if (!g_queue[i].filep) {
			LWQ_LOG("[LWQ] %s %d\t add filep %d %p \n", __FUNCTION__, __LINE__, i, filep);
			g_queue[i].filep = filep;
			filep->f_priv = (void *)&g_queue[i];
			g_connected++;
			LWQ_UNLOCK;
			return 0;
		}
	}
	LWQ_UNLOCK;
	LWQ_ERR;
	return -1;
}

int lwnl_remove_listener(struct file *filep)
{
	LWQ_LOCK;
	LWQ_ENTRY;
#ifdef CONFIG_DEBUG_LWNL80211_INFO
	LWQ_LOG("[LWQ] T%d remove listener filep %p %d %d \n",
			getpid(), filep, g_totalevt, LWNL_NPOLLWAITERS);
#endif
	struct lwnl_queue *queue = (struct lwnl_queue *)filep->f_priv;
	if (!queue) {
		// some socket doens't bind event listener.
		// so it can cause overhead.
		LWQ_UNLOCK;
		return 0;
	}
	filep->f_priv = queue->filep = NULL;
	struct lwnl_event *evt;
	while (queue->front) {
		LWQ_LOG("[LWQ] call %d remove (%p) \n", i, queue->front);
		evt = queue->front;
		queue->front = evt->flink;
		_lwnl_remove_event(evt);
	}
	queue->front = queue->rear = NULL;
	queue->check_header = 0;
	g_connected--;

	LWQ_UNLOCK;
	return 0;
}

/* Description: if filep has event then it return 1 else return 0 */
int lwnl_check_queue(struct file *filep)
{
	int res = 0;
	LWQ_LOCK;
	LWQ_ENTRY;
	struct lwnl_queue *queue = (struct lwnl_queue *)filep->f_priv;
	if (!queue) {
		LWQ_UNLOCK;
		return 0;
	}
	if (queue->front != NULL) {
		res = 1;
	}
	LWQ_UNLOCK;
	return res;
}
