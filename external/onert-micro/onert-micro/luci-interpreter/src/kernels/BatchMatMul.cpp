/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "kernels/BatchMatMul.h"
#include "kernels/Utils.h"

#include "PALBatchMatMul.h"

#include <tensorflow/lite/kernels/internal/reference/transpose.h>

namespace
{

tflite::RuntimeShape SwapRowColumnDims(const tflite::RuntimeShape &shape)
{
  tflite::RuntimeShape swapped_shape(shape);
  const int32_t dims = shape.DimensionsCount();
  swapped_shape.SetDim(dims - 2, shape.Dims(dims - 1));
  swapped_shape.SetDim(dims - 1, shape.Dims(dims - 2));
  return swapped_shape;
}

} // namespace

namespace luci_interpreter
{
namespace kernels
{

BatchMatMul::BatchMatMul(const Tensor *x, const Tensor *y, Tensor *output, Tensor *x_tmp,
                         Tensor *y_tmp, const BatchMatMulParams &params)
  : KernelWithParams({x, y}, {output, x_tmp, y_tmp}, params)
{
}

void BatchMatMul::configure()
{
  auto lhs = x();
  auto rhs = y();
  auto adj_x = params().adj_x;
  auto adj_y = params().adj_y;

  // TODO Support non-float types
  if (lhs->element_type() != DataType::FLOAT32 || rhs->element_type() != DataType::FLOAT32)
    assert(false && "Unsupported type.");

  LUCI_INTERPRETER_CHECK(lhs->element_type() == rhs->element_type());

  auto lhs_rank = lhs->shape().num_dims();
  auto rhs_rank = rhs->shape().num_dims();
  LUCI_INTERPRETER_CHECK(lhs_rank >= 2 && lhs_rank <= 4);
  LUCI_INTERPRETER_CHECK(rhs_rank >= 2 && rhs_rank <= 4);

  auto lhs_scratchpad = temp_lhs();
  auto rhs_scratchpad = temp_rhs();
  luci_interpreter_pal::SetupScratchpadTensor(lhs_scratchpad, rhs_scratchpad, getTensorShape(lhs),
                                              getTensorShape(rhs));

  auto output_rank = std::max(lhs_rank, rhs_rank);

  auto extended_lhs_shape = tflite::RuntimeShape::ExtendedShape(output_rank, getTensorShape(lhs));
  auto extended_rhs_shape = tflite::RuntimeShape::ExtendedShape(output_rank, getTensorShape(rhs));

  // Ensure any batch dimensions obey broacasting rules.
  for (int i = 0; i < output_rank - 2; ++i)
  {
    const int lhs_dim = extended_lhs_shape.Dims(i);
    const int rhs_dim = extended_rhs_shape.Dims(i);
    if (lhs_dim != rhs_dim)
    {
      if (lhs_dim != 1)
      {
        LUCI_INTERPRETER_CHECK(rhs_dim == 1);
      }
    }
  }

  // Ensure other dimensions work for matrix multiplication.
  int accum_dim_lhs =
    adj_x ? extended_lhs_shape.Dims(output_rank - 2) : extended_lhs_shape.Dims(output_rank - 1);
  int accum_dim_rhs =
    adj_y ? extended_rhs_shape.Dims(output_rank - 1) : extended_rhs_shape.Dims(output_rank - 2);
  LUCI_INTERPRETER_CHECK(accum_dim_lhs == accum_dim_rhs);

  Shape output_shape(output_rank);
  // Fill in any broadcast dimensions.
  for (int i = 0; i < output_rank - 2; ++i)
  {
    const int lhs_dim = extended_lhs_shape.Dims(i);
    const int rhs_dim = extended_rhs_shape.Dims(i);
    int broadcast_dim = lhs_dim;
    if ((lhs_dim != rhs_dim) && (lhs_dim == 1))
    {
      broadcast_dim = rhs_dim;
    }
    output_shape.dim(i) = broadcast_dim;
  }
  // Fill in the matmul dimensions.
  int lhs_rows_index = adj_x ? output_rank - 1 : output_rank - 2;
  int rhs_cols_index = adj_y ? output_rank - 2 : output_rank - 1;

  output_shape.dim(output_rank - 2) = extended_lhs_shape.Dims(lhs_rows_index);
  output_shape.dim(output_rank - 1) = extended_rhs_shape.Dims(rhs_cols_index);

  output()->resize(output_shape);
}

void TransposeRowsColumns(const Tensor *tensor_in, Tensor *tensor_out)
{
  tflite::RuntimeShape transposed_shape(getTensorShape(tensor_in));
  tflite::RuntimeShape shape(getTensorShape(tensor_in));
  tflite::TransposeParams params;
  int rank = shape.DimensionsCount();
  params.perm_count = rank;
  for (int i = 0; i < rank - 2; ++i)
  {
    params.perm[i] = i;
  }
  // Transpose the last two dimensions.
  params.perm[rank - 2] = rank - 1;
  params.perm[rank - 1] = rank - 2;
  transposed_shape.SetDim(rank - 1, shape.Dims(rank - 2));
  transposed_shape.SetDim(rank - 2, shape.Dims(rank - 1));
  switch (tensor_in->element_type())
  {
    case DataType::FLOAT32:
      tflite::reference_ops::Transpose(params, shape, getTensorData<float>(tensor_in),
                                       transposed_shape, getTensorData<float>(tensor_out));
      break;
    default:
      assert(false && "Only suppport fp32 BatchMatMul for now.");
  }
}

void BatchMatMul::execute() const
{
  auto lhs = x();
  auto rhs = y();

  bool adj_x = params().adj_x;
  bool adj_y = params().adj_y;

  auto orig_lhs_shape = getTensorShape(lhs);
  auto orig_rhs_shape = getTensorShape(rhs);

  auto rhs_tensor = adj_y ? rhs : temp_rhs();
  auto lhs_tensor = adj_x ? temp_lhs() : lhs;
  if (not adj_y)
  {
    TransposeRowsColumns(rhs, temp_rhs());
  }
  if (adj_x)
  {
    TransposeRowsColumns(lhs, temp_lhs());
  }
  tflite::RuntimeShape rhs_shape = adj_y ? orig_rhs_shape : SwapRowColumnDims(orig_rhs_shape);
  tflite::RuntimeShape lhs_shape = adj_x ? orig_lhs_shape : SwapRowColumnDims(orig_lhs_shape);

  switch (x()->element_type())
  {
    case DataType::FLOAT32:
      luci_interpreter_pal::BatchMatMul(rhs_shape, getTensorData<float>(rhs_tensor), lhs_shape,
                                        getTensorData<float>(lhs_tensor), getTensorShape(output()),
                                        getTensorData<float>(output()));
      break;
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace kernels
} // namespace luci_interpreter
