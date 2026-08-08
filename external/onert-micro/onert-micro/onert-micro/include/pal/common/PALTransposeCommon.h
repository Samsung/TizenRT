/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef ONERT_MICRO_PAL_TRANSPOSE_COMMON_H
#define ONERT_MICRO_PAL_TRANSPOSE_COMMON_H

#include "PALUtils.h"
#include "core/OMKernelData.h"
#include "core/OMRuntimeShape.h"
#include "OMStatus.h"
#include "ProcessBroadcastShapes.h"

#include <cassert>

namespace onert_micro
{
namespace execute
{
namespace pal
{
template <typename T, int N>
inline OMStatus TransposeImpl(const core::TransposeParams &params,
                              const core::OMRuntimeShape &unextended_input_shape,
                              const T *input_data,
                              const core::OMRuntimeShape &unextended_output_shape, T *output_data)
{
  const int unextended_input_size = unextended_input_shape.dimensionsCount();
  const int unextended_output_size = unextended_output_shape.dimensionsCount();

  const int input_ext_size = N - unextended_input_size;
  const int output_ext_size = N - unextended_output_size;
  NdArrayDesc<N> input_desc;
  NdArrayDesc<N> output_desc;
  copyDimsToDesc(core::OMRuntimeShape::extendedShape(N, unextended_input_shape), &input_desc);
  copyDimsToDesc(core::OMRuntimeShape::extendedShape(N, unextended_output_shape), &output_desc);

  // The perm data is extended to match the output, each index incremented by
  // the amount of front padding of the input shape.
  int extended_perm[N];
  for (int i = 0; i < N; ++i)
  {
    extended_perm[i] = i < output_ext_size ? i : params.perm[i - output_ext_size] + input_ext_size;
  }

  // Permutes the input shape so we don't need to permute the indexes inside
  // the loop. Check to make sure output_dims is matching input_dims.
  NdArrayDesc<N> perm_input_desc;
  for (int k = 0; k < N; ++k)
  {
    perm_input_desc.extents[k] = input_desc.extents[extended_perm[k]];
    perm_input_desc.strides[k] = input_desc.strides[extended_perm[k]];
  }

  // Naive transpose loop (iterate on output index and compute input index).
  auto tranpose_func = [&](int indexes[N]) {
    output_data[subscriptToIndex(output_desc, indexes)] =
      input_data[subscriptToIndex(perm_input_desc, indexes)];
  };
  NDOpsHelper<N>(output_desc, tranpose_func);
  return Ok;
}

template <typename T, int N = 5>
inline OMStatus Transpose(const core::TransposeParams &params,
                          const core::OMRuntimeShape &unextended_input_shape, const T *input_data,
                          const core::OMRuntimeShape &unextended_output_shape, T *output_data)
{
  // Transpose kernel only does rearranging values not numeric evaluations on
  // each cell. It's safe to implement per size of scalar type and this trick
  // keeps the total code size in a reasonable range.
  OMStatus status;
  switch (sizeof(T))
  {
    case 1:
      status = TransposeImpl<int8_t, N>(
        params, unextended_input_shape, reinterpret_cast<const int8_t *>(input_data),
        unextended_output_shape, reinterpret_cast<int8_t *>(output_data));
      break;
    case 2:
      status = TransposeImpl<int16_t, N>(
        params, unextended_input_shape, reinterpret_cast<const int16_t *>(input_data),
        unextended_output_shape, reinterpret_cast<int16_t *>(output_data));
      break;

    case 4:
      status = TransposeImpl<int32_t, N>(
        params, unextended_input_shape, reinterpret_cast<const int32_t *>(input_data),
        unextended_output_shape, reinterpret_cast<int32_t *>(output_data));
      break;
    case 8:
      status = TransposeImpl<int64_t, N>(
        params, unextended_input_shape, reinterpret_cast<const int64_t *>(input_data),
        unextended_output_shape, reinterpret_cast<int64_t *>(output_data));
      break;
    default:
      status = UnknownError;
      break;
  }
  return status;
}
} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_PAL_TRANSPOSE_COMMON_H
