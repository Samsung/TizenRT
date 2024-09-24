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

#ifndef LUCI_INTERPRETER_PAL_TANH_H
#define LUCI_INTERPRETER_PAL_TANH_H

#include "PALUtils.h"

namespace luci_interpreter_pal
{

inline void Tanh(const int flat_size, const float *input_data, float *output_data)
{
  for (int i = 0; i < flat_size; i++)
  {
    float val = input_data[i];
    float result = std::tanh(val);
    output_data[i] = result;
  }
}

inline void Tanh(int32_t input_multiplier, int32_t input_left_shift, const int flat_size,
                 const int16_t *ptr_input_data, int16_t *ptr_output_data)
{
  // We use the LUT for sigmoid and take into account, that
  // tanh(x) = 2*sigmoid(2*x) - 1

  // We scale by 3/4 to expand range [-8,8]->[-10.7,10.7].
  // In case of general parameter scale, multiplier 3 is taken into account
  // in TanhPrepare function and it is included in
  // input_multiplier already.

  if (input_multiplier == 0)
  { // power of two case
    input_multiplier = 3 << input_left_shift;
    input_left_shift = 0;
  }

  int32_t round = (input_left_shift > 0) ? 1 << (input_left_shift - 1) : 0;

  for (int i = 0; i < flat_size; ++i, ptr_input_data++, ptr_output_data++)
  {
    int32_t input_data = ((*ptr_input_data) * input_multiplier + round) >> input_left_shift;

    uint32_t abs_input_data = abs(input_data);
    uint32_t uh = abs_input_data >> 8;
    int32_t result;

    if (uh >= 255)
    {
      // Saturate to maximum.
      result = 0xFFFF << 8;
    }
    else
    {
      uint32_t ua = sigmoid_table_uint16[uh];
      uint32_t ub = sigmoid_table_uint16[uh + 1];

      uint8_t ut = abs_input_data & 0xFF;

      result = (ua << 8) + ut * (ub - ua);
    }

    result = (input_data >= 0) ? (result - (1 << (14 + 9)) + (1 << (9 - 2)))
                               : (-result + (1 << (14 + 9)) + (1 << (9 - 2)) - 1);

    // Convert back to 16-bit.
    result >>= (9 - 1);

    *ptr_output_data = result;
  }
}

inline void Tanh(int32_t input_zero_point, float input_scale,
                 int32_t output_zero_point, float output_scale,
                 const int flat_size, const int8_t* input_data, int8_t* output_data) {
    static constexpr int32_t min_val = std::numeric_limits<int8_t>::min();
    static constexpr int32_t max_val = std::numeric_limits<int8_t>::max();
    for (int i = 0; i < flat_size; i++)
    {
        float val = static_cast<float>(input_data[i] - input_zero_point) * input_scale;
        float result = std::tanh(val);
        int32_t unclamped = static_cast<int32_t>(std::round(result / output_scale) + output_zero_point);
        int32_t clamped = std::min(std::max(unclamped, min_val), max_val);
        output_data[i] = static_cast<int8_t>(clamped);
    }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_TANH_H
