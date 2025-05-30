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
#include "import/OMKernelConfigureBuilder.h"
#include "execute/OMRuntimeKernel.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t positionsTensorIdx = 1;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleGather(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  onert_micro::execute::OMRuntimeKernel runtime_kernel;

  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  const circle::Tensor *input = runtime_kernel.inputs[inputTensorIdx];
  const circle::Tensor *positions = runtime_kernel.inputs[positionsTensorIdx];
  const circle::Tensor *output = runtime_kernel.outputs[outputTensorIdx];

  assert(input != nullptr);
  assert(positions != nullptr);
  assert(output != nullptr);

  status = utils::checkCondition(input->type() == output->type());
  if (status != Ok)
    return status;

  const auto *options = runtime_kernel.first_operator->builtin_options_as_GatherOptions();

  if (options == nullptr)
    return UnknownError;

  status = utils::checkCondition(positions->type() == circle::TensorType_INT32);
  if (status != Ok)
    return status;

  auto input_type = input->type();
  status = utils::checkCondition(input_type == circle::TensorType_INT32 or
                                 input_type == circle::TensorType_FLOAT32 or
                                 input_type == circle::TensorType_INT8);
  if (status != Ok)
    return status;

#ifndef DIS_QUANT
  if (input_type == circle::TensorType_INT8)
  {
    status = utils::checkCondition(*output->quantization()->scale()->begin() ==
                                   *input->quantization()->scale()->begin());
    if (status != Ok)
      return status;
    status = utils::checkCondition(*output->quantization()->zero_point()->begin() ==
                                   *input->quantization()->zero_point()->begin());
    if (status != Ok)
      return status;
  }
#endif // DIS_QUANT

  int32_t axis = options->axis();

  core::OMRuntimeShape input_shape(input);
  core::OMRuntimeShape positions_shape(positions);

  int32_t num_dims = input_shape.dimensionsCount();
  if (axis < 0)
  {
    axis += num_dims;
  }

  status = utils::checkCondition(axis >= 0 and axis < num_dims);
  if (status != Ok)
    return status;

  int32_t batch_dims = options->batch_dims();
  int32_t coords_num_dims = positions_shape.dimensionsCount();
  // batch_dims should be in range: [-rank(coords), rank(coords)].
  // Negative batch_dims is added with rank of coords.
  if (batch_dims < 0)
  {
    batch_dims += coords_num_dims;
  }

  status = utils::checkCondition(batch_dims <= axis and batch_dims >= 0 and
                                 batch_dims < num_dims and batch_dims <= coords_num_dims);

  if (status != Ok)
    return status;

  for (int i = 0; i < batch_dims; ++i)
  {
    status = utils::checkCondition(input_shape.dims(i) == positions_shape.dims(i));
  }

  return status;
}

} // namespace import
} // namespace onert_micro
