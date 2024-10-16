/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2021 The TensorFlow Authors. All Rights Reserved.
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

#include "Builders.h"
#include "kernels/Utils.h"
#include "TISOKernel.h"

#include <cassert>

namespace luci_interpreter
{
namespace
{

template <typename InputT, typename CoordsT = int32_t>
void gather(const circle::GatherOptions *options, kernels::TISOKernel *kernel)
{
  kernels::TISOData tiso_data = kernel->readData();

  const InputT *input_data = kernels::getTensorData<InputT>(tiso_data.input1_data);
  const CoordsT *coords_data = kernels::getTensorData<CoordsT>(tiso_data.input2_data);
  InputT *output_data = kernels::getTensorData<InputT>(tiso_data.output_data);

  const circle::Tensor *input = kernel->input1();
  const circle::Tensor *coords = kernel->input2();

  const int input_dims_size = Tensor::num_dims(input);
  int axis = options->axis();
  if (axis < 0)
  {
    axis += input_dims_size;
  }

  int batch_dims = options->batch_dims();
  // batch_dims should be in range: [-rank(coords), rank(coords)].
  // Negative batch_dims is added with rank of coords.
  const int coords_dims_size = Tensor::num_dims(coords);
  if (batch_dims < 0)
  {
    batch_dims += coords_dims_size;
  }

  const int axis_size = Tensor::dim(input, axis);

  int batch_size = 1;
  for (int i = 0; i < batch_dims; ++i)
  {
    batch_size *= Tensor::dim(input, i);
  }
  int outer_size = 1;
  for (int i = batch_dims; i < axis; ++i)
  {
    outer_size *= Tensor::dim(input, i);
  }
  int inner_size = 1;
  for (int i = axis + 1; i < input_dims_size; ++i)
  {
    inner_size *= Tensor::dim(input, i);
  }
  int coord_size = 1;
  for (int i = batch_dims; i < coords_dims_size; ++i)
  {
    coord_size *= Tensor::dim(coords, i);
  }

  for (int batch = 0; batch < batch_size; ++batch)
  {
    for (int outer = 0; outer < outer_size; ++outer)
    {
      for (int coord = 0; coord < coord_size; ++coord)
      {
        auto x = coords_data[coord];
        std::memcpy(
          output_data + (((batch * outer_size) + outer) * coord_size + coord) * inner_size,
          input_data +
            (((batch * outer_size) + outer) * axis_size + coords_data[batch * coord_size + coord]) *
              inner_size,
          sizeof(InputT) * inner_size);
      }
    }
  }
}

} // namespace

void configure_kernel_CircleGather(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  const auto *options = cur_op->builtin_options_as_GatherOptions();

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input2()) == DataType::S32);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) == DataType::FLOAT32 or
                         Tensor::element_type(kernel.input1()) == DataType::S8 or
                         Tensor::element_type(kernel.input1()) == DataType::S32);

  int32_t axis = options->axis();
  int32_t num_dims = Tensor::num_dims(kernel.input1());
  if (axis < 0)
  {
    axis += num_dims;
  }

  LUCI_INTERPRETER_CHECK(axis >= 0 and axis < num_dims);

  int32_t batch_dims = options->batch_dims();
  int32_t coords_num_dims = Tensor::num_dims(kernel.input2());
  // batch_dims should be in range: [-rank(coords), rank(coords)].
  // Negative batch_dims is added with rank of coords.
  if (batch_dims < 0)
  {
    batch_dims += coords_num_dims;
  }
  LUCI_INTERPRETER_CHECK(batch_dims <= axis);
  LUCI_INTERPRETER_CHECK(batch_dims >= 0 and batch_dims < num_dims);
  LUCI_INTERPRETER_CHECK(batch_dims <= coords_num_dims);
  for (int i = 0; i < batch_dims; ++i)
  {
    LUCI_INTERPRETER_CHECK(Tensor::dim(kernel.input1(), i) == Tensor::dim(kernel.input2(), i));
  }
}

void execute_kernel_CircleGather(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  const auto *options = cur_op->builtin_options_as_GatherOptions();

  switch (Tensor::element_type(kernel.input1()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      return gather<float, int32_t>(options, &kernel);
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case DataType::S8:
      return gather<int8_t, int32_t>(options, &kernel);
#endif // DIS_QUANT
    case DataType::S32:
      return gather<int32_t, int32_t>(options, &kernel);
    default:
      assert(false && "Unsupported type");
  }
}

} // namespace luci_interpreter
