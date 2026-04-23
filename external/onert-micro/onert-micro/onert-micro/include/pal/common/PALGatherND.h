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
#ifndef ONERT_MICRO_EXECUTE_PAL_GATHER_ND_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_GATHER_ND_COMMON_H

#include "OMStatus.h"
#include "core/OMRuntimeShape.h"

#include <cmath>

namespace onert_micro
{
namespace execute
{
namespace pal
{

constexpr int MAX_INDICES_ND = 5;

template <typename ParamsT, typename IndicesT>
inline OMStatus GatherND(core::OMRuntimeShape params_shape, const ParamsT *param_data,
                         core::OMRuntimeShape indices_shape, const IndicesT *index_data,
                         ParamsT *output_data)
{
  const int indices_dims = indices_shape.dimensionsCount();
  const int indices_nd = indices_shape.dims(indices_dims - 1);
  const int params_dims = params_shape.dimensionsCount();

  int n_slices = 1;
  for (int i = 0; i < indices_dims - 1; ++i)
  {
    n_slices *= indices_shape.dims(i);
  }

  // If indices[-1] == params.rank, fetch single elements.
  // If indices[-1] < params.rank, fetch slices.
  int slice_size = 1;
  for (int i = indices_nd; i < params_dims; ++i)
  {
    slice_size *= params_shape.dims(i);
  }

  int params_flat_size = params_shape.flatSize();
  int remain_flat_size = params_flat_size;

  // Number of elements per dimension
  int dims_to_count[MAX_INDICES_ND];
  for (int i = 0; i < indices_nd; ++i)
  {
    dims_to_count[i] = remain_flat_size / params_shape.dims(i);
    remain_flat_size = dims_to_count[i];
  }

  for (int i = 0; i < n_slices; ++i)
  {
    int from_pos = 0;
    for (int j = 0; j < indices_nd; ++j)
    {
      int offset = i * indices_nd + j;
      IndicesT index = index_data[offset];
      from_pos += index * dims_to_count[j];
    }
    if (from_pos < 0 || from_pos + slice_size > params_flat_size)
    {
      assert(false && "GatherND error");
      return UnknownError;
    }
    std::memcpy(output_data + i * slice_size, param_data + from_pos, sizeof(ParamsT) * slice_size);
  }

  return Ok;
}

} // namespace pal

} // namespace execute

} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_GATHER_ND_COMMON_H
