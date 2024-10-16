/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2021 The TensorFlow Authors. All Rights Reserved.
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
#include "PALGatherND.h"

#include <cassert>

namespace luci_interpreter
{

void configure_kernel_CircleGatherND(const circle::Operator *cur_op,
                                     BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input2()) == DataType::S32);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.output()));

  const int params_rank = Tensor::num_dims(kernel.input1());
  const int indices_rank = Tensor::num_dims(kernel.input2());
  const int indices_nd = Tensor::dim(kernel.input2(), indices_rank - 1);

  LUCI_INTERPRETER_CHECK(params_rank >= 1);
  LUCI_INTERPRETER_CHECK(indices_rank >= 1);
  LUCI_INTERPRETER_CHECK(indices_nd <= params_rank);
  LUCI_INTERPRETER_CHECK(indices_nd <= luci_interpreter_pal::MAX_INDICES_ND);
}

void execute_kernel_CircleGatherND(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  const uint8_t *params_data = runtime_graph->getDataByTensor(kernel.input1());
  const uint8_t *indies_data = runtime_graph->getConstDataByTensor(kernel.input2());
  uint8_t *output_data = runtime_graph->getDataByTensor(kernel.output());

  switch (Tensor::element_type(kernel.input1()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      return luci_interpreter_pal::GatherND<float, int32_t>(
        kernels::getTensorRuntimeShape(kernel.input1(), runtime_graph),
        kernels::getTensorData<float>(params_data),
        kernels::getTensorRuntimeShape(kernel.input2(), runtime_graph),
        kernels::getTensorData<int32_t>(indies_data), kernels::getTensorData<float>(output_data));
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type");
  }
}

} // namespace luci_interpreter
