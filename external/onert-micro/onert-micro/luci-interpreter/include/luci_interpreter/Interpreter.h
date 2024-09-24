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

#ifndef LUCI_INTERPRETER_INTERPRETER_H
#define LUCI_INTERPRETER_INTERPRETER_H

#include "luci_interpreter/core/Tensor.h"

#ifdef USE_STATIC_ALLOC
#include "luci_interpreter/InterpreterConfigure.h"
#include "memory_managers/StaticMemoryManager.h"
#else
#include "memory_managers/SimpleMemoryManager.h"
#endif // USE_STATIC_ALLOC

#include "loader/ModuleLoader.h"
#include <memory>

namespace luci_interpreter
{

class Interpreter
{
public:
  // Construct default interpreter with dynamic allocations and with input allocations
  explicit Interpreter(const char *model_data_raw, bool dealloc_input);

#ifdef USE_STATIC_ALLOC
  // Construct interpreter with configurations
  explicit Interpreter(const char *model_data_raw, const InterpreterConfigure &configuration);
#endif // USE_STATIC_ALLOC

  ~Interpreter();

  void allocateAndWriteInputTensor(int32_t input_tensor_index, const void *data, size_t data_size);
  uint8_t *allocateInputTensor(int32_t input_tensor_index);

  uint8_t *readOutputTensor(int32_t output_tensor_index);

  int32_t getNumOfInputTensors();
  int32_t getNumOfOutputTensors();
  int32_t getInputDataSizeByIndex(int32_t input_tensor_index);
  int32_t getOutputDataSizeByIndex(int32_t output_tensor_index);

  void interpret();

private:
  // _default_memory_manager should be before _runtime_module due to
  // the order of deletion in the destructor
  MemoryManager _memory_manager{};
  RuntimeModule _runtime_module{};
};

} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_INTERPRETER_H
