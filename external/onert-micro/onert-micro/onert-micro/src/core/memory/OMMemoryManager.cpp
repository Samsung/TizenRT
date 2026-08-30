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

#include "core/memory/OMMemoryManager.h"

#include <algorithm>

using namespace onert_micro::core::memory;
using namespace onert_micro;

#ifdef OM_MEMORY_ESTIMATE

size_t OMMemoryManager::peak_memory_allocated = 0;
size_t OMMemoryManager::cur_memory_allocated = 0;

#endif // OM_MEMORY_ESTIMATE

OMStatus OMMemoryManager::allocateMemory(uint32_t size, uint8_t **data)
{
  if (size == 0)
    return UnknownError;
  auto data_tmp = new uint8_t[size];

#ifdef OM_MEMORY_ESTIMATE

  cur_memory_allocated += size;

  peak_memory_allocated = std::max(cur_memory_allocated, peak_memory_allocated);

#endif // OM_MEMORY_ESTIMATE

  *data = data_tmp;

  return Ok;
}

#ifdef OM_MEMORY_ESTIMATE
OMStatus OMMemoryManager::deallocateMemory(uint32_t size, uint8_t *data)
{
  if (int32_t(cur_memory_allocated) - int32_t(size) < 0 and data != nullptr)
    peak_memory_allocated = std::max(cur_memory_allocated, peak_memory_allocated);
  cur_memory_allocated -= data != nullptr ? size : 0;

  delete[] data;
  return Ok;
}
#endif // OM_MEMORY_ESTIMATE

OMStatus OMMemoryManager::deallocateMemory(uint8_t *data)
{
  delete[] data;
  return Ok;
}
