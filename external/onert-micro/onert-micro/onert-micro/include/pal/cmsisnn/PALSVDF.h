/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef ONERT_MICRO_EXECUTE_PAL_SVDF_H
#define ONERT_MICRO_EXECUTE_PAL_SVDF_H

#include "PALSVDFCommon.h"
#include "core/OMRuntimeShape.h"
#include "core/OMKernelData.h"
#include "core/memory/OMMemoryManager.h"

#include <arm_nnfunctions.h>

namespace onert_micro
{
namespace execute
{
namespace pal
{

OMStatus SVDF(const core::SVDFQuantParams &params, const int8_t *input_data,
              const int8_t *weights_feature_data, const int8_t *weights_time_data,
              const int32_t *bias_data, int8_t *state_data, int8_t *output_data,
              const core::OMRuntimeShape &input_shape,
              const core::OMRuntimeShape &weights_feature_shape,
              const core::OMRuntimeShape &weights_time_shape,
              const core::OMRuntimeShape &bias_shape, const core::OMRuntimeShape &output_shape)
{
  cmsis_nn_dims input_dims;
  input_dims.n = input_shape.dims(0);
  input_dims.h = input_shape.dims(1);

  cmsis_nn_dims weights_feature_dims;
  weights_feature_dims.n = weights_feature_shape.dims(0);
  weights_feature_dims.h = weights_feature_shape.dims(1);

  cmsis_nn_dims weights_time_dims;
  weights_time_dims.n = weights_time_shape.dims(0);
  weights_time_dims.h = weights_time_shape.dims(1);

  cmsis_nn_dims bias_dims;
  bias_dims.n = bias_shape.dims(0);

  cmsis_nn_dims state_dims;
  state_dims.n = bias_shape.dims(0);
  state_dims.h = bias_shape.dims(1);

  cmsis_nn_dims output_dims;
  output_dims.n = output_shape.dims(0);
  output_dims.h = output_shape.dims(1);

  cmsis_nn_svdf_params svdf_params;
  svdf_params.rank = params.rank;
  svdf_params.input_offset = params.input_zero_point;
  svdf_params.output_offset = params.output_zero_point;

  svdf_params.input_activation.min = INT16_MIN;
  svdf_params.input_activation.max = INT16_MAX;

  svdf_params.output_activation.min = INT8_MIN;
  svdf_params.output_activation.max = INT8_MAX;

  cmsis_nn_per_tensor_quant_params in_quant_params;
  in_quant_params.multiplier = params.effective_scale_1_a;
  in_quant_params.shift = params.effective_scale_1_b;

  cmsis_nn_per_tensor_quant_params out_quant_params;
  out_quant_params.multiplier = params.effective_scale_2_a;
  out_quant_params.shift = params.effective_scale_2_b;

  const int batch_size = input_shape.dims(0);
  const int input_size = input_shape.dims(1);
  const int num_filters = weights_feature_shape.dims(0);
  const int num_units = num_filters / params.rank;

  uint8_t *scratch_tensor_data;
  OMStatus status = core::memory::OMMemoryManager::allocateMemory(
    batch_size * num_filters * sizeof(int32_t), &scratch_tensor_data);
  assert(status == Ok);
  if (status != Ok)
    return status;

  uint8_t *scratch_output_tensor_data;
  status = core::memory::OMMemoryManager::allocateMemory(batch_size * num_units * sizeof(int32_t),
                                                         &scratch_output_tensor_data);
  assert(status == Ok);
  if (status != Ok)
    return status;

  cmsis_nn_context scratch_ctx;
  scratch_ctx.buf = reinterpret_cast<int32_t *>(scratch_tensor_data);

  cmsis_nn_context scratch_output_ctx;
  scratch_output_ctx.buf = reinterpret_cast<int32_t *>(scratch_output_tensor_data);

  arm_svdf_s8(&scratch_ctx, &scratch_output_ctx, &svdf_params, &in_quant_params, &out_quant_params,
              &input_dims, input_data, &state_dims, state_data, &weights_feature_dims,
              weights_feature_data, &weights_time_dims, weights_time_data, &bias_dims, bias_data,
              &output_dims, output_data);

  core::memory::OMMemoryManager::deallocateMemory(scratch_tensor_data);
  core::memory::OMMemoryManager::deallocateMemory(scratch_output_tensor_data);

  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_SVDF_H
