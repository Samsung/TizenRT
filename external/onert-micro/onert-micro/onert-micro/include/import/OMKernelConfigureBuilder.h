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

#ifndef ONERT_MICRO_IMPORT_KERNEL_CONFIGURE_BUILDER_H
#define ONERT_MICRO_IMPORT_KERNEL_CONFIGURE_BUILDER_H

#include "core/reader/OMCircleReader.h"
#include "core/OMKernelType.h"
#include "core/OMRuntimeStorage.h"
#include "core/OMRuntimeContext.h"
#include "core/OMRuntimeModule.h"
#include "import/OMConfigureArgs.h"
#include "OMConfig.h"

namespace onert_micro
{
namespace import
{

using KernelConfigureFunc = OMStatus(const OMConfigureArgs &);

#define REGISTER_KERNEL(builtin_operator, name) \
  OMStatus configure_kernel_Circle##name(const OMConfigureArgs &);
#include "KernelsToBuild.lst"
#undef REGISTER_KERNEL

#define REGISTER_CUSTOM_KERNEL(name, string_name) \
  OMStatus configure_kernel_Circle##name(const OMConfigureArgs &);
#include "CustomKernelsToBuild.lst"
#undef REGISTER_CUSTOM_KERNEL

class KernelBuiltinConfigureRegistry
{
public:
  constexpr KernelBuiltinConfigureRegistry() : _operator_configure()
  {
#define REGISTER_KERNEL(builtin_operator, name)                                  \
  registerKernelConfigure(core::OMBuilderID::BuiltinOperator_##builtin_operator, \
                          configure_kernel_Circle##name);

#include "KernelsToBuild.lst"

#undef REGISTER_KERNEL
  }

public:
  OMStatus getKernelConfigureFunc(core::OMBuilderID builderID,
                                  KernelConfigureFunc **configure_func) const
  {
    const auto builder_id_opcode = size_t(builderID);
    assert(builder_id_opcode < size_t(core::OMBuilderID::BuiltinOperatorsSize));
    if (builder_id_opcode >= size_t(core::OMBuilderID::BuiltinOperatorsSize))
    {
      *configure_func = nullptr;
      return UnknownError;
    }
    *configure_func = _operator_configure[builder_id_opcode];
    return Ok;
  }

private:
  constexpr void registerKernelConfigure(core::OMBuilderID id, KernelConfigureFunc *func)
  {
    assert(size_t(id) < size_t(core::OMBuilderID::BuiltinOperatorsSize));
    _operator_configure[size_t(id)] = func;
  }

private:
  KernelConfigureFunc *_operator_configure[size_t(core::OMBuilderID::BuiltinOperatorsSize)];
};

class KernelCustomConfigureRegistry
{
public:
  constexpr KernelCustomConfigureRegistry() : _operator_configure()
  {
#define REGISTER_CUSTOM_KERNEL(name, string_name) \
  registerKernelConfigure(core::OMBuilderID::CUSTOM_##name, configure_kernel_Circle##name);

#include "CustomKernelsToBuild.lst"

#undef REGISTER_CUSTOM_KERNEL
  }

public:
  OMStatus getKernelConfigureFunc(core::OMBuilderID builderID,
                                  KernelConfigureFunc **configure_func) const
  {
    auto builder_id_opcode = size_t(builderID);
    if (builder_id_opcode >= size_t(core::OMBuilderID::Size))
    {
      *configure_func = nullptr;
      return UnknownError;
    }
    const auto builder_id_offset = size_t(core::OMBuilderID::BuiltinOperatorsSize);
    builder_id_opcode -= builder_id_offset - 1;

    *configure_func = _operator_configure[builder_id_opcode];
    return Ok;
  }

private:
  constexpr void registerKernelConfigure(core::OMBuilderID id, KernelConfigureFunc *func)
  {
    auto builder_id_opcode = size_t(id);
    const auto builder_id_offset = size_t(core::OMBuilderID::BuiltinOperatorsSize);
    builder_id_opcode = builder_id_opcode - builder_id_offset - 1;
    _operator_configure[builder_id_opcode] = func;
  }

private:
  KernelConfigureFunc *_operator_configure[size_t(core::OMBuilderID::Size) -
                                           size_t(core::OMBuilderID::BuiltinOperatorsSize) - 1];
};

// Global constexpr kernel builtin and custom configure
constexpr KernelBuiltinConfigureRegistry kernel_builtin_configure;
constexpr KernelCustomConfigureRegistry kernel_custom_configure;

} // namespace import
} // namespace onert_micro

#endif // ONERT_MICRO_IMPORT_KERNEL_CONFIGURE_BUILDER_H
