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

#include "execute/OMKernelExecutionBuilder.h"
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"

#include "core/OMRuntimeShape.h"
#include "PALSpaceToDepth.h"

using namespace onert_micro;
using namespace onert_micro::execute;
namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t outputTensorIdx = 0;

} // namespace
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleSpaceToDepth(const onert_micro::execute::OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *output;

  uint8_t *input_data;
  uint8_t *output_data;

  // Read kernel
  execute::OMRuntimeKernel runtime_kernel;
  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  input = runtime_kernel.inputs[inputTensorIdx];
  output = runtime_kernel.outputs[outputTensorIdx];

  core::OMRuntimeShape input_shape(input);
  core::OMRuntimeShape output_shape(output);

  assert(input != nullptr);
  assert(output != nullptr);

  status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
  if (status != Ok)
    return status;

  input_data = runtime_kernel.inputs_data[inputTensorIdx];
  output_data = runtime_kernel.outputs_data[outputTensorIdx];
  const auto *options = runtime_kernel.first_operator->builtin_options_as_SpaceToDepthOptions();
  const int32_t block_size = options->block_size();
  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      status =
        pal::SpaceToDepth<float>(block_size, input_shape, reinterpret_cast<float *>(input_data),
                                 output_shape, reinterpret_cast<float *>(output_data));
    }
    break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case circle::TensorType_INT8:
    {
      status =
        pal::SpaceToDepth<int8_t>(block_size, input_shape, reinterpret_cast<int8_t *>(input_data),
                                  output_shape, reinterpret_cast<int8_t *>(output_data));
    }
    break;
#endif // DIS_QUANT
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
