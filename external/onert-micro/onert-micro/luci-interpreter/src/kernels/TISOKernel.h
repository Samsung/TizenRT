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

#ifndef LUCI_INTERPRETER_KERNELS_TISO_KERNEL_H
#define LUCI_INTERPRETER_KERNELS_TISO_KERNEL_H

#include "Builders.h"

namespace luci_interpreter
{
namespace kernels
{

struct TISOData
{
  uint8_t *input1_data = nullptr;
  uint8_t *input2_data = nullptr;
  uint8_t *output_data = nullptr;
};

// Two input single output kernel
class TISOKernel
{
public:
  TISOKernel() = delete;

  explicit TISOKernel(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
    : _runtime_graph(runtime_graph)
  {
    const auto input1_index = cur_op->inputs()->operator[](0);
    const auto input2_index = cur_op->inputs()->operator[](1);
    const auto output_index = cur_op->outputs()->operator[](0);

    assert(input1_index != -1);
    assert(input2_index != -1);
    assert(output_index != -1);

    _input1_tensor = _runtime_graph->getCircleTensorByIndex(input1_index);
    _input2_tensor = _runtime_graph->getCircleTensorByIndex(input2_index);
    _output_tensor = _runtime_graph->getCircleTensorByIndex(output_index);

    assert(_input1_tensor != nullptr);
    assert(_input2_tensor != nullptr);
    assert(_output_tensor != nullptr);
  }

  const circle::Tensor *input1() const { return _input1_tensor; }
  const circle::Tensor *input2() const { return _input2_tensor; }
  const circle::Tensor *output() const { return _output_tensor; }

  BaseRuntimeGraph *runtime_graph() const { return _runtime_graph; }

  TISOData readData()
  {
    auto *input1_data = _runtime_graph->getDataByTensor(_input1_tensor);
    if (input1_data == nullptr)
      input1_data = _runtime_graph->getConstDataByTensor(_input1_tensor);
    assert(input1_data);

    auto *input2_data = _runtime_graph->getDataByTensor(_input2_tensor);
    if (input2_data == nullptr)
      input2_data = _runtime_graph->getConstDataByTensor(_input2_tensor);
    assert(input2_data);

    auto *output_data = _runtime_graph->getDataByTensor(_output_tensor);
    assert(output_data);

    return {input1_data, input2_data, output_data};
  }

  TISOData readInplaceData(uint8_t *&inplace_data_ptr, circle::Tensor *&input_inplace_tensor)
  {
    auto *input1_data = _runtime_graph->getDataByTensor(_input1_tensor);
    if (input1_data != nullptr)
    {
      inplace_data_ptr = const_cast<uint8_t *>(input1_data);
      input_inplace_tensor = const_cast<circle::Tensor *>(_input1_tensor);
    }
    if (input1_data == nullptr)
      input1_data = _runtime_graph->getConstDataByTensor(_input1_tensor);

    assert(input1_data);

    auto *input2_data = _runtime_graph->getDataByTensor(_input2_tensor);
    if (inplace_data_ptr == nullptr)
    {
      assert(input2_data != nullptr);
      inplace_data_ptr = const_cast<uint8_t *>(input2_data);
      input_inplace_tensor = const_cast<circle::Tensor *>(_input2_tensor);
    }
    if (input2_data == nullptr)
      input2_data = _runtime_graph->getConstDataByTensor(_input2_tensor);
    assert(input2_data);

    assert(_runtime_graph->getDataByTensor(_output_tensor) == nullptr);

    auto *output_data = inplace_data_ptr;
    assert(output_data);

    return {input1_data, input2_data, output_data};
  }

private:
  const circle::Tensor *_input1_tensor;
  const circle::Tensor *_input2_tensor;
  const circle::Tensor *_output_tensor;

  BaseRuntimeGraph *_runtime_graph;
};

} // namespace kernels
} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNELS_TISO_KERNEL_H
