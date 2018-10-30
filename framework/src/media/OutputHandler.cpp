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

#include <tinyara/config.h>

#include <pthread.h>
#include <debug.h>

#include "OutputHandler.h"
#include "MediaRecorderImpl.h"

#ifndef CONFIG_HANDLER_STREAM_BUFFER_SIZE
#define CONFIG_HANDLER_STREAM_BUFFER_SIZE 4096
#endif

#ifndef CONFIG_HANDLER_STREAM_BUFFER_THRESHOLD
#define CONFIG_HANDLER_STREAM_BUFFER_THRESHOLD 2048
#endif

namespace media {
namespace stream {

OutputHandler::OutputHandler() :
	mOutputDataSource(nullptr),
	mWorker(0),
	mIsFlushing(false),
	mIsWorkerAlive(false)
{
}

void OutputHandler::setOutputDataSource(std::shared_ptr<OutputDataSource> source)
{
	mOutputDataSource = source;
}

bool OutputHandler::open()
{
	if (!getStreamBuffer()) {
		auto streamBuffer = StreamBuffer::Builder()
								.setBufferSize(CONFIG_HANDLER_STREAM_BUFFER_SIZE)
								.setThreshold(CONFIG_HANDLER_STREAM_BUFFER_THRESHOLD)
								.build();

		if (!streamBuffer) {
			meddbg("streamBuffer is nullptr!\n");
			return false;
		}

		setStreamBuffer(streamBuffer);
	}

	if (mOutputDataSource->open() && registerEncoder(mOutputDataSource->getAudioType(), mOutputDataSource->getChannels(), mOutputDataSource->getSampleRate())) {
		start();
		return true;
	}

	return false;
}

bool OutputHandler::close()
{
	stop();
	return mOutputDataSource->close();
}

ssize_t OutputHandler::write(unsigned char *buf, size_t size)
{
	medvdbg("OutputHandler::write(), size %u\n", size);

	if (buf == nullptr) {
		meddbg("buf is nullptr, hence return EOF\n");
		return (ssize_t)EOF;
	}

	std::shared_ptr<Encoder> encoder = mEncoder;

	size_t wlen = 0;
	while (wlen < size) {
		// Has an encoder?
		if (encoder) {
			// Yes... Push data to encode stream as much as possible.
			size_t pushed = encoder->pushData(buf + wlen, size - wlen);
			if (pushed == 0) {
				meddbg("Can not push any data! Error occurred during encoding!\n");
				break;
			}
			wlen += pushed;

			while (1) {
				// Encoded data size is usually smaller than origin PCM data size,
				// So we can reuse 'wlen' bytes free space in 'buf'.
				// Process encoding and get encoded data.
				size_t ret = wlen;
				if (!encoder->getFrame(buf, &ret)) {
					// Normal case, break and continue to push more PCM data
					break;
				}

				// Write encoded data to output stream.
				ssize_t written = writeToStreamBuffer(buf, ret);
				medvdbg("written size: %d\n", written);
				if (written != (ssize_t)ret) {
					meddbg("Can not write all!\n");
					break;
				}
			}
		} else {
			// No... Write original PCM data to output stream.
			wlen += writeToStreamBuffer(buf + wlen, size - wlen);
			medvdbg("written size : %d\n", wlen);
			break;
		}
	}

	medvdbg("OutputHandler::write(), written %u\n", size);
	return (ssize_t)wlen;
}

bool OutputHandler::start()
{
	medvdbg("OutputHandler::start()\n");
	if (!mOutputDataSource->isPrepare()) {
		return false;
	}

	createWorker();
	return true;
}

bool OutputHandler::stop()
{
	medvdbg("OutputHandler::stop()\n");
	flush();

	destroyWorker();
	return true;
}

void OutputHandler::flush()
{
	medvdbg("OutputHandler::flush() enter\n");

	if (mIsWorkerAlive) {
		std::unique_lock<std::mutex> lock(mFlushMutex);
		mIsFlushing = true;
		wakenWorker();
		mFlushCondv.wait(lock);
	}

	medvdbg("OutputDataSource::flush() exit\n");
}

void OutputHandler::createWorker()
{
	medvdbg("OutputHandler::createWorker()\n");
	if (mStreamBuffer && !mIsWorkerAlive) {
		mStreamBuffer->reset();
		mIsFlushing = false;
		mIsWorkerAlive = true;

		long stackSize = CONFIG_OUTPUT_DATASOURCE_STACKSIZE;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, stackSize);
		int ret = pthread_create(&mWorker, &attr, static_cast<pthread_startroutine_t>(OutputHandler::workerMain), this);
		if (ret != OK) {
			meddbg("Fail to create DataSourceWorker thread, return value : %d\n", ret);
			mIsWorkerAlive = false;
			return;
		}
		pthread_setname_np(mWorker, "OutputHandler");
	}
}

void OutputHandler::destroyWorker()
{
	medvdbg("OutputHandler::destoryWorker()\n");
	if (mIsWorkerAlive) {
		// Setup flag,
		mIsWorkerAlive = false;

		// Worker may be blocked in buffer reading.
		mBufferWriter->setEndOfStream();

		// Wake worker up,
		wakenWorker();

		// Join thread.
		pthread_join(mWorker, NULL);
	}
}

void OutputHandler::writeToSource(size_t size)
{
	auto buf = new unsigned char[size];
	auto readed = mBufferReader->read(buf, size);
	if (readed != size) {
		meddbg("StreamBufferReader::read failed! size : %u, readed : %u\n", size, readed);
		delete[] buf;
		return;
	}

	auto written = mOutputDataSource->write(buf, size);
	if (written <= 0) {
		// Error occurred, stop outputting
		meddbg("OutputDataSource::write returned <= 0! size : %u, written : %d\n", size, written);
		mBufferWriter->setEndOfStream();
	}

	delete[] buf;
}

void *OutputHandler::workerMain(void *arg)
{
	medvdbg("OutputHandler::workerMain()\n");

	if (arg == nullptr) {
		meddbg("%s[line : %d] Fail : arg is nullptr\n", __func__, __LINE__);
		return NULL;
	}

	auto stream = static_cast<OutputHandler *>(arg);
	auto worker = stream->mOutputDataSource;

	while (stream->mIsWorkerAlive) {
		// Waken up by a writing/flushing/stopping operation
		stream->sleepWorker();

		// Worker may be stoped
		if (!stream->mIsWorkerAlive) {
			break;
		}

		auto size = stream->mBufferReader->sizeOfData();
		if (stream->mIsFlushing) {
			if (size != 0) {
				stream->writeToSource(size);
			}
			std::unique_lock<std::mutex> lock(stream->mFlushMutex);
			stream->mIsFlushing = false;
			stream->mFlushCondv.notify_one();
		} else if (size >= stream->mStreamBuffer->getThreshold()) {
			stream->writeToSource(size);
		}
	}

	medvdbg("OutputHandler exit\n");
	return NULL;
}

void OutputHandler::sleepWorker()
{
	size_t spaces = mBufferWriter->sizeOfSpace();
	std::unique_lock<std::mutex> lock(mMutex);
	// In case of overrun, DO NOT sleep worker.
	if (mIsWorkerAlive && (spaces > 0)) {
		mCondv.wait(lock);
	}
}

void OutputHandler::wakenWorker()
{
	std::lock_guard<std::mutex> lock(mMutex);
	mCondv.notify_one();
}

void OutputHandler::setStreamBuffer(std::shared_ptr<StreamBuffer> streamBuffer)
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

void OutputHandler::onBufferOverrun()
{
	auto mr = getRecorder();
	if (mr) {
		mr->notifyObserver(RECORDER_OBSERVER_COMMAND_BUFFER_OVERRUN);
	}
}

void OutputHandler::onBufferUnderrun()
{
	auto mr = getRecorder();
	if (mr) {
		mr->notifyObserver(RECORDER_OBSERVER_COMMAND_BUFFER_UNDERRUN);
	}
}

void OutputHandler::onBufferUpdated(ssize_t change, size_t current)
{
	medvdbg("OutputHandler::onBufferUpdated(%d, %u)\n", change, current);
	if (change > 0) {
		// Writing wake worker up
		wakenWorker();
	}
}

ssize_t OutputHandler::writeToStreamBuffer(unsigned char *buf, size_t size)
{
	if (buf == nullptr) {
		meddbg("%s[line : %d] Fail : buf is nullptr\n");
		return (ssize_t)EOF;
	}

	size_t written = 0;
	if (mBufferWriter) {
		written = mBufferWriter->write(buf, size);
	}

	return (ssize_t)written;
}

bool OutputHandler::registerEncoder(audio_type_t audioType, unsigned int channels, unsigned int sampleRate)
{
	switch (audioType) {
	case AUDIO_TYPE_OPUS: {
		auto encoder = Encoder::create(audioType, channels, sampleRate);
		if (!encoder) {
			meddbg("%s[line : %d] Fail : Encoder::create failed\n", __func__, __LINE__);
			return false;
		}
		mEncoder = encoder;
		return true;
	}
	default:
		/* Don't set any encoder for unsupported formats */
		return true;
	}
}

void OutputHandler::unregisterEncoder()
{
	mEncoder = nullptr;
}

} // namespace stream
} // namespace media
