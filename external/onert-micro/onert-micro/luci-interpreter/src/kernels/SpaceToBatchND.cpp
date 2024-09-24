/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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
#include "MISOKernel.h"

#include "PALSpaceToBatchND.h"

namespace luci_interpreter
{

void configure_kernel_CircleSpaceToBatchND(const circle::Operator *cur_op,
                                           BaseRuntimeGraph *runtime_graph)
{
  kernels::MISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input2()) == DataType::S32);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input3()) == DataType::S32);
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.output()) >= 3);
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input1()) >= 3);
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.output()) <= 4);
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input1()) <= 4);
}

void execute_kernel_CircleSpaceToBatchND(const circle::Operator *cur_op,
                                         BaseRuntimeGraph *runtime_graph)
{
  kernels::MISOKernel kernel(cur_op, runtime_graph);
  const int32_t pad_value = 0;
  switch (Tensor::element_type(kernel.input1()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      luci_interpreter_pal::SpaceToBatchND(
        pad_value, kernels::getTensorRuntimeShape(kernel.input1(), runtime_graph),
        kernels::getTensorData<float>(runtime_graph->getDataByTensor(kernel.input1())),
        kernels::getTensorRuntimeShape(kernel.input2(), runtime_graph),
        kernels::getTensorData<int32_t>(runtime_graph->getConstDataByTensor(kernel.input2())),
        kernels::getTensorRuntimeShape(kernel.input3(), runtime_graph),
        kernels::getTensorData<int32_t>(runtime_graph->getConstDataByTensor(kernel.input3())),
        kernels::getTensorRuntimeShape(kernel.output(), runtime_graph),
        kernels::getTensorData<float>(runtime_graph->getDataByTensor(kernel.output())));
      break;
    }
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type");
  }
}
} // namespace luci_interpreter
