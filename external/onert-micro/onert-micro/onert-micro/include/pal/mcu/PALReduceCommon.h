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

#ifndef ONERT_MICRO_PAL_TANH_H
#define ONERT_MICRO_PAL_TANH_H

#include "PALUtils.h"
#include "core/OMRuntimeShape.h"

using namespace onert_micro::core;

namespace onert_micro
{
namespace execute
{
namespace pal
{

// This method parses the input 'axis' to remove duplicates and handle negative
// values, and returns a valid 'out_axis'
inline bool resolveAxis(const int num_dims, const int *axis, const int64_t num_axis, int *out_axis,
                        int *out_num_axis)
{
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
      if (*out_num_axis > 1) {
        return false;
      }
      out_axis[*out_num_axis] = current;
      *out_num_axis += 1;
    }
  }
  return true;
}

// Computes the generic value (i.e., sum/max/min/prod) of elements across
// dimensions given in axis. It needs to pass in init_value and reducer.
template <typename T>
inline bool ReduceGeneric(const T *input_data, const int *input_dims, const int input_num_dims,
                          T *output_data, const int *axis, const int64_t num_axis_dimensions,
                          T init_value, const int output_flat_size, T reducer(const T, const T))
{
  // Return early when input shape has zero dim.
  for (int i = 0; i < input_num_dims; ++i)
  {
    if (input_dims[i] == 0)
      return false;
  }

  for (size_t idx = 0; idx < output_flat_size; ++idx)
  {
    output_data[idx] = init_value;
  }

  // Resolve axis.
  int num_resolved_axis = 0;
  int resolved_axis[2];

  if (!resolveAxis(input_num_dims, axis, num_axis_dimensions, resolved_axis, &num_resolved_axis))
  {
    return false;
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

  return true;
}

// This method expects that output_data has been initialized.
template <typename T>
inline bool reduceSumImpl(const T *input_data, const int *input_dims, const int input_num_dims,
                          T *output_data, const int *axis, const int num_axis,
                          const int num_outputs)
{
  return ReduceGeneric<T>(input_data, input_dims, input_num_dims, output_data, axis, num_axis,
                          static_cast<T>(0), num_outputs,
                          [](const T current, const T in) -> T { return in + current; });
}


// Mean over WH of axis 1,2
inline void MeanROWH(const OMRuntimeShape &unextended_input_shape, const float *input_data,
                     const OMRuntimeShape &unextended_output_shape, float *output_data)
{
  // Current implementation only supports dimension equals 4 and simultaneous
  // reduction over width and height.
  const OMRuntimeShape input_shape =
    OMRuntimeShape::extendedShape(4, unextended_input_shape);
  const OMRuntimeShape output_shape =
    OMRuntimeShape::extendedShape(4, unextended_output_shape);

  const int output_batch = output_shape.dims(0);
  const int output_depth = output_shape.dims(3);

  const int input_height = input_shape.dims(1);
  const int input_width = input_shape.dims(2);

  for (int out_b = 0; out_b < output_batch; ++out_b)
  {
    for (int out_d = 0; out_d < output_depth; ++out_d)
    {
      float value = 0;
      for (int in_h = 0; in_h < input_height; ++in_h)
      {
        for (int in_w = 0; in_w < input_width; ++in_w)
        {
          value += input_data[offset(input_shape.dimsData(), out_b, in_h, in_w, out_d)];
        }
      }
      output_data[offset(output_shape.dimsData(), out_b, 0, 0, out_d)] =
        value / (input_width * input_height);
    }
  }
}

template <typename T>
inline bool Mean(const int *input_dims, const T *input_data, const int input_num_dims,
                 T *output_data, const int num_outputs, const int *axis,
                 const int num_axis_dimensions)
{
  if (!reduceSumImpl<T>(input_data, input_dims, input_num_dims, output_data, axis,
                        num_axis_dimensions, num_outputs))
  {
    return false;
  }

  // Resolve axis again for computing mean
  int num_resolved_axis = 0;
  int resolved_axis[2];

  if (!resolveAxis(input_num_dims, axis, num_axis_dimensions, resolved_axis, &num_resolved_axis))
  {
    return false;
  }

  // Calculate mean by dividing output_data by num of aggregated element.
  size_t num_elements_in_axis = 1;
  for (int idx = 0; idx < num_resolved_axis; ++idx)
  {
    size_t current = static_cast<size_t>(input_dims[resolved_axis[idx]]);
    // Overflow prevention.
    if (current > (std::numeric_limits<size_t>::max() / num_elements_in_axis))
    {
      return false;
    }
    num_elements_in_axis *= current;
  }

  if (num_elements_in_axis > 0)
  {
    for (size_t idx = 0; idx < num_outputs; ++idx)
    {
      output_data[idx] = static_cast<T>(output_data[idx] / static_cast<T>(num_elements_in_axis));
    }
  }
  return true;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_PAL_TANH_H
