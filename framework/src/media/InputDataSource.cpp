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
#include <media/InputDataSource.h>
#include "Decoder.h"
#include "MediaPlayerImpl.h"
#include "StreamBuffer.h"
#include "StreamBufferReader.h"
#include "StreamBufferWriter.h"

#ifndef CONFIG_INPUT_DATASOURCE_STACKSIZE
#define CONFIG_INPUT_DATASOURCE_STACKSIZE 4096
#endif

namespace media {
namespace stream {

InputDataSource::InputDataSource()
	: DataSource(), mAudioType(AUDIO_TYPE_INVALID), mDecoder(nullptr), mState(BUFFER_STATE_EMPTY), mTotalBytes(0), mIsWorkerAlive(false)
{
}

InputDataSource::InputDataSource(const InputDataSource &source) : DataSource(source), mAudioType(source.mAudioType)
{
}

InputDataSource &InputDataSource::operator=(const InputDataSource &source)
{
	DataSource::operator=(source);
	return *this;
}

InputDataSource::~InputDataSource()
{
}

void InputDataSource::registerDecoder(audio_type_t audioType, unsigned int channels, unsigned int sampleRate)
{
	std::shared_ptr<Decoder> decoder = nullptr;
	switch (audioType) {
	case AUDIO_TYPE_MP3:
	case AUDIO_TYPE_AAC:
	case AUDIO_TYPE_OPUS:
		decoder = std::make_shared<Decoder>(channels, sampleRate);
		break;
	case AUDIO_TYPE_FLAC:
		/* To be supported */
		meddbg("AUDIO_TYPE_FLAC is not supported now\n");
		break;
	case AUDIO_TYPE_PCM:
		medvdbg("AUDIO_TYPE_PCM does not need the decoder\n");
		break;
	default:
		/* Don't set any decoder for unsupported formats */
		meddbg("Decoder is not set\n");
		break;
	}

	mDecoder = decoder;
}

void InputDataSource::unregisterDecoder()
{
	mDecoder = nullptr;
}

void InputDataSource::setAudioType(audio_type_t audioType)
{
	mAudioType = audioType;
}

audio_type_t InputDataSource::getAudioType()
{
	return mAudioType;
}

size_t InputDataSource::getDecodeFrames(unsigned char *buf, size_t *size)
{
	unsigned int sampleRate = 0;
	unsigned short channels = 0;

	if (mDecoder->getFrame(buf, size, &sampleRate, &channels)) {
		/* TODO set configuration should be removed when we finish implement header parser */
		setSampleRate(sampleRate);
		setChannels(channels);
		medvdbg("size : %u samplerate : %d channels : %d\n", size, sampleRate, channels);
		return *size;
	}

	return 0;
}

int InputDataSource::readAt(long offset, int origin, unsigned char *buf, size_t size)
{
	stop(); // Stop before seeking, then restart in read()

	if (seek(offset, origin) != 0) {
		meddbg("InputDataSource::readAt : fail to seek\n");
		return -1;
	}

	return read(buf, size);
}

void InputDataSource::setStreamBuffer(std::shared_ptr<StreamBuffer> streamBuffer)
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

ssize_t InputDataSource::writeToStreamBuffer(unsigned char *buf, size_t size)
{
	assert(buf != NULL);

	size_t written = 0;

	if (mDecoder) {
		size_t push = 0;
		while (push < size) {
			size_t temp = mDecoder->pushData(buf + push, size - push);
			if (!temp) {
				meddbg("decode push data failed!\n");
				return EOF;
			}
			push += temp;

			while (1) {
				// Reuse free space: buf[0~push)
				size_t pcmlen = push & ~0x1;
				if (!getDecodeFrames(buf, &pcmlen)) {
					// Normal case: break and push more data...
					break;
				}
				// Write PCM data to input stream buffer.
				written += mBufferWriter->write(buf, pcmlen);
			}
		}
	} else {
		written = mBufferWriter->write(buf, size);
	}

	return (ssize_t) written;
}

ssize_t InputDataSource::read(unsigned char *buf, size_t size)
{
	size_t rlen = 0;

	start(); // Auto start

	if (mBufferReader) {
		rlen = mBufferReader->read(buf, size);
	}

	return (ssize_t) rlen;
}

bool InputDataSource::start()
{
	if (!isPrepare()) {
		return false;
	}

	createWorker();
	return true;
}

bool InputDataSource::stop()
{
	destoryWorker();
	return true;
}

void *InputDataSource::workerMain(void *arg)
{
	auto worker = static_cast<InputDataSource *>(arg);

	while (worker->mIsWorkerAlive) {
		// Waken up by a reading/stopping operation
		worker->sleepWorker();

		// Worker may be stoped
		if (!worker->mIsWorkerAlive) {
			break;
		}

		if (worker->mBufferWriter->sizeOfSpace() > 0) {
			if (worker->onStreamBufferWritable() <= 0) {
				// Error occurred, or inputting finished
				worker->mBufferWriter->setEndOfStream();
				break;
			}
		}
	}

	return NULL;
}

void InputDataSource::createWorker()
{
	medvdbg("InputDataSource::createWorker()\n");
	if (mStreamBuffer && !mIsWorkerAlive) {
		mStreamBuffer->reset();
		mState = BUFFER_STATE_EMPTY;
		mTotalBytes = 0;
		mIsWorkerAlive = true;

		long stackSize = CONFIG_INPUT_DATASOURCE_STACKSIZE;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, stackSize);
		int ret = pthread_create(&mWorker, &attr, static_cast<pthread_startroutine_t>(InputDataSource::workerMain), this);
		if (ret != OK) {
			meddbg("Fail to create DataSourceWorker thread, return value : %d\n", ret);
			mIsWorkerAlive = false;
			return;
		}
		pthread_setname_np(mWorker, "InputDataSourceWorker");
	}
}

void InputDataSource::destoryWorker()
{
	medvdbg("InputDataSource::destoryWorker()\n");
	if (mIsWorkerAlive) {
		// Setup flag,
		mIsWorkerAlive = false;

		// Worker may be blocked in buffer writing.
		mBufferWriter->setEndOfStream();

		// Wake worker up,
		wakenWorker();

		// Join thread.
		pthread_join(mWorker, NULL);
	}
}

void InputDataSource::sleepWorker()
{
	bool bEOS = mBufferReader->isEndOfStream();
	size_t spaces = mBufferWriter->sizeOfSpace();

	std::unique_lock<std::mutex> lock(mMutex);
	// In case of EOS or overrun, sleep worker.
	if (mIsWorkerAlive && (bEOS || (spaces == 0))) {
		mCondv.wait(lock);
	}
}

void InputDataSource::wakenWorker()
{
	std::lock_guard<std::mutex> lock(mMutex);
	mCondv.notify_one();
}

void InputDataSource::setBufferState(buffer_state_t state)
{
	if (mState != state) {
		mState = state;
		auto mp = getPlayer();
		if (mp) {
			mp->notifyObserver(PLAYER_OBSERVER_COMMAND_BUFFER_STATECHANGED, (int)state);
		}
	}
}

void InputDataSource::onBufferOverrun()
{
	auto mp = getPlayer();
	if (mp) {
		mp->notifyObserver(PLAYER_OBSERVER_COMMAND_BUFFER_OVERRUN);
	}
}

void InputDataSource::onBufferUnderrun()
{
	auto mp = getPlayer();
	if (mp) {
		mp->notifyObserver(PLAYER_OBSERVER_COMMAND_BUFFER_UNDERRUN);
	}
}

void InputDataSource::onBufferUpdated(ssize_t change, size_t current)
{
	if (change < 0) {
		// Reading wake worker up
		wakenWorker();
	}

	if (current == 0) {
		setBufferState(BUFFER_STATE_EMPTY);
	} else if (current == mStreamBuffer->getBufferSize()) {
		setBufferState(BUFFER_STATE_FULL);
	} else if (current >= mStreamBuffer->getThreshold()) {
		setBufferState(BUFFER_STATE_BUFFERED);
	} else {
		setBufferState(BUFFER_STATE_BUFFERING);
	}

	if (change > 0) {
		mTotalBytes += change;
		if (mTotalBytes > INT_MAX) {
			mTotalBytes = 0;
			meddbg("Too huge value: %u, set 0 to prevent overflow\n", mTotalBytes);
		}

		auto mp = getPlayer();
		if (mp) {
			mp->notifyObserver(PLAYER_OBSERVER_COMMAND_BUFFER_UPDATED, mTotalBytes);
		}
	}
}

} // namespace stream
} // namespace media
