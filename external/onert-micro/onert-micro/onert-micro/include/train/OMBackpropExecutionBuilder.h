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

#ifndef ONERT_MICRO_BACKPROP_EXECUTION_BUILDER_H
#define ONERT_MICRO_BACKPROP_EXECUTION_BUILDER_H

#include "core/reader/OMCircleReader.h"
#include "core/OMKernelType.h"
#include "core/OMRuntimeStorage.h"
#include "core/OMRuntimeContext.h"
#include "train/OMBackpropExecuteArgs.h"

namespace onert_micro
{
namespace train
{

using KernelTrainFunc = OMStatus(const OMBackpropExecuteArgs &);

#define REGISTER_TRAIN_KERNEL(builtin_operator, name) \
  OMStatus train_kernel_Circle##name(const OMBackpropExecuteArgs &);
#include "KernelsToTrain.lst"
#undef REGISTER_TRAIN_KERNEL

/*
 * Class to registry kernels for execution of backward graph (calculation backpropagation)
 * Kernels which will be register define in KernelsToTrain.lst current pal directory
 */
class KernelBuiltinTrainRegistry
{
public:
  constexpr KernelBuiltinTrainRegistry() : _operator_train()
  {
#define REGISTER_TRAIN_KERNEL(builtin_operator, name)                        \
  registerKernelTrain(core::OMBuilderID::BuiltinOperator_##builtin_operator, \
                      train_kernel_Circle##name);

#include "KernelsToTrain.lst"

#undef REGISTER_TRAIN_KERNEL
  }

public:
  OMStatus getKernelTrainFunc(core::OMBuilderID builderID, KernelTrainFunc **train_func) const
  {
    const auto builder_id_opcode = size_t(builderID);
    assert(builder_id_opcode < size_t(core::OMBuilderID::BuiltinOperatorsSize));
    if (builder_id_opcode >= size_t(core::OMBuilderID::BuiltinOperatorsSize))
    {
      *train_func = nullptr;
      return UnknownError;
    }
    *train_func = _operator_train[builder_id_opcode];
    return Ok;
  }

private:
  constexpr void registerKernelTrain(core::OMBuilderID id, KernelTrainFunc *func)
  {
    assert(size_t(id) < size_t(core::OMBuilderID::BuiltinOperatorsSize));
    _operator_train[size_t(id)] = func;
  }

private:
  KernelTrainFunc *_operator_train[size_t(core::OMBuilderID::BuiltinOperatorsSize)];
};

// Global constexpr kernel builtin train
constexpr KernelBuiltinTrainRegistry kernel_builtin_train;

} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_BACKPROP_EXECUTION_BUILDER_H
