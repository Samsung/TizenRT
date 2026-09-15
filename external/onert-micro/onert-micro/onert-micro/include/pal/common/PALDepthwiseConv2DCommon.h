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

#ifndef ONERT_MICRO_EXECUTE_PAL_DEPTHWISE_CONV_2D_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_DEPTHWISE_CONV_2D_COMMON_H

#include "PALUtils.h"
#include "core/OMKernelData.h"
#include "core/OMRuntimeShape.h"
#include "OMStatus.h"
#include <cassert>
namespace onert_micro
{
namespace execute
{
namespace pal
{
template <typename T>
inline OMStatus
DepthwiseConv2D(const core::FloatConv2D *params, const core::OMRuntimeShape &input_shape,
                const T *input_data, const core::OMRuntimeShape &filter_shape, const T *filter_data,
                const T *bias_data, const core::OMRuntimeShape &output_shape, T *output_data)
{
  assert(false && "Not IMPL yet");
}
template <>
inline OMStatus
DepthwiseConv2D<float>(const core::FloatConv2D *params, const core::OMRuntimeShape &input_shape,
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
  const int depth_multiplier = params->depth_multiplier;
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
  for (int b = 0; b < batches; ++b)
  {
    for (int out_y = 0; out_y < output_height; ++out_y)
    {
      for (int out_x = 0; out_x < output_width; ++out_x)
      {
        for (int ic = 0; ic < input_depth; ++ic)
        {
          for (int m = 0; m < depth_multiplier; m++)
          {
            const int oc = m + ic * depth_multiplier;
            const int in_x_origin = (out_x * stride_width) - pad_width;
            const int in_y_origin = (out_y * stride_height) - pad_height;
            float total = 0.f;
            for (int filter_y = 0; filter_y < filter_height; ++filter_y)
            {
              for (int filter_x = 0; filter_x < filter_width; ++filter_x)
              {
                const int in_x = in_x_origin + dilation_width_factor * filter_x;
                const int in_y = in_y_origin + dilation_height_factor * filter_y;
                // If the location is outside the bounds of the input image,
                // use zero as a default value.
                if ((in_x >= 0) && (in_x < input_width) && (in_y >= 0) && (in_y < input_height))
                {
                  float input_value = input_data[offset(input_shape.dimsData(), b, in_y, in_x, ic)];
                  float filter_value =
                    filter_data[offset(filter_shape.dimsData(), 0, filter_y, filter_x, oc)];
                  total += (input_value * filter_value);
                }
              }
            }
            float bias_value = 0.0f;
            if (bias_data)
            {
              bias_value = bias_data[oc];
            }
            output_data[offset(output_shape.dimsData(), b, out_y, out_x, oc)] =
              activationFunctionWithMinMax(total + bias_value, output_activation_min,
                                           output_activation_max);
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
#endif // ONERT_MICRO_EXECUTE_PAL_DEPTHWISE_CONV_2D_COMMON_H
