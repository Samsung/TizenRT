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
#include "kernels/Utils.h"

#include <cstring>

namespace luci_interpreter
{

void configure_kernel_CircleWhile(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  auto *main_runtime_graph = runtime_graph;

  auto *runtime_module = runtime_graph->getRuntimeModule();

  const auto *options = cur_op->builtin_options_as_WhileOptions();
  const auto body_subgraph_index = options->body_subgraph_index();
  const auto cond_subgraph_index = options->cond_subgraph_index();

  auto *cond_runtime_graph = runtime_module->getRuntimeGraphAt(cond_subgraph_index);
  auto *body_runtime_graph = runtime_module->getRuntimeGraphAt(body_subgraph_index);

  body_runtime_graph->selectOwnSubgraph();
  const auto body_input_size = body_runtime_graph->getNumOfInputTensors();
  const auto body_output_size = body_runtime_graph->getNumOfOutputTensors();
  LUCI_INTERPRETER_CHECK(body_input_size == cur_op->inputs()->size());
  LUCI_INTERPRETER_CHECK(body_output_size == cur_op->outputs()->size());
  LUCI_INTERPRETER_CHECK(body_output_size == cur_op->inputs()->size());
  body_runtime_graph->invalidate();
  body_runtime_graph->configure(false);

  cond_runtime_graph->selectOwnSubgraph();
  const auto cond_input_size = cond_runtime_graph->getNumOfInputTensors();
  const auto cond_output_size = cond_runtime_graph->getNumOfOutputTensors();
  LUCI_INTERPRETER_CHECK(cond_input_size == cur_op->inputs()->size());
  LUCI_INTERPRETER_CHECK(cond_output_size == 1);
  const circle::Tensor *cond_output_tensor = cond_runtime_graph->getOutputTensorByIndex(0);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(cond_output_tensor) == DataType::BOOL);
  cond_runtime_graph->invalidate();
  cond_runtime_graph->configure(false);

  main_runtime_graph->selectOwnSubgraph();
}

void execute_kernel_CircleWhile(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  auto *main_runtime_graph = runtime_graph;
  auto *runtime_module = runtime_graph->getRuntimeModule();

  const auto input_size = cur_op->inputs()->size();

  std::vector<uint8_t *> operation_inputs_data(input_size);
  std::vector<uint8_t *> operation_outputs_data;

  std::vector<int32_t> input_sizes(input_size);

  bool is_inplace = runtime_graph->is_inplace_op(cur_op);

  for (int32_t i = 0; i < input_size; ++i)
  {
    const auto op_input_index = cur_op->inputs()->operator[](i);
    const auto op_output_index = cur_op->outputs()->operator[](i);
    assert(op_input_index != -1);
    assert(op_output_index != -1);
    const auto input = main_runtime_graph->getCircleTensorByIndex(op_input_index);
    const auto output = main_runtime_graph->getCircleTensorByIndex(op_output_index);

    input_sizes[i] = Tensor::num_elements(input) * size(Tensor::element_type(input));

    auto *input_data = main_runtime_graph->getDataByTensor(input);

    uint8_t *tensor_data = nullptr;
    if (is_inplace)
    {
      if (input_data == nullptr)
      {
        tensor_data = new uint8_t[input_sizes[i]];
        input_data = main_runtime_graph->getConstDataByTensor(input);
        assert(input_data != nullptr);
        if ( input_data != nullptr )
          std::memcpy(tensor_data, input_data, input_sizes[i]);
      }
      else
      {
        tensor_data = input_data;
      }
    }
    else
    {
      if (input_data == nullptr)
        input_data = main_runtime_graph->getConstDataByTensor(input);
      assert(input_data != nullptr);
      tensor_data = main_runtime_graph->getDataByTensor(output);
      assert(tensor_data != nullptr);
      if ( input_data != nullptr && tensor_data != nullptr )
        std::memcpy(tensor_data, input_data, input_sizes[i]);
    }
    assert(tensor_data != nullptr);

    operation_inputs_data[i] = tensor_data;
  }

  const auto *options = cur_op->builtin_options_as_WhileOptions();
  const auto body_subgraph_index = options->body_subgraph_index();
  const auto cond_subgraph_index = options->cond_subgraph_index();

  auto *cond_runtime_graph = runtime_module->getRuntimeGraphAt(cond_subgraph_index);
  auto *body_runtime_graph = runtime_module->getRuntimeGraphAt(body_subgraph_index);

  do
  {
    cond_runtime_graph->selectOwnSubgraph();

    for (int32_t i = 0; i < input_size; ++i)
      cond_runtime_graph->configureGraphInput(i, operation_inputs_data[i]);

    cond_runtime_graph->execute();

    bool cond_value = (cond_runtime_graph->getOutputDataByIndex(0))[0];
    if (!cond_value)
      break;

    body_runtime_graph->selectOwnSubgraph();
    for (int32_t i = 0; i < input_size; ++i)
      body_runtime_graph->configureGraphInput(i, operation_inputs_data[i]);

    body_runtime_graph->execute();

    for (int32_t i = 0; i < input_size; ++i)
    {
      auto cur_output_body_data = body_runtime_graph->getOutputDataByIndex(i);
      if (cur_output_body_data == nullptr)
        continue;
      std::memcpy(operation_inputs_data[i], cur_output_body_data, input_sizes[i]);
    }
  } while (true);

  cond_runtime_graph->resetOutputTensorsData();
  cond_runtime_graph->clearTensors();

  body_runtime_graph->selectOwnSubgraph();
  body_runtime_graph->resetOutputTensorsData();
  body_runtime_graph->clearTensors();

  main_runtime_graph->selectOwnSubgraph();

  if (is_inplace)
  {
    for (int32_t i = 0; i < input_size; ++i)
    {
      const auto op_input_index = cur_op->inputs()->operator[](i);
      const auto op_output_index = cur_op->outputs()->operator[](i);
      assert(op_input_index != -1);
      assert(op_output_index != -1);
      const auto input = main_runtime_graph->getCircleTensorByIndex(op_input_index);
      const auto output = main_runtime_graph->getCircleTensorByIndex(op_output_index);

      if (main_runtime_graph->getDataByTensor(input))
      {
        main_runtime_graph->makeInplaceOperation(input, output);
      }
      else
      {
        main_runtime_graph->setDataToTensor(output, operation_inputs_data[i]);
      }
    }
  }
}

} // namespace luci_interpreter
