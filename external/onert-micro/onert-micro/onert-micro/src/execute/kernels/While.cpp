/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "execute/OMKernelExecutionBuilder.h"
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"
#include "core/OMUtils.h"
#include "core/OMDataType.h"
#include "execute/OMKernelExecute.h"

using namespace onert_micro;
using namespace onert_micro::execute;

// NOTE: doesnt currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleWhile(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeModule &runtime_module = execute_args.runtime_module;
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);
  auto options = runtime_kernel.first_operator->builtin_options_as_WhileOptions();

  // Obtain conditional and body runtime subgraphs
  const auto body_subgraph_index = options->body_subgraph_index();
  const auto cond_subgraph_index = options->cond_subgraph_index();
  core::OMRuntimeGraph *cond_runtime_graph = nullptr;
  core::OMRuntimeGraph *body_runtime_graph = nullptr;
  runtime_module.getRuntimeGraphAt(cond_subgraph_index, &cond_runtime_graph);
  runtime_module.getRuntimeGraphAt(body_subgraph_index, &body_runtime_graph);

  core::OMRuntimeContext &cond_runtime_context = cond_runtime_graph->getRuntimeContext();
  core::OMRuntimeStorage &cond_runtime_storage = cond_runtime_graph->getRuntimeStorage();
  core::memory::OMRuntimeAllocator &cond_runtime_allocator =
    cond_runtime_graph->getRuntimeAllocator();

  core::OMRuntimeContext &body_runtime_context = body_runtime_graph->getRuntimeContext();
  core::OMRuntimeStorage &body_runtime_storage = body_runtime_graph->getRuntimeStorage();
  core::memory::OMRuntimeAllocator &body_runtime_allocator =
    body_runtime_graph->getRuntimeAllocator();

  OMStatus status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
  if (status != Ok)
    return status;

  // Copy input data to the output
  assert(runtime_kernel.inputs_num == runtime_kernel.outputs_num);
  for (uint32_t i = 0; i < runtime_kernel.inputs_num; ++i)
  {
    const auto cur_input_tensor = runtime_kernel.inputs[i];
    const auto input_data_size = sizeof(core::OMDataType(cur_input_tensor->type())) *
                                 core::OMRuntimeShape(cur_input_tensor).flatSize();
    std::memcpy(runtime_kernel.outputs_data[i], runtime_kernel.inputs_data[i], input_data_size);
  }

  do
  {
    // Handle conditional graph
    {
      // Allocate cond graph inputs
      cond_runtime_graph->allocateGraphInputs();
      auto cond_graphs_inputs = cond_runtime_graph->getNumberOfInputs();
      for (uint32_t i = 0; i < cond_graphs_inputs; ++i)
      {
        auto *cur_cond_input_data =
          reinterpret_cast<uint8_t *>(cond_runtime_graph->getInputDataAt(i));
        uint8_t *cur_main_input_data = runtime_kernel.outputs_data[i];
        assert(cur_main_input_data != nullptr);
        assert(cur_cond_input_data != nullptr);
        const auto cur_input_tensor = runtime_kernel.inputs[i];
        const auto input_data_size = sizeof(core::OMDataType(cur_input_tensor->type())) *
                                     core::OMRuntimeShape(cur_input_tensor).flatSize();
        std::memcpy(cur_cond_input_data, cur_main_input_data, input_data_size);
      }
      // Run cond graph
      execute::OMExecuteArgs cond_execute_args = {cond_runtime_storage, cond_runtime_context, 0,
                                                  runtime_module};
      status = execute::OMKernelExecute::runForward(cond_execute_args, cond_runtime_allocator);
      if (status != Ok)
        return status;

      // Check cond graph result
      bool cond_result_value = reinterpret_cast<bool *>(cond_runtime_graph->getOutputDataAt(0))[0];
      // Reset cond graph values
      cond_runtime_graph->reset();
      // If false - then finish while loop
      if (cond_result_value == false)
        break;
    }

    // Handle body graph
    {
      // Allocate body graph inputs
      body_runtime_graph->allocateGraphInputs();
      // Copy data
      auto body_graphs_inputs = body_runtime_graph->getNumberOfInputs();
      for (uint32_t i = 0; i < body_graphs_inputs; ++i)
      {
        auto *cur_body_input_data =
          reinterpret_cast<uint8_t *>(body_runtime_graph->getInputDataAt(i));
        uint8_t *cur_main_input_data = runtime_kernel.outputs_data[i];
        assert(cur_main_input_data != nullptr);
        assert(cur_body_input_data != nullptr);
        const auto cur_input_tensor = runtime_kernel.inputs[i];
        const auto input_data_size = sizeof(core::OMDataType(cur_input_tensor->type())) *
                                     core::OMRuntimeShape(cur_input_tensor).flatSize();
        std::memcpy(cur_body_input_data, cur_main_input_data, input_data_size);
      }
      // Run body graph
      execute::OMExecuteArgs body_execute_args = {body_runtime_storage, body_runtime_context, 0,
                                                  runtime_module};
      status = execute::OMKernelExecute::runForward(body_execute_args, body_runtime_allocator);
      if (status != Ok)
        return status;

      // Copy body calculated data to the main output
      for (uint32_t i = 0; i < runtime_kernel.inputs_num; ++i)
      {
        auto cur_calculated_data = body_runtime_graph->getOutputDataAt(i);
        const auto cur_tensor = runtime_kernel.outputs[i];
        const auto data_size = sizeof(core::OMDataType(cur_tensor->type())) *
                               core::OMRuntimeShape(cur_tensor).flatSize();
        std::memcpy(runtime_kernel.outputs_data[i], cur_calculated_data, data_size);
      }

      body_runtime_graph->reset();
    }
  } while (true);

  return status;
}

} // namespace execute
} // namespace onert_micro
