/*
 * Copyright (c) 2021 Realtek, LLC.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @addtogroup HAL
 * @{
 *
 * @brief Declares the structures and interfaces for the Hardware Abstraction Layer (HAL) module.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file audio_hw_capture.h
 *
 * @brief Declares APIs for audio capturing.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_CAPTURE_H
#define AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_CAPTURE_H

#include <sys/types.h>
#include "hardware/audio/audio_hw_stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Provides capabilities for audio capturing, including controlling the capturing, setting audio attributes,
 * volume, and capturing audio frames.
 * @since 1.0
 * @version 1.0
 */
struct AudioHwCapture {
	/**
	 * Common methods of the audio stream in.  This *must* be the first member of AudioHwCapture
	 * as users of this structure will cast a AudioHwStream to AudioHwCapture pointer in contexts
	 * where it's known the AudioHwStream references an AudioHwCapture.
	 */
	struct AudioHwStream common;

	/**
	 * @brief Get current capture frames and timestamp of the current <b>AudioHwCapture</b> object .
	 *
	 * @param stream Indicates the pointer to the audio capture to operate.
	 * @param frames Indicates the pointer to the current capture frames of the current <b>AudioHwCapture</b> object.
	 * @param timestamp Indicates the pointer to the current timestamp of the current <b>AudioHwCapture</b> object.
	 * @return Returns <b>0</b> if capture frames and timestamp get successfully;
	 * returns a negative value otherwise.
	 */
	int (*GetCapturePosition)(const struct AudioHwCapture *stream, uint64_t *frames, struct timespec *timestamp);

	/**
	 * @brief Get current capture frames and timestamp of the current <b>AudioHwcapture</b> object .
	 *
	 * @param stream Indicates the pointer to the audio capture to operate.
	 * @param now_ns Indicates the pointer to system time, or tsf time.
	 * @param audio_ns Indicates the pointer to the playing audio time.
	 * @return Returns <b>0</b> if capture frames and timestamp get successfully;
	 * returns a negative value otherwise.
	 */
	int (*GetPresentTime)(const struct AudioHwCapture *stream, int64_t *now_ns, int64_t *audio_ns);

	/**
	 * @brief Get system latency of the current <b>AudioHwCapture</b> object .
	 *
	 * @param stream Indicates the pointer to the audio capture to operate.
	 * @return Returns the current capture latency;
	 */
	uint32_t (*GetLatency)(const struct AudioHwCapture *stream);

	/**
	 * @brief Read data from sound card for the current <b>AudioHwCapture</b> object .
	 *
	 * @param stream Indicates the pointer to the audio capture to operate.
	 * @param buffer Indicates the pointer to the current capture buffer need to read data from driver of the current <b>AudioHwCapture</b> object.
	 * @param bytes  Indicates the size of the buffer.
	 * @return Returns  the data size read from driver;
	 * returns <b>0</b> value otherwise.
	 */
	ssize_t (*Read)(struct AudioHwCapture *stream, void *buffer, size_t bytes);
};

typedef struct AudioHwCapture AudioHwCapture;

size_t GetHwInputBufferSize(uint32_t sample_rate, AudioHwFormat format, uint32_t channel_count);

#ifdef __cplusplus
}
#endif

#endif  // AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_CAPTURE_H
/** @} */
