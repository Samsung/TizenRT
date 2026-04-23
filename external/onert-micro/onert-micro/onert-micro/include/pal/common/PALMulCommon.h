/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2019 The TensorFlow Authors. All Rights Reserved.
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

#ifndef ONERT_MICRO_EXECUTE_PAL_MUL_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_MUL_COMMON_H

#include "PALArithmeticOpCommon.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{
namespace
{
// Maximum dimension supported by the broadcast mul operation.
constexpr int kMaxMulBroadcastDim = 6;
} // namespace

template <typename T>
OMStatus Mul(const core::BinaryArithmeticBroadcastParams &params, const int flat_size,
             const T *input1_data, const T *input2_data, T *output_data)
{
  ArithmeticOp<T, MulFn<T>>(params, flat_size, input1_data, input2_data, output_data);
  return Ok;
}

template <typename T>
OMStatus BroadcastMul4DSlow(const core::BinaryArithmeticBroadcastParams &params,
                            const core::OMRuntimeShape &input1_shape, const T *input1_data,
                            const core::OMRuntimeShape &input2_shape, const T *input2_data,
                            const core::OMRuntimeShape &output_shape, T *output_data)
{
  BroadcastArithmeticOp4DSlow<T, MulFn<T>>(params, input1_shape, input1_data, input2_shape,
                                           input2_data, output_shape, output_data);
  return Ok;
}

template <typename T>
OMStatus BroadcastMul6DSlow(const core::ArithmeticQuantParams &params,
                            const core::OMRuntimeShape &input1_shape, const T *input1_data,
                            const core::OMRuntimeShape &input2_shape, const T *input2_data,
                            const core::OMRuntimeShape &output_shape, T *output_data)
{
  NdArrayDesc<kMaxMulBroadcastDim> desc1{};
  NdArrayDesc<kMaxMulBroadcastDim> desc2{};
  // The input shapes are extended as part of NdArrayDesc initialization.
  NdArrayDescsForElementwiseBroadcast(input1_shape, input2_shape, &desc1, &desc2);
  const core::OMRuntimeShape extended_output_shape =
    core::OMRuntimeShape::extendedShape(kMaxMulBroadcastDim, output_shape);
  // Cache output shape dimensions.
  int32_t extended_output_shape_dims[kMaxMulBroadcastDim];
  std::memcpy(extended_output_shape_dims, extended_output_shape.dimsData(),
              sizeof(extended_output_shape_dims));

  size_t input1_offset_a = 0;
  size_t input2_offset_a = 0;
  size_t output_offset_a = 0;
  for (int a = 0; a < extended_output_shape_dims[0]; ++a)
  {
    size_t input1_offset_d = input1_offset_a;
    size_t input2_offset_d = input2_offset_a;
    size_t output_offset_d = output_offset_a;
    for (int d = 0; d < extended_output_shape_dims[1]; ++d)
    {
      size_t input1_offset_b = input1_offset_d;
      size_t input2_offset_b = input2_offset_d;
      size_t output_offset_b = output_offset_d;
      for (int b = 0; b < extended_output_shape_dims[2]; ++b)
      {
        size_t input1_offset_y = input1_offset_b;
        size_t input2_offset_y = input2_offset_b;
        size_t output_offset_y = output_offset_b;
        for (int y = 0; y < extended_output_shape_dims[3]; ++y)
        {
          size_t input1_offset_x = input1_offset_y;
          size_t input2_offset_x = input2_offset_y;
          size_t output_offset_x = output_offset_y;
          for (int x = 0; x < extended_output_shape_dims[4]; ++x)
          {
            size_t input1_offset_c = input1_offset_x;
            size_t input2_offset_c = input2_offset_x;
            size_t output_offset_c = output_offset_x;
            for (int c = 0; c < extended_output_shape_dims[5]; ++c)
            {
              const int32_t input1_val = params.input1_offset + input1_data[input1_offset_c];
              const int32_t input2_val = params.input2_offset + input2_data[input2_offset_c];
              const int32_t unclamped_result =
                params.output_offset + multiplyByQuantizedMultiplier(input1_val * input2_val,
                                                                     params.output_multiplier,
                                                                     params.output_shift);
              const int32_t clamped_output =
                std::min(params.quantized_activation_max,
                         std::max(params.quantized_activation_min, unclamped_result));
              output_data[output_offset_c] = static_cast<T>(clamped_output);
              input1_offset_c += desc1.strides[5];
              input2_offset_c += desc2.strides[5];
              ++output_offset_c;
            }
            input1_offset_x += desc1.strides[4];
            input2_offset_x += desc2.strides[4];
            output_offset_x += extended_output_shape_dims[5];
          }
          input1_offset_y += desc1.strides[3];
          input2_offset_y += desc2.strides[3];
          output_offset_y += extended_output_shape_dims[4] * extended_output_shape_dims[5];
        }
        input1_offset_b += desc1.strides[2];
        input2_offset_b += desc2.strides[2];
        output_offset_b += extended_output_shape_dims[3] * extended_output_shape_dims[4] *
                           extended_output_shape_dims[5];
      }
      input1_offset_d += desc1.strides[1];
      input2_offset_d += desc2.strides[1];
      output_offset_d += extended_output_shape_dims[2] * extended_output_shape_dims[3] *
                         extended_output_shape_dims[4] * extended_output_shape_dims[5];
    }
    input1_offset_a += desc1.strides[0];
    input2_offset_a += desc2.strides[0];
    output_offset_a += extended_output_shape_dims[1] * extended_output_shape_dims[2] *
                       extended_output_shape_dims[3] * extended_output_shape_dims[4] *
                       extended_output_shape_dims[5];
  }
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_MUL_COMMON_H
