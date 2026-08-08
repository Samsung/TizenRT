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

#include "core/OMUtils.h"
#include "core/OMRuntimeShape.h"
#include "core/OMKernelData.h"

#include "execute/OMUtils.h"
#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMRuntimeKernel.h"

#include "OMStatus.h"
#include "PALConcatenation.h"

using namespace onert_micro;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t numOutput = 1;

template <typename T> OMStatus evalGeneric(OMRuntimeKernel &runtime_kernel)
{
  auto output = runtime_kernel.outputs[0];

  const auto *options = runtime_kernel.first_operator->builtin_options_as_ConcatenationOptions();

  core::OMRuntimeShape output_shape(output);

  int axis = options->axis();
  if (axis < 0)
    axis += output_shape.dimensionsCount();

  const auto input_size = runtime_kernel.inputs_num;

  std::vector<const T *> all_input_data(input_size);
  std::vector<uint32_t> all_shape(input_size);

  OMStatus status = Ok;
  for (int32_t i = 0; i < input_size; ++i)
  {
    const auto *tensor = runtime_kernel.inputs[i];
    core::OMRuntimeShape shape(tensor);

    uint8_t *tensor_data = runtime_kernel.inputs_data[i];
    all_input_data[i] = core::utils::castInputData<T>(tensor_data);
    all_shape[i] = shape.dims(axis);
  }

  auto *output_data = core::utils::castOutputData<T>(runtime_kernel.outputs_data[0]);

  core::ConcatenationParams params{};
  params.axis = axis;
  params.num_inputs = input_size;
  status = pal::Concatenation<T>(params, all_shape, all_input_data, output_shape, output_data);

  return status;
}

} // namespace

namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleConcatenation(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  const auto *t0 = runtime_kernel.inputs[0];
  OMStatus status = Ok;

  status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);

  if (status != Ok)
    return status;

  switch (t0->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
      status = evalGeneric<float>(runtime_kernel);
      break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case circle::TensorType_INT8:
      status = evalGeneric<int8_t>(runtime_kernel);
      break;
#endif // DIS_QUANT
    case circle::TensorType_INT32:
      status = evalGeneric<int32_t>(runtime_kernel);
      break;
    case circle::TensorType_INT64:
      status = evalGeneric<int64_t>(runtime_kernel);
      break;
    default:
      assert(false && "Unsupported type.");
      status = UnsupportedType;
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
