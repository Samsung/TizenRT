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

#include "import/helpers/OMConfigureTISOKernel.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr uint32_t input1TensorIdx = 0;
constexpr uint32_t input2TensorIdx = 1;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

// TISO - Two Inputs Single Output
OMStatus onert_micro::import::helpers::configure_TISO_kernel(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  onert_micro::execute::OMRuntimeKernel runtime_kernel;

  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  const circle::Tensor *input1 = runtime_kernel.inputs[input1TensorIdx];
  const circle::Tensor *input2 = runtime_kernel.inputs[input2TensorIdx];
  const circle::Tensor *output = runtime_kernel.outputs[outputTensorIdx];

  assert(input1 != nullptr);
  assert(input2 != nullptr);
  assert(output != nullptr);

  status = utils::checkCondition(input1->type() == output->type());
  if (status != Ok)
    return status;

  status = utils::checkCondition(input2->type() == output->type());
  if (status != Ok)
    return status;

  if (input1->type() != circle::TensorType_INT8 and input1->type() != circle::TensorType_INT16)
    return status;

#ifndef DIS_QUANT

  // Check quantization params
  if (input1->quantization() == nullptr or input2->quantization() == nullptr or
      output->quantization() == nullptr)
  {
    return NoQuantization;
  }

  if (input1->quantization()->scale() == nullptr or
      input1->quantization()->zero_point() == nullptr or
      input1->quantization()->scale()->size() != 1 or
      input1->quantization()->zero_point()->size() != 1)
  {
    return NoQuantization;
  }

  if (input2->quantization()->scale() == nullptr or
      input2->quantization()->zero_point() == nullptr or
      input2->quantization()->scale()->size() != 1 or
      input2->quantization()->zero_point()->size() != 1)
  {
    return NoQuantization;
  }

  if (output->quantization()->scale() == nullptr or
      output->quantization()->zero_point() == nullptr or
      output->quantization()->scale()->size() != 1 or
      output->quantization()->zero_point()->size() != 1)
  {
    return NoQuantization;
  }

#endif // DIS_QUANT

  return status;
}
