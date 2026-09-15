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

#ifndef ONERT_MICRO_PAL_UNPACK_COMMON_H
#define ONERT_MICRO_PAL_UNPACK_COMMON_H

#include "core/OMKernelData.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{

template <typename T>
OMStatus Unpack(const core::SplitParams &params, const core::OMRuntimeShape &input_shape,
                const T *input_data, const core::OMRuntimeShape &output_shape, int32_t axis_value)
{
  const auto dimensions = input_shape.dimensionsCount();

  if (axis_value < 0)
  {
    axis_value += dimensions;
  }

  int outer_size = 1;
  for (int i = 0; i < axis_value; ++i)
  {
    outer_size *= input_shape.dims(i);
  }
  int copy_size = 1;
  for (int i = axis_value + 1; i < dimensions; ++i)
  {
    copy_size *= input_shape.dims(i);
  }
  int output_size = 1;
  for (int i = 0; i < output_shape.dimensionsCount(); ++i)
  {
    output_size *= output_shape.dims(i);
  }

  for (int i = 0; i < params.num_outputs; ++i)
  {
    T *output_data = core::utils::castOutputData<T>(params.output_data[i]);
    assert(output_data != nullptr);
    for (int k = 0; k < outer_size; ++k)
    {
      T *output_ptr = output_data + copy_size * k;
      int loc = k * params.num_outputs * copy_size + i * copy_size;
      const T *input_ptr = input_data + loc;
      for (int j = 0; j < copy_size; ++j)
        output_ptr[j] = input_ptr[j];
    }
  }
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_PAL_UNPACK_COMMON_H
