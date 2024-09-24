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

#ifndef LUCI_INTERPRETER_PAL_RESIZE_NEAREST_NEIGHBOR_COMMON_H
#define LUCI_INTERPRETER_PAL_RESIZE_NEAREST_NEIGHBOR_COMMON_H

#include "Params.h"
#include "PALUtils.h"

namespace luci_interpreter_pal
{

inline int32_t getNearestNeighbor(const int input_value, const int32_t input_size,
                                  const int32_t output_size, const bool align_corners,
                                  const bool half_pixel_centers)
{
  const float scale = (align_corners && output_size > 1)
                        ? (input_size - 1) / static_cast<float>(output_size - 1)
                        : input_size / static_cast<float>(output_size);
  const float offset = half_pixel_centers ? 0.5f : 0.0f;
  int32_t output_value =
    std::min(align_corners ? static_cast<int32_t>(std::round((input_value + offset) * scale))
                           : static_cast<int32_t>(std::floor((input_value + offset) * scale)),
             input_size - 1);
  if (half_pixel_centers)
  {
    output_value = std::max(static_cast<int32_t>(0), output_value);
  }
  return output_value;
}

template <typename T>
inline void
ResizeNearestNeighbor(const ResizeNearestNeighborParams &op_params,
                      const luci_interpreter::RuntimeShape &unextended_input_shape,
                      const T *input_data, const luci_interpreter::RuntimeShape &output_size_shape,
                      const int32_t *output_size_data,
                      const luci_interpreter::RuntimeShape &unextended_output_shape, T *output_data)
{
  const luci_interpreter::RuntimeShape input_shape =
    luci_interpreter::RuntimeShape::extendedShape(4, unextended_input_shape);
  const luci_interpreter::RuntimeShape output_shape =
    luci_interpreter::RuntimeShape::extendedShape(4, unextended_output_shape);

  int32_t batches = MatchingDim(input_shape, 0, output_shape, 0);
  int32_t input_height = input_shape.dims(1);
  int32_t input_width = input_shape.dims(2);
  int32_t depth = MatchingDim(input_shape, 3, output_shape, 3);

  int32_t output_height = output_size_data[0];
  int32_t output_width = output_size_data[1];

  const int col_offset = input_shape.dims(3);
  const int row_offset = input_shape.dims(2) * col_offset;
  const int batch_offset = input_shape.dims(1) * row_offset;

  const T *input_ptr = input_data;
  T *output_ptr = output_data;
  for (int b = 0; b < batches; ++b)
  {
    for (int y = 0; y < output_height; ++y)
    {
      int32_t in_y = getNearestNeighbor(y, input_height, output_height, op_params.align_corners,
                                        op_params.half_pixel_centers);
      const T *y_input_ptr = input_ptr + in_y * row_offset;
      for (int x = 0; x < output_width; ++x)
      {
        int32_t in_x = getNearestNeighbor(x, input_width, output_width, op_params.align_corners,
                                          op_params.half_pixel_centers);
        const T *x_input_ptr = y_input_ptr + in_x * col_offset;
        memcpy(output_ptr, x_input_ptr, depth * sizeof(T));
        output_ptr += depth;
      }
    }
    input_ptr += batch_offset;
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_RESIZE_NEAREST_NEIGHBOR_COMMON_H
