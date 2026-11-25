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
#include "execute/OMUtils.h"

#include "core/OMKernelData.h"
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr uint32_t inputGradTensorIdx = 0;
constexpr uint32_t weightsTensorIdx = 1;

constexpr uint32_t outputTensorIdx = 0;

} // namespace

OMStatus onert_micro::import::configure_kernel_CircleConv2DInputGrad(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  const circle::Tensor *input_grad = runtime_kernel.inputs[inputGradTensorIdx];
  const circle::Tensor *weights_tensor = runtime_kernel.inputs[weightsTensorIdx];

  const circle::Tensor *output_grad = runtime_kernel.outputs[outputTensorIdx];

  assert(input_grad != nullptr);
  assert(weights_tensor != nullptr);
  // Bias can be nullptr
  assert(output_grad != nullptr);

  OMStatus status = Ok;

  if (input_grad->type() == circle::TensorType_FLOAT32 && weights_tensor->type() != circle::TensorType_FLOAT32)
  {
    return UnsupportedType;
  }

  core::OMRuntimeShape input_grad_shape(input_grad);
  core::OMRuntimeShape weights_shape(weights_tensor);
  core::OMRuntimeShape output_grad_shape(output_grad);

  status = utils::checkCondition(input_grad_shape.dimensionsCount() == 4);
  if (status != Ok)
    return status;

  status = utils::checkCondition(input_grad_shape.dimensionsCount() == output_grad_shape.dimensionsCount());
  if (status != Ok)
    return status;

  status = utils::checkCondition(input_grad_shape.dimensionsCount() == weights_shape.dimensionsCount());
  if (status != Ok)
    return status;

  return status;
}
