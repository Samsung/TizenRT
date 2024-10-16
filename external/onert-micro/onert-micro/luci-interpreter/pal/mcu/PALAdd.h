/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_PAL_ADD_H
#define LUCI_INTERPRETER_PAL_ADD_H

#include "PALAddCommon.h"

namespace luci_interpreter_pal
{
namespace
{

inline int8_t AddFunc(int8_t x, int8_t y, const ArithmeticParams& params)
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

} // namespace

template <>
inline void Add<int8_t>(const ArithmeticParams &params,  const int flat_size, const int8_t *input_1, const int8_t *input_2,
                        int8_t *output)
{
  for (int i = 0; i < flat_size; ++i)
  {
    output[i] = AddFunc(input_1[i], input_2[i], params);
  }
}

template <>
inline void Add<int16_t>(const ArithmeticParams &, const int, const int16_t *, const int16_t *,
                         int16_t *)
{
  assert(false && "Not IMPL yet");
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_ADD_H
