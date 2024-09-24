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

#ifndef LUCI_INTERPRETER_KERNELS_MISO_KERNEL_H
#define LUCI_INTERPRETER_KERNELS_MISO_KERNEL_H

#include "Builders.h"

namespace luci_interpreter
{
namespace kernels
{

// Multiple input single output kernel
class MISOKernel
{
public:
  MISOKernel(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
  {
    const auto input1_index = cur_op->inputs()->operator[](0);
    const auto input2_index = cur_op->inputs()->operator[](1);
    const auto input3_index = cur_op->inputs()->operator[](2);
    const auto input4_index =
      cur_op->inputs()->size() == 4 ? cur_op->inputs()->operator[](3) : -1; // optional

    const auto output_index = cur_op->outputs()->operator[](0);

    assert(input1_index != -1);
    assert(input2_index != -1);
    assert(input3_index != -1);

    assert(output_index != -1);

    _input1_tensor = runtime_graph->getCircleTensorByIndex(input1_index);
    _input2_tensor = runtime_graph->getCircleTensorByIndex(input2_index);
    _input3_tensor = runtime_graph->getCircleTensorByIndex(input3_index);
    _output_tensor = runtime_graph->getCircleTensorByIndex(output_index);

    // optional
    if (input4_index != -1)
      _input4_tensor = runtime_graph->getCircleTensorByIndex(input4_index);
    else
      _input4_tensor = nullptr;

    assert(_input1_tensor != nullptr);
    assert(_input2_tensor != nullptr);
    assert(_input3_tensor != nullptr);
    assert(_output_tensor != nullptr);
  }

  const circle::Tensor *input1() const { return _input1_tensor; }
  const circle::Tensor *input2() const { return _input2_tensor; }
  const circle::Tensor *input3() const { return _input3_tensor; }

  const circle::Tensor *input4() const
  {
    assert(_input4_tensor != nullptr);
    return _input4_tensor;
  }

  const circle::Tensor *output() const { return _output_tensor; }

private:
  const circle::Tensor *_input1_tensor;
  const circle::Tensor *_input2_tensor;
  const circle::Tensor *_input3_tensor;
  const circle::Tensor *_input4_tensor; // optional
  const circle::Tensor *_output_tensor;
};

} // namespace kernels
} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNELS_MISO_KERNEL_H
