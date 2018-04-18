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

#include	<stdio.h>
#include	<stdint.h>
#include	<stdlib.h>
#include	<string.h>
#include	"samplerate.h"
#include	"common.h"

typedef struct {
	int16_t *fin;
	int16_t *fout;
	int size;
	int leftcnt;
	int lastused;
} SRC_INSTANCE;

#define RETURN_VAL_IF_FAIL(condition, val) \
	do {\
		if (!(condition))\
		{\
			return val;\
		}\
	} while (0)

#define GOTO_IF_FAIL(condition, tag) \
	do {\
		if (!(condition))\
		{\
			goto tag;\
		}\
	} while (0)

SRC_HANDLE src_init(int size)
{
	SRC_INSTANCE *src = (SRC_INSTANCE *)malloc(sizeof(SRC_INSTANCE));
	RETURN_VAL_IF_FAIL((src != NULL), NULL);

	src->fin = (int16_t *)malloc(size * sizeof(int16_t));
	GOTO_IF_FAIL((src->fin != NULL), fail_exit);

	src->fout = (int16_t *)malloc(size * sizeof(int16_t));
	GOTO_IF_FAIL((src->fout != NULL), fail_exit);

	src->size = size;
	src->lastused = 0;
	src->leftcnt = 0;
	return (SRC_HANDLE)src;

fail_exit:
	free(src->fin);
	free(src);
	return NULL;
}

int src_destroy(SRC_HANDLE handle)
{
	SRC_INSTANCE *src = (SRC_INSTANCE *)handle;
	RETURN_VAL_IF_FAIL((src != NULL), 1);

	free(src->fin);
	src->fin = NULL;

	free(src->fout);
	src->fout = NULL;

	free(src);
	return 1;
}

/**
 * @brief   Do convolution calculation for sample data
 * @remarks
 * @param   input: pointer to input samples buffer.
 * @param   coeff: pointer to coefficient array for convolution.
 * @param   num_samples: num of coefficients in array.
 * @param   channels_num: num of channels of input samples.
 * @return  value of convolution result.
 * @see clip()
 */
static int32_t fir_convolve_channles(const int16_t *input, const int32_t *coeff, int num_samples, int channels_num)
{
	int32_t sum = 1 << 13;
	for (int i = 0; i < num_samples; ++i) {
		sum += input[i * channels_num] * (coeff[i] >> 16);
	}
	return sum >> 14;
}

/**
 * @brief   Clip an integer value (32 bits) to a signed short type value(16 bits)
 * @remarks int16_t value in range [INT16_MIN, INT16_MAX], which is defined in <stdint.h>
 * @param   x: input 32 bits integer value.
 * @return  output 16 bits signed short value.
 */
static int16_t clip(int32_t x)
{
	if (x < INT16_MIN) {
		return INT16_MIN;
	} else if (x > INT16_MAX) {
		return INT16_MAX;
	} else {
		return x;
	}
}

/**
 * @brief   It handles samplerate up scaling in ratio 2.0/3.0... without fraction.
 * @remarks ratio 2.0/3.0..., e.g. 16K->32K, 16K->48K, 22.05K->44.1K...
 * @param   input: pointer to input samples buffer.
 * @param   output: pointer to output buffer, to store new samples of target sample rate.
 * @param   num_samples_in: number of samples in above input buffer.
 * @param   num_samples_out: number of samples will be stored in output buffer.
 * @param   quoti: quotient of target samplerate being divided by original samplerate.
 * @param   channels_num: num of channels of input samples.
 * @return  void
 * @see	upresample_int_channels()
 */
static void upresample_int_channels(const int16_t *input, int16_t *output, int num_samples_in, int num_samples_out, int quoti, int channels_num)
{
	(void)num_samples_in;
	const float step_float = (float)1 / (float)quoti;

	uint32_t in_sample_num = 0;   // 16.16 fixed point
	const uint32_t step = (uint32_t)(step_float * 65536.0f + 0.5f);  // 16.16 fixed point

	for (int j = 0; j < num_samples_out / channels_num; ++j, in_sample_num += step) {
		const uint32_t whole = in_sample_num >> 16;
		const uint32_t part = (in_sample_num & 0xffff);  // 0.16 fixed point
		for (int k = 0; k < channels_num; k++) {
			const int32_t s1 = input[whole * channels_num + k];
			const int32_t s2 = input[(whole + 1) * channels_num + k];
			*output++ = clip(s1 + (((s2 - s1) * (int32_t)part) >> 16));
		}
	}
}

/**
 * @brief   It handles samplerate up scaling in ratio 1.*,2.*... with fraction.
 * @remarks ratio 1.*,2.* ..., e.g. 16K->24K, 11.025->16K, 11.025->24K ...
 * @param   input: pointer to input samples buffer.
 * @param   output: pointer to output buffer, to store new samples of target sample rate.
 * @param   num_samples_in: number of samples in above input buffer.
 * @param   num_samples_out: number of samples will be stored in output buffer.
 * @param   frac: ratio of target samplerate being divided by original samplerate.
 * @param   channels_num: num of channels of input samples.
 * @return  void
 * @see upresample_int_channels()
 */
static void upresample_frac_channels(const int16_t *input, int16_t *output, int num_samples_in, int num_samples_out, int quoti, float frac, int channels_num)
{
	(void)num_samples_in;
	const float step_float = frac;
	uint32_t in_sample_num = 0;   // 16.16 fixed point
	const uint32_t step = (uint32_t)(step_float * 65536.0f + 0.5f);  // 16.16 fixed point
	for (int j = 0; j < num_samples_out / channels_num; ++j, in_sample_num += step) {
		const uint32_t whole = in_sample_num >> 16;
		const uint32_t part = (in_sample_num & 0xffff);  // 0.16 fixed point
		for (int k = 0; k < channels_num; k++) {
			const int32_t s1 = input[(whole / quoti) * channels_num + k];
			const int32_t s2 = input[((whole + 1) / quoti) * channels_num + k];
			*output++ = clip(s1 + (((s2 - s1) * (int32_t)part) >> 16));
		}
	}
}

/**
 * @brief   It handles samplerate down scaling in ratio 1.* with fraction.
 * @remarks ratio 1.*, e.g. 24K->16K, 16K->11.025K, ...
 * @param   input: pointer to input samples buffer.
 * @param   output: pointer to output buffer, to store new samples of target sample rate.
 * @param   num_samples_in: number of samples in above input buffer.
 * @param   num_samples_out: number of samples will be stored in output buffer.
 * @param   frac: ratio of orig samplerate being divided by target samplerate.
 * @param   channels_num: num of channels of input samples.
 * @return  void
 * @see
 */
static void downresample_frac_channels(const int16_t *input, int16_t *output, int num_samples_in, int num_samples_out, float frac, int channels_num)
{
	const float step_float = frac;
	uint32_t in_sample_num = 0;   // 16.16 fixed point
	const uint32_t step = (uint32_t)(step_float * 65536.0f + 0.5f);  // 16.16 fixed point
	for (int j = 0; j < num_samples_out / channels_num; ++j, in_sample_num += step) {
		const uint32_t whole = in_sample_num >> 16;
		const uint32_t part = (in_sample_num & 0xffff);  // 0.16 fixed point
		for (int k = 0; k < channels_num; k++) {
			const int32_t s1 = input[whole * channels_num + k];
			const int32_t s2 = input[(whole + 1) * channels_num + k];
			*output++ = clip(s1 + (((s2 - s1) * (int32_t)part) >> 16));
		}
	}
}

/**
 * @brief   It handles samplerate down scaling in ratio 2.0/3.0 without fraction.
 * @remarks ratio 2.0/3.0, e.g. 48K->16K, 48K->24K, 44.1K->22.05K...
 * @param   input: pointer to input samples buffer.
 * @param   output: pointer to output buffer, to store new samples of target sample rate.
 * @param   num_samples_in: number of samples in above input buffer.
 * @param   num_samples_out: number of samples will be stored in output buffer.
 * @param   quoti: quotient of orig samplerate being divided by target samplerate, it is the ratio mentioned above.
 * @param   channels_num: num of channels of input samples.
 * @return  void
 * @see	downresample_2_1_filter_frac_channels()
 */
static void resample_int_channels(const int16_t *input, int16_t *output, int num_samples_in, int num_samples_out, int quoti, int channels_num)
{
	(void)num_samples_out;
	for (int i = 0; i < num_samples_in; i += quoti) {
		output[i / quoti] = clip(fir_convolve_channles(input + i, filter_22khz_coeff, NUM_COEFF_22KHZ, channels_num));
	}
}

/**
 * @brief   It handles samplerate down scaling in special ratio.
 * @remarks The special ratio is 1.*, e.g. 44.1K->32K, 22.05K->16K, 11.025K->8K
 * @param   input: pointer to input samples buffer.
 * @param   output: pointer to output buffer, to store new samples of target sample rate.
 * @param   num_samples_in: number of samples in above input buffer.
 * @param   num_samples_out: number of samples will be stored in output buffer.
 * @param   channels_num: num of channels of input samples.
 * @return  void
 * @see
 */
static void resample_441_320_channels(int16_t *input, int16_t *output, int num_samples_in, int num_samples_out, int channels_num)
{
	const float step_float = (float)441 / (float)320;
	uint32_t in_sample_num = 0;   // 16.16 fixed point
	const uint32_t step = (uint32_t)(step_float * 65536.0f + 0.5f);  // 16.16 fixed point

	for (int i = 0; i < num_samples_in; ++i) {
		input[i] = fir_convolve_channles(input + i, filter_16khz_coeff, OVERLAP_16KHZ, channels_num);
	}

	for (int j = 0; j < num_samples_out / channels_num; ++j, in_sample_num += step) {
		const uint32_t whole = in_sample_num >> 16;
		const uint32_t part = (in_sample_num & 0xffff);  // 0.16 fixed point
		for (int k = 0; k < channels_num; k++) {
			const int32_t s1 = input[whole * channels_num + k];
			const int32_t s2 = input[(whole + 1) * channels_num + k];
			*output++ = clip(s1 + (((s2 - s1) * (int32_t)part) >> 16));
		}
	}
}

/**
 * @brief   It handles samplerate down scaling in ratio 2.* with fraction.
 * @remarks ratio 2.*, e.g. 48K->22.05K, 44.1K->16K, 24K->11.025K ...
 * @param   input: pointer to input samples buffer.
 * @param   output: pointer to output buffer, to store new samples of target sample rate.
 * @param   num_samples_in: number of samples in above input buffer.
 * @param   num_samples_out: number of samples will be stored in output buffer.
 * @param   frac: ratio of orig samplerate being divided by target samplerate, it has fraction value.
 * @param   channels_num: num of channels of input samples.
 * @return  void
 * @see	resample_int_channels()
 */
static void downresample_2_1_filter_frac_channels(int16_t *input, int16_t *output, int num_samples_in, int num_samples_out, float frac, int channels_num)
{
	const float step_float = frac;

	uint32_t in_sample_num = 0;   // 16.16 fixed point
	const uint32_t step = (uint32_t)(step_float * 65536.0f + 0.5f);  // 16.16 fixed point

	for (int i = 0; i < num_samples_in; ++i) {
		input[i] = fir_convolve_channles(input + i, filter_22khz_coeff, OVERLAP_22KHZ, channels_num);
	}

	for (int j = 0; j < num_samples_out / channels_num; ++j, in_sample_num += step) {
		const uint32_t whole = in_sample_num >> 16;
		const uint32_t part = (in_sample_num & 0xffff);  // 0.16 fixed point
		for (int k = 0; k < channels_num; k++) {
			const int32_t s1 = input[whole * channels_num + k];
			const int32_t s2 = input[(whole + 1) * channels_num + k];
			*output++ = clip(s1 + (((s2 - s1) * (int32_t)part) >> 16));
		}
	}
}

int src_is_valid_ratio(float ratio)
{
	if ((ratio <= 3) && (ratio >= (float)1 / (float)3)) {
		return 1;
	} else {
		return 0;
	}
}

int src_simple(SRC_HANDLE handle, SRC_DATA *src_data)
{
	int src_type = SRC_TYPE_INVALID;
	SRC_INSTANCE *src = (SRC_INSTANCE *)handle;
	RETURN_VAL_IF_FAIL((src != NULL && src_data != NULL), 0);

	src_data->src_ratio = (float)src_data->old_samples / (float)src_data->new_samples;
	if (src_is_valid_ratio(src_data->src_ratio) == 0) {
		return 0;
	}

	if (src_data->old_samples == src_data->new_samples) {
		// same rate, just copy data
		src_type = SRC_TYPE_NO_RESAMPLE;

		src_data->output_frames_gen = src_data->input_frames;
		src_data->input_frames_used = src_data->input_frames;
		for (int i = 0; i < src_data->input_frames; i++) {
			src->fout[i] = src_data->data_in[i];
		}
	} else {
		int channels_num = src_data->channels_num;
		int total_frames = 0; // frames in total

		// move remaining frames
		for (total_frames = 0; total_frames < src->leftcnt; total_frames++) {
			src->fin[total_frames] = src->fin[src->lastused + total_frames];
		}

		// accept input frames as much as possible
		src_data->input_frames_used = MIN(src_data->input_frames, src->size - total_frames);

		// append new input frames
		for (int j = 0; j < src_data->input_frames_used; j++, total_frames++) {
			src->fin[total_frames] = src_data->data_in[j];
		}

		// resample ...
		if (src_data->old_samples > src_data->new_samples) {
			// down resample
			int quotient = src_data->old_samples / src_data->new_samples;
			int remainer = src_data->old_samples % src_data->new_samples;

			if (quotient >= 2) {
				if (remainer == 0) {	// ratio 2.0/3.0 :: 48K->16K, 48K->24K, 44.1K->22.05K, ...
					src_type = SRC_TYPE_DOWN_INT;

					int blocks = (total_frames - OVERLAP_22KHZ * channels_num) / (quotient * channels_num);
					src->lastused = blocks * (quotient * channels_num);
					src->leftcnt = total_frames - src->lastused;

					src_data->output_frames_gen = src->lastused / quotient;
					resample_int_channels(src->fin, src->fout, src->lastused, src_data->output_frames_gen, quotient, channels_num);
				} else {	// ratio 2.* :: 48K->22.05K, 44.1K->16K, 24K->11.025K, ...
					src->leftcnt = OVERLAP_22KHZ * channels_num;
					src->lastused = total_frames - src->leftcnt;

					src_data->output_frames_gen = (int)((float)(src->lastused / channels_num) * ((float)src_data->new_samples / (float)src_data->old_samples) + 0.5) * channels_num;
					downresample_2_1_filter_frac_channels(src->fin, src->fout, src->lastused, src_data->output_frames_gen, (float)((float)src_data->old_samples / (float)src_data->new_samples), channels_num);
				}
			} else if (src_data->old_samples * 320 == src_data->new_samples * 441) { // ratio 1.* :: 44.1K->32K, 22.05K->16K, 11.025K->8K
				src->leftcnt = OVERLAP_16KHZ * channels_num;
				src->lastused = total_frames - src->leftcnt;

				src_data->output_frames_gen = (int)((float)(src->lastused / channels_num) * ((float)src_data->new_samples / (float)src_data->old_samples) + 0.5) * channels_num;
				resample_441_320_channels(src->fin, src->fout, src->lastused, src_data->output_frames_gen, channels_num);
			} else { // other ratio 1.* :: 24K->16K, 16K->11.025K, ...
				src->leftcnt = 1 * channels_num;
				src->lastused = total_frames - src->leftcnt;

				src_data->output_frames_gen = (int)((float)(src->lastused / channels_num) * ((float)src_data->new_samples / (float)src_data->old_samples) + 0.5) * channels_num;
				downresample_frac_channels(src->fin, src->fout, src->lastused, src_data->output_frames_gen, (float)((float)src_data->old_samples / (float)src_data->new_samples), channels_num);
			}
		} else {
			// up resample
			int quotient = src_data->new_samples / src_data->old_samples;
			int remainer = src_data->new_samples % src_data->old_samples;
			if (remainer == 0) { // ratio 2.0/3.0 :: ...
				src_type = SRC_TYPE_UP_INT;

				src->leftcnt = 1 * channels_num;
				src->lastused = total_frames - src->leftcnt;

				src_data->output_frames_gen = src->lastused * quotient;
				upresample_int_channels(src->fin, src->fout, src->lastused, src_data->output_frames_gen, quotient, channels_num);
			} else {	// ratio 1.*/2.* :: ...
				src_type = SRC_TYPE_UP_FRAC;

				src->leftcnt = 1 * channels_num;
				src->lastused = total_frames - src->leftcnt;

				float frac = (float)((quotient + 1) * src_data->old_samples) / (float) src_data->new_samples;
				src_data->output_frames_gen = (int)((float)((src->lastused / channels_num) * (quotient + 1)) / (float)frac + 0.5) * channels_num;
				upresample_frac_channels(src->fin, src->fout, src->lastused, src_data->output_frames_gen, quotient + 1, frac, channels_num);
			}
		}
	}

	(void)src_type;
	src_data->data_out = src->fout;
	return 1;
}

int src_StereoToMono(short *data, int len)
{
	int index = 0;
	for (index = 0; index < (len / 2); index++) {
		data[index] = (data[(index << 1) + 1] + data[index << 1]) / 2;
	}

	return index;
}

int src_MonoToStereo(short *data, int len)
{
	int index = 0;
	for (index = len - 1; index >= 0; index--) {
		data[(index << 1) + 1] = data[index];
		data[index << 1]   = data[index];
	}

	return len << 1;
}




