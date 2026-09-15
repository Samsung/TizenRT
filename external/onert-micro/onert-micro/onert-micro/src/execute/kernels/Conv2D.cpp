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
#include "execute/kernels/ConvolutionCommon.h"

#include "PALConv2D.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t weightTensorIdx = 1;
constexpr uint32_t biasTensorIdx = 2;

constexpr uint32_t outputTensorIdx = 0;

} // namespace

// NOTE: doesn't currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleConv2D(const OMExecuteArgs &execute_args)
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

  const circle::Conv2DOptions *options;
  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
    if (status != Ok)
      return status;

    input = runtime_kernel.inputs[inputTensorIdx];
    weight = runtime_kernel.inputs[weightTensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];
    assert(input != nullptr);
    assert(weight != nullptr);
    // Bias can be nullptr
    assert(output != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    input_data = runtime_kernel.inputs_data[inputTensorIdx];
    weight_data = runtime_kernel.inputs_data[weightTensorIdx];
    bias_data = runtime_kernel.inputs_data[biasTensorIdx];
    output_data = runtime_kernel.outputs_data[outputTensorIdx];
    assert(input_data != nullptr);
    assert(weight_data != nullptr);
    // Bias can be nullptr
    assert(output_data != nullptr);

    options = runtime_kernel.first_operator->builtin_options_as_Conv2DOptions();
  }

  OMStatus status;

  int32_t padding_h = 0;
  int32_t padding_w = 0;

  OMRuntimeShape weight_shape(weight);
  OMRuntimeShape input_shape(input);
  OMRuntimeShape output_shape(output);

  const int input_width = input_shape.dims(2);
  const int input_height = input_shape.dims(1);
  const int weight_width = weight_shape.dims(2);
  const int weight_height = weight_shape.dims(1);
  execute::computePaddingHeightWidth(options->stride_h(), options->stride_w(),
                                     options->dilation_h_factor(), options->dilation_w_factor(),
                                     input_height, input_width, weight_height, weight_width,
                                     options->padding(), &padding_h, &padding_w);

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      FloatConv2D params{};
      status = calculateActivationRange(options->fused_activation_function(),
                                        &params.activation_min, &params.activation_max);
      params.stride_w = options->stride_w();
      params.stride_h = options->stride_h();
      params.dilation_width_factor = options->dilation_w_factor();
      params.dilation_height_factor = options->dilation_h_factor();
      params.pad_h = padding_h;
      params.pad_w = padding_w;

      if (status != Ok)
        return status;

      status = pal::ConvFloat(&params, input_shape, core::utils::castInputData<float>(input_data),
                              weight_shape, core::utils::castInputData<float>(weight_data),
                              core::utils::castInputData<float>(bias_data), output_shape,
                              core::utils::castOutputData<float>(output_data));
      assert(status == Ok);
    }
    break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case circle::TensorType_INT8:
    {
      ConvQuant params{};
      params.pad_h = padding_h;
      params.pad_w = padding_w;

      const auto padding = options->padding();
      const auto stride_height = options->stride_h();
      const auto stride_width = options->stride_w();
      const auto dilation_height_factor = options->dilation_h_factor();
      const auto dilation_width_factor = options->dilation_h_factor();

      params.stride_height = stride_height;
      params.stride_width = stride_width;
      params.dilation_height_factor = dilation_height_factor;
      params.dilation_width_factor = dilation_width_factor;

      status =
        createConvParams(params, input, weight, output, options->fused_activation_function());
      assert(status == Ok);
      if (status != Ok)
        return status;

      status =
        pal::ConvPerChannel(params, input_shape, core::utils::castInputData<int8_t>(input_data),
                            weight_shape, core::utils::castInputData<int8_t>(weight_data),
                            core::utils::castInputData<int32_t>(bias_data), output_shape,
                            core::utils::castOutputData<int8_t>(output_data));
    }
    break;
#endif // DIS_QUANT
    default:
    {
      status = UnsupportedActivation;
      assert(false && "Unsupported type.");
    }
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
