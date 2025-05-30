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

#include "execute/OMKernelExecutionBuilder.h"
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"
#include "core/OMUtils.h"
#include "PALDequantize.h"
#include "core/OMRuntimeShape.h"

#include "execute/OMUtils.h"

using namespace onert_micro;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

// NOTE: doesnt currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleDequantize(const OMExecuteArgs &execute_args)
{
  const circle::Tensor *input = nullptr;
  const circle::Tensor *output = nullptr;

  uint8_t *input_data = nullptr;
  uint8_t *output_data = nullptr;

  SISOHeader(execute_args, &input, &output, &input_data, &output_data);

  assert(output->type() == circle::TensorType_FLOAT32);

  OMStatus status = Ok;
  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT16:
    {
      status = pal::DequantizeF16toF32(core::OMRuntimeShape(input).flatSize(),
                               core::utils::castInputData<uint16_t>(input_data),
                               core::utils::castOutputData<float>(output_data));
    }
    break;
    case circle::TensorType_INT8:
    {
      assert(input->quantization() != nullptr);
      assert(input->quantization()->scale() != nullptr and
             input->quantization()->scale()->size() == 1);
      assert(input->quantization()->zero_point() != nullptr and
             input->quantization()->zero_point()->size() == 1);
      core::QuantizationParams params{};
      params.zero_point = input->quantization()->zero_point()->operator[](0);
      params.scale = input->quantization()->scale()->operator[](0);

      status = pal::Dequantize(params, core::OMRuntimeShape(input).flatSize(),
                               core::utils::castInputData<int8_t>(input_data),
                               core::utils::castOutputData<float>(output_data));
    }
    break;
#endif // DIS_FLOAT
    default:
    {
      status = UnsupportedType;
      assert(false && "Unsupported type.");
    }
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
