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

#include "core/OMRuntimeContext.h"

using namespace onert_micro;
using namespace onert_micro::core;

const circle::Operator *OMRuntimeContext::getCircleOperatorAt(uint16_t index)
{
  const auto *operators = _reader.operators();
  assert(index < operators->size());
  return operators->operator[](index);
}

const reader::CircleOperators *OMRuntimeContext::getCircleOperators()
{
  return _reader.operators();
}

const circle::Tensor *OMRuntimeContext::getTensorByIndex(int32_t tensor_index)
{
  if (tensor_index == -1)
    return nullptr;
  return _reader.tensors()->operator[](tensor_index);
}

uint32_t OMRuntimeContext::getGraphInputTensorIndex(uint32_t index)
{
  auto *inputs = _reader.inputs();
  return inputs->operator[](index);
}

uint32_t OMRuntimeContext::getGraphOutputTensorIndex(uint32_t index)
{
  auto *outputs = _reader.outputs();
  return outputs->operator[](index);
}

OMStatus OMRuntimeContext::getConstDataByTensorIndex(uint8_t **data, uint16_t tensor_index)
{
  auto *tensor = getTensorByIndex(tensor_index);

  if (tensor == nullptr)
    return UnknownError;

  // To handle with separated weights:
  // 1) first we try to get weigths from wof file (using wof_reader)
  // 2) If it is not null -> return it
  // 3) It it is null try to get it from main circle file
  uint8_t *buffer_data = reinterpret_cast<uint8_t *>(_wof_reader.buffer(tensor_index));

  if (buffer_data != nullptr)
  {
    *data = buffer_data;
    return Ok;
  }

  auto const *buffer = _reader.buffers()->operator[](tensor->buffer())->data();

  if (buffer == nullptr)
    return Ok;

  *data = const_cast<uint8_t *>(buffer->data());

  return Ok;
}
