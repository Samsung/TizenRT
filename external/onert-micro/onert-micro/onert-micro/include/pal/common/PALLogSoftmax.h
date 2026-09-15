/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2021 The TensorFlow Authors. All Rights Reserved.
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

#ifndef ONERT_MICRO_EXECUTE_PAL_LOG_SOFTMAX_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_LOG_SOFTMAX_COMMON_H

#include "core/OMKernelData.h"

#include <cmath>

namespace onert_micro
{
namespace execute
{
namespace pal
{

inline OMStatus LogSoftmax(const core::LogSoftmaxParams &params, const float *input_data,
                           float *output_data)
{
  const int outer_size = params.num_rows;
  const int depth = params.row_size;

  for (int i = 0; i < outer_size; ++i)
  {
    // Find max element value which we'll use to ensure numerical stability
    // taking advantage of the following equality:
    // log(exp(x[i])/sum(exp(x[i]))) == log(exp(x[i]+C)/sum(exp(x[i]+C)))
    float max = std::numeric_limits<float>::lowest();
    for (int c = 0; c < depth; ++c)
    {
      max = std::max(max, input_data[i * depth + c]);
    }

    // Compute sum.
    float sum = 0.f;
    for (int c = 0; c < depth; ++c)
    {
      sum += std::exp(input_data[i * depth + c] - max);
    }

    // Compute result.
    const float log_sum = std::log(sum);
    for (int c = 0; c < depth; ++c)
    {
      output_data[i * depth + c] = input_data[i * depth + c] - max - log_sum;
    }
  }

  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_LOG_SOFTMAX_COMMON_H
