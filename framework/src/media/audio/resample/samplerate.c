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
#include	<math.h>
#include	"samplerate.h"


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
// Range of ratio supported for sample rate conversion
#define SRC_MAX_RATIO   ((float)3)
#define SRC_MIN_RATIO   ((float)1 / SRC_MAX_RATIO)

#define MAXIMUM(a, b)   (((a) > (b)) ? (a) : (b))
#define MINIMUM(a, b)   (((a) < (b)) ? (a) : (b))

// Fraction part bits
#define FRACBITS            (16)

// Convert to 16.16 fixed point value
#define TO_16_16_FIXED(x)   ((uint32_t)((float)(x) * (float)(1 << FRACBITS) + 0.5f))

// Int part value: 16.0 fixed point
#define INTPART_VALUE(x)    ((x) >> FRACBITS)

// Fraction part value: 0.16 fixed point
#define FRACPART_VALUE(x)   ((x) & 0xffff)

// Calculate new sample data
#define CALC_NEW_SAMPLE(s1, s2, part) ((s1) + ((((s2) - (s1)) * (int32_t)(part)) >> 16))

// Convert sample width in bytes
#define BYTES_PER_SAMPLE(bits_per_sample)   ((bits_per_sample) >> 3)

// Round the given positive float number to the nearest integer
#define LRINTPF(pf) ((long)((float)(pf) + 0.5f))

// Max channel num supported for SRC
#define SRC_MAX_CH  (2)

// Down resample ratio for 44.1k->32k, 22.05k->16k and 11.025k->8k
#define DOWN_RESAMPLE_441_320_RATIO ((float)441 / (float)320)

#define FLOAT_ACCURACY      (0.000001f)
#define FLOAT_EQUAL(a, b)   (fabsf((a)-(b)) < FLOAT_ACCURACY)

#define NUM_COEFF_16KHZ (sizeof(filter_16khz_coeff) / sizeof(filter_16khz_coeff[0]))
#define OVERLAP_16KHZ   (NUM_COEFF_16KHZ - 1)

#define NUM_COEFF_22KHZ (sizeof(filter_22khz_coeff) / sizeof(filter_22khz_coeff[0]))
#define OVERLAP_22KHZ   (NUM_COEFF_22KHZ - 2)

// At least remain one frame (one sample for each channel)
#define OVERLAP_DEFAULT (1)

#define RETURN_VAL_IF_FAIL(condition, val) \
	do { \
		if (!(condition)) { \
			return val; \
		} \
	} while (0)


/****************************************************************************
 * Private Declarations
 ****************************************************************************/
/**
 * @structure resampler_s: main structure used for SRC, it contains context
 *            variables used between src_simple() calls.
 * @brief It's internal structure, user can only get the handler via src_init().
 */
struct resampler_s {
	int16_t *in_buffer;     // internal input buffer
	int16_t *out_buffer;    // pointer of the external output buffer
	int size_in_bytes;      // input buffer size in bytes
	int size_in_frames;     // input buffer size in frames
	int outsize_in_frames;  // output buffer size in frames
	int left_frames;        // number of frames remained in input buffer
	int used_frames;        // number of frames used at last time src_simple()
	int channels_num;       // number of channels of input PCM samples
	float lastratio;        // memorize last sample rate coversion ratio
	int lastoldformat;      // memorize last origin sample width(format)
	int lastnewformat;      // memorize last desired sample width(format)
};

typedef struct resampler_s resampler_t;

/**
 * 16.16 fixed point FIR filter coefficients for conversion 44100 -> 32000,
 * 22050 -> 16000, or 11025 -> 8000.
 */
static const int32_t filter_16khz_coeff[] = {
	2057290, -2973608, 1880478, 4362037,
	-14639744, 18523609, -1609189, -38502470,
	78073125, -68353935, -59103896, 617555440,
	617555440, -59103896, -68353935, 78073125,
	-38502470, -1609189, 18523609, -14639744,
	4362037, 1880478, -2973608, 2057290,
};

/**
 * 16.16 fixed point FIR filter coefficients for conversion 44100 -> 22050.
 * (Works equivalently for 22010 -> 11025 or any other halving, of course.)
 */
static const int32_t filter_22khz_coeff[] = {
	2089257, 2898328, -5820678, -10484531,
	19038724, 30542725, -50469415, -81505260,
	152544464, 478517512, 478517512, 152544464,
	-81505260, -50469415, 30542725, 19038724,
	-10484531, -5820678, 2898328, 2089257,
};


/****************************************************************************
 * Private Functions
 ****************************************************************************/
/**
 * @brief   Do convolution calculation for sample data
 * @remarks FIR: Finite Impulse Response
 * @param   input: pointer to input samples buffer.
 * @param   coeff: pointer to coefficient array for convolution.
 * @param   num_samples: num of coefficients in array.
 * @param   channels_num: num of channels of input samples.
 * @return  value of convolution result.
 * @see
 */
static int32_t fir_convolve(const int16_t *input, const int32_t *coeff, int32_t num_samples, int32_t channels_num)
{
	int32_t sum = 1 << 13;
	int32_t i;
	for (i = 0; i < num_samples; ++i) {
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
	}

	return x;
}

/**
 * @brief   It handles samplerate up scaling in ratio 2.0/3.0... without fraction.
 * @remarks ratio 2.0/3.0..., e.g. 16K->32K, 16K->48K, 22.05K->44.1K...
 * @param   src: pointer to resampler object.
 * @param   num_frames_out: number of frames will be stored in output buffer.
 * @param   quoti: quotient of target samplerate being divided by original samplerate.
 * @return  void
 * @see     upresample_frac()
 */
static void upresample_int(resampler_t *src, int32_t num_frames_out, int32_t quoti)
{
	const int16_t *input = src->in_buffer;
	int16_t *output = src->out_buffer;
	int32_t channels_num = src->channels_num;
	float step_float = (float)1 / (float)quoti;
	uint32_t step = TO_16_16_FIXED(step_float);
	uint32_t fp_index = 0;   // 16.16 fixed point value

	int32_t j;
	for (j = 0; j < num_frames_out; ++j, fp_index += step) {
		uint32_t whole = INTPART_VALUE(fp_index);
		uint32_t part = FRACPART_VALUE(fp_index);
		int32_t k;
		for (k = 0; k < channels_num; k++) {
			int32_t s1 = input[whole * channels_num + k];
			int32_t s2 = input[(whole + 1) * channels_num + k];
			*output++ = clip(CALC_NEW_SAMPLE(s1, s2, part));
		}
	}
}

/**
 * @brief   It handles samplerate up scaling in ratio 1.*,2.*... with fraction.
 * @remarks ratio 1.*,2.* ..., e.g. 16K->24K, 11.025->16K, 11.025->24K ...
 * @param   src: pointer to resampler object.
 * @param   num_frames_out: number of frames will be stored in output buffer.
 * @param   frac: ratio of target samplerate being divided by original samplerate.
 * @return  void
 * @see     upresample_int()
 */
static void upresample_frac(resampler_t *src, int32_t num_frames_out, int32_t quoti, float frac)
{
	const int16_t *input = src->in_buffer;
	int16_t *output = src->out_buffer;
	int32_t channels_num = src->channels_num;
	float step_float = frac;
	uint32_t step = TO_16_16_FIXED(step_float);
	uint32_t fp_index = 0;   // 16.16 fixed point value

	int32_t j;
	for (j = 0; j < num_frames_out; ++j, fp_index += step) {
		uint32_t whole = INTPART_VALUE(fp_index);
		uint32_t part = FRACPART_VALUE(fp_index);
		int32_t k;
		for (k = 0; k < channels_num; k++) {
			int32_t s1 = input[(whole / quoti) * channels_num + k];
			int32_t s2 = input[((whole + 1) / quoti) * channels_num + k];
			*output++ = clip(CALC_NEW_SAMPLE(s1, s2, part));
		}
	}
}

/**
 * @brief   It handles samplerate down scaling in ratio 1.* with fraction.
 * @remarks ratio 1.*, e.g. 24K->16K, 16K->11.025K, ...
 * @param   src: pointer to resampler object.
 * @param   num_frames_out: number of frames will be stored in output buffer.
 * @param   frac: ratio of orig samplerate being divided by target samplerate.
 * @return  void
 * @see
 */
static void downresample_frac(resampler_t *src, int32_t num_frames_out, float frac)
{
	const int16_t *input = src->in_buffer;
	int16_t *output = src->out_buffer;
	int32_t channels_num = src->channels_num;
	float step_float = frac;
	uint32_t step = TO_16_16_FIXED(step_float);
	uint32_t fp_index = 0;   // 16.16 fixed point value

	int32_t j;
	for (j = 0; j < num_frames_out; ++j, fp_index += step) {
		uint32_t whole = INTPART_VALUE(fp_index);
		uint32_t part = FRACPART_VALUE(fp_index);
		int32_t k;
		for (k = 0; k < channels_num; k++) {
			int32_t s1 = input[whole * channels_num + k];
			int32_t s2 = input[(whole + 1) * channels_num + k];
			*output++ = clip(CALC_NEW_SAMPLE(s1, s2, part));
		}
	}
}

/**
 * @brief   It handles samplerate down scaling in ratio 2.0/3.0 without fraction.
 * @remarks ratio 2.0/3.0, e.g. 48K->16K, 48K->24K, 44.1K->22.05K...
 * @param   src: pointer to resampler object.
 * @param   num_frames_in: number of frames in above input buffer.
 * @param   quoti: quotient of orig samplerate being divided by target samplerate, it is the ratio mentioned above.
 * @return  void
 * @see     downresample_2_1_filter_frac()
 */
static void downresample_int(resampler_t *src, int32_t num_frames_in, int32_t quoti)
{
	const int16_t *input = src->in_buffer;
	int16_t *output = src->out_buffer;
	int32_t channels_num = src->channels_num;
	int32_t num_samples_in = num_frames_in * channels_num;

	int32_t i;
	for (i = 0; i < num_samples_in; i += quoti) {
		output[i / quoti] = clip(fir_convolve(input + i, filter_22khz_coeff, NUM_COEFF_22KHZ, channels_num));
	}
}

/**
 * @brief   It handles samplerate down scaling in special ratio.
 * @remarks The special ratio is 1.*, e.g. 44.1K->32K, 22.05K->16K, 11.025K->8K
 * @param   src: pointer to resampler object.
 * @param   num_frames_in: number of frames in above input buffer.
 * @param   num_frames_out: number of frames will be stored in output buffer.
 * @return  void
 * @see
 */
static void downresample_441_320(resampler_t *src, int32_t num_frames_in, int32_t num_frames_out)
{
	int16_t *input = src->in_buffer;
	int16_t *output = src->out_buffer;
	int32_t channels_num = src->channels_num;
	int32_t num_samples_in = num_frames_in * channels_num;
	float step_float = DOWN_RESAMPLE_441_320_RATIO;
	uint32_t step = TO_16_16_FIXED(step_float);
	uint32_t fp_index = 0;   // 16.16 fixed point value

	int32_t i;
	for (i = 0; i < num_samples_in; ++i) {
		input[i] = fir_convolve(input + i, filter_16khz_coeff, OVERLAP_16KHZ, channels_num);
	}

	int32_t j;
	for (j = 0; j < num_frames_out; ++j, fp_index += step) {
		uint32_t whole = INTPART_VALUE(fp_index);
		uint32_t part = FRACPART_VALUE(fp_index);
		int32_t k;
		for (k = 0; k < channels_num; k++) {
			int32_t s1 = input[whole * channels_num + k];
			int32_t s2 = input[(whole + 1) * channels_num + k];
			*output++ = clip(CALC_NEW_SAMPLE(s1, s2, part));
		}
	}
}

/**
 * @brief   It handles samplerate down scaling in ratio 2.* with fraction.
 * @remarks ratio 2.*, e.g. 48K->22.05K, 44.1K->16K, 24K->11.025K ...
 * @param   num_frames_in: number of frames in above input buffer.
 * @param   num_frames_out: number of frames will be stored in output buffer.
 * @param   frac: ratio of orig samplerate being divided by target samplerate, it has fraction value.
 * @return  void
 * @see     downresample_int()
 */
static void downresample_2_1_filter_frac(resampler_t *src, int32_t num_frames_in, int32_t num_frames_out, float frac)
{
	int16_t *input = src->in_buffer;
	int16_t *output = src->out_buffer;
	int32_t channels_num = src->channels_num;
	int32_t num_samples_in = num_frames_in * channels_num;
	float step_float = frac;
	uint32_t step = TO_16_16_FIXED(step_float);
	uint32_t fp_index = 0;   // 16.16 fixed point value

	int32_t i;
	for (i = 0; i < num_samples_in; ++i) {
		input[i] = fir_convolve(input + i, filter_22khz_coeff, OVERLAP_22KHZ, channels_num);
	}

	int32_t j;
	for (j = 0; j < num_frames_out; ++j, fp_index += step) {
		uint32_t whole = INTPART_VALUE(fp_index);
		uint32_t part = FRACPART_VALUE(fp_index);
		int32_t k;
		for (k = 0; k < channels_num; k++) {
			int32_t s1 = input[whole * channels_num + k];
			int32_t s2 = input[(whole + 1) * channels_num + k];
			*output++ = clip(CALC_NEW_SAMPLE(s1, s2, part));
		}
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
src_handle_t src_init(int size)
{
	resampler_t *src = (resampler_t *)malloc(sizeof(resampler_t));
	RETURN_VAL_IF_FAIL((src != NULL), NULL);

	// max frame size for 2(max) channels
	int max_frame_size = BYTES_PER_SAMPLE(SAMPLE_WIDTH_MAX) * 2;
	// frame size aligned
	src->size_in_bytes = (((size + max_frame_size - 1) / max_frame_size) * max_frame_size);

	src->size_in_frames = 0;
	src->in_buffer = NULL;

	src->outsize_in_frames = 0;
	src->out_buffer = NULL;

	src->used_frames = 0;
	src->left_frames = 0;
	src->lastratio = 0.0f;

	return (src_handle_t)src;
}

int src_destroy(src_handle_t handle)
{
	resampler_t *src = (resampler_t *)handle;
	RETURN_VAL_IF_FAIL((src != NULL), SRC_ERR_BAD_PARAMS);

	free(src->in_buffer);
	src->in_buffer = NULL;

	free(src);
	return SRC_ERR_NO_ERROR;
}

bool src_is_valid_ratio(float ratio)
{
	if ((ratio <= SRC_MAX_RATIO) && (ratio >= SRC_MIN_RATIO)) {
		return true;
	}

	return false;
}

int src_simple(src_handle_t handle, src_data_t *src_data)
{
	resampler_t *src = (resampler_t *)handle;
	RETURN_VAL_IF_FAIL((src != NULL && src_data != NULL), SRC_ERR_BAD_PARAMS);
	RETURN_VAL_IF_FAIL((src_data->data_in != NULL && src_data->data_out != NULL), SRC_ERR_BAD_PARAMS);

	int channels_num = src_data->channels_num;
	RETURN_VAL_IF_FAIL((channels_num > 0 && channels_num <= SRC_MAX_CH), SRC_ERR_BAD_CHANNEL_COUNT);

	int old_sample_rate = src_data->origin_sample_rate;
	int new_sample_rate = src_data->desired_sample_rate;
	int bytes_per_sample = BYTES_PER_SAMPLE(src_data->origin_sample_width);
	int old_sample_width = src_data->origin_sample_width;
	int new_sample_width = src_data->desired_sample_width;

	float src_ratio = (float)new_sample_rate / (float)old_sample_rate;
	src_data->src_ratio = src_ratio;
	if (src_is_valid_ratio(src_ratio) == false) {
		return SRC_ERR_BAD_SRC_RATIO;
	}

	// PCM format conversion isn't supported currently, and shall be supported in future.
	// Support SAMPLE_WIDTH_16BITS  currenty, and other format shall be supported in future.
	RETURN_VAL_IF_FAIL((old_sample_width == new_sample_width && old_sample_width == SAMPLE_WIDTH_16BITS), SRC_ERR_NOT_SUPPORT);

	src->outsize_in_frames = src_data->out_buf_length / (BYTES_PER_SAMPLE(new_sample_width) * channels_num);
	RETURN_VAL_IF_FAIL((src_data->out_buf_length > 0 && src->outsize_in_frames > src->left_frames), SRC_ERR_BAD_PARAMS);
	src->out_buffer = src_data->data_out;

	if (src->in_buffer == NULL) {
		src->in_buffer = (int16_t *)malloc(src->size_in_bytes);
		RETURN_VAL_IF_FAIL((src->in_buffer != NULL), SRC_ERR_MALLOC_FAILED);

		src->channels_num = channels_num;
		src->size_in_frames = src->size_in_bytes / (bytes_per_sample * channels_num);
		src->lastratio = src_ratio;
		src->lastoldformat = old_sample_width;
		src->lastnewformat = new_sample_width;
	} else {
		// Don't support to do different ratio SRC in same processing.
		// To be exact, both origin_sample_rate & desired_sample_rate shouldn't be changed.
		// Same restriction for sample width and channel num.
		RETURN_VAL_IF_FAIL((src->lastratio == src_ratio), SRC_ERR_NOT_SUPPORT);
		RETURN_VAL_IF_FAIL((src->channels_num == channels_num), SRC_ERR_NOT_SUPPORT);
		RETURN_VAL_IF_FAIL((src->lastoldformat == old_sample_width), SRC_ERR_NOT_SUPPORT);
		RETURN_VAL_IF_FAIL((src->lastnewformat == new_sample_width), SRC_ERR_NOT_SUPPORT);
	}

	int input_frames_used = 0;
	int output_frames_gen = 0;
	if (old_sample_rate == new_sample_rate) {
		// same rate, just copy data
		input_frames_used = MINIMUM(src_data->input_frames, src->outsize_in_frames);
		output_frames_gen = input_frames_used;
		memcpy((void *)src->out_buffer,\
			   src_data->data_in,\
			   output_frames_gen * channels_num * bytes_per_sample);
	} else {
		int frames_num = 0; // frames count in internal fin buffer

#define FRAMES_TO_BYTES(frames) ((frames) * channels_num * bytes_per_sample)
		// move remaining frames
		memcpy((void *)src->in_buffer,\
			   (const void *)((int8_t *)src->in_buffer + FRAMES_TO_BYTES(src->used_frames)),\
			   FRAMES_TO_BYTES(src->left_frames));
		frames_num += src->left_frames;

		//according to the outsize_in_frames, to calculate the adjust_size_in_frames
		int ratio = new_sample_rate / old_sample_rate;
		if ((new_sample_rate % old_sample_rate) != 0) {
			ratio += 1;
		}
		int adjust_size_in_frames = MINIMUM(src->size_in_frames, src->outsize_in_frames/ratio);
		
		// accept input frames as much as possible
		input_frames_used = MINIMUM(src_data->input_frames, (adjust_size_in_frames - frames_num));

		// append new input frames
		memcpy((void *)(int8_t *)src->in_buffer + FRAMES_TO_BYTES(frames_num),\
			   (const void *)src_data->data_in,\
			   FRAMES_TO_BYTES(input_frames_used));
		frames_num += input_frames_used;
#undef FRAMES_TO_BYTES

		// resample ...
		if (old_sample_rate > new_sample_rate) {
			// down resample
			int quotient = old_sample_rate / new_sample_rate;
			int remainer = old_sample_rate % new_sample_rate;
			float inverse_ratio = ((float)old_sample_rate / (float)new_sample_rate);

			if (quotient >= 2) {
				if (remainer == 0) {
					// ratio 2.0/3.0 :: 48K->16K, 48K->24K, 44.1K->22.05K, ...
					int blocks = (frames_num - OVERLAP_22KHZ) / quotient;
					src->used_frames = blocks * quotient;
					src->left_frames = frames_num - src->used_frames;

					output_frames_gen = src->used_frames / quotient;
					downresample_int(src, src->used_frames, quotient);
				} else {
					// ratio 2.* :: 48K->22.05K, 44.1K->16K, 24K->11.025K, ...
					src->left_frames = OVERLAP_22KHZ;
					src->used_frames = frames_num - src->left_frames;

					output_frames_gen = LRINTPF((float)src->used_frames * src_ratio);
					downresample_2_1_filter_frac(src, src->used_frames, output_frames_gen, inverse_ratio);
				}
			} else if (FLOAT_EQUAL(inverse_ratio, DOWN_RESAMPLE_441_320_RATIO)) {
				// ratio 1.* :: 44.1K->32K, 22.05K->16K, 11.025K->8K, ...
				src->left_frames = OVERLAP_16KHZ;
				src->used_frames = frames_num - src->left_frames;

				output_frames_gen = LRINTPF((float)src->used_frames * src_ratio);
				downresample_441_320(src, src->used_frames, output_frames_gen);
			} else {
				// other ratio 1.* :: 24K->16K, 16K->11.025K, ...
				src->left_frames = OVERLAP_DEFAULT;
				src->used_frames = frames_num - src->left_frames;

				output_frames_gen = LRINTPF((float)src->used_frames * src_ratio);
				downresample_frac(src, output_frames_gen, inverse_ratio);
			}
		} else {
			// up resample
			int quotient = new_sample_rate / old_sample_rate;
			int remainer = new_sample_rate % old_sample_rate;

			src->left_frames = OVERLAP_DEFAULT;
			src->used_frames = frames_num - src->left_frames;

			if (remainer == 0) {
				// ratio 2.0/3.0 :: 8K->16K, 8K->24K, 11.025K->22.05K, ...
				output_frames_gen = src->used_frames * quotient;
				upresample_int(src, output_frames_gen, quotient);
			} else {
				// ratio 1.*/2.* :: 8K->11.025K, 16K->44.1K, 11.025K->12K, ...
				float frac = (float)((quotient + 1) * old_sample_rate) / (float) new_sample_rate;
				output_frames_gen = LRINTPF((float)(src->used_frames * (quotient + 1)) / frac);
				upresample_frac(src, output_frames_gen, quotient + 1, frac);
			}
		}
	}

	src_data->data_out = src->out_buffer;
	src_data->src_ratio = src_ratio;
	src_data->input_frames_used = input_frames_used;
	src_data->output_frames_gen = output_frames_gen;

	return SRC_ERR_NO_ERROR;
}

int src_StereoToMono(short *data, int len)
{
	int index = 0;
	int channels = 2; // 2 channels stereo
	for (index = 0; index < (len / channels); index++) {
		// New Mono Sample = (L Sample + R Sample) / 2
		// Use the average value simpley.
		data[index] = (data[(index << 1) + 1] + data[index << 1]) >> 1;
	}

	return index;
}

int src_MonoToStereo(short *data, int len)
{
	int index = 0;
	for (index = len - 1; index >= 0; index--) {
		// L Sample = Mono Sample, R Sample = Mono Sample
		// Copy Mono Sample as L/R Sample simply
		data[(index << 1) + 1] = data[index];
		data[index << 1] = data[index];
	}

	return len << 1;
}

