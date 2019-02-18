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
#include	"../../utils/remix.h"


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

// Count bytes of the given frames
#define NEW_FRAMES_TO_BYTES(frames) ((frames) * new_channel_num * bytes_per_sample)

/****************************************************************************
 * Private Declarations
 ****************************************************************************/
/**
 * @structure resampler_s: main structure used for SRC, it contains context
 *            variables used between src_simple() calls.
 * @brief It's internal structure, user can only get the handler via src_init().
 */
struct resampler_s {
	int16_t *in_buffer;     // pointer to the internal input buffer allocated
	int16_t *out_buffer;    // pointer to the external output buffer assigned
	int in_buffer_bytes;    // internal input buffer capability in bytes
	int in_buffer_frames;   // internal input buffer capability in frames
	int out_buffer_frames;  // external output buffer capability in frames
	int left_frames;        // number of frames remained in internal input buffer
	int used_frames;        // number of frames used in internal input buffer
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
	src->in_buffer_bytes = (((size + max_frame_size - 1) / max_frame_size) * max_frame_size);

	src->in_buffer_frames = 0;
	src->in_buffer = NULL;

	src->out_buffer_frames = 0;
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
	// Check validation of input params
	RETURN_VAL_IF_FAIL((handle != NULL && src_data != NULL), SRC_ERR_BAD_PARAMS);
	RETURN_VAL_IF_FAIL((src_data->data_in != NULL && src_data->data_out != NULL), SRC_ERR_BAD_PARAMS);

	int old_sample_rate = src_data->origin_sample_rate;
	int new_sample_rate = src_data->desired_sample_rate;
	int old_sample_width = src_data->origin_sample_width;
	int new_sample_width = src_data->desired_sample_width;
	int old_channel_num = src_data->origin_channel_num;
	int new_channel_num = src_data->desired_channel_num;
	int bytes_per_sample = BYTES_PER_SAMPLE(old_sample_width);

	// Check supported multichannels number
	RETURN_VAL_IF_FAIL((old_channel_num >= 1 && old_channel_num <= 6), SRC_ERR_BAD_CHANNEL_COUNT);

	// Check supported output channel: 1-Mono/2-Stereo
	RETURN_VAL_IF_FAIL((new_channel_num == 1 || new_channel_num == 2), SRC_ERR_BAD_CHANNEL_COUNT);

	// Check supported sample width: SAMPLE_WIDTH_16BITS
	RETURN_VAL_IF_FAIL((old_sample_width == SAMPLE_WIDTH_16BITS), SRC_ERR_NOT_SUPPORT);

	// Check supported format conversion: Not support!
	RETURN_VAL_IF_FAIL((old_sample_width == new_sample_width), SRC_ERR_NOT_SUPPORT);

	// Check supported resampling ratio
	float src_ratio = (float)new_sample_rate / (float)old_sample_rate;
	src_data->src_ratio = src_ratio;
	if (src_is_valid_ratio(src_ratio) == false) {
		return SRC_ERR_BAD_SRC_RATIO;
	}

	resampler_t *src = (resampler_t *)handle;
	src->out_buffer = src_data->data_out;
	src->out_buffer_frames = src_data->out_buf_length / (BYTES_PER_SAMPLE(new_sample_width) * new_channel_num);
	RETURN_VAL_IF_FAIL((src->out_buffer_frames > 0), SRC_ERR_BAD_PARAMS);

	if (src->in_buffer == NULL) {
		src->in_buffer = (int16_t *)malloc(src->in_buffer_bytes);
		RETURN_VAL_IF_FAIL((src->in_buffer != NULL), SRC_ERR_MALLOC_FAILED);

		src->channels_num = new_channel_num;
		src->in_buffer_frames = src->in_buffer_bytes / (bytes_per_sample * old_channel_num);
		src->left_frames = 0;
		src->used_frames = 0;
		src->lastratio = src_ratio;
		src->lastoldformat = old_sample_width;
		src->lastnewformat = new_sample_width;
	} else {
		// Don't support to do different ratio SRC in same processing.
		// To be exact, both origin_sample_rate & desired_sample_rate shouldn't be changed.
		// Same restriction for sample width and channel num.
		RETURN_VAL_IF_FAIL((src->lastratio == src_ratio), SRC_ERR_NOT_SUPPORT);
		RETURN_VAL_IF_FAIL((src->channels_num == new_channel_num), SRC_ERR_NOT_SUPPORT);
		RETURN_VAL_IF_FAIL((src->lastoldformat == old_sample_width), SRC_ERR_NOT_SUPPORT);
		RETURN_VAL_IF_FAIL((src->lastnewformat == new_sample_width), SRC_ERR_NOT_SUPPORT);
	}

	int input_frames_used = 0;
	int output_frames_gen = 0;

	if (old_sample_rate == new_sample_rate) {
		// Same sample rate
		int32_t ret;
		ret = rechannel(ch2layout(old_channel_num), ch2layout(new_channel_num),\
				(const int16_t *)src_data->data_in, src_data->input_frames,\
				(int16_t *)src->out_buffer, src->out_buffer_frames);
		RETURN_VAL_IF_FAIL((ret > 0), SRC_ERR_BAD_PARAMS);
		output_frames_gen = ret;
		input_frames_used = output_frames_gen;
	} else {
		int frames_num = 0; // number of frames in internal input buffer

		// move remaining frames
		if (src->left_frames > 0) {
			memcpy((void *)src->in_buffer,\
				(const void *)((int8_t *)src->in_buffer + NEW_FRAMES_TO_BYTES(src->used_frames)),\
				NEW_FRAMES_TO_BYTES(src->left_frames));
			frames_num += src->left_frames;
		}

		// calculate number of frames, according to capabilities of internal-input and external-output buffers.
		int ratio = new_sample_rate / old_sample_rate;
		if ((new_sample_rate % old_sample_rate) != 0) {
			ratio += 1;
		}
		int in_buffer_frames_adjust = MINIMUM(src->in_buffer_frames, src->out_buffer_frames / ratio);

		// accept input frames as much as possible
		input_frames_used = MINIMUM(src_data->input_frames, (in_buffer_frames_adjust - frames_num));

		// append(rechannel/copy) some new input frames
		int32_t ret;
		ret = rechannel(ch2layout(old_channel_num), ch2layout(new_channel_num),\
				(const int16_t *)src_data->data_in, input_frames_used,\
				(int16_t *)((int8_t *)src->in_buffer + NEW_FRAMES_TO_BYTES(frames_num)), input_frames_used);
		RETURN_VAL_IF_FAIL((ret == input_frames_used), SRC_ERR_UNKNOWN);
		frames_num += input_frames_used;

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

