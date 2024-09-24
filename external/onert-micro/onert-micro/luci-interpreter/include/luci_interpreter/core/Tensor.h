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

#ifndef LUCI_INTERPRETER_CORE_TENSOR_H
#define LUCI_INTERPRETER_CORE_TENSOR_H

#include "luci_interpreter/core/DataType.h"
#include "luci_interpreter/core/reader/CircleMicroReader.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace luci_interpreter
{

static constexpr int kMaxSmallSize = 5;

class RuntimeShape
{
public:
  RuntimeShape(const RuntimeShape &other) : _size(other.dimensionsCount())
  {
    std::memcpy(dimsData(), other.dimsData(), sizeof(int32_t) * _size);
  }

  // Returns the total count of elements, that is the size when flattened into a
  // vector.
  inline int flatSize() const
  {
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

  RuntimeShape() : _size(0) {}

  explicit RuntimeShape(int dimensions_count) : _size(dimensions_count)
  {
    assert(dimensions_count <= kMaxSmallSize);
    assert(dimensions_count >= 0);
  }

  RuntimeShape(int dimensions_count, const int32_t *dims_data) : _size(0)
  {
    resize(dimensions_count);
    int32_t *dst_dims = dimsData();
    std::memcpy(dst_dims, dims_data, dimensions_count * sizeof(int32_t));
  }

  RuntimeShape(int new_shape_size, const RuntimeShape &shape, int pad_value) : _size(0)
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

  RuntimeShape(int shape_size, int32_t value) : _size(0)
  {
    resize(shape_size);
    for (int i = 0; i < shape_size; ++i)
    {
      setDim(i, value);
    }
  }

  inline static RuntimeShape extendedShape(int new_shape_size, const RuntimeShape &shape)
  {
    return RuntimeShape(new_shape_size, shape, 1);
  }

  bool operator==(const RuntimeShape &comp) const
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
    assert(dimensions_count <= kMaxSmallSize);
    assert(dimensions_count >= 0);
    _size = dimensions_count;
  }

private:
  int32_t _size;
  int32_t _dims[kMaxSmallSize];
};

class Tensor
{
public:
#ifndef DIS_QUANT
  static float scale(const circle::Tensor *circle_tensor)
  {
    const auto *quant_params = circle_tensor->quantization();
    if (quant_params == nullptr)
    {
      assert(false && "There is no quantization params");
      return 0;
    }

    return *quant_params->scale()->cbegin();
  }

  static int32_t zero_point(const circle::Tensor *circle_tensor)
  {
    const auto *quant_params = circle_tensor->quantization();
    if (quant_params == nullptr)
    {
      assert(false && "There is no quantization params");
      return 0;
    }

    return *quant_params->zero_point()->cbegin();
  }

  static const std::vector<float> scales(const circle::Tensor *circle_tensor)
  {
    const auto *quant_params = circle_tensor->quantization();
    if (quant_params == nullptr)
    {
      assert(false && "There is no quantization params");
      return {};
    }
    assert(quant_params->scale() != nullptr);
    std::vector<float> scales(quant_params->scale()->cbegin(), quant_params->scale()->cend());

    return scales;
  }

  static const std::vector<int32_t> zero_points(const circle::Tensor *circle_tensor)
  {
    const auto *quant_params = circle_tensor->quantization();
    if (quant_params == nullptr)
    {
      assert(false && "There is no quantization params");
      return {};
    }
    assert(quant_params->zero_point() != nullptr);
    std::vector<int32_t> zero_points(quant_params->zero_point()->cbegin(),
                                     quant_params->zero_point()->cend());

    return zero_points;
  }

  static int32_t quantized_dimension(const circle::Tensor *circle_tensor)
  {
    const auto *quant_params = circle_tensor->quantization();
    if (quant_params == nullptr)
    {
      assert(false && "There is no quantization params");
      return 0;
    }
    return quant_params->quantized_dimension();
  }
#endif

  static bool is_constant_tensor(const luci_interpreter::CircleReader *reader,
                                 const circle::Tensor *circle_tensor)
  {
    if (circle_tensor != nullptr)
    {
      return reader->buffers()[circle_tensor->buffer()]->data() != nullptr;
    }
    return false; // at this point, this is not even tensor at all
  }

  static DataType element_type(const circle::Tensor *circle_tensor)
  {
    return luci_datatype(circle_tensor->type());
  }

  static VectorWrapper<int32_t> tensor_shape(const circle::Tensor *circle_tensor)
  {
    return wrap(circle_tensor->shape());
  }

  static int num_dims(const circle::Tensor *circle_tensor)
  {
    // TODO check removing of wrap
    auto const const_dims = wrap(circle_tensor->shape());
    return const_dims.size();
  }

  static int32_t dim(const circle::Tensor *circle_tensor, int i)
  {
    // TODO check removing of wrap
    assert(i >= 0);
    auto const const_dims = wrap(circle_tensor->shape());
    assert(i < const_dims.size());

    return const_dims[i];
  }

  static int32_t num_elements(const circle::Tensor *circle_tensor)
  {
    int32_t result = 1;
    auto const const_dims = wrap(circle_tensor->shape());
    for (const int32_t dim : const_dims)
    {
      result *= dim;
    }
    return result;
  }
};

} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_CORE_TENSOR_H
