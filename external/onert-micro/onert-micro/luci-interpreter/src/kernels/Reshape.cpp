/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "Builders.h"
#include "Utils.h"

#include <cassert>
#include <cstring>

namespace luci_interpreter
{

void configure_kernel_CircleReshape(const circle::Operator *, BaseRuntimeGraph *)
{
  // Do nothing
}

// TODO: reduce code duplication with ExpandDims
void execute_kernel_CircleReshape(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto shape_index = cur_op->inputs()->operator[](1);
  const auto output_index = cur_op->outputs()->operator[](0);

  assert(input_index != -1);
  assert(shape_index != -1);
  assert(output_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto shape = runtime_graph->getCircleTensorByIndex(shape_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);
  bool is_inplace = runtime_graph->is_inplace_op(cur_op);
  if (is_inplace)
  {
    runtime_graph->makeInplaceOperation(input, output);
    return;
  }

  const auto input_data = runtime_graph->getDataByTensor(input);
  auto shape_data = runtime_graph->getConstDataByTensor(shape);
  auto output_data = runtime_graph->getDataByTensor(output);

  assert(input_data != nullptr);
  assert(output_data != nullptr);

  int32_t data_size = Tensor::num_elements(output) * getDataTypeSize(Tensor::element_type(output));

#ifndef DIS_DYN_SHAPES
  if (shape_data == nullptr)
  {
    shape_data = runtime_graph->getDataByTensor(shape);
    assert(shape_data != nullptr);

    assert(Tensor::element_type(shape) == DataType::S32);

    const int32_t *shape_data_int = kernels::getTensorData<int32_t>(shape_data);
    const auto num_dims = Tensor::num_dims(output);

    luci_interpreter::RuntimeShape dynamic_shape(num_dims);
    data_size = 1;
    for (int i = 0; i < num_dims; ++i)
    {
      dynamic_shape.setDim(i, shape_data_int[i]);
      data_size *= shape_data_int[i];
    }
    data_size *= size(Tensor::element_type(output));

    runtime_graph->addDynamicShapeTensor(output, std::move(dynamic_shape));

    if (data_size == 0)
    {
      runtime_graph->resetTensorData(nullptr, output);
      return;
    }

    auto new_output_data = new uint8_t[data_size];
    output_data = new_output_data;
    runtime_graph->resetTensorData(new_output_data, output);
  }
#else
  assert(shape_data != nullptr);
#endif // DIS_DYN_SHAPES

  std::memcpy(output_data, input_data, data_size);
}

} // namespace luci_interpreter
