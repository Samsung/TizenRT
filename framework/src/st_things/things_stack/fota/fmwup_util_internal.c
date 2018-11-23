/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <sys/types.h>
#include <pthread.h>
#include <errno.h>

#include "fmwup_util_internal.h"
#include "fmwup_util_http.h"
#include "fmwup_util_data.h"
#include "utils/things_rtos_util.h"
#include "utils/things_malloc.h"
#include "logging/things_logger.h"

#define TAG "[things_fota]"

#define TIMEOUT_SEC 5

static int g_thread_running = 0;

static pthread_mutex_t _lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t _cond = PTHREAD_COND_INITIALIZER;

void fmwup_internal_propagate_timed_wait()
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	struct timeval now;			/* start time to wait    */
	struct timespec timeout;	/* timeout value in waiting function */
	struct timezone zone;
	pthread_mutex_lock(&_lock);

	gettimeofday(&now, &zone);
	timeout.tv_sec = now.tv_sec + TIMEOUT_SEC;
	timeout.tv_nsec = now.tv_usec * 1000;
	/* timeval = microsecond */
	/* timespec = nanosecond */
	/* 1 nano = 1000 micro */

	int rc = pthread_cond_timedwait(&_cond, &_lock, &timeout);
	switch (rc) {
	case 0:
		break;
	case ETIMEDOUT:
		THINGS_LOG_E(TAG, "timeout %d second", TIMEOUT_SEC);
		break;
	default:
		THINGS_LOG_E(TAG, "failed pthread_cond_timedwait [%d]", rc);
		break;
	}
	pthread_mutex_unlock(&_lock);

	return;
}

void fmwup_internal_propagate_cond_signal()
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	pthread_mutex_lock(&_lock);
	pthread_cond_signal(&_cond);
	pthread_mutex_unlock(&_lock);
	return;
}

void fmwup_internal_propagate_resource(fmwup_state_e state, fmwup_result_e result, bool wait_flag)
{
	THINGS_LOG_E(TAG, "Propagate state[%d], result[%d]", state, result);

	int ret = 0;

	fmwup_data_set_property_int64(FIRMWARE_PROPERTY_STATE, (int64_t) state);
	fmwup_data_set_property_int64(FIRMWARE_PROPERTY_RESULT, (int64_t) result);

	ret = st_things_notify_observers(FIRMWARE_URI);
	if (ST_THINGS_ERROR_NONE != ret) {
		THINGS_LOG_E(TAG, "Failed st_things_notify_observers [%d]", ret);
		return;
	}

	if (wait_flag) {
		fmwup_internal_propagate_timed_wait();
	}

	if (result != FMWUP_RESULT_INIT) {
		fmwup_data_set_property_int64(FIRMWARE_PROPERTY_STATE, (int64_t) FMWUP_STATE_IDLE);
		fmwup_data_set_property_int64(FIRMWARE_PROPERTY_RESULT, (int64_t) FMWUP_RESULT_INIT);
	}
	return;
}

void _handle_update_command(int64_t update_type)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int result = FMWUP_RESULT_INIT;

	char *package_uri = fmwup_data_get_property(FIRMWARE_PROPERTY_PACKAGE_URI);
	char *new_version = fmwup_data_get_property(FIRMWARE_PROPERTY_NEW_VERSION);
	char *current_version = fmwup_data_get_property(FIRMWARE_PROPERTY_CURRENT_VERSION);
	char *temp_state = fmwup_data_get_property(FIRMWARE_PROPERTY_STATE);
	int state = 0;

	if (!package_uri || !new_version || !current_version || !temp_state) {
		THINGS_LOG_E(TAG, "failed get property");
		result = FMWUP_RESULT_UPDATE_FAILED;
		goto _END_OF_FUNC_;
	}

	state = atoi(temp_state);
	if (strcmp(package_uri, "") == 0 || strcmp(new_version, "") == 0 || strcmp(current_version, new_version) == 0) {
		THINGS_LOG_E(TAG, "Invalid value, package_uri[%s], new_version[%s], current_version[%s]", package_uri, new_version, current_version);
		result = FMWUP_RESULT_UPDATE_FAILED;
		goto _END_OF_FUNC_;
	}

	THINGS_LOG_D(TAG, "state : %d, update_type : %d", state, update_type);

	if ((state == FMWUP_STATE_IDLE) && (update_type == FMWUP_COMMAND_DOWNLOAD || update_type == FMWUP_COMMAND_DOWNLOADUPDATE)) {
		THINGS_LOG_V(TAG, "***Downloading image from [%s] ***", package_uri);
		state = FMWUP_STATE_DOWNLOADING;
		fmwup_internal_propagate_resource(FMWUP_STATE_DOWNLOADING, FMWUP_RESULT_INIT, true);

		if (fmwup_http_download_file(package_uri) != 0) {
			THINGS_LOG_E(TAG, "fmwup_http_download_file failed");
			result = FMWUP_RESULT_INVALID_URI;
			goto _END_OF_FUNC_;
		}

		THINGS_LOG_V(TAG, "*** Firmware image downloaded ***");
		state = FMWUP_STATE_DOWNLOADED;
		fmwup_internal_propagate_resource(FMWUP_STATE_DOWNLOADED, FMWUP_RESULT_INIT, true);
	}

	sleep(10);

	if ((state == FMWUP_STATE_DOWNLOADED) && (update_type == FMWUP_COMMAND_UPDATE || update_type == FMWUP_COMMAND_DOWNLOADUPDATE)) {
		state = FMWUP_STATE_UPDATING;
		fmwup_internal_propagate_resource(FMWUP_STATE_UPDATING, FMWUP_RESULT_INIT, true);

		key_manager_save_data();

		fotahal_update();
	}

_END_OF_FUNC_:
	if (result != FMWUP_RESULT_INIT) {
		THINGS_LOG_E(TAG, "propagate printf[%d]", result);
		state = FMWUP_STATE_IDLE;
		fmwup_internal_propagate_resource(FMWUP_STATE_IDLE, result, true);
	}

	return;
}

void *_update_worker(void *data)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int64_t exec_type = *(int64_t *) data;
	_handle_update_command(exec_type);
	things_free(data);

	pthread_mutex_lock(&_lock);
	g_thread_running = 0;
	pthread_mutex_unlock(&_lock);

	return;
}

int fmwup_internal_update_command(int64_t update_type)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	pthread_mutex_lock(&_lock);

	if (g_thread_running) {
		/* Allow only one running thread */
		pthread_mutex_unlock(&_lock);
		THINGS_LOG_E(TAG, "duplicated request");
		return FMWUP_ERROR_NONE;
	} else {
		g_thread_running = 1;
	}

	pthread_mutex_unlock(&_lock);

	int64_t *update = (int64_t *) things_calloc(1, sizeof(int64_t));
	if (!update) {
		THINGS_LOG_E(TAG, "Memory allocation error!");
		return FMWUP_ERROR_MEMORY_ERROR;
	}

	*update = update_type;

	pthread_t fota_thread_handler;
	if (pthread_create_rtos(&fota_thread_handler, NULL, _update_worker, (void *)update, THGINS_STACK_FOTA_UPDATE_THREAD) != 0) {
		THINGS_LOG_E(TAG, "Create thread is failed.");
		return FMWUP_ERROR_OPERATION_FAILED;
	}

	return FMWUP_ERROR_NONE;
}
