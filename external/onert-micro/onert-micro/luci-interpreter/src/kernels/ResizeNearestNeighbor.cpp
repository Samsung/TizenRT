/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2023 The TensorFlow Authors. All Rights Reserved.
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

#include "PALResizeNearestNeighbor.h"

namespace luci_interpreter
{

void configure_kernel_CircleResizeNearestNeighbor(const circle::Operator *cur_op,
                                                  BaseRuntimeGraph *runtime_graph)
{
  // Check of the size of input. Should be 2
  LUCI_INTERPRETER_CHECK(cur_op->inputs()->size() == 2);
  const kernels::TISOKernel tiso_kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(tiso_kernel.input1()) ==
                         Tensor::element_type(tiso_kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(tiso_kernel.input1()) == 4);
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(tiso_kernel.input2()) == 1);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(tiso_kernel.input2()) == DataType::S32);
  LUCI_INTERPRETER_CHECK(Tensor::dim(tiso_kernel.input2(), 0) == 2);

  const auto *params = cur_op->builtin_options_as_ResizeNearestNeighborOptions();
  if (params->half_pixel_centers() && params->align_corners())
    assert(false && "If half_pixel_centers is True, align_corners must be False.");
}

void execute_kernel_CircleResizeNearestNeighbor(const circle::Operator *cur_op,
                                                BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel tiso_kernel(cur_op, runtime_graph);
  kernels::TISOData tiso_data = tiso_kernel.readData();

  // Get parameters
  const auto *op_params = cur_op->builtin_options_as_ResizeNearestNeighborOptions();

  luci_interpreter_pal::ResizeNearestNeighborParams params;
  params.align_corners = op_params->align_corners();
  params.half_pixel_centers = false;

  const uint8_t *input_data = runtime_graph->getDataByTensor(tiso_kernel.input1());
  const uint8_t *size_data = runtime_graph->getConstDataByTensor(tiso_kernel.input2());
  uint8_t *output_data = runtime_graph->getDataByTensor(tiso_kernel.output());

  switch (Tensor::element_type(tiso_kernel.output()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      luci_interpreter_pal::ResizeNearestNeighbor(
        params, kernels::getTensorRuntimeShape(tiso_kernel.input1(), runtime_graph),
        kernels::getTensorData<float>(input_data),
        kernels::getTensorRuntimeShape(tiso_kernel.input2(), runtime_graph),
        kernels::getTensorData<int32_t>(size_data),
        kernels::getTensorRuntimeShape(tiso_kernel.output(), runtime_graph),
        kernels::getTensorData<float>(output_data));
      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
