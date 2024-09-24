/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2017 The TensorFlow Authors. All Rights Reserved.
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

#include "PALSVDF.h"

namespace luci_interpreter
{

namespace
{
const int kSvdfInputTensor = 0;
const int kSvdfWeightsFeatureTensor = 1;
const int kSvdfWeightsTimeTensor = 2;
const int kSvdfBiasTensor = 3;
const int kSvdfInputActivationStateTensor =
  4; // This is a variable tensor, and will be modified by this op.
const int kSvdfOutputTensor = 0;
} // namespace

void configure_kernel_CircleSVDF(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  // Validate Tensor Inputs (dtype depends on quantization):
  // [0] = Input, {2, batch_size, input_size}
  // [1] = Weights Feature, {2, num_filters, input_size}
  // [2] = Weights Time, {2, num_filters, memory_size}
  // [3] = Bias (optional), {1, num_units}
  // [4] = Activation State (variable),
  //         {2, batch_size, memory_size * num_filters}
  const auto input_index = cur_op->inputs()->operator[](kSvdfInputTensor);
  const auto weights_feature_index = cur_op->inputs()->operator[](kSvdfWeightsFeatureTensor);
  const auto weights_time_index = cur_op->inputs()->operator[](kSvdfWeightsTimeTensor);
  const auto bias_index = cur_op->inputs()->operator[](kSvdfBiasTensor);
  const auto activation_state_index = cur_op->inputs()->operator[](kSvdfInputActivationStateTensor);
  const auto output_index = cur_op->outputs()->operator[](kSvdfOutputTensor);

  assert(input_index != -1);
  assert(weights_feature_index != -1);
  assert(weights_time_index != -1);
  assert(activation_state_index != -1);
  assert(output_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto weights_feature = runtime_graph->getCircleTensorByIndex(weights_feature_index);
  const auto weights_time = runtime_graph->getCircleTensorByIndex(weights_time_index);
  const auto bias = runtime_graph->getCircleTensorByIndex(bias_index);
  const auto activation_state = runtime_graph->getCircleTensorByIndex(activation_state_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  assert(input != nullptr);
  assert(weights_feature != nullptr);
  assert(weights_time != nullptr);
  assert(activation_state != nullptr);
  assert(output != nullptr);

  const auto *options = cur_op->builtin_options_as_SVDFOptions();

  // Define input constants based on input tensor definition above:
  const int rank = options->rank();
  const int input_size = Tensor::dim(input, 1);
  const int batch_size = Tensor::dim(input, 0);
  const int num_filters = Tensor::dim(weights_feature, 0);
  LUCI_INTERPRETER_CHECK(num_filters % rank == 0);

  const int num_units = num_filters / rank;
  const int memory_size = Tensor::dim(weights_time, 1);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(input) == DataType::FLOAT32 or
                         Tensor::element_type(input) == DataType::S8);
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(input) == 2);

  // Validate Tensor Output:
  // [0] = float/int8_t, {2, batch_size, num_units}
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(output) == 2);
  LUCI_INTERPRETER_CHECK(Tensor::dim(output, 0) == batch_size);
  LUCI_INTERPRETER_CHECK(Tensor::dim(output, 1) == num_units);

  // Validate Weights Feature Input Tensor
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(weights_feature) == 2);
  LUCI_INTERPRETER_CHECK(Tensor::dim(weights_feature, 1) == input_size);

  // Validate Weights Time Input Tensor:
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(weights_time) == 2);
  LUCI_INTERPRETER_CHECK(Tensor::dim(weights_time, 0) == num_filters);
  LUCI_INTERPRETER_CHECK(Tensor::dim(weights_time, 1) == memory_size);

  // Validate Optional Bias Input Tensor:
  if (bias != nullptr)
  {
    LUCI_INTERPRETER_CHECK(Tensor::dim(bias, 0) == num_units);
  }

  // Validate Activation State Input Tensor:
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(activation_state) == 2);
  LUCI_INTERPRETER_CHECK(Tensor::dim(activation_state, 0) == batch_size);
  LUCI_INTERPRETER_CHECK(Tensor::dim(activation_state, 1) == memory_size * num_filters);

  if (Tensor::element_type(input) == DataType::FLOAT32)
  {
    LUCI_INTERPRETER_CHECK(Tensor::element_type(weights_feature) == DataType::FLOAT32);
    LUCI_INTERPRETER_CHECK(Tensor::element_type(weights_time) == DataType::FLOAT32);
    LUCI_INTERPRETER_CHECK(Tensor::element_type(activation_state) == DataType::FLOAT32);
    if (bias)
      LUCI_INTERPRETER_CHECK(Tensor::element_type(bias) == DataType::FLOAT32);
    LUCI_INTERPRETER_CHECK(Tensor::element_type(output) == DataType::FLOAT32);
  }
}

void execute_kernel_CircleSVDF(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](kSvdfInputTensor);
  const auto weights_feature_index = cur_op->inputs()->operator[](kSvdfWeightsFeatureTensor);
  const auto weights_time_index = cur_op->inputs()->operator[](kSvdfWeightsTimeTensor);
  const auto bias_index = cur_op->inputs()->operator[](kSvdfBiasTensor);
  const auto activation_state_index = cur_op->inputs()->operator[](kSvdfInputActivationStateTensor);
  const auto output_index = cur_op->outputs()->operator[](kSvdfOutputTensor);

  assert(input_index != -1);
  assert(weights_feature_index != -1);
  assert(weights_time_index != -1);
  assert(activation_state_index != -1);
  assert(output_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto weights_feature = runtime_graph->getCircleTensorByIndex(weights_feature_index);
  const auto weights_time = runtime_graph->getCircleTensorByIndex(weights_time_index);
  const auto bias = runtime_graph->getCircleTensorByIndex(bias_index);
  const auto activation_state = runtime_graph->getCircleTensorByIndex(activation_state_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  assert(input != nullptr);
  assert(weights_feature != nullptr);
  assert(weights_time != nullptr);
  assert(activation_state != nullptr);
  assert(output != nullptr);

  const auto *options = cur_op->builtin_options_as_SVDFOptions();

  // Define input constants based on input tensor definition above:
  const int rank = options->rank();
  const int input_size = Tensor::dim(input, 1);
  const int batch_size = Tensor::dim(input, 0);
  const int num_filters = Tensor::dim(weights_feature, 0);
  LUCI_INTERPRETER_CHECK(num_filters % rank == 0);

  const int num_units = num_filters / rank;
  const int memory_size = Tensor::dim(weights_time, 1);

  const uint8_t *input_data = runtime_graph->getDataByTensor(input);
  const uint8_t *weights_feature_data = runtime_graph->getConstDataByTensor(weights_feature);
  const uint8_t *weights_time_data = runtime_graph->getConstDataByTensor(weights_time);
  const uint8_t *bias_data = runtime_graph->getConstDataByTensor(bias);
  uint8_t *output_data = runtime_graph->getDataByTensor(output);

  const auto type = Tensor::element_type(input);
  switch (type)
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      // Create and fill with 0 state tensor
      auto state_data = std::make_unique<float[]>(Tensor::num_elements(activation_state));
      std::fill_n(state_data.get(), Tensor::num_elements(activation_state), 0);

      auto scratch_data = std::make_unique<uint8_t[]>(batch_size * num_filters * sizeof(float));

      luci_interpreter_pal::SVDF(
        kernels::getTensorData<float>(input_data),
        kernels::getTensorData<float>(weights_feature_data),
        kernels::getTensorData<float>(weights_time_data), kernels::getTensorData<float>(bias_data),
        state_data.get(), kernels::getTensorData<float>(scratch_data.get()),
        kernels::getTensorData<float>(output_data), rank, input_size, batch_size, num_filters,
        num_units, memory_size, options->fused_activation_function());
    }
    break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
