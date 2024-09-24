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
#include "TISOKernel.h"

#include "PALBroadcastTo.h"

namespace luci_interpreter
{
namespace
{
constexpr int kMaxDims = 5;
} // namespace

void configure_kernel_CircleBroadcastTo(const circle::Operator *cur_op,
                                        BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input2()) == DataType::S32 or
                         Tensor::element_type(kernel.input2()) == DataType::S64);

  // Ensure output dims is not less than input dims.
  int input_num_dims = Tensor::num_dims(kernel.input1());
  int output_num_dims = Tensor::num_dims(kernel.output());
  int shape_num_dims = Tensor::dim(kernel.input2(), 0);

  LUCI_INTERPRETER_CHECK(output_num_dims == shape_num_dims);
  LUCI_INTERPRETER_CHECK(input_num_dims <= output_num_dims);
  LUCI_INTERPRETER_CHECK(output_num_dims <= kMaxDims);
}

void execute_kernel_CircleBroadcastTo(const circle::Operator *cur_op,
                                      BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  const auto *input_data = runtime_graph->getDataByTensor(kernel.input1());
  assert(input_data);

  auto *output_data = runtime_graph->getDataByTensor(kernel.output());

  luci_interpreter_pal::BroadcastTo<kMaxDims>(
    kernels::getTensorRuntimeShape(kernel.input1(), runtime_graph), input_data,
    kernels::getTensorRuntimeShape(kernel.output(), runtime_graph), output_data,
    Tensor::element_type(kernel.input1()));
}
} // namespace luci_interpreter
