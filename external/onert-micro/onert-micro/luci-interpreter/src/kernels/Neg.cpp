/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "PALNeg.h"

namespace luci_interpreter
{

void configure_kernel_CircleNeg(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto output_index = cur_op->outputs()->operator[](0);

  assert(input_index != -1);
  assert(output_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(input) == Tensor::element_type(output));

  assert(Tensor::num_dims(input) == 4);

  // TODO: enable it only if kernel with dynamic shapes
  // output-> resize(input->shape());
}

void execute_kernel_CircleNeg(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto input_index = cur_op->inputs()->operator[](0);
  const auto output_index = cur_op->outputs()->operator[](0);

  assert(input_index != -1);
  assert(output_index != -1);

  const auto input = runtime_graph->getCircleTensorByIndex(input_index);
  const auto output = runtime_graph->getCircleTensorByIndex(output_index);

  const uint8_t *input_data = runtime_graph->getDataByTensor(input);
  uint8_t *output_data = runtime_graph->getDataByTensor(output);

  assert(input_data != nullptr);
  assert(output_data != nullptr);

  switch (Tensor::element_type(input))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:

      luci_interpreter_pal::Negate(
        kernels::getTensorShape(input), kernels::getTensorData<float>(input_data),
        kernels::getTensorShape(output), kernels::getTensorData<float>(output_data));

      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
