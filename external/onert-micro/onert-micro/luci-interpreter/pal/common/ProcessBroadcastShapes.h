/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_PAL_PROCESS_BROADCAST_SHAPES_H
#define LUCI_INTERPRETER_PAL_PROCESS_BROADCAST_SHAPES_H

namespace luci_interpreter_pal
{

// DO NOT USE THIS STRUCT FOR NEW FUNCTIONALITY BEYOND IMPLEMENTING
// BROADCASTING.
//
// NdArrayDesc<N> describes the shape and memory layout of an N-dimensional
// rectangular array of numbers.
//
// NdArrayDesc<N> is basically identical to Dims<N> defined in types.h.
// However, as Dims<N> is to be deprecated, this class exists as an adaptor
// to enable simple unoptimized implementations of element-wise broadcasting
// operations.
template <int N> struct NdArrayDesc
{
  // The "extent" of each dimension. Indices along dimension d must be in the
  // half-open interval [0, extents[d]).
  int extents[N];

  // The number of *elements* (not bytes) between consecutive indices of each
  // dimension.
  int strides[N];
};

// Copies dims to desc, calculating strides.
template <int N>
inline void copyDimsToDesc(const luci_interpreter::RuntimeShape &input_shape,
                           NdArrayDesc<N> *desc_out)
{
  int desc_stride = 1;
  for (int i = N - 1; i >= 0; --i)
  {
    desc_out->extents[i] = input_shape.dims(i);
    desc_out->strides[i] = desc_stride;
    desc_stride *= input_shape.dims(i);
  }
}

template <int N, int DIM, typename Calc>
typename std::enable_if<DIM == N - 1, void>::type NDOpsHelperImpl(const NdArrayDesc<N> &output,
                                                                  const Calc &calc, int indexes[N])
{
  for (indexes[DIM] = 0; indexes[DIM] < output.extents[DIM]; ++indexes[DIM])
  {
    calc(indexes);
  }
}

template <int N, int DIM, typename Calc>
typename std::enable_if<DIM != N - 1, void>::type NDOpsHelperImpl(const NdArrayDesc<N> &output,
                                                                  const Calc &calc, int indexes[N])
{
  for (indexes[DIM] = 0; indexes[DIM] < output.extents[DIM]; ++indexes[DIM])
  {
    NDOpsHelperImpl<N, DIM + 1, Calc>(output, calc, indexes);
  }
}

// Execute the calc function in the innermost iteration based on the shape of
// the output. The calc function should take a single argument of type int[N].
template <int N, typename Calc>
inline void NDOpsHelper(const NdArrayDesc<N> &output, const Calc &calc)
{
  int indexes[N] = {0};
  NDOpsHelperImpl<N, 0, Calc>(output, calc, indexes);
}

template <int N>
inline void NdArrayDescsForElementwiseBroadcast(const luci_interpreter::RuntimeShape &input0_shape,
                                                const luci_interpreter::RuntimeShape &input1_shape,
                                                NdArrayDesc<N> *desc0_out,
                                                NdArrayDesc<N> *desc1_out)
{

  auto extended_input0_shape = luci_interpreter::RuntimeShape::extendedShape(N, input0_shape);
  auto extended_input1_shape = luci_interpreter::RuntimeShape::extendedShape(N, input1_shape);

  // Copy dims to desc, calculating strides.
  copyDimsToDesc<N>(extended_input0_shape, desc0_out);
  copyDimsToDesc<N>(extended_input1_shape, desc1_out);

  // Walk over each dimension. If the extents are equal do nothing.
  // Otherwise, set the desc with extent 1 to have extent equal to the other and
  // stride 0.
  for (int i = 0; i < N; ++i)
  {
    const int extent0 = extended_input0_shape.dims(i);
    const int extent1 = extended_input1_shape.dims(i);
    if (extent0 != extent1)
    {
      if (extent0 == 1)
      {
        desc0_out->strides[i] = 0;
        desc0_out->extents[i] = extent1;
      }
      else
      {
        desc1_out->strides[i] = 0;
        desc1_out->extents[i] = extent0;
      }
    }
  }
}

inline int subscriptToIndex(const NdArrayDesc<4> &desc, int i0, int i1, int i2, int i3)
{
  return i0 * desc.strides[0] + i1 * desc.strides[1] + i2 * desc.strides[2] + i3 * desc.strides[3];
}

inline int subscriptToIndex(const NdArrayDesc<5> &desc, int indexes[5])
{
  return indexes[0] * desc.strides[0] + indexes[1] * desc.strides[1] +
         indexes[2] * desc.strides[2] + indexes[3] * desc.strides[3] + indexes[4] * desc.strides[4];
}

// Consolidates dimensions in broadcast inputs, checks for five-fold pattern.
//
// For example, if sequence of dimensions of one input is
// ..., 1, 3, 1, 7, 9, 5,... and the other is ..., 2, 3, 1, 7, 1, 1, ...
// we can consolidate these as
// ..., 1, 3*7, 9*5, ... and 2, 3*7, 1.
//
// The category is updated in the less-frequent case of shapes that are
// not suited to a fivefold-loop broadcast.
//
// Falls back to generic pattern when it does not know how to process properly.
//
// Returns true iff there is some sort of broadcast, which includes five-fold
// patterns and falling back to generic broadcast.
inline bool ProcessBroadcastShapes(const luci_interpreter::RuntimeShape &shape0,
                                   const luci_interpreter::RuntimeShape &shape1,
                                   luci_interpreter_pal::ArithmeticParams *params)
{
  const int dims_count = std::max(shape0.dimensionsCount(), shape1.dimensionsCount());

  params->broadcast_category = BroadcastableOpCategory::kGenericBroadcast;

  auto extended_shape0 = luci_interpreter::RuntimeShape::extendedShape(dims_count, shape0);
  auto extended_shape1 = luci_interpreter::RuntimeShape::extendedShape(dims_count, shape1);

  // Check for "exact" match, implicitly accepting any scalar shapes.
  if (extended_shape0 == extended_shape1)
  {
    params->broadcast_category = BroadcastableOpCategory::kNonBroadcast;
    return false;
  }

  if (shape0.flatSize() == 1)
  {
    params->broadcast_category = BroadcastableOpCategory::kScalarFirstBroadcast;
    return true;
  }
  else if (shape1.flatSize() == 1)
  {
    params->broadcast_category = BroadcastableOpCategory::kScalarSecondBroadcast;
    return true;
  }

  for (int i = dims_count - 1; i >= 0; --i)
  {
    if (extended_shape0.dims(i) == extended_shape1.dims(i))
    {
      continue;
    }
    else if (extended_shape0.dims(i) == 1)
    {
      params->broadcast_category = BroadcastableOpCategory::kFirstInputBroadcastsFast;
      return true;
    }
    else if (extended_shape1.dims(i) == 1)
    {
      params->broadcast_category = BroadcastableOpCategory::kSecondInputBroadcastsFast;
      return true;
    }
    else
    {
      // This case is erroneous: there is a dimension that does not match and
      // is not a broadcast from one shape to the other.
      params->broadcast_category = BroadcastableOpCategory::kGenericBroadcast;
      return true;
    }
  }

  return false;
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_PROCESS_BROADCAST_SHAPES_H
