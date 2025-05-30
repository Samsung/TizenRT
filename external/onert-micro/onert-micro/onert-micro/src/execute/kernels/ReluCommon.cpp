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

#include "execute/kernels/ReluCommon.h"
#include "PALRelu.h"

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

OMStatus execute_relu_common(const OMExecuteArgs &execute_args, bool is_relu_6)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input = nullptr;
  const circle::Tensor *output = nullptr;

  uint8_t *input_data = nullptr;
  uint8_t *output_data = nullptr;

  OMStatus status = Ok;

  OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  input = runtime_kernel.inputs[inputTensorIdx];
  output = runtime_kernel.outputs[outputTensorIdx];

  assert(input != nullptr);
  assert(output != nullptr);

  status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
  if (status != Ok)
    return status;

  input_data = runtime_kernel.inputs_data[inputTensorIdx];
  output_data = runtime_kernel.outputs_data[outputTensorIdx];

  assert(input_data != nullptr);
  assert(output_data != nullptr);

  float alpha = 0.f;
  auto options = runtime_kernel.first_operator->builtin_options_as_LeakyReluOptions();
  if (options != nullptr)
    alpha = options->alpha();

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      core::OMRuntimeShape input_shape(input);
      core::OMRuntimeShape output_shape(output);

      const auto *input_data_float = core::utils::castInputData<float>(input_data);
      auto *output_data_float = core::utils::castOutputData<float>(output_data);

      assert(output_data_float);
      const int flat_size = input_shape.flatSize();

      status = pal::ReLUCommon(flat_size, input_data_float, output_data_float, alpha, is_relu_6);
    }
    break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case circle::TensorType_INT8:
    {
      core::OMRuntimeShape input_shape(input);
      core::OMRuntimeShape output_shape(output);

      const auto *input_data_int8 = core::utils::castInputData<int8_t>(input_data);
      auto *output_data_int8 = core::utils::castOutputData<int8_t>(output_data);

      assert(output_data_int8);
      const int flat_size = input_shape.flatSize();

      status = pal::ReLUCommon(flat_size, input_data_int8, output_data_int8, alpha, is_relu_6);
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
