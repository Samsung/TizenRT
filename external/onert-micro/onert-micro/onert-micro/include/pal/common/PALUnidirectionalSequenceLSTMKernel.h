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

#ifndef ONERT_MICRO_UNIDIRECTIONAL_SEQUENCE_LSTM_KERNEL_H
#define ONERT_MICRO_UNIDIRECTIONAL_SEQUENCE_LSTM_KERNEL_H

#include "PALUtils.h"
#include "core/OMKernelData.h"

namespace onert_micro
{
namespace execute
{
namespace lstm
{

struct FullyConnectedParams
{
  int32_t input_offset;
  int32_t weights_offset;
  int32_t output_offset;
  int32_t output_multiplier;
  int output_shift;
  // uint8_t, etc, activation params.
  int32_t quantized_activation_min;
  int32_t quantized_activation_max;
  // float activation params.
  float float_activation_min;
  float float_activation_max;
};

// For Add, Sub, Mul ops.
struct ArithmeticParams
{
  // uint8_t inference params.
  int32_t input1_offset;
  int32_t input2_offset;
  int32_t output_offset;
  int32_t output_multiplier;
  int output_shift;
  // Add / Sub, not Mul, uint8_t inference params.
  int left_shift;
  int32_t input1_multiplier;
  int input1_shift;
  int32_t input2_multiplier;
  int input2_shift;
  // uint8_t, etc, activation params.
  int32_t quantized_activation_min;
  int32_t quantized_activation_max;
  // float activation params.
  float float_activation_min;
  float float_activation_max;
  // int64_t activation params.
  int64_t int64_activation_min;
  int64_t int64_activation_max;
};

struct LSTMStruct
{
  LSTMStruct() = default;
  LSTMStruct(const LSTMStruct &) = delete;

  OMStatus readKernel(uint16_t op_index, core::OMRuntimeStorage &storage,
                      core::OMRuntimeContext &runtime_context)
  {
    // TODO: add checks to return not Ok
    // const std::vector<uint16_t> &kernel_operators = kernel.getKernelOperators();

    // const uint16_t operator_index = kernel_operators.front();

    const circle::Operator *cur_op = runtime_context.getCircleOperatorAt(op_index);

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
    internal_tensors[0] = runtime_context.getTensorByIndex(input_index);
    inputs_index[0] = input_index;
    internal_tensors[1] = runtime_context.getTensorByIndex(input_to_input_weights_index);
    inputs_index[1] = input_to_input_weights_index;
    internal_tensors[2] = runtime_context.getTensorByIndex(input_to_forget_weights_index);
    inputs_index[2] = input_to_forget_weights_index;
    internal_tensors[3] = runtime_context.getTensorByIndex(input_to_cell_weights_index);
    inputs_index[3] = input_to_cell_weights_index;
    internal_tensors[4] = runtime_context.getTensorByIndex(input_to_output_weights_index);
    inputs_index[4] = input_to_output_weights_index;

    const auto recurrent_to_input_weights_index = cur_op->inputs()->operator[](5);
    const auto recurrent_to_forget_weights_index = cur_op->inputs()->operator[](6);
    const auto recurrent_to_cell_weights_index = cur_op->inputs()->operator[](7);
    const auto recurrent_to_output_weights_index = cur_op->inputs()->operator[](8);
    // recurrent_to_input_weights_index - optional
    assert(recurrent_to_forget_weights_index != -1);
    assert(recurrent_to_cell_weights_index != -1);
    assert(recurrent_to_output_weights_index != -1);
    internal_tensors[5] = runtime_context.getTensorByIndex(recurrent_to_input_weights_index);
    inputs_index[5] = recurrent_to_input_weights_index;
    internal_tensors[6] = runtime_context.getTensorByIndex(recurrent_to_forget_weights_index);
    inputs_index[6] = recurrent_to_forget_weights_index;
    internal_tensors[7] = runtime_context.getTensorByIndex(recurrent_to_cell_weights_index);
    inputs_index[7] = recurrent_to_cell_weights_index;
    internal_tensors[8] = runtime_context.getTensorByIndex(recurrent_to_output_weights_index);
    inputs_index[8] = recurrent_to_output_weights_index;

    const auto cell_to_input_weights_index = cur_op->inputs()->operator[](9);
    const auto cell_to_forget_weights_index = cur_op->inputs()->operator[](10);
    const auto cell_to_output_weights_index = cur_op->inputs()->operator[](11);
    // optional cell_to_input_weights_index
    // optional cell_to_forget_weights_index
    // optional cell_to_output_weights_index
    internal_tensors[9] = runtime_context.getTensorByIndex(cell_to_input_weights_index);
    inputs_index[9] = cell_to_input_weights_index;
    internal_tensors[10] = runtime_context.getTensorByIndex(cell_to_forget_weights_index);
    inputs_index[10] = cell_to_forget_weights_index;
    internal_tensors[11] = runtime_context.getTensorByIndex(cell_to_output_weights_index);
    inputs_index[11] = cell_to_output_weights_index;

    const auto input_gate_bias_index = cur_op->inputs()->operator[](12);
    const auto forget_gate_bias_index = cur_op->inputs()->operator[](13);
    const auto cell_gate_bias_index = cur_op->inputs()->operator[](14);
    const auto output_gate_bias_index = cur_op->inputs()->operator[](15);
    // optional input_gate_bias_index
    assert(forget_gate_bias_index != -1);
    assert(cell_gate_bias_index != -1);
    assert(output_gate_bias_index != -1);
    internal_tensors[12] = runtime_context.getTensorByIndex(input_gate_bias_index);
    inputs_index[12] = input_gate_bias_index;
    internal_tensors[13] = runtime_context.getTensorByIndex(forget_gate_bias_index);
    inputs_index[13] = forget_gate_bias_index;
    internal_tensors[14] = runtime_context.getTensorByIndex(cell_gate_bias_index);
    inputs_index[14] = cell_gate_bias_index;
    internal_tensors[15] = runtime_context.getTensorByIndex(output_gate_bias_index);
    inputs_index[15] = output_gate_bias_index;

    const auto projection_weights_index = cur_op->inputs()->operator[](16);
    const auto projection_bias_index = cur_op->inputs()->operator[](17);
    // optional projection_weights_index
    // optional projection_bias_index
    internal_tensors[16] = runtime_context.getTensorByIndex(projection_weights_index);
    inputs_index[16] = projection_weights_index;
    internal_tensors[17] = runtime_context.getTensorByIndex(projection_bias_index);
    inputs_index[17] = projection_bias_index;

    const auto output_state_index = cur_op->inputs()->operator[](18);
    const auto cell_state_index = cur_op->inputs()->operator[](19);
    assert(output_state_index != -1);
    assert(cell_state_index != -1);
    internal_tensors[18] = runtime_context.getTensorByIndex(output_state_index);
    inputs_index[18] = output_state_index;
    internal_tensors[19] = runtime_context.getTensorByIndex(cell_state_index);
    inputs_index[19] = cell_state_index;

    const auto input_layer_norm_coefficients_index = cur_op->inputs()->operator[](20);
    const auto forget_layer_norm_coefficients_index = cur_op->inputs()->operator[](21);
    const auto cell_layer_norm_coefficients_index = cur_op->inputs()->operator[](22);
    const auto output_layer_norm_coefficients_index = cur_op->inputs()->operator[](23);
    // optional input_layer_norm_coefficients_index
    // optional forget_layer_norm_coefficients_index
    // optional cell_layer_norm_coefficients_index
    // optional output_layer_norm_coefficients_index
    internal_tensors[20] = runtime_context.getTensorByIndex(input_layer_norm_coefficients_index);
    inputs_index[20] = input_layer_norm_coefficients_index;
    internal_tensors[21] = runtime_context.getTensorByIndex(forget_layer_norm_coefficients_index);
    inputs_index[21] = forget_layer_norm_coefficients_index;
    internal_tensors[22] = runtime_context.getTensorByIndex(cell_layer_norm_coefficients_index);
    inputs_index[22] = cell_layer_norm_coefficients_index;
    internal_tensors[23] = runtime_context.getTensorByIndex(output_layer_norm_coefficients_index);
    inputs_index[23] = output_layer_norm_coefficients_index;

    output_index = cur_op->outputs()->operator[](0);
    assert(output_index != -1);
    output_internal = runtime_context.getTensorByIndex(output_index);

    options = cur_op->builtin_options_as_UnidirectionalSequenceLSTMOptions();

    return Ok;
  }

  OMStatus validateTensorTypes()
  {
    if (input()->type() != output_state()->type() or output()->type() != input()->type())
      return FailedCheckCondition;

    for (int32_t i = 1; i < 9; ++i)
    {
      if (internal_tensors[i] != nullptr and
          input_to_forget_weights()->type() != internal_tensors[i]->type())
      {
        return FailedCheckCondition;
      }
    }

    for (int32_t i = 12; i < 16; ++i)
    {
      if (internal_tensors[i] != nullptr and
          forget_gate_bias()->type() != internal_tensors[i]->type())
      {
        return FailedCheckCondition;
      }
    }

    return Ok;
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

  OMStatus readData(uint16_t op_index, core::OMRuntimeStorage &storage,
                    core::OMRuntimeContext &context)
  {
    OMStatus status = Ok;

    for (uint32_t i = 0; i < 24; ++i)
    {
      if (inputs_index[i] == -1)
        continue;
      status = storage.getDataByTensorIndex(&inputs_data[i], inputs_index[i]);
      if (inputs_data[i] == nullptr)
        status = context.getConstDataByTensorIndex(&inputs_data[i], inputs_index[i]);
      if (status != Ok)
        return status;
    }

    if (output_index == -1)
      return UnknownError;

    status = storage.getDataByTensorIndex(&output_data, output_index);

    if (status != Ok)
      return status;

    if (storage.getKernelType(op_index) == core::Inplace)
    {
      output_data = inputs_data[0];
      status = storage.removeTensorFromTensorIndexToData(inputs_index[0]);

      if (status != Ok)
        return status;

      status = storage.saveDataToTensorIndex(output_data, output_index);
    }
    return status;
  }

public:
  const circle::Tensor *output_internal;
  const circle::Tensor *internal_tensors[24];

  int32_t inputs_index[24] = {-1};
  int32_t output_index = -1;

  uint8_t *inputs_data[24] = {nullptr};
  uint8_t *output_data = nullptr;
};

struct GateParameters
{
  FullyConnectedParams input_fc_params;
  FullyConnectedParams recurrent_fc_params;
};

struct InterGateParameters
{
  ArithmeticParams forget_cell_mul_params;
  ArithmeticParams input_mul_params;
  ArithmeticParams output_mul_params;
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
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_UNIDIRECTIONAL_SEQUENCE_LSTM_KERNEL_H
