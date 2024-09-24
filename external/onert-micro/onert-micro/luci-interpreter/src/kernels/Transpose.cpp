/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "PALTranspose.h"

namespace luci_interpreter
{
void configure_kernel_CircleTranspose(const circle::Operator *cur_op,
                                      BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input2()) == DataType::S32);

  const int32_t dims = Tensor::num_dims(kernel.input1());
  const int32_t *perm_data =
    kernels::getTensorData<int32_t>(runtime_graph->getConstDataByTensor(kernel.input2()));

  // Ensure validity of the permutations tensor as a 1D tensor
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input2()) == 1);
  LUCI_INTERPRETER_CHECK(Tensor::dim(kernel.input2(), 0) == dims);

  for (int idx = 0; idx < dims; ++idx)
    LUCI_INTERPRETER_CHECK(perm_data[idx] >= 0 and perm_data[idx] < dims);
}

void execute_kernel_CircleTranspose(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  const circle::Tensor *input = kernel.input1();
  const circle::Tensor *perm = kernel.input2();
  const circle::Tensor *output = kernel.output();

  kernels::TISOData tiso_data = kernel.readData();
  const int32_t *perm_data = kernels::getTensorData<int32_t>(tiso_data.input2_data);

  const int32_t size = Tensor::dim(perm, 0);
  luci_interpreter_pal::TransposeParams params;
  params.perm_count = size;
  for (int i = 0; i < size; ++i)
    params.perm[i] = perm_data[i];

  switch (Tensor::element_type(input))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      luci_interpreter_pal::Transpose(params, kernels::getTensorShape(input),
                                      kernels::getTensorData<float>(tiso_data.input1_data),
                                      kernels::getTensorShape(output),
                                      kernels::getTensorData<float>(tiso_data.output_data));
      break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case DataType::U8:
      luci_interpreter_pal::Transpose(params, kernels::getTensorShape(input),
                                      kernels::getTensorData<uint8_t>(tiso_data.input1_data),
                                      kernels::getTensorShape(output),
                                      kernels::getTensorData<uint8_t>(tiso_data.output_data));
      break;
#endif // DIS_QUANT
    default:
      assert(false && "Unsupported type");
  }
}

} // namespace luci_interpreter
