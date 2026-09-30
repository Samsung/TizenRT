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

constexpr unsigned int DEFAULT_RESAMPLING_QUALITY = 5; // Resampling quality between 0 and 10, where 0 has poor quality and 10 has very high quality.
constexpr unsigned int MAX_RESAMPLING_QUALITY = 10;

} // namespace

Resampler::Resampler() :
    mSpeexResampler(nullptr),
    mRechannelBuffer(nullptr),
    mRechannelBufferSize(0),
    mOutputFramesPerProcess(0),
    mInputFramesPerProcess(0),
    mInputBytes(0),
    mOutputFrames(0),
    mInputChannels(0),
    mInputSampleRate(0),
    mInputFormat(0),
    mOutputChannels(0),
    mOutputSampleRate(0),
    mOutputFormat(0),
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
	mRechannelBufferSize = 0;

	mOutputFramesPerProcess = 0;
	mInputFramesPerProcess = 0;
	mInputBytes = 0;
	mOutputFrames = 0;

	mInputChannels = 0;
	mInputSampleRate = 0;
	mInputFormat = 0;

	mOutputChannels = 0;
	mOutputSampleRate = 0;
	mOutputFormat = 0;

	mNeedsRechannel = false;
	mNeedsResampling = false;
}

bool Resampler::configure(unsigned int inputChannels,
                          unsigned int inputSampleRate,
                          unsigned int inputFormat,
                          unsigned int outputChannels,
                          unsigned int outputSampleRate,
                          unsigned int outputFormat,
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
		meddbg("Unsupported PCM formats: input = %u, output = %u\n", inputFormat, outputFormat);
		return false;
	}

	const size_t outputBytesPerFrame = outputChannels * outputFormat;

	if (outputPeriodBytes % outputBytesPerFrame != 0) {
		meddbg("Output period is not frame aligned\n");
		return false;
    }

	mInputChannels = inputChannels;
	mInputSampleRate = inputSampleRate;
	mInputFormat = inputFormat;
	mOutputChannels = outputChannels;
	mOutputSampleRate = outputSampleRate;
	mOutputFormat = outputFormat;

	mOutputFramesPerProcess = outputPeriodBytes / outputBytesPerFrame;
	mInputFramesPerProcess = static_cast<size_t>((static_cast<uint64_t>(mOutputFramesPerProcess) * mInputSampleRate) / mOutputSampleRate);

	if (mInputFramesPerProcess == 0) {
    	meddbg("Invalid input frame count per process\n");
    	release();
    	return false;
    }

    mNeedsRechannel = mInputChannels != mOutputChannels;
	mNeedsResampling = mInputSampleRate != mOutputSampleRate;

	mRechannelBufferSize = mInputFramesPerProcess * mOutputChannels * mOutputFormat;
	mRechannelBuffer = std::make_unique<unsigned char[]>(mRechannelBufferSize);

    if (!mRechannelBuffer) {
    	meddbg("Rechannel buffer allocation failed: %lu\n", static_cast<unsigned long>(mRechannelBufferSize));
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
			meddbg("speex_resampler_init failed. error: %d\n", errCode);
			release();
			return false;
        }
    }

	mInputBytes = 0;
	mOutputFrames = 0;

	medvdbg("Resampler configured: input %u Hz/%u ch -> output %u Hz/%u ch\n", mInputSampleRate, mInputChannels, mOutputSampleRate, mOutputChannels);
	medvdbg("Input frames/process: %lu, output frames/process: %lu\n", static_cast<unsigned long>(mInputFramesPerProcess), static_cast<unsigned long>(mOutputFramesPerProcess));

	return true;
}

size_t Resampler::getInputBytesPerProcess() const
{
	return mInputFramesPerProcess * mInputChannels * mInputFormat;
}

void Resampler::reset()
{
	if (mSpeexResampler) {
		int ret = speex_resampler_reset_mem(mSpeexResampler);
		if (ret != RESAMPLER_ERR_SUCCESS) {
			meddbg("speex_resampler_reset_mem failed: %d\n", ret);
        }
    }

	mInputBytes = 0;
	mOutputFrames = 0;

    if (mRechannelBuffer) {
		memset(mRechannelBuffer.get(), 0, mRechannelBufferSize);
    }
}

bool Resampler::rechannelData(const unsigned char *input, size_t inputFrames)
{
    if (!mNeedsRechannel) {
		return true;
    }

	const size_t outputFramesCapacity = mRechannelBufferSize / (mOutputChannels * mOutputFormat);

	const int32_t rechannelFrames = rechannel(ch2layout(mInputChannels), ch2layout(mOutputChannels),
		reinterpret_cast<const int16_t *>(input), static_cast<uint32_t>(inputFrames),
		reinterpret_cast<int16_t *>(mRechannelBuffer.get()), static_cast<uint32_t>(outputFramesCapacity));

	if (rechannelFrames < 0 || static_cast<size_t>(rechannelFrames) != inputFrames) {
		meddbg("Rechannel failed: %d/%lu\n", rechannelFrames, static_cast<unsigned long>(inputFrames));
		return false;
    }

	return true;
}

size_t Resampler::resampleData(const unsigned char *input, size_t inputFrames, unsigned char *resampleBuffer, size_t resampleBufferSize)
{
	if (!mSpeexResampler || !input || !resampleBuffer || inputFrames == 0 || resampleBufferSize == 0) {
		meddbg("Invalid resampleData parameters\n");
		return 0;
    }

	const size_t outputBytesPerFrame = mOutputChannels * mOutputFormat;
	if (outputBytesPerFrame == 0) {
		meddbg("Invalid output bytes per frame\n");
		return 0;
    }

    const size_t outputFrameCapacity = resampleBufferSize / outputBytesPerFrame;
    if (outputFrameCapacity == 0) {
		meddbg("Resample buffer is too small\n");
		return 0;
    }

	size_t usedFrames = 0;
	mOutputFrames = 0;

	while (usedFrames < inputFrames) {
		if (mOutputFrames >= outputFrameCapacity) {
			meddbg("Resample buffer is full\n");
			return 0;
		}

    	const size_t inputOffset = usedFrames * mOutputChannels * mOutputFormat;
    	const size_t outputOffset = mOutputFrames * mOutputChannels * mOutputFormat;

    	spx_int16_t *dataIn = reinterpret_cast<spx_int16_t *>(const_cast<unsigned char *>(input + inputOffset));
    	spx_int16_t *dataOut = reinterpret_cast<spx_int16_t *>(resampleBuffer + outputOffset);
    	spx_uint32_t inputFramesToProcess = static_cast<spx_uint32_t>(inputFrames - usedFrames);
    	spx_uint32_t outputFramesAvailable = static_cast<spx_uint32_t>(outputFrameCapacity - mOutputFrames);

    	int ret = speex_resampler_process_interleaved_int(mSpeexResampler, dataIn, &inputFramesToProcess, dataOut, &outputFramesAvailable);
    	if (ret != RESAMPLER_ERR_SUCCESS) {
			meddbg("speex_resampler_process_interleaved_int failed: %d\n", ret);
			mOutputFrames = 0;
			return 0;
        }

    	usedFrames += inputFramesToProcess;
    	mOutputFrames += outputFramesAvailable;

    	if (inputFramesToProcess == 0) {
    		meddbg("Resampler consumed no input\n");
			mOutputFrames = 0;
			return 0;
    	}
	}

	return mOutputFrames;
}

size_t Resampler::process(const unsigned char *buffPCM, size_t buffSize, unsigned char *resampleBuffer, size_t resampleBufferSize)
{
    if (!buffPCM || !resampleBuffer || buffSize == 0 || resampleBufferSize == 0) {
    	meddbg("Invalid process parameters\n");
		return 0;
    }

	const size_t inputBytesPerFrame = mInputChannels * mInputFormat;
	const size_t outputBytesPerFrame = mOutputChannels * mOutputFormat;

	if (inputBytesPerFrame == 0 || outputBytesPerFrame == 0) {
		meddbg("Invalid frame size\n");
		return 0;
	}
	if (buffSize % inputBytesPerFrame != 0) {
		meddbg("Input PCM is not frame aligned\n");
		return 0;
	}

	const size_t inputFrames = buffSize / inputBytesPerFrame;
	if (inputFrames == 0) {
		return 0;
	}

	if (inputFrames > mInputFramesPerProcess) {
		meddbg("Input frames exceed process capacity: %lu/%lu\n", static_cast<unsigned long>(inputFrames), static_cast<unsigned long>(mInputFramesPerProcess));
		return 0;
	}

	mInputBytes = buffSize;
	mOutputFrames = 0;

	const unsigned char *resampleInput = buffPCM;

	if (mNeedsRechannel) {
        if (!rechannelData(buffPCM, inputFrames)) {
			meddbg("Rechanneling failed\n");
			return 0;
		}

		resampleInput = mRechannelBuffer.get();
	}

	if (!mNeedsResampling) {
		const size_t outputBytes = inputFrames * outputBytesPerFrame;

		if (outputBytes > resampleBufferSize) {
			meddbg("Output buffer is too small: %lu/%lu\n", static_cast<unsigned long>(outputBytes), static_cast<unsigned long>(resampleBufferSize));
			return 0;
		}

		memcpy(resampleBuffer, resampleInput, outputBytes);
		mOutputFrames = inputFrames;

		return outputBytes;
	}

	const size_t outputFrames = resampleData( resampleInput, inputFrames, resampleBuffer, resampleBufferSize);

	if (outputFrames == 0) {
		meddbg("Resampling failed\n");
		return 0;
	}

	return outputFrames * outputBytesPerFrame;
}

} // namespace stream
} // namespace media
