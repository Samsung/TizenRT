/*
 * Copyright (c) 2021 Realtek, LLC.
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

/**
 * @addtogroup Audio
 * @{
 *
 * @brief Declares audio types and formats for audio framework.
 *
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file audio_type.h
 *
 * @brief Provides definition of the audio stream types and formats.
 *
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_FWK_MEDIA_AUDIO_INTERFACES_AUDIO_AUDIO_TYPE_H
#define AMEBA_FWK_MEDIA_AUDIO_INTERFACES_AUDIO_AUDIO_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defines all the audio playback usages.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	/** category type of track, for media */
	RTAUDIO_CATEGORY_MEDIA         = 0,
	/** category type of track, for calls*/
	RTAUDIO_CATEGORY_COMMUNICATION = 1,
	/** category type of track, for voice recognition */
	RTAUDIO_CATEGORY_SPEECH        = 2,
	/** category type of track, for beep sound, some as key tone */
	RTAUDIO_CATEGORY_BEEP          = 3,
	/** total category type number of track */
	RTAUDIO_CATEGORY_MAX_NUM       = 4,
};

/**
 * @brief Defines all the audio formats.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	/** invalid audio track and audio record data bit format*/
	RTAUDIO_FORMAT_INVALID           = 0xFFFFFFFFu,
	/** audio track and audio record data bit format, 8bit per channel per frame*/
	RTAUDIO_FORMAT_PCM_8_BIT         = 0x1u,
	/** audio track and audio record data bit format, 16bit per channel per frame*/
	RTAUDIO_FORMAT_PCM_16_BIT        = 0x2u,
	/** audio track and audio record data bit format, 32bit per channel per frame*/
	RTAUDIO_FORMAT_PCM_32_BIT        = 0x4u,
	/** audio track and audio record data bit format, float per channel per frame*/
	RTAUDIO_FORMAT_PCM_FLOAT         = 0x5u,
	/** audio track and audio record data bit format, 24bit packed per channel per frame*/
	RTAUDIO_FORMAT_PCM_24_BIT_PACKED = 0x6u,
	/** audio record and audio record data bit format, 24+8bit per channel per frame*/
	RTAUDIO_FORMAT_PCM_8_24_BIT      = 0x7u,
};

enum {
	/** Invalid pin */
	RTPIN_NONE        = 0x0u,
	/** Microphone input pin */
	RTPIN_IN_MIC      = 0x8000001u,
	/** Wired headset microphone pin for input */
	RTPIN_IN_HS_MIC   = 0x8000002u,
	/** Line-in pin */
	RTPIN_IN_LINEIN   = 0x8000004u,
	/**< dmic+ref-amic pin */
	RTPIN_IN_DMIC_REF_AMIC = 0x8000005u,
	RTPIN_IN_I2S           = 0x8000006u,
};

/**
 * @brief Defines all the audio output flags.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	RTAUDIO_OUTPUT_FLAG_NONE         = 0x0u,
	RTAUDIO_OUTPUT_FLAG_OFFLOAD      = 0x1u,
	RTAUDIO_OUTPUT_FLAG_NOIRQ        = 0x2u,
};

/**
 * @brief Defines all the audio input flags.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	RTAUDIO_INPUT_FLAG_NONE         = 0x0u,
	RTAUDIO_INPUT_FLAG_OFFLOAD      = 0x1u,
	RTAUDIO_INPUT_FLAG_NOIRQ        = 0x2u,
};

/**
 * @brief Defines all the audio effect types.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	RTAUDIO_EFFECT_INVALID           = 0xFFFFFFFFu,
	RTAUDIO_EFFECT_EQUALIZER         = 0x1u,
	RTAUDIO_EFFECT_VISUALIZER        = 0x2u,
};

/**
 * @brief Defines all the audio effect param types.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	RTAUDIO_EFFECT_PARAM_NUM_BANDS       = 0x0u,
	RTAUDIO_EFFECT_PARAM_LEVEL_RANGE     = 0x1u,
	RTAUDIO_EFFECT_PARAM_BAND_LEVEL      = 0x2u,
	RTAUDIO_EFFECT_PARAM_CENTER_FREQ     = 0x3u,
	RTAUDIO_EFFECT_PARAM_BAND_FREQ_RANGE = 0x4u,
	RTAUDIO_EFFECT_PARAM_GET_BAND        = 0x5u,
	RTAUDIO_EFFECT_PARAM_QFACTOR         = 0xAu,
};

/**
 * @brief Defines all the audio convert qualities.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	/** Invalid converter */
	RTAudio_CONVERT_INVALID        = 0x0u,
	RTAudio_CONVERT_LOW            = 0x1u,
	RTAudio_CONVERT_MID            = 0x2u,
	RTAudio_CONVERT_HIGH           = 0x3u,
	RTAudio_CONVERT_VERY_HIGH      = 0x4u,
	RTAudio_CONVERT_MAX            = 0x5u,
};

/**
 * @brief Defines all the audio resample types.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	/** Invalid converter */
	RTAudio_RESAMPLE_NONE          = 0x0u,
	RTAudio_RESAMPLE_SOXR          = 0x1u,
	RTAudio_RESAMPLE_SPEEX         = 0x2u,
	RTAudio_RESAMPLE_MAX           = 0x3u,
};

/**
 * @brief Defines all the audio min frames stages, only for mixer to use.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	/** allows more data for service to write to HAL one time, default uing it */
	RTAUDIO_OUT_MIN_FRAMES_STAGE1  = 0,
	/** allows less data for service to write to HAL one time */
	RTAUDIO_OUT_MIN_FRAMES_STAGE2  = 1,
};

#ifdef __cplusplus
}
#endif


#endif // AMEBA_FWK_MEDIA_AUDIO_INTERFACES_AUDIO_AUDIO_TYPE_H
/** @} */
