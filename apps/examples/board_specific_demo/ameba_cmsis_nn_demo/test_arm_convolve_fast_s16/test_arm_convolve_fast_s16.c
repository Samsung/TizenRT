/*
 * SPDX-FileCopyrightText: Copyright 2010-2022 Arm Limited and/or its affiliates <open-source-office@arm.com> All rights
 * reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>

#include <arch/chip/cmsis_nn/arm_nnfunctions.h>

#include "../TestData/int16xint8/config_data.h"
#include "../TestData/requantize_s64/config_data.h"
#include "../Utils/validate.h"

void int16xint8_arm_convolve_fast_s16(void)
{
    q15_t output[INT16XINT8_DST_SIZE] = {0};

    cmsis_nn_context ctx;
    cmsis_nn_conv_params conv_params;
    cmsis_nn_per_channel_quant_params quant_params;
    cmsis_nn_dims input_dims;
    cmsis_nn_dims filter_dims;
    cmsis_nn_dims bias_dims;
    cmsis_nn_dims output_dims;

    /* To avoid multiple definition */
    const int64_t int16xint8_biases[4] = {-2495236, -432952, -2150622, 3037347};
    const q7_t int16xint8_weights[96] = {
    -113, 41,  -99,  -23, -59,  -65, -88,  -123, 83,   127, -121, 97,  -96, -94,  52,  -8,   6,    -6,  90,   15,
    87,   111, -56,  -42, 80,   115, -1,   72,   116,  107, 67,   124, -50, -30,  -6,  23,   -127, 102, 58,   64,
    34,   52,  111,  19,  -104, -2,  -95,  -125, 77,   -16, -5,   20,  -21, -3,   91,  -124, -47,  -83, -67,  -3,
    124,  111, 101,  -21, 79,   127, -124, 69,   -15,  -82, 11,   113, 46,  -102, 95,  32,   -32,  -7,  -114, -102,
    32,   62,  -127, 87,  83,   -3,  -67,  97,   -100, -7,  34,   -73, 99,  -61,  -15, -104};
    const q15_t int16xint8_input[168] = {
    11645,  22850,  31319,  -24644, 2819,   1883,   -26935, -18937, 22831,  28845,  31347,  -31674, -27727, -23552,
    -10251, -13891, 26029,  17279,  -25122, 6570,   2539,   -4245,  -23085, 18088,  -23145, -32457, -29942, -6541,
    -31537, -1148,  8908,   3454,   20995,  2559,   9127,   -7295,  20003,  -19625, 3042,   5151,   -9477,  -32504,
    -32554, -7262,  16464,  -9399,  3808,   -16672, 16164,  -3372,  -22958, 7014,   -4084,  18590,  -13109, 26520,
    4925,   -3094,  -27051, -14280, -9399,  29082,  -16574, 5120,   -1813,  24619,  13455,  -7827,  -21207, 26555,
    -380,   31052,  9576,   -10092, -21755, -16522, 4976,   -438,   3725,   -22109, -18201, 9708,   32288,  8873,
    4352,   5560,   -4372,  19609,  32721,  2994,   23154,  -10829, -22219, -13091, 5930,   -17281, -19958, -3651,
    4127,   9918,   28773,  -2436,  -30694, -28239, -12289, -25622, -17760, -14319, -27872, 6694,   -522,   27721,
    -12826, -32554, -11147, 18112,  7633,   13587,  -23309, 14490,  28782,  -2318,  29818,  -24295, 21169,  18585,
    31536,  -8885,  4362,   14410,  8865,   -22691, -6651,  -6278,  -31346, 14182,  21068,  8149,   -4410,  -2909,
    -16315, -2756,  -28748, -28150, -2663,  1654,   -21926, 1999,   11965,  -12122, -12450, -27546, 31965,  -21814,
    26971,  -32390, 2906,   -8975,  -17418, -17112, 19933,  24141,  24802,  19217,  7530,   -3580,  1147,   -20321};
    const q15_t int16xint8_output_ref[48] = {
    -6939, -7913, -10478, 2083,  4909,   -7615, -12139, 56,   -2031,  -239,  -1461, 12852, -5087, -4686, -6984,  4797,
    -7156, -9860, 2349,   4743,  -10003, -1689, -4330,  159,  14093,  -5456, -8866, -1445, 2552,  3120,  -13335, -10211,
    -4546, -7367, 95,     -1593, -9522,  6399,  -8913,  3013, -10355, 13874, 8880,  8500,  -4208, 703,   -7717,  2509};
    const int32_t int16xint8_output_mult[4] = {1388772064, 1391969415, 1360038279, 1365367254};
    const int32_t int16xint8_output_shift[4] = {-9, -9, -9, -9};

    const q63_t *bias_data = int16xint8_biases;
    const q7_t *kernel_data = int16xint8_weights;
    const q15_t *input_data = int16xint8_input;
    const q15_t *output_ref = int16xint8_output_ref;
    const int32_t output_ref_size = INT16XINT8_DST_SIZE;

    input_dims.n = INT16XINT8_INPUT_BATCHES;
    input_dims.w = INT16XINT8_INPUT_W;
    input_dims.h = INT16XINT8_INPUT_H;
    input_dims.c = INT16XINT8_IN_CH;
    filter_dims.w = INT16XINT8_FILTER_X;
    filter_dims.h = INT16XINT8_FILTER_Y;
    output_dims.w = INT16XINT8_OUTPUT_W;
    output_dims.h = INT16XINT8_OUTPUT_H;
    output_dims.c = INT16XINT8_OUT_CH;

    conv_params.padding.w = INT16XINT8_PAD_X;
    conv_params.padding.h = INT16XINT8_PAD_Y;
    conv_params.stride.w = INT16XINT8_STRIDE_X;
    conv_params.stride.h = INT16XINT8_STRIDE_Y;
    conv_params.dilation.w = INT16XINT8_DILATION_X;
    conv_params.dilation.h = INT16XINT8_DILATION_X;

    conv_params.input_offset = 0;
    conv_params.output_offset = 0;
    conv_params.activation.min = INT16XINT8_OUT_ACTIVATION_MIN;
    conv_params.activation.max = INT16XINT8_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)int16xint8_output_mult;
    quant_params.shift = (int32_t *)int16xint8_output_shift;

    int buf_size = arm_convolve_wrapper_s16_get_buffer_size(&conv_params, &input_dims, &filter_dims, &output_dims);
    ctx.buf = malloc(buf_size);

    arm_cmsis_nn_status result = arm_convolve_wrapper_s16(&ctx,
                                                          &conv_params,
                                                          &quant_params,
                                                          &input_dims,
                                                          input_data,
                                                          &filter_dims,
                                                          kernel_data,
                                                          &bias_dims,
                                                          bias_data,
                                                          &output_dims,
                                                          output);
    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }

    TEST_ASSERT_EQUAL(ARM_CMSIS_NN_SUCCESS, result);
    TEST_ASSERT_TRUE(validate_s16(output, output_ref, output_ref_size));
    memset(output, 0, sizeof(output));

    buf_size = arm_convolve_fast_s16_get_buffer_size(&input_dims, &filter_dims);
    ctx.buf = malloc(buf_size);

    result = arm_convolve_fast_s16(&ctx,
                                   &conv_params,
                                   &quant_params,
                                   &input_dims,
                                   input_data,
                                   &filter_dims,
                                   kernel_data,
                                   &bias_dims,
                                   bias_data,
                                   &output_dims,
                                   output);
    if (ctx.buf)
    {
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
#if defined(ARM_MATH_DSP) && !defined(ARM_MATH_MVEI)
    TEST_ASSERT_EQUAL(ARM_CMSIS_NN_SUCCESS, result);
    TEST_ASSERT_TRUE(validate_s16(output, output_ref, output_ref_size));
#else
    TEST_ASSERT_EQUAL(ARM_CMSIS_NN_ARG_ERROR, result);
#endif
}

void requantize_s64_arm_convolve_fast_s16(void)
{
    q15_t output[REQUANTIZE_S64_DST_SIZE] = {0};

    cmsis_nn_context ctx;
    cmsis_nn_conv_params conv_params;
    cmsis_nn_per_channel_quant_params quant_params;
    cmsis_nn_dims input_dims;
    cmsis_nn_dims filter_dims;
    cmsis_nn_dims bias_dims;
    cmsis_nn_dims output_dims;

    /* To avoid multiple definition */
    const int64_t requantize_s64_biases[2] = {-21812718, 1073741952};
    const q7_t requantize_s64_weights[16] = {-23, 101, 3, -55, -120, 127, -120, 6, 103, 103, -95, 60, 102, 9, -60, -90};
    const q15_t requantize_s64_input[12] = {82, 66, -30, -105, 95, 74, 124, 93, -25, 93, 57, -32};
    const q15_t requantize_s64_output_ref[4] = {-899, 32636, -900, 32634};
    const int32_t requantize_s64_output_mult[2] = {1451496532, 2138792311};
    const int32_t requantize_s64_output_shift[2] = {-14, -15};

    const q63_t *bias_data = requantize_s64_biases;
    const q7_t *kernel_data = requantize_s64_weights;
    const q15_t *input_data = requantize_s64_input;
    const q15_t *output_ref = requantize_s64_output_ref;
    const int32_t output_ref_size = REQUANTIZE_S64_DST_SIZE;

    input_dims.n = REQUANTIZE_S64_INPUT_BATCHES;
    input_dims.w = REQUANTIZE_S64_INPUT_W;
    input_dims.h = REQUANTIZE_S64_INPUT_H;
    input_dims.c = REQUANTIZE_S64_IN_CH;
    filter_dims.w = REQUANTIZE_S64_FILTER_X;
    filter_dims.h = REQUANTIZE_S64_FILTER_Y;
    output_dims.w = REQUANTIZE_S64_OUTPUT_W;
    output_dims.h = REQUANTIZE_S64_OUTPUT_H;
    output_dims.c = REQUANTIZE_S64_OUT_CH;

    conv_params.padding.w = REQUANTIZE_S64_PAD_X;
    conv_params.padding.h = REQUANTIZE_S64_PAD_Y;
    conv_params.stride.w = REQUANTIZE_S64_STRIDE_X;
    conv_params.stride.h = REQUANTIZE_S64_STRIDE_Y;
    conv_params.dilation.w = REQUANTIZE_S64_DILATION_X;
    conv_params.dilation.h = REQUANTIZE_S64_DILATION_Y;

    conv_params.input_offset = REQUANTIZE_S64_INPUT_OFFSET;
    conv_params.output_offset = REQUANTIZE_S64_OUTPUT_OFFSET;
    conv_params.activation.min = REQUANTIZE_S64_OUT_ACTIVATION_MIN;
    conv_params.activation.max = REQUANTIZE_S64_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)requantize_s64_output_mult;
    quant_params.shift = (int32_t *)requantize_s64_output_shift;

    int buf_size = arm_convolve_wrapper_s16_get_buffer_size(&conv_params, &input_dims, &filter_dims, &output_dims);
    ctx.buf = malloc(buf_size);

    arm_cmsis_nn_status result = arm_convolve_wrapper_s16(&ctx,
                                                          &conv_params,
                                                          &quant_params,
                                                          &input_dims,
                                                          input_data,
                                                          &filter_dims,
                                                          kernel_data,
                                                          &bias_dims,
                                                          bias_data,
                                                          &output_dims,
                                                          output);

    if (ctx.buf)
    {
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(ARM_CMSIS_NN_SUCCESS, result);
    TEST_ASSERT_TRUE(validate_s16(output, output_ref, output_ref_size));
    memset(output, 0, sizeof(output));

    buf_size = arm_convolve_fast_s16_get_buffer_size(&input_dims, &filter_dims);
    ctx.buf = malloc(buf_size);

    result = arm_convolve_fast_s16(&ctx,
                                   &conv_params,
                                   &quant_params,
                                   &input_dims,
                                   input_data,
                                   &filter_dims,
                                   kernel_data,
                                   &bias_dims,
                                   bias_data,
                                   &output_dims,
                                   output);
    if (ctx.buf)
    {
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
#if defined(ARM_MATH_DSP) && !defined(ARM_MATH_MVEI)
    TEST_ASSERT_EQUAL(ARM_CMSIS_NN_SUCCESS, result);
    TEST_ASSERT_TRUE(validate_s16(output, output_ref, output_ref_size));
#else
    TEST_ASSERT_EQUAL(ARM_CMSIS_NN_ARG_ERROR, result);
#endif
}
