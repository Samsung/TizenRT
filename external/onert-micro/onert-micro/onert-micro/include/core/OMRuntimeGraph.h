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

#ifndef ONERT_MICRO_CORE_RUNTIME_GRAPH_H
#define ONERT_MICRO_CORE_RUNTIME_GRAPH_H

#include "OMStatus.h"

#include "OMRuntimeContext.h"
#include "OMRuntimeStorage.h"
#include "memory/OMRuntimeAllocator.h"

#include <cstdint>

namespace onert_micro
{
namespace core
{

class OMRuntimeGraph
{
private:
  OMRuntimeContext _context{};
  OMRuntimeStorage _storage{};
  memory::OMRuntimeAllocator _allocator{};

public:
  OMRuntimeGraph() = default;
  OMRuntimeGraph(const OMRuntimeGraph &) = delete;
  OMRuntimeGraph(OMRuntimeGraph &&) = default;
  OMRuntimeGraph &operator=(const OMRuntimeGraph &) = delete;
  OMRuntimeGraph &&operator=(const OMRuntimeGraph &&) = delete;
  ~OMRuntimeGraph();

  uint32_t getNumberOfInputs();
  uint32_t getNumberOfOutputs();

  uint32_t getInputSizeAt(uint32_t position);
  uint32_t getOutputSizeAt(uint32_t position);

  void *getInputDataAt(uint32_t position);
  void *getOutputDataAt(uint32_t position);

  size_t getInputDataTypeSize(uint32_t position);
  size_t getOutputDataTypeSize(uint32_t position);

  OMStatus allocateGraphInputs();

  OMStatus reset();

  OMRuntimeStorage &getRuntimeStorage() { return _storage; }

  memory::OMRuntimeAllocator &getRuntimeAllocator() { return _allocator; }

  OMRuntimeContext &getRuntimeContext() { return _context; }
};

} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_RUNTIME_GRAPH_H
