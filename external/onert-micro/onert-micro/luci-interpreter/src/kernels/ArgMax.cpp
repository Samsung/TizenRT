/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2019 The TensorFlow Authors. All Rights Reserved.
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

#include "PALArgMinMax.h"

namespace luci_interpreter
{

void configure_kernel_CircleArgMax(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);
  // dim tensor must be a scalar or has one element
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input2()) == 0 or
                         Tensor::num_elements(kernel.input2()) == 1);
  // value and output type must match
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.output()) == DataType::S32);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input2()) == DataType::S32);
}

void execute_kernel_CircleArgMax(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  const circle::Tensor *input = kernel.input1();
  const circle::Tensor *output = kernel.output();

  kernels::TISOData tiso_data = kernel.readData();
  const auto input_data = tiso_data.input1_data;
  const auto axis_data = tiso_data.input2_data;
  auto output_data = tiso_data.output_data;

  switch (Tensor::element_type(input))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      luci_interpreter_pal::ArgMinMax(
        kernels::getTensorRuntimeShape(input, runtime_graph),
        kernels::getTensorData<float>(input_data), kernels::getTensorData<int32_t>(axis_data),
        kernels::getTensorRuntimeShape(output, runtime_graph),
        kernels::getTensorData<int32_t>(output_data), std::greater<float>());
    }
    break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported ArgMax input type");
  }
}

} // namespace luci_interpreter
