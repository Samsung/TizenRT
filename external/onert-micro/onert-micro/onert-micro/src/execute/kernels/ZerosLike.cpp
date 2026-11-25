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

using namespace onert_micro;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t outputTensorIdx = 0;

template <typename T> void resetZeros(T *out, const int num_elements)
{
  for (int i = 0; i < num_elements; ++i)
  {
    out[i] = static_cast<T>(0);
  }
}

} // namespace

namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleZerosLike(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input = nullptr;
  uint8_t *output_data = nullptr;

  OMStatus status = Ok;
  {
    OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, runtime_context);

    input = runtime_kernel.inputs[inputTensorIdx];

    assert(input != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    output_data = runtime_kernel.outputs_data[outputTensorIdx];
  }

  assert(output_data != nullptr);

  core::OMRuntimeShape input_shape(input);
  const int flat_size = input_shape.flatSize();

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      resetZeros(core::utils::castOutputData<float>(output_data), flat_size);
    }
    break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case circle::TensorType_INT8:
    {
      resetZeros(core::utils::castOutputData<int8_t>(output_data), flat_size);
    }
    break;
#endif // DIS_QUANT

    default:
    {
      status = UnsupportedType;
      assert(false && "Unsupported type.");
      break;
    }
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
