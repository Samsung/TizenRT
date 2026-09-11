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

/**
 * @brief An audio processing component that handles channel conversion and sample rate conversion.
 *
 * Typical lifecycle:
 * 1. configure()      - Initialize formats and allocate buffers.
 * 2. pushData()       - Push raw PCM data until getRemainingInputBytes() == 0.
 * 3. process()        - Process the buffered data. Check the Result.
 * 4. getOutput()      - If process() returned OUTPUT_READY, retrieve the resampled data.
 * 5. releaseOutput()  - Acknowledge that you are done reading the output buffer.
 * 6. Repeat steps 2-5.
 * 7. drain()          - At End of Stream (EOS), call drain() to process any residual data.
 */
class Resampler
{
public:
	/**
	 * @brief A read-only view of the internal output buffer.
	 */
	struct ConstBufferView {
		const unsigned char *data;
		size_t size;
	};

	/**
	 * @brief Return codes for processing operations indicating what the caller must do next.
	 */
	enum class Result {
		ERROR,			/* An internal error occurred (e.g., state mismatch or processing failure). */
		NEED_INPUT,		/* The resampler needs more data. The caller should use pushData(). */
		OUTPUT_READY,	/* Audio processing succeeded. The caller MUST call getOutput() followed by releaseOutput(). */
		DRAINED,		/* Returned by drain() indicating all residual data has been processed and the pipeline is empty. */
	};

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

	/**
	 * @brief Clears all pending internal buffers and resets the processing state.
	 * 
	 * Use this when seeking, stopping playback, or recovering from an error. 
	 * It does not destroy the configuration.
	 */
	void reset();

	/**
	 * @brief Gets the number of bytes required to fill the input buffer for the next processing pass.
	 * 
	 * This is a dynamic value. As you call pushData(), this value decreases. 
	 * When it reaches 0, the buffer is full and you should call process().
	 * 
	 * @return Number of bytes needed. Returns 0 if the state is invalid or buffer is full.
	 */
	size_t getRemainingInputBytes() const;

	/**
	 * @brief Copies data into the resampler's internal buffer.
	 * 
	 * This method only consumes up to getRemainingInputBytes(). If you pass in more data 
	 * than needed, the remainder will be ignored, and you must push it in a subsequent call.
	 * 
	 * @param data Pointer to the input PCM data.
	 * @param size Size of the input data in bytes.
	 * @return The actual number of bytes consumed. 
	 */
	size_t pushData(const unsigned char *data, size_t size);

	/**
	 * @brief Processes the buffered input data.
	 * 
	 * Should be called when getRemainingInputBytes() == 0 (Internal state is BUFFERED).
	 * 
	 * @return 
	 * - Result::OUTPUT_READY: Processing succeeded. Caller must call getOutput() and releaseOutput().
	 * - Result::NEED_INPUT: Processing finished but yielded no output (e.g., due to algorithmic delay).
	 * - Result::ERROR: State violation or resampling failure.
	 */
	Result process();

	/**
	 * @brief Processes any residual, incomplete data sitting in the buffer at End of Stream.
	 * 
	 * @return 
	 * - Result::OUTPUT_READY: Residual data produced a final output frame.
	 * - Result::DRAINED: No output could be produced from residual data; the resampler is fully flushed.
	 * - Result::ERROR: State violation or resampling failure.
	 */
	Result drain();

	/**
	 * @brief Retrieves the pointer to the processed output data.
	 * 
	 * @warning This MUST ONLY be called if process() or drain() returned Result::OUTPUT_READY.
	 * 
	 * @return A view of the processed data. Returns {nullptr, 0} if called in an invalid state.
	 */
	ConstBufferView getOutput() const;

	/**
	 * @brief Signals that the caller is finished reading the buffer provided by getOutput().
	 * 
	 * @warning This MUST ONLY be called if process() or drain() returned Result::OUTPUT_READY.
	 * Calling this safely resets internal trackers so pushData() can begin again.
	 * 
	 * @return true on success, false if called in an invalid state (data is protected from deletion).
	 */
	bool releaseOutput();

private:
	/**
	 * @brief Internal state machine controlling the flow of data.
	 * 
	 * Normal batch lifecycle: 
	 * NONE (Unconfigured) -> IDLE -> BUFFERING (Pushing) -> BUFFERED (Full) -> PROCESSED -> IDLE.
	 */
	enum class State {
		NONE,			/* configure() has not been called or failed. */
		IDLE,			/* Configured and empty. Ready for the first pushData(). */
		BUFFERING,		/* pushData() has been called, but getRemainingInputBytes() > 0. */
		BUFFERED,		/* pushData() filled the buffer (getRemainingInputBytes() == 0). Ready for process(). */
		PROCESSED,		/* process() completed successfully. Ready for getOutput() and releaseOutput(). */
	};

	/**
	 * @brief Safely frees all allocated memory (Speex context, buffers) and resets metrics to 0.
	 * Called during destruction and re-configuration.
	 */
	void release();

	/**
	 * @brief Applies channel conversion (e.g., mono to stereo) to the buffered data.
	 * @param inputFrames The number of audio frames currently loaded in the rechannel buffer.
	 * @return true if successful or skipped, false on error.
	 */
	bool rechannelData(size_t inputFrames);

	/**
	 * @brief Applies sample rate conversion using the Speex library.
	 * @param inputFrames The number of audio frames available after rechanneling.
	 * @return true if successful or skipped, false on error.
	 */
	bool resample(size_t inputFrames);

	/**
	 * @brief The core processing pipeline executed by both process() and drain().
	 * Orchestrates rechanneling, resampling, and state transitions based on the current buffer contents.
	 * @return The Result to be passed back to the public caller.
	 */	
	Result processInput();

	SpeexResamplerState *mSpeexResampler;				/* Opaque pointer to the internal state of the Speex resampler library. */
	std::unique_ptr<unsigned char[]> mRechannelBuffer;	/* Buffer that holds pushed input data and the output of the channel conversion step. */
	std::unique_ptr<unsigned char[]> mResampleBuffer;	/* Buffer that holds the final output of the sample rate conversion step. */

	unsigned int mInputChannels;						/* Input channel number */
	unsigned int mInputSampleRate;						/* Input sample rate */
	unsigned int mInputFormat;							/* Input bytes per sample (e.g., 2 for 16-bit PCM). */
	unsigned int mOutputChannels;						/* Output channel number */				
	unsigned int mOutputSampleRate;						/* Output sample rate */
	unsigned int mOutputFormat;							/* Output bytes per sample (e.g., 2 for 16-bit PCM). */

	size_t mOutputFramesPerProcess;						/* The target capacity of output frames per processing batch, derived from the configured outputPeriodBytes. */
	size_t mInputFramesPerProcess;						/* Calculated number of input frames required to yield mOutputFramesPerProcess. */
	size_t mRechannelBufferSize;						/* Allocated size of mRechannelBuffer in bytes. */
	size_t mResampleBufferSize;							/* Allocated size of mResampleBuffer in bytes. */
	size_t mInputBytes;									/* Current number of valid input bytes accumulated in the buffer. Reset on releaseOutput(). */
	size_t mOutputFrames;								/* Current number of valid output frames generated after processing. Reset on releaseOutput(). */
	State mState;										/* Current state in the internal processing lifecycle. */

	bool mNeedsRechannel;								/* True if input and output channel counts differ. */
	bool mNeedsResampling;								/* True if input and output sample rates differ. */
};

} // namespace stream
} // namespace media

#endif
