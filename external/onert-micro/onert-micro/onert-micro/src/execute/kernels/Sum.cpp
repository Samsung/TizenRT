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

#include "execute/OMUtils.h"
#include "execute/OMKernelExecutionBuilder.h"
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"
#include "core/OMUtils.h"

#include "core/OMRuntimeShape.h"
#include "PALReduceCommon.h"

using namespace onert_micro;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t input1TensorIdx = 0;
constexpr uint32_t input2TensorIdx = 1;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace execute
{

// TODO: remove duplicated codes in ReduceXXX type operations
OMStatus execute_kernel_CircleSum(const OMExecuteArgs &execute_args)
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

  uint16_t input_index = 0;
  uint16_t axis_index = 0;

  const circle::ReducerOptions *options;
  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, runtime_context);

    input = runtime_kernel.inputs[input1TensorIdx];
    axis = runtime_kernel.inputs[input2TensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];
    assert(input != nullptr);
    assert(axis != nullptr);
    assert(output != nullptr);

    runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);

    input_data = runtime_kernel.inputs_data[input1TensorIdx];
    axis_data = runtime_kernel.inputs_data[input2TensorIdx];
    output_data = runtime_kernel.outputs_data[outputTensorIdx];
    assert(input_data != nullptr);
    assert(axis_data != nullptr);
    assert(output_data != nullptr);

    options = runtime_kernel.first_operator->builtin_options_as_ReducerOptions();

    input_index = runtime_kernel.inputs_index[input1TensorIdx];
    axis_index = runtime_kernel.inputs_index[input2TensorIdx];
  }

  OMStatus status;

  core::OMRuntimeShape input_shape(input);
  core::OMRuntimeShape axis_shape(axis);
  core::OMRuntimeShape output_shape(output);

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
      onert_micro::execute::pal::reduceSumImpl<float>(
        core::utils::castInputData<float>(input_data), input_shape.dimsData(),
        input_shape.dimensionsCount(), core::utils::castOutputData<float>(output_data),
        core::utils::castInputData<int>(axis_data), axis_shape.dimensionsCount(),
        output_shape.flatSize());
      break;
#endif // DIS_FLOAT
    case circle::TensorType_INT32:
      break;
    case circle::TensorType_INT64:
      break;
    default:
      assert(false && "Unsupported type");
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
