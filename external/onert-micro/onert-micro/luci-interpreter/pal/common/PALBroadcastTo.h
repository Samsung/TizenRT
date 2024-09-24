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

#ifndef LUCI_INTERPRETER_PAL_BROADCAST_TO_COMMON_H
#define LUCI_INTERPRETER_PAL_BROADCAST_TO_COMMON_H

#include "PALUtils.h"
#include "ProcessBroadcastShapes.h"

#include <cmath>

namespace luci_interpreter_pal
{

template <int N>
void BroadcastImpl(const NdArrayDesc<N> &input_desc, const uint8_t *input_data,
                   const NdArrayDesc<N> &output_desc, uint8_t *output_data, int indexes[N], int dim,
                   const int last_broadcasting_dim, const uint32_t type_size)
{
  // Copy data from input to output.
  if (dim == last_broadcasting_dim)
  {
    int copy_size = output_desc.strides[dim] * type_size;
    const uint8_t *data_src = input_data + subscriptToIndex(input_desc, indexes) * type_size;
    uint8_t *data_dst = output_data + subscriptToIndex(output_desc, indexes) * type_size;
    for (int i = 0; i < output_desc.extents[dim]; ++i, data_dst += copy_size)
    {
      memcpy(data_dst, data_src, copy_size);
    }
    return;
  }

  // Recursive call to find the next broadcasting.
  for (indexes[dim] = 0; indexes[dim] < input_desc.extents[dim]; ++indexes[dim])
  {
    BroadcastImpl<N>(input_desc, input_data, output_desc, output_data, indexes, dim + 1,
                     last_broadcasting_dim, type_size);
  }

  // Duplicate data in output tensor.
  indexes[dim] = 0;
  if (input_desc.extents[dim] != output_desc.extents[dim])
  {
    int copy_size = output_desc.strides[dim] * type_size;
    uint8_t *data_src = output_data + subscriptToIndex(output_desc, indexes) * type_size;
    uint8_t *data_dst = data_src + copy_size;
    for (int i = 1; i < output_desc.extents[dim]; ++i, data_dst += copy_size)
    {
      memcpy(data_dst, data_src, copy_size);
    }
  }
}

template <int N>
inline void BroadcastTo(const luci_interpreter::RuntimeShape &unextended_input_shape,
                        const uint8_t *input_data,
                        const luci_interpreter::RuntimeShape &unextended_output_shape,
                        uint8_t *output_data, luci_interpreter::DataType data_type)
{
  NdArrayDesc<N> input_desc;
  NdArrayDesc<N> output_desc;
  copyDimsToDesc(luci_interpreter::RuntimeShape::extendedShape(N, unextended_input_shape),
                 &input_desc);
  copyDimsToDesc(luci_interpreter::RuntimeShape::extendedShape(N, unextended_output_shape),
                 &output_desc);

  // Get the last dimension has broadcasting. At this dimension, the data is
  // copied from input tensor to output tensor.
  int last_broadcast_dim = -1;
  for (int i = N - 1; i >= 0; --i)
  {
    if (input_desc.extents[i] != output_desc.extents[i])
    {
      last_broadcast_dim = i;
      break;
    }
  }

  // If non-broadcasting, just copy data from input to output tensor.
  if (last_broadcast_dim == -1)
  {
    memcpy(output_data, input_data, unextended_input_shape.flatSize() * sizeof(data_type));
    return;
  }

  // Broadcasting using memcpy.
  int indexes[N] = {0};
  BroadcastImpl<N>(input_desc, input_data, output_desc, output_data, indexes, 0, last_broadcast_dim,
                   luci_interpreter::size(data_type));
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_BROADCAST_TO_COMMON_H
