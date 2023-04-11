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

#include <tinyara/config.h>
#include <iostream>
#include <semaphore.h>
#include <errno.h>
#include "lw_aifw/lw_aifw_log.h"
#include "lw_aifw/AIServiceCore.h"

#ifndef CONFIG_LW_AIFW_MODEL_MEM_POOL_SIZE
#define LW_AIFW_MODEL_MEM_POOL_SIZE 4096
#else
#define LW_AIFW_MODEL_MEM_POOL_SIZE CONFIG_LW_AIFW_MODEL_MEM_POOL_SIZE
#endif

#ifndef CONFIG_LW_AIFW_INFERENCE_PERIOD
#define CONFIG_LW_AIFW_INFERENCE_PERIOD 1
#endif

#ifndef NULL
#define NULL (void *)0
#endif

#define LW_AIFW_TIMER_SIGNAL 17

namespace lw_aifw {

static sem_t gTimerSemaphore;
static int gSignalReceivedCounter = 0;
static timer_t gTimerId;

static void *lw_aifw_timerthread_cb(void *parameter);

AIServiceCore::AIServiceCore() : mTimerInterval(0), mInferencePeriod(0), mAppMetadata(0)
{
	mDataBuffer = std::make_shared<AIDataBuffer>();
	LW_AIFW_LOGV("object created");
}

AIServiceCore::~AIServiceCore()
{
	LW_AIFW_LOGV("Destructor");
}

LW_AIFW_RESULT AIServiceCore::init(struct AIAppMetadata *appMetadata, std::shared_ptr<lw_aifw::AIDataSource> dataSource)
{
	LW_AIFW_RESULT ret;
	mTimerInterval = CONFIG_LW_AIFW_SERVICE_INTERVAL;
	mDataSource = dataSource;
	mDataSource->setAIDataCollectedListener(AIServiceCore::onAIDataCollectedListener, (void *)this);
	mAppMetadata = appMetadata;

	ret = loadModel(appMetadata);
	if (ret != LW_AIFW_OK) {
		return ret;
	}
	mDataBuffer->init();
	LW_AIFW_LOGV("init done");
	return ret;
}

uint32_t AIServiceCore::getTimerInterval(void)
{
	return mTimerInterval;
}

LW_AIFW_RESULT AIServiceCore::init(struct AIAppMetadata *appMetadata)
{
	/* Do not create timer & dataSource in this time */
	LW_AIFW_RESULT ret;
	mTimerInterval = 0; /* do not use timer but set 0 here */
	mAppMetadata = appMetadata;
	ret = loadModel(appMetadata);
	if (ret != LW_AIFW_OK) {
		return ret;
	}
	mDataBuffer->init();
	return ret;
}

LW_AIFW_RESULT AIServiceCore::loadModel(struct AIAppMetadata *appMetadata)
{
	LW_AIFW_RESULT ret = LW_AIFW_ERROR;
	mAppMetadata = appMetadata;
	for (uint16_t m = 0; m < appMetadata->ModelCount; m++) {
		m_AIModel[m] = std::make_shared<lw_aifw::AIModel>(LW_AIFW_MODEL_MEM_POOL_SIZE);
		if (mAppMetadata->Models[m].ModelType == ARRAY_MODEL) {
			ret = m_AIModel[m]->loadModel(mAppMetadata->Models[m].Model);
		} else if (mAppMetadata->Models[m].ModelType == FILE_MODEL) {
			ret = m_AIModel[m]->loadModel((const char *)mAppMetadata->Models[m].Model);
			LW_AIFW_LOGV("model: %s\n", mAppMetadata->Models[m].Model);
		} else {
			LW_AIFW_LOGE("model type %d not supported", mAppMetadata->Models[m].ModelType);
			ret = LW_AIFW_ERROR;
			break;
		}
	}
	return ret;
}

LW_AIFW_RESULT AIServiceCore::startTimer(void)
{
	LW_AIFW_LOGV("startTimer");
	int result = pthread_create(&mTimerThread, NULL, lw_aifw_timerthread_cb, (void *)this);
	if (result != 0) {
		LW_AIFW_LOGE("ERROR Failed to start lw_aifw_timerthread_cb");
		return LW_AIFW_ERROR;
	}
	LW_AIFW_LOGV("Started lw_aifw_timerthread_cb");
	return LW_AIFW_OK;
}

LW_AIFW_RESULT AIServiceCore::stopTimer(void)
{
	LW_AIFW_LOGV("stopTimer");
	if (sem_post(&gTimerSemaphore) != 0) {
		LW_AIFW_LOGE("stop timer failed %d", errno);
		return LW_AIFW_ERROR;
	}
	gSignalReceivedCounter = 0;
	LW_AIFW_LOGV("stopTimer posted");
	return LW_AIFW_OK;
}

LW_AIFW_RESULT AIServiceCore::doInference(uint16_t modelId, void *input, void **output)
{
	*output = m_AIModel[modelId]->invoke(input);
	if (*output == NULL) {
		LW_AIFW_LOGE("Inference failed");
		return LW_AIFW_ERROR;
	}
	return LW_AIFW_OK;
}

LW_AIFW_RESULT AIServiceCore::getModelDimensions(int *input, int *output, uint16_t modelId)
{
	m_AIModel[modelId]->getInOutDimensions(input, output);
	LW_AIFW_LOGV("model offset %d count of input: %d, output: %d", modelId, input, output);
	return LW_AIFW_OK;
}

int AIServiceCore::getBufferDataSetCount()
{
	return mDataBuffer->getDataSetCount();
}

LW_AIFW_RESULT AIServiceCore::getSensorData(float *data, uint16_t dataPerSet, uint16_t setCount)
{
	return mDataBuffer->read(data, dataPerSet, setCount);
}

LW_AIFW_RESULT AIServiceCore::getSensorData(float *result, uint16_t index)
{
	return mDataBuffer->read(result, index);
}

LW_AIFW_RESULT AIServiceCore::setInterval(uint16_t interval)
{
	if (interval <= 0) {
		LW_AIFW_LOGE("Invalid argument interval: %d", interval);
		return LW_AIFW_ERROR;
	}
	struct itimerspec timer;
	uint32_t interval_secs = interval / 1000;
	uint64_t interval_nsecs = (interval % 1000) * 1000000;
	LW_AIFW_LOGV("setInterval: %d %d %lld", interval, interval_secs, interval_nsecs);
	timer.it_value.tv_sec = interval_secs;
	timer.it_value.tv_nsec = interval_nsecs;
	timer.it_interval.tv_sec = interval_secs;
	timer.it_interval.tv_nsec = interval_nsecs;
	int status = timer_settime(gTimerId, 0, &timer, NULL);
	if (status != OK) {
		LW_AIFW_LOGE("setInterval: timer_settime failed, errno: %d", errno);
		return LW_AIFW_ERROR;
	}
	mTimerInterval = interval;
	return LW_AIFW_OK;
}

LW_AIFW_RESULT AIServiceCore::prepareData(void)
{
	LW_AIFW_LOGV("AIServiceCore::prepareData");
	LW_AIFW_RESULT ret = mDataSource->getDataAsync();
	if (ret != LW_AIFW_OK) {
		LW_AIFW_LOGE("get Data async operation failed = %d", ret);
	}
	return ret;
}

void AIServiceCore::executeDataReadyListener(LW_AIFW_RESULT result)
{
	mInferencePeriod++;
	if (mInferencePeriod == CONFIG_LW_AIFW_INFERENCE_PERIOD) {
		mAppMetadata->DataReadyListener(result);
		mInferencePeriod = 0;
	}
}

void AIServiceCore::processAIData(LW_AIFW_RESULT result, float *data, uint16_t dataCount)
{
	if (result == LW_AIFW_OK) {
		LW_AIFW_RESULT writeRes = this->mDataBuffer->write(data, dataCount);
		if (writeRes != LW_AIFW_OK) {
			LW_AIFW_LOGE("Write operation failed ret: %d", writeRes);
			result = LW_AIFW_ERROR;
		}
	} else {
		LW_AIFW_LOGE("Data receive failed: %d", result);
	}
	this->executeDataReadyListener(result);
}

void AIServiceCore::onAIDataCollectedListener(LW_AIFW_RESULT result, float *data, uint16_t dataCount, void *args)
{
	((AIServiceCore *)args)->processAIData(result, data, dataCount);
}

void AIServiceCore::pushSensorData(float *data, uint16_t len)
{
	LW_AIFW_RESULT ret;
	ret = this->mDataBuffer->write(data, len);
	if (ret != LW_AIFW_OK) {
		LW_AIFW_LOGE("Write operation failed ret: %d", ret);
	}
	this->executeDataReadyListener(ret);
}

void AIServiceCore::lw_aifw_timer_cb(int signo, siginfo_t *info, void *ucontext)
{
	LW_AIFW_LOGV("lw_aifw_timer_cb: Received signal %d", signo);
	if (signo != LW_AIFW_TIMER_SIGNAL) {
		LW_AIFW_LOGE("lw_aifw_timer_cb: ERROR expected signo: %d", LW_AIFW_TIMER_SIGNAL);
		return;
	}
	if (info->si_signo != LW_AIFW_TIMER_SIGNAL) {
		LW_AIFW_LOGE("lw_aifw_timer_cb: ERROR expected si_signo=%d, got=%d", LW_AIFW_TIMER_SIGNAL, info->si_signo);
		return;
	}
	if (info->si_code != SI_TIMER) {
		LW_AIFW_LOGE("lw_aifw_timer_cb: ERROR si_code=%d, expected SI_TIMER=%d", info->si_code, SI_TIMER);
		return;
	}
	LW_AIFW_LOGV("lw_aifw_timer_cb: si_code=%d (SI_TIMER)", info->si_code);
	gSignalReceivedCounter++;
	AIServiceCore *arg = (AIServiceCore *)info->si_value.sival_ptr;
	if (!arg) {
		LW_AIFW_LOGE("null argument received");
		return;
	}
	arg->prepareData();
}

static void *lw_aifw_timerthread_cb(void *parameter)
{
	sigset_t sigset;
	struct sigaction act;
	struct sigevent notify;
	struct itimerspec timer;
	int status;

	if (!parameter) {
		LW_AIFW_LOGE("lw_aifw_timerthread_cb: invalid argument");
		return NULL;
	}

	uint32_t interval = ((AIServiceCore*)parameter)->getTimerInterval();
	if (interval == 0) {
		LW_AIFW_LOGE("lw_aifw_timerthread_cb: invalid argument");
		return NULL;
	}
	uint32_t interval_secs = interval / 1000;
	uint64_t interval_nsecs = (interval % 1000) * 1000000;

	LW_AIFW_LOGV("lw_aifw_timerthread_cb: Initializing semaphore to 0");
	sem_init(&gTimerSemaphore, 0, 0);

	/* Start waiter thread  */
	LW_AIFW_LOGV("lw_aifw_timerthread_cb: Unmasking signal %d", LW_AIFW_TIMER_SIGNAL);
	(void)sigemptyset(&sigset);
	(void)sigaddset(&sigset, LW_AIFW_TIMER_SIGNAL);
	status = sigprocmask(SIG_UNBLOCK, &sigset, NULL);
	if (status != OK) {
		LW_AIFW_LOGE("lw_aifw_timerthread_cb: ERROR sigprocmask failed, status=%d", status);
		return NULL;
	}

	LW_AIFW_LOGV("lw_aifw_timerthread_cb: Registering signal handler");
	act.sa_sigaction = AIServiceCore::lw_aifw_timer_cb;
	act.sa_flags = SA_SIGINFO;

	(void)sigfillset(&act.sa_mask);
	(void)sigdelset(&act.sa_mask, LW_AIFW_TIMER_SIGNAL);
	status = sigaction(LW_AIFW_TIMER_SIGNAL, &act, NULL);
	if (status != OK) {
		LW_AIFW_LOGE("lw_aifw_timerthread_cb: ERROR sigaction failed, status=%d", status);
		return NULL;
	}

	/* Create the POSIX timer */
	LW_AIFW_LOGV("lw_aifw_timerthread_cb: Creating timer");
	notify.sigev_notify = SIGEV_SIGNAL;
	notify.sigev_signo = LW_AIFW_TIMER_SIGNAL;
	notify.sigev_value.sival_ptr = parameter;
	status = timer_create(CLOCK_REALTIME, &notify, &gTimerId);
	if (status != OK) {
		LW_AIFW_LOGE("lw_aifw_timerthread_cb: timer_create failed, errno=%d", errno);
		goto errorout;
	}

	/* Start the POSIX timer */
	LW_AIFW_LOGV("lw_aifw_timerthread_cb: Starting timer");
	LW_AIFW_LOGV("lw_aifw_timerthread_cb: %d %d %lld", interval, interval_secs, interval_nsecs);
	timer.it_value.tv_sec = interval_secs;
	timer.it_value.tv_nsec = interval_nsecs;
	timer.it_interval.tv_sec = interval_secs;
	timer.it_interval.tv_nsec = interval_nsecs;

	status = timer_settime(gTimerId, 0, &timer, NULL);
	if (status != OK) {
		LW_AIFW_LOGE("lw_aifw_timerthread_cb: timer_settime failed, errno=%d", errno);
		goto errorout;
	}
	LW_AIFW_LOGV("lw_aifw_timerthread_cb: exiting function");

	/* Take the semaphore */
	while (1) {
		LW_AIFW_LOGV("lw_aifw_timerthread_cb: Waiting on semaphore");
		status = sem_wait(&gTimerSemaphore);
		if (status != 0) {
			int error = errno;
			if (error == EINTR) {
				LW_AIFW_LOGV("lw_aifw_timerthread_cb: sem_wait() successfully interrupted by signal");
			} else {
				LW_AIFW_LOGE("lw_aifw_timerthread_cb: ERROR sem_wait failed, errno=%d", error);
			}
		} else {
			LW_AIFW_LOGI("lw_aifw_timerthread_cb: ERROR awakened with no error!");
			break;
		}
		LW_AIFW_LOGV("lw_aifw_timerthread_cb: Signal received counter: %d", gSignalReceivedCounter);
	}
errorout:
	LW_AIFW_LOGV("sem_destroy");
	sem_destroy(&gTimerSemaphore);
	/* Then delete the timer */
	LW_AIFW_LOGV("lw_aifw_timerthread_cb: Deleting timer");
	status = timer_delete(gTimerId);
	if (status != OK) {
		LW_AIFW_LOGE("lw_aifw_timerthread_cb: timer_delete failed, errno=%d", errno);
	}
	/* Detach the signal handler */
	act.sa_handler = SIG_DFL;
	status = sigaction(LW_AIFW_TIMER_SIGNAL, &act, NULL);
	LW_AIFW_LOGV("lw_aifw_timerthread_cb: done");
	return NULL;
}

} // namespace lw_aifw
