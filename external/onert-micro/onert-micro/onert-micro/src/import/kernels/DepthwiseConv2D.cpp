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

#include "import/OMKernelConfigureBuilder.h"

#include "execute/OMRuntimeKernel.h"
#include "execute/OMUtils.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t weightTensorIdx = 1;
constexpr uint32_t biasTensorIdx = 2;

constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleDepthwiseConv2D(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  const circle::Tensor *input = runtime_kernel.inputs[inputTensorIdx];
  const circle::Tensor *weight = runtime_kernel.inputs[weightTensorIdx];
  const circle::Tensor *bias = runtime_kernel.inputs[biasTensorIdx];

  const circle::Tensor *output = runtime_kernel.outputs[outputTensorIdx];

  assert(input != nullptr);
  assert(weight != nullptr);
  // Bias can be nullptr
  assert(output != nullptr);

  OMStatus status = Ok;
  const auto *options = runtime_kernel.first_operator->builtin_options_as_DepthwiseConv2DOptions();

  core::OMRuntimeShape input_shape(input);
  core::OMRuntimeShape weight_shape(weight);
  core::OMRuntimeShape bias_shape(bias);
  core::OMRuntimeShape output_shape(output);

  status = utils::checkCondition(input->type() == output->type());
  if (status != Ok)
    return status;

  status = utils::checkCondition(input->type() == weight->type());
  if (status != Ok)
    return status;

  status = utils::checkCondition(input_shape.dimensionsCount() == 4);
  if (status != Ok)
    return status;

  status = utils::checkCondition(input_shape.dimensionsCount() == output_shape.dimensionsCount());
  if (status != Ok)
    return status;

  status = utils::checkCondition(input_shape.dimensionsCount() == weight_shape.dimensionsCount());
  if (status != Ok)
    return status;

  const auto output_depth = output_shape.dims(3);

  status = utils::checkCondition(
    bias == nullptr or (bias_shape.dimensionsCount() == 1 && bias_shape.dims(0) == output_depth));
  if (status != Ok)
    return status;

  switch (options->fused_activation_function())
  {
    case circle::ActivationFunctionType_NONE:
    case circle::ActivationFunctionType_RELU:
    case circle::ActivationFunctionType_RELU6:
    case circle::ActivationFunctionType_RELU_N1_TO_1:
      break;
    default:
      return UnsupportedActivation;
  }

  if (input->type() == circle::TensorType_FLOAT32)
  {
    status = utils::checkCondition(bias == nullptr or input->type() == bias->type());
    return status;
  }

  auto input_quant = input->quantization();
  auto filter_quant = weight->quantization();
  auto output_quant = output->quantization();

  status = utils::checkCondition(input_quant != nullptr and filter_quant != nullptr and
                                 output_quant != nullptr);
  if (status != Ok)
    return status;

  auto input_scales = input_quant->scale();
  auto filter_scales = filter_quant->scale();
  auto output_scales = output_quant->scale();

  status = utils::checkCondition(input_scales != nullptr and filter_scales != nullptr and
                                 output_scales != nullptr);
  if (status != Ok)
    return status;

  // Support only per channel
  status = utils::checkCondition(filter_scales->size() > 1);
  if (status != Ok)
    return status;

  return status;
}

} // namespace import
} // namespace onert_micro
