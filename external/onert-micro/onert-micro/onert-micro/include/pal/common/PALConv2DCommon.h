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

#ifndef ONERT_MICRO_PAL_CONV2D_COMMON_H
#define ONERT_MICRO_PAL_CONV2D_COMMON_H

#include "PALUtils.h"

#include "OMStatus.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{
OMStatus ConvFloat(const core::FloatConv2D *params, const core::OMRuntimeShape &input_shape,
                   const float *input_data, const core::OMRuntimeShape &filter_shape,
                   const float *filter_data, const float *bias_data,
                   const core::OMRuntimeShape &output_shape, float *output_data)
{
  const int stride_width = params->stride_w;
  const int stride_height = params->stride_h;
  const int dilation_width_factor = params->dilation_width_factor;
  const int dilation_height_factor = params->dilation_height_factor;
  const int pad_width = params->pad_w;
  const int pad_height = params->pad_h;
  const float output_activation_min = params->activation_min;
  const float output_activation_max = params->activation_max;

  const auto batches = input_shape.dims(0);
  const int input_height = input_shape.dims(1);
  const int input_width = input_shape.dims(2);
  const int input_depth = input_shape.dims(3);
  const int output_depth = filter_shape.dims(0);
  const int filter_height = filter_shape.dims(1);
  const int filter_width = filter_shape.dims(2);
  const int output_height = output_shape.dims(1);
  const int output_width = output_shape.dims(2);
  for (int batch = 0; batch < batches; ++batch)
  {
    for (int out_y = 0; out_y < output_height; ++out_y)
    {
      const int in_y_origin = (out_y * stride_height) - pad_height;
      for (int out_x = 0; out_x < output_width; ++out_x)
      {
        const int in_x_origin = (out_x * stride_width) - pad_width;
        for (int out_channel = 0; out_channel < output_depth; ++out_channel)
        {
          float total = 0.f;
          for (int filter_y = 0; filter_y < filter_height; ++filter_y)
          {
            const int in_y = in_y_origin + dilation_height_factor * filter_y;
            for (int filter_x = 0; filter_x < filter_width; ++filter_x)
            {
              const int in_x = in_x_origin + dilation_width_factor * filter_x;

              // Zero padding by omitting the areas outside the image.
              const bool is_point_inside_image =
                (in_x >= 0) && (in_x < input_width) && (in_y >= 0) && (in_y < input_height);

              if (!is_point_inside_image)
              {
                continue;
              }

              for (int in_channel = 0; in_channel < input_depth; ++in_channel)
              {
                const int input_data_offset =
                  ((batch * input_height + in_y) * input_width + in_x) * input_depth + in_channel;

                const int filter_data_offset =
                  ((out_channel * filter_height + filter_y) * filter_width + filter_x) *
                    input_depth +
                  in_channel;

                const float input_value = input_data[input_data_offset];
                const float filter_value = filter_data[filter_data_offset];
                total += (input_value * filter_value);
              }
            }
          }
          // float bias_value = 0.0f;
          if (bias_data)
          {
            total += bias_data[out_channel];
          }

          const int output_data_offset =
            ((batch * output_height + out_y) * output_width + out_x) * output_depth + out_channel;

          output_data[output_data_offset] =
            std::min(std::max(total, output_activation_min), output_activation_max);
        }
      }
    }
  }
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_PAL_CONV2D_COMMON_H
