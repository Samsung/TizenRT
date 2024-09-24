/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "PALUnidirectionalSequenceLSTM.h"

namespace luci_interpreter
{
namespace
{

#ifndef DIS_QUANT

bool checkedLog2(const float x, int *log2_result)
{
  // Using TfLiteRound instead of std::round and std::log instead of
  // std::log2 to work around these functions being missing in a toolchain
  // used in some TensorFlow tests as of May 2018.
  const float x_log2 = std::log(x) * (1.0f / std::log(2.0f));
  const float x_log2_rounded = std::round(x_log2);
  const float x_log2_fracpart = x_log2 - x_log2_rounded;

  *log2_result = static_cast<int>(x_log2_rounded);
  return std::abs(x_log2_fracpart) < 1e-3f;
}

// Create parameters for element wise multiplication that happens in a) cell
// state update ; b) hidden state update
// Note that all the output of gates are symmetrically quantized so only scales
// are required for input. However, during the hidden state update phase, the
// output is the updated hidden state, which is asymmetrically quantized. Thus
// output may require zero point
luci_interpreter_pal::ArithmeticParams
createInterGateParams(const float input1_scale, const float input2_scale, const float output_scale,
                      const DataType output_type, const int output_zp)
{
  luci_interpreter_pal::ArithmeticParams op_params;
  if (output_type == DataType::S16)
  {
    op_params.quantized_activation_min = std::numeric_limits<int16_t>::min();
    op_params.quantized_activation_max = std::numeric_limits<int16_t>::max();
  }
  else if (output_type == DataType::S8)
  {
    op_params.quantized_activation_min = std::numeric_limits<int8_t>::min();
    op_params.quantized_activation_max = std::numeric_limits<int8_t>::max();
  }

  op_params.input1_offset = 0; // symmetric
  op_params.input2_offset = 0; // symmetric
  op_params.output_offset = output_zp;

  const double input_product_scale =
    static_cast<double>(input1_scale) * static_cast<double>(input2_scale);
  double effective_scale = input_product_scale / static_cast<double>(output_scale);
  auto output_shift = static_cast<int>(op_params.output_shift);
  kernels::quantizeMultiplier(effective_scale, &op_params.output_multiplier, &output_shift);
  op_params.output_shift = output_shift;
  return op_params;
}

void createGateParams(const circle::Tensor *input, const circle::Tensor *input_weight,
                      const circle::Tensor *input_bias, const circle::Tensor *hidden_state,
                      const circle::Tensor *hidden_state_weight,
                      const float nonlinear_activation_input_scale, const DataType cell_type,
                      lstm::GateParameters *gate_params)
{
  // Input CalculateOpDataFullyConnected
  {
    luci_interpreter_pal::FullyConnectedParams input_gate_params;
    double real_multiplier = 0.0;
    int output_shift;
    int32_t output_activation_min;
    int32_t output_activation_max;
    int32_t output_multiplier;
    real_multiplier = kernels::getQuantizedConvolutionMultipler(
      Tensor::scale(input), Tensor::scale(input_weight), nonlinear_activation_input_scale);
    kernels::quantizeMultiplier(real_multiplier, &output_multiplier, &output_shift);
    kernels::calculateActivationRangeQuantized(FusedActFunc::NONE, 0,
                                               nonlinear_activation_input_scale, cell_type,
                                               &output_activation_min, &output_activation_max);

    input_gate_params.output_shift = output_shift;
    input_gate_params.output_multiplier = output_multiplier;
    input_gate_params.quantized_activation_max = output_activation_max;
    input_gate_params.quantized_activation_min = output_activation_min;
    input_gate_params.input_offset = -Tensor::zero_point(input);
    input_gate_params.weights_offset = -Tensor::zero_point(input_weight);
    input_gate_params.output_offset = 0;

    gate_params->input_fc_params = input_gate_params;
  }

  // Recurrent CalculateOpDataFullyConnected
  {
    luci_interpreter_pal::FullyConnectedParams recurrent_gate_params;
    double real_multiplier = 0.0;
    int output_shift;
    int32_t output_activation_min;
    int32_t output_activation_max;
    int32_t output_multiplier;
    real_multiplier = kernels::getQuantizedConvolutionMultipler(Tensor::scale(hidden_state),
                                                                Tensor::scale(hidden_state_weight),
                                                                nonlinear_activation_input_scale);
    kernels::quantizeMultiplier(real_multiplier, &output_multiplier, &output_shift);
    kernels::calculateActivationRangeQuantized(FusedActFunc::NONE, 0,
                                               nonlinear_activation_input_scale, cell_type,
                                               &output_activation_min, &output_activation_max);

    recurrent_gate_params.output_shift = output_shift;
    recurrent_gate_params.output_multiplier = output_multiplier;
    recurrent_gate_params.quantized_activation_max = output_activation_max;
    recurrent_gate_params.quantized_activation_min = output_activation_min;
    recurrent_gate_params.input_offset = -Tensor::zero_point(hidden_state);
    recurrent_gate_params.weights_offset = -Tensor::zero_point(hidden_state_weight);
    recurrent_gate_params.output_offset = 0;

    gate_params->recurrent_fc_params = recurrent_gate_params;
  }
}

void prepareGateParamsInteger(lstm::LSTMStruct *lstm_struct,
                              lstm::LSTMParameters *quant_lstm_params)
{
  float nonlinear_input_scale = 0.00024414062; // 2^-12 Q3.12 -> Q0.15

  createGateParams(lstm_struct->input(), lstm_struct->input_to_forget_weights(),
                   lstm_struct->forget_gate_bias(), lstm_struct->output_state(),
                   lstm_struct->recurrent_to_forget_weights(), nonlinear_input_scale, DataType::S16,
                   &quant_lstm_params->forget_gate_parameters);

  createGateParams(lstm_struct->input(), lstm_struct->input_to_input_weights(),
                   lstm_struct->input_gate_bias(), lstm_struct->output_state(),
                   lstm_struct->recurrent_to_input_weights(), nonlinear_input_scale, DataType::S16,
                   &quant_lstm_params->input_gate_parameters);

  // lstm::GateParameters cell_gate_parameters;
  createGateParams(lstm_struct->input(), lstm_struct->input_to_cell_weights(),
                   lstm_struct->cell_gate_bias(), lstm_struct->output_state(),
                   lstm_struct->recurrent_to_cell_weights(), nonlinear_input_scale, DataType::S16,
                   &quant_lstm_params->cell_gate_parameters);

  // lstm::GateParameters output_gate_parameters;
  createGateParams(lstm_struct->input(), lstm_struct->input_to_output_weights(),
                   lstm_struct->output_gate_bias(), lstm_struct->output_state(),
                   lstm_struct->recurrent_to_output_weights(), nonlinear_input_scale, DataType::S16,
                   &quant_lstm_params->output_gate_parameters);

  // Inter gate multiplication parameters
  float nonlinear_output_scale = 0.00003051757; // 2^-15 Q3.12 -> Q0.15
  float cell_state_scale =
    Tensor::scale(lstm_struct->cell_state()); // lstm_tensors.CellStateTensor()->params.scale;
  // forget gate output (nonlinear output) x cell state -> cell state
  quant_lstm_params->inter_gate_parameters.forget_cell_mul_params = createInterGateParams(
    nonlinear_output_scale, cell_state_scale, cell_state_scale, DataType::S16, 0);

  // input gate output x cell gate output -> cell state
  quant_lstm_params->inter_gate_parameters.input_mul_params = createInterGateParams(
    nonlinear_output_scale, nonlinear_output_scale, cell_state_scale, DataType::S16, 0);

  // tanh output x output gate output -> hidden state (potentially asymmetric)
  quant_lstm_params->inter_gate_parameters.output_mul_params = createInterGateParams(
    nonlinear_output_scale, nonlinear_output_scale, Tensor::scale(lstm_struct->output_state()),
    Tensor::element_type(lstm_struct->output_state()),
    Tensor::zero_point(lstm_struct->output_state()));
}

// Create the additional information about the cell state, which include:
// cell_state_scale_power: used in integer nonlinear function (e.g., tanh)
// quantized_cell_clip: quantized cell clip range
lstm::CellStateInfo createLstmCellStateInfo(const float cell_state_scale, const float cell_clip)
{
  lstm::CellStateInfo cell_state_info;
  // cell_state_scale_power: 2^-cell_state_scale_power = cell state scale
  int buffer;
  checkedLog2(cell_state_scale, &buffer);
  cell_state_info.cell_state_scale_power = buffer;
  // Cell state specifics
  cell_state_info.cell_clip = cell_clip;
  cell_state_info.quantized_cell_clip = static_cast<int16_t>(std::min(
    std::max(static_cast<double>(cell_clip) / static_cast<double>(cell_state_scale), -32768.0),
    32767.0));
  return cell_state_info;
}

void evalInt8(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph, bool)
{
  lstm::LSTMStruct lstm_struct(cur_op, runtime_graph);

  lstm::LSTMParameters quant_lstm_params;
  prepareGateParamsInteger(&lstm_struct, &quant_lstm_params);

  lstm::CellStateInfo cell_state_info = createLstmCellStateInfo(
    luci_interpreter::Tensor::scale(lstm_struct.cell_state()), lstm_struct.options->cell_clip());

  const bool time_major = lstm_struct.options->time_major();
  const auto batch_size =
    time_major ? Tensor::dim(lstm_struct.input(), 1) : Tensor::dim(lstm_struct.input(), 0);
  const auto state_dimension = Tensor::dim(lstm_struct.output_state(), 1);
  const auto cell_state_type_size = getDataTypeSize(Tensor::element_type(lstm_struct.cell_state()));

  auto scratch_0_data =
    std::make_unique<uint8_t[]>(batch_size * state_dimension * cell_state_type_size);
  auto scratch_1_data =
    std::make_unique<uint8_t[]>(batch_size * state_dimension * cell_state_type_size);
  auto scratch_2_data =
    std::make_unique<uint8_t[]>(batch_size * state_dimension * cell_state_type_size);
  auto scratch_3_data =
    std::make_unique<uint8_t[]>(batch_size * state_dimension * cell_state_type_size);

  // Create and fill with 0 output state tensor
  auto output_state_data =
    std::make_unique<int8_t[]>(Tensor::num_elements(lstm_struct.output_state()));
  std::fill_n(output_state_data.get(), Tensor::num_elements(lstm_struct.output_state()), 0);

  // Create and fill with 0 cell state tensor
  auto cell_state_data =
    std::make_unique<int16_t[]>(Tensor::num_elements(lstm_struct.cell_state()));
  std::fill_n(cell_state_data.get(), Tensor::num_elements(lstm_struct.cell_state()), 0);

  luci_interpreter_pal::evalLSTM<int8_t, int8_t, int16_t, int32_t>(
    &lstm_struct, &quant_lstm_params, &cell_state_info, output_state_data.get(),
    cell_state_data.get(), kernels::getTensorData<int16_t>(scratch_0_data.get()),
    kernels::getTensorData<int16_t>(scratch_1_data.get()),
    kernels::getTensorData<int16_t>(scratch_2_data.get()),
    kernels::getTensorData<int16_t>(scratch_3_data.get()), runtime_graph);
}

#endif // DIS_QUANT

#ifndef DIS_FLOAT
luci_interpreter_pal::FullyConnectedParams createFcParamsFloat()
{
  luci_interpreter_pal::FullyConnectedParams op_params;
  kernels::calculateActivationRange(FusedActFunc::NONE, &op_params.float_activation_min,
                                    &op_params.float_activation_max);
  op_params.quantized_activation_max = op_params.float_activation_max;
  op_params.quantized_activation_min = op_params.float_activation_min;
  return op_params;
}

lstm::GateParameters createGateParamsFloat()
{
  lstm::GateParameters gate_params;

  gate_params.input_fc_params = createFcParamsFloat();
  gate_params.recurrent_fc_params = createFcParamsFloat();

  return gate_params;
}

lstm::CellStateInfo createLstmCellStateInfoFloat(const float cell_clip)
{
  lstm::CellStateInfo cell_state_info;
  cell_state_info.cell_clip = cell_clip;
  cell_state_info.cell_state_scale_power = 0; // no quantization
  cell_state_info.quantized_cell_clip = 0;    // no quantization
  return cell_state_info;
}

void prepareGateParamsFloat(lstm::LSTMParameters *float_lstm_params)
{
  // Gate Parameters
  float_lstm_params->forget_gate_parameters = createGateParamsFloat();
  float_lstm_params->input_gate_parameters = createGateParamsFloat();
  float_lstm_params->cell_gate_parameters = createGateParamsFloat();
  float_lstm_params->output_gate_parameters = createGateParamsFloat();

  // Inter gate multiplication parameters
  luci_interpreter_pal::ArithmeticParams op_params;
  kernels::calculateActivationRange(FusedActFunc::NONE, &op_params.float_activation_min,
                                    &op_params.float_activation_max);
  op_params.quantized_activation_max = op_params.float_activation_max;
  op_params.quantized_activation_min = op_params.float_activation_min;
  float_lstm_params->inter_gate_parameters.forget_cell_mul_params = op_params;
  float_lstm_params->inter_gate_parameters.input_mul_params = op_params;
  float_lstm_params->inter_gate_parameters.output_mul_params = op_params;
}

void evalFloat(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph, bool)
{
  lstm::LSTMStruct lstm_struct(cur_op, runtime_graph);

  lstm::CellStateInfo cell_state_info =
    createLstmCellStateInfoFloat(lstm_struct.options->cell_clip());

  lstm::LSTMParameters lstm_params;
  prepareGateParamsFloat(&lstm_params);

  const bool time_major = lstm_struct.options->time_major();
  const auto batch_size =
    time_major ? Tensor::dim(lstm_struct.input(), 1) : Tensor::dim(lstm_struct.input(), 0);
  const auto state_dimension = Tensor::dim(lstm_struct.output_state(), 1);
  const auto cell_state_type_size = getDataTypeSize(Tensor::element_type(lstm_struct.cell_state()));

  auto scratch_0_data =
    std::make_unique<uint8_t[]>(batch_size * state_dimension * cell_state_type_size);
  auto scratch_1_data =
    std::make_unique<uint8_t[]>(batch_size * state_dimension * cell_state_type_size);
  auto scratch_2_data =
    std::make_unique<uint8_t[]>(batch_size * state_dimension * cell_state_type_size);
  auto scratch_3_data =
    std::make_unique<uint8_t[]>(batch_size * state_dimension * cell_state_type_size);

  // Create and fill with 0 output state tensor
  auto output_state_data =
    std::make_unique<float[]>(Tensor::num_elements(lstm_struct.output_state()));
  std::fill_n(output_state_data.get(), Tensor::num_elements(lstm_struct.output_state()), 0);

  // Create and fill with 0 cell state tensor
  auto cell_state_data = std::make_unique<float[]>(Tensor::num_elements(lstm_struct.cell_state()));
  std::fill_n(cell_state_data.get(), Tensor::num_elements(lstm_struct.cell_state()), 0);

  luci_interpreter_pal::evalLSTM<float, float, float, float>(
    &lstm_struct, &lstm_params, &cell_state_info, output_state_data.get(), cell_state_data.get(),
    kernels::getTensorData<float>(scratch_0_data.get()),
    kernels::getTensorData<float>(scratch_1_data.get()),
    kernels::getTensorData<float>(scratch_2_data.get()),
    kernels::getTensorData<float>(scratch_3_data.get()), runtime_graph);
}
#endif // DIS_FLOAT

void validateWeightTensorSize(const circle::Tensor *weight_tensor, int dim1_size, int dim2_size)
{
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(weight_tensor) == 2);
  LUCI_INTERPRETER_CHECK(Tensor::dim(weight_tensor, 0) == dim1_size);
  LUCI_INTERPRETER_CHECK(Tensor::dim(weight_tensor, 1) == dim2_size);
}

void validateTensorsSize(lstm::LSTMStruct *lstm_struct, const bool time_major)
{
  const auto batch_size =
    time_major ? Tensor::dim(lstm_struct->input(), 1) : Tensor::dim(lstm_struct->input(), 0);

  const auto input_dimension = Tensor::dim(lstm_struct->input(), 2);
  const auto state_dimension = Tensor::dim(lstm_struct->output_state(), 1);

  // Input FC weights
  for (int32_t i = 1; i < 5; i++)
  {
    validateWeightTensorSize(lstm_struct->get_internal_tensor(i), state_dimension, input_dimension);
  }

  // Recurrent FC weights
  for (int32_t i = 5; i < 9; i++)
  {
    validateWeightTensorSize(lstm_struct->get_internal_tensor(i), state_dimension, state_dimension);
  }

  // Biases
  for (int32_t i = 12; i < 16; i++)
  {
    LUCI_INTERPRETER_CHECK(Tensor::num_dims(lstm_struct->get_internal_tensor(i)) == 1);
    LUCI_INTERPRETER_CHECK(Tensor::dim(lstm_struct->get_internal_tensor(i), 0) == state_dimension);
  }

  // Check the shape of input state tensors.
  // These tensor may be 1D or 2D. It's fine as long as the total size is
  // correct.
  LUCI_INTERPRETER_CHECK(Tensor::num_elements(lstm_struct->output_state()) ==
                         batch_size * state_dimension);
  LUCI_INTERPRETER_CHECK(Tensor::num_elements(lstm_struct->cell_state()) ==
                         batch_size * state_dimension);

  // Check the shape of output tensor against that of input tensor
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(lstm_struct->output()) == 3);
  LUCI_INTERPRETER_CHECK(Tensor::dim(lstm_struct->input(), 0) ==
                         Tensor::dim(lstm_struct->output(), 0));
  LUCI_INTERPRETER_CHECK(Tensor::dim(lstm_struct->input(), 1) ==
                         Tensor::dim(lstm_struct->output(), 1));
  LUCI_INTERPRETER_CHECK(Tensor::dim(lstm_struct->output(), 2) == state_dimension);
}

} // namespace

void configure_kernel_CircleUnidirectionalSequenceLSTM(const circle::Operator *cur_op,
                                                       BaseRuntimeGraph *runtime_graph)
{
  lstm::LSTMStruct lstm_struct(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(lstm_struct.input()) == DataType::FLOAT32 or
                         Tensor::element_type(lstm_struct.input()) == DataType::S8);

  lstm_struct.validateTensorTypes();

  const bool time_major = lstm_struct.options->time_major();

  validateTensorsSize(&lstm_struct, time_major);

  // No peephole
  for (int32_t i = 9; i < 12; ++i)
    LUCI_INTERPRETER_CHECK(lstm_struct.get_internal_tensor(i) == nullptr);

  // No projection
  for (int32_t i = 16; i < 18; ++i)
    LUCI_INTERPRETER_CHECK(lstm_struct.get_internal_tensor(i) == nullptr);

  // No internal layer norm
  for (int32_t i = 20; i < 24; ++i)
    LUCI_INTERPRETER_CHECK(lstm_struct.get_internal_tensor(i) == nullptr);
}

void execute_kernel_CircleUnidirectionalSequenceLSTM(const circle::Operator *cur_op,
                                                     BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  assert(input_index != -1);

  bool is_inplace = runtime_graph->is_inplace_op(cur_op);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);

  switch (Tensor::element_type(input))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      evalFloat(cur_op, runtime_graph, is_inplace);
      break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case DataType::S8:
      evalInt8(cur_op, runtime_graph, is_inplace);
      break;
#endif // DIS_QUANT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
