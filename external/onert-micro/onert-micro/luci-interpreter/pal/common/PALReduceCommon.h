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

#ifndef LUCI_INTERPRETER_PAL_TANH_H
#define LUCI_INTERPRETER_PAL_TANH_H

#include "PALUtils.h"

namespace luci_interpreter_pal
{
namespace
{
// This method parses the input 'axis' to remove duplicates and handle negative
// values, and returns a valid 'out_axis'
inline bool resolveAxis(const int num_dims, const int *axis, const int64_t num_axis,
                        int *out_num_axis)
{
  int out_axis[2];
  *out_num_axis = 0; // Just in case.
  // Short-circuit axis resolution for scalars; the axis will go unused.
  if (num_dims == 0)
  {
    return true;
  }
  // o(n^2) is fine since out_num_axis should be really small, mostly <= 4
  for (int64_t idx = 0; idx < num_axis; ++idx)
  {
    // Handle negative index. A positive index 'p_idx' can be represented as a
    // negative index 'n_idx' as: n_idx = p_idx-num_dims
    // eg: For num_dims=3, [0, 1, 2] is the same as [-3, -2, -1]  */
    int current = axis[idx] < 0 ? (axis[idx] + num_dims) : axis[idx];
    if (current < 0 || current >= num_dims)
    {
      return false;
    }
    bool is_dup = false;
    for (int j = 0; j < *out_num_axis; ++j)
    {
      if (out_axis[j] == current)
      {
        is_dup = true;
        break;
      }
    }
    if (!is_dup)
    {
      out_axis[*out_num_axis] = current;
      *out_num_axis += 1;
    }
  }
  return true;
}

} // namespace

// Computes the generic value (i.e., sum/max/min/prod) of elements across
// dimensions given in axis. It needs to pass in init_value and reducer.
template <typename T>
inline void ReduceGeneric(const T *input_data, const int *input_dims, const int input_num_dims,
                          T *output_data, const int *axis, const int64_t num_axis_dimensions,
                          T init_value, const int output_flat_size, T reducer(const T, const T))
{
  // Return early when input shape has zero dim.
  for (int i = 0; i < input_num_dims; ++i)
  {
    if (input_dims[i] == 0)
      return;
  }

  for (size_t idx = 0; idx < output_flat_size; ++idx)
  {
    output_data[idx] = init_value;
  }

  // Resolve axis.
  int num_resolved_axis = 0;
  if (!resolveAxis(input_num_dims, axis, num_axis_dimensions, &num_resolved_axis))
  {
    return;
  }

  int temp_index[5];
  // Reset input iterator.
  for (int idx = 0; idx < input_num_dims; ++idx)
  {
    temp_index[idx] = 0;
  }
  // Iterate through input_data.
  do
  {
    size_t input_offset = reducedOutputOffset(input_num_dims, input_dims, temp_index, 0, nullptr);
    size_t output_offset =
      reducedOutputOffset(input_num_dims, input_dims, temp_index, num_resolved_axis, axis);
    output_data[output_offset] = reducer(output_data[output_offset], input_data[input_offset]);
  } while (nextIndex(input_num_dims, input_dims, temp_index));
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_TANH_H
