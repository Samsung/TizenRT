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
#include "core/OMUtils.h"
#include "execute/kernels/ComparisonCommon.h"
#include "PALComparisons.h"

using namespace onert_micro;
using namespace onert_micro::core;
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

OMStatus execute_kernel_CircleLessEqual(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  OMStatus status = Ok;

  const circle::Tensor *input1 = nullptr;
  const circle::Tensor *input2 = nullptr;
  const circle::Tensor *output = nullptr;

  OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
  if (status != Ok)
    return status;

  input1 = runtime_kernel.inputs[input1TensorIdx];
  input2 = runtime_kernel.inputs[input2TensorIdx];
  output = runtime_kernel.outputs[outputTensorIdx];

  assert(input1 != nullptr);
  assert(input2 != nullptr);
  assert(output != nullptr);

  switch (input1->type())
  {
    case circle::TensorType_INT64:
      onert_micro::execute::evalComparisonGeneric<int64_t>(&runtime_kernel,
                                                           onert_micro::execute::pal::LessEqualFn);
      break;
    case circle::TensorType_INT32:
      onert_micro::execute::evalComparisonGeneric<int32_t>(&runtime_kernel,
                                                           onert_micro::execute::pal::LessEqualFn);
      break;

#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
      onert_micro::execute::evalComparisonGeneric<float>(&runtime_kernel,
                                                         onert_micro::execute::pal::LessEqualFn);
      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
