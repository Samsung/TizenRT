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

namespace media {
namespace stream {

InputHandler::InputHandler() :
	mDecoder(nullptr),
	mState(BUFFER_STATE_EMPTY),
	mTotalBytes(0)
{
	mWorkerStackSize = CONFIG_INPUT_DATASOURCE_STACKSIZE;
}

void InputHandler::setInputDataSource(std::shared_ptr<InputDataSource> source)
{
	if (source == nullptr) {
		meddbg("source is nullptr\n");
		return;
	}
	StreamHandler::setDataSource(source);
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

ssize_t InputHandler::read(unsigned char *buf, size_t size)
{
	size_t rlen = 0;

	start(); // Auto start

	if (mBufferReader) {
		rlen = mBufferReader->read(buf, size);
	}

	return (ssize_t)rlen;
}

void InputHandler::resetWorker()
{
	mState = BUFFER_STATE_EMPTY;
	mTotalBytes = 0;
}

bool InputHandler::processWorker()
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

		writeToStreamBuffer(buf, (size_t)readLen);
		delete[] buf;
	}

	return true;
}

void InputHandler::sleepWorker()
{
	bool bEOS = mBufferReader->isEndOfStream();
	size_t spaces = mBufferWriter->sizeOfSpace();

	/* In case of EOS or overrun, sleep worker. */
	if (bEOS || (spaces == 0)) {
		StreamHandler::sleepWorker();
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
	if (mDemuxer) {
		mDemuxer->pushData(buf, size);
		ssize_t ret  = mDemuxer->pullData(buf, size);
		if (ret < 0) {
			if (ret == DEMUXER_ERROR_WANT_DATA) {
				medvdbg("demuxer want more data!\n");
				return (ssize_t)size;
			}
			medwdbg("pull ES data failed! error: %d\n", ret);
			return EOF;
		}
		// size of elementary stream data got from demuxer
		size = (size_t)ret;
	}

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

	return (ssize_t)written;
}

bool InputHandler::registerCodec(audio_type_t audioType, unsigned int channels, unsigned int sampleRate)
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
		getDataSource()->setChannels(channels);
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
		return registerCodec(mDemuxer->getAudioType(), channels, sampleRate);
	}
	case AUDIO_TYPE_FLAC:
		/* To be supported */
	default:
		meddbg("%s[line : %d] Fail : type %d is not supported\n", __func__, __LINE__, audioType);
		return false;
	}
}

void InputHandler::unregisterCodec()
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

} // namespace stream
} // namespace media
