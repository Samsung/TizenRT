/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_PAL_SOFTMAX_COMMON_H
#define LUCI_INTERPRETER_PAL_SOFTMAX_COMMON_H

#include "Params.h"

namespace luci_interpreter_pal
{
inline void Softmax(const SoftmaxParams &params, const float *input_data, float *output_data)
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

    // Compute result.
    for (int c = 0; c < depth; ++c)
    {
      output_data[i * depth + c] = output_data[i * depth + c] / sum;
    }
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_SOFTMAX_COMMON_H
