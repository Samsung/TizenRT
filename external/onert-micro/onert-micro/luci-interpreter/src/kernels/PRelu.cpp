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
#include "TISOKernel.h"

#include "PALPreluCommon.h"

namespace luci_interpreter
{

void configure_kernel_CirclePRelu(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.input2()));

  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input1()) == Tensor::num_dims(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_elements(kernel.input1()) ==
                         Tensor::num_elements(kernel.output()));
}

void execute_kernel_CirclePRelu(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);
  kernels::TISOData kernel_data = kernel.readData();

  switch (Tensor::element_type(kernel.input1()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      const float *input_data_float = kernels::getTensorData<float>(kernel_data.input1_data);
      const float *alpha_data_float = kernels::getTensorData<float>(kernel_data.input2_data);
      float *output_data_float = kernels::getTensorData<float>(kernel_data.output_data);
      assert(output_data_float);
      assert(input_data_float);
      assert(alpha_data_float);

      luci_interpreter_pal::BroadcastPrelu4DSlowFloat(
        kernels::getTensorRuntimeShape(kernel.input1(), runtime_graph), input_data_float,
        kernels::getTensorRuntimeShape(kernel.input2(), runtime_graph), alpha_data_float,
        kernels::getTensorRuntimeShape(kernel.output(), runtime_graph), output_data_float);
      break;
    }
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type");
  }
}

} // namespace luci_interpreter
