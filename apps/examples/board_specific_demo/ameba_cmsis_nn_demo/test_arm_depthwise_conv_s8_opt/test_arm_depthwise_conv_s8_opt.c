/*
 * SPDX-FileCopyrightText: Copyright 2010-2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
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

#include <arch/chip/cmsis_nn/arm_nnfunctions.h>
#include <stdlib.h>

#include "../TestData/basic/config_data.h"
#include "../TestData/depthwise_eq_in_out_ch/test_data.h"
#include "../TestData/depthwise_sub_block/test_data.h"
#include "../TestData/stride2pad1/config_data.h"
#include "../Utils/validate.h"

/* To avoid multiple definition */
const int32_t *get_bias_address_test(const int32_t *bias, int32_t size)
{
    const int32_t *return_bias = NULL;
    for (int i = 0; i < size; i++)
    {
        if (bias[i] != 0)
        {
            return_bias = bias;
            break;
        }
    }
    return return_bias;
}

void basic_arm_depthwise_conv_s8_opt(void)
{
    const arm_cmsis_nn_status expected = ARM_CMSIS_NN_SUCCESS;
    q7_t output[BASIC_DST_SIZE] = {0};

    cmsis_nn_context ctx;
    cmsis_nn_dw_conv_params dw_conv_params;
    cmsis_nn_per_channel_quant_params quant_params;
    cmsis_nn_dims input_dims;
    cmsis_nn_dims filter_dims;
    cmsis_nn_dims bias_dims;
    cmsis_nn_dims output_dims;

    /* To avoid multiple definition */
    const int32_t basic_biases[1] = {14820};
    const q7_t basic_weights[8] = {69, -84, -127, -24, 28, 45, 51, -53};
    const q7_t basic_input[40] = {-9,  -38, 76,  -40, 99,  60, 64,  52,  47,  -43, 72, 28,  -82, 61,
                              -63, 48,  -21, -47, 74,  29, -28, 8,   -38, 13,  33, -45, 59,  -25,
                              -8,  -50, 107, 81,  -77, 43, -82, 125, 83,  23,  17, 15};
    const int32_t basic_output_mult[1] = {1877929154};
    const int32_t basic_output_shift[1] = {-5};
    const q7_t basic_output_ref[20] = {127, -128, 63,  -128, -128, 127, 127, 127, -41, 127,
                                   -17, 127,  127, 127,  -128, 127, 127, 127, 127, 127};

    const q31_t *bias_data = basic_biases;
    const q7_t *kernel_data = basic_weights;
    const q7_t *input_data = basic_input;

    input_dims.n = BASIC_INPUT_BATCHES;
    input_dims.w = BASIC_INPUT_W;
    input_dims.h = BASIC_INPUT_H;
    input_dims.c = BASIC_IN_CH;
    filter_dims.w = BASIC_FILTER_X;
    filter_dims.h = BASIC_FILTER_Y;
    output_dims.w = BASIC_OUTPUT_W;
    output_dims.h = BASIC_OUTPUT_H;
    output_dims.c = BASIC_OUT_CH;

    dw_conv_params.padding.w = BASIC_PAD_X;
    dw_conv_params.padding.h = BASIC_PAD_Y;
    dw_conv_params.stride.w = BASIC_STRIDE_X;
    dw_conv_params.stride.h = BASIC_STRIDE_Y;
    dw_conv_params.dilation.w = BASIC_DILATION_X;
    dw_conv_params.dilation.h = BASIC_DILATION_Y;

    dw_conv_params.ch_mult = 1;

    dw_conv_params.input_offset = BASIC_INPUT_OFFSET;
    dw_conv_params.output_offset = BASIC_OUTPUT_OFFSET;
    dw_conv_params.activation.min = BASIC_OUT_ACTIVATION_MIN;
    dw_conv_params.activation.max = BASIC_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)basic_output_mult;
    quant_params.shift = (int32_t *)basic_output_shift;

    ctx.size = arm_depthwise_conv_s8_opt_get_buffer_size(&input_dims, &filter_dims);
    ctx.buf = malloc(ctx.size);

    arm_cmsis_nn_status result = arm_depthwise_conv_s8_opt(&ctx,
                                                           &dw_conv_params,
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
        memset(ctx.buf, 0, ctx.size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
    TEST_ASSERT_TRUE(validate(output, basic_output_ref, BASIC_DST_SIZE));
}

void stride2pad1_arm_depthwise_conv_s8_opt(void)
{
    const arm_cmsis_nn_status expected = ARM_CMSIS_NN_SUCCESS;
    q7_t output[STRIDE2PAD1_DST_SIZE] = {0};

    cmsis_nn_context ctx;
    cmsis_nn_dw_conv_params dw_conv_params;
    cmsis_nn_per_channel_quant_params quant_params;
    cmsis_nn_dims input_dims;
    cmsis_nn_dims filter_dims;
    cmsis_nn_dims bias_dims;
    cmsis_nn_dims output_dims;

    /* To avoid multiple definition */
    const int32_t stride2pad1_biases[1] = {4318};
    const q7_t stride2pad1_weights[9] = {85, 77, -30, 10, -12, 97, 34, 127, -4};
    const q7_t stride2pad1_input[49] = {27,   70,  -117, 45,   99,   100,  107, -24, 5,   75,  106,  -19, -70,
                                    -123, 30,  55,   24,   -49,  71,   36,  -35, -46, -96, -102, -15, -127,
                                    121,  37,  -37,  -10,  -115, 82,   53,  -60, -74, 38,  -109, 4,   53,
                                    -95,  120, 5,    -122, -71,  -117, 6,   39,  61,  88};
    const q7_t stride2pad1_output_ref[16] = {-42, 3, -11, -112, -42, -38, -8, -29, -26, -19, -72, 31, -73, -68, -47, -41};
    const int32_t stride2pad1_output_mult[1] = {1373179967};
    const int32_t stride2pad1_output_shift[1] = {-8};

    const q31_t *bias_data = stride2pad1_biases;
    const q7_t *kernel_data = stride2pad1_weights;
    const q7_t *input_data = stride2pad1_input;

    input_dims.n = STRIDE2PAD1_INPUT_BATCHES;
    input_dims.w = STRIDE2PAD1_INPUT_W;
    input_dims.h = STRIDE2PAD1_INPUT_H;
    input_dims.c = STRIDE2PAD1_IN_CH;
    filter_dims.w = STRIDE2PAD1_FILTER_X;
    filter_dims.h = STRIDE2PAD1_FILTER_Y;
    output_dims.w = STRIDE2PAD1_OUTPUT_W;
    output_dims.h = STRIDE2PAD1_OUTPUT_H;
    output_dims.c = STRIDE2PAD1_OUT_CH;

    dw_conv_params.padding.w = STRIDE2PAD1_PAD_X;
    dw_conv_params.padding.h = STRIDE2PAD1_PAD_Y;
    dw_conv_params.stride.w = STRIDE2PAD1_STRIDE_X;
    dw_conv_params.stride.h = STRIDE2PAD1_STRIDE_Y;
    dw_conv_params.dilation.w = STRIDE2PAD1_DILATION_X;
    dw_conv_params.dilation.h = STRIDE2PAD1_DILATION_Y;

    dw_conv_params.ch_mult = 1;

    dw_conv_params.input_offset = STRIDE2PAD1_INPUT_OFFSET;
    dw_conv_params.output_offset = STRIDE2PAD1_OUTPUT_OFFSET;
    dw_conv_params.activation.min = STRIDE2PAD1_OUT_ACTIVATION_MIN;
    dw_conv_params.activation.max = STRIDE2PAD1_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)stride2pad1_output_mult;
    quant_params.shift = (int32_t *)stride2pad1_output_shift;

    ctx.size = arm_depthwise_conv_s8_opt_get_buffer_size(&input_dims, &filter_dims);
    ctx.buf = malloc(ctx.size);

    arm_cmsis_nn_status result = arm_depthwise_conv_s8_opt(&ctx,
                                                           &dw_conv_params,
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
        memset(ctx.buf, 0, ctx.size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
    TEST_ASSERT_TRUE(validate(output, stride2pad1_output_ref, STRIDE2PAD1_DST_SIZE));
}

void depthwise_eq_in_out_ch_arm_depthwise_conv_s8_opt(void)
{
    const arm_cmsis_nn_status expected = ARM_CMSIS_NN_SUCCESS;
    q7_t output[DEPTHWISE_EQ_IN_OUT_CH_DST_SIZE] = {0};

    cmsis_nn_context ctx;
    cmsis_nn_dw_conv_params dw_conv_params;
    cmsis_nn_per_channel_quant_params quant_params;
    cmsis_nn_dims input_dims;
    cmsis_nn_dims filter_dims;
    cmsis_nn_dims bias_dims;
    cmsis_nn_dims output_dims;

    const q31_t *bias_data = get_bias_address_test(depthwise_eq_in_out_ch_biases, DEPTHWISE_EQ_IN_OUT_CH_IN_CH);
    const q7_t *kernel_data = depthwise_eq_in_out_ch_weights;
    const q7_t *input_data = depthwise_eq_in_out_ch_input;

    input_dims.n = DEPTHWISE_EQ_IN_OUT_CH_INPUT_BATCHES;
    input_dims.w = DEPTHWISE_EQ_IN_OUT_CH_INPUT_W;
    input_dims.h = DEPTHWISE_EQ_IN_OUT_CH_INPUT_H;
    input_dims.c = DEPTHWISE_EQ_IN_OUT_CH_IN_CH;
    filter_dims.w = DEPTHWISE_EQ_IN_OUT_CH_FILTER_X;
    filter_dims.h = DEPTHWISE_EQ_IN_OUT_CH_FILTER_Y;
    output_dims.w = DEPTHWISE_EQ_IN_OUT_CH_OUTPUT_W;
    output_dims.h = DEPTHWISE_EQ_IN_OUT_CH_OUTPUT_H;
    output_dims.c = DEPTHWISE_EQ_IN_OUT_CH_OUT_CH;

    dw_conv_params.padding.w = DEPTHWISE_EQ_IN_OUT_CH_PAD_X;
    dw_conv_params.padding.h = DEPTHWISE_EQ_IN_OUT_CH_PAD_Y;
    dw_conv_params.stride.w = DEPTHWISE_EQ_IN_OUT_CH_STRIDE_X;
    dw_conv_params.stride.h = DEPTHWISE_EQ_IN_OUT_CH_STRIDE_Y;
    dw_conv_params.dilation.w = DEPTHWISE_EQ_IN_OUT_CH_DILATION_X;
    dw_conv_params.dilation.h = DEPTHWISE_EQ_IN_OUT_CH_DILATION_Y;

    dw_conv_params.ch_mult = 1;

    dw_conv_params.input_offset = DEPTHWISE_EQ_IN_OUT_CH_INPUT_OFFSET;
    dw_conv_params.output_offset = DEPTHWISE_EQ_IN_OUT_CH_OUTPUT_OFFSET;
    dw_conv_params.activation.min = DEPTHWISE_EQ_IN_OUT_CH_OUT_ACTIVATION_MIN;
    dw_conv_params.activation.max = DEPTHWISE_EQ_IN_OUT_CH_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)depthwise_eq_in_out_ch_output_mult;
    quant_params.shift = (int32_t *)depthwise_eq_in_out_ch_output_shift;

    ctx.size = arm_depthwise_conv_s8_opt_get_buffer_size(&input_dims, &filter_dims);
    ctx.buf = malloc(ctx.size);

    arm_cmsis_nn_status result = arm_depthwise_conv_s8_opt(&ctx,
                                                           &dw_conv_params,
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
        memset(ctx.buf, 0, ctx.size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
    TEST_ASSERT_TRUE(validate(output, depthwise_eq_in_out_ch_output_ref, DEPTHWISE_EQ_IN_OUT_CH_DST_SIZE));
}

void depthwise_sub_block_arm_depthwise_conv_s8_opt(void)
{
    const arm_cmsis_nn_status expected = ARM_CMSIS_NN_SUCCESS;
    q7_t output[DEPTHWISE_SUB_BLOCK_DST_SIZE] = {0};

    cmsis_nn_context ctx;
    cmsis_nn_dw_conv_params dw_conv_params;
    cmsis_nn_per_channel_quant_params quant_params;
    cmsis_nn_dims input_dims;
    cmsis_nn_dims filter_dims;
    cmsis_nn_dims bias_dims;
    cmsis_nn_dims output_dims;

    const q31_t *bias_data = get_bias_address_test(depthwise_sub_block_biases, DEPTHWISE_SUB_BLOCK_IN_CH);
    const q7_t *kernel_data = depthwise_sub_block_weights;
    const q7_t *input_data = depthwise_sub_block_input;

    input_dims.n = DEPTHWISE_SUB_BLOCK_INPUT_BATCHES;
    input_dims.w = DEPTHWISE_SUB_BLOCK_INPUT_W;
    input_dims.h = DEPTHWISE_SUB_BLOCK_INPUT_H;
    input_dims.c = DEPTHWISE_SUB_BLOCK_IN_CH;
    filter_dims.w = DEPTHWISE_SUB_BLOCK_FILTER_X;
    filter_dims.h = DEPTHWISE_SUB_BLOCK_FILTER_Y;
    output_dims.w = DEPTHWISE_SUB_BLOCK_OUTPUT_W;
    output_dims.h = DEPTHWISE_SUB_BLOCK_OUTPUT_H;
    output_dims.c = DEPTHWISE_SUB_BLOCK_OUT_CH;

    dw_conv_params.padding.w = DEPTHWISE_SUB_BLOCK_PAD_X;
    dw_conv_params.padding.h = DEPTHWISE_SUB_BLOCK_PAD_Y;
    dw_conv_params.stride.w = DEPTHWISE_SUB_BLOCK_STRIDE_X;
    dw_conv_params.stride.h = DEPTHWISE_SUB_BLOCK_STRIDE_Y;
    dw_conv_params.dilation.w = DEPTHWISE_SUB_BLOCK_DILATION_X;
    dw_conv_params.dilation.h = DEPTHWISE_SUB_BLOCK_DILATION_Y;

    dw_conv_params.ch_mult = 1;

    dw_conv_params.input_offset = DEPTHWISE_SUB_BLOCK_INPUT_OFFSET;
    dw_conv_params.output_offset = DEPTHWISE_SUB_BLOCK_OUTPUT_OFFSET;
    dw_conv_params.activation.min = DEPTHWISE_SUB_BLOCK_OUT_ACTIVATION_MIN;
    dw_conv_params.activation.max = DEPTHWISE_SUB_BLOCK_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)depthwise_sub_block_output_mult;
    quant_params.shift = (int32_t *)depthwise_sub_block_output_shift;

    ctx.size = arm_depthwise_conv_s8_opt_get_buffer_size(&input_dims, &filter_dims);
    ctx.buf = malloc(ctx.size);

    arm_cmsis_nn_status result = arm_depthwise_conv_s8_opt(&ctx,
                                                           &dw_conv_params,
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
        memset(ctx.buf, 0, ctx.size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
    TEST_ASSERT_TRUE(validate(output, depthwise_sub_block_output_ref, DEPTHWISE_SUB_BLOCK_DST_SIZE));
}
