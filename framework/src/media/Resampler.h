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

#include <resample/speex_resampler.h>

namespace media {
namespace stream {

class Resampler
{
public:
	Resampler();
	~Resampler();

	/**
	 * @brief Initializes the resampler with input and output constraints.
	 * 
	 * Must be called before pushing any data. If re-called, resets all internal buffers.
	 * Currently restricts to 16-bit PCM and a maximum of 2 output channels.
	 * 
	 * @return true on success, false if parameters are unsupported or allocation fails.
	 */
	bool configure(unsigned int inputChannels, unsigned int inputSampleRate, unsigned int inputFormat,
				   unsigned int outputChannels, unsigned int outputSampleRate, unsigned int outputFormat,
				   size_t outputPeriodBytes);

	size_t process(const unsigned char *buffPCM, size_t buffSize, unsigned char *resampleBuffer, size_t resampleBufferSize);

	size_t getInputBytesPerProcess() const;

    /**
	 * @brief Clears all pending internal buffers.
	 * 
	 * Use this when seeking, stopping playback, or recovering from an error. 
	 * It does not destroy the configuration.
	 */
	void reset();

private:
	void release();

	/**
	 * @brief Applies channel conversion (e.g., mono to stereo) to the buffered data.
	 * @param inputFrames The number of audio frames currently loaded in the rechannel buffer.
	 * @return true if successful or skipped, false on error.
	 */
	bool rechannelData(const unsigned char *input, size_t inputFrames);

	size_t resampleData(const unsigned char *input, size_t inputFrames, unsigned char *resampleBuffer, size_t resampleBufferSize);

private:
	SpeexResamplerState *mSpeexResampler;

	std::unique_ptr<unsigned char[]> mRechannelBuffer;
	size_t mRechannelBufferSize;

	size_t mOutputFramesPerProcess;			/* The target capacity of output frames per processing batch, derived from the configured outputPeriodBytes. */
	size_t mInputFramesPerProcess;			/* Calculated number of input frames required to yield mOutputFramesPerProcess. */
	size_t mInputBytes;						/* Current number of valid input bytes accumulated in the buffer. Reset on releaseOutput(). */
	size_t mOutputFrames;					/* Current number of valid output frames generated after processing. Reset on releaseOutput(). */

	unsigned int mInputChannels;			/* Input channel number */
	unsigned int mInputSampleRate;			/* Input sample rate */
	unsigned int mInputFormat;				/* Input bytes per sample (e.g., 2 for 16-bit PCM). */

	unsigned int mOutputChannels;			/* Output channel number */	
	unsigned int mOutputSampleRate;			/* Output sample rate */
	unsigned int mOutputFormat;				/* Output bytes per sample (e.g., 2 for 16-bit PCM). */

	bool mNeedsRechannel;					/* True if input and output channel counts differ. */
	bool mNeedsResampling;					/* True if input and output sample rates differ. */
};

} // namespace stream
} // namespace media

#endif
