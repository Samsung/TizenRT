/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

namespace luci_interpreter
{
namespace
{

constexpr int kInputTensor = 0;

template <typename T>
void UnpackImpl(const circle::Operator *cur_op, const circle::Tensor *input, int output_count,
                int axis, RuntimeGraph *runtime_graph)
{
  const auto output0_index = cur_op->outputs()->operator[](0);
  assert(output0_index != -1);

  const auto output0 = runtime_graph->getCircleTensorByIndex(output0_index);
  assert(output0 != nullptr);

  const auto input_dims = Tensor::tensor_shape(input);
  const auto output_dims = Tensor::tensor_shape(output0);
  const int dimensions = input_dims.size();

  if (axis < 0)
  {
    axis += input_dims.size();
  }

  int outer_size = 1;
  for (int i = 0; i < axis; ++i)
  {
    outer_size *= input_dims[i];
  }
  int copy_size = 1;
  for (int i = axis + 1; i < dimensions; ++i)
  {
    copy_size *= input_dims[i];
  }
  int output_size = 1;
  for (int i = 0; i < output_dims.size(); ++i)
  {
    output_size *= output_dims[i];
  }

  const T *input_data = kernels::getTensorData<T>(runtime_graph->getDataByTensor(input));

  for (int i = 0; i < output_count; ++i)
  {
    const auto output_index = cur_op->outputs()->operator[](i);
    assert(output_index != -1);

    const auto t = runtime_graph->getCircleTensorByIndex(output_index);
    assert(output0 != nullptr);
    T *output_data = kernels::getTensorData<T>(runtime_graph->getDataByTensor(t));
    for (int k = 0; k < outer_size; ++k)
    {
      T *output_ptr = output_data + copy_size * k;
      int loc = k * output_count * copy_size + i * copy_size;
      const T *input_ptr = input_data + loc;
      assert(input_ptr != nullptr);
      if (input_ptr != nullptr) {
        for (int j = 0; j < copy_size; ++j)
          output_ptr[j] = input_ptr[j];
      }
    }
  }
}
} // namespace

void configure_kernel_CircleUnpack(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto output_index = cur_op->outputs()->operator[](0);

  assert(input_index != -1);
  assert(output_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  assert(input != nullptr);
  assert(output != nullptr);

  const auto *options = cur_op->builtin_options_as_UnpackOptions();

  LUCI_INTERPRETER_CHECK(cur_op->outputs()->size() == options->num());
  LUCI_INTERPRETER_CHECK(Tensor::element_type(input) == Tensor::element_type(output));

  for (int i = 0; i < Tensor::num_dims(input); ++i)
  {
    if (i == options->axis())
      continue;

    if (i < options->axis())
    {
      LUCI_INTERPRETER_CHECK(Tensor::dim(input, i) == Tensor::dim(output, i));
    }
    else
    {
      LUCI_INTERPRETER_CHECK(Tensor::dim(input, i) == Tensor::dim(output, i - 1));
    }
  }
}

void execute_kernel_CircleUnpack(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  assert(input_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  assert(input != nullptr);

  const auto type = Tensor::element_type(input);

  const auto *options = cur_op->builtin_options_as_UnpackOptions();

  switch (type)
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      UnpackImpl<float>(cur_op, input, options->num(), options->axis(), runtime_graph);
      break;
    }
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case DataType::S8:
    {
      UnpackImpl<int8_t>(cur_op, input, options->num(), options->axis(), runtime_graph);
      break;
    }
#endif
    default:
      assert(false && "Unsupported type");
  }
}
} // namespace luci_interpreter
