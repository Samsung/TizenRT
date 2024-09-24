/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_PAL_SVDF_COMMON_H
#define LUCI_INTERPRETER_PAL_SVDF_COMMON_H

#include "Params.h"

#include <cmath>

namespace luci_interpreter_pal
{

namespace
{

// Returns the floating point value for a fused activation:
inline float activationValFloat(const circle::ActivationFunctionType act, float a)
{
  switch (act)
  {
    case circle::ActivationFunctionType_NONE:
      return a;
    case circle::ActivationFunctionType_RELU:
      return std::max(0.0f, a);
    case circle::ActivationFunctionType_RELU_N1_TO_1:
      return std::max(-1.0f, std::min(a, 1.0f));
    case circle::ActivationFunctionType_RELU6:
      return std::max(0.0f, std::min(a, 6.0f));
    case circle::ActivationFunctionType_TANH:
      return std::tanh(a);
    case circle::ActivationFunctionType_SIGN_BIT:
      return std::signbit(a);
    default:
      assert(false && "Not supported");
  }
  return 0.0f; // To indicate an unsupported activation (i.e. when a new fused
  // activation is added to the enum and not handled here).
}

static inline void
applyTimeWeightsBiasAndActivation(int batch_size, int memory_size, int num_filters, int num_units,
                                  int rank, const float *const weights_time_ptr,
                                  const float *const bias_ptr,
                                  circle::ActivationFunctionType activation, float *const state_ptr,
                                  float *const scratch_ptr, float *const output_ptr)
{
  // Compute matmul(activation_state, weights_time).
  for (int b = 0; b < batch_size; ++b)
  {
    // Perform batched vector dot product:
    float *scratch_ptr_batch = scratch_ptr + b * num_filters;
    const float *vector1_ptr = weights_time_ptr;
    const float *vector2_ptr = state_ptr + b * memory_size * num_filters;
    for (int i = 0; i < num_filters; ++i)
    {
      *scratch_ptr_batch = 0.f;
      for (int j = 0; j < memory_size; ++j)
      {
        *scratch_ptr_batch += *vector1_ptr++ * *vector2_ptr++;
      }
      scratch_ptr_batch++;
    }
  }

  // Initialize output with bias if provided.
  if (bias_ptr)
  {
    // VectorBatchVectorAssign
    for (int i = 0; i < batch_size; ++i)
    {
      float *output_data = output_ptr + i * num_units;
      const float *bias_data = bias_ptr;
      for (int j = 0; j < num_units; ++j)
      {
        *output_data++ = *bias_data++;
      }
    }
  }
  else
  {
    float *output_data = output_ptr;
    for (int i = 0; i < batch_size * num_units; ++i)
    {
      *output_data++ = 0.0f;
    }
  }

  // Reduction sum.
  for (int b = 0; b < batch_size; ++b)
  {
    float *output_ptr_batch = output_ptr + b * num_units;
    float *scratch_ptr_batch = scratch_ptr + b * num_filters;

    // Reduction sum vector
    for (int i = 0; i < num_units; ++i)
    {
      for (int j = 0; j < rank; j++)
      {
        output_ptr_batch[i] += *scratch_ptr_batch++;
      }
    }
  }

  // Apply activation.
  for (int b = 0; b < batch_size; ++b)
  {
    float *output_ptr_batch = output_ptr + b * num_units;
    for (int i = 0; i < num_units; ++i)
    {
      *output_ptr_batch = activationValFloat(activation, *output_ptr_batch);
      ++output_ptr_batch;
    }
  }
}

} // namespace

inline void SVDF(const float *input_data, const float *weights_feature_data,
                 const float *weights_time_data, const float *bias_data, float *state_data,
                 float *scratch_data, float *output_data, const int rank, const int input_size,
                 const int batch_size, const int num_filters, const int num_units,
                 const int memory_size, const circle::ActivationFunctionType activation)
{
  // Left shift the activation_state.
  {
    float *new_state_start = state_data;
    const float *old_state_start = state_data + 1;
    const float *old_state_end = state_data + batch_size * num_filters * memory_size;
    while (old_state_start != old_state_end)
    {
      *new_state_start++ = *old_state_start++;
    }
  }

  // Note: no need to clear the latest activation, matmul is not accumulative.

  // Compute conv1d(inputs, weights_feature).
  // The activation_state's rightmost column is used to save current cycle
  // activation. This is achieved by starting at state_ptr[memory_size - 1] and
  // having the stride equal to memory_size.

  // Perform batched matrix vector multiply operation:
  {
    const float *matrix = weights_feature_data;
    const float *vector = input_data;
    float *result = &state_data[memory_size - 1];
    float *result_in_batch = result;
    for (int i = 0; i < batch_size; ++i)
    {
      const float *matrix_ptr = matrix;
      for (int j = 0; j < num_filters; ++j)
      {
        float dot_prod = 0.0f;
        const float *vector_in_batch = vector + i * input_size;
        for (int k = 0; k < input_size; ++k)
        {
          dot_prod += *matrix_ptr++ * *vector_in_batch++;
        }
        *result_in_batch = dot_prod;
        result_in_batch += memory_size;
      }
    }
  }

  applyTimeWeightsBiasAndActivation(batch_size, memory_size, num_filters, num_units, rank,
                                    weights_time_data, bias_data, activation, state_data,
                                    scratch_data, output_data);
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_SVDF_COMMON_H
