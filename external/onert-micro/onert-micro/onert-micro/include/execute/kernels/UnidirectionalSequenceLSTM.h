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

#ifndef ONERT_MICRO_KERNELS_UNIDIRECTIONAL_SEQUENCE_LSTM_H
#define ONERT_MICRO_KERNELS_UNIDIRECTIONAL_SEQUENCE_LSTM_H

#include "OMStatus.h"

#include "core/OMUtils.h"
#include "core/OMKernelData.h"
#include "core/OMDataType.h"

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMUtils.h"
#include "execute/OMRuntimeKernel.h"

using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

int dim(const circle::Tensor *x, int index)
{
  onert_micro::core::OMRuntimeShape shape(x);
  return shape.dims(index);
}

int num_elements(const circle::Tensor *x)
{
  onert_micro::core::OMRuntimeShape shape(x);
  return shape.flatSize();
}

int num_dims(const circle::Tensor *x)
{
  onert_micro::core::OMRuntimeShape shape(x);
  return shape.dimensionsCount();
}

} // namespace
namespace onert_micro
{
namespace lstm
{

struct LSTMStruct
{
  LSTMStruct() = delete;
  LSTMStruct(const LSTMStruct &) = delete;

  explicit LSTMStruct(const OMExecuteArgs &execute_args)
  {
    core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
    core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
    uint16_t op_index = execute_args.kernel_index;

    execute::OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, runtime_context);

    for (int i; i < 24; i++)
    {
      internal_tensors[i] = runtime_kernel.inputs[i];
    }

    output_internal = runtime_kernel.outputs[0];

    options = runtime_kernel.first_operator->builtin_options_as_UnidirectionalSequenceLSTMOptions();
  }

  void validateTensorTypes()
  {
    assert(input()->type() == (output_state()->type()));
    assert(output()->type() == (input()->type()));

    for (int32_t i = 1; i < 9; ++i)
    {
      assert(internal_tensors[i] == nullptr or
             (input_to_forget_weights()->type()) == (internal_tensors[i])->type());
    }

    for (int32_t i = 12; i < 16; ++i)
    {
      assert(internal_tensors[i] == nullptr or
             (forget_gate_bias()->type()) == (internal_tensors[i]->type()));
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
  FullyConnectedParams input_fc_params;
  FullyConnectedParams recurrent_fc_params;
};

struct InterGateParameters
{
  BinaryArithmeticBroadcastParams forget_cell_mul_params;
  BinaryArithmeticBroadcastParams input_mul_params;
  BinaryArithmeticBroadcastParams output_mul_params;
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
} // namespace onert_micro

#endif // ONERT_MICRO_KERNELS_UNIDIRECTIONAL_SEQUENCE_LSTM_H
