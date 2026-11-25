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
#include "execute/OMRuntimeKernel.h"
#include "PALFill.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t valueTensorIdx = 1;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

// NOTE: doesn't currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleFill(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *value;
  const circle::Tensor *output;

  uint8_t *value_data;
  uint8_t *output_data;

  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
    if (status != Ok)
      return status;

    value = runtime_kernel.inputs[valueTensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];
    assert(value != nullptr);
    assert(output != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    value_data = runtime_kernel.inputs_data[valueTensorIdx];
    output_data = runtime_kernel.outputs_data[outputTensorIdx];
    assert(value_data != nullptr);
    assert(output_data != nullptr);
  }

  OMStatus status = Ok;

  assert(OMRuntimeShape(value).flatSize() == 1);
  OMRuntimeShape output_shape(output);

  switch (output->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      status = pal::Fill(core::utils::castInputData<float>(value_data), output_shape,
                         core::utils::castOutputData<float>(output_data));
    }
    break;
#endif // DIS_FLOAT
    case circle::TensorType_INT32:
    {
      status = pal::Fill(core::utils::castInputData<int32_t>(value_data), output_shape,
                         core::utils::castOutputData<int32_t>(output_data));
    }
    break;
    default:
    {
      status = UnsupportedActivation;
      assert(false && "Unsupported type.");
      break;
    }
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
