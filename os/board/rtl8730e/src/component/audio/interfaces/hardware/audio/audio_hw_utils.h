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
 * @file audio_hw_utils.h
 *
 * @brief Declares audio basic functions.
 *
 * @since 1.0
 * @version 1.0
 */


#ifndef AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_UTILS_H
#define AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_UTILS_H

#include "hardware/audio/audio_hw_types.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline size_t GetAudioBytesPerSample(AudioHwFormat format)
{
	size_t size = 0;

	switch (format) {
	case AUDIO_HW_FORMAT_PCM_32_BIT:
	case AUDIO_HW_FORMAT_PCM_8_24_BIT:
		size = sizeof(int32_t);
		break;
	case AUDIO_HW_FORMAT_PCM_24_BIT:
		size = sizeof(uint8_t) * 3;
		break;
	case AUDIO_HW_FORMAT_PCM_16_BIT:
		size = sizeof(int16_t);
		break;
	case AUDIO_HW_FORMAT_PCM_8_BIT:
		size = sizeof(uint8_t);
		break;
	default:
		break;
	}
	return size;
}

static inline bool AudioIsLinearPCM(AudioHwFormat format)
{
	return ((format & AUDIO_HW_FORMAT_MAIN_MASK) == AUDIO_HW_FORMAT_PCM);
}

static inline AudioHwFormat AudioGetMainFormat(AudioHwFormat format)
{
	return (AudioHwFormat)(format & AUDIO_HW_FORMAT_MAIN_MASK);
}

static inline bool AudioCheckValidFormat(AudioHwFormat format)
{
	switch (format & AUDIO_HW_FORMAT_MAIN_MASK) {
	case AUDIO_HW_FORMAT_PCM:
		switch (format) {
		case AUDIO_HW_FORMAT_PCM_16_BIT:
		case AUDIO_HW_FORMAT_PCM_8_BIT:
		case AUDIO_HW_FORMAT_PCM_32_BIT:
		case AUDIO_HW_FORMAT_PCM_24_BIT:
			return true;
		default:
			return false;
		}

	default:
		return false;
	}
}

#ifdef __cplusplus
}
#endif

#endif  // AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_UTILS_H
/** @} */
