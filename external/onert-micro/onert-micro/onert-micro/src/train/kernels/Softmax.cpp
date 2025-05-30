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
#include "train/OMBackpropExecutionBuilder.h"
#include "execute/OMRuntimeKernel.h"
#include "core/memory/OMMemoryManager.h"
#include "PALSoftmaxInputGrad.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::train;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

/*
 * - Calculate input gradient - Optional (not required if it is last op)
 */
OMStatus onert_micro::train::train_kernel_CircleSoftmax(const OMBackpropExecuteArgs &args)
{
  // Check is it last layer for training
  if (args.is_last_layer)
  {
    return Ok;
  }

  core::OMRuntimeStorage &forward_storage = args.forward_storage;
  core::OMRuntimeStorage &backward_storage = args.backward_storage;
  core::OMRuntimeContext &context = args.backward_context;
  uint16_t op_index = args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *output;

  uint8_t *dloss_dinput_data;

  uint8_t *output_data;
  uint8_t *dloss_doutput_data;

  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, context);

    input = runtime_kernel.inputs[inputTensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];
    assert(input != nullptr);
    assert(output != nullptr);

    // Read forward storage
    {
      runtime_kernel.getDataFromStorage(op_index, forward_storage, context);

      output_data = runtime_kernel.outputs_data[outputTensorIdx];
      assert(output_data != nullptr);
    }

    // Read backward storage
    {
      runtime_kernel.getDataFromStorage(op_index, backward_storage, context);

      dloss_dinput_data = runtime_kernel.inputs_data[inputTensorIdx];
      dloss_doutput_data = runtime_kernel.outputs_data[outputTensorIdx];

      assert(dloss_dinput_data != nullptr);
      assert(dloss_doutput_data != nullptr);
    }
  }

  OMRuntimeShape input_shape(input);
  OMRuntimeShape output_shape(output);

  // Check Softmax output and input shape
  assert(output_shape.dimensionsCount() == 2);
  assert(output_shape.dims(0) == 1);
  if (output_shape.dimensionsCount() != 2 or output_shape.dims(0) != 1)
    return UnsupportedType;

  // Allocate temporary buffer to save Jacobian row
  uint8_t *jacobian_row_data = nullptr;
  OMStatus status = core::memory::OMMemoryManager::allocateMemory(
    output_shape.flatSize() * sizeof(OMDataType(output->type())), &jacobian_row_data);
  assert(status == Ok);
  if (status != Ok)
    return status;

  // Calculate input grad
  pal::SoftmaxInputGrad(core::utils::castInputData<float>(dloss_doutput_data), output_shape,
                        core::utils::castInputData<float>(output_data),
                        core::utils::castOutputData<float>(jacobian_row_data),
                        core::utils::castOutputData<float>(dloss_dinput_data));

#ifdef OM_MEMORY_ESTIMATE
  // Deallocate temporary buffer with Jacobian row
  status = core::memory::OMMemoryManager::deallocateMemory(
    output_shape.flatSize() * sizeof(OMDataType(output->type())), jacobian_row_data);
#else
  // Deallocate temporary buffer with Jacobian row
  status = core::memory::OMMemoryManager::deallocateMemory(jacobian_row_data);
#endif

  return status;
}
