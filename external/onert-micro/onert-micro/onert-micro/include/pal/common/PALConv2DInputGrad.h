/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2017 The TensorFlow Authors. All Rights Reserved.
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

#ifndef ONERT_MICRO_PAL_COMMON_CONV2D_INPUT_GRAD_H
#define ONERT_MICRO_PAL_COMMON_CONV2D_INPUT_GRAD_H

#include "PALUtils.h"
#include "core/OMKernelData.h"
#include "OMStatus.h"

namespace onert_micro
{
namespace train
{
namespace pal
{

namespace
{

/*
 * Rotate square 2D weights by 180 degrees
 */
void rotate_180(float *weights, uint32_t num_rows, uint32_t num_cols, uint32_t oc, uint32_t ic,
                uint32_t num_input_channels)
{
  for (int row = 0; row < num_rows; ++row)
  {
    for (int col = 0; col < num_cols / 2; ++col)
    {
      uint32_t offset = ic + col * num_input_channels + row * num_cols * num_input_channels +
                        oc * num_cols * num_input_channels * num_rows;
      uint32_t rotated_offset = ic + (num_cols - col - 1) * num_input_channels +
                                row * num_cols * num_input_channels +
                                oc * num_cols * num_input_channels * num_rows;

      float tmp_value = weights[offset];
      weights[offset] = weights[rotated_offset];
      weights[rotated_offset] = tmp_value;
    }
  }
}

} // namespace

void Conv2DInputGrad(const core::FloatConv2D &params, const core::OMRuntimeShape &weight_shape,
                     const float *weight_data, const core::OMRuntimeShape &dloss_doutput_shape,
                     const float *dloss_doutput_data,
                     const core::OMRuntimeShape &dloss_dinput_shape, float *dloss_dinput_data)
{
  const int stride_width = params.stride_w;
  const int stride_height = params.stride_h;
  const int dilation_width_factor = params.dilation_width_factor;
  const int dilation_height_factor = params.dilation_height_factor;
  const int pad_width = 0;
  const int pad_height = 0;

  const int weight_h = weight_shape.dims(1);
  const int weight_w = weight_shape.dims(2);
  const int weight_d = weight_shape.dims(3);
  const int dloss_doutput_h = dloss_doutput_shape.dims(1);
  const int dloss_doutput_w = dloss_doutput_shape.dims(2);
  const int dloss_doutput_d = dloss_doutput_shape.dims(3);
  const int dloss_dinput_h = dloss_dinput_shape.dims(1);
  const int dloss_dinput_w = dloss_dinput_shape.dims(2);
  const int dloss_dinput_d = dloss_dinput_shape.dims(3);

  auto *n_c_weight_data = const_cast<float *>(weight_data);

  for (uint32_t oc = 0; oc < dloss_dinput_d; ++oc)
  {
    for (uint32_t ic = 0; ic < dloss_doutput_d; ++ic)
    {
      rotate_180(n_c_weight_data, weight_h, weight_w, ic, oc, dloss_dinput_d);
      for (int out_y = 0; out_y < dloss_dinput_h; ++out_y)
      {
        for (int out_x = 0; out_x < dloss_dinput_w; ++out_x)
        {
          const int in_x_origin = (out_x * stride_width) - pad_width;
          const int in_y_origin = (out_y * stride_height) - pad_height;
          float total = 0.f;

          for (int filter_y = 0; filter_y < weight_h; ++filter_y)
          {
            for (int filter_x = 0; filter_x < weight_w; ++filter_x)
            {
              const int in_x = in_x_origin + dilation_width_factor * filter_x;
              const int in_y = in_y_origin + dilation_height_factor * filter_y;
              // If the location is outside the bounds of the input image,
              // use zero as a default value.
              if ((in_x >= 0) && (in_x < dloss_doutput_w) && (in_y >= 0) &&
                  (in_y < dloss_doutput_h))
              {
                uint32_t input_offset =
                  in_x * dloss_doutput_d + in_y * dloss_doutput_w * dloss_doutput_d + ic;
                uint32_t filter_offset = oc + filter_x * dloss_dinput_d +
                                         filter_y * weight_w * dloss_dinput_d +
                                         ic * weight_w * dloss_dinput_d * weight_h;
                assert(input_offset < dloss_doutput_shape.flatSize());
                float input_value = dloss_doutput_data[input_offset];
                float filter_value = n_c_weight_data[filter_offset];
                total += (input_value * filter_value);
              }
            }
          }
          uint32_t output_offset =
            oc + dloss_dinput_d * out_x + out_y * dloss_dinput_d * dloss_dinput_w;
          assert(output_offset < dloss_dinput_shape.flatSize());
          dloss_dinput_data[output_offset] = total;
        }
      }
      // Rotate back
      rotate_180(n_c_weight_data, weight_h, weight_w, ic, oc, dloss_dinput_d);
    }
  }
}

} // namespace pal
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_PAL_COMMON_CONV2D_INPUT_GRAD_H
