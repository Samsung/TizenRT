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

#ifndef ONERT_MICRO_EXECUTE_PAL_ELU_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_ELU_COMMON_H

#include "core/OMKernelData.h"

#include <cmath>

namespace onert_micro
{
namespace execute
{
namespace pal
{

inline OMStatus Elu(const int flat_size, const float *input_data, float *output_data)
{
  for (int i = 0; i < flat_size; i++)
  {
    float val = input_data[i];
    float result = val < 0.0f ? std::exp(val) - 1 : val;
    output_data[i] = result;
  }

  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_RELU_COMMON_H
