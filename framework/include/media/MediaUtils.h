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
 * @cond
 * @internal
 * @file media/MediaUtils.h
 * @brief Media MediaUtils APIs
 * @endcond
 */

#ifndef __MEDIA_UTILS_H
#define __MEDIA_UTILS_H

#include <string>
#include <media/MediaTypes.h>

namespace media {
namespace utils {

/**
 * @cond
 * @internal
 * @brief Replace string with lowercase string.
 * @details @b #include <media/MediaUtils.h>
 * @param[out] str The str that lowercase string
 * @endcond
 */
void toLowerString(std::string& str);
/**
 * @cond
 * @internal
 * @brief Replace string with uppercase string.
 * @details @b #include <media/MediaUtils.h>
 * @param[out] str The str that uppercase string
 * @endcond
 */
void toUpperString(std::string& str);
/**
 * @cond
 * @internal
 * @brief Gets the audio type in path.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] path The path of audio data
 * @return The audio type
 * @endcond
 */
audio_type_t getAudioTypeFromPath(std::string path);
/**
 * @cond
 * @internal
 * @brief Gets the audio type from Mime-Type.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] mimeType The string of Mime-Type
 * @return The audio type
 * @endcond
 */
audio_type_t getAudioTypeFromMimeType(std::string &mimeType);
/**
 * @cond
 * @internal
 * @brief Parsing the audio type in file.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] audio file point, Audio type and channel, sample rate, pcm format address to receive.
 * @return true - parsing success. false - parsing fail.
 * @endcond
 */
bool file_header_parsing(FILE *fp, audio_type_t AudioType, unsigned int *channel, unsigned int *sample_rate, audio_format_type_t *pcmFormat);
/**
 * @cond
 * @internal
 * @brief Parsing the audio type in buffer.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] audio file buffer, buffer size, Audio type and channel, sample rate, pcm format address to receive.
 * @return true - parsing success. false - parsing fail.
 * @endcond
 */
bool buffer_header_parsing(const unsigned char *buffer, unsigned int bufferSize, audio_type_t audioType, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat);
/**
 * @cond
 * @internal
 * @brief Create a wav header in file.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] audio file point.
 * @return true - create success. false - create fail.
 * @endcond
 */
bool createWavHeader(FILE *fp);
/**
 * @cond
 * @internal
 * @brief Write a wav header in file.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] audio file point, Audio channel and sample rate, pcm format, file data size.
 * @return true - write success. false - write fail.
 * @endcond
 */
bool writeWavHeader(FILE *fp, unsigned int channel, unsigned int sampleRate, audio_format_type_t pcmFormat, unsigned int fileSize);
/*
 * @cond
 * @internal
 * @brief Split specified channels from an input audio stream into separated output buffers.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] layout: the channel layout of the input audio stream
 * @param[in] stream: pointer to the input audio stream
 * @param[in] frames: number of frames in input stream
 * @param[in] channels: number of channels to split from input audio stream
 * @param[in] ...: variable arguments in pairs <uint32_t, int16_t *> to tell channel masks and output buffers
 * @return channel masks to tell the channels split successfully
 * @endcond
 */
unsigned int splitChannel(unsigned int layout, const signed short *stream, unsigned int frames, unsigned int channels, ...);
/**
 * @cond
 * @internal
 * @brief Parsing the audio information from mp3 header buffer.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] header buffer pointer and channel, sample rate, frame length address to receive.
 * @return true - parsing success. false - parsing fail.
 * @endcond
 */
bool mp3_header_parsing(const unsigned char *header, unsigned int *channel, unsigned int *sampleRate, unsigned int *frameLength);
/**
 * @cond
 * @internal
 * @brief Parsing the audio information from aac header buffer.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] header buffer pointer and channel, sample rate, frame length address to receive.
 * @return true - parsing success. false - parsing fail.
 * @endcond
 */
bool aac_header_parsing(const unsigned char *header, unsigned int *channel, unsigned int *sampleRate, unsigned int *frameLength);
/**
 * @cond
 * @internal
 * @brief Parsing the audio information from wave header buffer.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] header buffer pointer and channel, sample rate, pcm format address to receive.
 * @return true - parsing success. false - parsing fail.
 * @endcond
 */
bool wave_header_parsing(const unsigned char *header, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat);
/**
 * @cond
 * @internal
 * @brief Gets the audio type from data stream.
 * @details @b #include <media/MediaUtils.h>
 * @param[in] buffer: Pointer to the data stream buffer.
 * @param[in] size: Size in bytes of data in the buffer.
 * @return The audio type.
 * @endcond
 */
audio_type_t getAudioTypeFromStream(const unsigned char *buffer, size_t size);
} // namespace utils
} // namespace media

#endif
/** @} */ // end of MEDIA group
