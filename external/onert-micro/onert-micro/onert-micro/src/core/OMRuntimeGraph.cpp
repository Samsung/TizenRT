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

#include "core/OMRuntimeGraph.h"
#include "core/OMDataType.h"
#include "core/memory/OMMemoryManager.h"
#include "OMStatus.h"

using namespace onert_micro::core;
using namespace onert_micro;

OMStatus OMRuntimeGraph::reset()
{
  OMStatus status = _allocator.clearAllTensorsData(&_context, &_storage);
  _storage.clearTensorIndexToData();

  return status;
}

OMRuntimeGraph::~OMRuntimeGraph()
{
  reset();
#ifdef OM_MEMORY_ESTIMATE
  memory::OMMemoryManager::cur_memory_allocated = 0;
  memory::OMMemoryManager::peak_memory_allocated = 0;
#endif // OM_MEMORY_ESTIMATE
}

void *OMRuntimeGraph::getInputDataAt(uint32_t position)
{
  const auto input_index = _context.getGraphInputTensorIndex(position);

  uint8_t *data;
  _storage.getDataByTensorIndex(&data, input_index);

  if (data == nullptr)
  {
    _allocator.allocateGraphInputs(&_context, &_storage);
    _storage.getDataByTensorIndex(&data, input_index);
  }

  return reinterpret_cast<void *>(data);
}

void *OMRuntimeGraph::getOutputDataAt(uint32_t position)
{
  const auto output_index = _context.getGraphOutputTensorIndex(position);

  uint8_t *data;
  _storage.getDataByTensorIndex(&data, output_index);
  return reinterpret_cast<void *>(data);
}

OMStatus OMRuntimeGraph::allocateGraphInputs()
{
  return _allocator.allocateGraphInputs(&_context, &_storage);
}

uint32_t OMRuntimeGraph::getOutputSizeAt(uint32_t position)
{
  const auto output_index = _context.getGraphOutputTensorIndex(position);
  const circle::Tensor *output_tensor = _context.getTensorByIndex(output_index);

  OMRuntimeShape shape(output_tensor);
  return shape.flatSize();
}

size_t OMRuntimeGraph::getInputDataTypeSize(uint32_t position)
{
  const auto input_index = _context.getGraphInputTensorIndex(position);
  const circle::Tensor *input_tensor = _context.getTensorByIndex(static_cast<int32_t>(input_index));

  auto type = input_tensor->type();
  return sizeof(OMDataType(type));
}

size_t OMRuntimeGraph::getOutputDataTypeSize(uint32_t position)
{
  const auto output_index = _context.getGraphOutputTensorIndex(position);
  const circle::Tensor *output_tensor =
    _context.getTensorByIndex(static_cast<int32_t>(output_index));

  auto type = output_tensor->type();
  return sizeof(OMDataType(type));
}

uint32_t OMRuntimeGraph::getInputSizeAt(uint32_t position)
{
  const auto input_index = _context.getGraphInputTensorIndex(position);
  const circle::Tensor *input_tensor = _context.getTensorByIndex(input_index);

  OMRuntimeShape shape(input_tensor);
  return shape.flatSize();
}

uint32_t OMRuntimeGraph::getNumberOfOutputs()
{
  auto *outputs = _context.getCircleOutputs();
  if (outputs == nullptr)
    return 0;

  return outputs->size();
}

uint32_t OMRuntimeGraph::getNumberOfInputs()
{
  auto *inputs = _context.getCircleInputs();
  if (inputs == nullptr)
    return 0;

  return inputs->size();
}
