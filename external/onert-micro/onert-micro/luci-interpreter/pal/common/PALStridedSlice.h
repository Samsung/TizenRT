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

#ifndef LUCI_INTERPRETER_PAL_STRIDED_SLICE_H
#define LUCI_INTERPRETER_PAL_STRIDED_SLICE_H

#include "Params.h"

namespace luci_interpreter_pal
{

namespace
{
// Use until std::clamp() is available from C++17.
inline int clamp(const int v, const int lo, const int hi)
{
  if (hi < v)
    return hi;
  if (v < lo)
    return lo;
  return v;
}

inline bool loopCondition(int index, int stop, int stride)
{
  // True when we have reached the end of an axis and should loop.
  return stride > 0 ? index >= stop : index <= stop;
}

// Return the "real" index for the end of iteration along that axis. This is an
// "end" in the traditional C sense, in that it points to one past the last
// element. ie. So if you were iterating through all elements of a 1D array of
// size 4, this function would return 4 as the stop, because it is one past the
// "real" indices of 0, 1, 2 & 3.
inline int stopForAxis(const StridedSliceParams &params,
                       const luci_interpreter::RuntimeShape &input_shape, int axis,
                       int start_for_axis)
{
  const auto end_mask = params.end_mask;
  const auto shrink_axis_mask = params.shrink_axis_mask;
  const auto *stop_indices = params.stop_indices;
  const auto *strides = params.strides;
  const int axis_size = input_shape.dims(axis);
  if (axis_size == 0)
  {
    return 0;
  }

  // Begin with the specified index
  const bool shrink_axis = shrink_axis_mask & (1 << axis);
  int stop = stop_indices[axis];

  // When shrinking an axis, the end position does not matter (and can be
  // incorrect when negative indexing is used, see Issue #19260). Always use
  // start_for_axis + 1 to generate a length 1 slice, since start_for_axis has
  // already been adjusted for negative indices.
  if (shrink_axis)
  {
    return start_for_axis + 1;
  }

  // end_mask override
  if (end_mask & (1 << axis))
  {
    if (strides[axis] > 0)
    {
      // Forward iteration - use the last element. These values will get
      // clamped below
      stop = std::numeric_limits<int>::max();
    }
    else
    {
      // Backward iteration - use the first element.
      stop = std::numeric_limits<int>::lowest();
    }
  }

  // Handle negative indices
  if (stop < 0)
  {
    stop += axis_size;
  }

  // Clamping
  // Because the end index points one past the last element, we need slightly
  // different clamping ranges depending on the direction.
  if (strides[axis] > 0)
  {
    // Forward iteration
    stop = clamp(stop, 0, axis_size);
  }
  else
  {
    // Backward iteration
    stop = clamp(stop, -1, axis_size - 1);
  }

  return stop;
}

// Return the index for the first element along that axis. This index will be a
// positive integer between [0, axis_size] (or [-1, axis_size -1] if stride < 0)
// that can be used to index directly into the data.
inline int startForAxis(const StridedSliceParams &params,
                        const luci_interpreter::RuntimeShape &input_shape, int axis)
{
  const auto begin_mask = params.begin_mask;
  const auto *start_indices = params.start_indices;
  const auto *strides = params.strides;
  const int axis_size = input_shape.dims(axis);
  if (axis_size == 0)
  {
    return 0;
  }
  // Begin with the specified index.
  int start = start_indices[axis];

  // begin_mask override
  if (begin_mask & 1 << axis)
  {
    if (strides[axis] > 0)
    {
      // Forward iteration - use the first element. These values will get
      // clamped below (Note: We could have set them to 0 and axis_size-1, but
      // use lowest() and max() to maintain symmetry with StopForAxis())
      start = std::numeric_limits<int>::lowest();
    }
    else
    {
      // Backward iteration - use the last element.
      start = std::numeric_limits<int>::max();
    }
  }

  // Handle negative indices
  if (start < 0)
  {
    start += axis_size;
  }

  // Clamping
  if (strides[axis] > 0)
  {
    // Forward iteration
    start = clamp(start, 0, axis_size);
  }
  else
  {
    // Backward iteration
    start = clamp(start, -1, axis_size - 1);
  }

  return start;
}

inline void stridedSlicePadIndices(StridedSliceParams *p, int dim_count)
{
  const int pad_count = dim_count - p->start_indices_count;

  // Pad indices at start, so move arrays by pad_count.
  for (int i = p->start_indices_count - 1; i >= 0; --i)
  {
    p->strides[i + pad_count] = p->strides[i];
    p->start_indices[i + pad_count] = p->start_indices[i];
    p->stop_indices[i + pad_count] = p->stop_indices[i];
  }
  for (int i = 0; i < pad_count; ++i)
  {
    p->start_indices[i] = 0;
    p->stop_indices[i] = 1;
    p->strides[i] = 1;
  }

  // Pad masks with 0s or 1s as required.
  p->shrink_axis_mask <<= pad_count;
  p->ellipsis_mask <<= pad_count;
  p->new_axis_mask <<= pad_count;
  p->begin_mask <<= pad_count;
  p->end_mask <<= pad_count;
  p->begin_mask |= (1 << pad_count) - 1;
  p->end_mask |= (1 << pad_count) - 1;

  p->start_indices_count = dim_count;
  p->stop_indices_count = dim_count;
  p->strides_count = dim_count;
}

} // namespace

template <typename T>
inline void StridedSlice(StridedSliceParams &op_params,
                         const luci_interpreter::RuntimeShape &unextended_input_shape,
                         const T *input_data, T *output_data)
{
  const luci_interpreter::RuntimeShape input_shape =
    luci_interpreter::RuntimeShape::extendedShape(5, unextended_input_shape);

  // Reverse and pad to 5 dimensions because that is what the runtime code
  // requires (ie. all shapes must be 5D and are given backwards).
  stridedSlicePadIndices(&op_params, 5);

  const int start_0 = startForAxis(op_params, input_shape, 0);
  const int stop_0 = stopForAxis(op_params, input_shape, 0, start_0);
  const int start_1 = startForAxis(op_params, input_shape, 1);
  const int stop_1 = stopForAxis(op_params, input_shape, 1, start_1);
  const int start_2 = startForAxis(op_params, input_shape, 2);
  const int stop_2 = stopForAxis(op_params, input_shape, 2, start_2);
  const int start_3 = startForAxis(op_params, input_shape, 3);
  const int stop_3 = stopForAxis(op_params, input_shape, 3, start_3);
  const int start_4 = startForAxis(op_params, input_shape, 4);
  const int stop_4 = stopForAxis(op_params, input_shape, 4, start_4);

  for (int offset_0 = start_0 * input_shape.dims(1), end_0 = stop_0 * input_shape.dims(1),
           step_0 = op_params.strides[0] * input_shape.dims(1);
       !loopCondition(offset_0, end_0, op_params.strides[0]); offset_0 += step_0)
  {
    for (int offset_1 = (offset_0 + start_1) * input_shape.dims(2),
             end_1 = (offset_0 + stop_1) * input_shape.dims(2),
             step_1 = op_params.strides[1] * input_shape.dims(2);
         !loopCondition(offset_1, end_1, op_params.strides[1]); offset_1 += step_1)
    {
      for (int offset_2 = (offset_1 + start_2) * input_shape.dims(3),
               end_2 = (offset_1 + stop_2) * input_shape.dims(3),
               step_2 = op_params.strides[2] * input_shape.dims(3);
           !loopCondition(offset_2, end_2, op_params.strides[2]); offset_2 += step_2)
      {
        for (int offset_3 = (offset_2 + start_3) * input_shape.dims(4),
                 end_3 = (offset_2 + stop_3) * input_shape.dims(4),
                 step_3 = op_params.strides[3] * input_shape.dims(4);
             !loopCondition(offset_3, end_3, op_params.strides[3]); offset_3 += step_3)
        {
          for (int offset_4 = offset_3 + start_4, end_4 = offset_3 + stop_4;
               !loopCondition(offset_4, end_4, op_params.strides[4]);
               offset_4 += op_params.strides[4])
          {
            *output_data++ = input_data[offset_4];
          }
        }
      }
    }
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_STRIDED_SLICE_H
