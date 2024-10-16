/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_KERNELS_CONVOLUTIONCOMMON_H
#define LUCI_INTERPRETER_KERNELS_CONVOLUTIONCOMMON_H

#include "Builders.h"
#include "Utils.h"

namespace luci_interpreter
{

int32_t computeConvPadding(const circle::Tensor *input, const circle::Tensor *filter,
                           circle::Padding padding, int32_t stride, int32_t dilation, int axis);

luci_interpreter_pal::ConvParams createConv2DParams(const circle::Tensor *input,
                                                    const circle::Tensor *filter,
                                                    const circle::Tensor *output,
                                                    const circle::Conv2DOptions *options);

namespace kernels
{

// Conv2D kernel for downsampling
class DownsamplingConv2DKernel
{
public:
  DownsamplingConv2DKernel() = delete;

  explicit DownsamplingConv2DKernel(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
    : _runtime_graph(runtime_graph)
  {
    const auto input_index = cur_op->inputs()->operator[](0);
    const auto filter_index = cur_op->inputs()->operator[](1);
    const auto bias_index = cur_op->inputs()->operator[](2);
    const auto output_index = cur_op->outputs()->operator[](0);

    assert(input_index != -1);
    assert(filter_index != -1);
    assert(output_index != -1);

    _input_tensor = _runtime_graph->getCircleTensorByIndex(input_index);
    _filter_tensor = _runtime_graph->getCircleTensorByIndex(filter_index);
    _bias_tensor = _runtime_graph->getCircleTensorByIndex(bias_index);
    _output_tensor = _runtime_graph->getCircleTensorByIndex(output_index);

    assert(_input_tensor != nullptr);
    assert(_filter_tensor != nullptr);
  }

  const circle::Tensor *input() const { return _input_tensor; }
  const circle::Tensor *filter() const { return _filter_tensor; }
  const circle::Tensor *bias() const { return _bias_tensor; }
  const circle::Tensor *output() const { return _output_tensor; }

  BaseRuntimeGraph *runtime_graph() const { return _runtime_graph; }

private:
  const circle::Tensor *_input_tensor;
  const circle::Tensor *_filter_tensor;
  const circle::Tensor *_bias_tensor;
  const circle::Tensor *_output_tensor;

  BaseRuntimeGraph *_runtime_graph;
};

} // namespace kernels
} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNELS_CONVOLUTIONCOMMON_H
