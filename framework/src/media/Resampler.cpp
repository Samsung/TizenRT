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
#include "utils/internal_defs.h"

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
	mInputChannels(0),
	mInputSampleRate(0),
	mInputFormat(0),
	mOutputChannels(0),
	mOutputSampleRate(0),
	mOutputFormat(0),
	mNeedsRechannel(false),
	mNeedsResampling(false),
	mConfigured(false)
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

	mInputChannels = 0;
	mInputSampleRate = 0;
	mInputFormat = 0;

	mOutputChannels = 0;
	mOutputSampleRate = 0;
	mOutputFormat = 0;

	mNeedsRechannel = false;
	mNeedsResampling = false;
	mConfigured = false;
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
	 * remix implementation supports output channel count 1-2 & input channel counts 1–6.
	 */
	if (outputChannels > 2) {
		meddbg("Unsupported output channel count: %u\n", outputChannels);
		return false;
	}

	if (ch2layout(inputChannels) == 0) {
		meddbg("Unsupported input channel count: %u\n", inputChannels);
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

	if (mNeedsRechannel) {
		mRechannelBufferSize = mInputFramesPerProcess * mOutputChannels * mOutputFormat;
		mRechannelBuffer = std::make_unique<unsigned char[]>(mRechannelBufferSize);

		if (!mRechannelBuffer) {
			meddbg("Rechannel buffer allocation failed: %lu\n", static_cast<unsigned long>(mRechannelBufferSize));
			release();
			return false;
		}
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

	mConfigured = true;

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

	if (mRechannelBuffer) {
		memset(mRechannelBuffer.get(), 0, mRechannelBufferSize);
	}
}

bool Resampler::rechannelData(const unsigned char *input, size_t inputFrames)
{
	const size_t outputFramesCapacity = mRechannelBufferSize / (mOutputChannels * mOutputFormat);
	const int32_t rechanneledFrames = rechannel(ch2layout(mInputChannels), ch2layout(mOutputChannels),
												reinterpret_cast<const int16_t *>(input), static_cast<uint32_t>(inputFrames),
												reinterpret_cast<int16_t *>(mRechannelBuffer.get()), static_cast<uint32_t>(outputFramesCapacity));
	if (rechanneledFrames < 0 || static_cast<size_t>(rechanneledFrames) != inputFrames) {
		meddbg("Fail to rechannel each frame, %d/%lu\n", rechanneledFrames, static_cast<unsigned long>(inputFrames));
		return false;
	}

	return true;
}

bool Resampler::resampleData(const unsigned char *input, size_t inputFrames, unsigned char *output, size_t &outputFrames)
{
	size_t usedFrames = 0;
	size_t resampledFrames = 0;
	const spx_int16_t *dataIn;
	spx_uint32_t inFrames;
	spx_int16_t *dataOut;
	spx_uint32_t outFrames;
	int ret;

	while (inputFrames > usedFrames) {
		dataIn = reinterpret_cast<const spx_int16_t *>(input + (usedFrames * mOutputChannels * mOutputFormat));
		inFrames = static_cast<spx_uint32_t>(inputFrames - usedFrames);
		dataOut = reinterpret_cast<int16_t *>(output + (resampledFrames * mOutputChannels * mOutputFormat));
		outFrames = static_cast<spx_uint32_t>(outputFrames - resampledFrames);
		medvdbg("dataIn %p, inputFrames %u\n", static_cast<const void *>(dataIn), inFrames);
		medvdbg("dataOut %p, outputFrames resample buffer can hold %u\n", static_cast<void *>(dataOut), outFrames);

		ret = speex_resampler_process_interleaved_int(mSpeexResampler, dataIn, &inFrames, dataOut, &outFrames);
		if (ret != RESAMPLER_ERR_SUCCESS) {
			meddbg("Fail to resample out:%lu/%lu, error %d\n", static_cast<unsigned long>(usedFrames), static_cast<unsigned long>(inputFrames), ret);
			return false;
		}

		usedFrames += inFrames;
		if (outFrames > 0) {
			resampledFrames += outFrames;
		} else if (inputFrames != usedFrames) {
			meddbg("Error: output buffer is full, used input frames %lu/%lu\n", static_cast<unsigned long>(usedFrames), static_cast<unsigned long>(inputFrames));
			return false;
		}
		medvdbg("%lu frames generated from %lu/%lu\n", static_cast<unsigned long>(resampledFrames), static_cast<unsigned long>(usedFrames), static_cast<unsigned long>(inputFrames));
	}

	medvdbg("resampled frames count: %lu\n", static_cast<unsigned long>(resampledFrames));
	outputFrames = resampledFrames;
	return true;
}

ssize_t Resampler::process(const unsigned char *input, size_t inputSize, unsigned char *output, size_t outputSize)
{
	if (!mConfigured) {
		meddbg("Resampler is not configured\n");
		return ERROR;
	}

	if (!input) {
		meddbg("Invalid parameter, input buffer is null.\n");
		return ERROR;
	}

	const size_t inputBytesPerFrame = mInputChannels * mInputFormat;
	const size_t inputBytesPerProcess = mInputFramesPerProcess * inputBytesPerFrame;
	if (inputSize > inputBytesPerProcess) {
		meddbg("Invalid parameter, input size exceed input bytes per process capacity: %lu/%lu\n", static_cast<unsigned long>(inputSize), static_cast<unsigned long>(inputBytesPerProcess));
		return ERROR;
	}

	if (!output) {
		meddbg("Invalid parameter, output buffer is null.\n");
		return ERROR;
	}

	const size_t outputBytesPerFrame = mOutputChannels * mOutputFormat;
	const size_t outputBytesPerProcess = mOutputFramesPerProcess * outputBytesPerFrame;
	if (outputSize < outputBytesPerProcess) {
		meddbg("Invalid parameter, Output buffer is too small: %lu/%lu\n", static_cast<unsigned long>(outputSize), static_cast<unsigned long>(outputBytesPerProcess));
		return ERROR;
	}

	/* Its responsibility of caller to give input as frame aligned */
	if (inputSize % inputBytesPerFrame != 0) {
		meddbg("Input size is not frame aligned: %lu/%lu\n", static_cast<unsigned long>(inputSize), static_cast<unsigned long>(inputBytesPerFrame));
		return ERROR;
	}

	const size_t inputFrames = inputSize / inputBytesPerFrame;
	size_t outputFrames = outputSize / outputBytesPerFrame;
	if (inputFrames == 0) {
		return 0;
	}

	if (mNeedsRechannel) {
		if (!rechannelData(input, inputFrames)) {
			meddbg("Rechanneling failed\n");
			return ERROR;
		}

		input = mRechannelBuffer.get();
	}

	if (mNeedsResampling) {
		if (!resampleData(input, inputFrames, output, outputFrames)) {
			meddbg("Resampling failed\n");
			return ERROR;
		}
	} else {
		outputFrames = inputFrames;
		memcpy(output, input, outputFrames * outputBytesPerFrame);
	}

	return outputFrames * outputBytesPerFrame;
}

} // namespace stream
} // namespace media
