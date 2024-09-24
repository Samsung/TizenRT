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

#ifndef LUCI_INTERPRETER_PAL_COMPARISONS_H
#define LUCI_INTERPRETER_PAL_COMPARISONS_H

#include "Params.h"
#include "ProcessBroadcastShapes.h"
#include "PALUtils.h"

namespace luci_interpreter_pal
{
namespace
{

struct BroadcastComparison4DSlowCommon
{
  const luci_interpreter::RuntimeShape output_shape;
  NdArrayDesc<4> desc1;
  NdArrayDesc<4> desc2;
};

inline BroadcastComparison4DSlowCommon
BroadcastComparison4DSlowPreprocess(const luci_interpreter::RuntimeShape &unextended_input1_shape,
                                    const luci_interpreter::RuntimeShape &unextended_input2_shape,
                                    const luci_interpreter::RuntimeShape &unextended_output_shape)
{
  NdArrayDesc<4> desc1;
  NdArrayDesc<4> desc2;
  NdArrayDescsForElementwiseBroadcast(unextended_input1_shape, unextended_input2_shape, &desc1,
                                      &desc2);
  return {luci_interpreter::RuntimeShape::extendedShape(4, unextended_output_shape), desc1, desc2};
}

} // namespace

template <typename T> inline bool LessFn(T lhs, T rhs) { return lhs < rhs; }
template <typename T> inline bool LessEqualFn(T lhs, T rhs) { return lhs <= rhs; }
template <typename T> inline bool EqualFn(T lhs, T rhs) { return lhs == rhs; }
template <typename T> inline bool GreaterFn(T lhs, T rhs) { return lhs > rhs; }
template <typename T> inline bool GreaterEqualFn(T lhs, T rhs) { return lhs >= rhs; }
template <typename T> inline bool NotEqualFn(T lhs, T rhs) { return lhs != rhs; }

template <typename T>
inline void ComparisonNoScaling(const int64_t flat_size, const T *input1_data, const T *input2_data,
                                bool *output_data, bool F(T, T))
{
  for (int64_t i = 0; i < flat_size; ++i)
  {
    output_data[i] = F(input1_data[i], input2_data[i]);
  }
}

template <typename T>
inline void BroadcastComparison4DSlowWithScaling(
  const ComparisonParams &op_params, const luci_interpreter::RuntimeShape &unextended_input1_shape,
  const T *input1_data, const luci_interpreter::RuntimeShape &unextended_input2_shape,
  const T *input2_data, const luci_interpreter::RuntimeShape &unextended_output_shape,
  bool *output_data, bool F(T, T))
{
  const BroadcastComparison4DSlowCommon dims = BroadcastComparison4DSlowPreprocess(
    unextended_input1_shape, unextended_input2_shape, unextended_output_shape);

  int left_shift = op_params.left_shift;
  int32_t input1_offset = op_params.input1_offset;
  int32_t input1_multiplier = op_params.input1_multiplier;
  int input1_shift = op_params.input1_shift;
  int32_t input2_offset = op_params.input2_offset;
  int32_t input2_multiplier = op_params.input2_multiplier;
  int input2_shift = op_params.input2_shift;

  for (int b = 0; b < dims.output_shape.dims(0); ++b)
  {
    for (int y = 0; y < dims.output_shape.dims(1); ++y)
    {
      for (int x = 0; x < dims.output_shape.dims(2); ++x)
      {
        for (int c = 0; c < dims.output_shape.dims(3); ++c)
        {
          const int32_t input1_val =
            input1_offset + input1_data[subscriptToIndex(dims.desc1, b, y, x, c)];
          const int32_t input2_val =
            input2_offset + input2_data[subscriptToIndex(dims.desc2, b, y, x, c)];
          const int32_t shifted_input1_val = input1_val * (1 << left_shift);
          const int32_t shifted_input2_val = input2_val * (1 << left_shift);
          const int32_t scaled_input1_val = multiplyByQuantizedMultiplierSmallerThanOneExp(
            shifted_input1_val, input1_multiplier, input1_shift);
          const int32_t scaled_input2_val = multiplyByQuantizedMultiplierSmallerThanOneExp(
            shifted_input2_val, input2_multiplier, input2_shift);

          const int output_data_offset =
            ((b * dims.output_shape.dims(1) + y) * dims.output_shape.dims(2) + x) *
              dims.output_shape.dims(3) +
            c;
          output_data[output_data_offset] = F(scaled_input1_val, scaled_input2_val);
        }
      }
    }
  }
}

template <typename T>
inline void ComparisonWithScaling(const ComparisonParams &op_params, const int64_t flat_size,
                                  const T *input1_data, const T *input2_data, bool *output_data,
                                  bool F(T, T))
{
  int left_shift = op_params.left_shift;
  int32_t input1_offset = op_params.input1_offset;
  int32_t input1_multiplier = op_params.input1_multiplier;
  int input1_shift = op_params.input1_shift;
  int32_t input2_offset = op_params.input2_offset;
  int32_t input2_multiplier = op_params.input2_multiplier;
  int input2_shift = op_params.input2_shift;

  for (int64_t i = 0; i < flat_size; ++i)
  {
    const int32_t input1_val = input1_offset + input1_data[i];
    const int32_t input2_val = input2_offset + input2_data[i];
    const int32_t shifted_input1_val = input1_val * (1 << left_shift);
    const int32_t shifted_input2_val = input2_val * (1 << left_shift);
    const int32_t scaled_input1_val = multiplyByQuantizedMultiplierSmallerThanOneExp(
      shifted_input1_val, input1_multiplier, input1_shift);
    const int32_t scaled_input2_val = multiplyByQuantizedMultiplierSmallerThanOneExp(
      shifted_input2_val, input2_multiplier, input2_shift);
    output_data[i] = F(scaled_input1_val, scaled_input2_val);
  }
}

template <typename T>
inline void BroadcastComparison4DSlowNoScaling(
  const ComparisonParams &op_params, const luci_interpreter::RuntimeShape &unextended_input1_shape,
  const T *input1_data, const luci_interpreter::RuntimeShape &unextended_input2_shape,
  const T *input2_data, const luci_interpreter::RuntimeShape &unextended_output_shape,
  bool *output_data, bool F(T, T))
{
  const BroadcastComparison4DSlowCommon dims = BroadcastComparison4DSlowPreprocess(
    unextended_input1_shape, unextended_input2_shape, unextended_output_shape);

  for (int b = 0; b < dims.output_shape.dims(0); ++b)
  {
    for (int y = 0; y < dims.output_shape.dims(1); ++y)
    {
      for (int x = 0; x < dims.output_shape.dims(2); ++x)
      {
        for (int c = 0; c < dims.output_shape.dims(3); ++c)
        {
          const int output_data_offset =
            ((b * dims.output_shape.dims(1) + y) * dims.output_shape.dims(2) + x) *
              dims.output_shape.dims(3) +
            c;
          output_data[output_data_offset] =
            F(input1_data[subscriptToIndex(dims.desc1, b, y, x, c)],
              input2_data[subscriptToIndex(dims.desc2, b, y, x, c)]);
        }
      }
    }
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_COMPARISONS_H
