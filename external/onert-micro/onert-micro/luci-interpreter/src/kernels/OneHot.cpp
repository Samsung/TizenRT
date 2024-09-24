/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "kernels/OneHot.h"
#include "kernels/Utils.h"

namespace luci_interpreter
{
namespace kernels
{

namespace
{

template <typename T>
void OneHotComputeImpl(const Tensor *indices_tensor, const Tensor *on_value_tensor,
                       const Tensor *off_value_tensor, int32_t depth, int32_t axis,
                       Tensor *output_tensor)
{
  // define input shape and correct axis
  auto const &input_shape = indices_tensor->shape();
  axis = axis == -1 ? input_shape.num_dims() : axis;

  // TODO support other integer input types
  auto const *indices = getTensorData<int32_t>(indices_tensor);
  auto const on_value = getTensorData<T>(on_value_tensor)[0];
  auto const off_value = getTensorData<T>(off_value_tensor)[0];
  auto *output = getTensorData<T>(output_tensor);

  // prefix_dim_size == # of elements before the axis
  // depth == # of elements per axis
  // suffix_dim_size == # of elements after the axis
  auto prefix_dim_size = 1;
  for (int32_t i = 0; i < axis; ++i)
  {
    prefix_dim_size *= input_shape.dim(i);
  }
  assert(prefix_dim_size > 0);
  auto const suffix_dim_size = input_shape.num_elements() / prefix_dim_size;

  // View the indices as a matrix of size:
  //     prefix_dim_size x suffix_dim_size
  // View the output as a matrix of size:
  //     prefix_dim_size x depth x suffix_dim_size
  // Then the output is:
  //     output(i, j, k) == (indices(i, k) == j) ? on : off
  for (int32_t i = 0; i < prefix_dim_size; ++i)
    for (int32_t j = 0; j < depth; ++j)
      for (int32_t k = 0; k < suffix_dim_size; ++k, ++output)
        *output = indices[i * suffix_dim_size + k] == j ? on_value : off_value;
}

} // namespace

OneHot::OneHot(const Tensor *indices, const Tensor *depth, const Tensor *on_value,
               const Tensor *off_value, Tensor *output, const OneHotParams &params)
  : KernelWithParams<OneHotParams>({indices, depth, on_value, off_value}, {output}, params)
{
  // Do nothing
}

void OneHot::configure()
{
  // check types
  LUCI_INTERPRETER_CHECK(indices()->element_type() == DataType::S32);
  LUCI_INTERPRETER_CHECK(depth()->element_type() == DataType::S32);
  LUCI_INTERPRETER_CHECK(on_value()->element_type() == off_value()->element_type());
  LUCI_INTERPRETER_CHECK(output()->element_type() == on_value()->element_type());

  // check shape dependent parameters
  LUCI_INTERPRETER_CHECK(on_value()->shape().num_elements() == 1);
  LUCI_INTERPRETER_CHECK(off_value()->shape().num_elements() == 1);
  LUCI_INTERPRETER_CHECK(depth()->shape().num_elements() == 1);
  LUCI_INTERPRETER_CHECK(params().axis >= -1 && params().axis <= indices()->shape().num_dims());

  // define parameters that affect the output shape
  auto const depth_value = getTensorData<int32_t>(depth())[0];
  auto const &input_shape = indices()->shape();
  auto const input_dims = input_shape.num_dims();
  auto const axis = params().axis == -1 ? input_dims : params().axis;

  // define output shape
  Shape output_shape(input_shape.num_dims() + 1);
  {
    for (int32_t d = 0; d < axis; ++d)
      output_shape.dim(d) = input_shape.dim(d);

    output_shape.dim(axis) = depth_value;

    for (int32_t d = axis + 1; d < output_shape.num_dims(); ++d)
      output_shape.dim(d) = input_shape.dim(d - 1);
  }
  // TODO: enable it only if kernel with dynamic shapes
  // reshape output
  output()->resize(output_shape);
}

void OneHot::execute() const
{
  auto const depth_value = getTensorData<int32_t>(depth())[0];
  auto const axis = params().axis;

  switch (output()->element_type())
  {
    case DataType::FLOAT32:
      OneHotComputeImpl<float>(indices(), on_value(), off_value(), depth_value, axis, output());
      break;
    case DataType::U8:
      OneHotComputeImpl<uint8_t>(indices(), on_value(), off_value(), depth_value, axis, output());
      break;
    case DataType::S16:
      OneHotComputeImpl<int16_t>(indices(), on_value(), off_value(), depth_value, axis, output());
      break;
    default:
      // TODO Support other data types
      assert(false && "Not supported, yet!");
      break;
  }
}

} // namespace kernels
} // namespace luci_interpreter
