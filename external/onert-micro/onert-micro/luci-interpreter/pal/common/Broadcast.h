/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_BROADCAST_H
#define LUCI_INTERPRETER_BROADCAST_H

#include <functional>
#include <luci_interpreter/core/Tensor.h>
#include "ProcessBroadcastShapes.h"
namespace luci_interpreter_pal
{
template <typename T>
inline void
BroadcastTISO4DSlow(const luci_interpreter::RuntimeShape &input1_shape, const T *input1_data,
                    const luci_interpreter::RuntimeShape &input2_shape, const T *input2_data,
                    const luci_interpreter::RuntimeShape &output_shape, T *output_data,
                    std::function<const T &(const T &, const T &)> func)
{
  NdArrayDesc<4> desc1;
  NdArrayDesc<4> desc2;
  NdArrayDescsForElementwiseBroadcast(input1_shape, input2_shape, &desc1, &desc2);

  const luci_interpreter::RuntimeShape extended_output_shape =
    luci_interpreter::RuntimeShape::extendedShape(4, output_shape);

  // In Tensorflow, the dimensions are canonically named (batch_number, row,
  // col, channel), with extents (batches, height, width, depth), with the
  // trailing dimension changing most rapidly (channels has the smallest stride,
  // typically 1 element).
  //
  // In generated C code, we store arrays with the dimensions reversed. The
  // first dimension has smallest stride.
  //
  // We name our variables by their Tensorflow convention, but generate C code
  // nesting loops such that the innermost loop has the smallest stride for the
  // best cache behavior.

  for (int b = 0; b < extended_output_shape.dims(0); ++b)
  {
    for (int y = 0; y < extended_output_shape.dims(1); ++y)
    {
      for (int x = 0; x < extended_output_shape.dims(2); ++x)
      {
        for (int c = 0; c < extended_output_shape.dims(3); ++c)
        {
          const int output_data_offset =
            ((b * extended_output_shape.dims(1) + y) * extended_output_shape.dims(2) + x) *
              extended_output_shape.dims(3) +
            c;

          output_data[output_data_offset] = func(input1_data[subscriptToIndex(desc1, b, y, x, c)],
                                                 input2_data[subscriptToIndex(desc2, b, y, x, c)]);
        }
      }
    }
  }
}
} // namespace luci_interpreter_pal
#endif // LUCI_INTERPRETER_BROADCAST_H
