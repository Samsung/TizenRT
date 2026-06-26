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

#ifndef ONERT_MICRO_EXECUTE_PAL_SPLIT_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_SPLIT_COMMON_H

#include "core/OMRuntimeShape.h"
#include "core/OMKernelData.h"
#include "execute/OMRuntimeKernel.h"
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
OMStatus Split(const core::SplitParams &params, const core::OMRuntimeShape &input_shape,
               const T *input_data, const core::OMRuntimeShape &output_shape, int32_t axis_value)
{
  const auto output_count = params.num_outputs;

  const auto split_dimensions = input_shape.dimensionsCount();

  assert(axis_value < split_dimensions);
  assert(output_shape.dimensionsCount() == split_dimensions);

  int64_t outer_size = 1;
  for (uint32_t i = 0; i < axis_value; ++i)
  {
    outer_size *= input_shape.dims(i);
  }

  int64_t base_inner_size = 1;
  for (uint32_t i = axis_value + 1; i < split_dimensions; ++i)
  {
    base_inner_size *= input_shape.dims(i);
  }

  assert(input_data != nullptr);
  for (int64_t k = 0; k < outer_size; ++k)
  {
    for (uint32_t i = 0; i < output_count; ++i)
    {
      T *output_data = core::utils::castOutputData<T>(params.output_data[i]);
      assert(output_data != nullptr);
      const auto copy_size = output_shape.dims(axis_value) * base_inner_size;
      T *output_ptr = output_data + k * copy_size;
      assert(output_ptr != nullptr);
      for (int64_t j = 0; j < copy_size; ++j)
        output_ptr[j] = input_data[j];
      input_data += copy_size;
    }
  }
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_SPLIT_COMMON_H
