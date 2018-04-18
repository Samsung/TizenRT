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

#ifndef SAMPLERATE_H
#define SAMPLERATE_H

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/**
 * @typedef SRC_HANDLE, SRC(Sample Rate Convertor) hanlde type declaration.
 * @brief   if NULL means invalid handle.
 */
typedef void *SRC_HANDLE;

/**
 * @structure SRC_DATA
 * @brief	 structure type for sample rate conversion
 * 			 Each sample data take 16bits.
 *			 src_ratio (old_samples/new_samples) should in a range, see src_is_valid_ratio()
 */
typedef struct {
	/* input */
	const int16_t *data_in; // pointer to input sample buffer
	int input_frames;		// number of input samples (of all channels)
	int channels_num;		// number of channels (1: mono, 2: stereo)
	int old_samples;		// original sample rate
	int new_samples;		// target sample rate

	/* output */
	int16_t	*data_out;		// pointer to output sample buffer
	int output_frames_gen;	// number of output samples (of all channels)
	int input_frames_used;	// number of samples used from input frames buffer
	float src_ratio; 		// old_samples/new_samples;
} SRC_DATA;


/**
 * @brief   SRC converting function
 * @remarks Sample Rate Converted synchronously in this function.
 * @param   handle: pointer to a SRC instance, returned by src_init().
 * @param   data: pointer to object of struture SRC_DATA, see detail description above.
 * @return  1 on success, otherwise, it means failure.
 * @see 	structure SRC_DATA
 */
int src_simple(SRC_HANDLE handle, SRC_DATA *data) ;

/**
 * @brief   SRC (Sample Rate Convertor) initialize.
 * @remarks Buffer of (size*sizeof(int16_t)*2) bytes will be allocated in this function.
 * @param   size: buffer size in 'signed short' specified for SRC.
 * @return  SRC handle, if NULL, it means failure.
 * @see 	src_destroy()
 */
SRC_HANDLE src_init(int size);

/**
 * @brief   Release buffers allocated by SRC in src_init().
 * @remarks This function must be called in pairs with src_init(), to avoid mem leak.
 * @param   handle: pointer to a SRC instance, returned by src_init().
 * @return  1 on success, otherwise, it means failure.
 * @see 	src_init()
 */
int src_destroy(SRC_HANDLE handle);

/**
 * @brief   Interface for converting samples from stereo channel to mono channel.
 * @remarks Used for 16 bit PCM.
 * @param   samples: pointer to input sample data buffer, converted sample data
 *          will also be stored in this buffer, overwrite original datas.
 * @param   size: number of samples of stero channel stored in 'samples' buffer.
 * @return  number of samples of mono channel in 'samples' buffer.
 * @see
 */
int src_StereoToMono(short *samples, int size);

/**
 * @brief   Interface for converting samples from mono channel to stereo channel.
 * @remarks Used for 16 bit PCM.
 * @param   samples: pointer to input sample data buffer, converted sample data
 *          will also be stored in this buffer, overwrite original data. So there
 *          should be enough (double 'size') space in 'samples' buffer!
 * @param   size: number of samples of stero channel stored in 'samples' buffer.
 * @return  number of samples of sterio channel in 'samples' buffer.
 * @see
 */
int src_MonoToStereo(short *samples, int size);

/**
 * @brief   Check the conversion ratio is valid or not.
 * @remarks To provide high quality SRC, conversion ratio is limited in a range.
 * @param   ratio: calculating formula is original_samplerate/target_samplerate.
 *          currently, ratio in range [1/3, 3] is valid, it may be changed in future.
 *          so, e.g. it's not possible to resample from 48KHz to 8KHz.
 * @return  1 if it's valid, otherwise, returns 0.
 * @see
 */
int src_is_valid_ratio(float ratio) ;

#ifdef __cplusplus
}		/* extern "C" */
#endif	/* __cplusplus */

#endif	/* SAMPLERATE_H */

