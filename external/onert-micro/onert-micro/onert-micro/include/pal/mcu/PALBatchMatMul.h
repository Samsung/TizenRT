/*
 * Copyright (c) 2025 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef ONERT_MICRO_EXECUTE_PAL_BATCHMATMUL_H
#define ONERT_MICRO_EXECUTE_PAL_BATCHMATMUL_H

#include "PALUtils.h"
#include "core/OMRuntimeShape.h"

using namespace onert_micro::core;

namespace onert_micro
{
namespace execute
{
namespace pal
{

// TODO: support more options and layouts
// Current Impl only care about plain BatchMatMul
// This reference implemetation is from tensorflow lite.
// Notice that original code assumes that RHS is transposed by default from caller, which is not
// true here.
template <typename T, typename AccumT>
inline void BatchMatMul(const OMRuntimeShape &lhs_shape, const T *lhs_data,
                        const OMRuntimeShape &rhs_shape, const T *rhs_data,
                        const OMRuntimeShape &output_shape, T *output_data)
{

  const OMRuntimeShape extended_lhs_shape = OMRuntimeShape::extendedShape(5, lhs_shape);
  const OMRuntimeShape extended_rhs_shape = OMRuntimeShape::extendedShape(5, rhs_shape);

  const int batch_dim0 = extended_lhs_shape.dims(0);
  const int batch_dim1 = extended_lhs_shape.dims(1);
  const int batch_dim2 = extended_lhs_shape.dims(2);

  const int lhs_ext0 = extended_lhs_shape.dims(0);
  const int lhs_ext1 = extended_lhs_shape.dims(1);
  const int lhs_ext2 = extended_lhs_shape.dims(2);
  const int rhs_ext0 = extended_rhs_shape.dims(0);
  const int rhs_ext1 = extended_rhs_shape.dims(1);
  const int rhs_ext2 = extended_rhs_shape.dims(2);

  // Set params for each matrix multiply.
  const int lhs_rows = extended_lhs_shape.dims(3);
  const int rhs_cols = extended_rhs_shape.dims(4);
  const int accum_depth = extended_lhs_shape.dims(4);

  // stride for each batch dim
  const int lhs_stride_b0 =
    extended_lhs_shape.dims(1) * extended_lhs_shape.dims(2) * lhs_rows * accum_depth;
  const int lhs_stride_b1 = extended_lhs_shape.dims(2) * lhs_rows * accum_depth;
  const int lhs_stride_b2 = lhs_rows * accum_depth;

  const int rhs_stride_b0 =
    extended_rhs_shape.dims(1) * extended_rhs_shape.dims(2) * accum_depth * rhs_cols;
  const int rhs_stride_b1 = extended_rhs_shape.dims(2) * accum_depth * rhs_cols;
  const int rhs_stride_b2 = accum_depth * rhs_cols;

  for (int b0 = 0; b0 < batch_dim0; ++b0)
  {
    const T *lhs_ptr0 = lhs_data + (b0 * lhs_stride_b0);
    const T *rhs_ptr0 = rhs_data + (b0 * rhs_stride_b0);
    for (int b1 = 0; b1 < batch_dim1; ++b1)
    {
      const T *lhs_ptr1 = lhs_ptr0 + b1 * lhs_stride_b1;
      const T *rhs_ptr1 = rhs_ptr0 + b1 * rhs_stride_b1;
      for (int b2 = 0; b2 < batch_dim2; ++b2)
      {
        const T *lhs_ptr2 = lhs_ptr1 + b2 * lhs_stride_b2;
        const T *rhs_ptr2 = rhs_ptr1 + b2 * rhs_stride_b2;
        T *out_ptr = output_data +
                     ((b0 * batch_dim1 * batch_dim2) + b1 * batch_dim2 + b2) * lhs_rows * rhs_cols;

        for (int j = 0; j < rhs_cols; ++j)
        {
          for (int i = 0; i < lhs_rows; ++i)
          {
            AccumT total = 0;
            for (int k = 0; k < accum_depth; ++k)
            {
              AccumT lhs_val = lhs_ptr2[accum_depth * i + k];
              AccumT rhs_val = rhs_ptr2[j + k * rhs_cols];
              total += (lhs_val) * (rhs_val);
            }
            const int idx = j + i * rhs_cols;
            out_ptr[idx] = static_cast<T>(total);
          }
        }
      }
    }
  }
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_BATCHMATMUL_H
