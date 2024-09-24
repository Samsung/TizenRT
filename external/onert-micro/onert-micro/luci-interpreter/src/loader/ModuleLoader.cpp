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

#include "ModuleLoader.h"

#include "GraphLoader.h"

namespace luci_interpreter
{

void ModuleLoader::load(RuntimeModule *runtime_module, SimpleMemoryManager *memory_manager,
                        const char *model_data_raw, bool dealloc_input)
{
  const circle::Model *model = circle::GetModel(model_data_raw);

  CircleReader &reader = runtime_module->getCircleReader();
  if (!reader.parse(model))
    assert(false && "Error during parse");

  for (size_t i = 0; i < reader.num_subgraph(); ++i)
  {
    if (!reader.select_subgraph(i))
      assert(false && "Error during select subgraph");
    runtime_module->addGraph(memory_manager);

#ifndef USE_STATIC_ALLOC
    auto *runtime_graph = runtime_module->getRuntimeGraphAt(i);
    // For Dynamic memory manager we can use inplace optimization
    GraphLoader::checkInplaceOps(&reader, runtime_graph);
#endif // USE_STATIC_ALLOC
  }

  // For Dynamic Memory manager we build memory allocate/deallocate plan and then configure kernels.
  // For Static Memory manager we only configure kernels.
  for (size_t i = 0; i < reader.num_subgraph(); ++i)
  {
    auto *runtime_graph = runtime_module->getRuntimeGraphAt(i);
#ifdef USE_STATIC_ALLOC
    runtime_graph->configure_kernels();
#else
    runtime_graph->configure(dealloc_input);
#endif // USE_STATIC_ALLOC
  }

  // Select main subgraph
  reader.select_subgraph(0);
}

} // namespace luci_interpreter
