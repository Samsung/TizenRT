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

using namespace onert_micro;
using namespace onert_micro::execute;

namespace
{

template <typename T> void packImpl(OMRuntimeKernel &runtime_kernel)
{
  const auto *options = runtime_kernel.first_operator->builtin_options_as_PackOptions();

  core::OMRuntimeShape input_shape(runtime_kernel.inputs[0]);
  core::OMRuntimeShape output_shape(runtime_kernel.outputs[0]);

  const int values_count = options->values_count();
  int axis = options->axis();
  const int dimensions = output_shape.dimensionsCount();

  if (axis < 0)
  {
    axis += dimensions;
  }

  int outer_size = 1;
  for (int i = 0; i < axis; ++i)
    outer_size *= output_shape.dims(i);

  int copy_size = 1;
  for (int i = axis + 1; i < dimensions; ++i)
    copy_size *= output_shape.dims(i);

  int input_size = 1;
  for (int i = 0; i < input_shape.dimensionsCount(); ++i)
    input_size *= input_shape.dims(i);

  assert(input_size == copy_size * outer_size);

  T *output_data = core::utils::castOutputData<T>(runtime_kernel.outputs_data[0]);
  assert(output_data != nullptr);

  for (int i = 0; i < values_count; ++i)
  {
    auto input_data = core::utils::castInputData<T>(runtime_kernel.inputs_data[i]);
    assert(input_data != nullptr);
    for (int k = 0; k < outer_size; ++k)
    {
      const T *input_ptr = input_data + copy_size * k;
      int loc = k * values_count * copy_size + i * copy_size;
      T *output_ptr = output_data + loc;
      for (int j = 0; j < copy_size; ++j)
        output_ptr[j] = input_ptr[j];
    }
  }
}

} // namespace

namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CirclePack(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  const auto type = runtime_kernel.inputs[0]->type();
  OMStatus status = Ok;

  status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);

  if (status != Ok)
    return status;

  switch (type)
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
      packImpl<float>(runtime_kernel);
      break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case circle::TensorType_INT8:
      packImpl<int8_t>(runtime_kernel);
      break;
#endif // DIS_QUANT
    case circle::TensorType_INT32:
      packImpl<int32_t>(runtime_kernel);
      break;
    case circle::TensorType_INT64:
      packImpl<int64_t>(runtime_kernel);
      break;
    default:
      assert(false && "Unsupported type.");
      status = UnsupportedType;
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
