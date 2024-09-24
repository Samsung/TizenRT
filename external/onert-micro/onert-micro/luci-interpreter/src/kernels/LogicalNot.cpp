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
#include "Builders.h"
#include "kernels/Utils.h"
#include "SISOKernel.h"

#include "PALLogicalNot.h"

namespace luci_interpreter
{
void configure_kernel_CircleLogicalNot(const circle::Operator *cur_op,
                                       BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input()) == DataType::BOOL);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.output()) == DataType::BOOL);

  // check that input and output dimensions are equal
  int N = Tensor::num_dims(kernel.input());
  LUCI_INTERPRETER_CHECK(N == Tensor::num_dims(kernel.output()));

  // check that sizes of all dimensions are equal
  for (int i = 0; i < N; ++i)
  {
    LUCI_INTERPRETER_CHECK(kernels::getTensorShape(kernel.input()).dims(i) ==
                           kernels::getTensorShape(kernel.output()).dims(i));
  }
}

void execute_kernel_CircleLogicalNot(const circle::Operator *cur_op,
                                     BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  auto data = kernels::getTensorData<bool>(runtime_graph->getDataByTensor(kernel.input()));
  if (data == nullptr)
    data = kernels::getTensorData<bool>(runtime_graph->getConstDataByTensor(kernel.input()));

  assert(data != nullptr);

  auto output_data = kernels::getTensorData<bool>(runtime_graph->getDataByTensor(kernel.output()));

  const int64_t flat_size = kernels::getTensorShape(kernel.input()).flatSize();
  luci_interpreter_pal::LogicalNot(flat_size, data, output_data);
}

} // namespace luci_interpreter
