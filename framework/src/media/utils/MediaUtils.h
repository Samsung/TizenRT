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
 * @since TizenRT v2.0 PRE
 */
void toLowerString(std::string& str);
/**
 * @brief Replace string with uppercase string.
 * @details @b #include <media/MediaUtils.h>
 * @param[out] str The str that uppercase string
 * @since TizenRT v2.0 PRE
 */
void toUpperString(std::string& str);
/**
 * @brief Gets the audio type in path.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] path The path of audio data
 * @return The audio type
 * @since TizenRT v2.0 PRE
 */
audio_type_t getAudioTypeFromPath(std::string path);
} // namespace utils
} // namespace media

#endif
/** @} */ // end of MEDIA group
