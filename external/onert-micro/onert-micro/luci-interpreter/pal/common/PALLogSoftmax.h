/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_PAL_LOG_SOFTMAX_COMMON_H
#define LUCI_INTERPRETER_PAL_LOG_SOFTMAX_COMMON_H

#include "PALUtils.h"

#include <cmath>

namespace luci_interpreter_pal
{

inline void LogSoftmax(const luci_interpreter::RuntimeShape &input_shape, const float *input_data,
                       const luci_interpreter::RuntimeShape &output_shape, float *output_data)
{
  const int trailing_dim = input_shape.dimensionsCount() - 1;
  const int outer_size =
    flatSizeSkipDim(input_shape.dimsData(), trailing_dim, input_shape.dimensionsCount());
  const int depth = MatchingDim(input_shape, trailing_dim, output_shape, trailing_dim);

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
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_LOG_SOFTMAX_COMMON_H
