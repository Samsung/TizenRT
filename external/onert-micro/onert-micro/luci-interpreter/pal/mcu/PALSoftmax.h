/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2017 The TensorFlow Authors. All Rights Reserved.
 * Copyright 2015 The Gemmlowp Authors. All Rights Reserved.
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

#ifndef LUCI_INTERPRETER_PAL_SOFTMAX_H
#define LUCI_INTERPRETER_PAL_SOFTMAX_H

#include "PALSoftmaxCommon.h"
#include "PALUtils.h"

namespace luci_interpreter_pal
{

inline void Softmax(const SoftmaxParams &params, const int8_t *input_data, int8_t *output_data)
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

    static constexpr int32_t min_val = std::numeric_limits<int8_t>::min();
    static constexpr int32_t max_val = std::numeric_limits<int8_t>::max();
    // Compute sum.
    // Compute sum.
    float sum = 0.f;
    for (int c = 0; c < depth; ++c)
    {
      float cur_val = static_cast<float>(input_data[i * depth + c] - input_zp) * input_scale;
      const float exp_c = std::exp((cur_val - max) * beta);
      sum += exp_c;
    }

    // Compute result.
    for (int c = 0; c < depth; ++c)
    {
      float cur_val = static_cast<float>(input_data[i * depth + c] - input_zp) * input_scale;
      const float exp_c = std::exp((cur_val - max) * beta);
      float softmax_val = exp_c / sum;
      int32_t unclamped = static_cast<int32_t>(std::round(softmax_val / output_scale) + output_zp);
      int32_t clamped = std::min(std::max(unclamped, min_val), max_val);
      output_data[i * depth + c] = static_cast<int8_t>(clamped);
    }
  }
}

inline void Softmax(const SoftmaxParams &params, const int8_t *input_data, int16_t *output_data)
{
  assert(false && "Not supported now");
}

inline void Softmax(const SoftmaxParams &params, const int16_t *input_data, int16_t *output_data)
{
  assert(false && "Not supported now");
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_SOFTMAX_H
