/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <sys/types.h>
#include "CSIParser.h"
#include "CSIManager.h"
#include "PingGenerator.h"
#include "CSINetworkMonitor.h"
#include "CSIPacketReceiver.h"

#define RUNNING 1
#define STOPPED 0

const static char *CSIFW_TASK_NAME = "csifw_task_Main";
const static int CSIFW_TASK_STACKSIZE = 8192;
const static int CSIFW_TASK_PRIORITY = SCHED_PRIORITY_DEFAULT;

int csifw_task_Main(int argc, char *argv[]);

int start_csi_framework(csifw_context_t *p_ctx)
{
	CSIFW_LOGV;
	if (p_ctx->task_run_state == RUNNING) {
		CSIFW_LOGE("csifw_task already running (task_run_state=%d) - cannot start again", p_ctx->task_run_state);
		CSIFW_LOGX;
		return -1;
	}
	CSIFW_LOGD("Current task state: task_run_state=%d", p_ctx->task_run_state);
	CSIFW_LOGD("Starting CSI Framework with context: %p", p_ctx);
	if (sem_init(&p_ctx->csifw_task_sema, 0, 0) < OK) {
		CSIFW_LOGE("Semaphore init failed errno: %d (%s)", get_errno(), strerror(get_errno()));
		CSIFW_LOGX;
		return -1;
	}
	CSIFW_LOGD("Semaphore initialized successfully");

	p_ctx->task_run_success = 0;

	CSIFW_LOGI("Starting CSIFW Task...");
	int result = task_create(CSIFW_TASK_NAME, CSIFW_TASK_PRIORITY, CSIFW_TASK_STACKSIZE,
			csifw_task_Main, NULL);
	if (result < 0) {
		CSIFW_LOGE("Failed to start %s. errno: %d (%s)", CSIFW_TASK_NAME, errno, strerror(errno));
		if (sem_destroy(&p_ctx->csifw_task_sema) < OK) {
			CSIFW_LOGE("Semaphore destroy failed, errno: %d (%s)", get_errno(), strerror(get_errno()));
		}
		CSIFW_LOGD("Destroyed semaphore for %s after failed start", CSIFW_TASK_NAME);
	} else {
		while (sem_wait(&p_ctx->csifw_task_sema) < 0) {
			if (errno != EINTR) {
				CSIFW_LOGE("Failed to wait for csifw task semaphore, errno: %d (%s)", errno, strerror(errno));
				break;
			} else {
				CSIFW_LOGE("Failed to wait for csifw task semaphore, errno == EINTR, wait continue");
			}
		}

		if (!p_ctx->task_run_success) {
			CSIFW_LOGE("CSI framework task initialization failed");
			if (sem_destroy(&p_ctx->csifw_task_sema) < OK) {
				CSIFW_LOGE("Semaphore destroy failed, errno: %d (%s)", get_errno(), strerror(get_errno()));
			}
			CSIFW_LOGX;
			return -1;
		}
		CSIFW_LOGI("Started %s", CSIFW_TASK_NAME);
		result = 0;
	}
	CSIFW_LOGX;
	return result;
}

int stop_csi_framework(csifw_context_t *p_ctx)
{
	CSIFW_LOGV;
	if (p_ctx->task_run_state == STOPPED) {
		CSIFW_LOGE("csifw_task already stopped (task_run_state=%d)", p_ctx->task_run_state);
		CSIFW_LOGX;
		return -1;
	}

	p_ctx->task_run_state = STOPPED;			//stop the main loop of the task
	CSIFW_LOGD("Waiting for csifw_task to complete...");
	while (sem_wait(&p_ctx->csifw_task_sema) < 0) {
		if (errno != EINTR) {
			CSIFW_LOGE("Failed to wait for csifw task semaphore, errno: %d (%s)", errno, strerror(errno));
			break;
		} else {
			CSIFW_LOGE("Failed to wait for csifw task semaphore, errno == EINTR, wait continue");
		}
	}
	CSIFW_LOGD("Destroying semaphore for %s", CSIFW_TASK_NAME);
	if (sem_destroy(&p_ctx->csifw_task_sema) < OK) {
		CSIFW_LOGE("Semaphore destroy failed, errno: %d (%s)", get_errno(), strerror(get_errno()));
	}
	CSIFW_LOGX;
	return 0;
}

int csifw_task_Main(int argc, char *argv[])
{
	CSIFW_LOGV;
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	p_csifw_ctx->task_run_success = 0;

	if (csi_packet_receiver_initialize() != CSIFW_OK) {
		CSIFW_LOGE("csi_packet_receiver_initialize failed - cannot start CSI framework");
		if (sem_post(&p_csifw_ctx->csifw_task_sema) < OK) {
			CSIFW_LOGE("Semaphore post failed, errno: %d (%s)", get_errno(), strerror(get_errno()));
		}
		CSIFW_LOGX;
		return -1;
	}
	if (csi_packet_receiver_start_collect() != CSIFW_OK) {
		CSIFW_LOGE("CSI packet receiver start failed");
		csi_packet_receiver_cleanup();
		if (sem_post(&p_csifw_ctx->csifw_task_sema) < OK) {
			CSIFW_LOGE("Semaphore post failed, errno: %d (%s)", get_errno(), strerror(get_errno()));
		}
		CSIFW_LOGX;
		return -1;
	}
	if (csi_ping_generator_initialize() != CSIFW_OK) {
		CSIFW_LOGE("CSI ping generator initialize failed");
		csi_packet_receiver_stop_collect();
		csi_packet_receiver_cleanup();
		CSIFW_LOGE("CSI packet receiver stopped as Ping Generator failed");
		if (sem_post(&p_csifw_ctx->csifw_task_sema) < OK) {
			CSIFW_LOGE("Semaphore post failed, errno: %d (%s)", get_errno(), strerror(get_errno()));
		}
		CSIFW_LOGX;
		return -1;
	}
	if (ping_generator_start() != CSIFW_OK) {
		CSIFW_LOGE("CSI ping generator start failed");
		csi_packet_receiver_stop_collect();
		csi_packet_receiver_cleanup();
		csi_ping_generator_cleanup();
		CSIFW_LOGE("CSI packet receiver stopped as Ping Generator start failed");
		if (sem_post(&p_csifw_ctx->csifw_task_sema) < OK) {
			CSIFW_LOGE("Semaphore post failed, errno: %d (%s)", get_errno(), strerror(get_errno()));
		}
		CSIFW_LOGX;
		return -1;
	}
	//to unlock start_csi_framework()
	p_csifw_ctx->task_run_success = 1;
	p_csifw_ctx->task_run_state = RUNNING;
	if (sem_post(&p_csifw_ctx->csifw_task_sema) < OK) {
		CSIFW_LOGE("Semaphore post failed, errno: %d (%s)", get_errno(), strerror(get_errno()));
	}

	while (p_csifw_ctx->task_run_state) {
		usleep(50 * 1000);
	}

	if (ping_generator_stop() != CSIFW_OK) {
		CSIFW_LOGE("CSI ping generator stop failed");
	}
	if (csi_ping_generator_cleanup() != CSIFW_OK) {
		CSIFW_LOGE("CSI ping generator cleanup failed");
	}
	if (csi_packet_receiver_stop_collect() != CSIFW_OK) {
		CSIFW_LOGE("CSI packet receiver stop failed");
	}
	if (csi_packet_receiver_cleanup() == CSIFW_ERROR){
		CSIFW_LOGE("CSI packet receiver cleanup failed");
	}
	//to unlock stop_csi_framework()
	if (sem_post(&p_csifw_ctx->csifw_task_sema) < OK) {
		CSIFW_LOGE("Semaphore post failed, errno: %d (%s)", get_errno(), strerror(get_errno()));
	}
	CSIFW_LOGX;
	return 0;
}

