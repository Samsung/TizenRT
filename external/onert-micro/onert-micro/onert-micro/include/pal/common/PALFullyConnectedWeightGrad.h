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

#ifndef ONERT_MICRO_EXECUTE_PAL_COMMON_FULLY_CONNECTED_WEIGHT_GRAD_H
#define ONERT_MICRO_EXECUTE_PAL_COMMON_FULLY_CONNECTED_WEIGHT_GRAD_H

#include "OMStatus.h"
#include "PALUtils.h"

#include <cmath>

namespace onert_micro
{
namespace train
{
namespace pal
{

// Note: dloss_dweight_data should be initialized
void inline FullyConnectedWeightGrad(
  const float *dloss_doutput_data, const core::OMRuntimeShape &dloss_doutput_shape,
  const float *input_data, const core::OMRuntimeShape &input_shape, float *dloss_dweight_data,
  const core::OMRuntimeShape &weight_shape, core::OpTrainableRankType rank)
{
  const uint32_t batches = input_shape.dims(0);
  const uint32_t output_depth = dloss_doutput_shape.dims(1);
  const uint32_t accum_depth = input_shape.dims(1);

  auto depth_bounds = execute::pal::getUpLowerWeightTensorDepth(rank, output_depth);

  auto weight_depth = weight_shape.dims(0);

  for (uint32_t o = 0; o < weight_depth; ++o)
  {
    float cur_dloss_doutput = dloss_doutput_data[o + depth_bounds.first];
    for (uint32_t i = 0; i < accum_depth; ++i)
    {
      dloss_dweight_data[i + o * accum_depth] += cur_dloss_doutput * input_data[i];
    }
  }

  for (int b = 1; b < batches; ++b)
  {
    for (uint32_t o = depth_bounds.first; o < depth_bounds.second; ++o)
    {
      float cur_dloss_doutput = dloss_doutput_data[o + b * output_depth];
      for (uint32_t i = 0; i < accum_depth; ++i)
      {
        dloss_dweight_data[i + o * accum_depth] +=
          cur_dloss_doutput * input_data[i + b * accum_depth];
      }
    }
  }
}

} // namespace pal
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_COMMON_FULLY_CONNECTED_WEIGHT_GRAD_H
