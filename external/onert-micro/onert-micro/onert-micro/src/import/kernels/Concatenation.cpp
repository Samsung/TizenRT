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

#include "import/OMKernelConfigureBuilder.h"
#include "core/OMUtils.h"
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"

#include "core/OMDataType.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleConcatenation(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  onert_micro::execute::OMRuntimeKernel runtime_kernel;

  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  const int num_inputs = runtime_kernel.inputs_num;

  const auto *t0 = runtime_kernel.inputs[0];
  const auto *output = runtime_kernel.outputs[0];

  const auto *params = runtime_kernel.first_operator->builtin_options_as_ConcatenationOptions();

  // TODO: Support concat with fused activation function
  if (params->fused_activation_function() != circle::ActivationFunctionType_NONE)
    return UnknownError;

  OMRuntimeShape input_shape(t0);
  int axis = params->axis();
  if (axis < 0)
    axis += input_shape.dimensionsCount();

  if (axis < 0 or axis > input_shape.dimensionsCount())
    return FailedCheckCondition;

  for (int i = 1; i < num_inputs; ++i)
  {
    const auto *tensor = runtime_kernel.inputs[i];
    if (tensor->type() != t0->type())
      return FailedCheckCondition;
  }

  if (t0->type() != circle::TensorType_INT8 and t0->type() != circle::TensorType_INT16)
    return Ok;

#ifndef DIS_QUANT
  // If input tensors are INT8 or INT16 type then quantization parameters of all input tensors and
  // the output should be the same
  for (int i = 0; i < num_inputs; ++i)
  {
    const auto *tensor = runtime_kernel.inputs[i];

    if (tensor->quantization() == nullptr)
      return FailedCheckCondition;

    if (tensor->quantization()->scale()->size() != 1)
      return FailedCheckCondition;

    if (tensor->quantization()->zero_point()->size() != 1)
      return FailedCheckCondition;

    if (*tensor->quantization()->scale()->begin() != *output->quantization()->scale()->begin())
      return FailedCheckCondition;

    if (*tensor->quantization()->zero_point()->begin() !=
        *output->quantization()->zero_point()->begin())
      return FailedCheckCondition;
  }
#endif // DIS_QUANT

  return Ok;
}

} // namespace import
} // namespace onert_micro
