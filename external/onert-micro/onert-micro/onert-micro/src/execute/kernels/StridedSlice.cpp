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

#include "PALStridedSlice.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t beginTensorIdx = 1;
constexpr uint32_t endTensorIdx = 2;
constexpr uint32_t stridesTensorIdx = 3;

constexpr uint32_t outputTensorIdx = 0;

core::StridedSliceParams buildStridedSliceParams(int32_t dims, const int32_t *begin,
                                                 const int32_t *end, const int32_t *strides,
                                                 const circle::StridedSliceOptions *options)
{
  core::StridedSliceParams op_params;
  op_params.start_indices_count = dims;
  op_params.stop_indices_count = dims;
  op_params.strides_count = dims;

  for (int i = 0; i < dims; ++i)
  {
    op_params.start_indices[i] = begin[i];
    op_params.stop_indices[i] = end[i];
    op_params.strides[i] = strides[i];
  }

  op_params.begin_mask = options->begin_mask();
  op_params.ellipsis_mask = 0;
  op_params.end_mask = options->end_mask();
  op_params.new_axis_mask = 0;
  op_params.shrink_axis_mask = options->shrink_axis_mask();
  return op_params;
}

} // namespace

// NOTE: doesnt currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleStridedSlice(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input = nullptr;
  const circle::Tensor *begin = nullptr;
  const circle::Tensor *end = nullptr;
  const circle::Tensor *strides = nullptr;

  const circle::Tensor *output = nullptr;

  uint8_t *input_data;
  const int32_t *begin_data;
  const int32_t *end_data;
  const int32_t *strides_data;
  uint8_t *output_data;

  OMStatus status = Ok;
  const circle::StridedSliceOptions *options;
  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, runtime_context);

    input = runtime_kernel.inputs[inputTensorIdx];
    begin = runtime_kernel.inputs[beginTensorIdx];
    end = runtime_kernel.inputs[endTensorIdx];
    strides = runtime_kernel.inputs[stridesTensorIdx];

    output = runtime_kernel.outputs[outputTensorIdx];
    assert(input != nullptr);
    assert(begin != nullptr);
    assert(end != nullptr);
    assert(strides != nullptr);
    assert(output != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    input_data = runtime_kernel.inputs_data[inputTensorIdx];
    begin_data = utils::castInputData<int32_t>(runtime_kernel.inputs_data[beginTensorIdx]);
    end_data = utils::castInputData<int32_t>(runtime_kernel.inputs_data[endTensorIdx]);
    strides_data = utils::castInputData<int32_t>(runtime_kernel.inputs_data[stridesTensorIdx]);
    output_data = runtime_kernel.outputs_data[outputTensorIdx];

    assert(input_data != nullptr);
    assert(begin_data != nullptr);
    assert(end_data != nullptr);
    assert(strides_data != nullptr);
    assert(output_data != nullptr);

    options = runtime_kernel.first_operator->builtin_options_as_StridedSliceOptions();
  }

  core::OMRuntimeShape input_shape(input);

  auto op_params = buildStridedSliceParams(input_shape.dimensionsCount(), begin_data, end_data,
                                           strides_data, options);

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      status = pal::StridedSlice(op_params, input_shape, utils::castInputData<float>(input_data),
                                 utils::castOutputData<float>(output_data));
    }
    break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case circle::TensorType_INT8:
    {
      status = pal::StridedSlice(op_params, input_shape, utils::castInputData<int8_t>(input_data),
                                 utils::castOutputData<int8_t>(output_data));
    }
    break;
#endif // DIS_QUANT
    case circle::TensorType_INT32:
    {
      status = pal::StridedSlice(op_params, input_shape, utils::castInputData<int32_t>(input_data),
                                 utils::castOutputData<int32_t>(output_data));
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
