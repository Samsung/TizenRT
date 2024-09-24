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

#include "ConvolutionCommon.h"
#include "kernels/Utils.h"

#include "PALDepthwiseConv2D.h"

namespace luci_interpreter
{

namespace
{

#ifndef DIS_FLOAT

void evalFloat(const circle::Tensor *input, const circle::Tensor *filter,
               const circle::Tensor *bias, const circle::Tensor *output,
               const circle::DepthwiseConv2DOptions *options, BaseRuntimeGraph *runtime_graph)
{
  float activation_min{};
  float activation_max{};
  kernels::calculateActivationRange(luci_actfunc(options->fused_activation_function()),
                                    &activation_min, &activation_max);

  luci_interpreter_pal::ConvParams params{};
  params.padding_values.height = computeConvPadding(
    input, filter, options->padding(), options->stride_h(), options->dilation_h_factor(), 1);
  params.padding_values.width = computeConvPadding(
    input, filter, options->padding(), options->stride_w(), options->dilation_w_factor(), 2);
  params.stride_height = options->stride_h();
  params.stride_width = options->stride_w();
  params.dilation_height_factor = options->dilation_h_factor();
  params.dilation_width_factor = options->dilation_w_factor();
  params.float_activation_min = activation_min;
  params.float_activation_max = activation_max;
  params.depth_multiplier = options->depth_multiplier();

  auto *input_data = runtime_graph->getDataByTensor(input);
  auto *output_data = runtime_graph->getDataByTensor(output);

  auto *filter_data = runtime_graph->getConstDataByTensor(filter);
  auto *bias_data = runtime_graph->getConstDataByTensor(bias);

  int32_t input_shape[kMaxSmallSize];
  kernels::getTensorDims(input, runtime_graph, input_shape);

  int32_t filter_shape[kMaxSmallSize];
  kernels::getTensorDims(filter, runtime_graph, filter_shape);

  int32_t output_shape[kMaxSmallSize];
  kernels::getTensorDims(output, runtime_graph, output_shape);

  luci_interpreter_pal::DepthwiseConv2D(
    params, input_shape, kernels::getTensorData<float>(input_data), filter_shape,
    kernels::getTensorData<float>(filter_data), kernels::getTensorData<float>(bias_data),
    output_shape, kernels::getTensorData<float>(output_data));
}

#endif // DIS_FLOAT

} // namespace

void configure_kernel_CircleDepthwiseConv2D(const circle::Operator *cur_op,
                                            BaseRuntimeGraph *runtime_graph)
{
  kernels::DownsamplingConv2DKernel kernel(cur_op, runtime_graph);

  const auto input = kernel.input();
  const auto filter = kernel.filter();
  const auto bias = kernel.bias();
  const auto output = kernel.output();

  auto filter_data = runtime_graph->getConstDataByTensor(filter);

  assert(filter_data != nullptr);

  const auto *options = cur_op->builtin_options_as_DepthwiseConv2DOptions();

  if (Tensor::element_type(input) == DataType::FLOAT32 &&
      Tensor::element_type(filter) == DataType::FLOAT32)
  {
    LUCI_INTERPRETER_CHECK(bias == nullptr || Tensor::element_type(bias) == DataType::FLOAT32);
  }
  else
  {
    assert(false && "Unsupported type.");
  }
  LUCI_INTERPRETER_CHECK(Tensor::element_type(output) == Tensor::element_type(input));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(input) == 4 && Tensor::num_dims(filter) == 4);

  const int32_t output_depth = Tensor::dim(output, 3);
  LUCI_INTERPRETER_CHECK(bias == nullptr ||
                         (Tensor::num_dims(bias) == 1 && Tensor::dim(bias, 0) == output_depth));

  switch (options->fused_activation_function())
  {
    case circle::ActivationFunctionType_NONE:
    case circle::ActivationFunctionType_RELU:
    case circle::ActivationFunctionType_RELU6:
    case circle::ActivationFunctionType_RELU_N1_TO_1:
      break;
    default:
      assert(false && "Unsupported fused activation");
  }
}

void execute_kernel_CircleDepthwiseConv2D(const circle::Operator *cur_op,
                                          BaseRuntimeGraph *runtime_graph)
{
  kernels::DownsamplingConv2DKernel kernel(cur_op, runtime_graph);

  const auto input = kernel.input();
  const auto weights = kernel.filter();
  const auto bias = kernel.bias();
  const auto output = kernel.output();

  const auto *options = cur_op->builtin_options_as_DepthwiseConv2DOptions();

  switch (Tensor::element_type(input))
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
