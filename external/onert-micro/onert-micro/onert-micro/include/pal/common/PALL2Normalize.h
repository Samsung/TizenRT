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

#ifndef ONERT_MICRO_L2_NORMALIZE_COMMON_H
#define ONERT_MICRO_L2_NORMALIZE_COMMON_H

#include "core/OMKernelData.h"
#include "PALUtils.h"

#include <cmath>

namespace onert_micro
{
namespace execute
{
namespace pal
{

inline OMStatus L2Normalization(const core::L2NormalizationParams &params, const float *input_data,
                                float *output_data)
{

  const int outer_size = params.num_rows;
  const int depth = params.row_size;
  const int epsilon = params.epsilon;

  for (int i = 0; i < outer_size; ++i)
  {
    float squared_l2_norm = 0;
    for (int c = 0; c < depth; ++c)
    {
      const float val = input_data[depth * i + c];
      squared_l2_norm += val * val;
    }
    float l2_norm = std::sqrt(squared_l2_norm);
    l2_norm = std::max(l2_norm, static_cast<float>(epsilon));
    for (int c = 0; c < depth; ++c)
    {
      output_data[depth * i + c] = input_data[depth * i + c] / l2_norm;
    }
  }

  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_L2_NORMALIZE_COMMON_H
