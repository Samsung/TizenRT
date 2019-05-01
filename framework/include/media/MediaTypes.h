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
 ******************************************************************/

/**
 * @ingroup MEDIA
 * @{
 */

/**
 * @file media/MediaTypes.h
 * @brief Provide definiton of Media Type
 */

#ifndef __MEDIA_TYPES_H
#define __MEDIA_TYPES_H

#include <tinyalsa/tinyalsa.h>

#define AAC_HEADER_LENGTH         7
#define MP3_HEADER_LENGTH         4
#define WAVE_HEADER_LENGTH        44

#define AUDIO_EXT_TYPE_AAC        "aac"
#define AUDIO_EXT_TYPE_FLAC       "flac"
#define AUDIO_EXT_TYPE_NULL       ""
#define AUDIO_EXT_TYPE_MP3        "mp3"
#define AUDIO_EXT_TYPE_MP4        "mp4"
#define AUDIO_EXT_TYPE_OPUS       "opus"
#define AUDIO_EXT_TYPE_PCM        "pcm"
#define AUDIO_EXT_TYPE_RAW        "raw"
#define AUDIO_EXT_TYPE_WAV        "wav"

namespace media {

/**
 * @brief Audio sample rate.
 * @details
 * @since TizenRT v2.0
 */
typedef enum audio_sample_rate_e {
	/** Audio sample rate is 7350 */
	AUDIO_SAMPLE_RATE_7350 = 7350,
	/** Audio sample rate is 8000 */
	AUDIO_SAMPLE_RATE_8000 = 8000,
	/** Audio sample rate is 11025 */
	AUDIO_SAMPLE_RATE_11025 = 11025,
	/** Audio sample rate is 12000 */
	AUDIO_SAMPLE_RATE_12000 = 12000,
	/** Audio sample rate is 16000 */
	AUDIO_SAMPLE_RATE_16000 = 16000,
	/** Audio sample rate is 22050 */
	AUDIO_SAMPLE_RATE_22050 = 22050,
	/** Audio sample rate is 24000 */
	AUDIO_SAMPLE_RATE_24000 = 24000,
	/** Audio sample rate is 32000 */
	AUDIO_SAMPLE_RATE_32000 = 32000,
	/** Audio sample rate is 44100 */
	AUDIO_SAMPLE_RATE_44100 = 44100,
	/** Audio sample rate is 48000 */
	AUDIO_SAMPLE_RATE_48000 = 48000,
	/** Audio sample rate is 64000 */
	AUDIO_SAMPLE_RATE_64000 = 64000,
	/** Audio sample rate is 88200 */
	AUDIO_SAMPLE_RATE_88200 = 88200,
	/** Audio sample rate is 96000 */
	AUDIO_SAMPLE_RATE_96000 = 96000

} audio_sample_rate_t;

/**
 * @brief Audio type.
 * @details
 * @since TizenRT v2.0
 */
typedef enum audio_type_e {
	/** Audio type is invalid */
	AUDIO_TYPE_INVALID = 0,
	/** Audio type is unknown */
	AUDIO_TYPE_UNKNOWN = AUDIO_TYPE_INVALID,
	/** Audio type is mp3 */
	AUDIO_TYPE_MP3 = 1,
	/** Audio type is AAC */
	AUDIO_TYPE_AAC = 2,
	/** Audio type is PCM */
	AUDIO_TYPE_PCM = 3,
	/** Audio type is OPUS */
	AUDIO_TYPE_OPUS = 4,
	/** Audio type is FLAC */
	AUDIO_TYPE_FLAC = 5,
	/** Audio type is WAVE */
	AUDIO_TYPE_WAVE = 6
} audio_type_t;

/**
 * @class
 * @brief Audio format type, each value follows pcm_format in tinyalsa.
 * @details
 * @since TizenRT v2.0
 */
typedef enum audio_format_type_e {
	/* Signed 8 bit */
	AUDIO_FORMAT_TYPE_S8 = PCM_FORMAT_S8,
	/* Signed 16 bit use this as a default */
	AUDIO_FORMAT_TYPE_S16_LE = PCM_FORMAT_S16_LE,
	/* Signed 32 bit */
	AUDIO_FORMAT_TYPE_S32_LE = PCM_FORMAT_S32_LE
} audio_format_type_t;

} // namespace media

#endif
/** @} */ // end of MEDIA group
