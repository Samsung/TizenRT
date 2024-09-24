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

#include "Builders.h"
#include "kernels/Utils.h"
#include "SISOKernel.h"

#include "PALRsqrt.h"

namespace luci_interpreter
{

void configure_kernel_CircleRsqrt(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input()) ==
                         Tensor::element_type(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_elements(kernel.input()) ==
                         Tensor::num_elements(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input()) == Tensor::num_dims(kernel.output()));
}

void execute_kernel_CircleRsqrt(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  const auto *input_data = runtime_graph->getDataByTensor(kernel.input());
  assert(input_data);

  auto *output_data = runtime_graph->getDataByTensor(kernel.output());

  bool is_inplace = runtime_graph->is_inplace_op(cur_op);

  switch (Tensor::element_type(kernel.input()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      const float *input_data_float = kernels::getTensorData<float>(input_data);
      float *output_data_float = kernels::getTensorData<float>(output_data);
      if (is_inplace)
      {
        output_data_float = const_cast<float *>(input_data_float);
      }

      assert(output_data_float);

      const int flat_size =
        kernels::getTensorRuntimeShape(kernel.input(), runtime_graph).flatSize();

      luci_interpreter_pal::Rsqrt(flat_size, input_data_float, output_data_float);
      break;
    }
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type");
  }

  if (is_inplace)
    runtime_graph->makeInplaceOperation(kernel.input(), kernel.output());
}
} // namespace luci_interpreter
