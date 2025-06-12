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

#ifndef ONERT_MICRO_EXECUTE_PAL_CONCATENATION_H
#define ONERT_MICRO_EXECUTE_PAL_CONCATENATION_H

#include "PALUtils.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{
template <typename Scalar>
OMStatus Concatenation(core::ConcatenationParams &params, std::vector<uint32_t> &input_shapes,
                       std::vector<const Scalar *> &input_data,
                       const core::OMRuntimeShape &output_shape, Scalar *output_data)
{
  int axis = params.axis;
  int inputs_count = params.num_inputs;
  const int concat_dimensions = output_shape.dimensionsCount();

  int64_t concat_size = 0;
  for (int i = 0; i < inputs_count; i++)
  {
    concat_size += input_shapes[i];
  }
  int64_t outer_size = 1;
  for (int i = 0; i < axis; ++i)
  {
    outer_size *= output_shape.dims(i);
  }
  // For all input arrays,
  int64_t base_inner_size = 1;
  for (int i = axis + 1; i < concat_dimensions; ++i)
  {
    base_inner_size *= output_shape.dims(i);
  }

  Scalar *output_ptr = output_data;
  for (int k = 0; k < outer_size; k++)
  {
    for (int i = 0; i < inputs_count; ++i)
    {
      const int copy_size = input_shapes[i] * base_inner_size;
      const Scalar *input_ptr = input_data[i] + k * copy_size;
      memcpy(output_ptr, input_ptr, copy_size * sizeof(Scalar));
      output_ptr += copy_size;
    }
  }

  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_CONCATENATION_H
