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

#ifndef LUCI_INTERPRETER_PAL_RESIZEBILINEAR_COMMON_H
#define LUCI_INTERPRETER_PAL_RESIZEBILINEAR_COMMON_H

#include "PALUtils.h"

namespace luci_interpreter_pal
{

// Offset function for positining corresponding index in input data
// int i0 - batches, int i1 - height, int i2 - width, int i3 - depth
inline int Offset(const luci_interpreter::RuntimeShape &shape, int i0, int i1, int i2, int i3)
{
  assert(shape.dimensionsCount() == 4);

  const int32_t *dims_data = reinterpret_cast<const int32_t *>(shape.dimsData());
  LUCI_INTERPRETER_CHECK(i0 >= 0 && i0 < dims_data[0]);
  LUCI_INTERPRETER_CHECK(i1 >= 0 && i1 < dims_data[1]);
  LUCI_INTERPRETER_CHECK(i2 >= 0 && i2 < dims_data[2]);
  LUCI_INTERPRETER_CHECK(i3 >= 0 && i3 < dims_data[3]);
  return ((i0 * dims_data[1] + i1) * dims_data[2] + i2) * dims_data[3] + i3;
}

inline void ComputeInterpolationValues(const float value, const float scale,
                                       const bool half_pixel_centers, int32_t input_size,
                                       float *scaled_value, int32_t *lower_bound,
                                       int32_t *upper_bound)
{
  if (half_pixel_centers)
  {
    *scaled_value = (value + 0.5f) * scale - 0.5f;
  }
  else
  {
    *scaled_value = value * scale;
  }
  float scaled_value_floor = std::floor(*scaled_value);
  *lower_bound = std::max(static_cast<int32_t>(scaled_value_floor), static_cast<int32_t>(0));
  *upper_bound = std::min(static_cast<int32_t>(std::ceil(*scaled_value)), input_size - 1);
}

template <typename T>
static inline void
ResizeBilinear(const circle::ResizeBilinearOptions *op_params,
               const luci_interpreter::RuntimeShape &unextended_input_shape, const T *input_data,
               const luci_interpreter::RuntimeShape &unextended_output_size_shape,
               const int32_t *output_size_data,
               const luci_interpreter::RuntimeShape &unextended_output_shape, T *output_data)
{
  // If half_pixel_centers is True, align_corners must be False.
  LUCI_INTERPRETER_CHECK(!op_params->half_pixel_centers() || !op_params->align_corners());

  assert(unextended_input_shape.dimensionsCount() >= 4);
  assert(unextended_output_size_shape.dimensionsCount() >= 1);
  assert(unextended_output_shape.dimensionsCount() >= 4);
  const luci_interpreter::RuntimeShape input_shape =
    luci_interpreter::RuntimeShape::extendedShape(4, unextended_input_shape);
  const luci_interpreter::RuntimeShape output_size_shape =
    luci_interpreter::RuntimeShape::extendedShape(4, unextended_output_size_shape);
  const luci_interpreter::RuntimeShape output_shape =
    luci_interpreter::RuntimeShape::extendedShape(4, unextended_output_shape);

  int32_t batches = MatchingDim(input_shape, 0, output_shape, 0);
  int32_t input_height = input_shape.dims(1);
  int32_t input_width = input_shape.dims(2);
  int32_t depth = MatchingDim(input_shape, 3, output_shape, 3);

  assert(output_size_shape.dims(0) == 1);
  assert(output_size_shape.dims(1) == 1);
  assert(output_size_shape.dims(2) == 1);
  assert(output_size_shape.dims(3) == 2);

  int32_t output_height = output_size_data[Offset(output_size_shape, 0, 0, 0, 0)];
  int32_t output_width = output_size_data[Offset(output_size_shape, 0, 0, 0, 1)];

  float height_scale = static_cast<float>(input_height) / output_height;
  float width_scale = static_cast<float>(input_width) / output_width;
  if (op_params->align_corners() && output_height > 1)
  {
    height_scale = static_cast<float>(input_height - 1) / (output_height - 1);
  }
  if (op_params->align_corners() && output_width > 1)
  {
    width_scale = static_cast<float>(input_width - 1) / (output_width - 1);
  }
  const float rounding_offset = std::numeric_limits<T>::is_integer ? .5f : .0f;

  for (int b = 0; b < batches; ++b)
  {
    for (int y = 0; y < output_height; ++y)
    {
      float input_y;
      int32_t y0, y1;
      ComputeInterpolationValues(y, height_scale, op_params->half_pixel_centers(), input_height,
                                 &input_y, &y0, &y1);
      for (int x = 0; x < output_width; ++x)
      {
        float input_x;
        int32_t x0, x1;
        ComputeInterpolationValues(x, width_scale, op_params->half_pixel_centers(), input_width,
                                   &input_x, &x0, &x1);
        for (int c = 0; c < depth; ++c)
        {
          T interpolation = static_cast<T>(
            input_data[Offset(input_shape, b, y0, x0, c)] * (1 - (input_y - y0)) *
              (1 - (input_x - x0)) +
            input_data[Offset(input_shape, b, y1, x0, c)] * (input_y - y0) * (1 - (input_x - x0)) +
            input_data[Offset(input_shape, b, y0, x1, c)] * (1 - (input_y - y0)) * (input_x - x0) +
            input_data[Offset(input_shape, b, y1, x1, c)] * (input_y - y0) * (input_x - x0) +
            rounding_offset);
          output_data[Offset(output_shape, b, y, x, c)] = interpolation;
        }
      }
    }
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_RESIZEBILINEAR_COMMON_H
