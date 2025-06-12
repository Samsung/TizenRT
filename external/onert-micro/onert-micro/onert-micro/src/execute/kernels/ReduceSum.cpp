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

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMUtils.h"
#include "core/OMUtils.h"
#include "core/OMKernelData.h"
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"

#include "PALReduceCommon.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t numInput = 2;
constexpr uint32_t numOutput = 1;

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t axisTensorIdx = 1;

constexpr uint32_t outputTensorIdx = 0;

} // namespace

// NOTE: doesn't currently support dynamic shapes
OMStatus onert_micro::execute::execute_kernel_CircleReduceSum(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *axis;
  const circle::Tensor *output;

  uint8_t *input_data;
  uint8_t *axis_data;
  uint8_t *output_data;

  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
    if (status != Ok)
      return status;

    input = runtime_kernel.inputs[inputTensorIdx];
    axis = runtime_kernel.inputs[axisTensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];
    assert(input != nullptr);
    assert(axis != nullptr);
    assert(output != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    input_data = runtime_kernel.inputs_data[inputTensorIdx];
    axis_data = runtime_kernel.inputs_data[axisTensorIdx];
    output_data = runtime_kernel.outputs_data[outputTensorIdx];
    assert(input_data != nullptr);
    assert(axis_data != nullptr);
    assert(output_data != nullptr);
  }

  OMStatus status = Ok;

  OMRuntimeShape axis_shape(axis);
  OMRuntimeShape input_shape(input);
  OMRuntimeShape output_shape(output);

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      pal::ReduceGeneric<float>(
        utils::castInputData<float>(input_data), input_shape.dimsData(),
        input_shape.dimensionsCount(), utils::castOutputData<float>(output_data),
        utils::castInputData<int32_t>(axis_data), axis_shape.flatSize(),
        /*init_value=*/1.0f, output_shape.flatSize(),
        [](const float current, const float in) -> float { return in + current; });
    }
    break;
#endif // DIS_FLOAT
    case circle::TensorType_INT32:
    {
      pal::ReduceGeneric<int32_t>(
        utils::castInputData<int32_t>(input_data), input_shape.dimsData(),
        input_shape.dimensionsCount(), utils::castOutputData<int32_t>(output_data),
        utils::castInputData<int32_t>(axis_data), axis_shape.flatSize(),
        /*init_value=*/1, output_shape.flatSize(),
        [](const int32_t current, const int32_t in) -> int32_t { return in + current; });
    }
    break;
    case circle::TensorType_INT64:
    {
      pal::ReduceGeneric<int64_t>(
        utils::castInputData<int64_t>(input_data), input_shape.dimsData(),
        input_shape.dimensionsCount(), utils::castOutputData<int64_t>(output_data),
        utils::castInputData<int32_t>(axis_data), axis_shape.flatSize(),
        /*init_value=*/1, output_shape.flatSize(),
        [](const int64_t current, const int64_t in) -> int64_t { return in + current; });
    }
    break;
    default:
    {
      status = UnsupportedActivation;
      assert(false && "Unsupported type.");
    }
  }

  return status;
}
