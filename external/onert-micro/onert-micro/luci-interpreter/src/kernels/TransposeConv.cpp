/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2019 The TensorFlow Authors. All Rights Reserved.
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

#include "Builders.h"
#include "kernels/Utils.h"

#include "PALTransposeConv.h"

namespace luci_interpreter
{

namespace
{

// For the TfLite transpose_conv implementation, input tensor 0 corresponds to
// the OutputShapeTensor. However, since TFLM does not support dynamic tensors,
// the TFLM implementation ignores input tensor 0 and the only inputs we care
// about are kFilterTensor, kInputTensor and kBiasTensor.
constexpr int kFilterTensor = 1;
constexpr int kInputTensor = 2;
constexpr int kBiasTensor = 3;
constexpr int kOutputTensor = 0;

// TODO: reduce code duplication with Conv
int32_t compute_padding_h(const circle::Tensor *input, const circle::Tensor *filter,
                          const circle::TransposeConvOptions *options)
{
  // Note: Dilation height and width are always 1 for transpose_conv
  const int32_t input_height = Tensor::dim(input, 1);
  const int32_t filter_height = Tensor::dim(filter, 1);
  const int32_t output_height = kernels::computeOutputSize(
    luci_padding(options->padding()), input_height, filter_height, options->stride_h(), 1);

  const auto padding_height =
    kernels::computePadding(options->stride_h(), 1, input_height, filter_height, output_height);
  return padding_height;
}

int32_t compute_padding_w(const circle::Tensor *input, const circle::Tensor *filter,
                          const circle::TransposeConvOptions *options)
{
  // Note: Dilation height and width are always 1 for transpose_conv
  const int32_t input_width = Tensor::dim(input, 2);
  const int32_t filter_width = Tensor::dim(filter, 2);
  const int32_t output_width = kernels::computeOutputSize(
    luci_padding(options->padding()), input_width, filter_width, options->stride_w(), 1);

  const auto padding_width =
    kernels::computePadding(options->stride_w(), 1, input_width, filter_width, output_width);

  return padding_width;
}

#ifndef DIS_FLOAT

void evalFloat(const circle::Tensor *input, const circle::Tensor *filter,
               const circle::Tensor *bias, const circle::Tensor *output,
               const circle::TransposeConvOptions *options, BaseRuntimeGraph *runtime_graph)
{
  float activation_min{};
  float activation_max{};
  kernels::calculateActivationRange(FusedActFunc::NONE, &activation_min, &activation_max);

  luci_interpreter_pal::ConvParams params{};
  params.padding_values.height = compute_padding_h(input, filter, options);
  params.padding_values.width = compute_padding_w(input, filter, options);
  params.stride_height = options->stride_h();
  params.stride_width = options->stride_w();
  params.dilation_height_factor = 1;
  params.dilation_width_factor = 1;
  params.float_activation_min = activation_min;
  params.float_activation_max = activation_max;

  auto *input_data = runtime_graph->getDataByTensor(input);
  auto *output_data = runtime_graph->getDataByTensor(output);

  auto *filter_data = runtime_graph->getConstDataByTensor(filter);
  auto *bias_data = runtime_graph->getConstDataByTensor(bias);

  luci_interpreter_pal::TransposeConv(
    params, kernels::getTensorRuntimeShape(input, runtime_graph),
    kernels::getTensorData<float>(input_data),
    kernels::getTensorRuntimeShape(filter, runtime_graph),
    kernels::getTensorData<float>(filter_data), kernels::getTensorRuntimeShape(bias, runtime_graph),
    kernels::getTensorData<float>(bias_data), kernels::getTensorRuntimeShape(output, runtime_graph),
    kernels::getTensorData<float>(output_data));
}

#endif // DIS_FLOAT

} // namespace

void configure_kernel_CircleTransposeConv(const circle::Operator *cur_op,
                                          BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](kInputTensor);
  const auto filter_index = cur_op->inputs()->operator[](kFilterTensor);
  const auto output_index = cur_op->outputs()->operator[](kOutputTensor);

  assert(input_index != -1);
  assert(filter_index != -1);
  assert(output_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto filter = runtime_graph->getCircleTensorByIndex(filter_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  assert(input != nullptr);
  assert(filter != nullptr);

  auto filter_data = runtime_graph->getConstDataByTensor(filter);

  assert(filter_data != nullptr);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(output) == Tensor::element_type(input));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(filter) == Tensor::element_type(input));

  LUCI_INTERPRETER_CHECK(Tensor::num_dims(input) == 4 && Tensor::num_dims(filter) == 4);
}

void execute_kernel_CircleTransposeConv(const circle::Operator *cur_op,
                                        BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](kInputTensor);
  const auto weight_index = cur_op->inputs()->operator[](kFilterTensor);
  const auto bias_index =
    cur_op->inputs()->size() == 4 ? cur_op->inputs()->operator[](kBiasTensor) : -1;
  const auto output_index = cur_op->outputs()->operator[](kOutputTensor);

  assert(input_index != -1);
  assert(weight_index != -1);
  assert(output_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto weights = runtime_graph->getCircleTensorByIndex(weight_index);
  const auto bias = runtime_graph->getCircleTensorByIndex(bias_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  assert(input != nullptr);
  assert(weights != nullptr);
  assert(output != nullptr);

  const auto *options = cur_op->builtin_options_as_TransposeConvOptions();

  const auto type = Tensor::element_type(input);
  switch (type)
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      if (Tensor::element_type(weights) == DataType::FLOAT32)
      {
        evalFloat(input, weights, bias, output, options, runtime_graph);
        break;
      }
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
