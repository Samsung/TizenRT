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

#include "execute/OMUtils.h"

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

OMStatus execute_kernel_CircleLess(const OMExecuteArgs &execute_args)
{
  OMStatus status = Ok;

  const circle::Tensor *input1 = nullptr;
  const circle::Tensor *input2 = nullptr;
  const circle::Tensor *output = nullptr;

  OMRuntimeKernel runtime_kernel;

  TISOHeader(execute_args, &input1, &input2, &output, &runtime_kernel);

  switch (input1->type())
  {
    case circle::TensorType_INT64:
      onert_micro::execute::evalComparisonGeneric<int64_t>(&runtime_kernel,
                                                           onert_micro::execute::pal::LessFn);
      break;
    case circle::TensorType_INT32:
      onert_micro::execute::evalComparisonGeneric<int32_t>(&runtime_kernel,
                                                           onert_micro::execute::pal::LessFn);
      break;
#ifndef DIS_QUANT
    case circle::TensorType_UINT8:
      evalQuantizedComparisonGeneric<uint8_t, int32_t>(&runtime_kernel,
                                                       onert_micro::execute::pal::LessFn);
      break;
    case circle::TensorType_INT8:
      evalQuantizedComparisonGeneric<int8_t, int32_t>(&runtime_kernel,
                                                      onert_micro::execute::pal::LessFn);
      break;
#endif // DIS_QUANT

#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
      onert_micro::execute::evalComparisonGeneric<float>(&runtime_kernel,
                                                         onert_micro::execute::pal::LessFn);
      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
