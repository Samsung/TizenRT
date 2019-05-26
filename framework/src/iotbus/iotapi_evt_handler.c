/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include "iotapi_evt_handler.h"
#include "iotbus_internal.h"

#define IOTAPI_QUEUE_SIZE 19

#define IOTAPI_MESSAGE_QUEUE_EMPTY	0
#define IOTAPI_MESSAGE_QUEUE_INSERT	1
#define IOTAPI_MESSAGE_QUEUE_REMOVE	2

struct _iotapi_msg_queue {
	int type;					// 1 is insert, 2 is remove, 0 is empty
	iotapi_elem evt;
};

struct _iotapi_msg_queue g_ia_msg_queue[2];
pthread_mutex_t g_ia_lock;
iotapi_elem g_ia_evt_queue[IOTAPI_QUEUE_SIZE];
static int g_ia_evt_size = 0;
struct pollfd g_ia_evtlist[IOTAPI_QUEUE_SIZE];

int g_pipes_handler[2];
int g_max_fd;

/**
 * Private API
 */
int _iotapi_alloc_event(void)
{
	memset(g_ia_evtlist, 0, sizeof(g_ia_evtlist));
	g_ia_evtlist[0].fd = g_pipes_handler[0];
	g_ia_evtlist[0].events = POLLIN | POLLERR;
	g_ia_evt_queue[0].idx = 0;

	int i = 0;
	int idx = 1;
	for (; i < IOTAPI_QUEUE_SIZE; i++) {
		if (g_ia_evt_queue[i].fd != -1) {
			g_ia_evt_queue[i].idx = idx;
			g_ia_evtlist[idx].fd = g_ia_evt_queue[i].fd;
			g_ia_evtlist[idx++].events = POLLIN | POLLERR;
		}
	}
	return idx;
}

void *iotapi_handler(void *data)
{
	char buf[3];

	pthread_mutex_lock(&g_ia_lock);
	if (g_ia_msg_queue[0].type == IOTAPI_MESSAGE_QUEUE_INSERT) {
		g_ia_evt_queue[0].fd = g_ia_msg_queue[0].evt.fd;
		g_ia_evt_queue[0].data = g_ia_msg_queue[0].evt.data;
		g_ia_evt_queue[0].func = g_ia_msg_queue[0].evt.func;
		g_ia_evt_size = 1;
		g_ia_msg_queue[0].type = IOTAPI_MESSAGE_QUEUE_EMPTY;
	}
	pthread_mutex_unlock(&g_ia_lock);

	for (;;) {
		int timeout = 1000;
		int size = _iotapi_alloc_event();
		idbg("[iotcom] Wait sysio events(%d)\n", size);
		int ret = poll(g_ia_evtlist, size, timeout);
		if (ret < 0) {
			idbg("[iotcom] poll error(%d)(%d)\n", ret, errno);
			break;
		} else if (ret == 0) {
			idbg("[iotcom] timeout(%d)\n", timeout);
			continue;
		}

		idbg("[iotcom] poll event(%d)\n", ret);
		int idx = 0;
		for (idx = 0; idx < IOTAPI_QUEUE_SIZE; idx++) {
			//idbg("[iotcom] search fd(%d) idx(%d)\n", g_ia_evt_queue[idx].fd, g_ia_evt_queue[idx].idx);
			if (g_ia_evt_queue[idx].fd != -1 && g_ia_evtlist[g_ia_evt_queue[idx].idx].revents & (POLLIN)) {
				idbg("[iotcom] search fd(%d) idx(%d)\n", g_ia_evt_queue[idx].fd, g_ia_evt_queue[idx].idx);
				g_ia_evt_queue[idx].func(g_ia_evt_queue[idx].data);
			}
		}

		if (g_ia_evtlist[0].revents & POLLIN) {
			
#ifdef CONFIG_IOTAPI_DEBUG
			int readed;
			readed = read(g_ia_evtlist[0].fd, buf, 3);
			idbg("[iotcom] receive command(%d)\n", readed);
#else
			read(g_ia_evtlist[0].fd, buf, 3);
#endif
			
			if (buf[0] == 's' && buf[1] == 't')
				break;

			pthread_mutex_lock(&g_ia_lock);
			if (g_ia_msg_queue[0].type == IOTAPI_MESSAGE_QUEUE_INSERT) {
				g_ia_evt_size++;
				int i = 0;
				for (; i < IOTAPI_QUEUE_SIZE; i++) {
					if (g_ia_evt_queue[i].fd == -1) {
						g_ia_evt_queue[i].fd = g_ia_msg_queue[0].evt.fd;
						g_ia_evt_queue[i].data = g_ia_msg_queue[0].evt.data;
						g_ia_evt_queue[i].func = g_ia_msg_queue[0].evt.func;
						idbg("[iotcom] insert 2-1-1(%d)\n", g_ia_evt_size);
						break;
					}
				}
				g_ia_msg_queue[0].type = IOTAPI_MESSAGE_QUEUE_EMPTY;
			}
			if (g_ia_msg_queue[1].type == IOTAPI_MESSAGE_QUEUE_REMOVE) {
				g_ia_evt_size--;
				int i = 0;
				for (; i < IOTAPI_QUEUE_SIZE; i++) {
					if (g_ia_evt_queue[i].fd == g_ia_msg_queue[0].evt.fd) {
						g_ia_evt_queue[i].fd = -1;
						idbg("[iotcom] remove 2-2-1(%d)\n", g_ia_evt_size);
						break;
					}
				}
				g_ia_msg_queue[1].type = IOTAPI_MESSAGE_QUEUE_EMPTY;
			}

			if (g_ia_evt_size == 0) {
				pthread_mutex_unlock(&g_ia_lock);
				break;
			}
			pthread_mutex_unlock(&g_ia_lock);
		}

	}
	idbg("[iotcom] exit iotapi handler\n");

	return NULL;
}

int iotapi_queue_command(char *buf)
{
	int ret = write(g_pipes_handler[1], buf, 3);
	if (ret < 0) {
		idbg("[iotcom] pipe write fail\n");
		return -1;
	}
	return 0;
}

int iotapi_handler_restart(void)
{
	char command[3] = "rs";
	int ret = iotapi_queue_command(command);
	if (ret < 0) {
		idbg("[iotcom] iotapi handler restart fail\n");
		return -1;
	}
	return 0;
}

int iotapi_handler_stop(void)
{
	idbg("[iotcom] iotapi handler stop\n");
	char command[3] = "st";
	int ret = iotapi_queue_command(command);
	if (ret < 0) {
		idbg("[iotcom] stop iotapi handler fail\n");
		return -1;
	}
	return 0;
}

int iotapi_handler_start(int fd)
{
	idbg("[iotcom] iotapi handler start\n");
	pthread_t tid;
	int ret;
	ret = pthread_create(&tid, NULL, iotapi_handler, NULL);
	if (ret < 0) {
		idbg("[iotcom] create iotapi handler fail(%d)\n", ret);
		return -1;
	}
	pthread_detach(tid);
	return 0;
}

/***
 * Public API
 */
void iotapi_initialize(void)
{
	idbg("[iotcom] init\n");
	int ret = 0, i = 0;
	// booting time

	// init mutex
	ret = pthread_mutex_init(&g_ia_lock, NULL);
	if (ret < 0) {
		idbg("[iotcom] IOTAPI mutex init error\n");
		return;
	}
	// init pipe
	if (pipe(g_pipes_handler) == -1) {
		idbg("[iotcom] Create handler pipe fail\n");
		return;
	}
	// init queue
	g_ia_evt_size = 0;
	for (i = 0; i < IOTAPI_QUEUE_SIZE; i++)
		g_ia_evt_queue[i].fd = -1;

	return;
}

int iotapi_insert(iotapi_elem * item)
{
	idbg("[iotcom] ==>iotapi_insert\n");
	int mode = 1, ret = 0;
	pthread_mutex_lock(&g_ia_lock);
	if (g_ia_evt_size == IOTAPI_QUEUE_SIZE) {
		pthread_mutex_unlock(&g_ia_lock);
		ret = -1;
	}
	if (g_ia_evt_size == 0)
		mode = 0;
	g_ia_msg_queue[0].type = IOTAPI_MESSAGE_QUEUE_INSERT;
	g_ia_msg_queue[0].evt.fd = item->fd;
	g_ia_msg_queue[0].evt.data = item->data;
	g_ia_msg_queue[0].evt.func = item->func;

	pthread_mutex_unlock(&g_ia_lock);

	if (mode == 0) {
		ret = iotapi_handler_start(item->fd);
		if (ret < 0)
			idbg("[iotcom] start iotapi_handler fail\n");
	} else {
		ret = iotapi_handler_restart();
		if (ret < 0)
			idbg("[iotcom] Restart iotapi_handler fail\n");
	}

	return ret;
}

int iotapi_remove(iotapi_elem * item)
{
	idbg("[iotcom] ==>iotapi_remove\n");
	int ret = 0;
	pthread_mutex_lock(&g_ia_lock);
	if (g_ia_evt_size == 0) {
		pthread_mutex_unlock(&g_ia_lock);
		ret = -1;
	}
	g_ia_msg_queue[1].type = IOTAPI_MESSAGE_QUEUE_REMOVE;
	g_ia_msg_queue[1].evt.fd = item->fd;

	pthread_mutex_unlock(&g_ia_lock);

	ret = iotapi_handler_restart();
	if (ret < 0)
		idbg("[iotcom] Restart iotapi_handler fail\n");

	return ret;
}
