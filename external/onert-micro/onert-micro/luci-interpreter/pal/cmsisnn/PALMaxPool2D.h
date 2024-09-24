/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_PAL_CMSIS_NN_MAX_POOL_2D_H
#define LUCI_INTERPRETER_PAL_CMSIS_NN_MAX_POOL_2D_H

#include "PALMaxPool2DCommon.h"

#include <arm_nnfunctions.h>

namespace luci_interpreter_pal
{

inline void MaxPool(const PoolParams &params, const luci_interpreter::RuntimeShape &input_shape,
                    const uint8_t *input_data, const luci_interpreter::RuntimeShape &output_shape,
                    uint8_t *output_data, luci_interpreter::DataType data_type)
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

  pool_params.stride.h = params.stride_height;
  pool_params.stride.w = params.stride_width;
  pool_params.padding.h = params.padding_values.height;
  pool_params.padding.w = params.padding_values.width;
  pool_params.activation.min = params.quantized_activation_min;
  pool_params.activation.max = params.quantized_activation_max;

  filter_dims.n = 1;
  filter_dims.h = params.filter_height;
  filter_dims.w = params.filter_width;
  filter_dims.c = 1;

  if (data_type == luci_interpreter::DataType::S8)
  {
    arm_max_pool_s8(&ctx, &pool_params, &input_dims,
                    luci_interpreter::kernels::getTensorData<int8_t>(input_data), &filter_dims,
                    &output_dims, luci_interpreter::kernels::getTensorData<int8_t>(output_data));
  }
  else
  {
    arm_max_pool_s16(&ctx, &pool_params, &input_dims,
                     luci_interpreter::kernels::getTensorData<int16_t>(input_data), &filter_dims,
                     &output_dims, luci_interpreter::kernels::getTensorData<int16_t>(output_data));
  }
}
} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_CMSIS_NN_MAX_POOL_2D_H
