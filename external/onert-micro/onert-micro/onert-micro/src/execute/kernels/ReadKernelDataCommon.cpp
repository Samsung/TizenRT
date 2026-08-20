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

#include "execute/kernels/ReadKernelDataCommon.h"
#include "execute/OMUtils.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace TensorIndexTISO
{

constexpr uint32_t input1TensorIdx = 0;
constexpr uint32_t input2TensorIdx = 1;
constexpr uint32_t outputTensorIdx = 0;

} // namespace TensorIndexTISO

namespace onert_micro
{
namespace execute
{

OMStatus readKernelDataTISO(const OMExecuteArgs &execute_args, uint8_t *&input_data1,
                            uint8_t *&input_data2, uint8_t *&output_data,
                            core::OMRuntimeShape &input1_shape_ref,
                            core::OMRuntimeShape &input2_shape_ref,
                            core::OMRuntimeShape &output_shape_ref, circle::TensorType &tensor_type)

{

  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  OMStatus status = Ok;

  OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
  if (status != Ok)
    return status;

  const circle::Tensor *input1 = nullptr;
  const circle::Tensor *input2 = nullptr;
  const circle::Tensor *output = nullptr;

  input1 = runtime_kernel.inputs[TensorIndexTISO::input1TensorIdx];
  input2 = runtime_kernel.inputs[TensorIndexTISO::input2TensorIdx];
  output = runtime_kernel.outputs[TensorIndexTISO::outputTensorIdx];

  assert(input1 != nullptr);
  assert(input2 != nullptr);
  assert(output != nullptr);

  input_data1 = runtime_kernel.inputs_data[TensorIndexTISO::input1TensorIdx];
  input_data2 = runtime_kernel.inputs_data[TensorIndexTISO::input2TensorIdx];
  output_data = runtime_kernel.outputs_data[TensorIndexTISO::outputTensorIdx];

  input1_shape_ref = std::move(core::OMRuntimeShape(input1));
  input2_shape_ref = std::move(core::OMRuntimeShape(input2));
  output_shape_ref = std::move(core::OMRuntimeShape(output));

  tensor_type = input1->type();

  return status;
}

} // namespace execute
} // namespace onert_micro
