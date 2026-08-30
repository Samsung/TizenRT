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

#ifndef ONERT_MICRO_EXECUTE_PAL_GRU_WEIGHT_GRAD_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_GRU_WEIGHT_GRAD_COMMON_H

#include "OMStatus.h"
#include "core/OMRuntimeShape.h"
#include "core/OMKernelType.h"

#include "PALUtils.h"
#include "ProcessBroadcastShapes.h"
#include "PALFullyConnectedWeightGrad.h"

namespace onert_micro
{
namespace train
{
namespace pal
{
namespace
{

void calculateGRUWeightGrads(
  const float *output_grad_data, const float *weight_input_data, float *weight_input_grad_data,
  const float *weight_hidden_data, float *weight_hidden_grad_data, const float *bias_input_data,
  float *bias_input_grad_data, const float *bias_hidden_data, float *bias_hidden_grad_data,
  const float *input_data, float *input_grad_data, float *state_grad_data,
  const core::OMRuntimeShape &input_shape, const core::OMRuntimeShape &output_fc_shape,
  const core::OMRuntimeShape &output_shape, const core::OMRuntimeShape &weight_input_shape,
  const core::OMRuntimeShape &weight_hidden_shape, float *output_data, float *left_logistic_data,
  float *left_mul_data, float *right_logistic_data, const float *right_mul_left_input_data,
  const float *right_mul_right_input_data, float *tanh_data, const float *middle_mul_left_input,
  const float *middle_mul_right_input, float *left_fc_output_grad_buffer,
  float *right_fc_output_grad_buffer)
{
  int num_elements = output_shape.flatSize();
  for (int i = 0; i < num_elements; ++i)
  {
    // Middle Mul left input grad
    float left_middle_mul = output_grad_data[i];
    left_middle_mul *= middle_mul_right_input[i];

    // Middle Mul right input grad
    float right_middle_mul = output_grad_data[i];
    right_middle_mul *= middle_mul_left_input[i];

    // Tanh` = 1 / (cos(x) ^ 2)
    float tanh_grad_value;
    {
      float tanh = std::tanh(tanh_data[i]);
      if (tanh_data[i] > 19)
      {
        tanh = 1;
      }
      else if (tanh_data[i] < -19)
      {
        tanh = -1;
      }

      tanh_grad_value = (1 - tanh * tanh) * right_middle_mul;
    }

    // Left mul
    float left_mul_grad_value = output_grad_data[i] * output_data[i];

    // Sub` = -1
    // Left Logistic: Logistic` = (exp(-x) * (1 / (1 + exp(-x))) ^ 2)
    float left_logistic_grad_value;
    {
      float log_value = (1 / (1 + std::exp(-left_logistic_data[i])));
      left_logistic_grad_value =
        log_value * (1 - log_value) * (left_middle_mul + left_mul_grad_value);
    }

    // Right mul left input
    float right_mul_left_input = tanh_grad_value;
    right_mul_left_input *= right_mul_right_input_data[i];

    // Right mul right input
    float right_mul_right_input = tanh_grad_value;
    right_mul_right_input *= right_mul_left_input_data[i];

    // Right logistic
    float right_logistic_grad_value;
    {
      float log_value = (1 / (1 + std::exp(-right_logistic_data[i])));
      right_logistic_grad_value = log_value * (1 - log_value) * right_mul_left_input;
    }

    // Left concatenation
    left_fc_output_grad_buffer[i] = left_logistic_grad_value;
    left_fc_output_grad_buffer[i + num_elements] = right_logistic_grad_value;
    left_fc_output_grad_buffer[i + 2 * num_elements] = right_mul_right_input;

    // Right concatenation
    right_fc_output_grad_buffer[i] = left_logistic_grad_value;
    right_fc_output_grad_buffer[i + num_elements] = right_logistic_grad_value;
    right_fc_output_grad_buffer[i + 2 * num_elements] = tanh_grad_value;
  }

  // Left fc weight grad
  FullyConnectedWeightGrad(left_fc_output_grad_buffer, output_fc_shape, output_data, output_shape,
                           weight_input_grad_data, weight_input_shape,
                           core::OpTrainableRankType::ALL);
  // Right fc weight grad
  FullyConnectedWeightGrad(right_fc_output_grad_buffer, output_fc_shape, input_data, input_shape,
                           weight_hidden_grad_data, weight_hidden_shape,
                           core::OpTrainableRankType::ALL);

  // Set state grad to zero
  std::memset(state_grad_data, 0, output_shape.flatSize() * sizeof(float));
}

} // namespace

OMStatus GRUWeightGrads(
  const float *output_grad_data, const float *weight_input_data, float *weight_input_grad_data,
  const float *weight_hidden_data, float *weight_hidden_grad_data, const float *bias_input_data,
  float *bias_input_grad_data, const float *bias_hidden_data, float *bias_hidden_grad_data,
  const float *input_data, float *input_grad_data, float *state_grad_data,
  const core::OMRuntimeShape &input_shape, const core::OMRuntimeShape &output_shape,
  const core::OMRuntimeShape &weight_input_shape, const core::OMRuntimeShape &weight_hidden_shape,
  const core::OMRuntimeShape &output_shape_fc, float *intermediate_buffer,
  float *left_fc_output_grad_buffer, float *right_fc_output_grad_buffer)
{
  const int32_t time = input_shape.dims(0);

  // Init pointers to intermediate values
  size_t offset = output_shape.flatSize();

  size_t data_type_size = sizeof(float);
  const int32_t num_of_intermediate_tensors = 9;
  size_t time_offset = num_of_intermediate_tensors * offset;

  core::OMRuntimeShape two_dim_input_shape(2);
  auto dim_count = input_shape.dimensionsCount();
  if (dim_count < 2)
    return UnsupportedType;
  two_dim_input_shape.setDim(0, input_shape.dims(dim_count - 2));
  two_dim_input_shape.setDim(1, input_shape.dims(dim_count - 1));

  core::OMRuntimeShape two_dim_output_shape(2);
  dim_count = output_shape.dimensionsCount();
  if (dim_count < 2)
    return UnsupportedType;
  two_dim_output_shape.setDim(0, output_shape.dims(dim_count - 2));
  two_dim_output_shape.setDim(1, output_shape.dims(dim_count - 1));

  std::memset(weight_input_grad_data, 0, weight_input_shape.flatSize() * sizeof(float));
  std::memset(weight_hidden_grad_data, 0, weight_hidden_shape.flatSize() * sizeof(float));

  for (int i = 0; i < time; ++i)
  {
    float *output_data = intermediate_buffer;
    float *left_logistic_data = output_data + offset;
    float *left_mul_data = left_logistic_data + offset;
    float *right_logistic_data = left_mul_data + offset;
    float *right_mul_left_input_data = right_logistic_data + offset;
    float *right_mul_right_input_data = right_mul_left_input_data + offset;
    float *tanh_data = right_mul_right_input_data + offset;
    float *middle_mul_left_input = tanh_data + offset;
    float *middle_mul_right_input = middle_mul_left_input + offset;

    calculateGRUWeightGrads(
      output_grad_data, weight_input_data, weight_input_grad_data, weight_hidden_data,
      weight_hidden_grad_data, bias_input_data, bias_input_grad_data, bias_hidden_data,
      bias_hidden_grad_data, input_data, input_grad_data, state_grad_data, two_dim_input_shape,
      output_shape_fc, two_dim_output_shape, weight_input_shape, weight_hidden_shape, output_data,
      left_logistic_data, left_mul_data, right_logistic_data, right_mul_left_input_data,
      right_mul_right_input_data, tanh_data, middle_mul_left_input, middle_mul_right_input,
      left_fc_output_grad_buffer, right_fc_output_grad_buffer);
    input_data += input_shape.dims(2);
    intermediate_buffer += time_offset;
  }
  return Ok;
}

} // namespace pal
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_GRU_WEIGHT_GRAD_COMMON_H
