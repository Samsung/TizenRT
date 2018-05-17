/******************************************************************
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

/*
** Copyright (c) 2002-2016, Erik de Castro Lopo <erikd@mega-nerd.com>
** All rights reserved.
**
** This code is released under 2-clause BSD license. Please see the
** file at : https://github.com/erikd/libsamplerate/blob/master/COPYING
*/

/*
** API documentation is available here:
**     http://www.mega-nerd.com/SRC/api.html
*/

#include <stdint.h>
#include <stdbool.h>

#ifndef SAMPLERATE_H
#define SAMPLERATE_H
#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/****************************************************************************
 * Public Data
 ****************************************************************************/
/**
 * @enum  Define error codes.
 * @brief SRC_ERR_NO_ERROR means success, others mean failure.
 */
enum {
	SRC_ERR_UNKNOWN = -1,
	SRC_ERR_NO_ERROR = 0,
	SRC_ERR_BAD_PARAMS,
	SRC_ERR_MALLOC_FAILED,
	SRC_ERR_NOT_SUPPORT,
	SRC_ERR_BAD_SRC_RATIO,
	SRC_ERR_BAD_CHANNEL_COUNT,

	/* This must be the last error number. */
	SRC_ERR_MAX_ERROR
};

/**
 * @enum  Define sample width types, the value means the bits per sample.
 * @brief Currently, we only support SAMPLE_WIDTH_16BITS input&output.
 */
enum {
	SAMPLE_WIDTH_8BITS = 8,
	SAMPLE_WIDTH_16BITS = 16,
	SAMPLE_WIDTH_24BITS = 24,
	SAMPLE_WIDTH_32BITS = 32,
	SAMPLE_WIDTH_MAX = SAMPLE_WIDTH_32BITS,
};

/**
 * @typedef src_handle_t, SRC(Sample Rate Convertor) hanlde type declaration.
 * @brief   NULL means invalid handle.
 */
typedef void *src_handle_t;

/**
 * @structure src_data_t
 * @brief     structure type for sample rate conversion
 *            Each sample data take 16bits.
 *            src_ratio should be in a range, see src_is_valid_ratio()
 */
struct src_data_s {
	/* input params */
	const void *data_in;        // pointer to input sample frames buffer (user buffer)
	int input_frames;           // number of input frames
	int channels_num;           // number of channels (1: mono, 2: stereo)
	int origin_sample_rate;     // original sample rate
	int origin_sample_width;    // original sample width (bits per sample), only support SAMPLE_WIDTH_16BITS now.
	void *data_out;             // pointer to output sample buffer (user buffer)
	int out_buf_length;         // length of data_out buffer (bytes)

	/* input params - specify desired output */
	int desired_sample_rate;    // target sample rate
	int desired_sample_width;   // target sample width (bits per sample), only support SAMPLE_WIDTH_16BITS now.

	/* output */
	int output_frames_gen;      // number of output frames generated
	int input_frames_used;      // number of frames used from input frames buffer
	float src_ratio;            // desired_sample_rate/origin_sample_rate;
};

typedef struct src_data_s src_data_t;


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief   SRC converting function
 * @remarks Sample Rate Converted synchronously in this function.
 * @param   handle: pointer to a SRC instance, returned by src_init().
 * @param   data: pointer to object of struture src_data_t, see detail description above.
 * @return  0 on success, otherwise, it means failure.
 * @see     src_data_t
 */
int src_simple(src_handle_t handle, src_data_t *data);

/**
 * @brief   SRC (Sample Rate Convertor) initialize.
 * @remarks Internal buffer will be allocated in src_simple().
 * @param   size: buffer size in bytes, it's better to be greater than the input
 *          sample buffer size in src_data passed to src_simple().
 * @return  SRC handle, if NULL, it means failure.
 * @see     src_destroy()
 */
src_handle_t src_init(int size);

/**
 * @brief   Release buffers allocated by SRC in src_init().
 * @remarks This function must be called in pairs with src_init(), to avoid mem leak.
 * @param   handle: pointer to a SRC instance, returned by src_init().
 * @return  0 on success, otherwise, it means failure.
 * @see     src_init()
 */
int src_destroy(src_handle_t handle);

/**
 * @brief   Interface for converting samples from stereo channel to mono channel.
 * @remarks Used for interlaced 16 bit PCM format, storing samples as: LRLRLR...
 * @param   samples: pointer to input sample data buffer, converted sample data
 *          will also be stored in this buffer, overwrite original datas.
 * @param   size: number of samples of stereo channel stored in 'samples' buffer.
 * @return  number of samples of mono channel in 'samples' buffer.
 * @see
 */
int src_StereoToMono(short *samples, int size);

/**
 * @brief   Interface for converting samples from mono channel to stereo channel.
 * @remarks Used for interlaced 16 bit PCM format, storing samples as: LRLRLR...
 * @param   samples: pointer to input sample data buffer, converted sample data
 *          will also be stored in this buffer, overwrite original data. So there
 *          should be enough (double 'size') space in 'samples' buffer!
 * @param   size: number of samples of mono channel stored in 'samples' buffer.
 * @return  number of samples of stereo channel in 'samples' buffer.
 * @see
 */
int src_MonoToStereo(short *samples, int size);

/**
 * @brief   Check if the conversion ratio is valid or not.
 * @remarks To provide high quality SRC, conversion ratio is limited in a range.
 * @param   ratio: calculating formula is original_samplerate/target_samplerate.
 *          currently, ratio in range [1/3, 3] is valid, it may be changed in future.
 *          so, e.g. it's not possible to resample from 48KHz to 8KHz.
 * @return  true if it's valid, otherwise, returns false.
 * @see
 */
bool src_is_valid_ratio(float ratio);

#ifdef __cplusplus
}		/* extern "C" */
#endif	/* __cplusplus */

#endif	/* SAMPLERATE_H */

