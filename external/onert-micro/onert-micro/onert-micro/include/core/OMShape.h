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

#ifndef ONERT_MICRO_CORE_SHAPE_H
#define ONERT_MICRO_CORE_SHAPE_H

#include "core/reader/OMCircleReader.h"

#include <cstdint>

namespace onert_micro
{
namespace core
{

class OMShape
{
private:
  const reader::CircleValues *_shape = nullptr;

public:
  OMShape() = default;
  OMShape(const circle::Tensor *tensor)
  {
    if (tensor != nullptr)
      _shape = tensor->shape();
  }
  OMShape(const OMShape&) = delete;
  OMShape(OMShape &&) = delete;
  OMShape &operator=(const OMShape &) = delete;
  OMShape &&operator=(const OMShape &&) = delete;
  ~OMShape() = default;

  void updateShape(const circle::Tensor *tensor)
  {
    if (tensor != nullptr)
      _shape = tensor->shape();
  }

  int32_t num_elements() const;
  int32_t dim(uint32_t idx) const;
  int32_t rank() const;
};

} // core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_KERNEL_H
