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
/****************************************************************************
*
* Developed by:
*
*   Copyright (C) 2013 Ken Pettit. All rights reserved.
*   Author: Ken Pettit <pettitkd@gmail.com>
*
* With ongoing support:
*
*   Copyright (C) 2014 Gregory Nutt. All rights reserved.
*   Author: Greory Nutt <gnutt@nuttx.org>
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name NuttX nor the names of its contributors may be
*    used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
/*
** Copyright 2011, The Android Open Source Project
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of The Android Open Source Project nor the names of
**       its contributors may be used to endorse or promote products derived
**       from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY The Android Open Source Project ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL The Android Open Source Project BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
** DAMAGE.
*/

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

namespace media {
namespace utils {

/**
 * @brief Audio type.
 * @details @b #include <media/MediaUtils.h>
 * @since TizenRT v2.0 PRE
 */
typedef enum audio_type_e {
	/** Audio type is invalid */
	AUDIO_TYPE_INVALID = 0,
	/** Audio type is mp3 */
	AUDIO_TYPE_MP3 = 1,
	/** Audio type is AAC */
	AUDIO_TYPE_AAC = 2,
	/** Audio type is PCM */
	AUDIO_TYPE_PCM = 3,
	/** Audio type is PCM */
	AUDIO_TYPE_OPUS = 4,
	/** Audio type is PCM */
	AUDIO_TYPE_FLAC = 5
} audio_type_t;

/**
 * @brief Audio sample format of a PCM.
 * @details The first letter specifiers whether the sample is signed or unsigned.
 * The letter 'S' means signed. The letter 'U' means unsigned.
 * The following number is the amount of bits that the sample occupies in memory.
 * Following the underscore, specifiers whether the sample is big endian or little endian.
 * The letters 'LE' mean little endian.
 * The letters 'BE' mean big endian.
 */
typedef enum pcm_format_e {
	/** Error Case */
	PCM_FORMAT_NONE = -1,
	/** Signed, 8-bit */
	PCM_FORMAT_S8 = 1,
	/** Signed 16-bit, little endian */
	PCM_FORMAT_S16_LE = 0,
	/** Signed, 16-bit, big endian */
	PCM_FORMAT_S16_BE = 2,
	/** Signed, 24-bit (32-bit in memory), little endian */
	PCM_FORMAT_S24_LE,
	/** Signed, 24-bit (32-bit in memory), big endian */
	PCM_FORMAT_S24_BE,
	/** Signed, 24-bit, little endian */
	PCM_FORMAT_S24_3LE,
	/** Signed, 24-bit, big endian */
	PCM_FORMAT_S24_3BE,
	/** Signed, 32-bit, little endian */
	PCM_FORMAT_S32_LE,
	/** Signed, 32-bit, big endian */
	PCM_FORMAT_S32_BE,
	/** Max of the enumeration list, not an actual format. */
	PCM_FORMAT_MAX
} pcm_format_t;

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
