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

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMUtils.h"
#include "execute/OMRuntimeKernel.h"

#include "PALFullyConnected.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t weightTensorIdx = 1;
constexpr uint32_t biasTensorIdx = 2;

constexpr uint32_t outputTensorIdx = 0;

#ifndef DIS_QUANT
void calculateOpDataFullyConnected(const circle::Tensor *input, const circle::Tensor *weights,
                                   const circle::Tensor *output,
                                   circle::ActivationFunctionType activation,
                                   FullyConnectedParams &params)
{
  double real_multiplier = 0.0;
  int output_shift;
  int32_t output_activation_min;
  int32_t output_activation_max;
  int32_t output_multiplier;

  assert(input->quantization() != nullptr);                 // Fix caller
  assert(input->quantization()->scale()->size() == 1);      // Fix caller
  assert(input->quantization()->zero_point()->size() == 1); // Fix caller

  assert(weights->quantization() != nullptr);                 // Fix caller
  assert(weights->quantization()->scale()->size() == 1);      // Fix caller
  assert(weights->quantization()->zero_point()->size() == 1); // Fix caller

  assert(output->quantization() != nullptr);                 // Fix caller
  assert(output->quantization()->scale()->size() == 1);      // Fix caller
  assert(output->quantization()->zero_point()->size() == 1); // Fix caller

  const float input_scale = *input->quantization()->scale()->begin();
  const float weight_scale = *weights->quantization()->scale()->begin();
  const float output_scale = *output->quantization()->scale()->begin();

  const long input_zero_point = *input->quantization()->zero_point()->begin();
  const long weights_zero_point = *weights->quantization()->zero_point()->begin();
  const long output_zero_point = *output->quantization()->zero_point()->begin();

  real_multiplier =
    execute::getQuantizedConvolutionMultipler(input_scale, weight_scale, output_scale);
  execute::quantizeMultiplier(real_multiplier, &output_multiplier, &output_shift);
  execute::calculateActivationRangeQuantized(activation, output_zero_point, output_scale,
                                             output->type(), &output_activation_min,
                                             &output_activation_max);

  params.output_shift = output_shift;
  params.output_multiplier = output_multiplier;
  params.input_offset = -input_zero_point;
  params.weights_offset = -weights_zero_point;
  params.output_offset = output_zero_point;
  params.quantized_activation_max = output_activation_max;
  params.quantized_activation_min = output_activation_min;
}
#endif

} // namespace

namespace onert_micro
{
namespace execute
{

// NOTE: doesnt currently support dynamic shapes
OMStatus execute_kernel_CircleFullyConnected(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *weight;
  const circle::Tensor *output;

  uint8_t *input_data;
  uint8_t *weight_data;
  uint8_t *bias_data;
  uint8_t *output_data;

  const circle::FullyConnectedOptions *options;
  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, runtime_context);

    input = runtime_kernel.inputs[inputTensorIdx];
    weight = runtime_kernel.inputs[weightTensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];
    assert(input != nullptr);
    assert(weight != nullptr);
    // Bias can be nullptr
    assert(output != nullptr);

    runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);

    input_data = runtime_kernel.inputs_data[inputTensorIdx];
    weight_data = runtime_kernel.inputs_data[weightTensorIdx];
    bias_data = runtime_kernel.inputs_data[biasTensorIdx];
    output_data = runtime_kernel.outputs_data[outputTensorIdx];
    assert(input_data != nullptr);
    assert(weight_data != nullptr);
    // Bias can be nullptr
    assert(output_data != nullptr);

    options = runtime_kernel.first_operator->builtin_options_as_FullyConnectedOptions();
  }

  OMStatus status;

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      FullyConnectedParams params{};
      status = calculateActivationRange(options->fused_activation_function(),
                                        &params.float_activation_min, &params.float_activation_max);
      if (status != Ok)
        return status;

      switch (weight->type())
      {
        case circle::TensorType_FLOAT32:
        {

          status = pal::FullyConnected(
            params, core::utils::castInputData<float>(input_data), OMRuntimeShape(weight),
            core::utils::castInputData<float>(weight_data),
            core::utils::castInputData<float>(bias_data), OMRuntimeShape(output),
            core::utils::castOutputData<float>(output_data));
        }
        break;
        case circle::TensorType_INT8:
        {
          // weight quantized INT8 mode
          params.weights_scales =
            reinterpret_cast<const float *>(weight->quantization()->scale()->data());
          params.is_channel_wise_quant = weight->quantization()->scale()->size() > 1;

          status = pal::FullyConnected(
            params, core::utils::castInputData<float>(input_data), OMRuntimeShape(weight),
            core::utils::castInputData<int8_t>(weight_data),
            core::utils::castInputData<float>(bias_data), OMRuntimeShape(output),
            core::utils::castOutputData<float>(output_data));
        }
        break;
        default:
          assert(false && "Unsupported hybrid weight type");
      }
    }
    break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case circle::TensorType_INT8:
    {
      FullyConnectedParams op_params{};

      calculateOpDataFullyConnected(input, weight, output, options->fused_activation_function(),
                                    op_params);

      status =
        pal::FullyConnected(op_params, core::utils::castInputData<int8_t>(input_data),
                            OMRuntimeShape(weight), core::utils::castInputData<int8_t>(weight_data),
                            core::utils::castInputData<int32_t>(bias_data), OMRuntimeShape(output),
                            core::utils::castOutputData<int8_t>(output_data));
    }
    break;
    case circle::TensorType_INT16:
    {
      FullyConnectedParams op_params{};

      calculateOpDataFullyConnected(input, weight, output, options->fused_activation_function(),
                                    op_params);

      status =
        pal::FullyConnected(op_params, core::utils::castInputData<int16_t>(input_data),
                            OMRuntimeShape(weight), core::utils::castInputData<int8_t>(weight_data),
                            core::utils::castInputData<int32_t>(bias_data), OMRuntimeShape(output),
                            core::utils::castOutputData<int16_t>(output_data));
    }
    break;
#endif // DIS_QUANT
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
