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
#include "core/OMKernelData.h"

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMUtils.h"
#include "execute/OMRuntimeKernel.h"

#include "PALSplit.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t axisTensorIdx = 0;
constexpr uint32_t inputTensorIdx = 1;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleSplit(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *axis;
  const circle::Tensor *output;

  uint8_t *input_data;
  uint8_t *axis_data;

  // Read kernel
  const circle::SplitOptions *options;

  core::SplitParams params{};
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
    assert(input_data != nullptr);
    assert(axis_data != nullptr);
    options = runtime_kernel.first_operator->builtin_options_as_SplitOptions();

    params.num_outputs = options->num_splits();

    for (uint32_t i = 0; i < params.num_outputs; ++i)
    {
      params.output_data[i] = runtime_kernel.outputs_data[i];
    }
  }
  OMStatus status;
  OMRuntimeShape axis_shape(axis);
  OMRuntimeShape input_shape(input);
  OMRuntimeShape output_shape(output);

  int32_t axis_value = utils::castInputData<int32_t>(axis_data)[0];
  if (axis_value < 0)
  {
    axis_value += input_shape.dimensionsCount() + 1;
  }

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
      status = pal::Split<float>(params, input_shape, core::utils::castInputData<float>(input_data),
                                 output_shape, axis_value);
      break;
#endif // DIS_FLOAT
    default:
    {
      status = UnsupportedActivation;
      assert(false && "Unsupported type.");
    }
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
