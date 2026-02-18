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

#include "import/OMKernelConfigureBuilder.h"

#include "core/OMUtils.h"
#include "core/OMKernelData.h"

#include "execute/OMRuntimeKernel.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t hiddenHiddenTensorIdx = 1;
constexpr uint32_t hiddenHiddenBiasTensorIdx = 2;
constexpr uint32_t hiddenInputTensorIdx = 3;
constexpr uint32_t hiddenInputBiasTensorIdx = 4;
constexpr uint32_t stateTensorIdx = 5;

constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleGRU(const OMConfigureArgs &config_args)
{
  core::OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *hidden_hidden;
  const circle::Tensor *hidden_hidden_bias;
  const circle::Tensor *hidden_input;
  const circle::Tensor *hidden_input_bias;
  const circle::Tensor *state;

  const circle::Tensor *output;

  // Read kernel
  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  input = runtime_kernel.inputs[inputTensorIdx];
  hidden_hidden = runtime_kernel.inputs[hiddenHiddenTensorIdx];
  hidden_hidden_bias = runtime_kernel.inputs[hiddenHiddenBiasTensorIdx];
  hidden_input = runtime_kernel.inputs[hiddenInputTensorIdx];
  hidden_input_bias = runtime_kernel.inputs[hiddenInputBiasTensorIdx];
  state = runtime_kernel.inputs[stateTensorIdx];

  output = runtime_kernel.outputs[outputTensorIdx];
  assert(input != nullptr);
  assert(hidden_hidden != nullptr);
  assert(hidden_input != nullptr);
  assert(state != nullptr);
  // Biases can be nullptr
  assert(output != nullptr);

  OMStatus status = Ok;

  OMRuntimeShape hidden_hidden_shape(hidden_hidden);
  OMRuntimeShape hidden_input_shape(hidden_input);
  OMRuntimeShape output_shape(output);
  OMRuntimeShape state_shape(state);

  status = utils::checkCondition(hidden_hidden_shape.dims(0) == hidden_input_shape.dims(0));
  if (status != Ok)
    return status;

  const int32_t div_factor = 3;
  status =
    utils::checkCondition(hidden_hidden_shape.dims(0) ==
                          (div_factor * output_shape.dims(output_shape.dimensionsCount() - 1)));
  if (status != Ok)
    return status;

  status = utils::checkCondition(output_shape.dims(output_shape.dimensionsCount() - 1) ==
                                 state_shape.dims(state_shape.dimensionsCount() - 1));
  if (status != Ok)
    return status;

  status = utils::checkCondition(input->type() == output->type());
  if (status != Ok)
    return status;

  return status;
}

} // namespace import
} // namespace onert_micro
