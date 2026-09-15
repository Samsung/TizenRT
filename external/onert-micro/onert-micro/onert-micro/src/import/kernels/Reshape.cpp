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
constexpr uint32_t shapeTensorIdx = 1;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleReshape(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;
  OMRuntimeStorage &runtime_storage = config_args.runtime_storage;

  onert_micro::execute::OMRuntimeKernel runtime_kernel;

  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  const circle::Tensor *input = runtime_kernel.inputs[inputTensorIdx];
  const circle::Tensor *shape = runtime_kernel.inputs[shapeTensorIdx];
  const circle::Tensor *output = runtime_kernel.outputs[outputTensorIdx];

  assert(input != nullptr);
  assert(shape != nullptr);
  assert(output != nullptr);

  status = utils::checkCondition(input->type() == output->type());
  if (status != Ok)
    return status;

  OMRuntimeShape input_shape(input);
  OMRuntimeShape output_shape(output);

#ifndef DIS_DYN_SHAPES
  auto is_dynamic =
    runtime_context.isConstTensor(runtime_kernel.inputs_index[shapeTensorIdx]) == false;

  if (is_dynamic)
  {
    auto input_shape_size = input_shape.flatSize();

    status = utils::checkCondition(output_shape.flatSize() == 1);
    if (status != Ok)
      return status;

    status = utils::checkCondition(input_shape_size != 1);
    if (status != Ok)
      return status;

    runtime_storage.setDynamicRuntimeShape(runtime_kernel.outputs_index[outputTensorIdx],
                                           input_shape);
  }
  else
  {
    status = utils::checkCondition(input_shape.flatSize() == output_shape.flatSize());
    assert(status == Ok);
    if (status != Ok)
      return status;
  }
#else
  status = utils::checkCondition(
    runtime_context.getCircleReader().isConstTensor(runtime_kernel.inputs_index[shapeTensorIdx]));
  assert(status == Ok);
  if (status != Ok)
    return status;
#endif // DIS_DYN_SHAPES

  return status;
}

} // namespace import
} // namespace onert_micro
