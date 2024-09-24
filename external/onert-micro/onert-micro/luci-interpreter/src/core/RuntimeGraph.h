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

#ifndef LUCI_INTERPRETER_CORE_RUNTIMEGRAPH_H
#define LUCI_INTERPRETER_CORE_RUNTIMEGRAPH_H

#include "luci_interpreter/core/Tensor.h"
#ifdef USE_STATIC_ALLOC
#include "memory_managers/StaticMemoryManager.h"
#else
#include "memory_managers/SimpleMemoryManager.h"
#endif // USE_STATIC_ALLOC

#include "luci_interpreter/core/reader/CircleMicroReader.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace luci_interpreter
{

class RuntimeModule;

#ifdef USE_STATIC_ALLOC
// TODO: Enable it
#if 0
class StaticRuntimeGraph final : public IBaseRuntimeGraph
{
public:
  explicit StaticRuntimeGraph(IMemoryManager *memory_manager, CircleReader *circle_reader);
  ~StaticRuntimeGraph() final;

  void configureGraphInputs() final;
  void execute() final;
  void configure() final;

  void configure_kernels() final;
};
#endif
#else

class RuntimeGraph
{
public:
  RuntimeGraph() = delete;

  explicit RuntimeGraph(SimpleMemoryManager *memory_manager, CircleReader *circle_reader,
                        RuntimeModule *runtime_module, uint32_t subgraph_index);
  ~RuntimeGraph();

  Tensor *addTensor(const circle::Tensor *raw_tensor, std::unique_ptr<Tensor> &&tensor);

  const circle::Tensor *getCircleTensorByIndex(int32_t index);

  void makeInplaceOperation(const circle::Tensor *src_tensor, const circle::Tensor *dst_tensor);

  uint8_t *getDataByTensor(const circle::Tensor *raw_tensor);
  uint8_t *getConstDataByTensor(const circle::Tensor *raw_tensor);

  uint8_t *configureGraphInput(int32_t input_index);
  void configureGraphInput(int32_t input_index, uint8_t *data);

  int32_t getInputDataSizeByIndex(int32_t input_index);
  int32_t getOutputDataSizeByIndex(int32_t output_index);

  int32_t getNumOfInputTensors();
  int32_t getNumOfOutputTensors();

  const circle::Tensor *getInputTensorByIndex(int32_t input_index);
  const circle::Tensor *getOutputTensorByIndex(int32_t input_index);

  uint8_t *getOutputDataByIndex(int32_t output_index);

  void addInplaceOpIndex(const circle::Operator *op) { _inplace_op_indexes.insert(op); }

  void execute();
  void configure(bool dealloc_input);

  void invalidate() { _is_valid = false; }
  bool isValid() const { return _is_valid; }

  void selectOwnSubgraph() { _reader->select_subgraph(_subgraph_index); };
  void resetOutputTensorsData();

  void clearTensors();

  void setDataToTensor(const circle::Tensor *tensor, uint8_t *data);

  void resetTensorData(uint8_t *new_data, const circle::Tensor *tensor);

  RuntimeModule *getRuntimeModule() { return _runtime_module; };

  bool is_inplace_op(const circle::Operator *op)
  {
    return _inplace_op_indexes.find(op) != _inplace_op_indexes.end();
  }

#ifndef DIS_DYN_SHAPES
  void addDynamicShapeTensor(const circle::Tensor *tensor, luci_interpreter::RuntimeShape &&shapes);

  luci_interpreter::RuntimeShape *getDynamicShapeTensor(const circle::Tensor *tensor);

  void removeDynamicShapeTensor(const circle::Tensor *tensor);
#endif // DIS_DYN_SHAPES

private:
  void buildAllocDeallocPlan(bool dealloc_input);
  void allocate(size_t kernel_index);
  void deallocate(size_t kernel_index);

private:
  SimpleMemoryManager *_memory_manager;
  CircleReader *_reader;
  RuntimeModule *_runtime_module;

  std::unordered_map<const circle::Tensor *, uint8_t *> _tensor_to_data;
  std::unordered_set<const circle::Operator *> _inplace_op_indexes;

  bool _is_valid = false;

  // Tensors that are not used anymore after given op
  std::vector<std::vector<const circle::Tensor *>> _alloc_plan;
  std::vector<std::vector<const circle::Tensor *>> _dealloc_plan;

  uint32_t _subgraph_index;

#ifndef DIS_DYN_SHAPES
  std::unordered_map<const circle::Tensor *, luci_interpreter::RuntimeShape> _dynamic_tensor_shapes;
#endif // DIS_DYN_SHAPES
};

#endif // USE_STATIC_ALLOC

} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_CORE_RUNTIMEGRAPH_H
