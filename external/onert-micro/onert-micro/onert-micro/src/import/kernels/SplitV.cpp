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
constexpr uint32_t sizeTensorIdx = 1;
constexpr uint32_t axisTensorIdx = 2;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleSplitV(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  OMRuntimeStorage &runtime_storage = config_args.runtime_storage;
  uint16_t op_index = config_args.kernel_index;

  onert_micro::execute::OMRuntimeKernel runtime_kernel;

  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  const circle::Tensor *input = runtime_kernel.inputs[inputTensorIdx];
  const circle::Tensor *size = runtime_kernel.inputs[sizeTensorIdx];
  const circle::Tensor *axis = runtime_kernel.inputs[axisTensorIdx];
  const circle::Tensor *output = runtime_kernel.outputs[outputTensorIdx];

  assert(input != nullptr);
  assert(axis != nullptr);
  assert(size != nullptr);
  assert(output != nullptr);

  status = utils::checkCondition(output->type() == input->type());
  if (status != Ok)
    return status;

  // Check all outputs have the same type
  for (uint32_t i = 1; i < runtime_kernel.outputs_num; ++i)
  {
    status = utils::checkCondition(output->type() == runtime_kernel.outputs[i]->type());
    if (status != Ok)
      return status;
  }

  status = utils::checkCondition(axis->type() == circle::TensorType_INT32);
  if (status != Ok)
    return status;

  status = utils::checkCondition(size->type() == circle::TensorType_INT32);
  if (status != Ok)
    return status;

  // Check size tensor contains exactly output_nums_tensors values
  status = utils::checkCondition(OMRuntimeShape(size).flatSize() == runtime_kernel.outputs_num);

  // Check it is scalar
  status = utils::checkCondition(OMRuntimeShape(axis).flatSize() == 1);
  if (status != Ok)
    return status;

  // Check axis value
  runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
  auto *axis_data = reinterpret_cast<int32_t *>(runtime_kernel.inputs_data[axisTensorIdx]);
  status = utils::checkCondition(axis_data != nullptr);
  if (status != Ok)
    return status;

  int32_t axis_value = axis_data[0];

  OMRuntimeShape input_shape(input);
  if (axis_value < 0)
  {
    axis_value += input_shape.dimensionsCount() + 1;
  }

  status = utils::checkCondition(axis_value <= input_shape.dimensionsCount() and axis_value >= 0);
  if (status != Ok)
    return status;

  return status;
}

} // namespace import
} // namespace onert_micro
