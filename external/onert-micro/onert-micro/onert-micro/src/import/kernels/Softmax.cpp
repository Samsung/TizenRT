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
#include "import/OMUtils.h"

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

OMStatus configure_kernel_CircleSoftmax(const OMConfigureArgs &config_args)
{
  const circle::Tensor *input;
  const circle::Tensor *output;

  SISOHeader(config_args, &input, &output);

  OMStatus status;
  status = utils::checkCondition(input->type() == output->type());
  if (status != Ok)
    return status;

  OMRuntimeShape input_shape(input);
  OMRuntimeShape output_shape(output);

  status = utils::checkCondition(input_shape.dimensionsCount() == output_shape.dimensionsCount());
  if (status != Ok)
    return status;

  status = utils::checkCondition(input_shape.dimensionsCount() >= 1);
  if (status != Ok)
    return status;

  if (input->type() != circle::TensorType_INT8 and input->type() != circle::TensorType_INT16 and
      input->type() != circle::TensorType_UINT8)
    return status;

  // Check quantized version
  if (input->quantization() == nullptr or output->quantization() == nullptr)
    return NoQuantization;

  if (output->quantization()->scale() == nullptr or output->quantization()->scale()->size() != 1)
    return NoQuantization;

  if (input->quantization()->scale() == nullptr or input->quantization()->scale()->size() != 1)
    return NoQuantization;

  if (output->quantization()->zero_point() == nullptr or
      output->quantization()->zero_point()->size() != 1)
    return NoQuantization;

  if (input->quantization()->zero_point() == nullptr or
      input->quantization()->zero_point()->size() != 1)
    return NoQuantization;

  return status;
}

} // namespace import
} // namespace onert_micro
