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

#ifndef ONERT_MICRO_EXECUTE_PAL_SLICE_H
#define ONERT_MICRO_EXECUTE_PAL_SLICE_H

namespace onert_micro
{
namespace execute
{
namespace pal
{

template <typename T>
OMStatus Slice(const core::SliceParams &op_params, const core::OMRuntimeShape &input_shape,
               const T *input_data, T *output_data)
{
  const core::OMRuntimeShape ext_shape = core::OMRuntimeShape::extendedShape(5, input_shape);
  const int begin_count = op_params.begin_count;
  const int size_count = op_params.size_count;
  // We front-pad the begin and size vectors.
  int start[5];
  int stop[5];
  for (int i = 0; i < 5; ++i)
  {
    int padded_i = 5 - i;
    start[i] = begin_count < padded_i ? 0 : op_params.begin[begin_count - padded_i];
    stop[i] = (size_count < padded_i || op_params.size[size_count - padded_i] == -1)
                ? ext_shape.dims(i)
                : start[i] + op_params.size[size_count - padded_i];
  }

  for (int i0 = start[0]; i0 < stop[0]; ++i0)
  {
    for (int i1 = start[1]; i1 < stop[1]; ++i1)
    {
      for (int i2 = start[2]; i2 < stop[2]; ++i2)
      {
        for (int i3 = start[3]; i3 < stop[3]; ++i3)
        {
          for (int i4 = start[4]; i4 < stop[4]; ++i4)
          {
            auto position =
              (((i0 * ext_shape.dims(1) + i1) * ext_shape.dims(2) + i2) * ext_shape.dims(3) + i3) *
                ext_shape.dims(4) +
              i4;
            *output_data++ = input_data[position];
          }
        }
      }
    }
  }
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_SLICE_H
