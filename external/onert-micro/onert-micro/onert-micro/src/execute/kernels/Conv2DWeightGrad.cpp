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
#include "execute/OMUtils.h"
#include "core/OMUtils.h"
#include "core/OMKernelData.h"
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"

#include "PALConv2DWeightGrad.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t inputGradTensorIdx = 0;
constexpr uint32_t inputActivationTensorIdx = 1;

constexpr uint32_t outputTensorIdx = 0;

} // namespace

// NOTE: doesn't currently support dynamic shapes
OMStatus onert_micro::execute::execute_kernel_CircleConv2DWeightGrad(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input_grad;
  const circle::Tensor *input_activation;
  const circle::Tensor *output_grad;

  uint8_t *input_grad_data;
  uint8_t *input_activation_data;
  uint8_t *output_grad_data;

  const circle::Conv2DWeightGradOptions *options;
  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
    if (status != Ok)
      return status;

    input_grad = runtime_kernel.inputs[inputGradTensorIdx];
    input_activation = runtime_kernel.inputs[inputActivationTensorIdx];
    output_grad = runtime_kernel.outputs[outputTensorIdx];
    assert(input_grad != nullptr);
    assert(input_activation != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    input_grad_data = runtime_kernel.inputs_data[inputGradTensorIdx];
    input_activation_data = runtime_kernel.inputs_data[inputActivationTensorIdx];
    output_grad_data = runtime_kernel.outputs_data[outputTensorIdx];
    assert(input_grad_data != nullptr);
    assert(input_activation != nullptr);

    options = runtime_kernel.first_operator->builtin_options_as_Conv2DWeightGradOptions();
  }

  OMStatus status = Ok;

  int32_t padding_h = 0;
  int32_t padding_w = 0;

  OMRuntimeShape input_grad_shape(input_grad);
  OMRuntimeShape input_activation_shape(input_activation);
  OMRuntimeShape output_shape(output_grad);

  const int input_width = input_activation_shape.dims(3);
  const int input_height = input_activation_shape.dims(2);
  const int weight_width = output_shape.dims(3);
  const int weight_height = output_shape.dims(2);
  execute::computePaddingHeightWidth(options->stride_h(), options->stride_w(),
                                     options->dilation_h_factor(), options->dilation_w_factor(),
                                     input_height, input_width, weight_height, weight_width,
                                     options->padding(), &padding_h, &padding_w);

  switch (input_grad->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {

      FloatConv2D params{};

      params.stride_w = options->stride_w();
      params.stride_h = options->stride_h();
      params.dilation_width_factor = options->dilation_w_factor();
      params.dilation_height_factor = options->dilation_h_factor();
      params.pad_h = padding_h;
      params.pad_w = padding_w;

      if (status != Ok)
        return status;

      status = pal::ConvWeightGradFloat(
        &params, input_activation_shape, core::utils::castInputData<float>(input_activation_data),
        input_grad_shape, core::utils::castInputData<float>(input_grad_data),
        OMRuntimeShape(output_shape), core::utils::castOutputData<float>(output_grad_data));
      assert(status == Ok);
    }
    break;
#endif // DIS_FLOAT
    default:
    {
      status = UnsupportedActivation;
      assert(false && "Unsupported type.");
    }
  }

  return status;
}
