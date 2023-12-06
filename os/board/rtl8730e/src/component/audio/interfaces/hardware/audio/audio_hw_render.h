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
 * @file audio_hw_render.h
 *
 * @brief Declares APIs for audio rendering.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_RENDER_H
#define AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_RENDER_H

#include <sys/types.h>
#include "hardware/audio/audio_hw_stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief AudioHwRender is the abstraction interface for the audio output hardware.
 * It provides information about various properties of the audio output
 * hardware driver.
 *
 * @since 1.0
 * @version 1.0
 */
struct AudioHwRender {
	struct AudioHwStream common;

	/**
	 * @brief Get system latency of the current <b>AudioHwRender</b> object .
	 *
	 * @param stream Indicates the pointer to the audio render to operate.
	 * @return Returns the current render latency;
	 */
	uint32_t (*GetLatency)(const struct AudioHwRender *stream);

	/**
	 * @brief Get current render frames and timestamp of the current <b>AudioHwRender</b> object .
	 *
	 * @param stream Indicates the pointer to the audio render to operate.
	 * @param frames Indicates the pointer to the current render frames of the current <b>AudioHwRender</b> object.
	 * @param timestamp Indicates the pointer to the current timestamp of the current <b>AudioHwRender</b> object.
	 * @return Returns <b>0</b> if render frames and timestamp get successfully;
	 * returns a negative value otherwise.
	 */
	int (*GetPresentationPosition)(const struct AudioHwRender *stream, uint64_t *frames, struct timespec *timestamp);

	/**
	 * @brief Get current render frames and timestamp of the current <b>AudioHwRender</b> object .
	 *
	 * @param stream Indicates the pointer to the audio render to operate.
	 * @param now_ns Indicates the pointer to system time, or tsf time.
	 * @param audio_ns Indicates the pointer to the playing audio time.
	 * @return Returns <b>0</b> if render frames and timestamp get successfully;
	 * returns a negative value otherwise.
	 */
	int (*GetPresentTime)(const struct AudioHwRender *stream, int64_t *now_ns, int64_t *audio_ns);

	/**
	 * @brief Write data to sound card for the current <b>AudioHwRender</b> object .
	 *
	 * @param stream Indicates the pointer to the audio render to operate.
	 * @param buffer Indicates the pointer to the current render buffer need to write to driver of the current <b>AudioHwRender</b> object.
	 * @param bytes  Indicates the size of the buffer.
	 * @param block  Indicates if dma buffer is full, should write block.
	 * @return Returns size written to driver;
	 * returns <b>0</b> value otherwise.
	 */
	ssize_t (*Write)(struct AudioHwRender *stream, const void *buffer, size_t bytes, bool block);

	int (*SetVolume)(struct AudioHwRender *stream, float left, float right);

	int (*Pause)(struct AudioHwRender *stream);

	int (*Resume)(struct AudioHwRender *stream);

	int (*Flush)(struct AudioHwRender *stream);
};

typedef struct AudioHwRender AudioHwRender;


#ifdef __cplusplus
}
#endif

#endif  // AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_RENDER_H
/** @} */
