/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "PALFloorMod.h"

namespace luci_interpreter
{

void configure_kernel_CircleFloorMod(const circle::Operator *cur_op,
                                     BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  CheckBinaryOpDataTypesEqual(kernel);
}

void execute_kernel_CircleFloorMod(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  const auto *options = cur_op->builtin_options_as_FloorModOptions();

  luci_interpreter::RuntimeShape input_shape1 =
    kernels::getTensorRuntimeShape(kernel.input1(), runtime_graph);
  luci_interpreter::RuntimeShape input_shape2 =
    kernels::getTensorRuntimeShape(kernel.input2(), runtime_graph);
  luci_interpreter::RuntimeShape output_shape =
    kernels::getTensorRuntimeShape(kernel.output(), runtime_graph);

  const uint8_t *input_data1 = runtime_graph->getDataByTensor(kernel.input1());
  const uint8_t *input_data2 = runtime_graph->getDataByTensor(kernel.input2());
  uint8_t *output_data = runtime_graph->getDataByTensor(kernel.output());

  assert(input_data1 != nullptr);
  assert(input_data2 != nullptr);
  assert(output_data != nullptr);

  switch (Tensor::element_type(kernel.input1()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      // Check the denominator
      for (int i = 0; i < input_shape2.flatSize(); ++i)
      {
        LUCI_INTERPRETER_CHECK(kernels::getTensorData<float>(input_data2)[i] != 0);
      }
      // check that input and output dimensions are equal
      if (kernels::areShapesEqual(input_shape1, input_shape2))
      {
        const int flat_size = input_shape1.flatSize();
        luci_interpreter_pal::FloorMod(flat_size, kernels::getTensorData<float>(input_data1),
                                       kernels::getTensorData<float>(input_data2),
                                       kernels::getTensorData<float>(output_data));
      }
      else
      {
        luci_interpreter_pal::BroadcastFloorMod4DSlow(
          input_shape1, kernels::getTensorData<float>(input_data1), input_shape2,
          kernels::getTensorData<float>(input_data2), output_shape,
          kernels::getTensorData<float>(output_data));
      }
    }
    break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
