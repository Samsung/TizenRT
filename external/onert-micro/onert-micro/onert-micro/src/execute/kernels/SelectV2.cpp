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

#include "execute/OMUtils.h"
#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMRuntimeKernel.h"
#include "PALSelectV2.h"

using namespace onert_micro;
using namespace onert_micro::execute;

namespace
{

constexpr int inputCond = 0;
constexpr int inputX = 1;
constexpr int inputY = 2;
constexpr int outputIndex = 0;

template <typename T>
void CallSelect(const core::OMRuntimeShape &input_cond_shape, const bool *input_cond_data,
                const core::OMRuntimeShape &input_x_shape, const T *input_x_data,
                const core::OMRuntimeShape &input_y_shape, const T *input_y_data,
                const core::OMRuntimeShape &output_shape, T *output_data)
{
  using Func = decltype(onert_micro::execute::pal::Select<bool, T>) *;
  Func select_func;
  select_func = onert_micro::execute::pal::Select<bool, T>;

  select_func(input_cond_shape, input_cond_data, input_x_shape, input_x_data, input_y_shape,
              input_y_data, output_shape, output_data);
}

} // namespace

// NOTE: doesnt currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleSelectV2(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input_cond;
  const circle::Tensor *input_x;
  const circle::Tensor *input_y;
  const circle::Tensor *output;

  uint8_t *input_cond_data;
  uint8_t *input_x_data;
  uint8_t *input_y_data;
  uint8_t *output_data;

  OMStatus status = Ok;

  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, runtime_context);

    input_cond = runtime_kernel.inputs[inputCond];
    input_x = runtime_kernel.inputs[inputX];
    input_y = runtime_kernel.inputs[inputY];
    output = runtime_kernel.outputs[outputIndex];

    assert(input_cond != nullptr);
    assert(input_x != nullptr);
    assert(input_y != nullptr);
    assert(output != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    input_cond_data = runtime_kernel.inputs_data[inputCond];
    input_x_data = runtime_kernel.inputs_data[inputX];
    input_y_data = runtime_kernel.inputs_data[inputY];
    output_data = runtime_kernel.outputs_data[outputIndex];

    assert(input_cond_data != nullptr);
    assert(input_x_data != nullptr);
    assert(input_y_data != nullptr);
    assert(output_data != nullptr);
  }

  const core::OMRuntimeShape input_cond_shape(input_cond);
  assert(input_cond_shape.flatSize() > 0);
  const core::OMRuntimeShape input_x_shape(input_x);
  const core::OMRuntimeShape input_y_shape(input_y);
  const core::OMRuntimeShape output_shape(output);

  switch (input_x->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      CallSelect<float>(input_cond_shape, core::utils::castInputData<bool>(input_cond_data),
                        input_x_shape, core::utils::castInputData<float>(input_x_data),
                        input_y_shape, core::utils::castInputData<float>(input_y_data),
                        output_shape, core::utils::castOutputData<float>(output_data));
    }
    break;
#endif
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
