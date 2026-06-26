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

#ifndef ONERT_MICRO_EXECUTE_PAL_BATCH_TO_SPACE_ND_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_BATCH_TO_SPACE_ND_COMMON_H

#include "OMStatus.h"
#include "core/OMRuntimeShape.h"
#include "PALUtils.h"
#include "ProcessBroadcastShapes.h"
#include <cmath>

namespace onert_micro
{
namespace execute
{
namespace pal
{
namespace
{
inline core::OMRuntimeShape extendShapeBatchToSpace(const core::OMRuntimeShape &shape)
{
  if (shape.dimensionsCount() == 4)
  {
    return shape;
  }
  core::OMRuntimeShape new_shape(4, 1);
  new_shape.setDim(0, shape.dims(0));
  new_shape.setDim(1, shape.dims(1));
  new_shape.setDim(3, shape.dims(2));
  return new_shape;
}
} // namespace

template <typename T>
inline OMStatus
BatchToSpaceND(const core::OMRuntimeShape &unextended_input1_shape, const T *input1_data,
               const core::OMRuntimeShape &unextended_input2_shape, const int32_t *block_shape_data,
               const core::OMRuntimeShape &unextended_input3_shape, const int32_t *crops_data,
               const core::OMRuntimeShape &unextended_output_shape, T *output_data)
{
  const core::OMRuntimeShape input1_shape = extendShapeBatchToSpace(unextended_input1_shape);
  const core::OMRuntimeShape output_shape = extendShapeBatchToSpace(unextended_output_shape);

  const int output_width = output_shape.dims(2);
  const int output_height = output_shape.dims(1);
  const int output_batch_size = output_shape.dims(0);

  const int depth = input1_shape.dims(3);
  const int input_width = input1_shape.dims(2);
  const int input_height = input1_shape.dims(1);
  const int input_batch_size = input1_shape.dims(0);

  const int block_shape_height = block_shape_data[0];
  const int block_shape_width =
    unextended_input1_shape.dimensionsCount() == 4 ? block_shape_data[1] : 1;
  const int crops_top = crops_data[0];
  const int crops_left = unextended_input1_shape.dimensionsCount() == 4 ? crops_data[2] : 0;
  for (int in_batch = 0; in_batch < input_batch_size; ++in_batch)
  {
    const int out_batch = in_batch % output_batch_size;
    const int spatial_offset = in_batch / output_batch_size;
    for (int in_h = 0; in_h < input_height; ++in_h)
    {
      const int out_h = in_h * block_shape_height + spatial_offset / block_shape_width - crops_top;
      if (out_h < 0 || out_h >= output_height)
      {
        continue;
      }
      for (int in_w = 0; in_w < input_width; ++in_w)
      {
        const int out_w =
          in_w * block_shape_width + spatial_offset % block_shape_width - crops_left;

        if (out_w < 0 || out_w >= output_width)
        {
          continue;
        }
        T *out = output_data + offset(output_shape.dimsData(), out_batch, out_h, out_w, 0);
        const T *in = input1_data + offset(input1_shape.dimsData(), in_batch, in_h, in_w, 0);
        memcpy(out, in, depth * sizeof(T));
      }
    }
  }
  return Ok;
}
} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_BATCH_TO_SPACE_ND_COMMON_H
