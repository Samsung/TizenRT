/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "Builders.h"
#include "kernels/Utils.h"
#include "SISOKernel.h"

namespace luci_interpreter
{
namespace
{

template <typename FromT, typename ToT> void copyCast(const FromT *in, ToT *out, int num_elements)
{
  std::transform(in, in + num_elements, out, [](FromT a) { return static_cast<ToT>(a); });
}

} // namespace

void configure_kernel_CircleCast(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::num_elements(kernel.input()) ==
                         Tensor::num_elements(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input()) == Tensor::num_dims(kernel.output()));
}

void execute_kernel_CircleCast(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  const auto *input_data = runtime_graph->getDataByTensor(kernel.input());
  assert(input_data);

  auto *output_data = runtime_graph->getDataByTensor(kernel.output());
  assert(output_data);

  const int flat_size = kernels::getTensorRuntimeShape(kernel.input(), runtime_graph).flatSize();

  switch (Tensor::element_type(kernel.input()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      const float *input_data_float = kernels::getTensorData<float>(input_data);

      switch (Tensor::element_type(kernel.output()))
      {
        case DataType::S8:
          copyCast(input_data_float, kernels::getTensorData<int8_t>(output_data), flat_size);
          break;
        case DataType::S16:
          copyCast(input_data_float, kernels::getTensorData<int16_t>(output_data), flat_size);
          break;
        case DataType::S32:
          copyCast(input_data_float, kernels::getTensorData<int32_t>(output_data), flat_size);
          break;
        default:
          assert(false && "Not supported type");
      }
      break;
    }
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type");
  }
}
} // namespace luci_interpreter
