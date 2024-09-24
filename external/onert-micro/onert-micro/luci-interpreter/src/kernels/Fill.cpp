/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2020 The TensorFlow Authors. All Rights Reserved.
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
#include "TISOKernel.h"
#include "kernels/Utils.h"

namespace luci_interpreter
{
namespace
{

template <typename T> void fillImpl(const size_t flat_size, const T *value_data, T *output_data)
{
  for (int i = 0; i < flat_size; ++i)
  {
    output_data[i] = *value_data;
  }
}

} // namespace

void configure_kernel_CircleFill(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);
  // value tensor must be a scalar or has one element
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input2()) == 0 or
                         Tensor::num_elements(kernel.input2()) == 1);
  // value and output type must match
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input2()) ==
                         Tensor::element_type(kernel.output()));
}

void execute_kernel_CircleFill(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  const circle::Tensor *value = kernel.input2();
  const circle::Tensor *output = kernel.output();

  kernels::TISOData tiso_data = kernel.readData();
  const uint8_t *value_data = tiso_data.input2_data;
  uint8_t *output_data = tiso_data.output_data;

  const size_t flat_size = Tensor::num_elements(output);

  switch (Tensor::element_type(value))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      fillImpl<float>(flat_size, kernels::getTensorData<float>(value_data),
                      kernels::getTensorData<float>(output_data));
      break;
#endif // DIS_FLOAT
    case DataType::S32:
      fillImpl<int32_t>(flat_size, kernels::getTensorData<int32_t>(value_data),
                        kernels::getTensorData<int32_t>(output_data));
      break;
#ifndef DIS_QUANT
    case DataType::U8:
      fillImpl<uint8_t>(flat_size, kernels::getTensorData<uint8_t>(value_data),
                        kernels::getTensorData<uint8_t>(output_data));
      break;
#endif // DIS_QUANT
    default:
      assert(false && "Not impl yet");
  }
}

} // namespace luci_interpreter
