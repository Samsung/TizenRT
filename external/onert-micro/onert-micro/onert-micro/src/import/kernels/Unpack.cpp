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
constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleUnpack(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  OMRuntimeStorage &runtime_storage = config_args.runtime_storage;
  uint16_t op_index = config_args.kernel_index;

  onert_micro::execute::OMRuntimeKernel runtime_kernel;

  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  const circle::Tensor *input = runtime_kernel.inputs[inputTensorIdx];
  const circle::Tensor *output = runtime_kernel.outputs[outputTensorIdx];

  assert(input != nullptr);
  assert(output != nullptr);

  status = utils::checkCondition(output->type() == input->type());
  if (status != Ok)
    return status;

  auto options = runtime_kernel.first_operator->builtin_options_as_UnpackOptions();
  status = utils::checkCondition(options != nullptr);
  if (status != Ok)
    return status;

  status = utils::checkCondition(runtime_kernel.outputs_num == options->num());
  if (status != Ok)
    return status;

  // Check all outputs have the same type and shapes
  OMRuntimeShape output_shape(output);
  for (uint32_t i = 1; i < options->num(); ++i)
  {
    const circle::Tensor *cur_output = runtime_kernel.outputs[i];
    status = utils::checkCondition(output->type() == cur_output->type());
    if (status != Ok)
      return status;

    OMRuntimeShape cur_output_shape(cur_output);
    status = utils::checkCondition(output_shape == cur_output_shape);
    if (status != Ok)
      return status;
  }

  // Check shapes input and output
  OMRuntimeShape input_shape(input);
  for (int i = 0; i < input_shape.dimensionsCount(); ++i)
  {
    if (i == options->axis())
      continue;

    if (i < options->axis())
    {
      status = utils::checkCondition(input_shape.dims(i) == output_shape.dims(i));
    }
    else
    {
      status = utils::checkCondition(input_shape.dims(i) == output_shape.dims(i - 1));
    }
    if (status != Ok)
      return status;
  }

  return status;
}

} // namespace import
} // namespace onert_micro
