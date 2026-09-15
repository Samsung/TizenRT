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

#ifndef ONERT_MICRO_CORE_RUNTIME_STORAGE_H
#define ONERT_MICRO_CORE_RUNTIME_STORAGE_H

#include "OMRuntimeShape.h"
#include "OMStatus.h"
#include "OMKernelType.h"

#include <vector>
#include <unordered_map>
#include <cstdint>

namespace onert_micro
{
namespace core
{

class OMRuntimeStorage
{
private:
#ifndef DIS_DYN_SHAPES
  std::unordered_map<uint16_t, OMRuntimeShape> _tensor_index_to_dynamic_tensor_size;
#endif
  std::unordered_map<uint16_t, uint8_t *> _tensor_index_to_data;
  std::unordered_map<uint16_t, OMKernelType> _operator_index_to_kernel_type;

public:
  OMRuntimeStorage() = default;
  OMRuntimeStorage(const OMRuntimeStorage &) = delete;
  OMRuntimeStorage(OMRuntimeStorage &&) = default;
  OMRuntimeStorage &operator=(const OMRuntimeStorage &) = delete;
  OMRuntimeStorage &&operator=(const OMRuntimeStorage &&) = delete;
  ~OMRuntimeStorage() = default;

  std::unordered_map<uint16_t, uint8_t *> &getTensorIndexToData() { return _tensor_index_to_data; }

  OMStatus saveDataToTensorIndex(uint8_t *data, uint16_t tensor_index);

  OMStatus removeTensorFromTensorIndexToData(uint16_t tensor_index);

  OMStatus getDataByTensorIndex(uint8_t **data, uint16_t tensor_index);

  OMKernelType getKernelType(uint16_t op_index)
  {
    auto it = _operator_index_to_kernel_type.find(op_index);
    if (it == _operator_index_to_kernel_type.end())
      return Normal;

    return it->second;
  }

  OMStatus setKernelType(uint16_t op_index, OMKernelType type)
  {
    _operator_index_to_kernel_type[op_index] = type;
    return Ok;
  }
#ifndef DIS_DYN_SHAPES
  OMRuntimeShape getDynamicRuntimeShape(uint16_t tensor_index)
  {
    auto it = _tensor_index_to_dynamic_tensor_size.find(tensor_index);
    if (it == _tensor_index_to_dynamic_tensor_size.end())
      return {}; // Return empty

    return it->second;
  }

  OMStatus setDynamicRuntimeShape(uint16_t tensor_index, const OMRuntimeShape &shape)
  {
    _tensor_index_to_dynamic_tensor_size[tensor_index] = shape;
    return Ok;
  }
#endif // DIS_DYN_SHAPES

  void clearTensorIndexToData() { _tensor_index_to_data.clear(); }
};

} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_RUNTIME_STORAGE_H
