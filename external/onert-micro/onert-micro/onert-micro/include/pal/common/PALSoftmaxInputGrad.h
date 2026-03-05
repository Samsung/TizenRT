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

#ifndef ONERT_MICRO_EXECUTE_PAL_COMMON_SOFTMAX_INPUT_GRAD_H
#define ONERT_MICRO_EXECUTE_PAL_COMMON_SOFTMAX_INPUT_GRAD_H

#include "OMStatus.h"
#include "PALUtils.h"

#include <cmath>

namespace onert_micro
{
namespace train
{
namespace pal
{

void inline SoftmaxInputGrad(const float *dloss_doutput_data,
                             const core::OMRuntimeShape &dloss_doutput_shape,
                             const float *calculated_data, float *jacobian_row_data,
                             float *dloss_dinput_data)
{
  assert(dloss_doutput_shape.dimensionsCount() == 2);
  assert(dloss_doutput_shape.dims(0) == 1);
  const uint32_t width = dloss_doutput_shape.dims(dloss_doutput_shape.dimensionsCount() - 1);
  for (int w1 = 0; w1 < width; ++w1)
  {
    float sum = 0.0f;
    for (int w2 = 0; w2 < width; ++w2)
    {
      float val;
      if (w1 == w2)
      {
        val = calculated_data[w2] * (1.f - calculated_data[w2]);
      }
      else
      {
        val = -calculated_data[w2] * calculated_data[w1];
      }
      val *= dloss_doutput_data[w2];
      sum += val;
    }
    dloss_dinput_data[w1] = sum;
  }
}

} // namespace pal
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_COMMON_SOFTMAX_INPUT_GRAD_H
