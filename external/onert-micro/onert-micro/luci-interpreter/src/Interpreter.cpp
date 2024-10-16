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

#include "luci_interpreter/Interpreter.h"

#include "loader/ModuleLoader.h"

namespace luci_interpreter
{

#ifdef USE_STATIC_ALLOC
// Construct static interpreter with configurations
Interpreter::Interpreter(const char *model_data_raw, const InterpreterConfigure &configuration)
{
  _runtime_module = std::make_unique<RuntimeModule>();

  _memory_manager = StaticMemoryManager(configuration._input_buf_size, configuration._temp_buf_size,
                                        configuration._output_buf_size)

    // Note:
    // configuration._input_buf_size, configuration._temp_buf_size, configuration._output_buf_size
    // will be removed and will be read from circle file
    if (configuration.isStaticManager())
  {
    _memory_manager = std::make_unique<StaticMemoryManager>(
      configuration._input_buf_size, configuration._temp_buf_size, configuration._output_buf_size);
  }
  else { _memory_manager = std::make_unique<SimpleMemoryManager>(); }

  _memory_manager->is_allocate_input(configuration.getAllocateInputValue());

  ModuleLoader loader();
  ModuleLoader::load(_runtime_module.get(), _memory_manager.get(),
                     /* is_static_allocations */ configuration.isStaticManager(), model_data_raw);

  ModuleLoader loader(_runtime_module.get(), _memory_manager.get());
  loader.load(configuration.isStaticManager(), model_data_raw);
}
#else

// Construct default interpreter with dynamic allocations and with input allocations
Interpreter::Interpreter(const char *model_data_raw, bool dealloc_input)
{
  ModuleLoader::load(&_runtime_module, &_memory_manager, model_data_raw, dealloc_input);
}

#endif // USE_STATIC_ALLOC

Interpreter::~Interpreter() = default;

void Interpreter::interpret() { _runtime_module.execute(); }

int32_t Interpreter::getNumOfInputTensors()
{
  auto *runtime_graph = _runtime_module.getMainGraph();
  return runtime_graph->getNumOfInputTensors();
}

int32_t Interpreter::getNumOfOutputTensors()
{
  auto *runtime_graph = _runtime_module.getMainGraph();
  return runtime_graph->getNumOfOutputTensors();
}

int32_t Interpreter::getInputDataSizeByIndex(int32_t input_tensor_index)
{
  auto *runtime_graph = _runtime_module.getMainGraph();

  return runtime_graph->getInputDataSizeByIndex(input_tensor_index);
}

int32_t Interpreter::getOutputDataSizeByIndex(int32_t output_tensor_index)
{
  auto *runtime_graph = _runtime_module.getMainGraph();

  return runtime_graph->getOutputDataSizeByIndex(output_tensor_index);
}

void Interpreter::allocateAndWriteInputTensor(int32_t input_tensor_index, const void *data,
                                              size_t data_size)
{
  assert(data_size > 0);
  assert(data != nullptr);
  assert(input_tensor_index >= 0);
  auto *runtime_graph = _runtime_module.getMainGraph();
  auto tensor_data = runtime_graph->configureGraphInput(input_tensor_index);

  std::memcpy(tensor_data, data, data_size);
}

uint8_t *Interpreter::allocateInputTensor(int32_t input_tensor_index)
{
  assert(input_tensor_index >= 0);

  auto *runtime_graph = _runtime_module.getMainGraph();

  return runtime_graph->configureGraphInput(input_tensor_index);
}

uint8_t *Interpreter::readOutputTensor(int32_t output_tensor_index)
{
  auto *runtime_graph = _runtime_module.getMainGraph();

  return runtime_graph->getOutputDataByIndex(output_tensor_index);
}

} // namespace luci_interpreter
