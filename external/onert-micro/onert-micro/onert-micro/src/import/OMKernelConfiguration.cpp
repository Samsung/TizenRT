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

#include "import/OMKernelConfiguration.h"
#include "core/OMKernelType.h"
#include "import/OMKernelConfigureBuilder.h"

using namespace onert_micro::core;
using namespace onert_micro::import;
using namespace onert_micro;

OMStatus OMKernelConfiguration::configureKernels(OMConfigureArgs &configure_args)
{
  OMRuntimeContext &runtime_context = configure_args.runtime_context;
  const reader::CircleOperators *operators = runtime_context.getCircleOperators();

  const auto num_operators = static_cast<uint16_t>(operators->size());
  const auto op_codes = runtime_context.getCircleOpcodes();

  OMStatus status = Ok;
  for (uint16_t i = 0; i < num_operators; ++i)
  {
    OMBuilderID builder_id = core::OMBuilderID::Size;
    const circle::Operator *op = operators->operator[](i);
    uint32_t index = op->opcode_index();

    assert(index < op_codes->size());

    const auto opcode = op_codes->operator[](index);

    status = core::getBuilderId(opcode, builder_id);

    assert(status == Ok);
    if (status != Ok)
      return status;

    KernelConfigureFunc *configure_func = nullptr;
    if (size_t(builder_id) < size_t(core::OMBuilderID::BuiltinOperatorsSize))
    {
      // Builtin operator
      status = kernel_builtin_configure.getKernelConfigureFunc(builder_id, &configure_func);
    }
    else
    {
      // Custom
      status = kernel_custom_configure.getKernelConfigureFunc(builder_id, &configure_func);
    }

    assert(configure_func != nullptr);

    if (status != Ok)
      return status;

    configure_args.kernel_index = i;
    status = configure_func(configure_args);
    assert(status == Ok && "");
  }

  return status;
}
