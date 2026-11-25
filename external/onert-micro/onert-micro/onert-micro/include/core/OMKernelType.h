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

#ifndef ONERT_MICRO_CORE_KERNEL_TYPE_H
#define ONERT_MICRO_CORE_KERNEL_TYPE_H

#include "OMStatus.h"
#include "reader/OMCircleReader.h"

namespace onert_micro
{
namespace core
{

// Enum  to indicate the degree(rank) to which part of the operation we will train
enum OpTrainableRankType
{
  ALL = 0,            // 0 - Train all weights in the operation
  ONLY_BIAS = 1,      // 1 - Train bias only in the operation
  UP_1_2_PART = 2,    // 2 - Train the upper 1/2 part of the operation
  LOWER_1_2_PART = 3, // 3 - Train the lower 1/2 part of the operation
  // TODO add more
};

enum OMKernelType
{
  Normal,
  Inplace,
};

enum OMBuilderCustomID
{
  CUSTOM_custom_gru,
};

#define REGISTER_KERNEL(builtin_operator, name) BuiltinOperator_##builtin_operator,
#define REGISTER_CUSTOM_KERNEL(name, string_name) CUSTOM_##name,
enum class OMBuilderID
{
#include "pal/mcu/KernelsToBuild.lst"
  BuiltinOperatorsSize, // casts to count of values in BuilderId enum
#include "pal/mcu/CustomKernelsToBuild.lst"
  Size
};
#undef REGISTER_CUSTOM_KERNEL
#undef REGISTER_KERNEL

OMStatus getBuiltinOperatorBuilderId(const circle::BuiltinOperator &opcode,
                                     core::OMBuilderID &builderID);

OMStatus getCustomOperatorBuilderId(const flatbuffers::String *custom_opcode,
                                    core::OMBuilderID &builderID);

OMStatus getCustomOperatorByBuilderId(core::OMBuilderID &builderID, OMBuilderCustomID &opcode);

OMStatus getBuilderId(const circle::OperatorCode *opcode, core::OMBuilderID &builderID);

} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_KERNEL_TYPE_H
