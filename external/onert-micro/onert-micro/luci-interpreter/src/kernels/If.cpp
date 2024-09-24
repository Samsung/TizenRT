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

#include <cstring>

namespace luci_interpreter
{

void configure_kernel_CircleIf(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  auto *main_runtime_graph = runtime_graph;

  auto *runtime_module = runtime_graph->getRuntimeModule();

  const auto *options = cur_op->builtin_options_as_IfOptions();

  const auto cond_index = cur_op->inputs()->operator[](0);
  const auto output_index = cur_op->outputs()->operator[](0);

  const auto then_subgraph_index = options->then_subgraph_index();
  const auto else_subgraph_index = options->else_subgraph_index();

  assert(cond_index != -1);
  assert(output_index != -1);

  assert(then_subgraph_index != -1);
  assert(else_subgraph_index != -1);

  const auto cond = runtime_graph->getCircleTensorByIndex(cond_index);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(cond) == DataType::BOOL);
  LUCI_INTERPRETER_CHECK(Tensor::num_elements(cond) == 1);

  const auto output = runtime_graph->getCircleTensorByIndex(output_index);
  auto *then_subgraph = runtime_module->getRuntimeGraphAt(then_subgraph_index);
  auto *else_subgraph = runtime_module->getRuntimeGraphAt(else_subgraph_index);
  for (RuntimeGraph *graph : {then_subgraph, else_subgraph})
  {
    graph->selectOwnSubgraph();
    const auto graph_input_size = graph->getNumOfInputTensors();
    const auto graph_output_size = graph->getNumOfOutputTensors();
    LUCI_INTERPRETER_CHECK(graph_input_size == cur_op->inputs()->size() - 1);
    LUCI_INTERPRETER_CHECK(graph_output_size == cur_op->outputs()->size());
    graph->invalidate();
    graph->configure(false);
  }
  main_runtime_graph->selectOwnSubgraph();
}

void execute_kernel_CircleIf(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  auto *main_runtime_graph = runtime_graph;
  auto *runtime_module = runtime_graph->getRuntimeModule();

  const auto input_size = cur_op->inputs()->size() - 1;
  const auto output_size = cur_op->outputs()->size();

  std::vector<uint8_t *> operation_inputs_data(input_size);
  std::vector<uint8_t *> operation_outputs_data(output_size);

  std::vector<int32_t> input_sizes(input_size);
  std::vector<int32_t> output_sizes(output_size);

  const auto *options = cur_op->builtin_options_as_IfOptions();
  const auto cond_index = cur_op->inputs()->operator[](0);

  const auto then_subgraph_index = options->then_subgraph_index();
  const auto else_subgraph_index = options->else_subgraph_index();

  auto *then_subgraph = runtime_module->getRuntimeGraphAt(then_subgraph_index);
  auto *else_subgraph = runtime_module->getRuntimeGraphAt(else_subgraph_index);

  const auto cond = runtime_graph->getCircleTensorByIndex(cond_index);

  const uint8_t *cond_data = runtime_graph->getDataByTensor(cond);
  const bool cond_value = kernels::getTensorData<bool>(cond_data)[0];

  RuntimeGraph *active_graph = cond_value ? then_subgraph : else_subgraph;

  for (int32_t i = 0; i < input_size; ++i)
  {
    const auto op_input_index = cur_op->inputs()->operator[](i + 1);
    assert(op_input_index != -1);
    const auto input = main_runtime_graph->getCircleTensorByIndex(op_input_index);
    input_sizes[i] = Tensor::num_elements(input) * size(Tensor::element_type(input));

    auto *input_data = main_runtime_graph->getDataByTensor(input);

    uint8_t *tensor_data = nullptr;
    if (input_data == nullptr)
      input_data = main_runtime_graph->getConstDataByTensor(input);
    assert(input_data != nullptr);
    tensor_data = main_runtime_graph->getDataByTensor(input);
    assert(tensor_data != nullptr);

    operation_inputs_data[i] = tensor_data;
  }
  for (int32_t i = 0; i < output_size; ++i)
  {
    const auto op_output_index = cur_op->outputs()->operator[](i);
    assert(op_output_index != -1);
    const auto output = main_runtime_graph->getCircleTensorByIndex(op_output_index);
    output_sizes[i] = Tensor::num_elements(output) * size(Tensor::element_type(output));

    auto *output_data = main_runtime_graph->getDataByTensor(output);

    uint8_t *tensor_data = nullptr;
    if (output_data == nullptr)
      output_data = main_runtime_graph->getConstDataByTensor(output);
    assert(output_data != nullptr);
    tensor_data = main_runtime_graph->getDataByTensor(output);
    assert(tensor_data != nullptr);

    operation_outputs_data[i] = tensor_data;
  }
  active_graph->selectOwnSubgraph();
  for (int32_t i = 0; i < input_size; ++i)
    active_graph->configureGraphInput(i, operation_inputs_data[i]);
  active_graph->execute();

  for (int32_t i = 0; i < output_size; ++i)
  {
    auto cur_output_active_data = active_graph->getOutputDataByIndex(i);
    if (cur_output_active_data == nullptr)
      continue;
    std::memcpy(operation_outputs_data[i], cur_output_active_data, output_sizes[i]);
  }
  active_graph->resetOutputTensorsData();
  active_graph->clearTensors();
  main_runtime_graph->selectOwnSubgraph();
}

} // namespace luci_interpreter
