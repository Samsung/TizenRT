/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2020 The TensorFlow Authors. All Rights Reserved.
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

#ifndef ONERT_MICRO_EXECUTE_PAL_AVERAGE_POOL_2D_H
#define ONERT_MICRO_EXECUTE_PAL_AVERAGE_POOL_2D_H

#include "PALAveragePool2DCommon.h"

#include <arm_nnfunctions.h>

namespace onert_micro
{
namespace execute
{
namespace pal
{

OMStatus AveragePool(const core::Pool2DParams &params, const core::OMRuntimeShape &input_shape,
                     const int8_t *input_data, const core::OMRuntimeShape &output_shape,
                     int8_t *output_data)
{
  cmsis_nn_dims input_dims;
  cmsis_nn_dims output_dims;
  cmsis_nn_pool_params pool_params;
  cmsis_nn_dims filter_dims;
  cmsis_nn_context ctx;

  const int depth = input_shape.dims(3);
  const int output_width = output_shape.dims(2);

  input_dims.n = 1;
  input_dims.h = input_shape.dims(1);
  input_dims.w = input_shape.dims(2);
  input_dims.c = depth;

  output_dims.n = 1;
  output_dims.h = output_shape.dims(1);
  output_dims.w = output_width;
  output_dims.c = depth;

  pool_params.stride.h = params.stride_h;
  pool_params.stride.w = params.stride_w;
  pool_params.padding.h = params.pad_h;
  pool_params.padding.w = params.pad_w;
  pool_params.activation.min = params.quantized_activation_min;
  pool_params.activation.max = params.quantized_activation_max;

  filter_dims.n = 1;
  filter_dims.h = params.filter_h;
  filter_dims.w = params.filter_w;
  filter_dims.c = 1;

  const int32_t buffer_size = arm_avgpool_s8_get_buffer_size(output_width, depth);
  int8_t *buffer = nullptr;
  if (buffer_size > 0)
  {
    buffer = new int8_t[buffer_size];
  }

  ctx.buf = buffer;
  ctx.size = buffer_size;

  auto res = arm_avgpool_s8(&ctx, &pool_params, &input_dims, input_data, &filter_dims, &output_dims,
                            output_data);

  if (buffer_size > 0)
    delete[] buffer;

  assert(res == ARM_CMSIS_NN_SUCCESS);
  if (res != ARM_CMSIS_NN_SUCCESS)
    return CmsisNNError;

  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_AVERAGE_POOL_2D_H
