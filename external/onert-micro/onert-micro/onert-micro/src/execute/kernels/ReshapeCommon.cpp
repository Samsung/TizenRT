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

#include "execute/kernels/ReshapeCommon.h"

using namespace onert_micro;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

// NOTE: doesnt currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_reshape_common(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  const circle::Tensor *input = runtime_kernel.inputs[inputTensorIdx];
  const circle::Tensor *output = runtime_kernel.outputs[outputTensorIdx];

  assert(input != nullptr);
  assert(output != nullptr);

  OMStatus status = Ok;

  status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
  if (status != Ok)
    return status;

  uint8_t *input_data = runtime_kernel.inputs_data[inputTensorIdx];
  uint8_t *output_data = runtime_kernel.outputs_data[outputTensorIdx];

  assert(input_data != nullptr);
  assert(output_data != nullptr);

  // Check is it inplace kernel
  if (input_data == output_data)
    return Ok;

  const core::OMRuntimeShape shape(input);

  const size_t element_size =
    static_cast<uint32_t>(getOMDataTypeSize(core::onertMicroDatatype(input->type())));
  const int32_t num_elements = shape.flatSize();
  std::memcpy(output_data, input_data, num_elements * element_size);

  return status;
}

} // namespace execute
} // namespace onert_micro
