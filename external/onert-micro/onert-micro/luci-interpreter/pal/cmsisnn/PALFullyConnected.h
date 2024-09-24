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

#ifndef LUCI_INTERPRETER_PAL_FULLY_CONNECTED_H
#define LUCI_INTERPRETER_PAL_FULLY_CONNECTED_H

#include "PALFullyConnectedCommon.h"

#include <arm_nnfunctions.h>

namespace luci_interpreter_pal
{

template <>
inline void FullyConnected<int8_t>(const luci_interpreter_pal::FullyConnectedParams &params,
                                   const int32_t *, const int8_t *input_data,
                                   const int32_t *filter_shape, const int8_t *filter_data,
                                   const int32_t *bias_data, const int32_t *output_shape,
                                   int8_t *output_data, uint32_t output_dims_count,
                                   uint32_t weights_dims_count)
{
  const int batches = flatSizeSkipDim(output_shape, output_dims_count - 1, output_dims_count);
  const int output_depth = output_shape[output_dims_count - 1];
  const int accum_depth = filter_shape[weights_dims_count - 1];

  cmsis_nn_fc_params fc_params;
  fc_params.input_offset = params.input_offset;
  fc_params.output_offset = params.output_offset;
  fc_params.filter_offset = params.weights_offset;
  fc_params.activation.min = params.quantized_activation_min;
  fc_params.activation.max = params.quantized_activation_max;

  cmsis_nn_per_tensor_quant_params quant_params;
  quant_params.multiplier = params.output_multiplier;
  quant_params.shift = params.output_shift;

  cmsis_nn_dims input_dims;
  input_dims.n = batches;
  input_dims.h = 1;
  input_dims.w = 1;
  input_dims.c = accum_depth;

  cmsis_nn_dims filter_dims;
  filter_dims.n = accum_depth;
  filter_dims.h = 1;
  filter_dims.w = 1;
  filter_dims.c = output_depth;

  cmsis_nn_dims bias_dims;
  bias_dims.n = 1;
  bias_dims.h = 1;
  bias_dims.w = 1;
  bias_dims.c = output_depth;

  cmsis_nn_dims output_dims;
  output_dims.n = batches;
  output_dims.h = 1;
  output_dims.w = 1;
  output_dims.c = output_depth;

  int32_t buf_size = arm_fully_connected_s8_get_buffer_size(&filter_dims);
  auto buffer = std::make_unique<int8_t[]>(buf_size);
  assert(buffer != nullptr);

  cmsis_nn_context ctx;
  ctx.buf = buffer.get();
  ctx.size = buf_size;

  auto res =
    arm_fully_connected_s8(&ctx, &fc_params, &quant_params, &input_dims, input_data, &filter_dims,
                           filter_data, &bias_dims, bias_data, &output_dims, output_data);
  assert(res == ARM_CMSIS_NN_SUCCESS);
}

template <>
inline void FullyConnected(const luci_interpreter_pal::FullyConnectedParams &params,
                           const int32_t *, const int16_t *input_data, const int32_t *filter_shape,
                           const int8_t *filter_data, const int64_t *bias_data,
                           const int32_t *output_shape, int16_t *output_data,
                           uint32_t output_dims_count, uint32_t weights_dims_count)
{
  const int batches = flatSizeSkipDim(output_shape, output_dims_count - 1, output_dims_count);
  const int output_depth = output_shape[output_dims_count - 1];
  const int accum_depth = filter_shape[weights_dims_count - 1];

  cmsis_nn_fc_params fc_params;
  fc_params.input_offset = params.input_offset;
  fc_params.output_offset = params.output_offset;
  fc_params.filter_offset = params.weights_offset;
  fc_params.activation.min = params.quantized_activation_min;
  fc_params.activation.max = params.quantized_activation_max;

  cmsis_nn_per_tensor_quant_params quant_params;
  quant_params.multiplier = params.output_multiplier;
  quant_params.shift = params.output_shift;

  cmsis_nn_dims input_dims;
  input_dims.n = batches;
  input_dims.h = 1;
  input_dims.w = 1;
  input_dims.c = accum_depth;

  cmsis_nn_dims filter_dims;
  filter_dims.n = accum_depth;
  filter_dims.h = 1;
  filter_dims.w = 1;
  filter_dims.c = output_depth;

  cmsis_nn_dims bias_dims;
  bias_dims.n = 1;
  bias_dims.h = 1;
  bias_dims.w = 1;
  bias_dims.c = output_depth;

  cmsis_nn_dims output_dims;
  output_dims.n = batches;
  output_dims.h = 1;
  output_dims.w = 1;
  output_dims.c = output_depth;

  int32_t buf_size = arm_fully_connected_s16_get_buffer_size(&filter_dims);
  auto buffer = std::make_unique<int8_t[]>(buf_size);
  assert(buffer != nullptr);

  cmsis_nn_context ctx;
  ctx.buf = buffer.get();
  ctx.size = buf_size;

  auto res =
    arm_fully_connected_s16(&ctx, &fc_params, &quant_params, &input_dims, input_data, &filter_dims,
                            filter_data, &bias_dims, bias_data, &output_dims, output_data);
  assert(res == ARM_CMSIS_NN_SUCCESS);
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_FULLY_CONNECTED_H
