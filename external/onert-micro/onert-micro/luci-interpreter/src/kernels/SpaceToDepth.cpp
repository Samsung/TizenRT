/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "PALSpaceToDepth.h"

namespace luci_interpreter
{

void configure_kernel_CircleSpaceToDepth(const circle::Operator *cur_op,
                                         BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input()) ==
                         Tensor::element_type(kernel.output()));

  const auto *options = cur_op->builtin_options_as_SpaceToDepthOptions();

  const int32_t block_size = options->block_size();
  LUCI_INTERPRETER_CHECK(block_size > 0);

  constexpr int kHeightRank = 1;
  constexpr int kWidthRank = 2;
  constexpr int kDepthRank = 3;

  const int input_height = Tensor::dim(kernel.input(), kHeightRank);
  const int input_width = Tensor::dim(kernel.input(), kWidthRank);
  int output_height = input_height / block_size;
  int output_width = input_width / block_size;

  LUCI_INTERPRETER_CHECK(input_height == output_height * block_size);
  LUCI_INTERPRETER_CHECK(input_width == output_width * block_size);
}

void execute_kernel_CircleSpaceToDepth(const circle::Operator *cur_op,
                                       BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  const auto *options = cur_op->builtin_options_as_SpaceToDepthOptions();
  const int32_t block_size = options->block_size();

  switch (Tensor::element_type(kernel.input()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      assert(block_size != 0);
      luci_interpreter_pal::SpaceToDepth(
        block_size, kernels::getTensorRuntimeShape(kernel.input(), runtime_graph),
        kernels::getTensorData<float>(runtime_graph->getDataByTensor(kernel.input())),
        kernels::getTensorRuntimeShape(kernel.output(), runtime_graph),
        kernels::getTensorData<float>(runtime_graph->getDataByTensor(kernel.output())));
      break;
    }
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type");
  }
}
} // namespace luci_interpreter
