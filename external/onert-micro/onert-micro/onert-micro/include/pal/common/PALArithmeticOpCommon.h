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

#ifndef ONERT_MICRO_EXECUTE_PAL_ARITHMETIC_OP_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_ARITHMETIC_OP_COMMON_H

#include "PALUtils.h"
#include "ProcessBroadcastShapes.h"

#include "core/OMKernelData.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{

template <typename T> struct AddFn
{
  T operator()(T lhs, T rhs) { return lhs + rhs; }
};
template <typename T> struct SubFn
{
  T operator()(T lhs, T rhs) { return lhs - rhs; }
};
template <typename T> struct MulFn
{
  T operator()(T lhs, T rhs) { return lhs * rhs; }
};
template <typename T> struct DivFn
{
  T operator()(T lhs, T rhs) { return lhs / rhs; }
};
template <typename T> struct SquaredDifferenceFn
{
  T operator()(T lhs, T rhs) { return (lhs - rhs) * (lhs - rhs); }
};
template <typename T, typename Fn>
OMStatus ArithmeticOp(const core::BinaryArithmeticBroadcastParams &params, const int flat_size,
                      const T *input1_data, const T *input2_data, T *output_data)
{
  T activation_min, activation_max;
  getActivationParams(params, &activation_min, &activation_max);

  Fn func;
  for (int i = 0; i < flat_size; ++i)
    output_data[i] =
      std::min(std::max(func(input1_data[i], input2_data[i]), activation_min), activation_max);

  return Ok;
}

template <typename T>
void ElementWise(const uint32_t size, const core::ArithmeticQuantParams &params,
                 const T *input1_data, const T *input2_data, T *output_data,
                 T (*binary_func)(T, T, const core::ArithmeticQuantParams &))
{
  for (int i = 0; i < size; ++i)
  {
    output_data[i] = binary_func(input1_data[i], input2_data[i], params);
  }
}

template <typename T, typename Fn>
inline void ArithmeticOpScalar(const core::BinaryArithmeticBroadcastParams &params,
                               const int flat_size, const T *input_data, const T scalar_value,
                               T *output_data)
{
  T activation_min, activation_max;
  getActivationParams(params, &activation_min, &activation_max);

  for (int i = 0; i < flat_size; ++i)
    output_data[i] =
      std::min(std::max(func(input_data[i], scalar_value), activation_min), activation_max);
}

template <typename T, typename Fn>
OMStatus BroadcastArithmeticOp4DSlow(const core::BinaryArithmeticBroadcastParams &params,
                                     const core::OMRuntimeShape &input1_shape, const T *input1_data,
                                     const core::OMRuntimeShape &input2_shape, const T *input2_data,
                                     const core::OMRuntimeShape &output_shape, T *output_data)
{
  NdArrayDesc<4> desc1;
  NdArrayDesc<4> desc2;
  NdArrayDescsForElementwiseBroadcast(input1_shape, input2_shape, &desc1, &desc2);
  const core::OMRuntimeShape extended_output_shape =
    core::OMRuntimeShape::extendedShape(4, output_shape);

  T activation_min, activation_max;
  getActivationParams(params, &activation_min, &activation_max);

  // In Tensorflow, the dimensions are canonically named (batch_number, row,
  // col, channel), with extents (batches, height, width, depth), with the
  // trailing dimension changing most rapidly (channels has the smallest stride,
  // typically 1 element).
  //
  // In generated C code, we store arrays with the dimensions reversed. The
  // first dimension has smallest stride.
  //
  // We name our variables by their Tensorflow convention, but generate C code
  // nesting loops such that the innermost loop has the smallest stride for the
  // best cache behavior.
  Fn func;
  for (int b = 0; b < extended_output_shape.dims(0); ++b)
  {
    for (int y = 0; y < extended_output_shape.dims(1); ++y)
    {
      for (int x = 0; x < extended_output_shape.dims(2); ++x)
      {
        for (int c = 0; c < extended_output_shape.dims(3); ++c)
        {
          const int output_data_offset =
            ((b * extended_output_shape.dims(1) + y) * extended_output_shape.dims(2) + x) *
              extended_output_shape.dims(3) +
            c;

          output_data[output_data_offset] =
            std::min(std::max(func(input1_data[subscriptToIndex(desc1, b, y, x, c)],
                                   input2_data[subscriptToIndex(desc2, b, y, x, c)]),
                              activation_min),
                     activation_max);
        }
      }
    }
  }
  return Ok;
}

template <typename T>
void BroadcastInput1(int size, const core::ArithmeticQuantParams &params, const T *input1_data,
                     const T *input2_data, T *output_data,
                     T (*binary_func)(T, T, const core::ArithmeticQuantParams &))
{
  for (int i = 0; i < size; ++i)
  {
    output_data[i] = binary_func(input1_data[0], input2_data[i], params);
  }
}

template <typename T>
void BroadcastInput2(int size, const core::ArithmeticQuantParams &params, const T *input1_data,
                     const T *input2_data, T *output_data,
                     T (*binary_func)(T, T, const core::ArithmeticQuantParams &))
{
  for (int i = 0; i < size; ++i)
  {
    output_data[i] = binary_func(input1_data[i], input2_data[0], params);
  }
}

template <typename T>
void BroadcastRecursiveDimensions(const core::ArithmeticQuantParams &params, int dimension,
                                  size_t *input1_offset_p, size_t *input2_offset_p,
                                  size_t *output_offset, size_t *compressed_input1_stride,
                                  size_t *compressed_input2_stride, size_t *compressed_output_shape,
                                  const T *input1_data, const T *input2_data, T *output_data,
                                  T (*binary_func)(T, T, const core::ArithmeticQuantParams &))
{
  if (dimension > 0)
  {
    for (size_t c = 0; c < compressed_output_shape[dimension]; ++c)
    {
      size_t input1_offset_c = *input1_offset_p;
      size_t input2_offset_c = *input2_offset_p;
      BroadcastRecursiveDimensions(params, dimension - 1, &input1_offset_c, &input2_offset_c,
                                   output_offset, compressed_input1_stride,
                                   compressed_input2_stride, compressed_output_shape, input1_data,
                                   input2_data, output_data, binary_func);
      *input1_offset_p += compressed_input1_stride[dimension];
      *input2_offset_p += compressed_input2_stride[dimension];
    }
  }
  else
  {
    assert(dimension == 0);
    bool input1_is_broadcast = compressed_input1_stride[dimension] == 0;
    bool input2_is_broadcast = compressed_input2_stride[dimension] == 0;
    assert(!(input1_is_broadcast && input2_is_broadcast));
    const T *input1_data_ptr = input1_data + *input1_offset_p;
    const T *input2_data_ptr = input2_data + *input2_offset_p;
    T *output_data_ptr = output_data + *output_offset;
    if (input1_is_broadcast)
    {
      // input1 is broadcast.
      BroadcastInput1<T>(compressed_output_shape[dimension], params, input1_data_ptr,
                         input2_data_ptr, output_data_ptr, binary_func);
      *input2_offset_p += compressed_output_shape[dimension];
    }
    else if (input2_is_broadcast)
    {
      // input2 is broadcast.
      BroadcastInput2<T>(compressed_output_shape[dimension], params, input1_data_ptr,
                         input2_data_ptr, output_data_ptr, binary_func);
      *input1_offset_p += compressed_output_shape[dimension];
    }
    else
    {
      // Add element-wise.
      ElementWise<T>(compressed_output_shape[dimension], params, input1_data_ptr, input2_data_ptr,
                     output_data_ptr, binary_func);
      *input1_offset_p += compressed_output_shape[dimension];
      *input2_offset_p += compressed_output_shape[dimension];
    }
    *output_offset += compressed_output_shape[dimension];
  }
}

template <typename T>
void BroadcastBinaryFunction6DSlow(const core::ArithmeticQuantParams &params,
                                   const core::OMRuntimeShape &input1_shape, const T *input1_data,
                                   const core::OMRuntimeShape &input2_shape, const T *input2_data,
                                   const core::OMRuntimeShape &output_shape, T *output_data,
                                   T (*binary_func)(T, T, const core::ArithmeticQuantParams &))
{
  constexpr int kMaxBroadcastDim = 6;

  // In Tensorflow, the dimensions are canonically named (batch_number, row,
  // col, channel), with extents (batches, height, width, depth), with the
  // trailing dimension changing most rapidly (channels has the smallest stride,
  // typically 1 element).
  //
  // In generated C code, we store arrays with the dimensions reversed. The
  // first dimension has smallest stride.
  //
  // We name our variables by their Tensorflow convention, but generate C code
  // nesting loops such that the innermost loop has the smallest stride for the
  // best cache behavior.
  size_t compressed_input1_stride[kMaxBroadcastDim];
  size_t compressed_input2_stride[kMaxBroadcastDim];
  size_t compressed_output_shape[kMaxBroadcastDim];
  bool broadcastable_shape = ReduceDimensionsForBroadcast<kMaxBroadcastDim>(
    input1_shape, input2_shape, compressed_input1_stride, compressed_input2_stride,
    compressed_output_shape);
  // Skip broadcasting for degenerate shapes.
  if (!broadcastable_shape)
  {
    return;
  }

  size_t input1_offset = 0;
  size_t input2_offset = 0;
  size_t output_offset = 0;
  BroadcastRecursiveDimensions(params, kMaxBroadcastDim - 1, &input1_offset, &input2_offset,
                               &output_offset, compressed_input1_stride, compressed_input2_stride,
                               compressed_output_shape, input1_data, input2_data, output_data,
                               binary_func);
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_ARITHMETIC_OP_COMMON_H
