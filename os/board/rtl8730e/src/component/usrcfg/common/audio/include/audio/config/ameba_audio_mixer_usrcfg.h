/*
 * Copyright (c) 2023 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AMEBA_COMPONENT_USRCFG_COMMON_AUDIO_MIXER_USRCFG_H
#define AMEBA_COMPONENT_USRCFG_COMMON_AUDIO_MIXER_USRCFG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PrimaryAudioConfig {
	/** rate to be output by service, can be 44100, 16000, 48000*/
	uint32_t out_rate;
	/** channels to be output by service, only support 2 channels now*/
	uint32_t out_channels;
	/** format to be output by service, only support 16bit now */
	uint32_t out_format;
	/** period_size from service to set to HAL */
	uint32_t out_period_frames;
    /** period_count from service to set to HAL, to be supported, please set 4 now */
    uint32_t out_period_count;
    /** setup the min frames stage to output to HAL */
    uint32_t out_min_frames_stage;
} PrimaryAudioConfig;

extern PrimaryAudioConfig kPrimaryAudioConfig;

#ifdef __cplusplus
}
#endif

#endif  // AMEBA_COMPONENT_USRCFG_COMMON_AUDIO_MIXER_USRCFG_H
