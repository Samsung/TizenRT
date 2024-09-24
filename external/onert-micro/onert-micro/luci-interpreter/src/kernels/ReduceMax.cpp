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
#include "TISOKernel.h"

#include "PALReduceCommon.h"

#include <cassert>

namespace luci_interpreter
{
namespace
{

template <typename T>
void reduceMaxGeneric(kernels::TISOData *tiso_data, const circle::Tensor *input,
                      const circle::Tensor *axis, const circle::Tensor *output)
{
  const int input_rank = Tensor::num_dims(input);
  const int num_axis = Tensor::num_elements(axis);

  auto const input_dims = wrap(input->shape());
  const auto output_shape = kernels::getTensorShape(output);

  luci_interpreter_pal::ReduceGeneric<T>(
    kernels::getTensorData<T>(tiso_data->input1_data),
    reinterpret_cast<const int *>(input_dims.data()), input_rank,
    kernels::getTensorData<T>(tiso_data->output_data),
    kernels::getTensorData<int>(tiso_data->input2_data), num_axis,
    /*init_value=*/std::numeric_limits<T>::lowest(), output_shape.flatSize(),
    [](const float current, const float in) -> float { return (in > current) ? in : current; });
}

} // namespace

void configure_kernel_CircleReduceMax(const circle::Operator *cur_op,
                                      BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input1()) == Tensor::num_dims(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input2()) == DataType::S32);

  const int32_t axis_value =
    kernels::getTensorData<int>(runtime_graph->getConstDataByTensor(kernel.input2()))[0];
  LUCI_INTERPRETER_CHECK(axis_value >= 0);
}

void execute_kernel_CircleReduceMax(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);
  kernels::TISOData tiso_data = kernel.readData();

  const auto *input = kernel.input1();
  const auto *axis = kernel.input2();
  const auto *output = kernel.output();

  switch (Tensor::element_type(kernel.input1()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      reduceMaxGeneric<float>(&tiso_data, input, axis, output);
      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type");
  }
}

} // namespace luci_interpreter
