/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#ifdef __TIZENRT__
#include <tinyara/config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/select.h>
#ifdef LINUX
#include <unistd.h>
#endif
#include "tiny_evtmgr.h"
#include "tiny_evtqueue.h"

/*
 * Macro
 */
#ifdef TINYEVT_ENABLE_LOG
#define TINYEVT_LOG printf
#else
#define TINYEVT_LOG(...)
#endif
#ifdef LINUX
#define TINYEVT_ERR TINYEVT_LOG("T%lx [ERR] %s\t%s:%d\n", pthread_self(), __FUNCTION__, __FILE__, __LINE__)
#define TINYEVT_ENTRY TINYEVT_LOG("T%lx --> %s\t%s:%d\n", pthread_self(), __FUNCTION__, __FILE__, __LINE__)
#define TINYEVT_OUT TINYEVT_LOG("T%lx <-- %s\t%s:%d\n", pthread_self(), __FUNCTION__, __FILE__, __LINE__)
#else
#define TINYEVT_ERR TINYEVT_LOG("T%x [ERR] %s\t%s:%d\n", getpid(), __FUNCTION__, __FILE__, __LINE__)
#define TINYEVT_ENTRY TINYEVT_LOG("T%x --> %s\t%s:%d\n", getpid(), __FUNCTION__, __FILE__, __LINE__)
#define TINYEVT_OUT TINYEVT_LOG("T%x <-- %s\t%s:%d\n", getpid(), __FUNCTION__, __FILE__, __LINE__)
#endif
#define TINYEVT_RETURN(res)						\
	do {										\
		if (res != TINY_EVTMGR_SUCCESS) {		\
			TINYEVT_ERR;						\
		} else {								\
			TINYEVT_OUT;						\
		}										\
		return res;								\
	} while (0)

#define TINYEVT_ASSERT							\
	do {										\
		TINYEVT_ERR;							\
		sleep(1000000);							\
	} while (0)

struct tem_context {
	pthread_t thread_id;
	tem_evt_loop func;
	tem_msg_queue queue;
	tem_free_msg rfunc;
};

struct tem_node_msg {
	tem_msg *msg;
	struct tem_context *hnd;
};

/**
 * Private Functions
 */

static void *_delete_msg(teq_node *node)
{
	TINYEVT_ENTRY;
	// delete remained nodes
	if (!node) {
		TINYEVT_ERR;
		return 0;
	}

	struct tem_node_msg *msg = (struct tem_node_msg *)node->data;
	if (!msg) {
		TINYEVT_ASSERT;
		return 0;
	}

	struct tem_context *ctx = msg->hnd;
	if (!ctx) {
		TINYEVT_ERR;
		return 0;
	}
	ctx->rfunc(msg->msg);
	free(msg);

	TINYEVT_RETURN(0);
}

static void *_run_process(void *arg)
{
	TINYEVT_ENTRY;

	struct tem_context *ctx = (struct tem_context *)arg;

	while (1) {
		teq_node *node;
		tq_result res = tq_pop_queue(ctx->queue, &node, 0);
		if (res != TQ_SUCCESS) {
			TINYEVT_LOG("receive stop message\n");
			break;
		}
		if (!node->data) {
			TINYEVT_ASSERT;
		}
		struct tem_node_msg *nmsg = (struct tem_node_msg *)node->data;
		if (!nmsg->msg || !nmsg->hnd) {
			TINYEVT_ASSERT;
		}
		ctx->func(nmsg->msg);
		free(nmsg);
		free(node);
	}
	TINYEVT_OUT;

	return 0;
}

/**
 * Public APIs
 */
tem_result tiny_evtmgr_init(tem_hnd *hnd)
{
	TINYEVT_ENTRY;
	if (!hnd) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}
	struct tem_context *ctx;
	ctx = (struct tem_context *)malloc(sizeof(struct tem_context));
	if (ctx == 0) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}
	*hnd = ctx;
	TINYEVT_RETURN(TINY_EVTMGR_SUCCESS);
}

tem_result tiny_evtmgr_deinit(tem_hnd hnd)
{
	TINYEVT_ENTRY;
	if (!hnd) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}

	struct tem_context *ctx = (struct tem_context *)hnd;
	free(ctx);
	TINYEVT_RETURN(TINY_EVTMGR_SUCCESS);
}

/*
 * Event loop
 */
tem_result tiny_evtmgr_start(tem_hnd hnd, tem_evt_loop evtl, tem_free_msg rfunc, tem_evt_attr *ettr)
{
	TINYEVT_ENTRY;
	if (!hnd) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}

	struct tem_context *ctx = (struct tem_context *)hnd;

	int res = tq_init_queue(&ctx->queue, _delete_msg);
	if (res < 0) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}
	ctx->func = evtl;
	ctx->rfunc = rfunc;

	pthread_attr_t attr;
	pthread_attr_t *in_attr = 0;
	if (ettr) {
		res = pthread_attr_init(&attr);
		if (res != 0) {
			TINYEVT_RETURN(TINY_EVTMGR_FAIL);
		}
		res = pthread_attr_setstacksize(&attr, ettr->stack_size);
		if (res != 0) {
			TINYEVT_RETURN(TINY_EVTMGR_FAIL);
		}
		in_attr = &attr;
	}
	res = pthread_create(&ctx->thread_id, in_attr, _run_process, (void *)ctx);
	if (res != 0) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}

	TINYEVT_RETURN(TINY_EVTMGR_SUCCESS);
}

tem_result tiny_evtmgr_stop(tem_hnd hnd)
{
	TINYEVT_ENTRY;
	if (!hnd) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}
	struct tem_context *ctx = (struct tem_context *)hnd;

	/*  Terminating queue make thread stop */
	int res = tq_deinit_queue(ctx->queue);
	if (res != 0) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}

	res = pthread_join(ctx->thread_id, 0);
	if (res < 0) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}
	ctx->thread_id = -1;
	ctx->func = 0;
	ctx->rfunc = 0;

	TINYEVT_RETURN(TINY_EVTMGR_SUCCESS);
}

/*
 * Message
 */
tem_result tiny_evtmgr_add_msg(tem_hnd hnd, tem_msg *msg, uint32_t delay)
{
	TINYEVT_ENTRY;
	if (!hnd) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}
	struct tem_context *ctx = hnd;

	struct tem_node_msg *nmsg = (struct tem_node_msg *)malloc(sizeof(struct tem_node_msg));
	if (!nmsg) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}
	nmsg->msg = msg;
	nmsg->hnd = hnd;

	teq_node *node = tq_create_node((void *)nmsg);
	if (!node) {
		free(nmsg);
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}
	node->data = nmsg;

	int ret = tq_push_queue(ctx->queue, node, delay);
	if (ret < 0) {
		TINYEVT_RETURN(TINY_EVTMGR_FAIL);
	}
	TINYEVT_RETURN(TINY_EVTMGR_SUCCESS);
}
