/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2019 The TensorFlow Authors. All Rights Reserved.
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

#ifndef ONERT_MICRO_EXECUTE_PAL_MUL_H
#define ONERT_MICRO_EXECUTE_PAL_MUL_H

#include "PALMulCommon.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{

template <typename InputType, typename OutputType>
OMStatus Mul(const core::ArithmeticQuantParams &params, uint32_t size, const InputType *input1_data,
             const InputType *input2_data, OutputType *output_data)
{
  for (int i = 0; i < size; ++i)
  {
    const int32_t input1_val = params.input1_offset + input1_data[i];
    const int32_t input2_val = params.input2_offset + input2_data[i];
    const int32_t unclamped_result =
      params.output_offset + multiplyByQuantizedMultiplier(input1_val * input2_val,
                                                           params.output_multiplier,
                                                           params.output_shift);
    const int32_t clamped_output = std::min(
      params.quantized_activation_max, std::max(params.quantized_activation_min, unclamped_result));
    output_data[i] = static_cast<OutputType>(clamped_output);
  }
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_MUL_H
