/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "PALFullyConnected.h"

namespace luci_interpreter
{

namespace
{
void evalFloat(const circle::Tensor *input, const circle::Tensor *weights,
               const circle::Tensor *bias, const circle::Tensor *output,
               const circle::FullyConnectedOptions *options, BaseRuntimeGraph *runtime_graph)
{
  float activation_min{};
  float activation_max{};
  kernels::calculateActivationRange(luci_actfunc(options->fused_activation_function()),
                                    &activation_min, &activation_max);

  luci_interpreter_pal::FullyConnectedParams params{};
  params.float_activation_min = activation_min;
  params.float_activation_max = activation_max;

  auto *input_data = runtime_graph->getDataByTensor(input);
  auto *output_data = runtime_graph->getDataByTensor(output);

  auto *weights_data = runtime_graph->getConstDataByTensor(weights);
  auto *bias_data = runtime_graph->getConstDataByTensor(bias);

  assert(input_data != nullptr);
  assert(weights_data != nullptr);
  assert(output_data != nullptr);

  int32_t input_shape[kMaxSmallSize];
  kernels::getTensorDims(input, runtime_graph, input_shape);

  int32_t weight_shape[kMaxSmallSize];
  kernels::getTensorDims(weights, runtime_graph, weight_shape);

  int32_t output_shape[kMaxSmallSize];
  kernels::getTensorDims(output, runtime_graph, output_shape);
  // TODO remove code duplication, introduce func
#ifndef DIS_DYN_SHAPES
  // Dynamic shape case
  if (output_shape[0] != input_shape[0] or output_shape[1] != weight_shape[1])
  {
    output_shape[0] = input_shape[0];
    output_shape[1] = weight_shape[0];
    uint32_t num_dims = Tensor::num_dims(output);
    luci_interpreter::RuntimeShape dynamic_shape(num_dims);
    int32_t data_size = 1;
    for (int i = 0; i < num_dims; ++i)
    {
      dynamic_shape.setDim(i, output_shape[i]);
      data_size *= output_shape[i];
    }
    data_size *= size(Tensor::element_type(output));

    runtime_graph->addDynamicShapeTensor(output, std::move(dynamic_shape));

    if (data_size == 0)
    {
      runtime_graph->resetTensorData(nullptr, output);
      return;
    }

    auto new_output_data = new uint8_t[data_size];
    output_data = new_output_data;
    runtime_graph->resetTensorData(new_output_data, output);
  }
#endif // DIS_DYN_SHAPES

  switch (Tensor::element_type(weights))
  {
    case DataType::FLOAT32:
    {
      luci_interpreter_pal::FullyConnected(
        params, input_shape, kernels::getTensorData<float>(input_data), weight_shape,
        kernels::getTensorData<float>(weights_data), kernels::getTensorData<float>(bias_data),
        output_shape, kernels::getTensorData<float>(output_data), Tensor::num_dims(output),
        Tensor::num_dims(weights));
      break;
    }
    case DataType::S8:
    {
      // Hybrid mode
      params.weights_scales =
        reinterpret_cast<const float *>(weights->quantization()->scale()->data());
      params.is_channel_wise_quant = weights->quantization()->scale()->size() > 1;
      luci_interpreter_pal::FullyConnected(
        params, input_shape, kernels::getTensorData<float>(input_data), weight_shape,
        kernels::getTensorData<int8_t>(weights_data), kernels::getTensorData<float>(bias_data),
        output_shape, kernels::getTensorData<float>(output_data), Tensor::num_dims(output),
        Tensor::num_dims(weights));
      break;
    }
    default:
      assert(false && "Unsupported hybrid weight type");
  }
}

#ifndef DIS_QUANT
void evalQuantized(const circle::Tensor *input, const circle::Tensor *weights,
                   const circle::Tensor *bias, const circle::Tensor *output,
                   const circle::FullyConnectedOptions *options, BaseRuntimeGraph *runtime_graph,
                   DataType type)
{
  double real_multiplier = 0.0;
  int output_shift;
  int32_t output_activation_min;
  int32_t output_activation_max;
  int32_t output_multiplier;
  real_multiplier = kernels::getQuantizedConvolutionMultipler(
    Tensor::scale(input), Tensor::scale(weights), Tensor::scale(output));
  kernels::quantizeMultiplier(real_multiplier, &output_multiplier, &output_shift);
  kernels::calculateActivationRangeQuantized(luci_actfunc(options->fused_activation_function()),
                                             output, &output_activation_min,
                                             &output_activation_max);

  int32_t input_offset = -Tensor::zero_point(input);
  int32_t filter_offset = 0;
  if (type == DataType::U8)
    filter_offset = -Tensor::zero_point(weights);
  int32_t output_offset = Tensor::zero_point(output);

  luci_interpreter_pal::FullyConnectedParams op_params{};
  op_params.input_offset = input_offset;
  op_params.weights_offset = filter_offset;
  op_params.output_offset = output_offset;
  op_params.output_multiplier = output_multiplier;
  op_params.output_shift = output_shift;
  op_params.quantized_activation_min = output_activation_min;
  op_params.quantized_activation_max = output_activation_max;
  op_params.lhs_cacheable = false;
  op_params.rhs_cacheable = false;

  auto *input_data = runtime_graph->getDataByTensor(input);
  auto *output_data = runtime_graph->getDataByTensor(output);

  auto *weights_data = runtime_graph->getConstDataByTensor(weights);
  auto *bias_data = runtime_graph->getConstDataByTensor(bias);

  assert(input_data != nullptr);
  assert(weights_data != nullptr);
  assert(output_data != nullptr);

  int32_t input_shape[kMaxSmallSize];
  kernels::getTensorDims(input, runtime_graph, input_shape);

  int32_t weights_shape[kMaxSmallSize];
  kernels::getTensorDims(weights, runtime_graph, weights_shape);

  int32_t output_shape[kMaxSmallSize];
  kernels::getTensorDims(output, runtime_graph, output_shape);
  if (type == DataType::S8)
  {
    luci_interpreter_pal::FullyConnected<int8_t>(
      op_params, input_shape, kernels::getTensorData<int8_t>(input_data), weights_shape,
      kernels::getTensorData<int8_t>(weights_data), kernels::getTensorData<int32_t>(bias_data),
      output_shape, kernels::getTensorData<int8_t>(output_data), Tensor::num_dims(output),
      Tensor::num_dims(weights));
  }
  else if (type == DataType::U8)
  {
    luci_interpreter_pal::FullyConnected<uint8_t>(
      op_params, input_shape, kernels::getTensorData<uint8_t>(input_data), weights_shape,
      kernels::getTensorData<uint8_t>(weights_data), kernels::getTensorData<int32_t>(bias_data),
      output_shape, kernels::getTensorData<uint8_t>(output_data), Tensor::num_dims(output),
      Tensor::num_dims(weights));
  }
  else if (type == DataType::S16)
  {
    luci_interpreter_pal::FullyConnected(
      op_params, input_shape, kernels::getTensorData<int16_t>(input_data), weights_shape,
      kernels::getTensorData<int8_t>(weights_data), kernels::getTensorData<int64_t>(bias_data),
      output_shape, kernels::getTensorData<int16_t>(output_data), Tensor::num_dims(output),
      Tensor::num_dims(weights));
  }
  else
  {
    assert(false && "Unsupported quantize type");
  }
}
#endif

} // namespace

void configure_kernel_CircleFullyConnected(const circle::Operator *cur_op,
                                           BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto weight_index = cur_op->inputs()->operator[](1);
  const auto bias_index = cur_op->inputs()->operator[](2);
  const auto output_index = cur_op->outputs()->operator[](0);

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

#ifndef DIS_FLOAT
  if (Tensor::element_type(weights) == DataType::S8 and
      Tensor::element_type(input) == DataType::FLOAT32)
  {
    // hybrid mode
    LUCI_INTERPRETER_CHECK(Tensor::element_type(output) == DataType::FLOAT32);
    LUCI_INTERPRETER_CHECK(!bias || Tensor::element_type(bias) == DataType::FLOAT32)
  }
  else if (Tensor::element_type(weights) == DataType::FLOAT32)
  {
    LUCI_INTERPRETER_CHECK(Tensor::element_type(input) == DataType::FLOAT32);
    LUCI_INTERPRETER_CHECK(Tensor::element_type(output) == DataType::FLOAT32);
    LUCI_INTERPRETER_CHECK(!bias || Tensor::element_type(bias) == DataType::FLOAT32)
  }
#endif // DIS_FLOAT
#ifndef DIS_QUANT
  else if (Tensor::element_type(weights) == DataType::U8)
  {
    LUCI_INTERPRETER_CHECK(Tensor::element_type(input) == DataType::U8);
    LUCI_INTERPRETER_CHECK(Tensor::element_type(output) == DataType::U8);
    LUCI_INTERPRETER_CHECK(!bias || Tensor::element_type(bias) == DataType::S32)
  }
  else if (Tensor::element_type(weights) == DataType::S8)
  {
    LUCI_INTERPRETER_CHECK(Tensor::element_type(input) == DataType::S8 ||
                           Tensor::element_type(input) == DataType::FLOAT32);
    LUCI_INTERPRETER_CHECK(Tensor::element_type(output) == DataType::S8 ||
                           Tensor::element_type(output) == DataType::FLOAT32);
    LUCI_INTERPRETER_CHECK(!bias || Tensor::element_type(bias) == DataType::S32 ||
                           Tensor::element_type(bias) == DataType::S64 ||
                           Tensor::element_type(bias) == DataType::FLOAT32)
    if (Tensor::element_type(input) == DataType::FLOAT32)
    {
      // Check it is channel wise quantization
      LUCI_INTERPRETER_CHECK(weights->quantization() != nullptr);
      LUCI_INTERPRETER_CHECK(weights->quantization()->scale() != nullptr);
    }
  }
#endif // DIS_QUANT
  else
  {
    assert(false && "Unsupported type.");
  }

  LUCI_INTERPRETER_CHECK(Tensor::num_dims(weights) == 2);
  LUCI_INTERPRETER_CHECK(bias == nullptr || Tensor::num_elements(bias) == Tensor::dim(weights, 0));

#ifdef DIS_DYN_SHAPES
  int32_t input_num_elements = Tensor::num_elements(input);
  LUCI_INTERPRETER_CHECK(input_num_elements % Tensor::dim(weights, 1) == 0);
#endif // DIS_DYN_SHAPES

  if (bias)
    LUCI_INTERPRETER_CHECK(Tensor::num_elements(bias) == Tensor::dim(weights, 0));
}

// TODO think how remove unused param
void execute_kernel_CircleFullyConnected(const circle::Operator *cur_op,
                                         BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto weight_index = cur_op->inputs()->operator[](1);
  const auto bias_index = cur_op->inputs()->operator[](2);
  const auto output_index = cur_op->outputs()->operator[](0);

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

  const auto *options = cur_op->builtin_options_as_FullyConnectedOptions();
  const auto input_type = Tensor::element_type(input);
  switch (input_type)
  {
#ifndef DIS_QUANT
    case DataType::U8:
    case DataType::S8:
    case DataType::S16:
      evalQuantized(input, weights, bias, output, options, runtime_graph, input_type);
      break;
#endif // DIS_QUANT
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      evalFloat(input, weights, bias, output, options, runtime_graph);
      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
