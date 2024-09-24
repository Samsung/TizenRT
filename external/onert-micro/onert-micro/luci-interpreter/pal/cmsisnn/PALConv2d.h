/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LUCI_INTERPRETER_PAL_CONV2D_H
#define LUCI_INTERPRETER_PAL_CONV2D_H

#include "PALConv2DCommon.h"

#include <arm_nnfunctions.h>

namespace luci_interpreter_pal
{

static inline void QuantizedConvPerChannel(const ConvParams &params, const int32_t *input_shape,
                                           const int8_t *input_data, const int32_t *filter_shape,
                                           const int8_t *filter_data, const int32_t *bias_data,
                                           const int32_t *output_shape, int8_t *output_data)
{
  cmsis_nn_conv_params conv_params;
  conv_params.dilation.h = params.dilation_height_factor;
  conv_params.dilation.w = params.dilation_width_factor;

  assert(conv_params.dilation.h == 1);
  assert(conv_params.dilation.w == 1);

  conv_params.input_offset = params.input_offset;
  conv_params.output_offset = params.output_offset;
  conv_params.stride.h = params.stride_height;
  conv_params.stride.w = params.stride_width;
  conv_params.padding.h = params.padding_values.height;
  conv_params.padding.w = params.padding_values.width;
  conv_params.activation.min = params.quantized_activation_min;
  conv_params.activation.max = params.quantized_activation_max;

  cmsis_nn_per_channel_quant_params quant_params;
  quant_params.multiplier = const_cast<int32_t *>(params.per_channel_output_multiplier.data());
  quant_params.shift = const_cast<int32_t *>(
    reinterpret_cast<const int32_t *>(params.per_channel_output_shift.data()));

  assert(conv_params.activation.min <= conv_params.activation.max);
  const int batch_size = input_shape[0];
  const int input_depth = input_shape[3];
  const int output_depth = filter_shape[0];

  cmsis_nn_dims input_dims;
  input_dims.n = batch_size;
  input_dims.h = input_shape[1];
  input_dims.w = input_shape[2];
  input_dims.c = input_depth;

  cmsis_nn_dims filter_dims;
  filter_dims.n = output_depth;
  filter_dims.h = filter_shape[1];
  filter_dims.w = filter_shape[2];
  filter_dims.c = input_depth;

  cmsis_nn_dims bias_dims;
  bias_dims.n = 1;
  bias_dims.h = 1;
  bias_dims.w = 1;
  bias_dims.c = output_depth;

  cmsis_nn_dims output_dims;
  output_dims.n = batch_size;
  output_dims.h = output_shape[1];
  output_dims.w = output_shape[2];
  output_dims.c = output_depth;

  auto buf_size =
    arm_convolve_wrapper_s8_get_buffer_size(&conv_params, &input_dims, &filter_dims, &output_dims);

  auto buffer = std::make_unique<int8_t[]>(buf_size);
  assert(buffer != nullptr);

  cmsis_nn_context ctx;
  ctx.buf = buffer.get();
  ctx.size = buf_size;

  auto res = arm_convolve_wrapper_s8(&ctx, &conv_params, &quant_params, &input_dims, input_data,
                                     &filter_dims, filter_data, &bias_dims, bias_data, &output_dims,
                                     output_data);

  assert(res == ARM_CMSIS_NN_SUCCESS);
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_CONV2D_H
