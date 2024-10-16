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

#ifndef LUCI_INTERPRETER_PAL_DEPTH_TO_SPACE_COMMON_H
#define LUCI_INTERPRETER_PAL_DEPTH_TO_SPACE_COMMON_H

#include "PALUtils.h"

#include <cmath>

namespace luci_interpreter_pal
{

template <typename T>
inline void
DepthToSpace(const int32_t block_size, const luci_interpreter::RuntimeShape &unextended_input_shape,
             const T *input_data, const luci_interpreter::RuntimeShape &unextended_output_shape,
             T *output_data)
{
  const luci_interpreter::RuntimeShape input_shape =
    luci_interpreter::RuntimeShape::extendedShape(4, unextended_input_shape);
  const luci_interpreter::RuntimeShape output_shape =
    luci_interpreter::RuntimeShape::extendedShape(4, unextended_output_shape);

  const int output_depth = output_shape.dims(3);
  const int output_width = output_shape.dims(2);
  const int output_height = output_shape.dims(1);
  const int output_batch = output_shape.dims(0);

  for (int out_b = 0; out_b < output_batch; ++out_b)
  {
    for (int out_h = 0; out_h < output_height; ++out_h)
    {
      for (int out_w = 0; out_w < output_width; ++out_w)
      {
        for (int out_d = 0; out_d < output_depth; ++out_d)
        {
          const int in_d =
            out_d + ((out_h % block_size) * block_size + out_w % block_size) * output_depth;

          const int in_w = out_w / block_size;
          const int in_h = out_h / block_size;
          const int in_b = out_b;

          const int input_index = offset(input_shape.dimsData(), in_b, in_h, in_w, in_d);
          const int output_index = offset(output_shape.dimsData(), out_b, out_h, out_w, out_d);

          output_data[output_index] = input_data[input_index];
        }
      }
    }
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_DEPTH_TO_SPACE_COMMON_H
