/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_KERNEL_KERNELBUILDER_H
#define LUCI_INTERPRETER_KERNEL_KERNELBUILDER_H

#include "core/RuntimeModule.h"
#include "luci_interpreter/core/reader/CircleMicroReader.h"
#include "Builders.h"

#include <memory>
#include <unordered_map>

namespace luci_interpreter
{
#define REGISTER_KERNEL(builtin_operator, name) BuiltinOperator_##builtin_operator,

enum class BuilderID
{
#if USE_GENERATED_LIST
#include "GeneratedKernelsToBuild.lst"
#else
#include "KernelsToBuild.lst"
#endif
  Size // casts to count of values in BuilderId enum
};

#undef REGISTER_KERNEL

constexpr BuilderID get_builder_id(circle::BuiltinOperator opcode)
{
  switch (opcode)
  {
#define REGISTER_KERNEL(builtin_operator, name)    \
  case circle::BuiltinOperator_##builtin_operator: \
    return BuilderID::BuiltinOperator_##builtin_operator;

#if USE_GENERATED_LIST
#include "GeneratedKernelsToBuild.lst"
#else
#include "KernelsToBuild.lst"
#endif

#undef REGISTER_KERNEL
    default:
      assert(false && "Unsupported operation");
  }
}

class KernelConfigureRegistry
{
public:
  using KernelConfigureFunc = void(const circle::Operator *, BaseRuntimeGraph *);

  constexpr KernelConfigureRegistry() : _operator_configure()
  {
#define REGISTER_KERNEL(builtin_operator, name)                            \
  register_kernel_configure(BuilderID::BuiltinOperator_##builtin_operator, \
                            configure_kernel_Circle##name);

#if USE_GENERATED_LIST
#include "GeneratedKernelsToBuild.lst"
#else
#include "KernelsToBuild.lst"
#endif

#undef REGISTER_KERNEL
  }

  void configure_kernel(const circle::Operator *cur_op, circle::BuiltinOperator opcode,
                        BaseRuntimeGraph *runtime_graph) const;

private:
  constexpr KernelConfigureFunc *get_kernel_configure_func(circle::BuiltinOperator opcode) const
  {
    const auto builder_id_opcode = size_t(get_builder_id(opcode));
    assert(builder_id_opcode < size_t(BuilderID::Size));
    return _operator_configure[builder_id_opcode];
  }

  constexpr void register_kernel_configure(BuilderID id, KernelConfigureFunc *func)
  {
    assert(size_t(id) < size_t(BuilderID::Size));
    _operator_configure[size_t(id)] = func;
  }

private:
  KernelConfigureFunc *_operator_configure[size_t(BuilderID::Size)];
};

class KernelExecuteRegistry
{
public:
  using KernelExecuteFunc = void(const circle::Operator *, BaseRuntimeGraph *);

  constexpr KernelExecuteRegistry() : _operator_execute()
  {
#define REGISTER_KERNEL(builtin_operator, name)                          \
  register_kernel_execute(BuilderID::BuiltinOperator_##builtin_operator, \
                          execute_kernel_Circle##name);

#if USE_GENERATED_LIST
#include "GeneratedKernelsToBuild.lst"
#else
#include "KernelsToBuild.lst"
#endif

#undef REGISTER_KERNEL
  }

  void execute_kernel(const circle::Operator *cur_op, circle::BuiltinOperator opcode,
                      BaseRuntimeGraph *runtime_graph) const;

private:
  constexpr KernelExecuteFunc *get_kernel_execute_func(circle::BuiltinOperator opcode) const
  {
    const auto tmp = size_t(get_builder_id(opcode));
    assert(tmp < size_t(BuilderID::Size));
    return _operator_execute[tmp];
  }

  constexpr void register_kernel_execute(BuilderID id, KernelExecuteFunc *func)
  {
    assert(size_t(id) < size_t(BuilderID::Size));
    _operator_execute[size_t(id)] = func;
  }

private:
  KernelExecuteFunc *_operator_execute[size_t(BuilderID::Size)];
};

#ifdef ENABLE_TRAINING

namespace training
{
class KernelTrainRegistry
{
public:
  using KernelTrainFunc = Status(const circle::Operator *, CircleReader *,
                                 GradientCalculationStorage *, const TrainingSettings &,
                                 TrainableWeightStorage *, bool);

  constexpr KernelTrainRegistry() : _operator_train()
  {
#define REGISTER_TRAIN_KERNEL(builtin_operator, name) \
  register_kernel_train(BuilderID::BuiltinOperator_##builtin_operator, train_kernel_Circle##name);

#if USE_GENERATED_LIST
#include "GeneratedKernelsToBuild.lst"
#else
#include "KernelsToTrain.lst"
#endif

#undef REGISTER_TRAIN_KERNEL
  }

  Status train_kernel(const circle::Operator *cur_op, circle::BuiltinOperator opcode,
                      CircleReader *reader,
                      GradientCalculationStorage *gradient_calculation_storage,
                      const TrainingSettings &settings, TrainableWeightStorage *weight_storage,
                      bool is_compute_gradient) const;

private:
  constexpr KernelTrainFunc *get_kernel_train_func(circle::BuiltinOperator opcode) const
  {
    const auto tmp = size_t(get_builder_id(opcode));
    assert(tmp < size_t(BuilderID::Size));
    return _operator_train[tmp];
  }

  constexpr void register_kernel_train(BuilderID id, KernelTrainFunc *func)
  {
    assert(size_t(id) < size_t(BuilderID::Size));
    _operator_train[size_t(id)] = func;
  }

private:
  KernelTrainFunc *_operator_train[size_t(BuilderID::Size)];
};

constexpr KernelTrainRegistry kernel_train;
} // namespace training
#endif // ENABLE_TRAINING

// Global constexpr kernel configure and kernel executor
constexpr KernelConfigureRegistry kernel_configure;
constexpr KernelExecuteRegistry kernel_executor;

} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNEL_KERNELBUILDER_H
