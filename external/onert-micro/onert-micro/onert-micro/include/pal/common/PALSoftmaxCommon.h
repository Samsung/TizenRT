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

#ifndef ONERT_MICRO_EXECUTE_PAL_SOFTMAX_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_SOFTMAX_COMMON_H

#include "core/OMKernelData.h"

#include <cmath>

namespace onert_micro
{
namespace execute
{
namespace pal
{

template <typename T, typename U>
OMStatus Softmax(const core::SoftmaxParams &params, const T *input_data, U *output_data)
{
  const int outer_size = params.num_rows;
  const int depth = params.row_size;
  const double beta = params.beta;

  const float input_scale = params.input_scale;
  const float output_scale = params.output_scale;

  const int input_zp = params.input_zp;
  const int output_zp = params.output_zp;

  for (int i = 0; i < outer_size; ++i)
  {
    // Find max element value which we'll use to ensure numerical stability
    // taking advantage of the following equality:
    // exp(x[i])/sum(exp(x[i])) == exp(x[i]+C)/sum(exp(x[i]+C))
    float max = std::numeric_limits<float>::lowest();
    for (int c = 0; c < depth; ++c)
    {
      auto t = input_data[i * depth + c] - input_zp;
      auto t_f = static_cast<float>(input_data[i * depth + c] - input_zp);
      float cur_val = static_cast<float>(input_data[i * depth + c] - input_zp) * input_scale;
      max = std::max(max, cur_val);
    }

    static constexpr int32_t min_val = std::numeric_limits<U>::min();
    static constexpr int32_t max_val = std::numeric_limits<U>::max();
    // Compute sum.
    float sum = 0.f;
    for (int c = 0; c < depth; ++c)
    {
      float cur_val = static_cast<float>(input_data[i * depth + c] - input_zp) * input_scale;
      const auto exp_c = static_cast<float>(std::exp((cur_val - max) * beta));
      sum += exp_c;
    }

    // Compute result.
    for (int c = 0; c < depth; ++c)
    {
      float cur_val = static_cast<float>(input_data[i * depth + c] - input_zp) * input_scale;
      const auto exp_c = static_cast<float>(std::exp((cur_val - max) * beta));
      float softmax_val = exp_c / sum;
      auto unclamped = static_cast<int32_t>(std::round(softmax_val / output_scale) +
                                            static_cast<float>(output_zp));
      int32_t clamped = std::min(std::max(unclamped, min_val), max_val);
      output_data[i * depth + c] = static_cast<U>(clamped);
    }
  }
  return Ok;
}

template <>
OMStatus Softmax<float, float>(const core::SoftmaxParams &params, const float *input_data,
                               float *output_data)
{
  const int outer_size = params.num_rows;
  const int depth = params.row_size;
  const double beta = params.beta;

  for (int i = 0; i < outer_size; ++i)
  {
    // Find max element value which we'll use to ensure numerical stability
    // taking advantage of the following equality:
    // exp(x[i])/sum(exp(x[i])) == exp(x[i]+C)/sum(exp(x[i]+C))
    float max = std::numeric_limits<float>::lowest();
    for (int c = 0; c < depth; ++c)
    {
      max = std::max(max, input_data[i * depth + c]);
    }

    // Compute sum.
    float sum = 0.f;
    for (int c = 0; c < depth; ++c)
    {
      const float exp_c = std::exp((input_data[i * depth + c] - max) * static_cast<float>(beta));
      output_data[i * depth + c] = exp_c;
      sum += exp_c;
    }

    assert(sum != 0);

    if (sum == 0)
      return UnknownError;

    // Compute result.
    for (int c = 0; c < depth; ++c)
    {
      output_data[i * depth + c] = output_data[i * depth + c] / sum;
    }
  }
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_SOFTMAX_COMMON_H
