/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2020 The TensorFlow Authors. All Rights Reserved.
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

#ifndef LUCI_INTERPRETER_PAL_FULLY_CONNECTED_COMMON_H
#define LUCI_INTERPRETER_PAL_FULLY_CONNECTED_COMMON_H

#include "PALUtils.h"
#include "Params.h"

#include <type_traits>

namespace luci_interpreter_pal
{

template <typename InputType, typename WeightType, typename OutputType, typename BiasType>
inline void FullyConnected(const FullyConnectedParams &params, const int32_t *input_shape,
                           const InputType *input_data, const int32_t *filter_shape,
                           const WeightType *filter_data, const BiasType *bias_data,
                           const int32_t *output_shape, OutputType *output_data,
                           uint32_t output_dims_count, uint32_t weights_dims_count)
{
  const int32_t input_offset = params.input_offset;
  const int32_t filter_offset = params.weights_offset;
  const int32_t output_offset = params.output_offset;
  const int32_t output_multiplier = params.output_multiplier;
  const int output_shift = params.output_shift;
  const int32_t output_activation_min = params.quantized_activation_min;
  const int32_t output_activation_max = params.quantized_activation_max;

  const int batches = flatSizeSkipDim(output_shape, output_dims_count - 1, output_dims_count);
  const int output_depth = output_shape[output_dims_count - 1];
  const int accum_depth = filter_shape[weights_dims_count - 1];

  for (int b = 0; b < batches; ++b)
  {
    for (int out_c = 0; out_c < output_depth; ++out_c)
    {
      BiasType acc = 0;
      for (int d = 0; d < accum_depth; ++d)
      {
        int32_t input_val = input_data[b * accum_depth + d];
        int32_t filter_val = filter_data[out_c * accum_depth + d];
        acc += (filter_val + filter_offset) * (input_val + input_offset);
      }
      if (bias_data)
      {
        acc += bias_data[out_c];
      }
      int32_t acc_scaled = multiplyByQuantizedMultiplier(acc, output_multiplier, output_shift);
      acc_scaled += output_offset;
      acc_scaled = std::max(acc_scaled, output_activation_min);
      acc_scaled = std::min(acc_scaled, output_activation_max);
      output_data[out_c + output_depth * b] = static_cast<OutputType>(acc_scaled);
    }
  }
}
template <typename WeightType>
inline void FullyConnected(const FullyConnectedParams &params, const int32_t *input_shape,
                           const float *input_data, const int32_t *filter_shape,
                           const WeightType *filter_data, const float *bias_data,
                           const int32_t *output_shape, float *output_data,
                           uint32_t output_dims_count, uint32_t weights_dims_count)
{
  const float output_activation_min = params.float_activation_min;
  const float output_activation_max = params.float_activation_max;

  const int batches = flatSizeSkipDim(output_shape, output_dims_count - 1, output_dims_count);
  const int output_depth = output_shape[output_dims_count - 1];
  const int accum_depth = filter_shape[weights_dims_count - 1];

  for (int b = 0; b < batches; ++b)
  {
    const float *weight_scale_ptr = params.weights_scales;
    for (int out_c = 0; out_c < output_depth; ++out_c)
    {
      float total = 0.f;
      for (int d = 0; d < accum_depth; ++d)
      {
        auto input_value = input_data[b * accum_depth + d];
        if (std::is_same<WeightType, float>::value)
        {
          total += input_value * filter_data[out_c * accum_depth + d];
        }
        else
        {
          const float filter_scale = *weight_scale_ptr;
          const float filter_value =
            static_cast<float>(filter_data[out_c * accum_depth + d]) * filter_scale;
          total += input_value * filter_value;
        }
      }
      float bias_value = 0.0f;
      if (bias_data)
      {
        bias_value = bias_data[out_c];
      }
      output_data[out_c + output_depth * b] =
        std::min(std::max(total + bias_value, output_activation_min), output_activation_max);
      if (std::is_same<WeightType, int8_t>::value)
      {
        if (params.is_channel_wise_quant)
          weight_scale_ptr++;
      }
    }
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_FULLY_CONNECTED_COMMON_H
