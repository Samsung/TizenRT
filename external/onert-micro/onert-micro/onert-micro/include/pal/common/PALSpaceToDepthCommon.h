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

#ifndef ONERT_MICRO_EXECUTE_PAL_SPACE_TO_DEPTH_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_SPACE_TO_DEPTH_COMMON_H

#include "OMStatus.h"
#include "core/OMRuntimeShape.h"
#include "PALUtils.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{
template <typename T>
inline OMStatus SpaceToDepth(const int32_t block_size,
                             const core::OMRuntimeShape &unextended_input_shape,
                             const T *input_data,
                             const core::OMRuntimeShape &unextended_output_shape, T *output_data)
{
  if (block_size == 0)
  {
    return FailedCheckCondition;
  }

  const core::OMRuntimeShape input_shape =
    core::OMRuntimeShape::extendedShape(4, unextended_input_shape);
  const core::OMRuntimeShape output_shape =
    core::OMRuntimeShape::extendedShape(4, unextended_output_shape);

  const int input_depth = input_shape.dims(3);
  const int input_width = input_shape.dims(2);
  const int input_height = input_shape.dims(1);
  const int input_batch = input_shape.dims(0);

  for (int in_b = 0; in_b < input_batch; ++in_b)
  {
    for (int in_h = 0; in_h < input_height; ++in_h)
    {
      for (int in_w = 0; in_w < input_width; ++in_w)
      {
        for (int in_d = 0; in_d < input_depth; ++in_d)
        {
          const int out_d =
            in_d + ((in_h % block_size) * block_size + in_w % block_size) * input_depth;
          const int out_w = in_w / block_size;
          const int out_h = in_h / block_size;
          const int out_b = in_b;

          const int input_index = offset(input_shape.dimsData(), in_b, in_h, in_w, in_d);
          const int output_index = offset(output_shape.dimsData(), out_b, out_h, out_w, out_d);

          output_data[output_index] = input_data[input_index];
        }
      }
    }
  }
  return Ok;
}
} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_SPACE_TO_DEPTH_COMMON_H
