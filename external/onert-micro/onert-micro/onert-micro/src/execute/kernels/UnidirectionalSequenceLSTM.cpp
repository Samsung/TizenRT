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

#include "OMStatus.h"

#include "core/OMUtils.h"
#include "core/OMKernelData.h"
#include "core/OMDataType.h"

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMUtils.h"
#include "execute/OMRuntimeKernel.h"

#include "execute/kernels/UnidirectionalSequenceLSTM.h"
#include "PALUnidirectionalSequenceLSTM.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::core::utils;
using namespace onert_micro::execute;

namespace onert_micro
{
namespace
{

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

#ifndef DIS_FLOAT
FullyConnectedParams createFcParamsFloat()
{
  FullyConnectedParams op_params;
  calculateActivationRange(circle::ActivationFunctionType::ActivationFunctionType_NONE,
                           &op_params.float_activation_min, &op_params.float_activation_max);
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
  BinaryArithmeticBroadcastParams op_params;
  calculateActivationRange(circle::ActivationFunctionType::ActivationFunctionType_NONE,
                           &op_params.float_activation_min, &op_params.float_activation_max);
  // op_params.quantized_activation_max = op_params.float_activation_max;
  // op_params.quantized_activation_min = op_params.float_activation_min;
  float_lstm_params->inter_gate_parameters.forget_cell_mul_params = op_params;
  float_lstm_params->inter_gate_parameters.input_mul_params = op_params;
  float_lstm_params->inter_gate_parameters.output_mul_params = op_params;
}

void evalFloat(const OMExecuteArgs &execute_args)
{
  lstm::LSTMStruct lstm_struct(execute_args);

  lstm::CellStateInfo cell_state_info =
    createLstmCellStateInfoFloat(lstm_struct.options->cell_clip());

  lstm::LSTMParameters lstm_params;
  prepareGateParamsFloat(&lstm_params);

  const bool time_major = lstm_struct.options->time_major();
  const auto batch_size = time_major ? dim(lstm_struct.input(), 1) : dim(lstm_struct.input(), 0);
  const auto state_dimension = dim(lstm_struct.output_state(), 1);
  const auto cell_state_type_size =
    getOMDataTypeSize(onertMicroDatatype(lstm_struct.cell_state()->type()));

  size_t scratch_buf_size = batch_size * state_dimension * cell_state_type_size;

  auto scratch_0_data =
    std::make_unique<uint8_t[]>(scratch_buf_size);
  auto scratch_1_data =
    std::make_unique<uint8_t[]>(scratch_buf_size);
  auto scratch_2_data =
    std::make_unique<uint8_t[]>(scratch_buf_size);
  auto scratch_3_data =
    std::make_unique<uint8_t[]>(scratch_buf_size);

  // Create and fill with 0 output state tensor
  auto output_state_data = std::make_unique<float[]>(num_elements(lstm_struct.output_state()));
  std::fill_n(output_state_data.get(), num_elements(lstm_struct.output_state()), 0);

  // Create and fill with 0 cell state tensor
  auto cell_state_data = std::make_unique<float[]>(num_elements(lstm_struct.cell_state()));
  std::fill_n(cell_state_data.get(), num_elements(lstm_struct.cell_state()), 0);

  evalLSTM<float, float, float, float>(
    &lstm_struct, &lstm_params, &cell_state_info, output_state_data.get(), cell_state_data.get(),
    core::utils::castOutputData<float>(scratch_0_data.get()),
    core::utils::castOutputData<float>(scratch_1_data.get()),
    core::utils::castOutputData<float>(scratch_2_data.get()),
    core::utils::castOutputData<float>(scratch_3_data.get()), execute_args);
}
#endif // DIS_FLOAT

void validateWeightTensorSize(const circle::Tensor *weight_tensor, int dim1_size, int dim2_size)
{
  assert(num_dims(weight_tensor) == 2);
  assert(dim(weight_tensor, 0) == dim1_size);
  assert(dim(weight_tensor, 1) == dim2_size);
}

void validateTensorsSize(lstm::LSTMStruct *lstm_struct, const bool time_major)
{
  const auto batch_size = time_major ? dim(lstm_struct->input(), 1) : dim(lstm_struct->input(), 0);

  const auto input_dimension = dim(lstm_struct->input(), 2);
  const auto state_dimension = dim(lstm_struct->output_state(), 1);

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
    assert(num_dims(lstm_struct->get_internal_tensor(i)) == 1);
    assert(dim(lstm_struct->get_internal_tensor(i), 0) == state_dimension);
  }

  // Check the shape of input state tensors.
  // These tensor may be 1D or 2D. It's fine as long as the total size is
  // correct.
  assert(num_elements(lstm_struct->output_state()) == batch_size * state_dimension);
  assert(num_elements(lstm_struct->cell_state()) == batch_size * state_dimension);

  // Check the shape of output tensor against that of input tensor
  assert(num_dims(lstm_struct->output()) == 3);
  assert(dim(lstm_struct->input(), 0) == dim(lstm_struct->output(), 0));
  assert(dim(lstm_struct->input(), 1) == dim(lstm_struct->output(), 1));
  assert(dim(lstm_struct->output(), 2) == state_dimension);
}

} // namespace

namespace execute
{

OMStatus execute_kernel_CircleUnidirectionalSequenceLSTM(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input;

  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);
  input = runtime_kernel.inputs[0];

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
      evalFloat(execute_args);
      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
  return OMStatus::Ok;
}

} // namespace execute
} // namespace onert_micro
