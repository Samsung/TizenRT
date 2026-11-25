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

#include "core/OMRuntimeStorage.h"

using namespace onert_micro::core;
using namespace onert_micro;

OMStatus OMRuntimeStorage::saveDataToTensorIndex(uint8_t *data, uint16_t tensor_index)
{
  _tensor_index_to_data[tensor_index] = data;

  return Ok;
}

OMStatus OMRuntimeStorage::removeTensorFromTensorIndexToData(uint16_t tensor_index)
{
  auto tensor_to_data_it = _tensor_index_to_data.find(tensor_index);
  assert(tensor_to_data_it != _tensor_index_to_data.end() && "No data");

  if (tensor_to_data_it == _tensor_index_to_data.end())
    return UnknownError;

  _tensor_index_to_data.erase(tensor_to_data_it);

  return Ok;
}

OMStatus OMRuntimeStorage::getDataByTensorIndex(uint8_t **data, uint16_t tensor_index)
{
  auto tensor_to_data_it = _tensor_index_to_data.find(tensor_index);

  if (tensor_to_data_it != _tensor_index_to_data.end())
    *data = tensor_to_data_it->second;
  else
    *data = nullptr;

  return Ok;
}
