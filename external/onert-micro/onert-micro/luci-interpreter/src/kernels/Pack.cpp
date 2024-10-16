/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "Builders.h"
#include "Utils.h"

#include <cassert>

namespace luci_interpreter
{
namespace
{

template <typename T>
void packImpl(const circle::Tensor *input0, const circle::Tensor *output,
              const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph,
              uint8_t *output_data_raw)
{
  const auto *options = cur_op->builtin_options_as_PackOptions();

  const int values_count = options->values_count();
  int axis = options->axis();
  const int dimensions = Tensor::num_dims(output);

  const auto input_dims = wrap(input0->shape());
  const auto output_dims = wrap(output->shape());

  if (axis < 0)
  {
    axis += dimensions;
  }

  int outer_size = 1;
  for (int i = 0; i < axis; ++i)
    outer_size *= output_dims[i];

  int copy_size = 1;
  for (int i = axis + 1; i < dimensions; ++i)
    copy_size *= output_dims[i];

  int input_size = 1;
  for (int i = 0; i < input_dims.size(); ++i)
    input_size *= input_dims[i];

  assert(input_size == copy_size * outer_size);

  T *output_data = kernels::getTensorData<T>(output_data_raw);
  assert(output_data != nullptr);

  for (int i = 0; i < values_count; ++i)
  {
    const auto input_index = cur_op->inputs()->operator[](i);
    assert(input_index != -1);
    const auto input = runtime_graph->getCircleTensorByIndex(input_index);

    auto input_data = kernels::getTensorData<T>(runtime_graph->getDataByTensor(input));
    assert(input_data != nullptr);
    for (int k = 0; k < outer_size; ++k)
    {
      const T *input_ptr = input_data + copy_size * k;
      int loc = k * values_count * copy_size + i * copy_size;
      T *output_ptr = output_data + loc;
      for (int j = 0; j < copy_size; ++j)
        output_ptr[j] = input_ptr[j];
    }
  }
}

} // namespace

void configure_kernel_CirclePack(const circle::Operator *, BaseRuntimeGraph *)
{
  // Do nothing
}

void execute_kernel_CirclePack(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto output_index = cur_op->outputs()->operator[](0);
  assert(output_index != -1);
  assert(input_index != -1);
  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  auto output_data = runtime_graph->getDataByTensor(output);
  assert(output_data != nullptr);

  switch (Tensor::element_type(output))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      packImpl<float>(input, output, cur_op, runtime_graph, output_data);
      break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case DataType::S8:
      packImpl<int8_t>(input, output, cur_op, runtime_graph, output_data);
      break;
    case DataType::U8:
      packImpl<uint8_t>(input, output, cur_op, runtime_graph, output_data);
      break;
#endif // DIS_QUANT
    case DataType::S32:
      packImpl<int32_t>(input, output, cur_op, runtime_graph, output_data);
      break;
    case DataType::S64:
      packImpl<int64_t>(input, output, cur_op, runtime_graph, output_data);
      break;
    default:
      assert(false && "Unsupported types");
  }
}

} // namespace luci_interpreter
