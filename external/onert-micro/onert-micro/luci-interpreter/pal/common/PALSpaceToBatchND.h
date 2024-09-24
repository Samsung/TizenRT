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

#ifndef LUCI_INTERPRETER_PAL_SPACE_TO_BATCH_ND_COMMON_H
#define LUCI_INTERPRETER_PAL_SPACE_TO_BATCH_ND_COMMON_H

#include "PALUtils.h"

#include <cmath>

namespace luci_interpreter_pal
{

namespace
{
inline luci_interpreter::RuntimeShape
extendShapeSpaceToBatch(const luci_interpreter::RuntimeShape &shape)
{
  if (shape.dimensionsCount() == 4)
  {
    return shape;
  }
  luci_interpreter::RuntimeShape new_shape(4, 1);
  new_shape.setDim(0, shape.dims(0));
  new_shape.setDim(1, shape.dims(1));
  new_shape.setDim(3, shape.dims(2));
  return new_shape;
}
} // namespace

template <typename T>
inline void SpaceToBatchND(
  const int32_t pad_value, const luci_interpreter::RuntimeShape &unextended_input1_shape,
  const T *input1_data, const luci_interpreter::RuntimeShape &unextended_input2_shape,
  const int32_t *block_shape_data, const luci_interpreter::RuntimeShape &unextended_input3_shape,
  const int32_t *paddings_data, const luci_interpreter::RuntimeShape &unextended_output_shape,
  T *output_data)
{
  // Extends the input/output shape from 3D to 4D if needed, NHC -> NH1C.
  const luci_interpreter::RuntimeShape input1_shape =
    extendShapeSpaceToBatch(unextended_input1_shape);
  const luci_interpreter::RuntimeShape output_shape =
    extendShapeSpaceToBatch(unextended_output_shape);

  const int depth = input1_shape.dims(3);
  const int input_width = input1_shape.dims(2);
  const int input_height = input1_shape.dims(1);
  const int input_batch_size = input1_shape.dims(0);

  const int output_width = output_shape.dims(2);
  const int output_height = output_shape.dims(1);
  const int output_batch_size = output_shape.dims(0);

  const int block_shape_height = block_shape_data[0];
  const int block_shape_width =
    unextended_input1_shape.dimensionsCount() == 4 ? block_shape_data[1] : 1;
  const int padding_top = paddings_data[0];
  const int padding_left = unextended_input1_shape.dimensionsCount() == 4 ? paddings_data[2] : 0;

  for (int out_b = 0; out_b < output_batch_size; ++out_b)
  {
    int input_batch = out_b % input_batch_size;
    int shift_w = (out_b / input_batch_size) % block_shape_width;
    int shift_h = (out_b / input_batch_size) / block_shape_width;
    for (int out_h = 0; out_h < output_height; ++out_h)
    {
      for (int out_w = 0; out_w < output_width; ++out_w)
      {
        T *out = output_data + offset(output_shape.dimsData(), out_b, out_h, out_w, 0);
        if (out_h * block_shape_height + shift_h < padding_top ||
            out_h * block_shape_height + shift_h >= padding_top + input_height ||
            out_w * block_shape_width + shift_w < padding_left ||
            out_w * block_shape_width + shift_w >= padding_left + input_width)
        {
          // This may not execute correctly when pad_value != 0 and T != uint8.
          memset(out, pad_value, depth * sizeof(T));
        }
        else
        {
          const T *in =
            input1_data + offset(input1_shape.dimsData(), input_batch,
                                 (out_h * block_shape_height + shift_h) - padding_top,
                                 (out_w * block_shape_width + shift_w) - padding_left, 0);
          memcpy(out, in, depth * sizeof(T));
        }
      }
    }
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_SPACE_TO_BATCH_ND_COMMON_H
