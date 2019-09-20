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
#include <debug.h>
#include <errno.h>
#include "lwnl_evt_queue.h"

#define LWQ_LOCK
#define LWQ_UNLOCK

#define LWQ_ENTRY										\
	do {												\
		ndbg("[pkbuild] [T%d] %s\t%s:%d\n", getpid(),	\
			 __FUNCTION__, __FILE__, __LINE__);			\
	} while (0)

struct lwnl_event {
	struct lwnl_event *flink;
	lwnl80211_cb_data data;
	int8_t refs;
};

struct lwnl_queue {
	struct file *filep; // file index
	int8_t check_header;
	struct lwnl_event *front;
	struct lwnl_event *rear;
};

// both data should be protected by LWQ_LOCK
static struct lwnl_queue g_queue[LWNL_NPOLLWAITERS];
static int g_connected = 0;

/*
 * private
 */
int _lwnl_add_event(struct lwnl_event *event)
{
	LWQ_ENTRY;
	event->flink = NULL;

	LWQ_LOCK;

	for (int i = 0; i < LWNL_NPOLLWAITERS; i++) {
		if (g_queue[i].filep) {
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
	LWQ_UNLOCK;
	return 0;
}

// this function is protected by LWQ_LOCK;
int _lwnl_remove_event(struct lwnl_event *evt)
{
	LWQ_ENTRY;
	evt->refs--;

	if (evt->refs > 0) {
		return 0;
	}

	// it's not refered.
	if (evt->data.u.data) {
		free(evt->data.u.data);
		evt->data.u.data = NULL;
	}
	free(evt);

	return 0;
}



int lwnl_get_event(struct file *filep, char *buf, int len)
{
	LWQ_ENTRY;
	LWQ_LOCK;
	for (int i = 0; i < LWNL_NPOLLWAITERS; i++) {
		if (g_queue[i].filep != filep) {
			continue;
		}

		if (!g_queue[i].front) {
			return 0;
		}
		struct lwnl_event *evt = g_queue[i].front;
		int written = 0;

		if (g_queue[i].check_header == 0) {
			if (len < sizeof(lwnl80211_cb_status) + sizeof(uint32_t)) {
				return -1;
			}
			memcpy(buf, &evt->data.status, sizeof(lwnl80211_cb_status));
			buf += sizeof(lwnl80211_cb_status);
			memcpy(buf, &evt->data.data_len, sizeof(uint32_t));
			written = sizeof(lwnl80211_cb_status) + sizeof(uint32_t);
			if (evt->data.data_len > 0) {
				g_queue[i].check_header = 1;
				LWQ_UNLOCK;
				return written;
			}
		} else {
			if (len < evt->data.data_len) {
				return -1;
			}
			memcpy(buf, evt->data.u.data, evt->data.data_len);
			g_queue[i].check_header = 0;
			written = evt->data.data_len;
		}

		g_queue[i].front = evt->flink;
		_lwnl_remove_event(evt);
		LWQ_UNLOCK;
		return written;
	}
	LWQ_UNLOCK;
	return -1;
}


int _lwnl_copy_scan_info(char **buffer, lwnl80211_scan_list_s *scan_list)
{
	lwnl80211_scan_list_s *item = scan_list;
	int cnt = 0, total = 0;
	while (item) {
		item = item->next;
		cnt++;
	}
	total = cnt;
	ndbg("total size(%d) (%d)\n", sizeof(lwnl80211_ap_scan_info_s), sizeof(lwnl80211_ap_scan_info_s) * total);
	*buffer = (char *)malloc(sizeof(lwnl80211_ap_scan_info_s) * total);
	if (!(*buffer)) {
		return -1;
	}
	item = scan_list;
	cnt = 0;
	while (item) {
		memcpy(*buffer + (sizeof(lwnl80211_ap_scan_info_s) * cnt), &item->ap_info, sizeof(lwnl80211_ap_scan_info_s));
		item = item->next;
		cnt++;
	}
	return total * sizeof(lwnl80211_ap_scan_info_s);
}


int lwnl_add_event(lwnl80211_cb_status type, void *buffer)
{
	LWQ_ENTRY;
	struct lwnl_event *evt = (struct lwnl_event *)malloc(sizeof(struct lwnl_event));
	if (!evt) {
		return -1;
	}
	evt->flink = NULL;
	evt->refs = 0;

	switch (type) {
	case LWNL80211_STA_CONNECTED:
	case LWNL80211_STA_CONNECT_FAILED:
	case LWNL80211_STA_DISCONNECTED:
	case LWNL80211_SOFTAP_STA_JOINED:
	case LWNL80211_SOFTAP_STA_LEFT:
	case LWNL80211_SCAN_FAILED:
	{
		evt->data.status = type;
		evt->data.u.data = NULL;
		evt->data.data_len = 0;
		break;
	}
	case LWNL80211_SCAN_DONE:
	{
		// ToDo
		char *output = NULL;
		int res = _lwnl_copy_scan_info(&output, (lwnl80211_scan_list_s *)buffer);
		if (res < 0) {
			evt->data.status = LWNL80211_SCAN_FAILED;
			evt->data.u.data = NULL;
			evt->data.data_len = 0;
			break;
		}
		evt->data.status = LWNL80211_SCAN_DONE;
		evt->data.u.data = output;
		evt->data.data_len = res;
		break;
	}
	case LWNL80211_UNKNOWN:
	default:
		LWNL80211_ERR;
		return -3;
	}

	int res = _lwnl_add_event(evt);
	if (res < 0) {
		free(evt);
		return -2;
	}
	return 0;
}



void lwnl_queue_initialize(void)
{
	LWQ_ENTRY;
	for (int i = 0; i < LWNL_NPOLLWAITERS; i++) {
		g_queue[i].filep = NULL;
		g_queue[i].front = NULL;
		g_queue[i].rear = NULL;
		g_queue[i].check_header = 0;
	}
	g_connected = 0;
}


int lwnl_add_listener(struct file *filep)
{
	LWQ_ENTRY;
	LWQ_LOCK;
	for (int i = 0;i < LWNL_NPOLLWAITERS; i++) {
		if (!g_queue[i].filep) {
			g_queue[i].filep = filep;
			g_connected++;
			LWQ_UNLOCK;
			return 0;
		}
	}
	LWQ_UNLOCK;
	return -1;
}


int lwnl_remove_listener(struct file *filep)
{
	LWQ_ENTRY;
	LWQ_LOCK;
	for (int i = 0; i < LWNL_NPOLLWAITERS; i++) {
		if (g_queue[i].filep == filep) {
			g_queue[i].filep = NULL;
			struct lwnl_event *evt;
			while (g_queue[i].front) {
				evt = g_queue[i].front;
				g_queue[i].front = evt->flink;
				_lwnl_remove_event(evt);
			}
			g_queue[i].front = g_queue[i].rear = NULL;
			g_queue[i].check_header = 0;
			g_connected--;

			LWQ_UNLOCK;
			return 0;
		}
	}
	LWQ_UNLOCK;
	return -1;
}
