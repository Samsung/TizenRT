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

#ifndef LUCI_INTERPRETER_PAL_UNIDIRECTIONAL_SEQUENCE_LSTM_H
#define LUCI_INTERPRETER_PAL_UNIDIRECTIONAL_SEQUENCE_LSTM_H

#include "PALUnidirectionalSequenceLSTMCommon.h"

#ifndef DIS_QUANT

namespace luci_interpreter_pal
{
// Evaluate the LSTM kernel with (potential) multi-steps and multi-batch input
template <>
void evalLSTM<int8_t, int8_t, int16_t, int32_t>(
  luci_interpreter::lstm::LSTMStruct *lstm_struct,
  luci_interpreter::lstm::LSTMParameters *lstm_params,
  luci_interpreter::lstm::CellStateInfo *cell_state_info, int8_t *output_state_data,
  int16_t *cell_state_data, int16_t *scratch0, int16_t *scratch1, int16_t *scratch2,
  int16_t *scratch3, luci_interpreter::BaseRuntimeGraph *runtime_graph)
{
  lstm_internal::LstmSizeInfo size_info;

  size_info.time_major = lstm_struct->options->time_major();
  size_info.batch_size = size_info.time_major
                           ? luci_interpreter::Tensor::dim(lstm_struct->input(), 1)
                           : luci_interpreter::Tensor::dim(lstm_struct->input(), 0);
  size_info.time_steps = size_info.time_major
                           ? luci_interpreter::Tensor::dim(lstm_struct->input(), 0)
                           : luci_interpreter::Tensor::dim(lstm_struct->input(), 1);
  size_info.input_dimension = luci_interpreter::Tensor::dim(lstm_struct->input(), 2);
  size_info.state_dimension = luci_interpreter::Tensor::dim(lstm_struct->output_state(), 1);

  lstm_internal::LstmStepManager step_info(size_info);

  // time is the first dimention, enable batch computation
  if (size_info.time_major)
  {
    for (int t = 0; t < size_info.time_steps; t++)
    {
      lstm_internal::lstmStep<int8_t, int8_t, int16_t, int32_t>(
        lstm_struct, lstm_params, &step_info, cell_state_info, output_state_data, cell_state_data,
        scratch0, scratch1, scratch2, scratch3, runtime_graph);
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
        lstm_internal::lstmStep<int8_t, int8_t, int16_t, int32_t>(
          lstm_struct, lstm_params, &step_info, cell_state_info, output_state_data, cell_state_data,
          scratch0, scratch1, scratch2, scratch3, runtime_graph);
        // prepare for the next time step
        step_info.updateTime();
      }
      // prepare for the next batch
      step_info.updateBatch();
      step_info.resetTime();
    }
  }
}

} // namespace luci_interpreter_pal

#endif // DIS_QUANT

#endif // LUCI_INTERPRETER_PAL_UNIDIRECTIONAL_SEQUENCE_LSTM_H
