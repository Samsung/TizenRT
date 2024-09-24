/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "kernels/BinaryOpCommon.h"

#include "PALResizeBilinear.h"

namespace luci_interpreter
{

/*
 * ResizeBilinear Kernel:
 * Description: resizing input Tensor by input constants using Bilinear Interpolation
 * 2 Inputs: Input tensor ( Shape dimensions count = 4); Input constant (Shape dimensions count = 1,
 * Num elements =2) Parameters: align_corners; half_pixel_centers;
 *
 * Example:
 *                       Input(2, 2, 2, 1)
 *                               |
 *                               |   Constant Input(2) [3,3] INT32
 *                               |  /
 *                          ResizeBilinear
 *                                 |
 *                         Output(2, 3, 3, 1) UINT8
 */

void configure_kernel_CircleResizeBilinear(const circle::Operator *cur_op,
                                           BaseRuntimeGraph *runtime_graph)
{
  // Check of the size of input. Should be 2
  assert(cur_op->inputs()->size() == 2);
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto size_index = cur_op->inputs()->operator[](1);
  const auto output_index = cur_op->outputs()->operator[](0);

  assert(input_index != -1);
  assert(size_index != -1);
  assert(output_index != -1);
  // Get tensors
  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto size = runtime_graph->getCircleTensorByIndex(size_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);
  // Check of the Input shape
  assert(kernels::getTensorShape(input).dimensionsCount() == 4);
  // Check of the Const input size shape
  assert(kernels::getTensorShape(size).dimensionsCount() == 1);
  assert(Tensor::element_type(size) == DataType::S32);
  assert(kernels::getTensorShape(size).dims(0) == 2);

  const auto *params = cur_op->builtin_options_as_ResizeBilinearOptions();
  if (params->half_pixel_centers() && params->align_corners())
    assert(false && "If half_pixel_centers is True, align_corners must be False.");
}

void execute_kernel_CircleResizeBilinear(const circle::Operator *cur_op,
                                         BaseRuntimeGraph *runtime_graph)
{
  assert(cur_op->inputs()->size() == 2);
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto size_index = cur_op->inputs()->operator[](1);
  const auto output_index = cur_op->outputs()->operator[](0);

  assert(input_index != -1);
  assert(size_index != -1);
  assert(output_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto size = runtime_graph->getCircleTensorByIndex(size_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  const uint8_t *input_data = runtime_graph->getDataByTensor(input);
  const uint8_t *size_data = runtime_graph->getConstDataByTensor(size);
  uint8_t *output_data = runtime_graph->getDataByTensor(output);

  assert(input_data != nullptr);
  assert(size_data != nullptr);
  assert(output_data != nullptr);

  // Get parameters
  const auto *op_params = cur_op->builtin_options_as_ResizeBilinearOptions();

  switch (Tensor::element_type(output))
  {
    case DataType::FLOAT32:
      luci_interpreter_pal::ResizeBilinear(
        op_params, kernels::getTensorShape(input), kernels::getTensorData<float>(input_data),
        kernels::getTensorShape(size), kernels::getTensorData<int32_t>(size_data),
        kernels::getTensorShape(output), kernels::getTensorData<float>(output_data));
      break;
    case DataType::U8:
      luci_interpreter_pal::ResizeBilinear(
        op_params, kernels::getTensorShape(input), kernels::getTensorData<uint8_t>(input_data),
        kernels::getTensorShape(size), kernels::getTensorData<int32_t>(size_data),
        kernels::getTensorShape(output), kernels::getTensorData<uint8_t>(output_data));
      break;
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
