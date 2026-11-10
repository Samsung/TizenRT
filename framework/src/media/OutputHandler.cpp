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

namespace media {
namespace stream {

static inline unsigned int audioFormatToBits(audio_format_type_t format) {
	switch (format) {
	case AUDIO_FORMAT_TYPE_S32_LE:
		return 32;
	case AUDIO_FORMAT_TYPE_S16_LE:
		return 16;
	case AUDIO_FORMAT_TYPE_S8:
		return 8;
	default:
		return 0;
	}
}

OutputHandler::OutputHandler() :
	mIsFlushing(false)
{
	mWorkerStackSize = CONFIG_OUTPUT_DATASOURCE_STACKSIZE;
}

void OutputHandler::setOutputDataSource(std::shared_ptr<OutputDataSource> source)
{
	if (source == nullptr) {
		meddbg("source is nullptr\n");
		return;
	}
	StreamHandler::setDataSource(source);
	mOutputDataSource = source;
}

bool OutputHandler::startBuffering(unsigned int outputSampleRate, unsigned int outputChannel, unsigned int outputBytesPerFormat, size_t outputPeriodBytes)
{
	mResampler = std::make_unique<Resampler>();
	if (!mResampler) {
		meddbg("Resampler allocation failed\n");
		return false;
	}

	if (!mResampler->configure(outputChannel, outputSampleRate, outputBytesPerFormat, mOutputDataSource->getChannels(), mOutputDataSource->getSampleRate(), 
								audioFormatToBits(mOutputDataSource->getPcmFormat()) >> 3, outputPeriodBytes)) {
		meddbg("Resampler configuration failed\n");
		mResampler.reset();
		return false;
	}

	if (!mResampleBuffer) {
		mResampleBufferSize = outputPeriodBytes;
		mResampleBuffer = std::make_unique<unsigned char[]>(mResampleBufferSize);
		if (!mResampleBuffer) {
			meddbg("ResampleBuffer allocation fail size: %d\n", mResampleBufferSize);
			mResampler.reset();
			mResampleBufferSize = 0;
			return false;
		}
	}

	bool ret = StreamHandler::start();
	if (!ret) {
		mResampler.reset();
		mResampleBuffer.reset();
		mResampleBufferSize = 0;
		return false;
	}
	
	return ret;
}

ssize_t OutputHandler::write(unsigned char *buf, size_t size)
{
	medvdbg("OutputHandler::write(), size %u\n", size);

	if (buf == nullptr) {
		meddbg("buf is nullptr, hence return EOF\n");
		return (ssize_t)EOF;
	}

	size_t wlen = 0;
	wlen += writeToStreamBuffer(buf + wlen, size - wlen);
	meddbg("written size : %d\n", wlen);

	meddbg("OutputHandler::write(), written %u\n", size);
	return (ssize_t)wlen;
}

bool OutputHandler::stop()
{
	medvdbg("OutputHandler::stop()\n");
	return StreamHandler::stop();
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

void OutputHandler::resetWorker()
{
	std::unique_lock<std::mutex> lock(mFlushMutex);
	mIsFlushing = false;
}

void OutputHandler::writeToSource(size_t size)
{
	auto buf = new unsigned char[size];
	if (!buf) {
		meddbg("Failed to allocate buffer of size %zu\n", size);
		return;
	}
	/*
	frameSize = hardwareChannels * hardwareFormat per bytes
	size -= size % frameSize 
	*/
	auto readed = mBufferReader->read(buf, size);
	if (readed != size) {
		meddbg("StreamBufferReader::read failed! size : %u, readed : %u\n", size, readed);
		delete[] buf;
		buf = nullptr;
		return;
	}

	ssize_t resampledSize = mResampler->process(buf, readed, mResampleBuffer.get(), mResampleBufferSize);
	if (resampledSize < 0) {
		meddbg("Resampler process failed!\n");
		return;
	}

	std::shared_ptr<Encoder> encoder = mEncoder;

	size_t wlen = 0;
	while (wlen < resampledSize) {
		if (encoder) {
			size_t pushed = encoder->pushData(mResampleBuffer.get() + wlen, resampledSize - wlen);
			if (pushed == 0) {
				meddbg("Can not push any data! Error occurred during encoding!\n");
				break;
			}

			wlen += pushed;

			while (1) {
				size_t ret = wlen;
				if (!encoder->getFrame(mResampleBuffer.get(), &ret)) {
					break;
				}

				auto written = mOutputDataSource->write(mResampleBuffer.get(), ret);
				medvdbg("written size: %d\n", written);
				if (written <= 0) {
					meddbg("OutputDataSource::write returned <= 0! size : %u, written : %d\n", size, written);
					mBufferWriter->setEndOfStream();
					break;
				}			
				if (written != (ssize_t)ret) {
					meddbg("Can not write all!\n");
					break;
				}
			}
		} else {
			auto written = mOutputDataSource->write(mResampleBuffer.get(), resampledSize);
			medvdbg("written size : %d\n", wlen);
			if (written <= 0) {
				meddbg("OutputDataSource::write returned <= 0! size : %u, written : %d\n", size, written);
				mBufferWriter->setEndOfStream();
			}
			break;
		}
	}

	delete[] buf;
	buf = nullptr;
}

bool OutputHandler::processWorker()
{
	auto size = this->mBufferReader->sizeOfData();
	if (this->mIsFlushing) {
		if (size != 0) {
			this->writeToSource(size);
		}
		std::unique_lock<std::mutex> lock(this->mFlushMutex);
		this->mIsFlushing = false;
		this->mFlushCondv.notify_one();
	} else if (size >= this->mStreamBuffer->getThreshold()) {
		this->writeToSource(size);
	}

	return true;
}

void OutputHandler::sleepWorker()
{
	size_t spaces = mBufferWriter->sizeOfSpace();
	if (spaces > 0 && !mIsFlushing) {
		StreamHandler::sleepWorker();
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

bool OutputHandler::registerCodec(audio_type_t audioType, unsigned int channels, unsigned int sampleRate)
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
	case AUDIO_TYPE_WAVE:
		medvdbg("AUDIO_TYPE_WAVE does not need the Encoder\n");
		return true;
	default:
		/* Don't set any encoder for unsupported formats */
		return true;
	}
}

void OutputHandler::unregisterCodec()
{
	mEncoder = nullptr;
}

bool OutputHandler::probeDataSource()
{
	// do nothing
	return true;
}

} // namespace stream
} // namespace media
