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

#include "execute/kernels/PoolingCommon.h"
#include "execute/OMUtils.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace execute
{

OMStatus execute_pooling_common(
  const OMExecuteArgs &execute_args,
  const std::function<OMStatus(const core::Pool2DParams &, const core::OMRuntimeShape &,
                               const float *, const core::OMRuntimeShape &, float *)> &f_float,
  const std::function<OMStatus(const core::Pool2DParams &, const core::OMRuntimeShape &,
                               const int8_t *, const core::OMRuntimeShape &, int8_t *)> &f_int8)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input = nullptr;
  const circle::Tensor *output = nullptr;

  uint8_t *input_data = nullptr;
  uint8_t *output_data = nullptr;

  OMStatus status = Ok;

  const circle::Pool2DOptions *options = nullptr;
  {
    OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, runtime_context);

    input = runtime_kernel.inputs[inputTensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];

    assert(input != nullptr);
    assert(output != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    input_data = runtime_kernel.inputs_data[inputTensorIdx];
    output_data = runtime_kernel.outputs_data[outputTensorIdx];

    options = runtime_kernel.first_operator->builtin_options_as_Pool2DOptions();
  }

  assert(input_data != nullptr);
  assert(output_data != nullptr);
  assert(options != nullptr);

  core::OMRuntimeShape input_shape(input);

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

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      calculateActivationRange(options->fused_activation_function(), &params.activation_min,
                               &params.activation_max);
      status =
        f_float(params, input_shape, core::utils::castInputData<float>(input_data),
                core::OMRuntimeShape(output), core::utils::castOutputData<float>(output_data));
    }
    break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case circle::TensorType_INT8:
    {
      assert(output->quantization() != nullptr);
      assert(output->quantization()->scale() != nullptr);
      assert(output->quantization()->scale()->size() == 1);
      const auto output_scale = output->quantization()->scale()->operator[](0);

      assert(output->quantization()->zero_point() != nullptr);
      assert(output->quantization()->zero_point()->size() == 1);
      const auto output_zp = output->quantization()->zero_point()->operator[](0);

      calculateActivationRangeQuantized(
        options->fused_activation_function(), output_zp, output_scale, output->type(),
        &params.quantized_activation_min, &params.quantized_activation_max);
      status =
        f_int8(params, input_shape, core::utils::castInputData<int8_t>(input_data),
               core::OMRuntimeShape(output), core::utils::castOutputData<int8_t>(output_data));
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
