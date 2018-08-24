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
#include <assert.h>
#include <debug.h>
#include <unistd.h>
#include <media/OutputDataSource.h>
#include "Encoder.h"
#include "MediaRecorderImpl.h"
#include "StreamBuffer.h"
#include "StreamBufferReader.h"
#include "StreamBufferWriter.h"

#ifndef CONFIG_OUTPUT_DATASOURCE_STACKSIZE
#define CONFIG_OUTPUT_DATASOURCE_STACKSIZE 4096
#endif

namespace media {
namespace stream {
OutputDataSource::OutputDataSource()
	: DataSource()
	, mAudioType(AUDIO_TYPE_INVALID)
	, mEncoder(nullptr)
	, mIsFlushing(false)
	, mIsWorkerAlive(false)
{
}

OutputDataSource::OutputDataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat)
	: DataSource(channels, sampleRate, pcmFormat)
	, mAudioType(AUDIO_TYPE_INVALID)
	, mEncoder(nullptr)
	, mIsFlushing(false)
	, mIsWorkerAlive(false)
{
}

OutputDataSource::OutputDataSource(const OutputDataSource& source)
	: DataSource(source), mAudioType(source.mAudioType), mEncoder(source.mEncoder)
{
}

OutputDataSource& OutputDataSource::operator=(const OutputDataSource& source)
{
	DataSource::operator=(source);
	return *this;
}

OutputDataSource::~OutputDataSource()
{
}

void OutputDataSource::setEncoder(std::shared_ptr<Encoder> encoder)
{
	mEncoder = encoder;
}

const std::shared_ptr<Encoder> OutputDataSource::getEncoder()
{
	return mEncoder;
}

void OutputDataSource::setAudioType(audio_type_t audioType)
{
	mAudioType = audioType;
}

audio_type_t OutputDataSource::getAudioType()
{
	return mAudioType;
}

void OutputDataSource::setStreamBuffer(std::shared_ptr<StreamBuffer> streamBuffer)
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

ssize_t OutputDataSource::writeToStreamBuffer(unsigned char *buf, size_t size)
{
	medvdbg("size %u\n", size);
	size_t written = 0;
	if (mBufferWriter) {
		written = mBufferWriter->write(buf, size);
	}

	return (ssize_t) written;
}

ssize_t OutputDataSource::write(unsigned char *buf, size_t size)
{
	medvdbg("OutputDataSource::write(), size %u\n", size);

	if (!isPrepare()) {
		meddbg("source isn't prepared, hence return EOF\n");
		return (ssize_t)EOF;
	}

	if (!buf) {
		meddbg("buf is nullptr, hence return EOF\n");
		return (ssize_t)EOF;
	}

	std::shared_ptr<Encoder> encoder = getEncoder();

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

	medvdbg("OutputDataSource::write(), written %u\n", size);
	return (ssize_t) wlen;
}

bool OutputDataSource::start()
{
	medvdbg("OutputDataSource::start()\n");
	if (!isPrepare()) {
		return false;
	}

	createWorker();
	return true;
}

bool OutputDataSource::stop()
{
	medvdbg("OutputDataSource::stop()\n");
	flush();

	destoryWorker();
	return true;
}

void OutputDataSource::flush()
{
	medvdbg("OutputDataSource::flush() enter\n");
	mIsFlushing = true;

	// Worker may be sleeping, wake it up!
	wakenWorker();

	while (mIsWorkerAlive) {
		if (mBufferReader->sizeOfData() == 0) {
			// Flushed
			break;
		}

		usleep(20*1000);
	}

	mIsFlushing = false;
	medvdbg("OutputDataSource::flush() exit\n");
}

void *OutputDataSource::workerMain(void *arg)
{
	medvdbg("OutputDataSource::workerMain()\n");
	auto worker = static_cast<OutputDataSource *>(arg);

	while (worker->mIsWorkerAlive) {
		// Waken up by a writing/flushing/stopping operation
		worker->sleepWorker();

		// Worker may be stoped
		if (!worker->mIsWorkerAlive) {
			break;
		}

		if ((worker->mBufferReader->sizeOfData() >= worker->mStreamBuffer->getThreshold()) ||
			(worker->mBufferReader->sizeOfData() > 0 && worker->mIsFlushing)) {
			if (worker->onStreamBufferReadable(worker->mIsFlushing) < 0) {
				// Error occurred, stop outputting
				worker->mBufferWriter->setEndOfStream();
				break;
			}
		}
	}

	return NULL;
}

void OutputDataSource::createWorker()
{
	medvdbg("OutputDataSource::createWorker()\n");
	if (mStreamBuffer && !mIsWorkerAlive) {
		mStreamBuffer->reset();
		mIsFlushing = false;
		mIsWorkerAlive = true;

		long stackSize = CONFIG_OUTPUT_DATASOURCE_STACKSIZE;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, stackSize);
		int ret = pthread_create(&mWorker, &attr, static_cast<pthread_startroutine_t>(OutputDataSource::workerMain), this);
		if (ret != OK) {
			meddbg("Fail to create DataSourceWorker thread, return value : %d\n", ret);
			mIsWorkerAlive = false;
			return;
		}
		pthread_setname_np(mWorker, "OutputDataSourceWorker");
	}
}

void OutputDataSource::destoryWorker()
{
	medvdbg("OutputDataSource::destoryWorker()\n");
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

void OutputDataSource::sleepWorker()
{
	size_t spaces = mBufferWriter->sizeOfSpace();
	std::unique_lock<std::mutex> lock(mMutex);
	// In case of flushing or overrun, DO NOT sleep worker.
	if (mIsWorkerAlive && !mIsFlushing && (spaces > 0)) {
		mCondv.wait(lock);
	}
}

void OutputDataSource::wakenWorker()
{
	std::lock_guard<std::mutex> lock(mMutex);
	mCondv.notify_one();
}

void OutputDataSource::onBufferOverrun()
{
	auto mr = getRecorder();
	if (mr) {
		mr->notifyObserver(OBSERVER_COMMAND_BUFFER_OVERRUN);
	}
}

void OutputDataSource::onBufferUnderrun()
{
	auto mr = getRecorder();
	if (mr) {
		mr->notifyObserver(OBSERVER_COMMAND_BUFFER_UNDERRUN);
	}
}

void OutputDataSource::onBufferUpdated(ssize_t change, size_t current)
{
	medvdbg("OutputDataSource::onBufferUpdated(%d, %u)\n", change, current);
	if (change > 0) {
		// Writing wake worker up
		wakenWorker();
	}
}

} // namespace stream
} // namespace media
