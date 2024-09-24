/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_KERNELS_UNIDIRECTIONAL_SEQUENCE_LSTM_H
#define LUCI_INTERPRETER_KERNELS_UNIDIRECTIONAL_SEQUENCE_LSTM_H

#include "PALUtils.h"

namespace luci_interpreter
{
namespace lstm
{

struct LSTMStruct
{
  LSTMStruct() = delete;
  LSTMStruct(const LSTMStruct &) = delete;

  explicit LSTMStruct(const circle::Operator *cur_op,
                      luci_interpreter::BaseRuntimeGraph *runtime_graph)
  {
    const auto input_index = cur_op->inputs()->operator[](0);
    const auto input_to_input_weights_index = cur_op->inputs()->operator[](1);
    const auto input_to_forget_weights_index = cur_op->inputs()->operator[](2);
    const auto input_to_cell_weights_index = cur_op->inputs()->operator[](3);
    const auto input_to_output_weights_index = cur_op->inputs()->operator[](4);
    assert(input_index != -1);
    // input_to_input_weights_index - optional
    assert(input_to_forget_weights_index != -1);
    assert(input_to_cell_weights_index != -1);
    assert(input_to_output_weights_index != -1);
    internal_tensors[0] = runtime_graph->getCircleTensorByIndex(input_index);
    internal_tensors[1] = runtime_graph->getCircleTensorByIndex(input_to_input_weights_index);
    internal_tensors[2] = runtime_graph->getCircleTensorByIndex(input_to_forget_weights_index);
    internal_tensors[3] = runtime_graph->getCircleTensorByIndex(input_to_cell_weights_index);
    internal_tensors[4] = runtime_graph->getCircleTensorByIndex(input_to_output_weights_index);

    const auto recurrent_to_input_weights_index = cur_op->inputs()->operator[](5);
    const auto recurrent_to_forget_weights_index = cur_op->inputs()->operator[](6);
    const auto recurrent_to_cell_weights_index = cur_op->inputs()->operator[](7);
    const auto recurrent_to_output_weights_index = cur_op->inputs()->operator[](8);
    // recurrent_to_input_weights_index - optional
    assert(recurrent_to_forget_weights_index != -1);
    assert(recurrent_to_cell_weights_index != -1);
    assert(recurrent_to_output_weights_index != -1);
    internal_tensors[5] = runtime_graph->getCircleTensorByIndex(recurrent_to_input_weights_index);
    internal_tensors[6] = runtime_graph->getCircleTensorByIndex(recurrent_to_forget_weights_index);
    internal_tensors[7] = runtime_graph->getCircleTensorByIndex(recurrent_to_cell_weights_index);
    internal_tensors[8] = runtime_graph->getCircleTensorByIndex(recurrent_to_output_weights_index);

    const auto cell_to_input_weights_index = cur_op->inputs()->operator[](9);
    const auto cell_to_forget_weights_index = cur_op->inputs()->operator[](10);
    const auto cell_to_output_weights_index = cur_op->inputs()->operator[](11);
    // optional cell_to_input_weights_index
    // optional cell_to_forget_weights_index
    // optional cell_to_output_weights_index
    internal_tensors[9] = runtime_graph->getCircleTensorByIndex(cell_to_input_weights_index);
    internal_tensors[10] = runtime_graph->getCircleTensorByIndex(cell_to_forget_weights_index);
    internal_tensors[11] = runtime_graph->getCircleTensorByIndex(cell_to_output_weights_index);

    const auto input_gate_bias_index = cur_op->inputs()->operator[](12);
    const auto forget_gate_bias_index = cur_op->inputs()->operator[](13);
    const auto cell_gate_bias_index = cur_op->inputs()->operator[](14);
    const auto output_gate_bias_index = cur_op->inputs()->operator[](15);
    // optional input_gate_bias_index
    assert(forget_gate_bias_index != -1);
    assert(cell_gate_bias_index != -1);
    assert(output_gate_bias_index != -1);
    internal_tensors[12] = runtime_graph->getCircleTensorByIndex(input_gate_bias_index);
    internal_tensors[13] = runtime_graph->getCircleTensorByIndex(forget_gate_bias_index);
    internal_tensors[14] = runtime_graph->getCircleTensorByIndex(cell_gate_bias_index);
    internal_tensors[15] = runtime_graph->getCircleTensorByIndex(output_gate_bias_index);

    const auto projection_weights_index = cur_op->inputs()->operator[](16);
    const auto projection_bias_index = cur_op->inputs()->operator[](17);
    // optional projection_weights_index
    // optional projection_bias_index
    internal_tensors[16] = runtime_graph->getCircleTensorByIndex(projection_weights_index);
    internal_tensors[17] = runtime_graph->getCircleTensorByIndex(projection_bias_index);

    const auto output_state_index = cur_op->inputs()->operator[](18);
    const auto cell_state_index = cur_op->inputs()->operator[](19);
    assert(output_state_index != -1);
    assert(cell_state_index != -1);
    internal_tensors[18] = runtime_graph->getCircleTensorByIndex(output_state_index);
    internal_tensors[19] = runtime_graph->getCircleTensorByIndex(cell_state_index);

    const auto input_layer_norm_coefficients_index = cur_op->inputs()->operator[](20);
    const auto forget_layer_norm_coefficients_index = cur_op->inputs()->operator[](21);
    const auto cell_layer_norm_coefficients_index = cur_op->inputs()->operator[](22);
    const auto output_layer_norm_coefficients_index = cur_op->inputs()->operator[](23);
    // optional input_layer_norm_coefficients_index
    // optional forget_layer_norm_coefficients_index
    // optional cell_layer_norm_coefficients_index
    // optional output_layer_norm_coefficients_index
    internal_tensors[20] =
      runtime_graph->getCircleTensorByIndex(input_layer_norm_coefficients_index);
    internal_tensors[21] =
      runtime_graph->getCircleTensorByIndex(forget_layer_norm_coefficients_index);
    internal_tensors[22] =
      runtime_graph->getCircleTensorByIndex(cell_layer_norm_coefficients_index);
    internal_tensors[23] =
      runtime_graph->getCircleTensorByIndex(output_layer_norm_coefficients_index);

    const auto output_index = cur_op->outputs()->operator[](0);
    assert(output_index != -1);
    output_internal = runtime_graph->getCircleTensorByIndex(output_index);

    options = cur_op->builtin_options_as_UnidirectionalSequenceLSTMOptions();
  }

  void validateTensorTypes()
  {
    LUCI_INTERPRETER_CHECK(Tensor::element_type(input()) == Tensor::element_type(output_state()));
    LUCI_INTERPRETER_CHECK(Tensor::element_type(output()) == Tensor::element_type(input()));

    for (int32_t i = 1; i < 9; ++i)
    {
      LUCI_INTERPRETER_CHECK(internal_tensors[i] == nullptr or
                             Tensor::element_type(input_to_forget_weights()) ==
                               Tensor::element_type(internal_tensors[i]));
    }

    for (int32_t i = 12; i < 16; ++i)
    {
      LUCI_INTERPRETER_CHECK(internal_tensors[i] == nullptr or
                             Tensor::element_type(forget_gate_bias()) ==
                               Tensor::element_type(internal_tensors[i]));
    }
  }

  const circle::Tensor *input() { return internal_tensors[0]; };

  const circle::Tensor *input_to_input_weights() { return internal_tensors[1]; };
  const circle::Tensor *input_to_forget_weights() { return internal_tensors[2]; };
  const circle::Tensor *input_to_cell_weights() { return internal_tensors[3]; };
  const circle::Tensor *input_to_output_weights() { return internal_tensors[4]; };

  const circle::Tensor *recurrent_to_input_weights() { return internal_tensors[5]; };
  const circle::Tensor *recurrent_to_forget_weights() { return internal_tensors[6]; };
  const circle::Tensor *recurrent_to_cell_weights() { return internal_tensors[7]; };
  const circle::Tensor *recurrent_to_output_weights() { return internal_tensors[8]; };

  const circle::Tensor *cell_to_input_weights() { return internal_tensors[9]; };
  const circle::Tensor *cell_to_forget_weights() { return internal_tensors[10]; };
  const circle::Tensor *cell_to_output_weights() { return internal_tensors[11]; };

  const circle::Tensor *input_gate_bias() { return internal_tensors[12]; };
  const circle::Tensor *forget_gate_bias() { return internal_tensors[13]; };
  const circle::Tensor *cell_gate_bias() { return internal_tensors[14]; };
  const circle::Tensor *output_gate_bias() { return internal_tensors[15]; };

  const circle::Tensor *projection_weights() { return internal_tensors[16]; };
  const circle::Tensor *projection_bias() { return internal_tensors[17]; };

  const circle::Tensor *output_state() { return internal_tensors[18]; };
  const circle::Tensor *cell_state() { return internal_tensors[19]; };

  const circle::Tensor *input_layer_norm_coefficients() { return internal_tensors[20]; };
  const circle::Tensor *forget_layer_norm_coefficients() { return internal_tensors[21]; };
  const circle::Tensor *cell_layer_norm_coefficients() { return internal_tensors[22]; };
  const circle::Tensor *output_layer_norm_coefficients() { return internal_tensors[23]; };
  const circle::Tensor *output() { return output_internal; };

  const circle::UnidirectionalSequenceLSTMOptions *options;

  const circle::Tensor *get_internal_tensor(int i) { return internal_tensors[i]; }

private:
  const circle::Tensor *output_internal;
  const circle::Tensor *internal_tensors[24];
};

struct GateParameters
{
  luci_interpreter_pal::FullyConnectedParams input_fc_params;
  luci_interpreter_pal::FullyConnectedParams recurrent_fc_params;
};

struct InterGateParameters
{
  luci_interpreter_pal::ArithmeticParams forget_cell_mul_params;
  luci_interpreter_pal::ArithmeticParams input_mul_params;
  luci_interpreter_pal::ArithmeticParams output_mul_params;
};

struct CellStateInfo
{
  float cell_clip;
  // clipping range for cell state only 16 bits cell is supported (could be
  // generalized through templatation)
  int16_t quantized_cell_clip;
  // 2^-cell_state_scale_power = cell state scale, required by integer tanh
  // computation
  int32_t cell_state_scale_power;
};

struct LSTMParameters
{
  GateParameters forget_gate_parameters;
  GateParameters input_gate_parameters;
  GateParameters cell_gate_parameters;
  GateParameters output_gate_parameters;
  InterGateParameters inter_gate_parameters;
};

} // namespace lstm
} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNELS_UNIDIRECTIONAL_SEQUENCE_LSTM_H
