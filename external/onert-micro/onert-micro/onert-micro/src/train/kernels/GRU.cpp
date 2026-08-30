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
#include "core/OMDataType.h"
#include "core/memory/OMMemoryManager.h"

#include "train/OMBackpropExecutionBuilder.h"
#include "execute/OMRuntimeKernel.h"

#include "PALGRUWeightGrad.h"

using namespace onert_micro;
using namespace onert_micro::train;

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

OMStatus onert_micro::train::train_kernel_CircleGRU(const OMBackpropExecuteArgs &args)
{
  // Check is it last layer for training
  core::OMRuntimeContext &runtime_context = args.backward_context;
  core::OMRuntimeStorage &backward_storage = args.backward_storage;
  core::OMRuntimeStorage &forward_storage = args.forward_storage;
  uint16_t op_index = args.kernel_index;

  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  const circle::Tensor *input = runtime_kernel.inputs[inputTensorIdx];
  const circle::Tensor *weight_input = runtime_kernel.inputs[hiddenInputTensorIdx];
  const circle::Tensor *weight_hidden = runtime_kernel.inputs[hiddenHiddenTensorIdx];
  const circle::Tensor *output = runtime_kernel.outputs[outputTensorIdx];

  assert(input != nullptr);
  assert(output != nullptr);

  OMStatus status = Ok;

  // Read forward
  status = runtime_kernel.getDataFromStorage(op_index, forward_storage, runtime_context);
  if (status != Ok)
    return status;
  uint8_t *input_data = runtime_kernel.inputs_data[inputTensorIdx];
  uint8_t *weight_input_data = runtime_kernel.inputs_data[hiddenInputTensorIdx];
  uint8_t *weight_hidden_data = runtime_kernel.inputs_data[hiddenHiddenTensorIdx];
  uint8_t *bias_input_data = runtime_kernel.inputs_data[hiddenInputBiasTensorIdx];
  uint8_t *bias_hidden_data = runtime_kernel.inputs_data[hiddenHiddenBiasTensorIdx];
  uint8_t *intermediate_buffer = runtime_kernel.inputs_data[stateTensorIdx];
  // Bias_data can be nullptr
  assert(input_data != nullptr);
  assert(weight_input_data != nullptr);
  assert(weight_hidden_data != nullptr);
  assert(intermediate_buffer != nullptr);

  // Read backward
  status = runtime_kernel.getDataFromStorage(op_index, backward_storage, runtime_context);
  uint8_t *output_grad_data = runtime_kernel.outputs_data[outputTensorIdx];
  uint8_t *weight_input_grad_data = runtime_kernel.inputs_data[hiddenInputTensorIdx];
  uint8_t *weight_hidden_grad_data = runtime_kernel.inputs_data[hiddenHiddenTensorIdx];
  uint8_t *bias_input_grad_data = runtime_kernel.inputs_data[hiddenInputBiasTensorIdx];
  uint8_t *bias_hidden_grad_data = runtime_kernel.inputs_data[hiddenHiddenBiasTensorIdx];
  uint8_t *state_grad_data = runtime_kernel.inputs_data[stateTensorIdx];
  uint8_t *input_grad_data = runtime_kernel.inputs_data[inputTensorIdx];
  // Bias_data and input_grad_data can be nullptr
  // Note: input_grad_data can be nullptr due to it can be last trainable node
  assert(output_grad_data != nullptr);
  assert(weight_input_grad_data != nullptr);
  assert(weight_hidden_grad_data != nullptr);
  assert(state_grad_data != nullptr);

  // Obtain shapes
  core::OMRuntimeShape input_shape(input);
  core::OMRuntimeShape output_shape(output);
  core::OMRuntimeShape weight_input_shape(weight_input);
  core::OMRuntimeShape weight_hidden_shape(weight_hidden);

  // Init output shape for FullyConnected layers
  core::OMRuntimeShape output_shape_fc(2);
  output_shape_fc.setDim(0, 1);
  output_shape_fc.setDim(1, weight_hidden_shape.dims(0));

  // Allocate memory for outputs temporary gradients for FullyConnected layers
  uint8_t *left_fc_output_grad_buffer;
  uint8_t *right_fc_output_grad_buffer;
  // Checking during import
  assert(weight_hidden_shape.dims(0) == weight_input_shape.dims(0));
  size_t allocation_size = sizeof(core::OMDataType(input->type())) * weight_hidden_shape.dims(0);
  status =
    core::memory::OMMemoryManager::allocateMemory(allocation_size, &left_fc_output_grad_buffer);
  if (status != Ok)
    return status;
  status =
    core::memory::OMMemoryManager::allocateMemory(allocation_size, &right_fc_output_grad_buffer);
  if (status != Ok)
    return status;

  assert(left_fc_output_grad_buffer != nullptr and right_fc_output_grad_buffer != nullptr);

  // Currently support only float training
  if (input->type() != circle::TensorType_FLOAT32)
    return UnsupportedType;

  status =
    pal::GRUWeightGrads(core::utils::castInputData<float>(output_grad_data),
                        core::utils::castInputData<float>(weight_input_data),
                        core::utils::castOutputData<float>(weight_input_grad_data),
                        core::utils::castInputData<float>(weight_hidden_data),
                        core::utils::castOutputData<float>(weight_hidden_grad_data),
                        core::utils::castInputData<float>(bias_input_data),
                        core::utils::castOutputData<float>(bias_input_grad_data),
                        core::utils::castInputData<float>(bias_hidden_data),
                        core::utils::castOutputData<float>(bias_hidden_grad_data),
                        core::utils::castInputData<float>(input_data),
                        core::utils::castOutputData<float>(input_grad_data),
                        core::utils::castOutputData<float>(state_grad_data), input_shape,
                        output_shape, weight_input_shape, weight_hidden_shape, output_shape_fc,
                        core::utils::castOutputData<float>(intermediate_buffer),
                        core::utils::castOutputData<float>(left_fc_output_grad_buffer),
                        core::utils::castOutputData<float>(right_fc_output_grad_buffer));

  // TODO: add input grads calculation

  // Deallocate
  core::memory::OMMemoryManager::deallocateMemory(intermediate_buffer);
  core::memory::OMMemoryManager::deallocateMemory(left_fc_output_grad_buffer);
  core::memory::OMMemoryManager::deallocateMemory(right_fc_output_grad_buffer);

  forward_storage.removeTensorFromTensorIndexToData(runtime_kernel.inputs_index[stateTensorIdx]);

  return status;
}
