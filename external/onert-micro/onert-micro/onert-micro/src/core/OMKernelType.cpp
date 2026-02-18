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

#include "core/OMKernelType.h"

using namespace onert_micro::core;
using namespace onert_micro;

OMStatus onert_micro::core::getBuiltinOperatorBuilderId(const circle::BuiltinOperator &opcode,
                                                        core::OMBuilderID &builderID)
{
  switch (opcode)
  {
#define REGISTER_KERNEL(builtin_operator, name)                        \
  case circle::BuiltinOperator_##builtin_operator:                     \
    builderID = core::OMBuilderID::BuiltinOperator_##builtin_operator; \
    break;
#include "KernelsToBuild.lst"
#undef REGISTER_KERNEL
    default:
      assert(false && "Unsupported operation");
      return UnsupportedOp;
  }
  return Ok;
}

OMStatus onert_micro::core::getCustomOperatorByBuilderId(core::OMBuilderID &builderID,
                                                         OMBuilderCustomID &opcode)
{
  switch (builderID)
  {
#define REGISTER_CUSTOM_KERNEL(name, string_name)    \
  case core::OMBuilderID::CUSTOM_##name:             \
    opcode = core::OMBuilderCustomID::CUSTOM_##name; \
    break;
#include "CustomKernelsToBuild.lst"
#undef REGISTER_CUSTOM_KERNEL
    default:
      assert(false && "Unsupported operation");
      return UnsupportedOp;
  }
  return Ok;
}

OMStatus onert_micro::core::getBuilderId(const circle::OperatorCode *opcode,
                                         core::OMBuilderID &builderID)
{
  OMStatus status;

  if (opcode->builtin_code() == circle::BuiltinOperator_CUSTOM)
    status = core::getCustomOperatorBuilderId(opcode->custom_code(), builderID);
  else
    status = core::getBuiltinOperatorBuilderId(opcode->builtin_code(), builderID);

  assert(status == Ok && "Unknown operation");
  if (status == UnsupportedOp or builderID == core::OMBuilderID::Size)
    return UnsupportedOp;

  return status;
}

OMStatus onert_micro::core::getCustomOperatorBuilderId(const flatbuffers::String *custom_opcode,
                                                       core::OMBuilderID &builderID)
{
#define REGISTER_CUSTOM_KERNEL(name, string_name)    \
  const char arr[] = string_name;                    \
  if (std::strcmp(custom_opcode->c_str(), arr) == 0) \
  {                                                  \
    builderID = core::OMBuilderID::CUSTOM_##name;    \
    return Ok;                                       \
  }
#include "CustomKernelsToBuild.lst"
#undef REGISTER_CUSTOM_KERNEL
  assert(false && "Unsupported custom operation");
  return UnsupportedOp;
}
