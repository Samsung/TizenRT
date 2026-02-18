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

#ifndef ONERT_MICRO_EXECUTE_PAL_COMMON_RELU_INPUT_GRAD_H
#define ONERT_MICRO_EXECUTE_PAL_COMMON_RELU_INPUT_GRAD_H

#include "OMStatus.h"
#include "PALUtils.h"

#include <cmath>

namespace onert_micro
{
namespace train
{
namespace pal
{

// Note: Perform inplace calculation
void inline ReluInputGrad(const float *input_relu_data, float *dloss_doutput_data,
                          const core::OMRuntimeShape &dloss_doutput_shape)
{
  const uint32_t flat_size = dloss_doutput_shape.flatSize();

  for (uint32_t i = 0; i < flat_size; ++i)
  {
    dloss_doutput_data[i] = input_relu_data[i] > 0 ? dloss_doutput_data[i] : 0.f;
  }
}

} // namespace pal
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_COMMON_RELU_INPUT_GRAD_H
