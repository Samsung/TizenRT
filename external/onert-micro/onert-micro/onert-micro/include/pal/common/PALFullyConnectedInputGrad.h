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

#ifndef ONERT_MICRO_EXECUTE_PAL_COMMON_FULLY_CONNECTED_INPUT_GRAD_H
#define ONERT_MICRO_EXECUTE_PAL_COMMON_FULLY_CONNECTED_INPUT_GRAD_H

#include "OMStatus.h"
#include "PALUtils.h"

#include <cmath>

namespace onert_micro
{
namespace train
{
namespace pal
{

void inline FullyConnectedInputGrad(const float *dloss_doutput_data,
                                    const core::OMRuntimeShape &dloss_doutput_shape,
                                    const float *weight_data,
                                    const core::OMRuntimeShape &weight_shape,
                                    float *dloss_dinput_data)
{
  const uint32_t input_rows = dloss_doutput_shape.dims(0);
  const uint32_t input_col = weight_shape.dims(1);
  const uint32_t output_cols = dloss_doutput_shape.dims(1);

  for (uint32_t i = 0; i < input_rows; ++i)
  {
    for (uint32_t j = 0; j < input_col; ++j)
    {
      float total = 0.f;
      for (uint32_t o = 0; o < output_cols; ++o)
      {
        total += weight_data[o * input_col + j] * dloss_doutput_data[o + i * output_cols];
      }
      dloss_dinput_data[j + i * input_col] = total;
    }
  }
}

} // namespace pal
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_COMMON_FULLY_CONNECTED_WEIGHT_GRAD_H
