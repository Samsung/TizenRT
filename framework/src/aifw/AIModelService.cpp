/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#include "aifw/aifw_timer.h"
#include <stdlib.h>
#include <memory>
#include "aifw/aifw.h"
#include "aifw/aifw_log.h"
#include "aifw/AIModelService.h"
#include "aifw/AIInferenceHandler.h"

namespace aifw {

AIModelService::AIModelService(CollectRawDataListener collectRawDataCallback, std::shared_ptr<AIInferenceHandler> inferenceHandler) :
	mInterval(0), mServiceRunning(false), mInferenceHandler(inferenceHandler), mCollectRawDataCallback(collectRawDataCallback), mTimer(NULL)
{
}

AIModelService::~AIModelService()
{
	freeTimer();
	AIFW_LOGV("model service object destoyed");
}

AIFW_RESULT AIModelService::freeTimer(void)
{
	if (mTimer) {
		int status = sem_wait(&(mTimer->exitSemaphore));
		if (status != 0) {
			int error = errno;
			AIFW_LOGE("ERROR sem_wait failed, errno=%d", error);
			return AIFW_ERROR;
		}
		aifw_timer_result res = aifw_timer_destroy(mTimer);
		if (res != AIFW_TIMER_SUCCESS) {
			AIFW_LOGE("Destroying timer failed. ret: %d", res);
			return AIFW_ERROR;
		}
		free(mTimer);
		mTimer = NULL;
	}
	return AIFW_OK;
}

AIFW_RESULT AIModelService::start(void)
{
	if (mServiceRunning) {
		AIFW_LOGV("Service already running.");
		return AIFW_OK;
	}
	if (mInterval == 0) {
		mServiceRunning = true;
		return AIFW_OK;
	}
	AIFW_RESULT ret = setInterval(mInterval);
	if (ret != AIFW_OK) {
		AIFW_LOGE("timer set Failed, interval = %d msec", mInterval);
		return ret;
	}
	AIFW_LOGV("timer set OK, interval = %d msec", mInterval);
	mServiceRunning = true;
	return AIFW_OK;
}

AIFW_RESULT AIModelService::stop(void)
{
	if (!mServiceRunning) {
		AIFW_LOGV("Service already stopped.");
		return AIFW_OK;
	}
	if (mInterval == 0) {
		mServiceRunning = false;
		return AIFW_OK;
	}
	aifw_timer_result dret = AIFW_TIMER_SUCCESS;
	dret = aifw_timer_stop(mTimer);
	if (dret != AIFW_TIMER_SUCCESS) {
		AIFW_LOGE("Timer stop failed, error: %d", dret);
		return AIFW_ERROR;
	}
	mServiceRunning = false;
	return AIFW_OK;
}

/* ToDo: Interval needs to be updated in json file so that updated value is used after device restarts */
AIFW_RESULT AIModelService::setInterval(uint16_t interval)
{
	aifw_timer_result ret;
	if (!mTimer) {
		AIFW_LOGE("Timer not created yet, Ignoring request");
		return AIFW_ERROR;
	}
	if (interval <= 0) {
		AIFW_LOGE("Invalid interval=%d Ignoring request", interval);
		return AIFW_ERROR;
	}
	ret = aifw_timer_change_interval(mTimer, (unsigned int)interval);
	if (ret != AIFW_TIMER_SUCCESS) {
		AIFW_LOGE("timer interval change failed=%d", ret);
		return AIFW_ERROR;
	}
	AIFW_LOGI("Timer change interval success for interval=%d msec", interval);

	return AIFW_OK;
}

AIFW_RESULT AIModelService::pushData(void *data, uint16_t count)
{
	if (!mServiceRunning) {
		AIFW_LOGE("Service not running");
		return AIFW_SERVICE_NOT_RUNNING;
	}
	return mInferenceHandler->pushData(data, count);
}

AIFW_RESULT AIModelService::prepare(void)
{
	AIFW_RESULT res = mInferenceHandler->prepare();
	if (res != AIFW_OK) {
		AIFW_LOGE("inference handler prepare api failed");
		return res;
	}
	mInterval = mInferenceHandler->getModelServiceInterval();
	AIFW_LOGV("Timer interval %d", mInterval);
	if (mInterval > 0) {
		mTimer = (aifw_timer *)calloc(1, sizeof(aifw_timer));
		if (mTimer == NULL) {
			AIFW_LOGE("Memory allocation failed for timer");
			return AIFW_NO_MEM;
		}
		aifw_timer_result ret;
		/* create timer */
		ret = aifw_timer_create(mTimer, (void *)timerTaskHandler, (void *)this, mInterval);
		if (ret != AIFW_TIMER_SUCCESS) {
			AIFW_LOGE("Timer creation failed. ret: %d", ret);
			return AIFW_ERROR;
		}
		AIFW_LOGV("Timer created OK");
	}
	return AIFW_OK;
}

AIFW_RESULT AIModelService::clearData(void)
{
	if (!mServiceRunning) {
		AIFW_LOGE("Service not running");
		return AIFW_SERVICE_NOT_RUNNING;
	}
	return mInferenceHandler->clearData();
}

AIFW_RESULT AIModelService::clearData(uint16_t offset, uint16_t count)
{
	if (!mServiceRunning) {
		AIFW_LOGE("Service not running");
		return AIFW_SERVICE_NOT_RUNNING;
	}
	return mInferenceHandler->clearData(offset, count);
}

CollectRawDataListener AIModelService::getCollectRawDataCallback(void)
{
	return mCollectRawDataCallback;
}

void AIModelService::timerTaskHandler(void *args)
{
	AIModelService *modelService = (AIModelService *)args;
	(modelService->getCollectRawDataCallback())();
}

} /* namespace aifw */

