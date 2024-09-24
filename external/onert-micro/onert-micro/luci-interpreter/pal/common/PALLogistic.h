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

#ifndef LUCI_INTERPRETER_PAL_LOGISTIC_H
#define LUCI_INTERPRETER_PAL_LOGISTIC_H

#include "Params.h"
#include "PALUtils.h"

namespace luci_interpreter_pal
{

inline void Logistic(const int flat_size, const float *input_data, float *output_data)
{
  const float cutoff_upper = 16.619047164916992188f;
  const float cutoff_lower = -9.f;

  // Rational for using approximation in reference kernel.
  // 0. This approximation gives enough precision for float.
  // 1. This works around an issue on an embedded chipset where exp() does not
  // return correctly as expected - exp(x) should return inf when overflown
  // not 1.701417   IEEE 754 defines representation for inf.
  // 2. This will speed up calculation and is matching the behavior in the
  // optimized kernels. (check the definition of scalar_logistic_op<float>)

  for (int i = 0; i < flat_size; i++)
  {
    float val = input_data[i];
    float result;
    if (val > cutoff_upper)
    {
      result = 1.0f;
    }
    else if (val < cutoff_lower)
    {
      result = std::exp(val);
    }
    else
    {
      result = 1.f / (1.f + std::exp(-val));
    }
    output_data[i] = result;
  }
}

inline void Logistic(const int flat_size, const int8_t *input_data, float input_scale,
                     int input_zero_point, int8_t *output_data, float output_scale,
                     int output_zero_point)
{
  const float cutoff_upper = 16.619047164916992188f;
  const float cutoff_lower = -9.f;
  const int32_t min_val = std::numeric_limits<int8_t>::min();
  const int32_t max_val = std::numeric_limits<int8_t>::max();

  for (int i = 0; i < flat_size; i++)
  {
    // Dequantize.
    float val = static_cast<float>((input_data[i] - input_zero_point) * input_scale);
    float result;
    if (val > cutoff_upper)
    {
      result = 1.0f;
    }
    else if (val < cutoff_lower)
    {
      result = 0.0f; // Для значений меньше cutoff_lower результат близок к 0.
    }
    else
    {
      result = 1.f / (1.f + std::exp(-val));
    }
    // Requantize
    int32_t unclamped = static_cast<int32_t>(std::round(result / output_scale) + output_zero_point);
    int32_t clamped = std::min(std::max(unclamped, min_val), max_val);
    output_data[i] = static_cast<int8_t>(clamped);
  }
}

inline void Logistic(int32_t input_multiplier, int32_t input_left_shift, int32_t input_size,
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

  for (int i = 0; i < input_size; ++i, ptr_input_data++, ptr_output_data++)
  {
    int32_t input_data = ((*ptr_input_data) * input_multiplier + round) >> input_left_shift;

    // We do interpolation on unsigned values.
    uint32_t abs_input_data = abs(input_data);

    // We divide by 2 power of 9, because
    // we need to divide by 2 in power of 7 for
    // the input conversion + 1/4 from the scale above.

    // Define uh as uint32_t type not to make this function overflow.
    uint32_t uh = abs_input_data >> 9;
    uint32_t result;

    if (uh >= 255)
    {
      // Saturate to maximum.
      result = 0x7FFF << 10;
    }
    else
    {
      uint32_t ua = sigmoid_table_uint16[uh];
      uint32_t ub = sigmoid_table_uint16[uh + 1];
      uint32_t ut = abs_input_data & 0x1ff;
      // Interpolation is done using the fractional bit.
      result = (ua << 9) + ut * (ub - ua);
    }

    result = (input_data >= 0) ? (result + (1 << 9)) : ((1 << (16 + 9)) - result + (1 << 9) - 1);

    // Back to 16-bit.
    result >>= 10;

    *ptr_output_data = result;
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_LOGISTIC_H
