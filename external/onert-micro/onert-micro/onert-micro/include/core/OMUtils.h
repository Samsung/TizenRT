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

#ifndef ONERT_MICRO_CORE_UTILS_H
#define ONERT_MICRO_CORE_UTILS_H

#include "OMStatus.h"
#include "core/reader/OMCircleReader.h"
#include "core/OMRuntimeShape.h"

#include <cassert>
#include <cstdint>

namespace onert_micro
{
namespace core
{
namespace utils
{

OMStatus checkCondition(bool cond);

template <typename T> const T *castInputData(uint8_t *tensor_data)
{
  return tensor_data != nullptr ? reinterpret_cast<const T *>(tensor_data) : nullptr;
}

template <typename T> T *castOutputData(uint8_t *tensor_data)
{
  return tensor_data != nullptr ? reinterpret_cast<T *>(tensor_data) : nullptr;
}

} // namespace utils
} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_UTILS_H
