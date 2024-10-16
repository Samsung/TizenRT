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

#include "luci_interpreter/core/reader/CircleMicroReaderHelper.h"

namespace circle
{

::circle::BuiltinOperator builtin_code_neutral(const ::circle::OperatorCode *opcode)
{
  assert(opcode != nullptr);
  if (opcode->deprecated_builtin_code() == 127)
  {
    assert(opcode->builtin_code() >= 127);
    return opcode->builtin_code();
  }
  // There was no 255(-1) value in v0.3
  assert(opcode->deprecated_builtin_code() != -1);
  return static_cast<::circle::BuiltinOperator>(opcode->deprecated_builtin_code());
}

bool is_valid(const ::circle::OperatorCode *opcode)
{
  ::circle::BuiltinOperator code = opcode->builtin_code();
  return (::circle::BuiltinOperator_MIN <= code && code <= ::circle::BuiltinOperator_MAX);
}

bool is_custom(const ::circle::OperatorCode *opcode)
{
  ::circle::BuiltinOperator code = opcode->builtin_code();
  return (code == ::circle::BuiltinOperator_CUSTOM);
}

const char *tensor_type(const ::circle::Tensor *tensor)
{
  return ::circle::EnumNameTensorType(tensor->type());
}

} // namespace circle
