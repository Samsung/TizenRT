/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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
#include "TISOKernel.h"

#include "PALMirrorPad.h"

namespace luci_interpreter
{
namespace
{
constexpr int maxInputSize = 5;
}

void configure_kernel_CircleMirrorPad(const circle::Operator *cur_op,
                                      BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input2()) == 2);
  LUCI_INTERPRETER_CHECK(Tensor::dim(kernel.input2(), 0) == Tensor::num_dims(kernel.input1()));
}

void execute_kernel_CircleMirrorPad(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  kernels::TISOData data = kernel.readData();

  const auto *options = cur_op->builtin_options_as_MirrorPadOptions();

  const auto offset = options->mode() != circle::MirrorPadMode_REFLECT ? 0 : 1;
  const auto input_dims = Tensor::num_dims(kernel.input1());
  const auto output_size = Tensor::num_elements(kernel.output());

  int output_dims_num_elements[5];
  int input_dims_num_elements[5];

  for (int i = 0; i < input_dims; i++)
  {
    output_dims_num_elements[i] = 1;
    input_dims_num_elements[i] = 1;
  }

  for (int i = input_dims - 2; i >= 0; i--)
  {
    output_dims_num_elements[i] =
      output_dims_num_elements[i + 1] * Tensor::dim(kernel.output(), i + 1);

    input_dims_num_elements[i] =
      input_dims_num_elements[i + 1] * Tensor::dim(kernel.input1(), i + 1);
  }

  switch (Tensor::element_type(kernel.input1()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      luci_interpreter_pal::MirrorPad(
        Tensor::element_type(kernel.input2()), data.input2_data,
        wrap(kernel.input1()->shape()).data(), output_dims_num_elements, input_dims_num_elements,
        kernels::getTensorData<float>(data.input1_data),
        kernels::getTensorData<float>(data.output_data), offset, input_dims, output_size);

      break;
    }
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type");
  }
}

} // namespace luci_interpreter
