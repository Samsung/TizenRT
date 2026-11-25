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

#ifndef ONERT_MICRO_EXECUTE_PAL_FILL_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_FILL_COMMON_H

#include "core/OMRuntimeShape.h"
#include "OMStatus.h"
#include "PALUtils.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{

template <typename T>
inline OMStatus Fill(const T *input_data, const core::OMRuntimeShape &output_shape, T *output_data)
{
  const uint32_t flat_size = output_shape.flatSize();

  if (flat_size == -1)
    return UnknownError;

  assert(input_data != nullptr);
  assert(output_data != nullptr);

  for (int i = 0; i < flat_size; ++i)
  {
    output_data[i] = input_data[0];
  }

  return Ok;
}
} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_EXP_COMMON_H
