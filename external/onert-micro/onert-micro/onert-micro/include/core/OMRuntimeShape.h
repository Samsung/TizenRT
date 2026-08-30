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

#ifndef ONERT_MICRO_CORE_RUNTIME_SHAPE_H
#define ONERT_MICRO_CORE_RUNTIME_SHAPE_H

#include "core/reader/OMCircleReader.h"

#include <cstring>
#include <cstdint>
#include <cassert>

namespace onert_micro
{
namespace core
{

static constexpr int maxTensorShapeSize = 6;

class OMRuntimeShape
{
private:
  int32_t _size = 0;
  int32_t _dims[maxTensorShapeSize] = {0};

public:
  OMRuntimeShape(const OMRuntimeShape &other) : _size(other.dimensionsCount())
  {
    std::memcpy(dimsData(), other.dimsData(), sizeof(int32_t) * _size);
  }

  OMRuntimeShape(const circle::Tensor *tensor)
  {
    if (tensor == nullptr)
      return;

    // Shape is scalar
    if (tensor->shape() == nullptr or tensor->shape()->size() == 0)
    {
      _size = 1;
      _dims[0] = 1;
      return;
    }

    _size = tensor->shape()->size();
    std::memcpy(_dims, tensor->shape()->data(), sizeof(int32_t) * _size);
  }

  // Returns the total count of elements, that is the size when flattened into a
  // vector.
  inline int flatSize() const
  {
    if (_size == 0)
      return 0;
    int buffer_size = 1;
    const int *dims_data = reinterpret_cast<const int *>(dimsData());
    for (int i = 0; i < _size; i++)
    {
      buffer_size *= dims_data[i];
    }
    return buffer_size;
  }

  inline int32_t *dimsData() { return _dims; }
  inline const int32_t *dimsData() const { return _dims; }

  OMRuntimeShape() : _size(0) {}

  explicit OMRuntimeShape(int dimensions_count) : _size(dimensions_count)
  {
    assert(dimensions_count <= maxTensorShapeSize);
    assert(dimensions_count >= 0);
  }

  OMRuntimeShape(int dimensions_count, const int32_t *dims_data) : _size(0)
  {
    resize(dimensions_count);
    int32_t *dst_dims = dimsData();
    std::memcpy(dst_dims, dims_data, dimensions_count * sizeof(int32_t));
  }

  OMRuntimeShape(int new_shape_size, const OMRuntimeShape &shape, int pad_value) : _size(0)
  {
    resize(new_shape_size);
    const int size_increase = new_shape_size - shape.dimensionsCount();
    for (int i = 0; i < size_increase; ++i)
    {
      setDim(i, pad_value);
    }
    std::memcpy(dimsData() + size_increase, shape.dimsData(),
                sizeof(int32_t) * shape.dimensionsCount());
  }

  OMRuntimeShape(int shape_size, int32_t value) : _size(0)
  {
    resize(shape_size);
    for (int i = 0; i < shape_size; ++i)
    {
      setDim(i, value);
    }
  }

  inline static OMRuntimeShape extendedShape(int new_shape_size, const OMRuntimeShape &shape)
  {
    return OMRuntimeShape(new_shape_size, shape, 1);
  }

  bool operator==(const OMRuntimeShape &comp) const
  {
    return this->_size == comp._size &&
           std::memcmp(dimsData(), comp.dimsData(), _size * sizeof(int32_t)) == 0;
  }

  inline int32_t dimensionsCount() const { return _size; }

  inline int32_t dims(int i) const
  {
    assert(i <= _size);
    assert(i >= 0);
    return _dims[i];
  }

  inline void setDim(int i, int32_t val)
  {
    assert(i <= _size);
    assert(i >= 0);
    _dims[i] = val;
  }

  inline void resize(int dimensions_count)
  {
    assert(dimensions_count <= maxTensorShapeSize);
    assert(dimensions_count >= 0);
    _size = dimensions_count;
  }
};

} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_RUNTIME_SHAPE_H
