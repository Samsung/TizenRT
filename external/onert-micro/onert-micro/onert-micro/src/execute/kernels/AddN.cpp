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
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"

#include "core/OMRuntimeShape.h"
#include "PALAddN.h"

using namespace onert_micro;
using namespace onert_micro::execute;

namespace
{
constexpr uint32_t outputTensorIdx = 0;
} // namespace

namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleAddN(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;
  const circle::Tensor *output;

  uint8_t *output_data;

  // Read kernel
  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  output = runtime_kernel.outputs[outputTensorIdx];
  assert(output != nullptr);

  runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
  output_data = runtime_kernel.outputs_data[outputTensorIdx];
  assert(output_data != nullptr);

  OMStatus status;

  core::OMRuntimeShape output_shape(output);
  switch (output->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      status = onert_micro::execute::pal::AddN<float>(
        output_shape.flatSize(), runtime_kernel.inputs_num,
        reinterpret_cast<const float *const *>(runtime_kernel.inputs_data),
        reinterpret_cast<float *>(output_data));
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
