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
#include "PALAbs.h"
#include "SISOKernel.h"

namespace luci_interpreter
{

void configure_kernel_CircleAbs(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input()) ==
                         Tensor::element_type(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input()) == Tensor::num_dims(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_elements(kernel.input()) ==
                         Tensor::num_elements(kernel.output()));
}

void execute_kernel_CircleAbs(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  const auto input = kernel.input();
  const auto output = kernel.output();

  bool is_inplace = runtime_graph->is_inplace_op(cur_op);

  const uint8_t *input_data = runtime_graph->getDataByTensor(input);
  uint8_t *output_data = runtime_graph->getDataByTensor(output);

  if (is_inplace)
  {
    output_data = const_cast<uint8_t *>(input_data);
  }

  assert(input_data != nullptr);
  assert(output_data != nullptr);

  const int flat_size = kernels::getTensorRuntimeShape(input, runtime_graph).flatSize();

  switch (Tensor::element_type(input))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      luci_interpreter_pal::Abs(flat_size, kernels::getTensorData<float>(input_data),
                                kernels::getTensorData<float>(output_data));
      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }

  if (is_inplace)
  {
    runtime_graph->makeInplaceOperation(input, output);
  }
}

} // namespace luci_interpreter
