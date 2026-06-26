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

#include "import/helpers/OMPooingCommon.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

OMStatus
onert_micro::import::helpers::configure_pooling_kernel_common(const OMConfigureArgs &config_args)
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

  status = utils::checkCondition(input->type() == output->type());
  if (status != Ok)
    return status;

  OMRuntimeShape input_shape(input);
  OMRuntimeShape output_shape(output);

  status = utils::checkCondition(input_shape.dimensionsCount() == output_shape.dimensionsCount());
  if (status != Ok)
    return status;

  status = utils::checkCondition(input_shape.dimensionsCount() == 4);

  auto option = runtime_kernel.first_operator->builtin_options_as_Pool2DOptions();

  if (option == nullptr)
    return UnknownError;

  assert(option != nullptr);

  if (input->type() != circle::TensorType_INT8 and input->type() != circle::TensorType_INT16)
    return status;

  // Check quantization params
  if (input->quantization() == nullptr)
  {
    return NoQuantization;
  }

  if (input->quantization()->scale()->size() != 1)
  {
    return UnsupportedType;
  }

  // Check quantization params
  if (output->quantization() == nullptr)
  {
    return NoQuantization;
  }

  if (output->quantization()->scale()->size() != 1)
  {
    return UnsupportedType;
  }

  return status;
}
