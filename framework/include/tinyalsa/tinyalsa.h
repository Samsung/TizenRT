/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/* tinyalsa.h
**
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
 * @defgroup TinyAlsa TinyAlsa
 * @ingroup TinyAlsa
 * @brief Provides APIs for Audio Framework
 * @{
 */

/**
 * @file tinyalsa.h
 * @brief All macros, structures and functions that make up the PCM interface.
 */

#ifndef __AUDIO_TINYALSA_H
#define __AUDIO_TINYALSA_H

#include <sys/time.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

/** A flag that specifies that the PCM is an output.
 * May not be bitwise AND'd with @ref PCM_IN.
 * Used in @ref pcm_open.
 * @ingroup libtinyalsa-pcm
 */
#define PCM_OUT 0x00000010

/** Specifies that the PCM is an input.
 * May not be bitwise AND'd with @ref PCM_OUT.
 * Used in @ref pcm_open.
 * @ingroup libtinyalsa-pcm
 */
#define PCM_IN 0x00000020

/** Specifies that the PCM will use mmap read and write methods.
 * Used in @ref pcm_open.
 * @ingroup libtinyalsa-pcm
 */
#define PCM_MMAP 0x00000001

/** Specifies no interrupt requests.
 * May only be bitwise AND'd with @ref PCM_MMAP.
 * Used in @ref pcm_open.
 * @ingroup libtinyalsa-pcm
 */
#define PCM_NOIRQ 0x00000002

/** When set, calls to @ref pcm_write
 * for a playback stream will not attempt
 * to restart the stream in the case of an
 * underflow, but will return -EPIPE instead.
 * After the first -EPIPE error, the stream
 * is considered to be stopped, and a second
 * call to pcm_write will attempt to restart
 * the stream.
 * Used in @ref pcm_open.
 * @ingroup libtinyalsa-pcm
 */
#define PCM_NORESTART 0x00000004

/** For inputs, this means the PCM is recording audio samples.
 * For outputs, this means the PCM is playing audio samples.
 * @ingroup libtinyalsa-pcm
 */
#define	PCM_STATE_RUNNING	0x03

/** For inputs, this means an overrun occured.
 * For outputs, this means an underrun occured.
 */
#define	PCM_STATE_XRUN 0x04

/** For outputs, this means audio samples are played.
 * A PCM is in a draining state when it is coming to a stop.
 */
#define	PCM_STATE_DRAINING 0x05

/** Means a PCM is suspended.
 * @ingroup libtinyalsa-pcm
 */
#define	PCM_STATE_SUSPENDED 0x07

/** Means a PCM has been disconnected.
 * @ingroup libtinyalsa-pcm
 */
#define	PCM_STATE_DISCONNECTED 0x08

/**
 * @brief Audio sample format of a PCM.
 * @details The first letter specifiers whether the sample is signed or unsigned.
 * The letter 'S' means signed. The letter 'U' means unsigned.
 * The following number is the amount of bits that the sample occupies in memory.
 * Following the underscore, specifiers whether the sample is big endian or little endian.
 * The letters 'LE' mean little endian.
 * The letters 'BE' mean big endian.
 * This enumeration is used in the @ref pcm_config structure.
 * @ingroup libtinyalsa-pcm
 */
enum pcm_format {
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
};

/**
 * @brief Encapsulates the hardware and software parameters of a PCM.
 * @ingroup libtinyalsa-pcm
 */
struct pcm_config {
	/** The number of channels in a frame */
	unsigned int channels;
	/** The number of frames per second */
	unsigned int rate;
	/** The sample format of a PCM */
	enum pcm_format format;
	/** The number of frames in a period */
	unsigned int period_size;
	/** The number of periods in a PCM */
	unsigned int period_count;
};

struct pcm;

/**
 * @brief Opens a PCM for playback or recording.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] card   The card that the pcm belongs to.
 * @param[in] device The device that the pcm belongs to.
 * @param[in] flags  Specify characteristics and functionality about the pcm.
 * @param[in] config The hardware and software parameters to open the PCM with.
 * @returns On success, A PCM structure returned. On failure, invalid PCM structure returned.
 * @since TizenRT v1.1
 */
struct pcm *pcm_open(unsigned int card, unsigned int device, unsigned int flags, const struct pcm_config *config);

/**
 * @brief Opens a PCM by it's name.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] name   The name of the PCM.
 * @param[in] flags  Specify characteristics and functionality about the pcm.
 * @param[in] config The hardware and software parameters to open the PCM with.
 * @returns On success, A PCM structure returned. On failure, NULL or invalid PCM structure returned.
 * @since TizenRT v1.1
 */
struct pcm *pcm_open_by_name(const char *name, unsigned int flags, const struct pcm_config *config);

/**
 * @brief Closes a PCM returned by @ref pcm_open.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm A PCM returned by pcm_open.
 * @return Always returns 0.
 * @since TizenRT v1.1
 */
int pcm_close(struct pcm *pcm);

/**
 * @brief Checks if a PCM file has been opened without error.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm A PCM handle.
 * @return On success, 0 returned. On failure, 1 returned
 * @since TizenRT v1.1
 */
int pcm_is_ready(const struct pcm *pcm);

/**
 * @brief Gets the channel count of the PCM.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm A PCM handle.
 * @return The channel count of the PCM.
 * @since TizenRT v1.1
 */
unsigned int pcm_get_channels(const struct pcm *pcm);

/**
 * @brief Gets the PCM configuration.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm A PCM handle.
 * @return On success, The PCM configuration returned. On failure, NULL returned.
 * @since TizenRT v1.1
 */
const struct pcm_config *pcm_get_config(const struct pcm *pcm);

/**
 * @brief Gets the sample rate of the PCM.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm A PCM handle.
 * @return The rate of the PCM.
 * @since TizenRT v1.1
 */
unsigned int pcm_get_rate(const struct pcm *pcm);

/**
 * @brief Gets the format of the PCM.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm A PCM handle.
 * @return The format of the PCM.
 * @since TizenRT v1.1
 */
enum pcm_format pcm_get_format(const struct pcm *pcm);

/**
 * @brief Gets the file descriptor of the PCM.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm A PCM handle.
 * @return The file descriptor of the PCM.
 * @since TizenRT v1.1
 */
int pcm_get_file_descriptor(const struct pcm *pcm);

/**
 * @brief Gets the error message for the last error that occured.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm A PCM handle.
 * @return The error message of the last error that occured.
 * @since TizenRT v1.1
 */
const char *pcm_get_error(const struct pcm *pcm);

/**
 * @brief Gets the buffer size of the PCM.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm A PCM handle.
 * @return The buffer size of the PCM.
 * @since TizenRT v1.1
 */
unsigned int pcm_get_buffer_size(const struct pcm *pcm);

/**
 * @brief Determines how many bytes are occupied by a number of frames of a PCM.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm    A PCM handle.
 * @param[in] frames The number of frames of a PCM.
 * @return The bytes occupied by frames.
 * @since TizenRT v1.1
 */
unsigned int pcm_frames_to_bytes(const struct pcm *pcm, unsigned int frames);

/**
 * @brief Determines how many frames of a PCM can fit into a number of bytes.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm   A PCM handle.
 * @param[in] bytes The number of bytes.
 * @return The number of frames that may fit into @p bytes
 * @since TizenRT v1.1
 */
unsigned int pcm_bytes_to_frames(const struct pcm *pcm, unsigned int bytes);

/**
 * @brief Writes audio samples to PCM.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[out] pcm         A PCM handle.
 * @param[in]  data        The audio sample array
 * @param[in]  frame_count The number of frames occupied by the sample array.
 * @return On success, written number of frames returned. On failure, a negative number returned.
 * @since TizenRT v1.1
 */
int pcm_writei(struct pcm *pcm, const void *data, unsigned int frame_count);

/**
 * @brief Reads audio samples from PCM.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[out] pcm         A PCM handle.
 * @param[out] data        The audio sample array which will contain the audio data recieved from the input stream
 * @param[in]  frame_count The number of frames that the user wants to read
 * @return On success, number of frames read returned. On failure, a negative number returned.
 * @since TizenRT v1.1
 */
int pcm_readi(struct pcm *pcm, void *data, unsigned int frame_count);

/**
* @brief Stops a PCM. Any data present in the buffers will be dropped.
*
* @details @b #include <tinyalsa/tinyalsa.h>
* @param[in] pcm A PCM handle.
* @returns On success, zero; on failure, a negative number.
* @since TizenRT v2.0
*/
int pcm_drop(struct pcm *pcm);

/**
* @brief Stop a PCM preserving pending frames.
*
* @details @b #include <tinyalsa/tinyalsa.h>
* @param[in] pcm A PCM handle.
* @returns On success, zero; on failure, a negative number.
* @since TizenRT v2.0
*/
int pcm_drain(struct pcm *pcm);

/**
 * @brief Prepares a PCM, if it has not been prepared already.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm A PCM handle.
 * @return On success, 0 returned. On failure, a negative number returned.
 * @since TizenRT v1.1
 */
int pcm_prepare(struct pcm *pcm);

/**
 * @brief Determines the number of bits occupied by a @ref pcm_format.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] format A PCM format
 * @return The number of bits associated with @p format
 * @since TizenRT v1.1
 */
unsigned int pcm_format_to_bits(enum pcm_format format);

/**
 * @brief Gets the subdevice on which the pcm has been opened.
 *
 * @details @b #include <tinyalsa/tinyalsa.h>
 * @param[in] pcm A PCM handle.
 * @return The subdevice on which the pcm has been opened.
 * @since TizenRT v1.1
 */
unsigned int pcm_get_subdevice(const struct pcm *pcm);

/**
* @brief Writes audio samples to the PCM using the mmap buffer
*
* @details @b #include <tinyalsa/tinyalsa.h>
* @param[in] pcm A PCM handle
* @param[in] data Pointer to the buffer containing data to be written
* @param[in] count Size of the data in bytes
* @returns On success, written number of frames returned. On failure, a negative number returned
* @since TizenRT v2.0
*/
int pcm_mmap_write(struct pcm *pcm, const void *data, unsigned int count);

/**
* @brief Reads audio samples from the PCM using the mmap buffer
*
* @details @b #include <tinyalsa/tinyalsa.h>
* @param[in] pcm A PCM handle
* @param[in] data Pointer to the buffer to read into
* @param[in] count Size of the buffer in bytes
* @returns On success, read number of frames returned. On failure, a negative number returned
* @since TizenRT v2.0
*/
int pcm_mmap_read(struct pcm *pcm, void *data, unsigned int count);

/**
* @brief Application request to access a portion of direct (mmap) area
*
* @details @b #include <tinyalsa/tinyalsa.h>
* @param[in] pcm A PCM handle
* @param[out] areas Returned mmap channel areas
* @param[out] offset Returned mmap area offset in frames
* @param[out] frames Mmap area portion size in frames
* @returns On success, zero is returned. On failure, a negative number returned
* @since TizenRT v2.0
*/
int pcm_mmap_begin(struct pcm *pcm, void **areas, unsigned int *offset, unsigned int *frames);

/**
* @brief Application has completed the access to area requested with pcm_mmap_begin
*
* @details @b #include <tinyalsa/tinyalsa.h>
* @param[in] pcm A PCM handle
* @param[in] offset Area offset in frames. This must be same as the offset returned by pcm_mmap_begin
* @param[in] frames Mmap area portion size in frames that application wishes to commit
* @returns On success, zero is returned. On failure, a negative number returned
* @since TizenRT v2.0
*/
int pcm_mmap_commit(struct pcm *pcm, unsigned int offset, unsigned int frames);

/**
* @brief Waits for buffer to become available for mmap access
*
* @details @b #include <tinyalsa/tinyalsa.h>
* @param[in] pcm A PCM handle
* @param[in] timeout Maximum time in milliseconds to wait, a negative value means infinity
* @returns On success, one is returned. On timeout, zero is returned. On failure, a negative number returned
* @since TizenRT v2.0
*/
int pcm_wait(struct pcm *pcm, int timeout);

/**
* @brief Returns the number of frames ready to be written or read
*
* @details @b #include <tinyalsa/tinyalsa.h>
* @param[in] pcm A PCM handle
* @returns On success, positive number is returned. On failure, a negative number returned
* @since TizenRT v2.0
*/
int pcm_avail_update(struct pcm *pcm);
#if defined(__cplusplus)
}								/* extern "C" */
#endif
/** @} */// end of TinyAlsa group
#endif
