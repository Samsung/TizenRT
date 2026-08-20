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
constexpr uint32_t weightTensorIdx = 1;
constexpr uint32_t biasTensorIdx = 2;

constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleFullyConnected(const OMConfigureArgs &config_args)
{

  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;
  OMRuntimeStorage &runtime_storage = config_args.runtime_storage;

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

#ifndef DIS_FLOAT
  if (weight->type() == circle::TensorType_FLOAT32)
  {

    status = utils::checkCondition(input->type() == circle::TensorType_FLOAT32 and
                                   output->type() == circle::TensorType_FLOAT32 and
                                   (!bias or bias->type() == circle::TensorType_FLOAT32));
    if (status != Ok)
      return status;
  }
#endif // DIS_FLOAT
#ifndef DIS_QUANT
  if (weight->type() == circle::TensorType_UINT8)
  {

    status = utils::checkCondition(input->type() == circle::TensorType_UINT8 and
                                   output->type() == circle::TensorType_UINT8 and
                                   (!bias or bias->type() == circle::TensorType_INT32));
    if (status != Ok)
      return status;
  }
  else if (weight->type() == circle::TensorType_INT8)
  {
    status = utils::checkCondition(input->type() == circle::TensorType_INT8 or
                                   input->type() == circle::TensorType_FLOAT32);
    if (status != Ok)
      return status;

    status = utils::checkCondition(output->type() == circle::TensorType_INT8 or
                                   output->type() == circle::TensorType_FLOAT32);
    if (status != Ok)
      return status;

    status = utils::checkCondition(!bias or bias->type() == circle::TensorType_INT32 or
                                   bias->type() == circle::TensorType_INT64 or
                                   bias->type() == circle::TensorType_FLOAT32);
    if (status != Ok)
      return status;

    if (input->type() == circle::TensorType_FLOAT32)
    {
      // hybrid mode
      // Check it is channel wise quantization
      status = utils::checkCondition(weight->quantization() != nullptr and
                                     weight->quantization()->scale() != nullptr);
      if (status != Ok)
        return status;
    }
  }
  else if (weight->type() == circle::TensorType_INT16)
  {

    status = utils::checkCondition(input->type() == circle::TensorType_INT16 and
                                   output->type() == circle::TensorType_INT16 and
                                   (!bias or bias->type() == circle::TensorType_INT32));
    if (status != Ok)
      return status;
  }
#endif // DIS_QUANT

  core::OMRuntimeShape weight_shape(weight);
  core::OMRuntimeShape bias_shape(bias);
  core::OMRuntimeShape input_shape(input);
  core::OMRuntimeShape output_shape(output);

  status = utils::checkCondition(weight_shape.dimensionsCount() == 2);
  if (status != Ok)
    return status;

  if (input_shape.flatSize() == 1 and output_shape.flatSize() != 1)
  {
#ifndef DIS_DYN_SHAPES
    input_shape =
      runtime_storage.getDynamicRuntimeShape(runtime_kernel.inputs_index[inputTensorIdx]);
    if (input_shape.flatSize() == 0)
      return UnsupportedDynamicShapeCase;
#else
    return UnsupportedDynamicShapeCase;
#endif // DIS_DYN_SHAPES
  }

  status = utils::checkCondition(bias == nullptr or weight_shape.dims(0) == bias_shape.flatSize());

  if (input->type() == circle::TensorType_FLOAT32)
    return status;

#ifndef DIS_QUANT

  // Check quantized version
  if (input->quantization() == nullptr or output->quantization() == nullptr or
      weight->quantization() == nullptr)
    return NoQuantization;

  if (output->quantization()->scale() == nullptr or output->quantization()->scale()->size() != 1)
    return UnsupportedQuantizationType;

  if (output->quantization()->zero_point() == nullptr or
      output->quantization()->zero_point()->size() != 1)
    return UnsupportedQuantizationType;

  if (weight->quantization()->scale() == nullptr or weight->quantization()->scale()->size() != 1)
    return UnsupportedQuantizationType;

  if (weight->quantization()->zero_point() == nullptr or
      weight->quantization()->zero_point()->size() != 1)
    return UnsupportedQuantizationType;

#endif // DIS_QUANT

  return status;
}

} // namespace import
} // namespace onert_micro
