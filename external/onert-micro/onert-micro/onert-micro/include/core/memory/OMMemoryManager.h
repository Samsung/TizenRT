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

#ifndef ONERT_MICRO_CORE_MEMORY_MEMORY_MANAGER_H
#define ONERT_MICRO_CORE_MEMORY_MEMORY_MANAGER_H

#include "OMStatus.h"

#include <cstdint>
#include <stdlib.h>

namespace onert_micro
{
namespace core
{
namespace memory
{

struct OMMemoryManager
{
  // Need for configure tool estimations
#ifdef OM_MEMORY_ESTIMATE
  static size_t peak_memory_allocated;
  static size_t cur_memory_allocated;
  static OMStatus deallocateMemory(uint32_t size, uint8_t *data);
#endif // OM_MEMORY_ESTIMATE
  static OMStatus allocateMemory(uint32_t size, uint8_t **data);
  static OMStatus deallocateMemory(uint8_t *data);
};

} // namespace memory
} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_MEMORY_MEMORY_MANAGER_H
