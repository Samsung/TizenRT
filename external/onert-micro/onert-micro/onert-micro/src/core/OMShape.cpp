/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "core/OMShape.h"

using namespace onert_micro::core;
using namespace onert_micro;

int32_t OMShape::num_elements() const
{
  if (_shape == nullptr)
    return -1;

  int32_t result = 1;

  uint32_t size = _shape->size();
  for (uint32_t i = 0; i < size; ++i)
  {
    result *= _shape->operator[](i);
  }

  assert(size >= 0 && "Size cannot be negative");
  if (size < 0)
    return -1;

  return result;
}

int32_t OMShape::rank() const
{
  if (_shape == nullptr)
    return -1;

  int32_t result = static_cast<int32_t>(_shape->size());
  return result;
}

int32_t OMShape::dim(uint32_t idx) const
{
  if (_shape == nullptr)
    return -1;

  uint32_t size = _shape->size();
  assert(idx < size && "Wrong index");
  if (idx >= size)
    return -1;

  return _shape->operator[](idx);
}
