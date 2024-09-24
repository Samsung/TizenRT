/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef USE_STATIC_ALLOC
#ifndef LUCI_INTERPRETER_SIMPLE_MEMORY_MANAGER_H
#define LUCI_INTERPRETER_SIMPLE_MEMORY_MANAGER_H

#include "luci_interpreter/core/DataType.h"
#include "luci_interpreter/core/Tensor.h"

#include <cassert>

namespace luci_interpreter
{

class SimpleMemoryManager
{
public:
  uint8_t *allocate_memory(const circle::Tensor *tensor);
  void release_memory(uint8_t *data);
};

} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_SIMPLE_MEMORY_MANAGER_H
#endif // USE_STATIC_ALLOC
