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

#ifndef ONERT_MICRO_PAL_COMMON_UNIDIRECTIONAL_SEQUENCE_LSTM_COMMON_H
#define ONERT_MICRO_PAL_COMMON_UNIDIRECTIONAL_SEQUENCE_LSTM_COMMON_H

#include "core/OMUtils.h"
#include "execute/kernels/UnidirectionalSequenceLSTM.h"
#include "PALTanh.h"
#include "PALLogistic.h"
#include "PALFullyConnected.h"
#include "PALMul.h"
#include "PALUtils.h"

using namespace onert_micro::core::utils;
using namespace onert_micro::execute::pal;

namespace onert_micro
{
namespace lstm_internal
{
namespace
{
// Possible fused activation functions.
typedef enum
{
  kTfLiteActNone = 0,
  kTfLiteActRelu,
  kTfLiteActReluN1To1, // min(max(-1, x), 1)
  kTfLiteActRelu6,     // min(max(0, x), 6)
  kTfLiteActTanh,
  kTfLiteActSignBit,
  kTfLiteActSigmoid,
} FusedActivation;

} // namespace

#ifndef DIS_FLOAT
// Input and output have the same shape in LSTM
void mul(const onert_micro::core::OMRuntimeShape &shape,
         const BinaryArithmeticBroadcastParams *params, const float *input1_data,
         const float *input2_data, float *output_data)
{
  const int flat_size = shape.flatSize();
  onert_micro::execute::pal::Mul(*params, flat_size, input1_data, input2_data, output_data);
}

void addElementWise(const float *input_1, const float *input_2, int n_batch, int n_input,
                    float *output)
{
  for (int batch = 0; batch < n_batch; ++batch)
  {
    for (int i = 0; i < n_input; ++i)
    {
      const int index = batch * n_input + i;
      output[index] = input_1[index] + input_2[index];
    }
  }
}

void tanh(int32_t, const onert_micro::core::OMRuntimeShape &input_data_shape, float *input_data,
          const onert_micro::core::OMRuntimeShape &output_data_shape, float *output_data)
{
  onert_micro::execute::pal::Tanh(input_data_shape, input_data, output_data_shape, output_data);
}

void sigmoid(const onert_micro::core::OMRuntimeShape &data_shape, float *data)
{
  const int flat_size = data_shape.flatSize();
  onert_micro::execute::pal::Logistic(flat_size, data, data);
}

void clipping(const int v_size, const onert_micro::lstm::CellStateInfo *cell_state_info,
              float *vector)
{
  for (int i = 0; i < v_size; i++)
  {
    vector[i] =
      std::max(std::min(cell_state_info->cell_clip, vector[i]), -cell_state_info->cell_clip);
  }
}
#endif // DIS_FLOAT

// Size information about the LSTM kernel, which is deduced from tensors stored
// in the flat buffer file.
struct LstmSizeInfo
{
  bool time_major;
  int32_t batch_size;
  int32_t time_steps;
  int32_t input_dimension;
  int32_t state_dimension;
};

class LstmStepManager
{
public:
  LstmStepManager() = delete;
  // Does not take any ownership, and all pointers must refer to valid objects
  // that outlive the one constructed.
  explicit LstmStepManager(const LstmSizeInfo &size_info) : size_info_(size_info) {}

  void updateTime()
  {
    current_time_ += 1;
    // default as one batch per inference
    int input_step = size_info_.input_dimension;
    int output_step = size_info_.state_dimension;
    // time major: batch inference
    if (size_info_.time_major)
    {
      input_step = input_step * size_info_.batch_size;
      output_step = output_step * size_info_.batch_size;
    }

    input_offset_ += input_step;
    output_offset_ += output_step;
  }

  void updateBatch()
  {
    current_batch_ += 1;
    // batch inference for time major: no action needed
    if (size_info_.time_major)
    {
      return;
    }
    // otherwise: singe batch inference, go to the next batch
    hidden_state_offset_ += size_info_.state_dimension;
    cell_state_offset_ += size_info_.state_dimension;
  }

  void resetTime() { current_time_ = 0; }

  onert_micro::core::OMRuntimeShape inputShape() const
  {
    int batch_size = 1;
    if (size_info_.time_major)
    {
      batch_size = size_info_.batch_size;
    }
    const int dims[2] = {batch_size, size_info_.input_dimension};
    const int32_t *dims_data = reinterpret_cast<const int32_t *>(dims);
    return onert_micro::core::OMRuntimeShape(2, dims_data);
  }

  onert_micro::core::OMRuntimeShape stateShape() const
  {
    int batch_size = 1;
    if (size_info_.time_major)
    {
      batch_size = size_info_.batch_size;
    }
    const int dims[2] = {batch_size, size_info_.state_dimension};
    const int32_t *dims_data = reinterpret_cast<const int32_t *>(dims);
    return onert_micro::core::OMRuntimeShape(2, dims_data);
  }

  int inputOffset() const { return input_offset_; }

  int outputOffset() const { return output_offset_; }

  int hiddenStateOffset() const { return hidden_state_offset_; }

  int cellStateOffset() const { return cell_state_offset_; }

private:
  int32_t current_time_ = 0;
  int32_t current_batch_ = 0;
  int32_t input_offset_ = 0;
  int32_t output_offset_ = 0;
  int32_t hidden_state_offset_ = 0;
  int32_t cell_state_offset_ = 0;

  const LstmSizeInfo &size_info_;
};

// Calculates a single LSTM gate.
// Implements the following formula:
//   gate = activate(FC(input) + FC(recurrent))
// Activation is sigmoid except for the "cell" gate (configurable, usually tanh)
template <typename ActivationType, typename WeightType, typename CellType, typename BiasType>
void calculateLstmGate(const LstmStepManager *step_info,
                       const onert_micro::lstm::GateParameters *gate_params,
                       // Input FC
                       ActivationType *input_data, const WeightType *input_weight_data,
                       const OMRuntimeShape &input_weight_shape, const BiasType *input_bias_data,
                       // Recurrent FC
                       ActivationType *recurrent_data, const WeightType *recurrent_weight_data,
                       const OMRuntimeShape &recurrent_weight_shape,
                       const BiasType *recurrent_bias_data,
                       // Output
                       CellType *gate_output,
                       // Scratch arrays
                       CellType *fc_output_buffer, const FusedActivation activation)
{
  // Input FC
  const auto gate_output_shape = step_info->stateShape();
  {
    FullyConnectedParams op_params{};
    op_params.float_activation_max = gate_params->input_fc_params.float_activation_max;
    op_params.float_activation_min = gate_params->input_fc_params.float_activation_min;

    FullyConnected(op_params, input_data + step_info->inputOffset(), input_weight_shape,
                   input_weight_data, input_bias_data, gate_output_shape, gate_output);
  }

  // Recurrent FC
  {
    FullyConnectedParams op_params{};
    op_params.float_activation_max = gate_params->recurrent_fc_params.float_activation_max;
    op_params.float_activation_min = gate_params->recurrent_fc_params.float_activation_min;

    FullyConnected(op_params, recurrent_data + step_info->hiddenStateOffset(),
                   recurrent_weight_shape, recurrent_weight_data, recurrent_bias_data,
                   gate_output_shape, fc_output_buffer);

    addElementWise(gate_output, fc_output_buffer, /*n_batch=*/gate_output_shape.dimsData()[0],
                   /*n_state=*/gate_output_shape.dimsData()[1], gate_output);

    switch (activation)
    {
      case FusedActivation::kTfLiteActSigmoid:
        sigmoid(gate_output_shape, gate_output);
        break;
      case FusedActivation::kTfLiteActTanh:
      {
        // Set the scale power to -12 to avoid shift
        tanh(/*cell_state_scale_power=*/-12, gate_output_shape, gate_output, gate_output_shape,
             gate_output);
      }
      break;
      default:
        // Only Sigmoid or Tanh is used.
        assert(false && "Only Sigmoid or Tanh is used");
    }
  }
}

// Update the hidden state of the LSTM kernel using the following formula:
// updated_hidden_state = Tanh(updated_cell_state) * output_gate_output, * means
// element wise multiplication
template <typename CellType, typename ActivationType>
void updateLstmHidden(const LstmStepManager *step_info, CellType *cell_state_data_base,
                      ActivationType *hidden_state_data, const CellType *output_gate_output,
                      const BinaryArithmeticBroadcastParams *mul_params,
                      int32_t cell_state_scale_power, CellType *buffer)
{
  auto cell_state_shape = step_info->stateShape();
  CellType *cell_state_data = cell_state_data_base + step_info->cellStateOffset();
  // Tanh(cell_state)
  tanh(cell_state_scale_power, cell_state_shape, cell_state_data, cell_state_shape, buffer);
  // Update the hidden state
  mul(cell_state_shape, mul_params, buffer, output_gate_output,
      hidden_state_data + step_info->hiddenStateOffset());
}

// Update the cell state using the output from the forget gate, input gate, and
// cell gate Formula: updated_cell_state = forget_gate_output*cell_state +
// input_gate_output * cell_gate_output, where * denotes element wise
// multiplication
template <typename CellType>
void updateLstmCell(const LstmStepManager *step_info, CellType *cell_state_data,
                    // Gate outputs
                    CellType *forget_gate_output, const CellType *input_gate_output,
                    const CellType *cell_gate_output,
                    // Mul parameters
                    const BinaryArithmeticBroadcastParams &forget_cell_mul_params,
                    const BinaryArithmeticBroadcastParams &input_mul_params,
                    const onert_micro::lstm::CellStateInfo *cell_state_info, CellType *buffer)
{
  auto cell_state_shape = step_info->stateShape();
  // Forget Gate x Cell State
  mul(cell_state_shape, &forget_cell_mul_params, forget_gate_output,
      cell_state_data + step_info->cellStateOffset(),
      cell_state_data + step_info->cellStateOffset());
  // Input Gate x Cell Gate
  mul(cell_state_shape, &input_mul_params, input_gate_output, cell_gate_output, buffer);

  // Update the cell state
  addElementWise(cell_state_data + step_info->cellStateOffset(), buffer,
                 /*n_batch=*/cell_state_shape.dimsData()[0],
                 /*n_state=*/cell_state_shape.dimsData()[1],
                 cell_state_data + step_info->cellStateOffset());

  if (cell_state_info->cell_clip > 0)
  {
    clipping(cell_state_shape.flatSize(), cell_state_info,
             cell_state_data + step_info->cellStateOffset());
  }
}

template <typename ActivationType, typename WeightType, typename CellType, typename BiasType>
void lstmStep(onert_micro::lstm::LSTMStruct *lstm_struct,
              onert_micro::lstm::LSTMParameters *lstm_params, LstmStepManager *step_info,
              onert_micro::lstm::CellStateInfo *cell_state_info, ActivationType *output_state_data,
              CellType *cell_state_data, CellType *scratch0, CellType *scratch1, CellType *scratch2,
              CellType *scratch3, const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;
  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);
  runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);

  const WeightType *input_to_forget_weights_data =
    core::utils::castInputData<WeightType>(runtime_kernel.inputs_data[2]);
  const OMRuntimeShape input_to_forget_weights_shape(runtime_kernel.inputs[2]);
  const BiasType *forget_gate_bias_data =
    core::utils::castInputData<BiasType>(runtime_kernel.inputs_data[13]);
  const WeightType *recurrent_to_forget_weights_data =
    core::utils::castInputData<WeightType>(runtime_kernel.inputs_data[6]);
  const OMRuntimeShape recurrent_to_forget_weights_shape(runtime_kernel.inputs[6]);

  /*Step1: Calculate gate outputs to prepare cell state update*/
  CellType *gate_internal_buffer = scratch3;
  CellType *forget_gate_output = scratch0;

  ActivationType *input_data =
    core::utils::castOutputData<ActivationType>(runtime_kernel.inputs_data[0]);

  calculateLstmGate<ActivationType, WeightType, CellType, BiasType>(
    step_info, &lstm_params->forget_gate_parameters,
    // Input FC
    input_data, input_to_forget_weights_data, input_to_forget_weights_shape, forget_gate_bias_data,
    // Recurrent FC
    output_state_data, recurrent_to_forget_weights_data, recurrent_to_forget_weights_shape, nullptr,
    // Output
    forget_gate_output, gate_internal_buffer, FusedActivation::kTfLiteActSigmoid);

  // Input Gate calculation;
  const WeightType *input_to_input_weights_data =
    core::utils::castInputData<WeightType>(runtime_kernel.inputs_data[1]);
  const OMRuntimeShape input_to_input_weights_shape(runtime_kernel.inputs[1]);
  const BiasType *input_gate_bias_data =
    core::utils::castInputData<BiasType>(runtime_kernel.inputs_data[12]);
  const WeightType *recurrent_to_input_weights_data =
    core::utils::castInputData<WeightType>(runtime_kernel.inputs_data[5]);
  const OMRuntimeShape recurrent_to_input_weights_shape(runtime_kernel.inputs[5]);

  CellType *input_gate_output = scratch1;
  calculateLstmGate<ActivationType, WeightType, CellType, BiasType>(
    step_info, &lstm_params->input_gate_parameters,
    // Input FC
    input_data, input_to_input_weights_data, input_to_input_weights_shape, input_gate_bias_data,
    // Recurrent FC
    output_state_data, recurrent_to_input_weights_data, recurrent_to_input_weights_shape,
    /*recurrent_bias*/ nullptr,
    // Output
    input_gate_output,
    // Scratch arrays
    gate_internal_buffer, FusedActivation::kTfLiteActSigmoid);

  // Cell Gate calculation
  const WeightType *input_to_cell_weights_data =
    core::utils::castInputData<WeightType>(runtime_kernel.inputs_data[3]);
  const OMRuntimeShape input_to_cell_weights_shape(runtime_kernel.inputs[3]);
  const BiasType *cell_gate_bias_data =
    core::utils::castInputData<BiasType>(runtime_kernel.inputs_data[14]);
  const WeightType *recurrent_to_cell_weights_data =
    core::utils::castInputData<WeightType>(runtime_kernel.inputs_data[7]);
  const OMRuntimeShape recurrent_to_cell_weights_shape(runtime_kernel.inputs[7]);

  CellType *cell_gate_output = scratch2;
  calculateLstmGate<ActivationType, WeightType, CellType, BiasType>(
    step_info, &lstm_params->cell_gate_parameters,
    // Input FC
    input_data, input_to_cell_weights_data, input_to_cell_weights_shape, cell_gate_bias_data,
    // Recurrent FC
    output_state_data, recurrent_to_cell_weights_data, recurrent_to_cell_weights_shape,
    /*recurrent_bias*/ nullptr,
    // Output
    cell_gate_output,
    // Scratch arrays
    gate_internal_buffer, FusedActivation::kTfLiteActTanh);

  /*Step2: update the cell state */
  {
    // const InterGateParameters& inter_gate_params = op_data.inter_gate_parameters;
    CellType *updated_input_buffer = scratch1; // reuse buffer

    updateLstmCell<CellType>(
      step_info, cell_state_data, forget_gate_output, input_gate_output, cell_gate_output,
      lstm_params->inter_gate_parameters.forget_cell_mul_params,
      lstm_params->inter_gate_parameters.input_mul_params, cell_state_info, updated_input_buffer);
  }

  {
    /*Step3: update the hidden state */
    const WeightType *input_to_output_weights_data =
      core::utils::castInputData<WeightType>(runtime_kernel.inputs_data[4]);
    const OMRuntimeShape input_to_output_weights_shape(runtime_kernel.inputs[4]);
    const BiasType *output_gate_bias_data =
      core::utils::castInputData<BiasType>(runtime_kernel.inputs_data[15]);
    const WeightType *recurrent_to_output_weights_data =
      core::utils::castInputData<WeightType>(runtime_kernel.inputs_data[8]);
    const OMRuntimeShape recurrent_to_output_weights_shape(runtime_kernel.inputs[8]);

    CellType *output_gate_output = scratch1; // reuse buffer
    calculateLstmGate<ActivationType, WeightType, CellType, BiasType>(
      step_info, &lstm_params->output_gate_parameters,
      // Input FC
      input_data, input_to_output_weights_data, input_to_output_weights_shape,
      output_gate_bias_data,
      // Recurrent FC
      output_state_data, recurrent_to_output_weights_data, recurrent_to_output_weights_shape,
      nullptr,
      // Output
      output_gate_output,
      // Scratch arrays
      gate_internal_buffer, FusedActivation::kTfLiteActSigmoid);
    CellType *tanh_activated_cell_buffer = scratch0; // reuse buffer
    updateLstmHidden<CellType, ActivationType>(
      step_info, cell_state_data, output_state_data, output_gate_output,
      &lstm_params->inter_gate_parameters.output_mul_params,
      cell_state_info->cell_state_scale_power, tanh_activated_cell_buffer);

    ActivationType *output_ptr =
      core::utils::castOutputData<ActivationType>(runtime_kernel.outputs_data[0]);
    std::memcpy(output_ptr + step_info->outputOffset(),
                output_state_data + step_info->hiddenStateOffset(),
                step_info->stateShape().flatSize() * sizeof(ActivationType));
  }
}

} // namespace lstm_internal

// Evaluate the LSTM kernel with (potential) multi-steps and multi-batch input
template <typename ActivationType, typename WeightType, typename CellType, typename BiasType>
void evalLSTM(onert_micro::lstm::LSTMStruct *lstm_struct,
              onert_micro::lstm::LSTMParameters *lstm_params,
              onert_micro::lstm::CellStateInfo *cell_state_info, ActivationType *output_state_data,
              CellType *cell_state_data, CellType *scratch0, CellType *scratch1, CellType *scratch2,
              CellType *scratch3, const OMExecuteArgs &execute_args)
{
  lstm_internal::LstmSizeInfo size_info;

  size_info.time_major = lstm_struct->options->time_major();
  size_info.batch_size =
    size_info.time_major ? dim(lstm_struct->input(), 1) : dim(lstm_struct->input(), 0);
  size_info.time_steps =
    size_info.time_major ? dim(lstm_struct->input(), 0) : dim(lstm_struct->input(), 1);
  size_info.input_dimension = dim(lstm_struct->input(), 2);
  size_info.state_dimension = dim(lstm_struct->output_state(), 1);

  lstm_internal::LstmStepManager step_info(size_info);

  // time is the first dimention, enable batch computation
  if (size_info.time_major)
  {
    for (int t = 0; t < size_info.time_steps; t++)
    {
      lstm_internal::lstmStep<ActivationType, WeightType, CellType, BiasType>(
        lstm_struct, lstm_params, &step_info, cell_state_info, output_state_data, cell_state_data,
        scratch0, scratch1, scratch2, scratch3, execute_args);
      // prepare for the next time step
      step_info.updateTime();
    }
  }
  else
  {
    // batch first, unable to size the input data. single batch inference
    for (int b = 0; b < size_info.batch_size; b++)
    {
      for (int t = 0; t < size_info.time_steps; t++)
      {
        lstm_internal::lstmStep<ActivationType, WeightType, CellType, BiasType>(
          lstm_struct, lstm_params, &step_info, cell_state_info, output_state_data, cell_state_data,
          scratch0, scratch1, scratch2, scratch3, execute_args);
        // prepare for the next time step
        step_info.updateTime();
      }
      // prepare for the next batch
      step_info.updateBatch();
      step_info.resetTime();
    }
  }
}

} // namespace onert_micro

#endif // ONERT_MICRO_PAL_COMMON_UNIDIRECTIONAL_SEQUENCE_LSTM_COMMON_H
