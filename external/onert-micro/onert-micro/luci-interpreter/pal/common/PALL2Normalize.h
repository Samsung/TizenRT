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

#ifndef LUCI_INTERPRETER_PAL_L2_NORMALIZE_COMMON_H
#define LUCI_INTERPRETER_PAL_L2_NORMALIZE_COMMON_H

#include "PALUtils.h"
#include "tinyara/math.h"

namespace luci_interpreter_pal
{

inline void L2Normalization(const luci_interpreter::RuntimeShape &input_shape,
                            const float *input_data,
                            const luci_interpreter::RuntimeShape &output_shape, float *output_data,
                            float epsilon = 1e-6)
{
  const int trailing_dim = input_shape.dimensionsCount() - 1;
  const int outer_size =
    flatSizeSkipDim(input_shape.dimsData(), trailing_dim, input_shape.dimensionsCount());
  const int depth = MatchingDim(input_shape, trailing_dim, output_shape, trailing_dim);
  for (int i = 0; i < outer_size; ++i)
  {
    float squared_l2_norm = 0;
    for (int c = 0; c < depth; ++c)
    {
      const float val = input_data[depth * i + c];
      squared_l2_norm += val * val;
    }
    float l2_norm = std::sqrt(squared_l2_norm);
    l2_norm = fmax(l2_norm, epsilon);
    for (int c = 0; c < depth; ++c)
    {
      output_data[depth * i + c] = input_data[depth * i + c] / l2_norm;
    }
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_L2_NORMALIZE_COMMON_H
