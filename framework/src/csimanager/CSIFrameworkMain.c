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

#include "debug.h"
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include "CSIParser.h"
#include "CSIManager.h"
#include "PingGenerator.h"
#include "CSINetworkMonitor.h"
#include "CSIPacketReceiver.h"
#include <task_manager/task_manager.h>

#define RUNNING 1
#define STOPPED 0

const static char **CSIFW_TASK_ARGV = NULL;
const static char *CSIFW_TASK_NAME = "csifw_task_Main";

const static int CSIFW_TASK_STACKSIZE = 8192;
const static int CSIFW_TASK_PRIORITY = 100;
const static int CSIFW_TASK_PERMISSION = TM_APP_PERMISSION_ALL;

int csifw_task_Main(int argc, char *argv[]);

int start_csi_framework(csifw_context_t *p_ctx)
{
	if (p_ctx->task_run_state == RUNNING) {
		CSIFW_LOGE("csifw_task already running (task_run_state=%d) - cannot start again", p_ctx->task_run_state);
		return -1;
	}
	CSIFW_LOGD("Current task state: task_run_state=%d", p_ctx->task_run_state);

	CSIFW_LOGD("Starting CSI Framework with context: %p", p_ctx);
	if (sem_init(&p_ctx->csifw_task_sema, 0, 0) < OK) {
		CSIFW_LOGE("Semaphore init failed for csifw_task, errno: %d (%s)", get_errno(), strerror(get_errno()));
		return -1;
	}
	CSIFW_LOGD("Semaphore initialized successfully");
	
	p_ctx->task_run_success = 0; // Track if task initialization succeeded
	p_ctx->task_handle = -1;

	CSIFW_LOGI("Starting CSIFW Task...");
	CSIFW_LOGD("Registering task with params: name=%s, priority=%d, stacksize=%d, permission=%d",
		CSIFW_TASK_NAME, CSIFW_TASK_PRIORITY, CSIFW_TASK_STACKSIZE, CSIFW_TASK_PERMISSION);
	int handle = task_manager_register_task(
			(char *)CSIFW_TASK_NAME, 
			CSIFW_TASK_PRIORITY, 
			CSIFW_TASK_STACKSIZE, 
			csifw_task_Main, 
			(char **)CSIFW_TASK_ARGV, 
			CSIFW_TASK_PERMISSION, 
			TM_RESPONSE_WAIT_INF);

	if (handle < 0) {
		CSIFW_LOGE("Failed to create csifw task to the task manager! Return: %d", handle);
		sem_destroy(&p_ctx->csifw_task_sema);
		CSIFW_LOGD("Destroyed semaphore for %s after failed create", CSIFW_TASK_NAME);
		return handle;
	}
	CSIFW_LOGI("Created csifw task to the task manager PID: %d", handle);
	
	int result = task_manager_start(handle, TM_RESPONSE_WAIT_INF);
	if (result < 0) {
		CSIFW_LOGE("Failed to start %s (handle=%d). Return: %d. Cleaning up...", CSIFW_TASK_NAME, handle, result);
		task_manager_unregister(handle, TM_RESPONSE_WAIT_INF);
		CSIFW_LOGD("Unregistered task %s (handle=%d) after failed start", CSIFW_TASK_NAME, handle);
		sem_destroy(&p_ctx->csifw_task_sema);
		CSIFW_LOGD("Destroyed semaphore for %s after failed start", CSIFW_TASK_NAME);
	} else {
		p_ctx->task_handle = handle;
		sem_wait(&p_ctx->csifw_task_sema);

		if (!p_ctx->task_run_success) {
			CSIFW_LOGE("CSI framework task initialization failed");
			task_manager_unregister(handle, TM_RESPONSE_WAIT_INF);
			sem_destroy(&p_ctx->csifw_task_sema);
			return -1;
		}
		CSIFW_LOGI("Started %s", CSIFW_TASK_NAME);
	}
	return result;
}

int stop_csi_framework(csifw_context_t *p_ctx)
{
	if (p_ctx->task_run_state == STOPPED) {
		CSIFW_LOGE("csifw_task already stopped (task_run_state=%d)", p_ctx->task_run_state);
		return -1;
	}
	
	p_ctx->task_run_state = STOPPED;			//stop the main loop of the task
	CSIFW_LOGD("Waiting for csifw_task to complete...");
	sem_wait(&p_ctx->csifw_task_sema);
	CSIFW_LOGD("Unregistering task %s (handle=%d)", CSIFW_TASK_NAME, p_ctx->task_handle);
	task_manager_unregister(p_ctx->task_handle, TM_RESPONSE_WAIT_INF);
	CSIFW_LOGD("Destroying semaphore for %s", CSIFW_TASK_NAME);
	sem_destroy(&p_ctx->csifw_task_sema);
	return 0;
}

int csifw_task_Main(int argc, char *argv[])
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	p_csifw_ctx->task_run_success = 0;

	if (csi_packet_receiver_initialize() != CSIFW_OK) {
		CSIFW_LOGE("csi_packet_receiver_initialize failed - cannot start CSI framework");
		sem_post(&p_csifw_ctx->csifw_task_sema);
		return -1;
	}
	if (csi_packet_receiver_start_collect() != CSIFW_OK) {
		CSIFW_LOGE("CSI packet receiver start failed");
		csi_packet_receiver_cleanup();
		sem_post(&p_csifw_ctx->csifw_task_sema);
		return -1;
	}

	if (csi_ping_generator_initialize() != CSIFW_OK) {
		CSIFW_LOGE("CSI ping generator initialize failed");
		csi_packet_receiver_stop_collect();
		csi_packet_receiver_cleanup();
		CSIFW_LOGE("CSI packet receiver stopped as Ping Generator failed");
		sem_post(&p_csifw_ctx->csifw_task_sema);
		return -1;
	}
	if (ping_generator_start() != CSIFW_OK) {
		CSIFW_LOGE("CSI ping generator start failed");
		csi_packet_receiver_stop_collect();
		csi_packet_receiver_cleanup();
		csi_ping_generator_cleanup();
		CSIFW_LOGE("CSI packet receiver stopped as Ping Generator start failed");
		sem_post(&p_csifw_ctx->csifw_task_sema);
		return -1;
	}
	//to unlock start_csi_framework()
	p_csifw_ctx->task_run_success = 1;
	p_csifw_ctx->task_run_state = RUNNING;
	sem_post(&p_csifw_ctx->csifw_task_sema);

	while (p_csifw_ctx->task_run_state) {
		usleep(50 * 1000);		//50 milisecond sleep
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
		CSIFW_LOGD("CSI packet receiver cleanup failed");
	}
	//to unlock stop_csi_framework()
	sem_post(&p_csifw_ctx->csifw_task_sema);
	return 0;
}
