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

#include "core/OMRuntimeModule.h"
#include "optimize/OMOptimizer.h"
#include "import/OMExecutionPlanCreator.h"
#include "import/OMKernelConfiguration.h"
#include "import/OMConfigureArgs.h"
#include "execute/OMKernelExecute.h"

using namespace onert_micro::core;
using namespace onert_micro;

uint32_t OMRuntimeModule::getNumberOfInputs() { return _graphs.at(0).getNumberOfInputs(); }

uint32_t OMRuntimeModule::getNumberOfOutputs() { return _graphs.at(0).getNumberOfOutputs(); }

uint32_t OMRuntimeModule::getInputSizeAt(uint32_t position)
{
  return _graphs.at(0).getInputSizeAt(position);
}

uint32_t OMRuntimeModule::getOutputSizeAt(uint32_t position)
{
  return _graphs.at(0).getOutputSizeAt(position);
}

void *OMRuntimeModule::getInputDataAt(uint32_t position)
{
  return _graphs.at(0).getInputDataAt(position);
}

void *OMRuntimeModule::getOutputDataAt(uint32_t position)
{
  return _graphs.at(0).getOutputDataAt(position);
}

OMStatus OMRuntimeModule::importModel(const char *model_ptr, const OMConfig &config)
{
  assert(model_ptr != nullptr && "Model ptr shouldn't be nullptr");
  if (model_ptr == nullptr)
    return UnknownError;

  // 1 - parse reader
  // 2 - load default graph
  // 3 - optimize it until can
  // 4 - AllocDeallocPlan creation
  // 5 - KernelConfigure
  // 6 - Allocate inputs

  OMStatus status;
  // First - parse reader
  uint32_t num_subgraph = 0;
  {
    reader::OMCircleReader reader;
    status = reader.parse(model_ptr);
    if (status != Ok)
      return status;
    num_subgraph = reader.num_subgraph();
  }

  assert(num_subgraph >= 1);
  if (num_subgraph == 0)
    return UnknownError;

  _graphs.resize(num_subgraph);

  for (uint32_t i = 0; i < num_subgraph; ++i)
  {
    // Second - load default graph
    OMRuntimeGraph &graph = _graphs.at(i);

    OMRuntimeContext &runtime_context = graph.getRuntimeContext();
    OMRuntimeStorage &runtime_storage = graph.getRuntimeStorage();
    memory::OMRuntimeAllocator &runtime_allocator = graph.getRuntimeAllocator();

    runtime_context.setModel(model_ptr, i);

    // Parse and validate WOF file if it is exist
    // WARNING: setWofFile method of RuntimeContext should follow after setModel.
    if (config.wof_ptr != nullptr)
      runtime_context.setWofFile(config.wof_ptr);

    // Parse and validate Train Config File if it is exists
    // WARNING: setTrainConfigFile method of RuntimeContext should follow after setModel.
    if (config.train_mode and config.training_context.training_config_info_data != nullptr)
      runtime_context.setTrainConfigFile(config.training_context.training_config_info_data);

    // Third - optimize it until can
    status = optimize::OMOptimizer::optimize(runtime_storage, runtime_context, config);
    if (status != Ok)
      return status;

    // 4 - AllocDeallocPlan creation
    if (not config.train_mode)
    {
      // Non trainable mode
      status = import::OMExecutionPlanCreator::createExecutionPlan(runtime_storage, runtime_context,
                                                                   runtime_allocator, config);
    }
    else
    {
      // Trainable mode
      status = import::OMExecutionPlanCreator::createForwardExecutionPlan(
        runtime_storage, runtime_context, runtime_allocator, config);
    }
    if (status != Ok)
      return status;
  }
  for (uint32_t i = 0; i < num_subgraph; ++i)
  {
    // Second - load default graph
    OMRuntimeGraph &graph = _graphs.at(i);

    OMRuntimeContext &runtime_context = graph.getRuntimeContext();
    OMRuntimeStorage &runtime_storage = graph.getRuntimeStorage();
    memory::OMRuntimeAllocator &runtime_allocator = graph.getRuntimeAllocator();
    // 5 - KernelConfigure
    import::OMConfigureArgs configure_args = {runtime_storage, runtime_context, 0, config, *this};

    status = import::OMKernelConfiguration::configureKernels(configure_args);
    if (status != Ok)
      return status;
  }
  // Done!

  return Ok;
}

OMStatus OMRuntimeModule::allocateInputs()
{
  assert(_graphs.size() > 0);
  if (_graphs.size() == 0)
    return ModelNotImport;
  return _graphs.at(0).allocateGraphInputs();
}

OMStatus OMRuntimeModule::run(const OMConfig &config)
{
  OMStatus status = Ok;

  if (_graphs.empty())
    return ModelNotImport;

  core::OMRuntimeGraph &main_graph = _graphs.at(0);

  execute::OMExecuteArgs execute_args = {main_graph.getRuntimeStorage(),
                                         main_graph.getRuntimeContext(),
                                         0,
                                         *this,
                                         config.training_context.num_of_train_layers,
                                         config.train_mode};

  status = execute::OMKernelExecute::runForward(execute_args, main_graph.getRuntimeAllocator());
  if (status != Ok)
    return status;

  return status;
}

OMStatus OMRuntimeModule::reset()
{
  OMStatus status = Ok;

  if (_graphs.empty())
    return ModelNotImport;

  for (auto &graph : _graphs)
  {
    graph.reset();
  }

  return status;
}

OMStatus OMRuntimeModule::getRuntimeGraphAt(uint32_t pos, OMRuntimeGraph **runtime_graph)
{
  if (pos >= _graphs.size())
    return UnknownError;

  *runtime_graph = &_graphs.at(pos);

  return Ok;
}
