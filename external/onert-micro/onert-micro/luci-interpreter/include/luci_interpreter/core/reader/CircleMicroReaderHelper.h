/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef __LUCI_MICRO_CIRCLE_MICRO_READER_HELPER_H__
#define __LUCI_MICRO_CIRCLE_MICRO_READER_HELPER_H__

#include <circle-generated/circle/schema_generated.h>

#include <vector>

namespace circle
{

::circle::BuiltinOperator builtin_code_neutral(const ::circle::OperatorCode *opcode);
bool is_valid(const ::circle::OperatorCode *opcode);
bool is_custom(const ::circle::OperatorCode *opcode);
const char *tensor_type(const ::circle::Tensor *tensor);

template <typename T> std::vector<T> as_index_vector(const flatbuffers::Vector<T> *flat_array)
{
  if (flat_array == nullptr)
  {
    assert(false && "flat array is nullptr");
  }

  std::vector<T> ret(flat_array->Length());
  for (uint32_t i = 0; i < flat_array->Length(); i++)
  {
    ret[i] = flat_array->Get(i);
  }
  return ret;
}

} // namespace circle

#endif // __LUCI_MICRO_CIRCLE_MICRO_READER_HELPER_H__
