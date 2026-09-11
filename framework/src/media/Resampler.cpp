/* ****************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

#include "Resampler.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <new>

#include <resample/speex_resampler.h>

#include "utils/remix.h"

namespace media {
namespace stream {

namespace {

constexpr unsigned int DEFAULT_RESAMPLING_QUALITY = 5;
constexpr unsigned int MAX_RESAMPLING_QUALITY = 10;

} // namespace

Resampler::Resampler() :
	mSpeexResampler(nullptr),
	mRechannelBuffer(nullptr),
	mResampleBuffer(nullptr),
	mInputChannels(0),
	mInputSampleRate(0),
	mInputFormat(0),
	mOutputChannels(0),
	mOutputSampleRate(0),
	mOutputFormat(0),
	mOutputFramesPerProcess(0),
	mInputFramesPerProcess(0),
	mRechannelBufferSize(0),
	mResampleBufferSize(0),
	mInputBytes(0),
	mOutputFrames(0),
	mState(State::NONE),
	mNeedsRechannel(false),
	mNeedsResampling(false)
{
}

Resampler::~Resampler()
{
	release();
}

void Resampler::release()
{
	if (mSpeexResampler) {
		speex_resampler_destroy(mSpeexResampler);
		mSpeexResampler = nullptr;
	}

	mRechannelBuffer.reset();
	mResampleBuffer.reset();

	mInputChannels = 0;
	mInputSampleRate = 0;
	mInputFormat = 0;
	mOutputChannels = 0;
	mOutputSampleRate = 0;
	mOutputFormat = 0;
	mOutputFramesPerProcess = 0;
	mInputFramesPerProcess = 0;
	mRechannelBufferSize = 0;
	mResampleBufferSize = 0;
	mInputBytes = 0;
	mOutputFrames = 0;
	mState = State::NONE;
	mNeedsRechannel = false;
	mNeedsResampling = false;
}

bool Resampler::configure(unsigned int inputChannels, unsigned int inputSampleRate, unsigned int inputFormat,
							unsigned int outputChannels, unsigned int outputSampleRate, unsigned int outputFormat,
							size_t outputPeriodBytes)
{
	release();

	if (inputChannels == 0 || inputSampleRate == 0 || inputFormat == 0 ||
		outputChannels == 0 || outputSampleRate == 0 || outputFormat == 0 ||
		outputPeriodBytes == 0) {
		meddbg("Invalid resampler configuration\n");
		return false;
	}
	/*
	 * rechannel() currently supports only mono or stereo output.
	 */
	if (outputChannels > 2) {
		meddbg("Unsupported output channel count: %u\n", outputChannels);
		return false;
	}

	/*
	 * Currently both input and output formats support only 2 bytes per sample (signed 16-bit PCM).
	 */
	if (inputFormat != 2 || outputFormat != 2) {
		meddbg("Unsupported PCM formats: input = %u output = %u\n", inputFormat, outputFormat);
		return false;
	}

	if (outputPeriodBytes % (outputChannels * outputFormat) != 0) {
		meddbg("Output period is not frame aligned: %lu\n", static_cast<unsigned long>(outputPeriodBytes));
		return false;
	}

	mInputChannels = inputChannels;
	mInputSampleRate = inputSampleRate;
	mInputFormat = inputFormat;
	mOutputChannels = outputChannels;
	mOutputSampleRate = outputSampleRate;
	mOutputFormat = outputFormat;

	mOutputFramesPerProcess = outputPeriodBytes / (outputChannels * outputFormat);
	mInputFramesPerProcess = static_cast<size_t>((static_cast<uint64_t>(mOutputFramesPerProcess) * mInputSampleRate) / mOutputSampleRate);
	if (mInputFramesPerProcess == 0) {
		meddbg("Output period is too small for the sample-rate ratio\n");
		release();
		return false;
	}

	mNeedsRechannel = inputChannels != outputChannels;
	mNeedsResampling = inputSampleRate != outputSampleRate;

	size_t inputBytesPerProcess = mInputFramesPerProcess * mInputChannels * mInputFormat;
	size_t rechannelBytesPerProcess = mInputFramesPerProcess * mOutputChannels * mOutputFormat;
	mRechannelBufferSize = std::max(inputBytesPerProcess, rechannelBytesPerProcess);
	mRechannelBuffer = std::make_unique<unsigned char[]>(mRechannelBufferSize);
	if (!mRechannelBuffer) {
		meddbg("Rechannel buffer allocation failed: %lu bytes\n", static_cast<unsigned long>(mRechannelBufferSize));
		release();
		return false;
	}

	if (mNeedsResampling) {
		int errCode = RESAMPLER_ERR_SUCCESS;
		unsigned int resamplingQuality = DEFAULT_RESAMPLING_QUALITY;

		if ((outputSampleRate >= inputSampleRate && outputSampleRate % inputSampleRate == 0) ||
			(inputSampleRate > outputSampleRate && inputSampleRate % outputSampleRate == 0)) {
			resamplingQuality = MAX_RESAMPLING_QUALITY;
		}

		mSpeexResampler = speex_resampler_init(outputChannels, inputSampleRate, outputSampleRate, resamplingQuality, &errCode);
		if (!mSpeexResampler) {
			meddbg("speex_resampler_init failed. errno: %d\n", errCode);
			release();
			return false;
		}

		mResampleBufferSize = outputPeriodBytes;
		mResampleBuffer = std::make_unique<unsigned char[]>(mResampleBufferSize);
		if (!mResampleBuffer) {
			meddbg("Resample buffer allocation failed: %lu bytes\n", static_cast<unsigned long>(mResampleBufferSize));
			release();
			return false;
		}
	}

	mState = State::IDLE;
	reset();

	medvdbg("Resampler configured: %u/%u -> %u/%u, input frames = %lu, output frames = %lu frames\n",
			mInputChannels, mInputSampleRate, mOutputChannels, mOutputSampleRate, static_cast<unsigned long>(mInputFramesPerProcess), static_cast<unsigned long>(mOutputFramesPerProcess));

	return true;
}

void Resampler::reset()
{
	if (mState == State::NONE) {
		return;
	}

	if (mSpeexResampler) {
		const int error = speex_resampler_reset_mem(mSpeexResampler);
		if (error != RESAMPLER_ERR_SUCCESS) {
			meddbg("speex_resampler_reset_mem failed: %d\n", error);
		}
	}

	std::memset(mRechannelBuffer.get(), 0, mRechannelBufferSize);
	if (mResampleBuffer) {
		std::memset(mResampleBuffer.get(), 0, mResampleBufferSize);
	}

	mInputBytes = 0;
	mOutputFrames = 0;
	mState = State::IDLE;
}

size_t Resampler::getRemainingInputBytes() const
{
	if (mState != State::IDLE && mState != State::BUFFERING) {
		meddbg("Invalid resampler state. state: %d, expected state: IDLE or BUFFERING\n", static_cast<int>(mState));
		return 0;
	}

	const size_t inputBytesPerProcess = mInputFramesPerProcess * mInputChannels * mInputFormat;
	const size_t pendingInputBytes = inputBytesPerProcess - mInputBytes;

	return pendingInputBytes;
}

size_t Resampler::pushData(const unsigned char *data, size_t size)
{
	if (!data || size == 0) {
		meddbg("Invalid input parameter\n");
		return 0;
	}

	if (mState != State::IDLE && mState != State::BUFFERING) {
		meddbg("Invalid resampler state. state: %d, expected state: IDLE or BUFFERING\n", static_cast<int>(mState));
		return 0;
	}

	size_t pendingInputBytes = getRemainingInputBytes();
	const size_t bytesToCopy = std::min(size, pendingInputBytes);

	std::memcpy(mRechannelBuffer.get() + mInputBytes, data, bytesToCopy);
	mInputBytes += bytesToCopy;
	pendingInputBytes -= bytesToCopy;

	mState = pendingInputBytes == 0 ? State::BUFFERED : State::BUFFERING;

	return bytesToCopy;
}

bool Resampler::rechannelData(size_t inputFrames)
{
	if (!mNeedsRechannel) {
		medvdbg("No need for rechanneling\n");
		return true;
	}

	const int32_t rechanneledFrames = rechannel(ch2layout(mInputChannels), ch2layout(mOutputChannels),
												reinterpret_cast<const int16_t *>(mRechannelBuffer.get()), static_cast<uint32_t>(inputFrames),
												reinterpret_cast<int16_t *>(mRechannelBuffer.get()), static_cast<uint32_t>(mRechannelBufferSize / (mOutputChannels * mOutputFormat)));
	if (rechanneledFrames < 0 || static_cast<size_t>(rechanneledFrames) != inputFrames) {
		meddbg("Fail to rechannel each frame, %d/%lu\n", rechanneledFrames, static_cast<unsigned long>(inputFrames));
		return false;
	}

	return true;
}

bool Resampler::resample(size_t frames)
{
	if (!mNeedsResampling) {
		medvdbg("No need for resampling\n");
		mOutputFrames = frames;
		return true;
	}

	size_t usedFrames = 0;
	size_t resampledFrames = 0;
	spx_int16_t *dataIn;
	spx_uint32_t inputFrames;
	spx_int16_t *dataOut;
	spx_uint32_t outputFrames;
	int ret;

	while (frames > usedFrames) {
		dataIn = reinterpret_cast<int16_t *>(mRechannelBuffer.get() + (usedFrames * mOutputChannels * mOutputFormat));
		inputFrames = static_cast<spx_uint32_t>(frames - usedFrames);
		dataOut = reinterpret_cast<int16_t *>(mResampleBuffer.get() + (resampledFrames * mOutputChannels * mOutputFormat));
		outputFrames = static_cast<spx_uint32_t>(mOutputFramesPerProcess - resampledFrames);
		medvdbg("dataIn %p, inputFrames %u\n", static_cast<void *>(dataIn), inputFrames);
		medvdbg("dataOut %p, outputFrames resample buffer can hold %u\n", static_cast<void *>(dataOut), outputFrames);

		ret = speex_resampler_process_interleaved_int(mSpeexResampler, dataIn, &inputFrames, dataOut, &outputFrames);
		if (ret != RESAMPLER_ERR_SUCCESS) {
			meddbg("Fail to resample out:%lu/%lu, error %d\n", static_cast<unsigned long>(usedFrames), static_cast<unsigned long>(frames), ret);
			return false;
		}

		usedFrames += inputFrames;
		if (outputFrames > 0) {
			resampledFrames += outputFrames;
		} else if (frames != usedFrames) {
			meddbg("Error: output buffer is full, used input frames %lu/%lu\n", static_cast<unsigned long>(usedFrames), static_cast<unsigned long>(frames));
			return false;
		}
		medvdbg("%lu frames generated from %lu/%lu\n", static_cast<unsigned long>(resampledFrames), static_cast<unsigned long>(usedFrames), static_cast<unsigned long>(frames));
	}

	medvdbg("resampled frames count: %lu\n", static_cast<unsigned long>(resampledFrames));
	mOutputFrames = resampledFrames;
	return true;
}

Resampler::Result Resampler::processInput()
{
	const size_t inputBytesPerFrame = mInputChannels * mInputFormat;
	const size_t trailingBytes = mInputBytes % inputBytesPerFrame;
	if (trailingBytes != 0) {
		meddbg("Discarding %lu incomplete PCM bytes at EOS\n", static_cast<unsigned long>(trailingBytes));
		mInputBytes -= trailingBytes;
	}

	if (mInputBytes == 0) {
		mState = State::IDLE;
		return Result::NEED_INPUT;
	}

	const size_t inputFrames = mInputBytes / inputBytesPerFrame;

	if (!rechannelData(inputFrames)) {
		meddbg("Rechanneling failed\n");
		return Result::ERROR;
	}

	if (!resample(inputFrames)) {
		meddbg("Resampling failed\n");
		return Result::ERROR;
	}

	if (mOutputFrames == 0) {
		mInputBytes = 0;
		mState = State::IDLE;
		return Result::NEED_INPUT;
	}

	mState = State::PROCESSED;
	return Result::OUTPUT_READY;
}

Resampler::Result Resampler::process()
{
	if (mState != State::BUFFERED) {
		meddbg("Invalid resampler state. state: %d, expected state: BUFFERED\n", static_cast<int>(mState));
		return Result::ERROR;
	}

	return processInput();
}

Resampler::Result Resampler::drain()
{
	if (mState == State::IDLE) {
		return Result::DRAINED;
	}

	if (mState == State::PROCESSED) {
		return Result::OUTPUT_READY;
	}

	if (mState != State::BUFFERING && mState != State::BUFFERED) {
		return Result::ERROR;
	}

	Result ret = processInput();
	
	// If processInput couldn't generate output from the residual data,
	// then the resampler is successfully drained.
	if (ret == Result::NEED_INPUT) {
		return Result::DRAINED;
	}

	// Otherwise, return OUTPUT_READY or ERROR
	return ret;
}

Resampler::ConstBufferView Resampler::getOutput() const
{
	ConstBufferView result = { nullptr, 0 };

	if (mState != State::PROCESSED) {
		meddbg("Invalid resampler state. state: %d, expected state: PROCESSED\n", static_cast<int>(mState));
		return result;
	}

	result.data = mNeedsResampling ? mResampleBuffer.get() : mRechannelBuffer.get();
	result.size = mOutputFrames * mOutputChannels * mOutputFormat;

	return result;
}

bool Resampler::releaseOutput()
{
	if (mState != State::PROCESSED) {
		meddbg("Invalid resampler state. state: %d, expected state: PROCESSED\n", static_cast<int>(mState));
		return false;
	}

	mInputBytes = 0;
	mOutputFrames = 0;
	mState = State::IDLE;

	return true;
}

} // namespace stream
} // namespace media
