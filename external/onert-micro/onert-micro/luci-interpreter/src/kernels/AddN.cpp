/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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
#include "kernels/Utils.h"

#include "PALAddN.h"

namespace luci_interpreter
{

namespace
{

template <typename T>
void evalGeneric(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto output_index = cur_op->outputs()->operator[](0);
  assert(output_index != -1);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  const auto input_sizes = cur_op->inputs()->size();

  auto input1_index = cur_op->inputs()->operator[](0);
  const auto *tensor1 = runtime_graph->getCircleTensorByIndex(input1_index);

  const int flat_size = Tensor::num_elements(tensor1);

  std::vector<const T *> all_input_data;
  for (int32_t i = 0; i < input_sizes; ++i)
  {
    auto input_index = cur_op->inputs()->operator[](i);
    const auto *tensor = runtime_graph->getCircleTensorByIndex(input_index);

    const auto *tensor_data = runtime_graph->getDataByTensor(tensor);
    if (tensor_data == nullptr)
      tensor_data = runtime_graph->getConstDataByTensor(tensor);

    auto *data = reinterpret_cast<const T *>(tensor_data);
    all_input_data.push_back(data);
  }

  auto *output_data = reinterpret_cast<T *>(runtime_graph->getDataByTensor(output));

  luci_interpreter_pal::AddN(flat_size, input_sizes, all_input_data.data(), output_data);
}

} // namespace

void configure_kernel_CircleAddN(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const int num_inputs = cur_op->inputs()->size();

  LUCI_INTERPRETER_CHECK(num_inputs >= 2);
  LUCI_INTERPRETER_CHECK(cur_op->outputs()->size() == 1);

  const auto input1_index = cur_op->inputs()->operator[](0);
  assert(input1_index != -1);

  const auto input1_tensor = runtime_graph->getCircleTensorByIndex(input1_index);
  assert(input1_tensor != nullptr);

  for (int i = 1; i < num_inputs; ++i)
  {
    const auto input_index = cur_op->inputs()->operator[](i);
    assert(input_index != -1);

    const auto input_tensor = runtime_graph->getCircleTensorByIndex(input_index);
    assert(input_tensor != nullptr);

    LUCI_INTERPRETER_CHECK(Tensor::element_type(input1_tensor) ==
                           Tensor::element_type(input_tensor));
    LUCI_INTERPRETER_CHECK(Tensor::num_dims(input1_tensor) == Tensor::num_dims(input_tensor));
    LUCI_INTERPRETER_CHECK(Tensor::num_elements(input1_tensor) ==
                           Tensor::num_elements(input_tensor));
  }
}

void execute_kernel_CircleAddN(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto output_index = cur_op->outputs()->operator[](0);
  assert(output_index != -1);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  switch (Tensor::element_type(output))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      evalGeneric<float>(cur_op, runtime_graph);
    }
    break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
