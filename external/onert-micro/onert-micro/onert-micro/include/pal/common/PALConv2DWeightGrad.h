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

#ifndef ONERT_MICRO_PAL_COMMON_CONV2D_WEIGHT_GRAD_H
#define ONERT_MICRO_PAL_COMMON_CONV2D_WEIGHT_GRAD_H

#include "PALUtils.h"
#include "core/OMKernelData.h"
#include "OMStatus.h"

namespace onert_micro
{
namespace train
{
namespace pal
{
void Conv2DBiasGrad(const core::OMRuntimeShape &dloss_doutput_shape,
                    const float *dloss_doutput_data, float *dloss_dbias_data)
{
  assert(dloss_doutput_shape.dimensionsCount() == 4);
  assert(dloss_doutput_shape.dims(0) == 1);
  const int dloss_doutput_h = dloss_doutput_shape.dims(1);
  const int dloss_doutput_w = dloss_doutput_shape.dims(2);
  const int dloss_doutput_d = dloss_doutput_shape.dims(3);

  // Reduce sum over last dim
  for (uint32_t oc = 0; oc < dloss_doutput_d; ++oc)
  {
    float total = 0.f;
    for (uint32_t h = 0; h < dloss_doutput_h; ++h)
    {
      for (uint32_t w = 0; w < dloss_doutput_w; ++w)
      {
        uint32_t offset = oc + w * dloss_doutput_d + h * dloss_doutput_w * dloss_doutput_d;
        assert(offset < dloss_doutput_shape.flatSize());
        total +=
          dloss_doutput_data[oc + w * dloss_doutput_d + h * dloss_doutput_w * dloss_doutput_d];
      }
    }
    dloss_dbias_data[oc] = total;
  }
}

void Conv2DWeightGrad(const core::FloatConv2D &params, const core::OMRuntimeShape &input_shape,
                      const float *input_data, const core::OMRuntimeShape &dloss_doutput_shape,
                      const float *dloss_doutput_data,
                      const core::OMRuntimeShape &dloss_dweight_shape, float *dloss_dweight_data,
                      core::OpTrainableRankType rank)
{
  const int stride_width = params.stride_w;
  const int stride_height = params.stride_h;
  const int dilation_width_factor = params.dilation_width_factor;
  const int dilation_height_factor = params.dilation_height_factor;
  const int pad_width = 0;
  const int pad_height = 0;

  const int batches = dloss_doutput_shape.dims(0);
  const int input_h = input_shape.dims(1);
  const int input_w = input_shape.dims(2);
  const int input_d = input_shape.dims(3);
  const int dloss_doutput_h = dloss_doutput_shape.dims(1);
  const int dloss_doutput_w = dloss_doutput_shape.dims(2);
  const int dloss_doutput_d = dloss_doutput_shape.dims(3);
  const int dloss_dweight_h = dloss_dweight_shape.dims(1);
  const int dloss_dweight_w = dloss_dweight_shape.dims(2);
  const int dloss_dweight_d = dloss_dweight_shape.dims(3);
  const int dloss_dweight_o = dloss_dweight_shape.dims(0);

  auto depth_bounds = execute::pal::getUpLowerWeightTensorDepth(rank, dloss_doutput_d);

  for (uint32_t oc = 0; oc < dloss_dweight_o; ++oc)
  {
    for (uint32_t ic = 0; ic < input_d; ++ic)
    {
      for (int out_y = 0; out_y < dloss_dweight_h; ++out_y)
      {
        for (int out_x = 0; out_x < dloss_dweight_w; ++out_x)
        {
          const int in_x_origin = (out_x * stride_width) - pad_width;
          const int in_y_origin = (out_y * stride_height) - pad_height;
          float total = 0.f;

          for (int filter_y = 0; filter_y < dloss_doutput_h; ++filter_y)
          {
            for (int filter_x = 0; filter_x < dloss_doutput_w; ++filter_x)
            {
              const int in_x = in_x_origin + dilation_width_factor * filter_x;
              const int in_y = in_y_origin + dilation_height_factor * filter_y;
              // If the location is outside the bounds of the input image,
              // use zero as a default value.
              if ((in_x >= 0) && (in_x < input_w) && (in_y >= 0) && (in_y < input_h))
              {
                uint32_t input_offset = in_x * input_d + in_y * input_w * input_d + ic;
                uint32_t filter_offset =
                  oc + filter_x * dloss_doutput_d + filter_y * dloss_doutput_w * dloss_doutput_d;
                assert(input_offset < input_shape.flatSize());
                assert(filter_offset < dloss_doutput_shape.flatSize());
                float input_value = input_data[input_offset];
                float filter_value = dloss_doutput_data[filter_offset];
                total += (input_value * filter_value);
              }
            }
          }
          uint32_t output_offset = ic + input_d * out_x + input_d * dloss_dweight_w * out_y +
                                   input_d * dloss_dweight_w * dloss_dweight_h * oc;
          assert(output_offset < dloss_dweight_shape.flatSize());
          dloss_dweight_data[output_offset] = total;
        }
      }
    }
  }
}

} // namespace pal
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_PAL_COMMON_CONV2D_WEIGHT_GRAD_H
