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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <debug.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/select.h>

#include "iotapi_dev_handler.h"
#include "iotbus_internal.h"

#ifdef CONFIG_IOTDEV

#define IOTAPI_CBK_QUEUE_SIZE 10

#define IOT_QUEUE_LOCK											\
	do {														\
		pthread_mutex_lock(&g_iot_cbk_mgr.queue_lock);	\
	} while (0)

#define IOT_QUEUE_UNLOCK											\
	do {															\
		pthread_mutex_unlock(&g_iot_cbk_mgr.queue_lock);	\
	} while (0)

#define IOTAPI_ERROR													\
	do {																\
		idbg("[ERR] IOTAPI %s(%d)(%s:%d)\n", __FUNCTION__, errno, __FILE__, __LINE__); \
	} while (0)

struct _iotapi_cbk_entry {
	struct _iotapi_cbk_entry *flink;
	struct _iotapi_dev_ctx_s *ctx;
};

struct _iotapi_cbk_queue {
	struct _iotapi_cbk_entry *head;
	struct _iotapi_cbk_entry *tail;

	pthread_mutex_t queue_lock;

	int refcnt; // debugging
};

struct _iotapi_dev_ctx_s {
	int fd;
	iotbus_int_type_e type;
	iotapi_cbk cb;
	struct _iotapi_cbk_entry entry;
};

static int g_pfd[2] = {0,};

static struct _iotapi_cbk_queue g_iot_cbk_mgr = {
	NULL, NULL, PTHREAD_MUTEX_INITIALIZER, 0
};

/*
 * Private
 */
static void iotapi_insert_cbk(struct _iotapi_dev_ctx_s *ctx)
{
	struct _iotapi_cbk_entry *node = &(ctx->entry);

	node->flink = NULL;
	struct _iotapi_cbk_queue *queue = &g_iot_cbk_mgr;

	IOT_QUEUE_LOCK;
	node->flink = NULL;
	if (!queue->head) {
		queue->head = node;
		queue->tail = node;
	} else {
		queue->tail->flink = node;
		queue->tail = node;
	}
	queue->refcnt++;
	IOT_QUEUE_UNLOCK;
}

static void iotapi_remove_cbk(struct _iotapi_dev_ctx_s *ctx)
{
	struct _iotapi_cbk_queue *queue = &g_iot_cbk_mgr;

	IOT_QUEUE_LOCK;

	struct _iotapi_cbk_entry *node = queue->head;
	while (node) {
		if (node->ctx == ctx) {
			queue->head = node->flink;
			if (node == queue->tail) {
				queue->tail = NULL;
			}
			queue->refcnt--;
			break;
		}
		node = node->flink;
	}

	IOT_QUEUE_UNLOCK;
}

static void iotdev_callback(iotbus_int_type_e evt)
{
	struct _iotapi_cbk_queue *queue = &g_iot_cbk_mgr;
	IOT_QUEUE_LOCK;
	struct _iotapi_cbk_entry *node = queue->head;
	while (node) {
		if (node->ctx->type == evt) {
			node->ctx->cb(evt);
		}
		node = node->flink;
	}
	IOT_QUEUE_UNLOCK;
}

void *iotdev_handler(void *data)
{
	fd_set rfds;
	fd_set fds;

	int fd = open(IOTDEV_DRVPATH, O_RDWR);
	if (fd == -1) {
		idbg("error\n");
		return NULL;
	}
	int sig = *((int *)data);
	int max_fd = fd > sig ? (fd + 1) : (sig + 1);

	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	FD_SET(sig, &fds);

	while (1) {
		rfds = fds;
		int res = select(max_fd, &rfds, NULL, NULL, NULL);
		if (res < 0) {
			idbg("select error\n");
			return NULL;
		}
		if (FD_ISSET(sig, &rfds)) {
			idbg("terminate thread\n");
			break;
		}
		if (FD_ISSET(fd, &rfds)) {
			idbg("get signal from iotdev\n");
			char buf[1];
			int readed = read(fd, buf, 1);
			if (readed <= 0) {
				idbg("read error\n");
				continue;
			}

			// call callbacks
			iotdev_callback((iotbus_int_type_e)buf[0]);
		}
	}
	close(sig);
	close(fd);

	return NULL;
}

/*
 * Public
 */
int iotapi_dev_init(iotapi_hnd *hnd)
{
	int res = pipe(g_pfd);
	if (res == -1) {
		IOTAPI_ERROR;
		return -1;
	}
	struct _iotapi_dev_ctx_s *ctx = (struct _iotapi_dev_ctx_s *)malloc(sizeof(struct _iotapi_dev_ctx_s));
	if (!ctx) {
		close(g_pfd[0]);
		close(g_pfd[1]);

		return -1;
	}

	ctx->cb = NULL;
	ctx->entry.ctx = ctx;
	ctx->entry.flink = NULL;

	pthread_t tid;
	int ret;
	ret = pthread_create(&tid, NULL, iotdev_handler, (void *)&g_pfd[0]);
	if (ret < 0) {
		idbg("[iotcom] create iotapi handler fail(%d)\n", ret);
		close(g_pfd[0]);
		close(g_pfd[1]);
		free(ctx);
		return -1;
	}

	*hnd = ctx;

	return 0;
}

int iotapi_dev_deinit(iotapi_hnd hnd)
{
	char buf[5] = "term";
	int res = write(g_pfd[1], buf, 5);
	if (res <= 0) {
		idbg("send term signal error(%d)(%d)\n", res, errno);
		return -1;
	}
	// TBD : wait here until thread is done?
	return 0;
}

int iotapi_dev_register(iotapi_hnd hnd, iotbus_int_type_e evt, iotapi_cbk cbk)
{
	struct _iotapi_dev_ctx_s *ctx = (struct _iotapi_dev_ctx_s *)hnd;
	if (!ctx) {
		IOTAPI_ERROR;
		return -1;
	}

	ctx->cb = cbk;
	ctx->type = evt;
	iotapi_insert_cbk(ctx);

	return 0;
}

int iotapi_dev_unregister(iotapi_hnd hnd)
{
	struct _iotapi_dev_ctx_s *ctx = (struct _iotapi_dev_ctx_s *)hnd;
	if (!ctx) {
		IOTAPI_ERROR;
		return -1;
	}

	iotapi_remove_cbk(ctx);

	return 0;
}

iotbus_int_type_e iotapi_dev_get_int_type(iotapi_hnd hnd)
{
	struct _iotapi_dev_ctx_s *ctx = (struct _iotapi_dev_ctx_s *)hnd;
	if (!ctx) {
		IOTAPI_ERROR;
		return -1;
	}

	return ctx->type;
}
#else
int iotapi_dev_init(iotapi_hnd *hnd)
{
	idbg("Turn on IOTDEV driver");
	return -1;
}

int iotapi_dev_deinit(iotapi_hnd hnd)
{
	idbg("Turn on IOTDEV driver");
	return -1;
}

int iotapi_dev_register(iotapi_hnd hnd, iotbus_int_type_e evt, iotapi_cbk cbk)
{
	idbg("Turn on IOTDEV driver");
	return -1;
}

int iotapi_dev_unregister(iotapi_hnd hnd)
{
	idbg("Turn on IOTDEV driver");
	return -1;
}

iotbus_int_type_e iotapi_dev_get_int_type(iotapi_hnd hnd)
{
	idbg("Turn on IOTDEV driver");
	return -1;
}
#endif
