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
#include "core/OMRuntimeShape.h"

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMRuntimeKernel.h"

#include "PALPad.h"

using namespace onert_micro;
using namespace onert_micro::execute;

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

OMStatus execute_kernel_CirclePad(const OMExecuteArgs &execute_args)
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

  const circle::PadOptions *options;
  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, runtime_context);

    input1 = runtime_kernel.inputs[input1TensorIdx];
    input2 = runtime_kernel.inputs[input2TensorIdx];
    input3 = runtime_kernel.inputs[input3TensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];
    assert(input1 != nullptr);
    assert(input2 != nullptr);
    // input3 - can be nullptr
    assert(output != nullptr);

    runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);

    input1_data = runtime_kernel.inputs_data[input1TensorIdx];
    input2_data = runtime_kernel.inputs_data[input2TensorIdx];
    input3_data = runtime_kernel.inputs_data[input3TensorIdx];
    output_data = runtime_kernel.outputs_data[outputTensorIdx];
    assert(input1_data != nullptr);
    assert(input2_data != nullptr);
    // input3_data can be nullptr
    assert(output_data != nullptr);

    options = runtime_kernel.first_operator->builtin_options_as_PadOptions();
  }

  OMStatus status = Ok;

  core::OMRuntimeShape input1_shape(input1);
  core::OMRuntimeShape input2_shape(input2);
  core::OMRuntimeShape output_shape(output);

  // Create PadParams
  core::PadParams pad_params{};
  const auto num_input_dimensions = input1_shape.dimensionsCount();
  assert(num_input_dimensions <= 5);

  if (num_input_dimensions > 5)
    return UnsupportedType;

  pad_params.left_padding_count = num_input_dimensions;
  pad_params.right_padding_count = num_input_dimensions;

  auto *paddings_data = reinterpret_cast<int32_t *>(input2_data);
  for (int idx = num_input_dimensions - 1; idx >= 0; --idx)
  {
    pad_params.left_padding[idx] = paddings_data[idx * 2];
    pad_params.right_padding[idx] = paddings_data[idx * 2 + 1];
  }

  switch (input1->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      float pad_value = input3_data == nullptr ? 0.f : *reinterpret_cast<float *>(input3_data[0]);
      status = pal::Pad(pad_params, input1_shape, core::utils::castInputData<float>(input1_data),
                        pad_value, output_shape, core::utils::castOutputData<float>(output_data));
    }
    break;
#endif // DIS_FLOAT
    default:
    {
      status = UnsupportedType;
      assert(false && "Unsupported type");
    }
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
