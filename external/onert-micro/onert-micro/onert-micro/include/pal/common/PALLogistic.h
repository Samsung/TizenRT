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

#ifndef ONERT_MICRO_EXECUTE_PAL_LOGISTIC_H
#define ONERT_MICRO_EXECUTE_PAL_LOGISTIC_H

#include "PALUtils.h"

#include <cmath>

namespace onert_micro
{
namespace execute
{
namespace pal
{

OMStatus inline Logistic(const int flat_size, const float *input_data, float *output_data)
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
  return Ok;
}

OMStatus inline Logistic(const int flat_size, const int8_t *input_data, float input_scale,
                         int input_zero_point, int8_t *output_data, float output_scale,
                         int output_zero_point)
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
    // Dequantize.
    float val = static_cast<float>((input_data[i] - input_zero_point) * input_scale);
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
    // Requantize
    int8_t output = static_cast<int8_t>(std::round(result / output_scale) + output_zero_point);
    output_data[i] = output;
  }
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_LOGISTIC_H
