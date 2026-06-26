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

OMStatus configure_kernel_CircleQuantize(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  onert_micro::execute::OMRuntimeKernel runtime_kernel;

  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  const circle::Tensor *input = runtime_kernel.inputs[inputTensorIdx];
  const circle::Tensor *output = runtime_kernel.outputs[outputTensorIdx];

  assert(input != nullptr);
  assert(output != nullptr);

  OMRuntimeShape input_shape(input);
  OMRuntimeShape output_shape(output);

  // Check shapes
  status = utils::checkCondition(input_shape == output_shape);
  if (status != Ok)
    return status;

  // Check input type is float
  status = utils::checkCondition(input->type() == circle::TensorType_FLOAT32);
  if (status != Ok)
    return status;

  // Check output quantization params
  const auto *output_quantization = output->quantization();
  status = utils::checkCondition(output->type() != circle::TensorType_FLOAT32 or
                                 output_quantization != nullptr and
                                   output_quantization->scale() != nullptr and
                                   output_quantization->scale()->size() == 1);
  if (status != Ok)
    return status;

  return status;
}

} // namespace import
} // namespace onert_micro
