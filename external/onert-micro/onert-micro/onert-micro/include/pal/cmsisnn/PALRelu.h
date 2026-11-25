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

#ifndef ONERT_MICRO_EXECUTE_PAL_RELU_H
#define ONERT_MICRO_EXECUTE_PAL_RELU_H

#include "PALReluCommon.h"

#include <arm_nnfunctions.h>

namespace onert_micro
{
namespace execute
{
namespace pal
{

template <>
OMStatus ReLUCommon<int8_t>(const int flat_size, const int8_t *input_data, int8_t *output_data,
                            const float alpha, const bool is_relu_6)
{
  // 1. Relu
  if (is_relu_6 == false && alpha == 0)
  {
    memcpy(output_data, input_data, flat_size);
    arm_relu_q7(output_data, flat_size);
  }
  // 2. Relu6
  else if (is_relu_6 && alpha == 0)
  {
    memcpy(output_data, input_data, flat_size);
    arm_relu6_s8(output_data, flat_size);
  }
  // 3. Leaky_Relu not supported by cmsis_nn
  else if (alpha != 0)
  {
    for (int i = 0; i < flat_size; i++)
    {
      const int8_t val = input_data[i];
      int8_t result = val > 0 ? val : val * alpha;
      output_data[i] = result;
    }
  }

  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro
#endif // ONERT_MICRO_EXECUTE_PAL_RELU_H
