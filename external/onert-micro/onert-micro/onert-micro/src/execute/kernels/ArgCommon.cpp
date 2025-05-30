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

#include "execute/kernels/ArgCommon.h"
#include "PALArgMax.h"

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

OMStatus execute_arg_common(
  const OMExecuteArgs &execute_args,
  const std::function<OMStatus(const core::OMRuntimeShape &input1_shape, const float *input1_data,
                               const int *input2_data, const core::OMRuntimeShape &output_shape,
                               int *output_data)> &f_float)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;
  const circle::Tensor *output;
  const circle::Tensor *input1;
  const circle::Tensor *input2;

  uint8_t *output_data;
  uint8_t *input_data;
  uint8_t *axis_data;

  // Read kernel
  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  output = runtime_kernel.outputs[outputTensorIdx];
  assert(output != nullptr);

  input1 = runtime_kernel.inputs[input1TensorIdx];
  assert(input1 != nullptr);

  input2 = runtime_kernel.inputs[input2TensorIdx];
  assert(input2 != nullptr);

  runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);

  output_data = runtime_kernel.outputs_data[outputTensorIdx];
  assert(output_data != nullptr);

  input_data = runtime_kernel.inputs_data[input1TensorIdx];
  assert(input_data != nullptr);

  axis_data = runtime_kernel.inputs_data[input2TensorIdx];
  assert(axis_data != nullptr);

  OMStatus status;
  const core::OMRuntimeShape input1_shape(input1);
  const core::OMRuntimeShape output_shape(output);
  switch (input1->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      status = f_float(input1_shape, reinterpret_cast<const float *>(input_data),
                       reinterpret_cast<const int *>(axis_data), output_shape,
                       reinterpret_cast<int *>(output_data));
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
