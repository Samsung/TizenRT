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

#ifndef LUCI_INTERPRETER_PAL_CONCATENATION_H
#define LUCI_INTERPRETER_PAL_CONCATENATION_H

#include "Params.h"
#include "PALUtils.h"

namespace luci_interpreter_pal
{

template <typename Scalar>
inline void Concatenation(const ConcatenationParams &params,
                          const luci_interpreter::RuntimeShape *const *input_shapes,
                          const Scalar *const *input_data,
                          const luci_interpreter::RuntimeShape &output_shape, Scalar *output_data)
{
  int axis = params.axis;
  int inputs_count = params.inputs_count;
  const int concat_dimensions = output_shape.dimensionsCount();

  int64_t concat_size = 0;
  for (int i = 0; i < inputs_count; i++)
  {
    concat_size += input_shapes[i]->dims(axis);
  }
  int64_t outer_size = 1;
  for (int i = 0; i < axis; ++i)
  {
    outer_size *= output_shape.dims(i);
  }
  // For all input arrays,
  // FlatSize() = outer_size * Dims(axis) * base_inner_size;
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
      const int copy_size = input_shapes[i]->dims(axis) * base_inner_size;
      const Scalar *input_ptr = input_data[i] + k * copy_size;
      memcpy(output_ptr, input_ptr, copy_size * sizeof(Scalar));
      output_ptr += copy_size;
    }
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_CONCATENATION_H
