/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "PALSquaredDifference.h"

namespace luci_interpreter
{

void configure_kernel_CircleSquaredDifference(const circle::Operator *cur_op,
                                              BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.input2()));
  LUCI_INTERPRETER_CHECK(Tensor::num_elements(kernel.input1()) ==
                         Tensor::num_elements(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_elements(kernel.input1()) ==
                         Tensor::num_elements(kernel.input2()));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input1()) == Tensor::num_dims(kernel.output()));
}

void execute_kernel_CircleSquaredDifference(const circle::Operator *cur_op,
                                            BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  const auto *input_data_1 = runtime_graph->getDataByTensor(kernel.input1());
  const auto *input_data_2 = runtime_graph->getDataByTensor(kernel.input2());
  assert(input_data_1);
  assert(input_data_2);

  auto *output_data = runtime_graph->getDataByTensor(kernel.output());

  switch (Tensor::element_type(kernel.input1()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      const float *input_data_1_float = kernels::getTensorData<float>(input_data_1);
      const float *input_data_2_float = kernels::getTensorData<float>(input_data_2);
      float *output_data_float = kernels::getTensorData<float>(output_data);

      assert(output_data_float);

      const int flat_size =
        kernels::getTensorRuntimeShape(kernel.input1(), runtime_graph).flatSize();

      luci_interpreter_pal::SquaredDifference(flat_size, input_data_1_float, input_data_2_float,
                                              output_data_float);
      break;
    }
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type");
  }
}
} // namespace luci_interpreter
