/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef ONERT_MICRO_EXECUTE_PAL_ROUND_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_ROUND_COMMON_H

#include "core/OMRuntimeShape.h"
#include "OMStatus.h"
#include "PALSISOOperation.h"
#include "PALUtils.h"
#include <cmath>

namespace onert_micro
{
namespace execute
{
namespace pal
{

template <typename T>
inline OMStatus Round(const core::OMRuntimeShape &input_shape, const T *input_data,
                      const core::OMRuntimeShape &output_shape, T *output_data);

template <>
inline OMStatus Round<float>(const core::OMRuntimeShape &input_shape, const float *input_data,
                             const core::OMRuntimeShape &output_shape, float *output_data)
{
  const uint32_t flat_size = input_shape.flatSize();

  if (flat_size == -1)
    return UnknownError;

  assert(input_data != nullptr);
  assert(output_data != nullptr);

  assert(input_shape == output_shape);

  for (int i = 0; i < flat_size; i++)
  {
    // Note that this implementation matches that of tensorFlow tf.round
    // and corresponds to the bankers rounding method.
    auto floor_val = std::floor(input_data[i]);
    auto diff = input_data[i] - floor_val;
    if ((diff < 0.5f) || ((diff == 0.5f) && (static_cast<int>(floor_val) % 2 == 0)))
    {
      output_data[i] = floor_val;
    }
    else
    {
      output_data[i] = floor_val + 1.0f;
    }
  }

  return Ok;
}
} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_ROUND_COMMON_H
