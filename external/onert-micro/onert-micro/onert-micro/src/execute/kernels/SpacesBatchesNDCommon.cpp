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

#include "execute/kernels/SpacesBatchesNDCommon.h"
#include "execute/OMUtils.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr uint32_t input1TensorIdx = 0;
constexpr uint32_t input2TensorIdx = 1;
constexpr uint32_t input3TensorIdx = 2;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace execute
{

OMStatus execute_spaces_batches_nd_common(
  const OMExecuteArgs &execute_args,
  const std::function<
    OMStatus(const core::OMRuntimeShape &unextended_input1_shape, const float *input1_data,
             const core::OMRuntimeShape &unextended_input2_shape, const int32_t *block_shape_data,
             const core::OMRuntimeShape &unextended_input3_shape, const int32_t *crops_data,
             const core::OMRuntimeShape &unextended_output_shape, float *output_data)> &func)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input1;
  const circle::Tensor *input2;
  const circle::Tensor *input3;
  const circle::Tensor *output;

  uint8_t *input1_data;
  uint8_t *input2_data;
  uint8_t *input3_data;
  uint8_t *output_data;

  uint16_t input1_index = 0;
  uint16_t input2_index = 0;

  // Read kernel

  execute::OMRuntimeKernel runtime_kernel;
  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  input1 = runtime_kernel.inputs[input1TensorIdx];
  input2 = runtime_kernel.inputs[input2TensorIdx];
  input3 = runtime_kernel.inputs[input3TensorIdx];
  output = runtime_kernel.outputs[outputTensorIdx];

  core::OMRuntimeShape input1_shape(input1);
  core::OMRuntimeShape input2_shape(input1);
  core::OMRuntimeShape input3_shape(input1);
  core::OMRuntimeShape output_shape(output);

  assert(input1 != nullptr);
  assert(input2 != nullptr);
  assert(input3 != nullptr);
  assert(output != nullptr);

  status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
  if (status != Ok)
    return status;

  input1_data = runtime_kernel.inputs_data[input1TensorIdx];
  input2_data = runtime_kernel.inputs_data[input2TensorIdx];
  input3_data = runtime_kernel.inputs_data[input3TensorIdx];
  output_data = runtime_kernel.outputs_data[outputTensorIdx];

  switch (input1->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      status = func(input1_shape, reinterpret_cast<float *>(input1_data), input2_shape,
                    reinterpret_cast<int32_t *>(input2_data), input3_shape,
                    reinterpret_cast<int32_t *>(input3_data), output_shape,
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
