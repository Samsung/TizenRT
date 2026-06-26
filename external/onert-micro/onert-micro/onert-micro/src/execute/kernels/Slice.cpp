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
#include "core/OMKernelData.h"

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMUtils.h"
#include "execute/OMRuntimeKernel.h"

#include "PALSlice.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

const int MAX_DIM = 5;

constexpr uint32_t input1TensorIdx = 0;
constexpr uint32_t input2TensorIdx = 1;
constexpr uint32_t input3TensorIdx = 2;

constexpr uint32_t outputTensorIdx = 0;

void getBeginAndSizeVectors(int dimensions, const int32_t *begin_data, const int32_t *size_data,
                            int32_t *begins, int32_t *sizes)
{
  int offset = MAX_DIM - dimensions;
  for (int idx = 0; idx < dimensions; ++idx)
  {
    begins[offset + idx] = begin_data[idx];
    sizes[offset + idx] = size_data[idx];
  }
}

} // namespace

// NOTE: doesnt currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleSlice(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input1 = nullptr;
  const circle::Tensor *input2 = nullptr;
  const circle::Tensor *input3 = nullptr;

  const circle::Tensor *output = nullptr;

  uint8_t *input1_data;
  const int32_t *input2_data;
  const int32_t *input3_data;
  uint8_t *output_data;

  OMStatus status = Ok;
  const circle::SliceOptions *options;
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
    assert(input3 != nullptr);
    assert(output != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    input1_data = runtime_kernel.inputs_data[input1TensorIdx];
    input2_data = utils::castInputData<int32_t>(runtime_kernel.inputs_data[input2TensorIdx]);
    input3_data = utils::castInputData<int32_t>(runtime_kernel.inputs_data[input3TensorIdx]);
    output_data = runtime_kernel.outputs_data[outputTensorIdx];

    assert(input1_data != nullptr);
    assert(input2_data != nullptr);
    assert(input3_data != nullptr);
    assert(output_data != nullptr);

    options = runtime_kernel.first_operator->builtin_options_as_SliceOptions();
  }

  OMRuntimeShape input_shape(input1);

  SliceParams op_params{};
  op_params.begin_count = MAX_DIM;
  op_params.size_count = MAX_DIM;
  for (int i = 0; i < MAX_DIM; i++)
  {
    op_params.begin[i] = 0;
    op_params.size[i] = 1;
  }
  auto num_dim = input_shape.dimensionsCount();

  getBeginAndSizeVectors(num_dim, input2_data, input3_data, op_params.begin, op_params.size);

  switch (input1->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      status = pal::Slice(op_params, input_shape, utils::castInputData<float>(input1_data),
                          utils::castOutputData<float>(output_data));
    }
    break;
#endif // DIS_FLOAT
    case circle::TensorType_INT32:
    {
      status = pal::Slice(op_params, input_shape, utils::castInputData<int32_t>(input1_data),
                          utils::castOutputData<int32_t>(output_data));
    }
    break;
    case circle::TensorType_INT64:
    {
      status = pal::Slice(op_params, input_shape, utils::castInputData<int64_t>(input1_data),
                          utils::castOutputData<int64_t>(output_data));
    }
    break;
    default:
    {
      status = UnsupportedActivation;
      assert(false && "Unsupported type.");
    }
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
