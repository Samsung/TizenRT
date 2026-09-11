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
 ****************************************************************/

#include <tinyara/config.h>
#include <debug.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "resample/speex_resampler.h"
#include "utils/remix.h"
#include "PCMSampler.h"

#define RESAMPLING_QUALITY 5
#define MAX_RESAMPLING_QUALITY 10

namespace media {
namespace stream {

PCMSampler::PCMSampler() :
	mUser{0, 0, PCM_FORMAT_NONE},
	mOutput{0, 0, PCM_FORMAT_NONE},
	mResampler(nullptr),
	mInputBuffer(nullptr),
	mInputBufferSize(0),
	mPendingInputBytes(0),
	mS16Buffer(nullptr),
	mS16BufferSize(0),
	mRechannelBuffer(nullptr),
	mRechannelBufferSize(0),
	mOutputBuffer(nullptr),
	mOutputBufferSize(0),
	mConfigured(false)
{
}

PCMSampler::~PCMSampler()
{
	release();
}

void PCMSampler::release()
{
	if (mResampler) {
		speex_resampler_destroy(mResampler);
		mResampler = nullptr;
	}
	delete[] mInputBuffer;
	mInputBuffer = nullptr;
	mInputBufferSize = 0;
	mPendingInputBytes = 0;
	delete[] mS16Buffer;
	mS16Buffer = nullptr;
	mS16BufferSize = 0;
	delete[] mRechannelBuffer;
	mRechannelBuffer = nullptr;
	mRechannelBufferSize = 0;
	delete[] mOutputBuffer;
	mOutputBuffer = nullptr;
	mOutputBufferSize = 0;
	mConfigured = false;
}

void PCMSampler::reset()
{
	mPendingInputBytes = 0;
	if (mResampler) {
		int ret = speex_resampler_reset_mem(mResampler);
		if (ret != RESAMPLER_ERR_SUCCESS) {
			meddbg("Failed to reset output resampler, error: %d\n", ret);
		}
	}
}

bool PCMSampler::configure(const pcm_stream_format_t &user, const pcm_stream_format_t &output)
{
	release();

	if (user.channels == 0 || user.channels > 2 || user.sampleRate == 0 ||
		output.channels == 0 || output.channels > 2 || output.sampleRate == 0) {
		meddbg("Invalid PCM channel or sample rate\n");
		return false;
	}

	if (output.format != PCM_FORMAT_S16_LE) {
		meddbg("Only S16_LE hardware output is supported, format: %d\n", output.format);
		return false;
	}

	if (user.format != PCM_FORMAT_S8 && user.format != PCM_FORMAT_S16_LE &&
		user.format != PCM_FORMAT_S32_LE) {
		meddbg("Unsupported source PCM format: %d\n", user.format);
		return false;
	}

	mUser = user;
	mOutput = output;

	if (mUser.sampleRate != mOutput.sampleRate) {
		int error = RESAMPLER_ERR_SUCCESS;
		int quality = RESAMPLING_QUALITY;
		if ((mOutput.sampleRate > mUser.sampleRate && mOutput.sampleRate % mUser.sampleRate == 0) ||
			(mUser.sampleRate > mOutput.sampleRate && mUser.sampleRate % mOutput.sampleRate == 0)) {
			quality = MAX_RESAMPLING_QUALITY;
		}

		mResampler = speex_resampler_init(mOutput.channels, mUser.sampleRate,
										mOutput.sampleRate, quality, &error);
		if (!mResampler) {
			meddbg("Failed to create output resampler, error: %d\n", error);
			release();
			return false;
		}
	}

	mConfigured = true;
	return true;
}

size_t PCMSampler::getSourceFrameBytes() const
{
	return mUser.channels * (pcm_format_to_bits(mUser.format) >> 3);
}

size_t PCMSampler::getOutputFrameBytes() const
{
	return mOutput.channels * (pcm_format_to_bits(mOutput.format) >> 3);
}

bool PCMSampler::isConfigured() const
{
	return mConfigured;
}

size_t PCMSampler::getInputBytesForOutput(size_t outputBytes) const
{
	if (!mConfigured) {
		return outputBytes;
	}

	size_t outputFrameBytes = getOutputFrameBytes();
	size_t sourceFrameBytes = getSourceFrameBytes();
	if (outputFrameBytes == 0 || sourceFrameBytes == 0) {
		return 0;
	}

	uint64_t outputFrames = outputBytes / outputFrameBytes;
	if (outputFrames > UINT64_MAX / mUser.sampleRate) {
		return 0;
	}
	uint64_t sourceFrames = outputFrames * mUser.sampleRate / mOutput.sampleRate;
	if (sourceFrames == 0 && outputFrames > 0) {
		sourceFrames = 1;
	}
	if (sourceFrames > SIZE_MAX / sourceFrameBytes) {
		return 0;
	}
	return (size_t)sourceFrames * sourceFrameBytes;
}

ssize_t PCMSampler::convert(const unsigned char *input, size_t inputBytes, const unsigned char **output)
{
	if (!mConfigured || !input || !output) {
		return -1;
	}

	size_t sourceFrameBytes = getSourceFrameBytes();
	if (sourceFrameBytes == 0 || sourceFrameBytes > sizeof(mPendingInput) ||
		inputBytes > SIZE_MAX - mPendingInputBytes) {
		return -1;
	}

	size_t totalBytes = mPendingInputBytes + inputBytes;
	if (totalBytes < sourceFrameBytes) {
		memcpy(mPendingInput + mPendingInputBytes, input, inputBytes);
		mPendingInputBytes = totalBytes;
		*output = nullptr;
		return 0;
	}

	const unsigned char *completeInput = input;
	if (mPendingInputBytes > 0) {
		if (!ensureBuffer(&mInputBuffer, &mInputBufferSize, totalBytes)) {
			return -1;
		}
		memcpy(mInputBuffer, mPendingInput, mPendingInputBytes);
		memcpy(mInputBuffer + mPendingInputBytes, input, inputBytes);
		completeInput = mInputBuffer;
	}

	size_t completeBytes = totalBytes - totalBytes % sourceFrameBytes;
	size_t trailingBytes = totalBytes - completeBytes;
	if (trailingBytes > 0) {
		memcpy(mPendingInput, completeInput + completeBytes, trailingBytes);
	}
	mPendingInputBytes = trailingBytes;

	size_t frames = completeBytes / sourceFrameBytes;
	if (frames > INT_MAX || !convertToS16(completeInput, frames)) {
		return -1;
	}

	const int16_t *resampleInput = reinterpret_cast<const int16_t *>(mS16Buffer);
	if (mUser.channels != mOutput.channels) {
		if (frames > SIZE_MAX / mOutput.channels / sizeof(int16_t)) {
			return -1;
		}
		size_t rechannelBytes = frames * mOutput.channels * sizeof(int16_t);
		if (!ensureBuffer(&mRechannelBuffer, &mRechannelBufferSize, rechannelBytes)) {
			return -1;
		}
		int32_t convertedFrames = rechannel(ch2layout(mUser.channels), ch2layout(mOutput.channels),
										   resampleInput, frames,
										   reinterpret_cast<int16_t *>(mRechannelBuffer), frames);
		if (convertedFrames != static_cast<int32_t>(frames)) {
			meddbg("Failed to rechannel source PCM: %d/%u\n", convertedFrames, frames);
			return -1;
		}
		resampleInput = reinterpret_cast<const int16_t *>(mRechannelBuffer);
	}

	return resample(resampleInput, frames, output);
}

ssize_t PCMSampler::resample(const int16_t *input, size_t frames, const unsigned char **output)
{
	uint64_t estimatedFrames = ((uint64_t)frames * mOutput.sampleRate + mUser.sampleRate - 1) /
							 mUser.sampleRate;
	if (mResampler) {
		estimatedFrames += speex_resampler_get_output_latency(mResampler) + 1;
	}

	size_t outputFrameBytes = getOutputFrameBytes();
	if (estimatedFrames > SIZE_MAX / outputFrameBytes ||
		estimatedFrames > static_cast<uint64_t>(INT_MAX) / outputFrameBytes) {
		return -1;
	}
	if (!ensureBuffer(&mOutputBuffer, &mOutputBufferSize, (size_t)estimatedFrames * outputFrameBytes)) {
		return -1;
	}

	if (!mResampler) {
		size_t bytes = frames * outputFrameBytes;
		memcpy(mOutputBuffer, input, bytes);
		*output = mOutputBuffer;
		return (ssize_t)bytes;
	}

	spx_uint32_t inputFrames = static_cast<spx_uint32_t>(frames);
	spx_uint32_t outputFrames = static_cast<spx_uint32_t>(estimatedFrames);
	int ret = speex_resampler_process_interleaved_int(mResampler, input, &inputFrames,
													  reinterpret_cast<int16_t *>(mOutputBuffer), &outputFrames);
	if (ret != RESAMPLER_ERR_SUCCESS || inputFrames != frames) {
		meddbg("Output resampling failed, error: %d, consumed: %u/%u\n", ret, inputFrames, frames);
		return -1;
	}

	*output = mOutputBuffer;
	return static_cast<ssize_t>((size_t)outputFrames * outputFrameBytes);
}

bool PCMSampler::convertToS16(const unsigned char *input, size_t frames)
{
	if (frames > SIZE_MAX / mUser.channels) {
		return false;
	}
	size_t samples = frames * mUser.channels;
	if (samples > SIZE_MAX / sizeof(int16_t) ||
		!ensureBuffer(&mS16Buffer, &mS16BufferSize, samples * sizeof(int16_t))) {
		return false;
	}

	int16_t *output = reinterpret_cast<int16_t *>(mS16Buffer);
	switch (mUser.format) {
	case PCM_FORMAT_S8:
		for (size_t i = 0; i < samples; i++) {
			output[i] = static_cast<int16_t>(static_cast<int8_t>(input[i])) * 256;
		}
		break;
	case PCM_FORMAT_S16_LE:
		memcpy(output, input, samples * sizeof(int16_t));
		break;
	case PCM_FORMAT_S32_LE:
		for (size_t i = 0; i < samples; i++) {
			uint32_t value = static_cast<uint32_t>(input[i * 4]) |
							 (static_cast<uint32_t>(input[i * 4 + 1]) << 8) |
							 (static_cast<uint32_t>(input[i * 4 + 2]) << 16) |
							 (static_cast<uint32_t>(input[i * 4 + 3]) << 24);
			output[i] = static_cast<int16_t>(value >> 16);
		}
		break;
	default:
		return false;
	}

	return true;
}

bool PCMSampler::ensureBuffer(unsigned char **buffer, size_t *capacity, size_t required)
{
	if (*capacity >= required) {
		return true;
	}

	unsigned char *newBuffer = new (std::nothrow) unsigned char[required];
	if (!newBuffer) {
		meddbg("Failed to allocate conversion buffer, bytes: %u\n", required);
		return false;
	}

	delete[] *buffer;
	*buffer = newBuffer;
	*capacity = required;
	return true;
}

} // namespace stream
} // namespace media
