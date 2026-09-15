/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2017 The TensorFlow Authors. All Rights Reserved.
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

#ifndef ONERT_MICRO_EXECUTE_PAL_ADD_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_ADD_COMMON_H

#include "PALArithmeticOpCommon.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{

int8_t AddFunc(int8_t x, int8_t y, const core::ArithmeticQuantParams &params)
{
  const int32_t input1_val = params.input1_offset + x;
  const int32_t input2_val = params.input2_offset + y;
  const int32_t shifted_input1_val = input1_val * (1 << params.left_shift);
  const int32_t shifted_input2_val = input2_val * (1 << params.left_shift);
  const int32_t scaled_input1_val = multiplyByQuantizedMultiplierSmallerThanOneExp(
    shifted_input1_val, params.input1_multiplier, params.input1_shift);
  const int32_t scaled_input2_val = multiplyByQuantizedMultiplierSmallerThanOneExp(
    shifted_input2_val, params.input2_multiplier, params.input2_shift);
  const int32_t raw_sum = scaled_input1_val + scaled_input2_val;
  const int32_t raw_output = multiplyByQuantizedMultiplierSmallerThanOneExp(
                               raw_sum, params.output_multiplier, params.output_shift) +
                             params.output_offset;
  const int32_t clamped_output = std::min(params.quantized_activation_max,
                                          std::max(params.quantized_activation_min, raw_output));
  return static_cast<int8_t>(clamped_output);
}

template <typename T>
OMStatus Add(const core::BinaryArithmeticBroadcastParams &params, const int flat_size,
             const T *input1_data, const T *input2_data, T *output_data)
{
  ArithmeticOp<T, AddFn<T>>(params, flat_size, input1_data, input2_data, output_data);
  return Ok;
}

template <typename T>
OMStatus BroadcastAdd4DSlow(const core::BinaryArithmeticBroadcastParams &params,
                            const core::OMRuntimeShape &input1_shape, const T *input1_data,
                            const core::OMRuntimeShape &input2_shape, const T *input2_data,
                            const core::OMRuntimeShape &output_shape, T *output_data)
{
  BroadcastArithmeticOp4DSlow<T, AddFn<T>>(params, input1_shape, input1_data, input2_shape,
                                           input2_data, output_shape, output_data);
  return Ok;
}

OMStatus BroadcastAdd4DSlow(const core::ArithmeticQuantParams &params,
                            const core::OMRuntimeShape &input1_shape, const int8_t *input1_data,
                            const core::OMRuntimeShape &input2_shape, const int8_t *input2_data,
                            const core::OMRuntimeShape &output_shape, int8_t *output_data)
{
  BroadcastBinaryFunction6DSlow(params, input1_shape, input1_data, input2_shape, input2_data,
                                output_shape, output_data, AddFunc);
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_ADD_COMMON_H
