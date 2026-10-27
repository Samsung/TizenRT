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

#ifndef __MEDIA_RESAMPLER_H
#define __MEDIA_RESAMPLER_H

#include <cstddef>
#include <memory>
#include <sys/types.h>

#include <resample/speex_resampler.h>

namespace media {
namespace stream {

/**
 * @class Resampler
 * @brief Converts interleaved signed 16-bit PCM to a configured channel count
 *        and sample rate.
 *
 * @details The caller owns both the input and output buffers. This class owns
 * only the optional intermediate rechannel buffer and the Speex resampler
 * state. It never retains caller input between process() calls; the caller
 * must preserve incomplete PCM frames and submit only frame-aligned input.
 *
 * outputPeriodBytes configures the maximum output period expected from one
 * processing batch. A successful process() call can produce fewer bytes due
 * to a smaller input block, the sample-rate ratio, or Speex filter state.
 * Speex state is preserved across calls to maintain stream continuity and is
 * cleared by reset() or configure().
 *
 * @note The class is not thread-safe. Configuration, processing, and reset
 *       operations must be serialized by the caller.
 */
class Resampler
{
public:
	/**
	 * @brief Creates an unconfigured Resampler without allocating buffers.
	 */
	Resampler();

	/**
	 * @brief Releases the Speex state and any intermediate buffer.
	 */
	~Resampler();

	/**
	 * @brief Configures the input format, output format, and processing period.
	 *
	 * Any previous configuration and processing history are released before the
	 * new configuration is applied. On failure, the object remains
	 * unconfigured and process() returns an error until configure() succeeds.
	 *
	 * The current implementation accepts interleaved signed 16-bit PCM only:
	 * inputFormat and outputFormat must both be 2 bytes per sample. Supported
	 * input channel counts are 1 through 6; supported output channel counts are
	 * 1 and 2. outputPeriodBytes must contain a whole number of output frames.
	 *
	 * @param[in] inputChannels Number of interleaved input channels.
	 * @param[in] inputSampleRate Input sample rate in Hz.
	 * @param[in] inputFormat Input sample width in bytes; currently must be 2.
	 * @param[in] outputChannels Desired output channel count; must be 1 or 2.
	 * @param[in] outputSampleRate Desired output sample rate in Hz.
	 * @param[in] outputFormat Output sample width in bytes; currently must be 2.
	 * @param[in] outputPeriodBytes Output-buffer period in bytes. This determines
	 *            mOutputFramesPerProcess and the maximum accepted input batch.
	 *
	 * @retval true Configuration succeeded.
	 * @retval false A parameter is unsupported, the calculated input batch is
	 *         empty, or an internal allocation/initialization failed.
	 */
	bool configure(unsigned int inputChannels, unsigned int inputSampleRate,
				   unsigned int inputFormat, unsigned int outputChannels,
				   unsigned int outputSampleRate, unsigned int outputFormat,
				   size_t outputPeriodBytes);

	/**
	 * @brief Rechannels and/or resamples one interleaved PCM input block.
	 *
	 * The object must be configured. input and output must be non-null. inputSize
	 * must be frame-aligned and no greater than getInputBytesPerProcess().
	 * outputSize must be at least the configured output period. The caller must
	 * retain incomplete input-frame bytes and combine them with later input; this
	 * function does not buffer them.
	 *
	 * Input and output buffers must not overlap. Speex explicitly requires
	 * non-overlapping buffers during sample-rate conversion, and the direct-copy
	 * path uses memcpy(). A successful call consumes the supplied input even when
	 * zero output bytes are produced because of Speex filter state; callers must
	 * not submit the same input again.
	 *
	 * @param[in] input Input PCM buffer in the configured input layout.
	 * @param[in] inputSize Number of valid bytes in input.
	 * @param[out] output Caller-owned buffer for converted PCM.
	 * @param[in] outputSize Capacity of output in bytes.
	 *
	 * @return Number of output bytes produced, which may be zero.
	 * @retval -1 The object is not configured, a precondition is violated, or
	 *         channel/sample-rate conversion fails.
	 */
	ssize_t process(const unsigned char *input, size_t inputSize,
				unsigned char *output, size_t outputSize);

	/**
	 * @brief Returns the maximum input block accepted by one process() call.
	 *
	 * The value is derived from the configured output period and sample-rate
	 * ratio. A smaller frame-aligned input block is accepted and can produce a
	 * smaller output block.
	 *
	 * @return Maximum input size in bytes, or 0 while unconfigured.
	 */
	size_t getInputBytesPerProcess() const;

	/**
	 * @brief Clears stream history without destroying the configuration.
	 *
	 * Resets the Speex filter state and clears the intermediate rechannel buffer.
	 * Call this after seeking, stopping playback, or abandoning a failed stream.
	 * Calling reset() while unconfigured is harmless.
	 */
	void reset();

private:
	/**
	 * @brief Destroys internal resources and returns the object to its
	 *        unconfigured state.
	 *
	 * Used by the destructor and before every configure() attempt.
	 */
	void release();

	/**
	 * @brief Converts one input block to the configured output channel layout.
	 *
	 * This helper is called only when mNeedsRechannel is true. It writes the
	 * converted frames into mRechannelBuffer without retaining the caller input.
	 *
	 * @param[in] input Frame-aligned PCM in the configured input layout.
	 * @param[in] inputFrames Number of complete PCM frames in input; must not
	 *            exceed mInputFramesPerProcess.
	 *
	 * @retval true Every input frame was converted.
	 * @retval false The channel conversion failed or the scratch buffer could not
	 *         hold every converted frame.
	 */
	bool rechannelData(const unsigned char *input, size_t inputFrames);

	/**
	 * @brief Resamples PCM that is already in the configured output channel layout.
	 *
	 * This helper is called only when mNeedsResampling is true. input and output
	 * must not overlap. Speex processing state is updated as input frames are
	 * consumed; after a failure, reset() should be called before reusing the
	 * object for another stream.
	 *
	 * @param[in] input PCM using mOutputChannels and mInputSampleRate.
	 * @param[in] inputFrames Number of input frames available.
	 * @param[out] output Caller-owned output PCM buffer.
	 * @param[in,out] outputFrames On entry, output-buffer capacity in frames. On
	 *                success, number of frames actually produced.
	 *
	 * @retval true All input frames were consumed; outputFrames contains the
	 *         produced frame count, which may be zero.
	 * @retval false Speex failed or the output capacity was exhausted before all
	 *         input frames were consumed.
	 */
	bool resampleData(const unsigned char *input, size_t inputFrames,
					  unsigned char *output, size_t &outputFrames);

private:
	/** Speex state used only when input and output sample rates differ. */
	SpeexResamplerState *mSpeexResampler;

	/** Intermediate PCM buffer allocated only when channel conversion is needed. */
	std::unique_ptr<unsigned char[]> mRechannelBuffer;

	/** Capacity of mRechannelBuffer in bytes; zero when no buffer is allocated. */
	size_t mRechannelBufferSize;

	/** Output-frame capacity represented by the configured output period. */
	size_t mOutputFramesPerProcess;

	/**
	 * Maximum input frames accepted per call, calculated as
	 * floor(mOutputFramesPerProcess * inputRate / outputRate).
	 */
	size_t mInputFramesPerProcess;

	/** Configured input channel count; supported range is 1 through 6. */
	unsigned int mInputChannels;

	/** Configured input sample rate in Hz. */
	unsigned int mInputSampleRate;

	/** Configured input bytes per sample; currently always 2. */
	unsigned int mInputFormat;

	/** Configured output channel count; supported values are 1 and 2. */
	unsigned int mOutputChannels;

	/** Configured output sample rate in Hz. */
	unsigned int mOutputSampleRate;

	/** Configured output bytes per sample; currently always 2. */
	unsigned int mOutputFormat;

	/** True when input and output channel counts differ. */
	bool mNeedsRechannel;

	/** True when input and output sample rates differ. */
	bool mNeedsResampling;

	/** True only after configure() has completed successfully. */
	bool mConfigured;
};

} // namespace stream
} // namespace media

#endif
