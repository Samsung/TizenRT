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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "samplerate.h"
#include "../../utils/remix.h"


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
#define CALC_NEW_SAMPLE(s1, s2, part) ((s1) + INTPART_VALUE(((s2) - (s1)) * (int32_t)(part)))

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
#define OLD_FRAMES_TO_BYTES(src, frames) ((frames) * (src)->old_channel_num * BYTES_PER_SAMPLE((src)->old_sample_width))
#define NEW_FRAMES_TO_BYTES(src, frames) ((frames) * (src)->new_channel_num * BYTES_PER_SAMPLE((src)->new_sample_width))

// Check src context initialized or not
#define CHECK_SRC_CONTEXT_INIT(src) ((src)->in_buffer != NULL)

/****************************************************************************
 * Private Declarations
 ****************************************************************************/
/**
 * @structure src_context_s: main structure used for SRC, it contains context
 *            variables used between src_simple() calls.
 * @brief It's internal structure, user can only get the handler via src_init().
 */
struct src_context_s {
	int16_t *in_buffer;     // pointer to the internal input buffer allocated
	int16_t *out_buffer;    // pointer to the external output buffer assigned
	int in_buffer_bytes;    // internal input buffer capability in bytes
	int in_buffer_frames;   // internal input buffer capability in frames
	int left_frames;        // number of frames remained in internal input buffer
	int used_frames;        // number of frames used in internal input buffer
	int old_channel_num;    // memorize old channel number
	int new_channel_num;    // memorize new channel number
	int old_sample_rate;    // memorize old sample rate
	int new_sample_rate;    // memorize new sample rate
	int old_sample_width;   // memorize old sample width(format)
	int new_sample_width;   // memorize new sample width(format)
	const int *filter_coeff;// pointer to filter coefficient array
	int overlap_frames;     // number of overlap frames reserved in internal buffer
	float ratio;            // (float)new_sample_rate / (float)old_sample_rate
	float inverse_ratio;    // (float)old_sample_rate / (float)new_sample_rate
	uint32_t fp_frac;       // fraction part value of last fixed point index
	/**
	 * @brief   Function pointer to resampling process function
	 * @param   src_context_t *: pointer to resampler object.
	 * @param   int32_t *: give number of frames available for converting
	 *                     and retrieve number of frames used actually.
	 * @return  number of frames generated
	 */
	int32_t (*src_func)(struct src_context_s *, int32_t *);
};

typedef struct src_context_s src_context_t;

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
 * It handles sample rate up scaling in all ratio cases (i.e. inverse ratio 0.*)
 * and sample rate down scaling cases in inverse ratio 1.* and 2.* with fraction.
 */
static int32_t resample_frac(src_context_t *src, int32_t *num_frames_in)
{
	int32_t num_frames_out = (int32_t)((float)*num_frames_in * src->ratio);
	const int16_t *input = src->in_buffer;
	int16_t *output = src->out_buffer;
	int32_t channels_num = src->new_channel_num;
	uint32_t step = TO_16_16_FIXED(src->inverse_ratio);
	uint32_t fp_index = src->fp_frac;
	uint32_t whole, frac;
	int32_t i, j, s1, s2;

	for (i = 0; i < num_frames_out; ++i, fp_index += step) {
		whole = INTPART_VALUE(fp_index);
		frac = FRACPART_VALUE(fp_index);
		for (j = 0; j < channels_num; j++) {
			s1 = input[whole * channels_num + j];
			s2 = input[(whole + 1) * channels_num + j];
			*output++ = clip(CALC_NEW_SAMPLE(s1, s2, frac));
		}
	}

	*num_frames_in = INTPART_VALUE(fp_index);
	src->fp_frac = FRACPART_VALUE(fp_index);;
	return num_frames_out;
}

/**
 * It handles sample rate down scaling cases in inverse ratio 2.0 and 3.0 without fraction.
 * This function has same logic as resample_frac(), that means resample_frac() also works,
 * but downresample_int() is more efficient in special inverse ratio 2.0 and 3.0 cases.
 * Optimization: The fraction value of fp_index is always 0, so remove unnecessary calculation.
 */
static int32_t downresample_int(src_context_t *src, int32_t *num_frames_in)
{
	int32_t quotient = (int32_t)src->inverse_ratio;
	*num_frames_in = *num_frames_in / quotient * quotient;
	int32_t num_frames_out = *num_frames_in / quotient;

	const int16_t *input = src->in_buffer;
	int16_t *output = src->out_buffer;
	int32_t channels_num = src->new_channel_num;
	uint32_t step = TO_16_16_FIXED(quotient);
	uint32_t fp_index = 0;
	uint32_t whole;
	int32_t i, j;

	for (i = 0; i < num_frames_out; ++i, fp_index += step) {
		whole = INTPART_VALUE(fp_index);
		for (j = 0; j < channels_num; j++) {
			*output++ = input[whole * channels_num + j];
		}
	}

	return num_frames_out;
}

/**
 * @brief   Do filtering once new frames added to internal buffer.
 * @param   src: pointer to resampler object.
 * @param   num_frames_add: number of frames added
 */
static void convolution_filtering(src_context_t *src, int32_t num_frames_add)
{
	if ((src->filter_coeff != NULL) && (src->left_frames > src->overlap_frames)) {
		int16_t *input;
		int32_t samples;
		if (src->left_frames == num_frames_add) {
			input = src->in_buffer;
			samples = (num_frames_add - src->overlap_frames) * src->new_channel_num;
		} else {
			input = src->in_buffer + src->left_frames - num_frames_add - src->overlap_frames;
			samples = num_frames_add * src->new_channel_num;
		}

		int32_t i;
		for (i = 0; i < samples; ++i) {
			input[i] = fir_convolve(input + i, src->filter_coeff, src->overlap_frames, src->new_channel_num);
		}
	}
}

/**
 * @brief   Check validation of the given src_data.
 * @param   src: pointer to resampler object.
 * @param   src_data: pointer to the user given src_data_t structure
 * @return  0 on success, negative value means failure.
 */
static int check_src_data(src_context_t *src, src_data_t *src_data)
{
	RETURN_VAL_IF_FAIL((src_data != NULL), SRC_ERR_BAD_PARAMS);
	RETURN_VAL_IF_FAIL(((src_data->data_in != NULL) && (src_data->data_out != NULL)), SRC_ERR_BAD_PARAMS);

	if (!CHECK_SRC_CONTEXT_INIT(src)) {
		// Check supported converting ratio
		if (!src_is_valid_ratio((float)src_data->desired_sample_rate / (float)src_data->origin_sample_rate)) {
			return SRC_ERR_BAD_SRC_RATIO;
		}
		// Check supported input multichannels number: 1-Mono/.../6-5.1 Stereo
		RETURN_VAL_IF_FAIL(((src_data->origin_channel_num >= 1) && (src_data->origin_channel_num <= 6)), SRC_ERR_BAD_CHANNEL_COUNT);
		// Check supported output channel: 1-Mono/2-Stereo
		RETURN_VAL_IF_FAIL(((src_data->desired_channel_num == 1) || (src_data->desired_channel_num == 2)), SRC_ERR_BAD_CHANNEL_COUNT);
		// Check supported sample width: SAMPLE_WIDTH_16BITS
		RETURN_VAL_IF_FAIL((src_data->origin_sample_width == SAMPLE_WIDTH_16BITS), SRC_ERR_NOT_SUPPORT);
		// Check supported format conversion: Not support!
		RETURN_VAL_IF_FAIL((src_data->origin_sample_width == src_data->desired_sample_width), SRC_ERR_NOT_SUPPORT);
	} else {
		// Old/New sample rate, sample width and channel number must stay the same.
		RETURN_VAL_IF_FAIL((src->old_sample_rate == src_data->origin_sample_rate), SRC_ERR_NOT_SUPPORT);
		RETURN_VAL_IF_FAIL((src->new_sample_rate == src_data->desired_sample_rate), SRC_ERR_NOT_SUPPORT);
		RETURN_VAL_IF_FAIL((src->old_channel_num == src_data->origin_channel_num), SRC_ERR_NOT_SUPPORT);
		RETURN_VAL_IF_FAIL((src->new_channel_num == src_data->desired_channel_num), SRC_ERR_NOT_SUPPORT);
		RETURN_VAL_IF_FAIL((src->old_sample_width == src_data->origin_sample_width), SRC_ERR_NOT_SUPPORT);
		RETURN_VAL_IF_FAIL((src->new_sample_width == src_data->desired_sample_width), SRC_ERR_NOT_SUPPORT);
	}

	return SRC_ERR_NO_ERROR;
}

/**
 * @brief   Initialize src context members before first use (converting).
 * @param   src: pointer to resampler object.
 * @param   src_data: pointer to the user given src_data_t structure
 * @return  0 on success, negative value means failure.
 */
static int init_src_context(src_context_t *src, src_data_t *src_data)
{
	// Allocate internal buffer
	src->in_buffer = (int16_t *)malloc(src->in_buffer_bytes);
	RETURN_VAL_IF_FAIL((src->in_buffer != NULL), SRC_ERR_MALLOC_FAILED);

	// Initialize other members
	src->old_channel_num = src_data->origin_channel_num;
	src->new_channel_num = src_data->desired_channel_num;
	src->old_sample_width = src_data->origin_sample_width;
	src->new_sample_width = src_data->desired_sample_width;
	src->old_sample_rate = src_data->origin_sample_rate;
	src->new_sample_rate = src_data->desired_sample_rate;
	src->in_buffer_frames = src->in_buffer_bytes / NEW_FRAMES_TO_BYTES(src, 1);
	src->left_frames = 0;
	src->used_frames = 0;
	src->fp_frac = 0;

	// Calculate converting ratio for later use
	src->ratio = (float)src->new_sample_rate / (float)src->old_sample_rate;
	src->inverse_ratio = (float)src->old_sample_rate / (float)src->new_sample_rate;

	// Set overlap frame number and converting function as per converting ratio
	if (src->old_sample_rate > src->new_sample_rate) {
		// down resampling
		if (src->old_sample_rate % src->new_sample_rate == 0) {
			// inverse ratio 2.0/3.0 cases. e.g. 48K->16K, 48K->24K, 44.1K->22.05K, ...
			src->filter_coeff = filter_22khz_coeff;
			src->overlap_frames = OVERLAP_22KHZ;
			src->src_func = downresample_int;
		} else if ((int)src->inverse_ratio >= 2) {
			// inverse ratio 2.* cases. e.g. 48K->22.05K, 44.1K->16K, 24K->11.025K, ...
			src->filter_coeff = filter_22khz_coeff;
			src->overlap_frames = OVERLAP_22KHZ;
			src->src_func = resample_frac;
		} else {
			// inverse ratio 1.* cases. e.g. 48K->44.1K, 44.1K->32K, 44.1K->24K, ...
			src->filter_coeff = NULL;
			src->overlap_frames = OVERLAP_DEFAULT;
			src->src_func = resample_frac;
		}
	} else {
		// up resampling
		src->filter_coeff = NULL;
		src->overlap_frames = OVERLAP_DEFAULT;
		src->src_func = resample_frac;
		// TODO: Noises appeared in ratio 2.* cases, consider fir-filtering after converting process
	}

	return SRC_ERR_NO_ERROR;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
src_handle_t src_init(int size)
{
	src_context_t *src = (src_context_t *)malloc(sizeof(src_context_t));
	RETURN_VAL_IF_FAIL((src != NULL), NULL);

	// max frame size for 2(max) channels
	int max_frame_size = BYTES_PER_SAMPLE(SAMPLE_WIDTH_MAX) * 2;
	// frame size aligned
	src->in_buffer_bytes = (((size + max_frame_size - 1) / max_frame_size) * max_frame_size);
	src->in_buffer_frames = 0;
	src->in_buffer = NULL;
	// Other members will be initilized before first use,
	// as soon as in_buffer allocated in init_src_context().

	return (src_handle_t)src;
}

int src_destroy(src_handle_t handle)
{
	src_context_t *src = (src_context_t *)handle;
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
	// Convert and check src_handle
	src_context_t *src = (src_context_t *)handle;
	RETURN_VAL_IF_FAIL((src != NULL), SRC_ERR_BAD_PARAMS);

	// Check validation of src_data
	int ret = check_src_data(src, src_data);
	RETURN_VAL_IF_FAIL((ret == SRC_ERR_NO_ERROR), ret);

	// Calculate output buffer capability
	int bps = BYTES_PER_SAMPLE(src_data->origin_sample_width);
	int out_buffer_frames = src_data->out_buf_length / (bps * src_data->desired_channel_num);
	RETURN_VAL_IF_FAIL((out_buffer_frames > 0), SRC_ERR_BAD_PARAMS);

	// If the sample rate is same, direct to rechannel()
	int frames;
	if (src_data->origin_sample_rate == src_data->desired_sample_rate) {
		frames = rechannel(ch2layout(src_data->origin_channel_num), ch2layout(src_data->desired_channel_num), \
						(const int16_t *)src_data->data_in, src_data->input_frames, \
						(int16_t *)src_data->data_out, out_buffer_frames);
		RETURN_VAL_IF_FAIL((frames > 0), SRC_ERR_BAD_PARAMS);
		src_data->input_frames_used = frames;
		src_data->output_frames_gen = frames;
		return SRC_ERR_NO_ERROR;
	}

	// Sample Rate Converting ...
	// Initialize src context before first use
	if (!CHECK_SRC_CONTEXT_INIT(src)) {
		ret = init_src_context(src, src_data);
		RETURN_VAL_IF_FAIL((ret == SRC_ERR_NO_ERROR), ret);
		RETURN_VAL_IF_FAIL((src->src_func != NULL), SRC_ERR_UNKNOWN);
	}

	// Update output buffer to src context (used in converting proccess functions)
	src->out_buffer = (int16_t *)src_data->data_out;

	// Move remaining frames in internal buffer
	if ((src->used_frames > 0) && (src->left_frames > 0)) {
		memcpy((void *)src->in_buffer, \
			(const void *)((int8_t *)src->in_buffer + NEW_FRAMES_TO_BYTES(src, src->used_frames)), \
			NEW_FRAMES_TO_BYTES(src, src->left_frames));
		src->used_frames = 0;
	}

	// Accept input frames as much as possible, append (rechannel/copy) input frames to internal buffer
	int input_frames_used = MINIMUM(src_data->input_frames, (src->in_buffer_frames - src->left_frames));
	frames = rechannel(ch2layout(src->old_channel_num), ch2layout(src->new_channel_num), \
					(const int16_t *)src_data->data_in, input_frames_used, \
					(int16_t *)((int8_t *)src->in_buffer + NEW_FRAMES_TO_BYTES(src, src->left_frames)), input_frames_used);
	RETURN_VAL_IF_FAIL((frames == input_frames_used), SRC_ERR_UNKNOWN);
	src->left_frames += input_frames_used;

	// Filtering on new appended frames
	convolution_filtering(src, input_frames_used);

	// Calculate how many input frames needed if fill up out buffer
	float input_frames_need = (float)out_buffer_frames / src->ratio;
	if (input_frames_need - (int)input_frames_need > 0) {
		input_frames_need = (int)input_frames_need + 1;
	}

	int output_frames_gen = 0;
	// Reserve overlap frames, then do converting process with available frames
	frames = MINIMUM(src->left_frames - src->overlap_frames, (int)input_frames_need);
	if (frames > 0) {
		output_frames_gen = src->src_func(src, &frames);
		if (output_frames_gen != 0) {
			src->used_frames = frames;
			src->left_frames -= frames;
		}
	}

	src_data->input_frames_used = input_frames_used;
	src_data->output_frames_gen = output_frames_gen;
	return SRC_ERR_NO_ERROR;
}
