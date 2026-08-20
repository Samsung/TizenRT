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

#ifndef ONERT_MICRO_PAL_TRANSPOSE_CONV_COMMON_H
#define ONERT_MICRO_PAL_TRANSPOSE_CONV_COMMON_H

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
TransposeConv(const core::FloatConv2D *params, const core::OMRuntimeShape &input_shape,
              const T *input_data, const core::OMRuntimeShape &filter_shape, const T *filter_data,
              const T *bias_data, const core::OMRuntimeShape &output_shape, T *output_data)
{
  assert(false && "Not IMPL yet");
}
template <>
inline OMStatus
TransposeConv<float>(const core::FloatConv2D *params, const core::OMRuntimeShape &input_shape,
                     const float *input_data, const core::OMRuntimeShape &filter_shape,
                     const float *filter_data, const float *bias_data,
                     const core::OMRuntimeShape &output_shape, float *output_data)
{
  const int stride_width = params->stride_w;
  const int stride_height = params->stride_h;
  const int pad_width = params->pad_w;
  const int pad_height = params->pad_h;

  const int batches = input_shape.dims(0);
  const int input_depth = input_shape.dims(3);
  const int output_depth = filter_shape.dims(0);
  const int input_height = input_shape.dims(1);
  const int input_width = input_shape.dims(2);
  const int filter_height = filter_shape.dims(1);
  const int filter_width = filter_shape.dims(2);
  const int output_height = output_shape.dims(1);
  const int output_width = output_shape.dims(2);
  const float output_activation_min = params->activation_min;
  const float output_activation_max = params->activation_max;

  // Although transpose convolution simplifies to convolution with transposed
  // weights for strides of 1, non-unitary striding complicates matters. To
  // keep this reference implementation as clear as possible, we use a
  // "scatter" access pattern, where we loop through all the input elements,
  // computing their influence on the output, rather than looping through the
  // output elements in the typical "gather" access pattern of a conv. We
  // therefore must initialize the output array to zero.
  const int num_elements = output_shape.flatSize();
  for (int i = 0; i < num_elements; i++)
  {
    output_data[i] = 0.0f;
  }

  // Loop through input elements one at a time.
  for (int batch = 0; batch < batches; ++batch)
  {
    for (int in_y = 0; in_y < input_height; ++in_y)
    {
      for (int in_x = 0; in_x < input_width; ++in_x)
      {
        for (int in_channel = 0; in_channel < input_depth; ++in_channel)
        {
          // Loop through the output elements it will influence
          const int out_x_origin = (in_x * stride_width) - pad_width;
          const int out_y_origin = (in_y * stride_height) - pad_height;
          for (int filter_y = 0; filter_y < filter_height; ++filter_y)
          {
            for (int filter_x = 0; filter_x < filter_width; ++filter_x)
            {
              for (int out_channel = 0; out_channel < output_depth; ++out_channel)
              {
                // Compute output element location
                const int out_x = out_x_origin + filter_x;
                const int out_y = out_y_origin + filter_y;
                // We cannot accumulate out of bounds
                if ((out_x >= 0) && (out_x < output_width) && (out_y >= 0) &&
                    (out_y < output_height))
                {
                  float input_value =
                    input_data[offset(input_shape.dimsData(), batch, in_y, in_x, in_channel)];
                  float filter_value = filter_data[offset(filter_shape.dimsData(), out_channel,
                                                          filter_y, filter_x, in_channel)];
                  output_data[offset(output_shape.dimsData(), batch, out_y, out_x, out_channel)] +=
                    input_value * filter_value;
                }
              }
            }
          }
        }
      }
    }
  }

  for (int batch = 0; batch < batches; ++batch)
  {
    for (int out_y = 0; out_y < output_height; ++out_y)
    {
      for (int out_x = 0; out_x < output_width; ++out_x)
      {
        for (int out_channel = 0; out_channel < output_depth; ++out_channel)
        {
          float acc =
            output_data[offset(output_shape.dimsData(), batch, out_y, out_x, out_channel)];
          if (bias_data)
            acc += bias_data[out_channel];

          output_data[offset(output_shape.dimsData(), batch, out_y, out_x, out_channel)] =
            activationFunctionWithMinMax(acc, output_activation_min, output_activation_max);
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
