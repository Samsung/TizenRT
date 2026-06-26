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

#include <core/OMDataType.h>
#include "OMStatus.h"

#include "core/OMUtils.h"
#include "core/OMKernelData.h"
#include "core/memory/OMMemoryManager.h"

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMUtils.h"
#include "execute/OMRuntimeKernel.h"

#include "PALGRU.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t hiddenHiddenTensorIdx = 1;
constexpr uint32_t hiddenHiddenBiasTensorIdx = 2;
constexpr uint32_t hiddenInputTensorIdx = 3;
constexpr uint32_t hiddenInputBiasTensorIdx = 4;
constexpr uint32_t stateTensorIdx = 5;

constexpr uint32_t outputTensorIdx = 0;

} // namespace

// NOTE: doesnt currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleGRU(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *hidden_hidden;
  const circle::Tensor *hidden_hidden_bias;
  const circle::Tensor *hidden_input;
  const circle::Tensor *hidden_input_bias;
  const circle::Tensor *state;

  const circle::Tensor *output;

  uint8_t *input_data;
  uint8_t *hidden_hidden_data;
  uint8_t *hidden_hidden_bias_data;
  uint8_t *hidden_input_data;
  uint8_t *hidden_input_bias_data;
  uint8_t *state_data;
  uint8_t *output_data;

  uint16_t state_tensor_index = 0;

  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, runtime_context);

    input = runtime_kernel.inputs[inputTensorIdx];
    hidden_hidden = runtime_kernel.inputs[hiddenHiddenTensorIdx];
    hidden_hidden_bias = runtime_kernel.inputs[hiddenHiddenBiasTensorIdx];
    hidden_input = runtime_kernel.inputs[hiddenInputTensorIdx];
    hidden_input_bias = runtime_kernel.inputs[hiddenInputBiasTensorIdx];
    state = runtime_kernel.inputs[stateTensorIdx];

    output = runtime_kernel.outputs[outputTensorIdx];
    assert(input != nullptr);
    assert(hidden_hidden != nullptr);
    assert(hidden_input != nullptr);
    assert(state != nullptr);
    // Biases can be nullptr
    assert(output != nullptr);

    runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);

    input_data = runtime_kernel.inputs_data[inputTensorIdx];
    hidden_hidden_data = runtime_kernel.inputs_data[hiddenHiddenTensorIdx];
    hidden_hidden_bias_data = runtime_kernel.inputs_data[hiddenHiddenBiasTensorIdx];
    hidden_input_data = runtime_kernel.inputs_data[hiddenInputTensorIdx];
    hidden_input_bias_data = runtime_kernel.inputs_data[hiddenInputBiasTensorIdx];
    state_data = runtime_kernel.inputs_data[stateTensorIdx];

    output_data = runtime_kernel.outputs_data[outputTensorIdx];
    assert(input_data != nullptr);
    assert(hidden_hidden_data != nullptr);
    assert(hidden_input_data != nullptr);
    assert(state_data != nullptr);
    // Bias can be nullptr
    assert(output_data != nullptr);

    state_tensor_index = runtime_kernel.inputs_index[stateTensorIdx];
  }

  OMStatus status;

  uint8_t *output_hidden_data;
  uint8_t *output_input_data;

  status =
    core::memory::OMMemoryManager::allocateMemory(core::OMRuntimeShape(hidden_hidden).flatSize() *
                                                    sizeof(core::OMDataType(hidden_hidden->type())),
                                                  &output_hidden_data);
  if (status != Ok)
    return status;
  status = core::memory::OMMemoryManager::allocateMemory(
    core::OMRuntimeShape(hidden_input).flatSize() * sizeof(core::OMDataType(hidden_input->type())),
    &output_input_data);
  if (status != Ok)
    return status;

  // If train mode need to allocate memory for internal intermediate tensors for calculation
  // gradients further Number of intermediate tensors
  const int32_t num_of_intermediate_tensors = 9;
  // Note: size of the intermediate is equal to output size (should be checked during import phase)
  const int32_t size_of_intermediate_tensors = core::OMRuntimeShape(output).flatSize();
  assert(size_of_intermediate_tensors > 0);
  if (size_of_intermediate_tensors == 0)
    return UnknownError;

  const int32_t input_size = core::OMRuntimeShape(input).flatSize();
  const int32_t output_size = size_of_intermediate_tensors;

  // Allocate buffer with following schema:
  // times * [output_size * sizeof(data_type),
  // num_of_intermediate_tensors * size_of_intermediate_tensors * sizeof(data_type)]
  // Note: need to save all necessary intermediate data to calculate gradients
  // Deallocation should perform train/GRU kernel
  const size_t data_type_size = sizeof(core::OMDataType(input->type()));
  const int32_t time = OMRuntimeShape(input).dims(0);
  size_t intermediate_buffer_size = 0;
  uint8_t *intermediate_buffer = nullptr;
  if (execute_args.is_train_mode)
  {
    const auto num_operators = runtime_context.getCircleOperators()->size();

    uint32_t num_train_layers =
      execute_args.num_train_layers == 0 ? num_operators : execute_args.num_train_layers;
    uint32_t last_node_pos = std::min(num_operators, num_train_layers);
    uint32_t last_train_op_index = num_operators - last_node_pos;

    if (execute_args.kernel_index >= last_train_op_index)
    {
      intermediate_buffer_size = num_of_intermediate_tensors * size_of_intermediate_tensors;

      status = core::memory::OMMemoryManager::allocateMemory(
        time * intermediate_buffer_size * data_type_size, &intermediate_buffer);
      if (status != Ok)
        return status;

      // Save its buffer to state tensor index
      runtime_storage.saveDataToTensorIndex(intermediate_buffer, state_tensor_index);
    }
  }

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      status =
        pal::GRU(core::utils::castInputData<float>(input_data),
                 core::utils::castInputData<float>(hidden_input_data),
                 core::utils::castInputData<float>(hidden_hidden_data),
                 core::utils::castInputData<float>(hidden_input_bias_data),
                 core::utils::castInputData<float>(hidden_hidden_bias_data),
                 core::utils::castInputData<float>(state_data),
                 core::utils::castOutputData<float>(output_data),
                 core::utils::castOutputData<float>(output_input_data),
                 core::utils::castOutputData<float>(output_hidden_data),
                 core::OMRuntimeShape(input), core::OMRuntimeShape(output),
                 core::OMRuntimeShape(hidden_input), core::OMRuntimeShape(hidden_hidden),
                 intermediate_buffer_size, core::utils::castOutputData<float>(intermediate_buffer));
    }
    break;
#endif // DIS_FLOAT
    default:
    {
      status = UnsupportedType;
      assert(false && "Unsupported type.");
    }
  }

  core::memory::OMMemoryManager::deallocateMemory(output_input_data);
  core::memory::OMMemoryManager::deallocateMemory(output_hidden_data);

  return status;
}

} // namespace execute
} // namespace onert_micro
