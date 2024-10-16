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

#include "PALConv2d.h"

namespace luci_interpreter
{

namespace
{

#ifndef DIS_FLOAT

void evalFloat(const circle::Tensor *input, const circle::Tensor *filter,
               const circle::Tensor *bias, const circle::Tensor *output,
               const circle::Conv2DOptions *options, BaseRuntimeGraph *runtime_graph)
{
  const auto params = createConv2DParams(input, filter, output, options);

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

  luci_interpreter_pal::Conv(params, input_shape, kernels::getTensorData<float>(input_data),
                             filter_shape, kernels::getTensorData<float>(filter_data),
                             kernels::getTensorData<float>(bias_data), output_shape,
                             kernels::getTensorData<float>(output_data));
}

#endif // DIS_FLOAT

#ifndef DIS_QUANT

void evalQuantized(const circle::Tensor *input, const circle::Tensor *filter,
                   const circle::Tensor *bias, const circle::Tensor *output,
                   const circle::Conv2DOptions *options, BaseRuntimeGraph *runtime_graph)
{
  const auto params = createConv2DParams(input, filter, output, options);

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

  luci_interpreter_pal::Conv(params, input_shape, kernels::getTensorData<uint8_t>(input_data),
                             filter_shape, kernels::getTensorData<uint8_t>(filter_data),
                             kernels::getTensorData<int32_t>(bias_data), output_shape,
                             kernels::getTensorData<uint8_t>(output_data));
}

void evalQuantizedPerChannel(const circle::Tensor *input, const circle::Tensor *filter,
                             const circle::Tensor *bias, const circle::Tensor *output,
                             const circle::Conv2DOptions *options, BaseRuntimeGraph *runtime_graph,
                             DataType type)
{
  auto *raw_input_data = runtime_graph->getDataByTensor(input);
  auto *raw_output_data = runtime_graph->getDataByTensor(output);

  auto *raw_filter_data = runtime_graph->getConstDataByTensor(filter);
  auto *raw_bias_data = runtime_graph->getConstDataByTensor(bias);

  const auto params = createConv2DParams(input, filter, output, options);

  if (type == DataType::S8)
  {
    int32_t input_shape[kMaxSmallSize];
    kernels::getTensorDims(input, runtime_graph, input_shape);

    int32_t filter_shape[kMaxSmallSize];
    kernels::getTensorDims(filter, runtime_graph, filter_shape);

    int32_t output_shape[kMaxSmallSize];
    kernels::getTensorDims(output, runtime_graph, output_shape);

    luci_interpreter_pal::QuantizedConvPerChannel(
      params, input_shape, kernels::getTensorData<int8_t>(raw_input_data), filter_shape,
      kernels::getTensorData<int8_t>(raw_filter_data),
      kernels::getTensorData<int32_t>(raw_bias_data), output_shape,
      kernels::getTensorData<int8_t>(raw_output_data));

    return;
  }

  const int32_t batches = Tensor::dim(input, 0);
  const int32_t input_height = Tensor::dim(input, 1);
  const int32_t input_width = Tensor::dim(input, 2);
  const int32_t input_depth = Tensor::dim(input, 3);
  const int32_t output_depth = Tensor::dim(filter, 0);
  const int32_t filter_height = Tensor::dim(filter, 1);
  const int32_t filter_width = Tensor::dim(filter, 2);
  const int32_t output_height = Tensor::dim(output, 1);
  const int32_t output_width = Tensor::dim(output, 2);

  const int32_t padding_h = params.padding_values.height;
  const int32_t padding_w = params.padding_values.width;
  const int32_t stride_height = params.stride_height;
  const int32_t stride_width = params.stride_width;
  const int32_t dilation_height_factor = params.dilation_height_factor;
  const int32_t dilation_width_factor = params.dilation_width_factor;

  const int32_t activation_min = params.quantized_activation_min;
  const int32_t activation_max = params.quantized_activation_max;

  const std::vector<double> effective_output_scale = kernels::getQuantizedConvolutionMultiplers(
    Tensor::scale(input), Tensor::scales(filter), Tensor::scale(output));

  // Type U8

  const auto *input_data = kernels::getTensorData<uint8_t>(raw_input_data);
  assert(input_data != nullptr);
  const auto *filter_data = kernels::getTensorData<uint8_t>(raw_filter_data);
  assert(filter_data != nullptr);
  const auto *bias_data = kernels::getTensorData<int32_t>(raw_bias_data);
  assert(bias_data != nullptr);
  auto *output_data = kernels::getTensorData<uint8_t>(raw_output_data);
  assert(output_data != nullptr);

  const std::vector<kernels::ChannelQuantMultipliers> multipliers_raw =
    kernels::quantizeMultipliers(effective_output_scale);
  kernels::BroadcastableWrapper<kernels::ChannelQuantMultipliers> quant_multipliers(
    multipliers_raw);

  for (int32_t batch = 0; batch < batches; ++batch)
  {
    for (int32_t out_y = 0; out_y < output_height; ++out_y)
    {
      for (int32_t out_x = 0; out_x < output_width; ++out_x)
      {
        for (int32_t out_c = 0; out_c < output_depth; ++out_c)
        {
          const int32_t in_y_origin = out_y * stride_height - padding_h;
          const int32_t in_x_origin = out_x * stride_width - padding_w;
          int32_t acc = 0;
          for (int32_t filter_y = 0; filter_y < filter_height; ++filter_y)
          {
            for (int32_t filter_x = 0; filter_x < filter_width; ++filter_x)
            {
              const int32_t in_y = in_y_origin + dilation_height_factor * filter_y;
              const int32_t in_x = in_x_origin + dilation_width_factor * filter_x;
              if ((in_y >= 0 && in_y < input_height) && (in_x >= 0 && in_x < input_width))
              {
                for (int32_t in_c = 0; in_c < input_depth; ++in_c)
                {
                  const uint8_t input_val =
                    input_data[kernels::calcOffset(input, batch, in_y, in_x, in_c)];
                  const uint8_t filter_val =
                    filter_data[kernels::calcOffset(filter, out_c, filter_y, filter_x, in_c)];
                  acc += static_cast<int32_t>(input_val - Tensor::zero_point(input)) *
                         static_cast<int32_t>(filter_val - Tensor::zero_points(filter)[out_c]);
                }
              }
            }
          }
          if (bias_data)
          {
            acc += bias_data[out_c];
          }

          int32_t scaled_acc = luci_interpreter_pal::multiplyByQuantizedMultiplier(
            acc, quant_multipliers[out_c].multiplier, quant_multipliers[out_c].shift);

          scaled_acc += Tensor::zero_point(output);
          scaled_acc = std::max(scaled_acc, activation_min);
          scaled_acc = std::min(scaled_acc, activation_max);
          output_data[kernels::calcOffset(output, batch, out_y, out_x, out_c)] = scaled_acc;
        }
      }
    }
  }
}
#endif // DIS_QUANT

} // namespace

void configure_kernel_CircleConv2D(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::DownsamplingConv2DKernel kernel(cur_op, runtime_graph);

  const auto input = kernel.input();
  const auto filter = kernel.filter();
  const auto bias = kernel.bias();
  const auto output = kernel.output();

  auto filter_data = runtime_graph->getConstDataByTensor(filter);

  assert(filter_data != nullptr);

  const auto *options = cur_op->builtin_options_as_Conv2DOptions();

  if (Tensor::element_type(input) == DataType::FLOAT32 &&
      Tensor::element_type(filter) == DataType::FLOAT32)
  {
    LUCI_INTERPRETER_CHECK(bias == nullptr || Tensor::element_type(bias) == DataType::FLOAT32);
  }
#ifndef DIS_QUANT
  else if (Tensor::element_type(input) == DataType::U8 &&
           Tensor::element_type(filter) == DataType::U8)
  {
    LUCI_INTERPRETER_CHECK(bias == nullptr || Tensor::element_type(bias) == DataType::S32);
  }
  else if (Tensor::element_type(input) == DataType::S8 &&
           Tensor::element_type(filter) == DataType::S8)
  {
    LUCI_INTERPRETER_CHECK(bias == nullptr || Tensor::element_type(bias) == DataType::S32);
    LUCI_INTERPRETER_CHECK(Tensor::num_dims(filter) == 4);
    LUCI_INTERPRETER_CHECK(Tensor::scales(filter).size() ==
                           static_cast<size_t>(Tensor::dim(filter, 0)));
    for (auto zerop : Tensor::zero_points(filter))
    {
      LUCI_INTERPRETER_CHECK(zerop == 0);
    }
  }
  else if (Tensor::element_type(input) == DataType::S16 &&
           Tensor::element_type(filter) == DataType::S16)
  {
    LUCI_INTERPRETER_CHECK(bias == nullptr || Tensor::element_type(bias) == DataType::S64);
  }
#endif // DIS_QUANT
  else
  {
    assert(false && "Unsupported type.");
  }
  LUCI_INTERPRETER_CHECK(Tensor::element_type(output) == Tensor::element_type(input));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(input) == 4 && Tensor::num_dims(filter) == 4);

  const int32_t output_depth = Tensor::dim(filter, 0);
  LUCI_INTERPRETER_CHECK(Tensor::dim(filter, 3) == Tensor::dim(input, 3));

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

void execute_kernel_CircleConv2D(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::DownsamplingConv2DKernel kernel(cur_op, runtime_graph);

  const auto input = kernel.input();
  const auto weights = kernel.filter();
  const auto bias = kernel.bias();
  const auto output = kernel.output();

  const auto *options = cur_op->builtin_options_as_Conv2DOptions();

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
#ifndef DIS_QUANT
    case DataType::U8:
    case DataType::S8:
      if (Tensor::scales(weights).size() == 1 and type == DataType::U8)
      {
        evalQuantized(input, weights, bias, output, options, runtime_graph);
      }
      else if (Tensor::scales(weights).size() > 1)
      {
        LUCI_INTERPRETER_CHECK(Tensor::num_dims(weights) == 4);
        LUCI_INTERPRETER_CHECK(Tensor::scales(weights).size() ==
                               static_cast<size_t>(Tensor::dim(weights, 0)));
        evalQuantizedPerChannel(input, weights, bias, output, options, runtime_graph, type);
      }
      else
      {
        assert(false && "Unsupported yet.");
      }
      break;
#endif // DIS_QUANT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
