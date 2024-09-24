/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2017 The TensorFlow Authors. All Rights Reserved.
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

#include "Pool2DCommon.h"

#include "PALL2Pool2D.h"

namespace luci_interpreter
{
void configure_kernel_CircleL2Pool2D(const circle::Operator *cur_op,
                                     BaseRuntimeGraph *runtime_graph)
{
  configure_kernel_CirclePool2DCommon(cur_op, runtime_graph);
}

void execute_kernel_CircleL2Pool2D(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const kernels::SISOKernel siso_kernel(cur_op, runtime_graph);

  const auto input = siso_kernel.input();
  const auto output = siso_kernel.output();

  const auto *input_data = runtime_graph->getDataByTensor(input);
  auto *output_data = runtime_graph->getDataByTensor(output);

  const DataType input_type = Tensor::element_type(input);

  const auto params = createPoolParams(cur_op, siso_kernel);

  switch (input_type)
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      luci_interpreter_pal::L2Pool(
        params, kernels::getTensorShape(input), kernels::getTensorData<float>(input_data),
        kernels::getTensorShape(output), kernels::getTensorData<float>(output_data));
      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
