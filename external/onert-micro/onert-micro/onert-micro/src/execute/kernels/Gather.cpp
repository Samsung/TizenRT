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

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t positionsTensorIdx = 1;

constexpr uint32_t outputTensorIdx = 0;

template <typename InputT, typename CoordsT = int32_t>
void gather(const InputT *input_data, const CoordsT *coords_data, InputT *output_data,
            int32_t axis_size, int32_t batch_size, int32_t outer_size, int32_t inner_size,
            int32_t coord_size)
{

  for (int batch = 0; batch < batch_size; ++batch)
  {
    for (int outer = 0; outer < outer_size; ++outer)
    {
      for (int coord = 0; coord < coord_size; ++coord)
      {
        auto x = coords_data[coord];
        std::memcpy(
          output_data + (((batch * outer_size) + outer) * coord_size + coord) * inner_size,
          input_data +
            (((batch * outer_size) + outer) * axis_size + coords_data[batch * coord_size + coord]) *
              inner_size,
          sizeof(InputT) * inner_size);
      }
    }
  }
}

} // namespace

// NOTE: doesn't currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleGather(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *position;
  const circle::Tensor *output;

  uint8_t *input_data;
  uint8_t *position_data;
  uint8_t *output_data;

  const circle::GatherOptions *options;
  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
    if (status != Ok)
      return status;

    input = runtime_kernel.inputs[inputTensorIdx];
    position = runtime_kernel.inputs[positionsTensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];
    assert(input != nullptr);
    assert(position != nullptr);
    assert(output != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    input_data = runtime_kernel.inputs_data[inputTensorIdx];
    position_data = runtime_kernel.inputs_data[positionsTensorIdx];
    output_data = runtime_kernel.outputs_data[outputTensorIdx];
    assert(input_data != nullptr);
    assert(position_data != nullptr);
    assert(output_data != nullptr);

    options = runtime_kernel.first_operator->builtin_options_as_GatherOptions();
  }

  OMStatus status = Ok;

  OMRuntimeShape position_shape(position);
  OMRuntimeShape input_shape(input);

  const int input_dims_size = input_shape.dimensionsCount();
  int axis = options->axis();
  if (axis < 0)
  {
    axis += input_dims_size;
  }

  int batch_dims = options->batch_dims();
  // batch_dims should be in range: [-rank(coords), rank(coords)].
  // Negative batch_dims is added with rank of coords.
  const int coords_dims_size = position_shape.dimensionsCount();
  if (batch_dims < 0)
  {
    batch_dims += coords_dims_size;
  }

  const int axis_size = input_shape.dims(axis);

  int batch_size = 1;
  for (int i = 0; i < batch_dims; ++i)
  {
    batch_size *= input_shape.dims(i);
  }
  int outer_size = 1;
  for (int i = batch_dims; i < axis; ++i)
  {
    outer_size *= input_shape.dims(i);
  }
  int inner_size = 1;
  for (int i = axis + 1; i < input_dims_size; ++i)
  {
    inner_size *= input_shape.dims(i);
  }
  int coord_size = 1;
  for (int i = batch_dims; i < coords_dims_size; ++i)
  {
    coord_size *= position_shape.dims(i);
  }

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      gather<float, int32_t>(utils::castInputData<float>(input_data),
                             utils::castInputData<int32_t>(position_data),
                             utils::castOutputData<float>(output_data), axis_size, batch_size,
                             outer_size, inner_size, coord_size);
    }
    break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case circle::TensorType_INT8:
    {
      gather<int8_t, int32_t>(utils::castInputData<int8_t>(input_data),
                              utils::castInputData<int32_t>(position_data),
                              utils::castOutputData<int8_t>(output_data), axis_size, batch_size,
                              outer_size, inner_size, coord_size);
    }
    break;
#endif // DIS_QUANT
    case circle::TensorType_INT32:
    {
      gather<int32_t, int32_t>(utils::castInputData<int32_t>(input_data),
                               utils::castInputData<int32_t>(position_data),
                               utils::castOutputData<int32_t>(output_data), axis_size, batch_size,
                               outer_size, inner_size, coord_size);
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
