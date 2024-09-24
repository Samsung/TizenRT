/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2023 The TensorFlow Authors. All Rights Reserved.
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
#include "Split.h"

namespace luci_interpreter
{

void configure_kernel_CircleSplitV(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto axis_index = cur_op->inputs()->operator[](2);
  LUCI_INTERPRETER_CHECK(axis_index != -1);

  const auto axis = runtime_graph->getCircleTensorByIndex(axis_index);
  LUCI_INTERPRETER_CHECK(axis != nullptr);

  // Dynamic output tensors are needed if axis tensor is not constant
  // Now doesn't support dynamic shapes for SplitV
  LUCI_INTERPRETER_CHECK(runtime_graph->getConstDataByTensor(axis) != nullptr);
}

void execute_kernel_CircleSplitV(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto axis_index = cur_op->inputs()->operator[](2);

  assert(input_index != -1);
  assert(axis_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto axis = runtime_graph->getCircleTensorByIndex(axis_index);

  assert(input != nullptr);
  assert(axis != nullptr);

  const auto *axis_data = runtime_graph->getDataByTensor(axis);
  if (axis_data == nullptr)
    axis_data = runtime_graph->getConstDataByTensor(axis);

  assert(axis_data);

  int32_t axis_value = (kernels::getTensorData<int32_t>(axis_data))[0];
  if (axis_value < 0)
    axis_value += Tensor::num_dims(input);

  assert(axis_value >= 0);
  assert(axis_value < Tensor::num_dims(input));

  switch (Tensor::element_type(input))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      return splitImpl<float>(cur_op, input, axis_value, runtime_graph);
    }
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case DataType::S8:
    {
      return splitImpl<int8_t>(cur_op, input, axis_value, runtime_graph);
    }
    case DataType::S16:
    {
      return splitImpl<int16_t>(cur_op, input, axis_value, runtime_graph);
    }
#endif // DIS_QUANT
    case DataType::S32:
    {
      return splitImpl<int32_t>(cur_op, input, axis_value, runtime_graph);
    }
    default:
      assert(false && "Unsupported type");
  }
}

} // namespace luci_interpreter
