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

#include "PALTranspose.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{
constexpr int kInputTensorIdx = 0;
constexpr int kPermTensorIdx = 1;
constexpr int kOutputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleTranspose(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *perm;
  const circle::Tensor *output;

  uint8_t *input_data;
  uint8_t *perm_data;
  uint8_t *output_data;

  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
    if (status != Ok)
      return status;

    input = runtime_kernel.inputs[kInputTensorIdx];
    perm = runtime_kernel.inputs[kPermTensorIdx];
    output = runtime_kernel.outputs[kOutputTensorIdx];
    assert(input != nullptr);
    assert(perm != nullptr);
    assert(output != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    input_data = runtime_kernel.inputs_data[kInputTensorIdx];
    perm_data = runtime_kernel.inputs_data[kPermTensorIdx];
    output_data = runtime_kernel.outputs_data[kOutputTensorIdx];
    assert(input_data != nullptr);
    assert(perm_data != nullptr);
    assert(output_data != nullptr);
  }
  OMStatus status;
  OMRuntimeShape perm_shape(perm);
  OMRuntimeShape input_shape(input);
  OMRuntimeShape output_shape(output);

  for (int idx = 0; idx < input_shape.dimensionsCount(); ++idx)
    assert(reinterpret_cast<int32_t *>(perm_data)[idx] >= 0 and
           perm_data[idx] < input_shape.dimensionsCount());

  core::TransposeParams params;
  params.perm_count = perm_shape.dims(0);
  for (int i = 0; i < params.perm_count; ++i)
    params.perm[i] = reinterpret_cast<int32_t *>(perm_data)[i];

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
      status = pal::Transpose<float>(params, input_shape, reinterpret_cast<float *>(input_data),
                                     output_shape, reinterpret_cast<float *>(output_data));
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
