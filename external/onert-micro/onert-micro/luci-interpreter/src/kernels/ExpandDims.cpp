/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd. All Rights Reserved
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

void configure_kernel_CircleExpandDims(const circle::Operator *cur_op,
                                       BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto axis_index = cur_op->inputs()->operator[](1);
  const auto output_index = cur_op->outputs()->operator[](0);

  assert(input_index != -1);
  assert(axis_index != -1);
  assert(output_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto axis = runtime_graph->getCircleTensorByIndex(axis_index);
  auto output = runtime_graph->getCircleTensorByIndex(output_index);

  assert(input != nullptr);
  assert(axis != nullptr);
  assert(output != nullptr);

  auto axis_data = runtime_graph->getConstDataByTensor(axis);
  assert(axis_data != nullptr);

  int32_t axis_value = 0;

  switch (Tensor::element_type(axis))
  {
    case DataType::S32:
      axis_value = *reinterpret_cast<int32_t *>(axis_data);
      break;
    case DataType::S64:
      axis_value = static_cast<int32_t>(*reinterpret_cast<int64_t *>(axis_data));
      break;
    default:
      assert(false && "Unsupported type.");
  }

  if (axis_value < 0)
  {
    axis_value += Tensor::num_dims(input) + 1;
  }

  LUCI_INTERPRETER_CHECK(axis_value <= Tensor::num_dims(input) and axis_value >= 0);
}

void execute_kernel_CircleExpandDims(const circle::Operator *cur_op,
                                     BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto output_index = cur_op->outputs()->operator[](0);

  assert(input_index != -1);
  assert(output_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  bool is_inplace = runtime_graph->is_inplace_op(cur_op);

  if (is_inplace)
  {
    runtime_graph->makeInplaceOperation(input, output);
    return;
  }

  // Just copy input to output
  const auto input_data = runtime_graph->getDataByTensor(input);
  auto output_data = runtime_graph->getDataByTensor(output);

  assert(input_data != nullptr);
  assert(output_data != nullptr);

  const size_t element_size = getDataTypeSize(Tensor::element_type(input));
  const int32_t num_elements = Tensor::num_elements(input);
  std::memcpy(output_data, input_data, num_elements * element_size);
}

} // namespace luci_interpreter
