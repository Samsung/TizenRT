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
 * @file media/MediaUtils.h
 * @brief Media MediaUtils APIs
 */

#ifndef __MEDIA_UTILS_H
#define __MEDIA_UTILS_H

#include <string>
#include <media/MediaTypes.h>

namespace media {
namespace utils {

/**
 * @brief Replace string with lowercase string.
 * @details @b #include <media/MediaUtils.h>
 * @param[out] str The str that lowercase string
 * @since TizenRT v2.0
 */
void toLowerString(std::string& str);
/**
 * @brief Replace string with uppercase string.
 * @details @b #include <media/MediaUtils.h>
 * @param[out] str The str that uppercase string
 * @since TizenRT v2.0
 */
void toUpperString(std::string& str);
/**
 * @brief Gets the audio type in path.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] path The path of audio data
 * @return The audio type
 * @since TizenRT v2.0
 */
audio_type_t getAudioTypeFromPath(std::string path);
/**
 * @brief Parsing the audio type in file.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] audio file point, Audio type and channel, sample rate, pcm format adderss to receive.
 * @return ture - parsing success. false - parsing fail.
 * @since TizenRT v2.0
 */
bool header_parsing(FILE *fp, audio_type_t AudioType, unsigned int *channel, unsigned int *sample_rate, audio_format_type_t *pcmFormat);
/**
 * @brief Parsing the audio type in buffer.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] audio file buffer, buffer size, Audio type and channel, sample rate, pcm format adderss to receive.
 * @return ture - parsing success. false - parsing fail.
 * @since TizenRT v2.0
 */
bool header_parsing(unsigned char *buffer, unsigned int bufferSize, audio_type_t audioType, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat);
} // namespace utils
} // namespace media

#endif
/** @} */ // end of MEDIA group
