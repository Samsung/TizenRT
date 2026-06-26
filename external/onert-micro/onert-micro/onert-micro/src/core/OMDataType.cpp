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

#include "core/OMDataType.h"

onert_micro::core::OMDataType onert_micro::core::onertMicroDatatype(const circle::TensorType type)
{
  switch (type)
  {
    case circle::TensorType_FLOAT32:
      return OMDataType::FLOAT32;
    case circle::TensorType_FLOAT16:
      return OMDataType::FLOAT16;
    case circle::TensorType_INT32:
      return OMDataType::S32;
    case circle::TensorType_UINT8:
      return OMDataType::U8;
    case circle::TensorType_INT64:
      return OMDataType::S64;
    case circle::TensorType_BOOL:
      return OMDataType::BOOL;
    case circle::TensorType_INT16:
      return OMDataType::S16;
    case circle::TensorType_COMPLEX64:
      break;
    case circle::TensorType_INT8:
      return OMDataType::S8;
    default:
      break;
  }
  assert(false);
  return OMDataType::Unknown;
}
