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

#ifndef LUCI_INTERPRETER_KERNELS_SISO_KERNEL_H
#define LUCI_INTERPRETER_KERNELS_SISO_KERNEL_H

#include "Builders.h"

namespace luci_interpreter
{
namespace kernels
{

// Single input single output kernel
class SISOKernel
{
public:
  SISOKernel(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
  {
    const auto input_index = cur_op->inputs()->operator[](0);
    const auto output_index = cur_op->outputs()->operator[](0);

    assert(input_index != -1);
    assert(output_index != -1);

    _input_tensor = runtime_graph->getCircleTensorByIndex(input_index);
    _output_tensor = runtime_graph->getCircleTensorByIndex(output_index);

    assert(_input_tensor != nullptr);
    assert(_output_tensor != nullptr);
  }

  const circle::Tensor *input() const { return _input_tensor; }
  const circle::Tensor *output() const { return _output_tensor; }

private:
  const circle::Tensor *_input_tensor;
  const circle::Tensor *_output_tensor;
};

} // namespace kernels
} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNELS_SISO_KERNEL_H
