/* ****************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include "StreamHandler.h"

#include <debug.h>

#ifndef CONFIG_HANDLER_STREAM_THREAD_PRIORITY
#define CONFIG_HANDLER_STREAM_THREAD_PRIORITY 199
#endif

namespace media {
namespace stream {

StreamHandler::StreamHandler() :
	mWorker(0),
	mWorkerStackSize(4096),
	mIsWorkerAlive(false)
{
}

bool StreamHandler::prepare(size_t buffSize)
{
	if (!mDataSource) {
		meddbg("mDataSource is nullptr!\n");
		return false;
	}

	if (!getStreamBuffer()) {
		auto streamBuffer = StreamBuffer::Builder()
								.setBufferSize(buffSize)
								.setThreshold(CONFIG_HANDLER_STREAM_BUFFER_THRESHOLD)
								.build();

		if (!streamBuffer) {
			meddbg("streamBuffer is nullptr!\n");
			return false;
		}

		setStreamBuffer(streamBuffer);
	}

	if (!(mDataSource->isPrepared() || mDataSource->open())) {
		meddbg("open data source failed!\n");
		return false;
	}

	if (!probeDataSource()) {
		meddbg("probe data source failed!\n");
		return false;
	}

	if (!registerCodec(mDataSource->getAudioType(), mDataSource->getChannels(), mDataSource->getSampleRate())) {
		meddbg("register codec failed!\n");
		return false;
	}

	return true;
}

bool StreamHandler::open()
{
	if (!prepare(CONFIG_HANDLER_STREAM_BUFFER_SIZE)) {
		meddbg("prepare stream handler failed!");
		return false;
	}

	if (!start()) {
		meddbg("start stream handler failed!\n");
		return false;
	}

	return true;
}

bool StreamHandler::open(size_t buffSize)
{
	if (!prepare(buffSize)) {
		meddbg("prepare stream handler failed!");
		return false;
	}

	if (!start()) {
		meddbg("start stream handler failed!\n");
		return false;
	}

	return true;
}

bool StreamHandler::close()
{
	stop();
	unregisterCodec();
	return mDataSource->close();
}

bool StreamHandler::start()
{
	medvdbg("StreamHandler::start()\n");
	if (mDataSource && !mDataSource->isPrepared()) {
		meddbg("DataSource is not prepared!\n");
		return false;
	}

	createWorker();
	return mIsWorkerAlive;
}

bool StreamHandler::stop()
{
	destroyWorker();
	return true;
}

void StreamHandler::createWorker()
{
	meddbg("StreamHandler::createWorker, mIsWorkerAlive : %d\n", mIsWorkerAlive.load());
	if (mStreamBuffer && !mIsWorkerAlive) {
		resetWorker();
		mStreamBuffer->reset();
		mIsWorkerAlive = true;

		struct sched_param sparam;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, mWorkerStackSize);
		sparam.sched_priority = CONFIG_HANDLER_STREAM_THREAD_PRIORITY;
		pthread_attr_setschedparam(&attr, &sparam);
		int ret = pthread_create(&mWorker, &attr, StreamHandler::workerMain, this);
		if (ret != OK) {
			meddbg("Fail to create StreamHandler Worker thread, return value : %d\n", ret);
			mIsWorkerAlive = false;
			return;
		}
		pthread_setname_np(mWorker, getWorkerName());
	}
}

void StreamHandler::destroyWorker()
{
	meddbg("StreamHandler::destoryWorker()\n");
	if (mIsWorkerAlive) {
		// Setup flag,
		mIsWorkerAlive = false;

		// Worker may be blocked in buffer reading.
		meddbg("before setEndStream\n");
		mBufferWriter->setEndOfStream();

		// Wake worker up,
		meddbg("before wakenWorker\n");
		wakenWorker();
		meddbg("after wakenWorker\n");

		// Join thread.
		pthread_join(mWorker, NULL);
		meddbg("after pthread join\n");
	}
}

void StreamHandler::sleepWorker()
{
	std::unique_lock<std::mutex> lock(mMutex);
	// In case of overrun, DO NOT sleep worker.
	if (mIsWorkerAlive) {
		mCondv.wait(lock);
	}
}

void StreamHandler::wakenWorker()
{
	std::lock_guard<std::mutex> lock(mMutex);
	mCondv.notify_one();
}

void *StreamHandler::workerMain(void *arg)
{
	meddbg("StreamHandler::workerMain()\n");

	if (arg == nullptr) {
		meddbg("%s[line : %d] Fail : arg is nullptr\n", __func__, __LINE__);
		return NULL;
	}

	auto stream = static_cast<StreamHandler *>(arg);

	while (stream->mIsWorkerAlive) {
		// Waken up by a writing/flushing/stopping operation
		meddbg("inside workerMain before sleep\n");
		stream->sleepWorker();
		meddbg("inside workerMain after sleep\n");

		// Worker may be stoped
		if (!stream->mIsWorkerAlive || !stream->processWorker()) {
			meddbg("before break\n");
			break;
		}
	}

	meddbg("StreamHandler exit\n");
	return NULL;
}

void StreamHandler::setStreamBuffer(std::shared_ptr<StreamBuffer> streamBuffer)
{
	if (mStreamBuffer) {
		mStreamBuffer->setObserver(nullptr);
		mBufferReader = nullptr;
		mBufferWriter = nullptr;
	}

	mStreamBuffer = streamBuffer;

	if (mStreamBuffer) {
		mStreamBuffer->setObserver(this);
		mBufferReader = std::make_shared<StreamBufferReader>(mStreamBuffer);
		mBufferWriter = std::make_shared<StreamBufferWriter>(mStreamBuffer);
	}
}

} // namespace stream
} // namespace media
