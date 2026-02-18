/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef ONERT_MICRO_EXECUTE_PAL_MAX_POOL_2D_H
#define ONERT_MICRO_EXECUTE_PAL_MAX_POOL_2D_H

#include "PALMaxPool2DCommon.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{

OMStatus MaxPool(const core::Pool2DParams &params, const core::OMRuntimeShape &input_shape,
                 const int8_t *input_data, const core::OMRuntimeShape &output_shape,
                 int8_t *output_data)
{
  assert(input_shape.dimensionsCount() == 4);
  assert(output_shape.dimensionsCount() == 4);
  const int batches = MatchingDim(input_shape, 0, output_shape, 0);
  const int depth = MatchingDim(input_shape, 3, output_shape, 3);
  const int input_height = input_shape.dims(1);
  const int input_width = input_shape.dims(2);
  const int output_height = output_shape.dims(1);
  const int output_width = output_shape.dims(2);
  const int stride_height = params.stride_h;
  const int stride_width = params.stride_w;
  const int pad_w = params.pad_w;
  const int pad_h = params.pad_h;
  const int filter_h = params.filter_h;
  const int filter_w = params.filter_w;
  for (int batch = 0; batch < batches; ++batch)
  {
    for (int out_y = 0; out_y < output_height; ++out_y)
    {
      for (int out_x = 0; out_x < output_width; ++out_x)
      {
        for (int channel = 0; channel < depth; ++channel)
        {
          const int in_x_origin = (out_x * stride_width) - pad_w;
          const int in_y_origin = (out_y * stride_height) - pad_h;
          // Compute the boundaries of the filter region clamped so as to
          // ensure that the filter window fits in the input array.
          const int filter_x_start = std::max(0, -in_x_origin);
          const int filter_x_end = std::min(filter_w, input_width - in_x_origin);
          const int filter_y_start = std::max(0, -in_y_origin);
          const int filter_y_end = std::min(filter_h, input_height - in_y_origin);
          int8_t max = std::numeric_limits<int8_t>::lowest();
          for (int filter_y = filter_y_start; filter_y < filter_y_end; ++filter_y)
          {
            for (int filter_x = filter_x_start; filter_x < filter_x_end; ++filter_x)
            {
              const int in_x = in_x_origin + filter_x;
              const int in_y = in_y_origin + filter_y;
              max = std::max(
                max, input_data[offset(input_shape.dimsData(), batch, in_y, in_x, channel)]);
            }
          }
          max = std::max<int8_t>(max, params.quantized_activation_min);
          max = std::min<int8_t>(max, params.quantized_activation_max);
          output_data[offset(output_shape.dimsData(), batch, out_y, out_x, channel)] =
            static_cast<int8_t>(max);
        }
      }
    }
  }
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_MAX_POOL_2D_H
