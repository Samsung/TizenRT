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

#include "execute/OMKernelExecute.h"
#include "core/OMKernelType.h"
#include "execute/OMKernelExecutionBuilder.h"

using namespace onert_micro::execute;
using namespace onert_micro;

OMStatus OMKernelExecute::runForward(OMExecuteArgs &execute_args,
                                     core::memory::OMRuntimeAllocator &allocator)
{
  OMStatus status = Ok;

  core::OMRuntimeContext &context = execute_args.runtime_context;
  core::OMRuntimeStorage &storage = execute_args.runtime_storage;

  const core::reader::CircleOperators *operators = context.getCircleOperators();

  const auto num_operators = static_cast<uint16_t>(operators->size());
  const auto *op_codes = context.getCircleOpcodes();

  for (uint16_t i = 0; i < num_operators; ++i)
  {
    status = allocator.allocate(i, &context, &storage);

    if (status != Ok)
      return status;

    core::OMBuilderID builder_id = core::OMBuilderID::Size;
    const circle::Operator *op = operators->operator[](i);
    uint32_t index = op->opcode_index();

    assert(index < op_codes->size());

    const auto opcode = op_codes->operator[](index);

    status = core::getBuilderId(opcode, builder_id);

    assert(status == Ok);
    if (status != Ok)
      return status;

    execute_args.kernel_index = i;

    KernelExecuteFunc *execute_func = nullptr;
    if (size_t(builder_id) < size_t(core::OMBuilderID::BuiltinOperatorsSize))
    {
      // Builtin operator
      status = kernel_builtin_execute.getKernelExecuteFunc(builder_id, &execute_func);
    }
    else
    {
      // Custom
      status = kernel_custom_execute.getKernelExecuteFunc(builder_id, &execute_func);
    }

    assert(execute_func != nullptr);

    if (status != Ok)
      return status;

    status = execute_func(execute_args);

    assert(status == Ok);

    if (status != Ok)
      return status;

#ifdef OM_MEMORY_ESTIMATE
    status = allocator.deallocate(i, &storage, &context);
#else
    status = allocator.deallocate(i, &storage);
#endif // OM_MEMORY_ESTIMATE
  }

  return status;
}
