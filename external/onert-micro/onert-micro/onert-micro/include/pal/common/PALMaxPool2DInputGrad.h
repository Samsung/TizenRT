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

#ifndef ONERT_MICRO_EXECUTE_PAL_COMMON_MAX_POOL_2D_INPUT_GRAD_H
#define ONERT_MICRO_EXECUTE_PAL_COMMON_MAX_POOL_2D_INPUT_GRAD_H

#include "OMStatus.h"
#include "PALUtils.h"

#include <cmath>

namespace onert_micro
{
namespace train
{
namespace pal
{

void inline MaxPool2D(const core::Pool2DParams &params, const core::OMRuntimeShape &input_shape,
                      const float *input_data, const core::OMRuntimeShape &dloss_doutput_shape,
                      const float *dloss_doutput_data, float *dloss_dinput_data)
{
  const int32_t batches = input_shape.dims(0);
  const int32_t depth = dloss_doutput_shape.dims(3);
  const int32_t input_height = input_shape.dims(1);
  const int32_t input_width = input_shape.dims(2);
  const int32_t output_height = dloss_doutput_shape.dims(1);
  const int32_t output_width = dloss_doutput_shape.dims(2);
  const int32_t stride_height = params.stride_h;
  const int32_t stride_width = params.stride_w;
  for (int batch = 0; batch < batches; ++batch)
  {
    for (int out_y = 0; out_y < output_height; ++out_y)
    {
      for (int out_x = 0; out_x < output_width; ++out_x)
      {
        for (int channel = 0; channel < depth; ++channel)
        {
          const int in_x_origin = (out_x * stride_width) - params.pad_w;
          const int in_y_origin = (out_y * stride_height) - params.pad_h;
          // Compute the boundaries of the filter region clamped so as to
          // ensure that the filter window fits in the input array.
          const int filter_x_start = std::max(0, -in_x_origin);
          const int filter_x_end = std::min(params.filter_w, input_width - in_x_origin);
          const int filter_y_start = std::max(0, -in_y_origin);
          const int filter_y_end = std::min(params.filter_h, input_height - in_y_origin);

          const int output_data_offset =
            ((batch * output_height + out_y) * output_width + out_x) * depth + channel;

          float max = std::numeric_limits<float>::lowest();
          int max_index = 0;

          for (int filter_y = filter_y_start; filter_y < filter_y_end; ++filter_y)
          {
            for (int filter_x = filter_x_start; filter_x < filter_x_end; ++filter_x)
            {
              const int in_x = in_x_origin + filter_x;
              const int in_y = in_y_origin + filter_y;

              const int input_data_offset =
                ((batch * input_height + in_y) * input_width + in_x) * depth + channel;

              if (input_data[input_data_offset] > max)
              {
                max = input_data[input_data_offset];
                max_index = input_data_offset;
              }
            }
          }
          dloss_dinput_data[max_index] = dloss_doutput_data[output_data_offset];
        }
      }
    }
  }
}

} // namespace pal
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_COMMON_MAX_POOL_2D_INPUT_GRAD_H
