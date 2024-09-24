/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2018 The TensorFlow Authors. All Rights Reserved.
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
#include "TISOKernel.h"

#include "PALLogicalCommon.h"

namespace luci_interpreter
{
namespace
{
bool LogicalAnd(bool x, bool y) { return x && y; }
} // namespace

void configure_kernel_CircleLogicalAnd(const circle::Operator *cur_op,
                                       BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.input2()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) == DataType::BOOL);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.output()) == DataType::BOOL);

  // TODO support broadcast
  LUCI_INTERPRETER_CHECK(Tensor::num_elements(kernel.input1()) ==
                         Tensor::num_elements(kernel.input2()));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input1()) == Tensor::num_dims(kernel.input2()));
}

// TODO: add inplace
// TODO: reduce code duplication with LogicalOr
void execute_kernel_CircleLogicalAnd(const circle::Operator *cur_op,
                                     BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  auto x_data = kernels::getTensorData<bool>(runtime_graph->getDataByTensor(kernel.input1()));
  if (x_data == nullptr)
    x_data = kernels::getTensorData<bool>(runtime_graph->getConstDataByTensor(kernel.input1()));

  assert(x_data != nullptr);

  auto y_data = kernels::getTensorData<bool>(runtime_graph->getDataByTensor(kernel.input2()));
  if (y_data == nullptr)
    y_data = kernels::getTensorData<bool>(runtime_graph->getConstDataByTensor(kernel.input2()));

  assert(y_data != nullptr);

  auto output_data = kernels::getTensorData<bool>(runtime_graph->getDataByTensor(kernel.output()));

  const int64_t flat_size = kernels::getTensorShape(kernel.input1()).flatSize();
  luci_interpreter_pal::LogicalCommon(flat_size, x_data, y_data, output_data, LogicalAnd);
}

} // namespace luci_interpreter
