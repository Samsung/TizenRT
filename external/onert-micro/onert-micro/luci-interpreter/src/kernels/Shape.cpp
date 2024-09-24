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
#include "SISOKernel.h"
#include "kernels/Utils.h"

namespace luci_interpreter
{
void configure_kernel_CircleShape(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.output()) == DataType::S32);
}

void execute_kernel_CircleShape(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  const circle::Tensor *input = kernel.input();
  const circle::Tensor *output = kernel.output();

  assert(Tensor::element_type(output) == DataType::S32);
  int32_t *output_data = kernels::getTensorData<int32_t>(runtime_graph->getDataByTensor(output));

  const int rank = Tensor::num_dims(input);
  for (int i = 0; i < rank; ++i)
  {
    output_data[i] = Tensor::dim(input, i);
  }
}

} // namespace luci_interpreter
