/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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
#include "kernels/Utils.h"

#include "PALSelectV2.h"

namespace luci_interpreter
{

namespace
{

constexpr int kInputTensorCondition = 0;
constexpr int kInputTensorX = 1;
constexpr int kInputTensorY = 2;
constexpr int kOutputTensor = 0;

template <typename T>
void CallSelect(const circle::Tensor *input_condition, const circle::Tensor *input_x,
                const circle::Tensor *input_y, const circle::Tensor *output, bool need_broadcast,
                RuntimeGraph *runtime_graph)
{
  using Func = decltype(luci_interpreter_pal::Select<bool, T>) *;
  Func select_func;
  if (need_broadcast)
  {
    assert(false && "Broadcast not supported now");
  }
  else
  {
    select_func = luci_interpreter_pal::Select<bool, T>;
  }

  select_func(kernels::getTensorRuntimeShape(input_condition, runtime_graph),
              kernels::getTensorData<bool>(runtime_graph->getDataByTensor(input_condition)),
              kernels::getTensorRuntimeShape(input_x, runtime_graph),
              kernels::getTensorData<T>(runtime_graph->getDataByTensor(input_x)),
              kernels::getTensorRuntimeShape(input_y, runtime_graph),
              kernels::getTensorData<T>(runtime_graph->getDataByTensor(input_y)),
              kernels::getTensorRuntimeShape(output, runtime_graph),
              kernels::getTensorData<T>(runtime_graph->getDataByTensor(output)));
}

} // namespace

void configure_kernel_CircleSelectV2(const circle::Operator *cur_op,
                                     BaseRuntimeGraph *runtime_graph)
{
  const auto input_cond_index = cur_op->inputs()->operator[](kInputTensorCondition);
  const auto input_x_index = cur_op->inputs()->operator[](kInputTensorX);
  const auto input_y_index = cur_op->inputs()->operator[](kInputTensorY);
  const auto output_index = cur_op->outputs()->operator[](kOutputTensor);

  assert(input_cond_index != -1);
  assert(input_x_index != -1);
  assert(input_y_index != -1);
  assert(output_index != -1);

  const auto input_cond = runtime_graph->getCircleTensorByIndex(input_cond_index);
  const auto input_x = runtime_graph->getCircleTensorByIndex(input_x_index);
  const auto input_y = runtime_graph->getCircleTensorByIndex(input_y_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  assert(input_cond != nullptr);
  assert(input_x != nullptr);
  assert(input_y != nullptr);

  // Input condition should be bool
  LUCI_INTERPRETER_CHECK(Tensor::element_type(input_cond) == DataType::BOOL);

  // X, Y and Output should be the same type
  LUCI_INTERPRETER_CHECK(Tensor::element_type(input_x) == Tensor::element_type(input_y));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(input_x) == Tensor::element_type(output));

  bool possible_mixed_scaler =
    Tensor::num_elements(input_cond) == 1 && Tensor::num_elements(input_x) == 1 &&
    Tensor::num_elements(input_y) == 1 && Tensor::num_elements(output) == 1;

  bool same_shape = Tensor::num_elements(input_cond) == Tensor::num_elements(input_x) &&
                    Tensor::num_elements(input_x) == Tensor::num_elements(input_y);

  // Broadcast not supported now
  if (not same_shape and not possible_mixed_scaler)
  {
    LUCI_INTERPRETER_CHECK(false);
  }
}

void execute_kernel_CircleSelectV2(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto input_cond_index = cur_op->inputs()->operator[](kInputTensorCondition);
  const auto input_x_index = cur_op->inputs()->operator[](kInputTensorX);
  const auto input_y_index = cur_op->inputs()->operator[](kInputTensorY);
  const auto output_index = cur_op->outputs()->operator[](kOutputTensor);

  assert(input_cond_index != -1);
  assert(input_x_index != -1);
  assert(input_y_index != -1);
  assert(output_index != -1);

  const auto input_cond = runtime_graph->getCircleTensorByIndex(input_cond_index);
  const auto input_x = runtime_graph->getCircleTensorByIndex(input_x_index);
  const auto input_y = runtime_graph->getCircleTensorByIndex(input_y_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  assert(input_cond != nullptr);
  assert(input_x != nullptr);
  assert(input_y != nullptr);

  bool possible_mixed_scaler =
    Tensor::num_elements(input_cond) == 1 && Tensor::num_elements(input_x) == 1 &&
    Tensor::num_elements(input_y) == 1 && Tensor::num_elements(output) == 1;

  bool same_shape = Tensor::num_elements(input_cond) == Tensor::num_elements(input_x) &&
                    Tensor::num_elements(input_x) == Tensor::num_elements(input_y);
  bool is_broadcast = false;
  if (not possible_mixed_scaler and not same_shape)
    is_broadcast = true;

  const auto type = Tensor::element_type(input_x);
  switch (type)
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      CallSelect<float>(input_cond, input_x, input_y, output, is_broadcast, runtime_graph);
      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
