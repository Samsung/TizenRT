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

#include <debug.h>
#include <pthread.h>

#include "InputHandler.h"
#include "MediaPlayerImpl.h"
#include "Decoder.h"
#include "Demuxer.h"

#ifndef CONFIG_HANDLER_STREAM_BUFFER_SIZE
#define CONFIG_HANDLER_STREAM_BUFFER_SIZE 4096
#endif

#ifndef CONFIG_HANDLER_STREAM_BUFFER_THRESHOLD
#define CONFIG_HANDLER_STREAM_BUFFER_THRESHOLD 2048
#endif

namespace media {
namespace stream {

InputHandler::InputHandler() :
	mInputDataSource(nullptr),
	mDecoder(nullptr),
	mWorker(0),
	mIsWorkerAlive(false),
	mState(BUFFER_STATE_EMPTY),
	mTotalBytes(0)
{
}

void InputHandler::setInputDataSource(std::shared_ptr<InputDataSource> source)
{
	mInputDataSource = source;
}

bool InputHandler::doStandBy()
{
	auto mp = getPlayer();
	if (!mp) {
		meddbg("get player handle failed!\n");
		return false;
	}

	std::thread wk = std::thread([=]() {
		medvdbg("InputHandler::doStandBy thread enter\n");
		player_event_t event;
		if (mInputDataSource->open()) {
			event = PLAYER_EVENT_SOURCE_PREPARED;
		} else {
			event = PLAYER_EVENT_SOURCE_OPEN_FAILED;
		}
		mp->notifyAsync(event);
		medvdbg("InputHandler::doStandBy thread exit\n");
	});

	wk.detach();
	return true;
}

bool InputHandler::open()
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

	if (mInputDataSource->open()) {
		/* Media f/w playback supports only mono and stereo.
		 * In case of multiple channel audio, we ask decoder always outputting stereo PCM data.
		 */
		if (mInputDataSource->getChannels() > 2) {
			medvdbg("Set multiple channel %u to stereo forcely!\n", mInputDataSource->getChannels());
			mInputDataSource->setChannels(2);
		}

		if (registerDecoder(mInputDataSource->getAudioType(), mInputDataSource->getChannels(), mInputDataSource->getSampleRate())) {
			start();
			return true;
		}
	}

	return false;
}

bool InputHandler::close()
{
	stop();
	unregisterDecoder();
	return mInputDataSource->close();
}

ssize_t InputHandler::read(unsigned char *buf, size_t size)
{
	size_t rlen = 0;

	start(); // Auto start

	if (mBufferReader) {
		rlen = mBufferReader->read(buf, size);
	}

	return (ssize_t)rlen;
}

bool InputHandler::start()
{
	if (!mInputDataSource->isPrepared()) {
		return false;
	}

	createWorker();
	return true;
}

bool InputHandler::stop()
{
	size_t size = getAvailSpace();
	if (size > 0) {
		auto buf = new unsigned char[size];
		if (!buf) {
			meddbg("run out of memory! size: 0x%x\n", size);
			return false;
		}

		ssize_t readLen = mInputDataSource->read(buf, size);
		if (readLen <= 0) {
			// Error occurred, or inputting finished
			mBufferWriter->setEndOfStream();
			delete[] buf;
			return false;
		}

		ssize_t writeLen = writeToStreamBuffer(buf, (size_t)readLen);
		delete[] buf;
		if (writeLen <= 0) {
			meddbg("write to stream buffer failed!\n");
			mBufferWriter->setEndOfStream();
			return false;
		}
	}

	destroyWorker();
	return true;
}

void InputHandler::createWorker()
{
	medvdbg("InputHandler::createWorker()\n");
	if (mStreamBuffer && !mIsWorkerAlive) {
		mStreamBuffer->reset();
		mState = BUFFER_STATE_EMPTY;
		mTotalBytes = 0;
		mIsWorkerAlive = true;

		long stackSize = CONFIG_INPUT_DATASOURCE_STACKSIZE;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, stackSize);
		int ret = pthread_create(&mWorker, &attr, static_cast<pthread_startroutine_t>(InputHandler::workerMain), this);
		if (ret != OK) {
			meddbg("Fail to create DataSourceWorker thread, return value : %d\n", ret);
			mIsWorkerAlive = false;
			return;
		}
		pthread_setname_np(mWorker, "InputHandlerWorker");
	}
}

void InputHandler::destroyWorker()
{
	medvdbg("InputHandler::destroyWorker()\n");
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

void *InputHandler::workerMain(void *arg)
{
	auto stream = static_cast<InputHandler *>(arg);
	auto worker = stream->mInputDataSource;

	while (stream->mIsWorkerAlive) {
		// Waken up by a reading/stopping operation
		stream->sleepWorker();

		// Worker may be stoped
		if (!stream->mIsWorkerAlive) {
			break;
		}

		auto size = stream->mBufferWriter->sizeOfSpace();
		if (size > 0) {
			auto buf = new unsigned char[size];
			if (worker->read(buf, size) <= 0) {
				// Error occurred, or inputting finished
				stream->mBufferWriter->setEndOfStream();
				delete[] buf;
				break;
			}

			stream->writeToStreamBuffer(buf, size);
			delete[] buf;
		}
	}

	return NULL;
}

void InputHandler::sleepWorker()
{
	bool bEOS = mBufferReader->isEndOfStream();
	size_t spaces = mBufferWriter->sizeOfSpace();

	std::unique_lock<std::mutex> lock(mMutex);
	// In case of EOS or overrun, sleep worker.
	if (mIsWorkerAlive && (bEOS || (spaces == 0))) {
		mCondv.wait(lock);
	}
}

void InputHandler::wakenWorker()
{
	std::lock_guard<std::mutex> lock(mMutex);
	mCondv.notify_one();
}

void InputHandler::setStreamBuffer(std::shared_ptr<StreamBuffer> streamBuffer)
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

void InputHandler::setBufferState(buffer_state_t state)
{
	if (mState != state) {
		mState = state;
		auto mp = getPlayer();
		if (mp) {
			mp->notifyObserver(PLAYER_OBSERVER_COMMAND_BUFFER_STATECHANGED, (int)state);
		}
	}
}

void InputHandler::onBufferOverrun()
{
	auto mp = getPlayer();
	if (mp) {
		mp->notifyObserver(PLAYER_OBSERVER_COMMAND_BUFFER_OVERRUN);
	}
}

void InputHandler::onBufferUnderrun()
{
	auto mp = getPlayer();
	if (mp) {
		mp->notifyObserver(PLAYER_OBSERVER_COMMAND_BUFFER_UNDERRUN);
	}
}

void InputHandler::onBufferUpdated(ssize_t change, size_t current)
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

size_t InputHandler::getAvailSpace()
{
	if (mDemuxer) {
		return mDemuxer->getAvailSpace();
	}

	if (mDecoder) {
		return mDecoder->getAvailSpace();
	}

	// return PCM buffer space size
	return mBufferWriter->sizeOfSpace();
}

ssize_t InputHandler::writeToStreamBuffer(unsigned char *buf, size_t size)
{
	size_t used = 0;
	while (1) {
		unsigned char *buffES = nullptr;
		size_t sizeES = mDecoder ? mDecoder->getAvailSpace() : mBufferWriter->sizeOfSpace();
		ssize_t ret = getElementaryStream(buf, size, &used, &buffES, &sizeES);
		if (ret < 0) {
			meddbg("getElementaryStream failed! error: %d\n", ret);
			return ret;
		}
		if (ret == 0) {
			// want more data
			break;
		}

		size_t usedES = 0;
		while (1) {
			unsigned char *buffPCM = buf;
			size_t sizePCM = used;
			ret = getPCM(buffES, sizeES, &usedES, &buffPCM, &sizePCM);
			if (ret < 0) {
				meddbg("getPCM failed! error: %d\n", ret);
				return ret;
			}
			if (ret == 0) {
				// want more data
				break;
			}

			// write PCM data to stream buffer
			size_t written = mBufferWriter->write(buffPCM, sizePCM);
			if (written != sizePCM) {
				meddbg("End of writting!\n");
				return EOF;
			}
		}
	}
	return size;
}

bool InputHandler::registerDecoder(audio_type_t audioType, unsigned int channels, unsigned int sampleRate)
{
	/* Media f/w playback supports only mono and stereo.
	 * In case of multiple channel audio, we ask decoder always outputting stereo PCM data.
	 */
	if (channels == 0) {
		meddbg("Channel can not be zero\n");
		return false;
	} else if (channels > 2) {
		medvdbg("Set multiple channel %u to stereo forcely!\n", channels);
		channels = 2;
		getInputDataSource()->setChannels(channels);
	}

	switch (audioType) {
	case AUDIO_TYPE_MP3:
	case AUDIO_TYPE_AAC:
	case AUDIO_TYPE_WAVE:
	case AUDIO_TYPE_OPUS: {
		auto decoder = Decoder::create(audioType, channels, sampleRate);
		if (!decoder) {
			meddbg("%s[line : %d] Fail : Decoder::create failed\n", __func__, __LINE__);
			return false;
		}
		mDecoder = decoder;
		return true;
	}
	case AUDIO_TYPE_PCM:
		medvdbg("AUDIO_TYPE_PCM does not need the decoder\n");
		return true;
	case AUDIO_TYPE_MP2T: {
		/* Register demuxer according to the given audio/container type */
		auto demuxer = Demuxer::create(audioType);
		if (!demuxer) {
			meddbg("Create demuxer failed! audioType %d\n", audioType);
			return false;
		}
		/* Prepare demuxer (probe the datasource to get audio informations) */
		do {
			size_t size = demuxer->getAvailSpace() / 4;
			unsigned char *buf = new unsigned char[size];
			if (!buf) {
				meddbg("Run out of memory! bytes: 0x%x\n", size);
				return false;
			}
			ssize_t readLen = mInputDataSource->read(buf, size);
			if (readLen <= 0) {
				meddbg("Read source failed! error: %d\n", readLen);
				delete[] buf;
				return false;
			}
			demuxer->pushData(buf, (size_t)readLen);
			delete[] buf;
			int ret = demuxer->prepare();
			if (ret < 0) {
				if (ret == DEMUXER_ERROR_WANT_DATA) {
					medvdbg("Demuxer want more data!\n");
					continue;
				}
				meddbg("Prepare demuxer failed! error: %d\n", ret);
				return false;
			}
		} while (!demuxer->isReady());
		/* Demuxer is ready (to get audio information and elementary stream). */
		mDemuxer = demuxer;
		/* Register underlying codec */
		return registerDecoder(mDemuxer->getAudioType(), channels, sampleRate);
	}
	case AUDIO_TYPE_FLAC:
		/* To be supported */
	default:
		meddbg("%s[line : %d] Fail : type %d is not supported\n", __func__, __LINE__, audioType);
		return false;
	}
}

void InputHandler::unregisterDecoder()
{
	mDecoder = nullptr;
}

size_t InputHandler::getDecodeFrames(unsigned char *buf, size_t *size)
{
	unsigned int sampleRate = 0;
	unsigned short channels = 0;

	if (mDecoder->getFrame(buf, size, &sampleRate, &channels)) {
		medvdbg("size : %u samplerate : %d channels : %d\n", *size, sampleRate, channels);
		return *size;
	}

	return 0;
}

ssize_t InputHandler::getElementaryStream(unsigned char *buf, size_t size, size_t *used, unsigned char **out, size_t *expect)
{
	if (mDemuxer) {
		ssize_t ret;
		if (*used < size) {
			ret = mDemuxer->pushData(buf + *used, size - *used);
			if (ret <= 0) {
				meddbg("push data to demuxer failed! error: %d\n", ret);
				return EOF;
			}
			*used += (size_t)ret;
		}

		if (*out == nullptr) {
			*out = buf;
			if (*expect > *used) {
				*expect = *used;
			}
		}

		ret = mDemuxer->pullData(*out, *expect);
		if (ret < 0) {
			if (ret == DEMUXER_ERROR_WANT_DATA) {
				// normal case: demuxer want more data
				return 0;
			}
			meddbg("pull data from demuxer failed! error: %d\n", ret);
			return EOF;
		}
		*expect = (size_t)ret;
	} else {
		fetchData(buf, size, used, out, expect);
	}

	return (ssize_t)(*expect);
}

ssize_t InputHandler::getPCM(unsigned char *buf, size_t size, size_t *used, unsigned char **out, size_t *expect)
{
	if (mDecoder) {
		ssize_t ret;
		if (*used < size) {
			ret = mDecoder->pushData(buf + *used, size - *used);
			if (ret <= 0) {
				meddbg("push data to decoder failed! error: %d\n", ret);
				return EOF;
			}
			*used += (size_t)ret;
		}

		if (*out == nullptr) {
			*out = buf;
			if (*expect > *used) {
				*expect = *used;
			}
		}

		*expect &= ~0x1;
		if (!getDecodeFrames(*out, expect)) {
			// normal case: decoder want more data
			return 0;
		}
	} else {
		fetchData(buf, size, used, out, expect);
	}

	return (ssize_t)(*expect);
}

size_t InputHandler::fetchData(unsigned char *buf, size_t size, size_t *used, unsigned char **out, size_t *expect)
{
	if (*used < size) {
		size_t remained = size - *used;
		if (*expect > remained) {
			*expect = remained;
		}
		if (*out == nullptr) {
			// Point to unused data in `buf`
			*out = buf + *used;
		} else {
			// Copy data to the given output buffer
			memcpy(*out, buf + *used, *expect);
		}
		*used += *expect;
	} else {
		// no more data
		*expect = 0;
	}
	return *expect;
}

} // namespace stream
} // namespace media
