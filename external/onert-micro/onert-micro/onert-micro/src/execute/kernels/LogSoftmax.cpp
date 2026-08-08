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

#include "OMStatus.h"

#include "core/OMUtils.h"

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMRuntimeKernel.h"

#include "execute/OMUtils.h"
#include "PALLogSoftmax.h"

using namespace onert_micro;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

// NOTE: doesnt currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleLogSoftmax(const OMExecuteArgs &execute_args)
{
  const circle::Tensor *input = nullptr;
  const circle::Tensor *output = nullptr;

  uint8_t *input_data = nullptr;
  uint8_t *output_data = nullptr;

  SISOHeader(execute_args, &input, &output, &input_data, &output_data);

  OMStatus status;

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {

      core::OMRuntimeShape inputs_shape(input);
      core::OMRuntimeShape outputs_shape(output);

      const auto dim_count = inputs_shape.dimensionsCount();

      const auto trailing_dim = dim_count - 1;

      int flat_size = 1;
      for (int i = 0; i < inputs_shape.dimensionsCount(); ++i)
      {
        flat_size *= (i == trailing_dim) ? 1 : inputs_shape.dims(i);
      }

      core::LogSoftmaxParams params;
      params.num_rows = flat_size;

      assert(inputs_shape.dims(trailing_dim) == outputs_shape.dims(trailing_dim));
      params.row_size = inputs_shape.dims(trailing_dim);

      status = pal::LogSoftmax(params, core::utils::castInputData<float>(input_data),
                               core::utils::castOutputData<float>(output_data));
    }
    break;
#endif // DIS_FLOAT
    default:
    {
      status = UnsupportedType;
      assert(false && "Unsupported type.");
    }
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
