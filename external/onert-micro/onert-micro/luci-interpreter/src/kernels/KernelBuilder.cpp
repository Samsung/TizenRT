/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "KernelBuilder.h"

namespace luci_interpreter
{

void KernelConfigureRegistry::configure_kernel(const circle::Operator *cur_op,
                                               circle::BuiltinOperator opcode,
                                               BaseRuntimeGraph *runtime_graph) const
{
  printf("Vibhor opcode %d\n", opcode);
  auto specific_configure_func = get_kernel_configure_func(opcode);
  if (specific_configure_func == nullptr)
    assert(false && "Unsupported operator");

  specific_configure_func(cur_op, runtime_graph);
}

void KernelExecuteRegistry::execute_kernel(const circle::Operator *cur_op,
                                           circle::BuiltinOperator opcode,
                                           BaseRuntimeGraph *runtime_graph) const
{
  auto specific_execute_func = get_kernel_execute_func(opcode);
  if (specific_execute_func == nullptr)
    assert(false && "Unsupported operator");

  specific_execute_func(cur_op, runtime_graph);
}

#ifdef ENABLE_TRAINING

training::Status training::KernelTrainRegistry::train_kernel(
  const circle::Operator *cur_op, circle::BuiltinOperator opcode, CircleReader *reader,
  GradientCalculationStorage *gradient_calculation_storage, const TrainingSettings &settings,
  TrainableWeightStorage *weight_storage, bool is_compute_gradient) const
{
  auto specific_train_func = get_kernel_train_func(opcode);
  if (specific_train_func == nullptr)
    assert(false && "Unsupported operator");

  return specific_train_func(cur_op, reader, gradient_calculation_storage, settings, weight_storage,
                             is_compute_gradient);
}

#endif // ENABLE_TRAINING

} // namespace luci_interpreter
