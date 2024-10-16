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
#include "ComparisonCommon.h"
#include "TISOKernel.h"

namespace luci_interpreter
{

void configure_kernel_CircleEqual(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.input2()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.output()) == DataType::BOOL);
}

void execute_kernel_CircleEqual(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  switch (Tensor::element_type(kernel.input1()))
  {
    case DataType::S64:
      kernels::evalComparisonGeneric<int64_t>(kernel.input1(), kernel.input2(), kernel.output(),
                                              runtime_graph, luci_interpreter_pal::EqualFn);
      break;
    case DataType::S32:
      kernels::evalComparisonGeneric<int32_t>(kernel.input1(), kernel.input2(), kernel.output(),
                                              runtime_graph, luci_interpreter_pal::EqualFn);
      break;
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      kernels::evalComparisonGeneric<float>(kernel.input1(), kernel.input2(), kernel.output(),
                                            runtime_graph, luci_interpreter_pal::EqualFn);
      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
