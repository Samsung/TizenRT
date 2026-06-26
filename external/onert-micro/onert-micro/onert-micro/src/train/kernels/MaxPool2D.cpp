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
#include "core/OMDataType.h"
#include "train/OMBackpropExecutionBuilder.h"
#include "execute/OMRuntimeKernel.h"
#include "execute/OMUtils.h"
#include "core/memory/OMMemoryManager.h"
#include "PALMaxPool2DInputGrad.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::train;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

/*
 * - Calculate input gradient - Optional (not required if it is last op)
 */
OMStatus onert_micro::train::train_kernel_CircleMaxPool2D(const OMBackpropExecuteArgs &args)
{
  // Check is it last layer for training
  if (args.is_last_layer)
  {
    return Ok;
  }

  core::OMRuntimeStorage &forward_storage = args.forward_storage;
  core::OMRuntimeStorage &backward_storage = args.backward_storage;
  core::OMRuntimeContext &context = args.backward_context;
  uint16_t op_index = args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *output;

  uint8_t *input_data;
  uint8_t *dloss_dinput_data;

  uint8_t *dloss_doutput_data;

  const circle::Pool2DOptions *options;
  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, context);

    input = runtime_kernel.inputs[inputTensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];
    assert(input != nullptr);
    assert(output != nullptr);

    // Read forward storage
    {
      runtime_kernel.getDataFromStorage(op_index, forward_storage, context);

      input_data = runtime_kernel.inputs_data[inputTensorIdx];
      assert(input_data != nullptr);
    }

    // Read backward storage
    {
      runtime_kernel.getDataFromStorage(op_index, backward_storage, context);

      dloss_dinput_data = runtime_kernel.inputs_data[inputTensorIdx];
      dloss_doutput_data = runtime_kernel.outputs_data[outputTensorIdx];

      assert(dloss_dinput_data != nullptr);
      assert(dloss_doutput_data != nullptr);
    }

    options = runtime_kernel.first_operator->builtin_options_as_Pool2DOptions();
  }

  assert(options->fused_activation_function() == circle::ActivationFunctionType_NONE);
  if (options->fused_activation_function() != circle::ActivationFunctionType_NONE)
    return UnsupportedType;

  OMRuntimeShape input_shape(input);
  OMRuntimeShape output_shape(output);

  int32_t padding_h = 0;
  int32_t padding_w = 0;

  const int input_width = input_shape.dims(2);
  const int input_height = input_shape.dims(1);
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

  // Set input grad to zero
  for (size_t i = 0; i < input_shape.flatSize() * sizeof(float); i += sizeof(float))
    *static_cast<float *>(static_cast<void *>(dloss_dinput_data + i)) = 0;

  // Calculate input grad
  pal::MaxPool2D(params, input_shape, core::utils::castInputData<float>(input_data), output_shape,
                 core::utils::castInputData<float>(dloss_doutput_data),
                 core::utils::castOutputData<float>(dloss_dinput_data));

  return Ok;
}
