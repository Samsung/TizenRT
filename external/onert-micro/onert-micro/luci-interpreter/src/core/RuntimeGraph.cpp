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

#include "core/RuntimeGraph.h"
#include "kernels/KernelBuilder.h"

#include <algorithm>
#include <map>

namespace luci_interpreter
{

// IBaseRuntimeGraph
RuntimeGraph::RuntimeGraph(SimpleMemoryManager *memory_manager, CircleReader *circle_reader,
                           RuntimeModule *runtime_module, uint32_t subgraph_index)
  : _memory_manager(memory_manager),
    _tensor_to_data(std::unordered_map<const circle::Tensor *, uint8_t *>{}),
    _runtime_module(runtime_module), _reader(circle_reader),
    _inplace_op_indexes(std::unordered_set<const circle::Operator *>{}),
    _subgraph_index(subgraph_index)
{
}

RuntimeGraph::~RuntimeGraph()
{
  for (auto &idx_to_tensor : _tensor_to_data)
  {
    auto *data = idx_to_tensor.second;

    _memory_manager->release_memory(data);
  }
}

// TODO: modify this
void RuntimeGraph::buildAllocDeallocPlan(bool dealloc_input)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  invalidate();
  using Lifetime = std::pair<int32_t, int32_t>;
  std::map<const circle::Tensor *, Lifetime> lifetimes;
  const size_t num_kernels = _reader->operators().size();

  if (dealloc_input)
  {
    for (const auto input_ind : _reader->inputs())
    {
      const auto raw_tensor = _reader->tensors()[input_ind];

      assert(lifetimes.count(raw_tensor) == 0);
      lifetimes[raw_tensor] = Lifetime(-1, 0);
    }
  }

  for (int32_t index = 0; index < num_kernels; ++index)
  {
    const auto kernel = _reader->operators().at(index);
    assert(kernel != nullptr);

    for (int32_t j = 0; j < kernel->inputs()->size(); ++j)
    {
      const auto input_index = kernel->inputs()->operator[](j);

      if (input_index == -1)
        continue;

      const auto raw_tensor = _reader->tensors()[input_index];

      // Pass constant tensors
      if (Tensor::is_constant_tensor(_reader, raw_tensor))
        continue;

      if (lifetimes.count(raw_tensor) > 0)
      {
        if (_inplace_op_indexes.find(kernel) != _inplace_op_indexes.end())
          lifetimes.at(raw_tensor).second = -1;
        else
          lifetimes.at(raw_tensor).second = index;
      }
    }

    for (int32_t j = 0; j < kernel->outputs()->size(); ++j)
    {
      const auto output_index = kernel->outputs()->operator[](j);
      const auto raw_tensor = _reader->tensors()[output_index];

      assert(lifetimes.count(raw_tensor) == 0);
      if (_inplace_op_indexes.find(kernel) != _inplace_op_indexes.end())
        lifetimes[raw_tensor] = Lifetime(-1, index);
      else
        lifetimes[raw_tensor] = Lifetime(index, index);
    }
  }

  for (const auto output_ind : _reader->outputs())
  {
    const auto raw_tensor = _reader->tensors()[output_ind];

    if (lifetimes.count(raw_tensor) > 0)
      lifetimes.at(raw_tensor).second = num_kernels;
  }

  _alloc_plan.assign(num_kernels, std::vector<const circle::Tensor *>());
  _dealloc_plan.assign(num_kernels + 1, std::vector<const circle::Tensor *>());
  for (const auto &item : lifetimes)
  {
    if (item.second.first != -1)
      _alloc_plan[item.second.first].push_back(item.first);
    if (item.second.second != -1)
      _dealloc_plan[item.second.second].push_back(item.first);
  }
  _is_valid = true;
}

void RuntimeGraph::allocate(size_t kernel_index)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  assert(_is_valid && kernel_index < _alloc_plan.size());
  for (const circle::Tensor *tensor : _alloc_plan[kernel_index])
  {
    if (_tensor_to_data.find(tensor) != _tensor_to_data.end())
    {
      auto *data = _tensor_to_data.at(tensor);
      _memory_manager->release_memory(data);
    }
    auto *data = _memory_manager->allocate_memory(tensor);
    _tensor_to_data[tensor] = data;
  }
}

#ifndef DIS_DYN_SHAPES
void RuntimeGraph::addDynamicShapeTensor(const circle::Tensor *tensor,
                                         luci_interpreter::RuntimeShape &&shapes)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  _dynamic_tensor_shapes[tensor] = std::move(shapes);
}

luci_interpreter::RuntimeShape *RuntimeGraph::getDynamicShapeTensor(const circle::Tensor *tensor)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  auto it = _dynamic_tensor_shapes.find(tensor);

  return it == _dynamic_tensor_shapes.end() ? nullptr : &_dynamic_tensor_shapes[tensor];
}

void RuntimeGraph::removeDynamicShapeTensor(const circle::Tensor *tensor)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  auto it = _dynamic_tensor_shapes.find(tensor);

  assert(it != _dynamic_tensor_shapes.end());

  _dynamic_tensor_shapes.erase(it);
}

#endif // DIS_DYN_SHAPES

void RuntimeGraph::deallocate(size_t kernel_index)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  assert(_is_valid && kernel_index < _dealloc_plan.size());
  for (const circle::Tensor *tensor : _dealloc_plan[kernel_index])
  {
    const auto it = _tensor_to_data.find(tensor);
    assert(it != _tensor_to_data.end());

    auto *data = _tensor_to_data.at(tensor);
    _memory_manager->release_memory(data);

    _tensor_to_data.erase(it);
  }
}

void RuntimeGraph::resetTensorData(uint8_t *new_data, const circle::Tensor *tensor)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  auto tensor_it = _tensor_to_data.find(tensor);
  if (tensor_it != _tensor_to_data.end())
  {
    auto *data = _tensor_to_data.at(tensor);
    _memory_manager->release_memory(data);
  }

  _tensor_to_data[tensor] = new_data;
}

void RuntimeGraph::resetOutputTensorsData()
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  const auto graph_inputs = _reader->inputs();
  for (int i = 0; i < _reader->outputs().size(); ++i)
  {
    const auto tensor_index = _reader->outputs()[i];
    assert(tensor_index != -1);

    if (std::find(graph_inputs.begin(), graph_inputs.end(), tensor_index) != graph_inputs.end())
      return;

    const auto tensor = _reader->tensors()[tensor_index];
    assert(tensor != nullptr);

    auto tensor_it = _tensor_to_data.find(tensor);
    if (tensor_it != _tensor_to_data.end())
    {
      auto *data = _tensor_to_data.at(tensor);
      _memory_manager->release_memory(data);
      _tensor_to_data.erase(tensor_it);
    }
  }
}

uint8_t *RuntimeGraph::configureGraphInput(int32_t input_index)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);

  const auto tensor_index = _reader->inputs()[input_index];
  assert(tensor_index != -1);
  const auto tensor = _reader->tensors()[tensor_index];
  assert(tensor != nullptr);

  auto *data = _memory_manager->allocate_memory(tensor);
  configureGraphInput(input_index, data);

  return data;
}

void RuntimeGraph::configureGraphInput(int32_t input_index, uint8_t *data)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  resetOutputTensorsData();

  const auto tensor_index = _reader->inputs()[input_index];
  assert(tensor_index != -1);
  const auto tensor = _reader->tensors()[tensor_index];
  assert(tensor != nullptr);

  if (_tensor_to_data.find(tensor) != _tensor_to_data.end())
  {
    auto *data_prev = _tensor_to_data.at(tensor);
    if (data_prev != data)
      _memory_manager->release_memory(data_prev);
  }
  _tensor_to_data[tensor] = data;
}

int32_t RuntimeGraph::getInputDataSizeByIndex(int32_t input_index)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  const auto tensor_index = _reader->inputs()[input_index];
  assert(tensor_index != -1);
  const auto tensor = _reader->tensors()[tensor_index];
  assert(tensor != nullptr);

  return Tensor::num_elements(tensor) * size(Tensor::element_type(tensor));
}

int32_t RuntimeGraph::getNumOfInputTensors()
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  return _reader->inputs().size();
}

int32_t RuntimeGraph::getNumOfOutputTensors()
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  return _reader->outputs().size();
}

const circle::Tensor *RuntimeGraph::getInputTensorByIndex(int32_t input_index)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);

  const auto tensor_index = _reader->inputs()[input_index];
  const auto tensor = _reader->tensors()[tensor_index];
  assert(tensor != nullptr);
  return tensor;
}

const circle::Tensor *RuntimeGraph::getOutputTensorByIndex(int32_t input_index)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);

  const auto tensor_index = _reader->outputs()[input_index];
  const auto tensor = _reader->tensors()[tensor_index];
  assert(tensor != nullptr);
  return tensor;
}

int32_t RuntimeGraph::getOutputDataSizeByIndex(int32_t output_index)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);

  const auto tensor_index = _reader->outputs()[output_index];
  assert(tensor_index != -1);
  const auto tensor = _reader->tensors()[tensor_index];
  assert(tensor != nullptr);

  return Tensor::num_elements(tensor) * size(Tensor::element_type(tensor));
}

uint8_t *RuntimeGraph::getOutputDataByIndex(int32_t output_index)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);

  const auto tensor_index = _reader->outputs()[output_index];
  assert(tensor_index != -1);
  const auto tensor = _reader->tensors()[tensor_index];
  assert(tensor != nullptr);

  assert(_tensor_to_data.find(tensor) != _tensor_to_data.end());

  return _tensor_to_data[tensor];
}

uint8_t *RuntimeGraph::getDataByTensor(const circle::Tensor *raw_tensor)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);

  if (raw_tensor == nullptr)
    return nullptr;

  if (_tensor_to_data.find(raw_tensor) == _tensor_to_data.end())
  {
    return nullptr;
  }

  return _tensor_to_data.at(raw_tensor);
}

void RuntimeGraph::clearTensors() { _tensor_to_data.clear(); }

void RuntimeGraph::makeInplaceOperation(const circle::Tensor *removing_tensor,
                                        const circle::Tensor *dst_tensor)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  assert(removing_tensor != nullptr);

  auto src_it = _tensor_to_data.find(removing_tensor);

  if (src_it == _tensor_to_data.end())
    return;

  auto *data = _tensor_to_data[removing_tensor];

  _tensor_to_data.erase(src_it);

  if (dst_tensor == nullptr)
  {
    delete[] data;
    return;
  }

  assert(_tensor_to_data.find(dst_tensor) == _tensor_to_data.end() &&
         "Failed makeInplaceOperation");
  _tensor_to_data[dst_tensor] = data;
}

uint8_t *RuntimeGraph::getConstDataByTensor(const circle::Tensor *raw_tensor)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  if (raw_tensor == nullptr)
    return nullptr;

  auto const &buffer = wrap(_reader->buffers()[raw_tensor->buffer()]->data());

  return const_cast<uint8_t *>(buffer.data());
}

const circle::Tensor *RuntimeGraph::getCircleTensorByIndex(int32_t index)
{
  assert(_reader->get_current_subgraph_index() == _subgraph_index);
  if (index < 0)
    return nullptr;

  const auto raw_tensor = _reader->tensors()[index];

  return raw_tensor;
}

void RuntimeGraph::configure(bool dealloc_input)
{
  selectOwnSubgraph();

  for (uint32_t i = 0; i < _reader->operators().size(); ++i)
  {
    const auto op = _reader->operators().at(i);
    assert(op != nullptr);

    const auto opcode = _reader->builtin_code(op);

    kernel_configure.configure_kernel(op, opcode, this);
  }

  if (not _is_valid)
    buildAllocDeallocPlan(dealloc_input);

  _is_valid = true;
}

void RuntimeGraph::setDataToTensor(const circle::Tensor *tensor, uint8_t *data)
{
  _tensor_to_data[tensor] = data;
}

void RuntimeGraph::execute()
{
  selectOwnSubgraph();

  if (not _is_valid)
    configure(true);

  const auto operators_size = _reader->operators().size();
  const auto operators = _reader->operators();

  for (uint32_t i = 0; i < operators_size; ++i)
  {
    const auto op = operators.at(i);
    assert(op != nullptr);

    const auto opcode = _reader->builtin_code(op);

    allocate(i);

    kernel_executor.execute_kernel(op, opcode, this);

    deallocate(i);
  }
}

} // namespace luci_interpreter
