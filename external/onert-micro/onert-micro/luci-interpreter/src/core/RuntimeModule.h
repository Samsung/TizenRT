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

#ifndef LUCI_INTERPRETER_CORE_RUNTIMEMODULE_H
#define LUCI_INTERPRETER_CORE_RUNTIMEMODULE_H

#include "core/RuntimeGraph.h"
#include "luci_interpreter/core/reader/CircleMicroReader.h"

#include <memory>
#include <vector>

namespace luci_interpreter
{

#ifdef USE_STATIC_ALLOC
using BaseRuntimeGraph = StaticRuntimeGraph;
using MemoryManager = StaticMemoryManager;
#else
using BaseRuntimeGraph = RuntimeGraph;
using MemoryManager = SimpleMemoryManager;
#endif // USE_STATIC_ALLOC

class RuntimeModule
{
public:
  RuntimeModule() = default;

  void addGraph(MemoryManager *memory_manager)
  {
    _graphs.emplace_back(memory_manager, &_circle_reader, this,
                         _circle_reader.get_current_subgraph_index());
  }

  BaseRuntimeGraph *getRuntimeGraphAt(uint32_t pos) { return &_graphs.at(pos); }

  void execute() { getMainGraph()->execute(); }

  CircleReader &getCircleReader() { return _circle_reader; }

  BaseRuntimeGraph *getMainGraph() const { return const_cast<BaseRuntimeGraph *>(&_graphs[0]); }

  void selectSubgraph(uint32_t index) { _circle_reader.select_subgraph(index); }

private:
  std::vector<BaseRuntimeGraph> _graphs;

  CircleReader _circle_reader;
};

} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_CORE_RUNTIMEMODULE_H
