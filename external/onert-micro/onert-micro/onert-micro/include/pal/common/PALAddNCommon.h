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

#ifndef ONERT_MICRO_EXECUTE_PAL_ADD_N_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_ADD_N_COMMON_H

#include "PALArithmeticOpCommon.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{

// T is expected to be either float or int.
template <typename T>
OMStatus AddN(const size_t flat_size, const size_t num_inputs, const T *const *input_data,
              T *output_data)
{
  // All inputs and output should have the same shape, this is checked during
  // Prepare stage.
  for (size_t i = 0; i < flat_size; ++i)
  {
    T x = 0;
    for (size_t j = 0; j < num_inputs; ++j)
    {
      x += input_data[j][i];
    }
    output_data[i] = x;
  }
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_ADD_N_COMMON_H
