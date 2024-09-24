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

#ifndef LUCI_INTERPRETER_KERNELS_SPLIT_IMPL_H
#define LUCI_INTERPRETER_KERNELS_SPLIT_IMPL_H

#include "Builders.h"
#include "Utils.h"

namespace luci_interpreter
{

template <typename T>
void splitImpl(const circle::Operator *cur_op, const circle::Tensor *input, int axis_value,
               BaseRuntimeGraph *runtime_graph)
{
  const int output_count = cur_op->outputs()->size();

  const auto output0_index = cur_op->outputs()->operator[](0);
  assert(output0_index != -1);

  const auto output0 = runtime_graph->getCircleTensorByIndex(output0_index);
  assert(output0 != nullptr);

  const int split_dimensions = Tensor::num_dims(input);

  assert(axis_value < split_dimensions);
  assert(Tensor::num_dims(output0) == split_dimensions);

  int64_t outer_size = 1;
  for (int i = 0; i < axis_value; ++i)
  {
    outer_size *= Tensor::dim(input, i);
  }

  int64_t base_inner_size = 1;
  for (int i = axis_value + 1; i < split_dimensions; ++i)
  {
    base_inner_size *= Tensor::dim(input, i);
  }

  const T *input_ptr = kernels::getTensorData<T>(runtime_graph->getDataByTensor(input));
  assert(input_ptr != nullptr);
  for (int k = 0; k < outer_size; ++k)
  {
    for (int i = 0; i < output_count; ++i)
    {
      const auto output_index = cur_op->outputs()->operator[](i);
      assert(output_index != -1);

      const auto output = runtime_graph->getCircleTensorByIndex(output_index);
      assert(output != nullptr);

      T *output_data = kernels::getTensorData<T>(runtime_graph->getDataByTensor(output));
      assert(output_data != nullptr);
      const int copy_size = Tensor::dim(output, axis_value) * base_inner_size;
      T *output_ptr = output_data + k * copy_size;
      assert(output_ptr != nullptr);
      for (int j = 0; j < copy_size; ++j)
        output_ptr[j] = input_ptr[j];
      input_ptr += copy_size;
    }
  }
}

} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNELS_SPLIT_IMPL_H
