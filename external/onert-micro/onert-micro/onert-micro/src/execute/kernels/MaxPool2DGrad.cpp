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
#include "execute/OMUtils.h"
#include "execute/OMRuntimeKernel.h"
#include "core/OMUtils.h"

#include "PALMaxPool2DGrad.h"

#include "core/OMKernelData.h"

using namespace onert_micro;
using namespace onert_micro::execute;

namespace
{

constexpr uint32_t inputGradTensorIdx = 0;
constexpr uint32_t inputActivationTensorIdx = 0;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

// NOTE: doesnt currently support dynamic shapes
OMStatus onert_micro::execute::execute_kernel_CircleMaxPool2DGrad(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input_grad = nullptr;
  const circle::Tensor *input_activation = nullptr;
  const circle::Tensor *output = nullptr;

  uint8_t *input_grad_data = nullptr;
  uint8_t *input_activation_data = nullptr;
  uint8_t *output_data = nullptr;

  OMStatus status = Ok;

  const circle::Pool2DOptions *options = nullptr;
  {
    OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, runtime_context);

    input_grad = runtime_kernel.inputs[inputGradTensorIdx];
    input_activation = runtime_kernel.inputs[inputActivationTensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];

    assert(input_grad != nullptr);
    assert(input_activation != nullptr);
    assert(output != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    input_grad_data = runtime_kernel.inputs_data[inputGradTensorIdx];
    input_activation_data = runtime_kernel.inputs_data[inputActivationTensorIdx];
    output_data = runtime_kernel.outputs_data[outputTensorIdx];

    options = runtime_kernel.first_operator->builtin_options_as_Pool2DOptions();
  }

  assert(input_grad_data != nullptr);
  assert(input_activation_data != nullptr);
  assert(output_data != nullptr);
  assert(options != nullptr);

  core::OMRuntimeShape input_grad_shape(input_grad);
  core::OMRuntimeShape input_activation_shape(input_activation);

  int32_t padding_h = 0;
  int32_t padding_w = 0;

  const int input_width = input_activation_shape.dims(2);
  const int input_height = input_activation_shape.dims(1);
  execute::computePaddingHeightWidth(
    options->stride_h(), options->stride_w(), 1 /* dilation_rate_height */,
    1 /* dilation_rate_width */, input_height, input_width, options->filter_height(),
    options->filter_width(), options->padding(), &padding_h, &padding_w);

  core::Pool2DParams params{};
  params.pad_h = padding_h;
  params.pad_w = padding_w;
  params.stride_h = options->stride_h();
  params.stride_w = options->stride_w();
  params.filter_h = options->filter_height();
  params.filter_w = options->filter_width();

  switch (input_grad->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      calculateActivationRange(options->fused_activation_function(), &params.activation_min,
                               &params.activation_max);
      status = pal::MaxPoolGrad(
        params, input_grad_shape, core::utils::castInputData<float>(input_grad_data),
        input_activation_shape, core::utils::castInputData<float>(input_activation_data),
        core::OMRuntimeShape(output), core::utils::castOutputData<float>(output_data));
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
