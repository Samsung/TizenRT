/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2019 The TensorFlow Authors. All Rights Reserved.
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

#ifndef LUCI_INTERPRETER_PAL_PAD_H
#define LUCI_INTERPRETER_PAL_PAD_H

#include "PALUtils.h"

namespace luci_interpreter_pal
{

constexpr int PadKernelMaxDimensionCount() { return 5; }

void Pad(const PadParams &op_params, const luci_interpreter::RuntimeShape &input_shape,
         const float *input_data, const float *pad_value_ptr,
         const luci_interpreter::RuntimeShape &output_shape, float *output_data)
{
  // Runtime calls are currently fixed at 5 dimensions. Copy inputs so we can
  // pad them to 5 dims (yes, we are "padding the padding").
  int left_padding_copy[PadKernelMaxDimensionCount()];
  for (int i = 0; i < PadKernelMaxDimensionCount(); i++)
  {
    left_padding_copy[i] = 0;
  }
  for (int i = 0; i < op_params.left_padding_count; ++i)
  {
    left_padding_copy[i + PadKernelMaxDimensionCount() - op_params.left_padding_count] =
      op_params.left_padding[i];
  }
  int right_padding_copy[PadKernelMaxDimensionCount()];
  for (int i = 0; i < PadKernelMaxDimensionCount(); i++)
  {
    right_padding_copy[i] = 0;
  }
  for (int i = 0; i < op_params.right_padding_count; ++i)
  {
    right_padding_copy[i + PadKernelMaxDimensionCount() - op_params.right_padding_count] =
      op_params.right_padding[i];
  }
  const auto extended_output =
    luci_interpreter::RuntimeShape::extendedShape(PadKernelMaxDimensionCount(), output_shape);
  const int output_batch = extended_output.dims(0);
  const int output_plane = extended_output.dims(1);
  const int output_height = extended_output.dims(2);
  const int output_width = extended_output.dims(3);
  const int output_depth = extended_output.dims(4);

  const int left_b_padding = left_padding_copy[0];
  const int left_p_padding = left_padding_copy[1];
  const int left_h_padding = left_padding_copy[2];
  const int left_w_padding = left_padding_copy[3];
  const int left_d_padding = left_padding_copy[4];

  const int right_b_padding = right_padding_copy[0];
  const int right_p_padding = right_padding_copy[1];
  const int right_h_padding = right_padding_copy[2];
  const int right_w_padding = right_padding_copy[3];
  const int right_d_padding = right_padding_copy[4];

  const float pad_value = *pad_value_ptr;

  const float *in_ptr = input_data;
  float *out_ptr = output_data;
  for (int out_b = 0; out_b < output_batch; ++out_b)
  {
    for (int out_p = 0; out_p < output_plane; ++out_p)
    {
      for (int out_h = 0; out_h < output_height; ++out_h)
      {
        for (int out_w = 0; out_w < output_width; ++out_w)
        {
          for (int out_d = 0; out_d < output_depth; ++out_d)
          {
            if (out_b < left_b_padding || out_b >= output_batch - right_b_padding ||
                out_p < left_p_padding || out_p >= output_plane - right_p_padding ||
                out_h < left_h_padding || out_h >= output_height - right_h_padding ||
                out_w < left_w_padding || out_w >= output_width - right_w_padding ||
                out_d < left_d_padding || out_d >= output_depth - right_d_padding)
            {
              *out_ptr++ = pad_value;
            }
            else
            {
              *out_ptr++ = *in_ptr++;
            }
          }
        }
      }
    }
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_PAD_H
