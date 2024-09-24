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

namespace
{
#ifndef DIS_QUANT
void evalQuantized(const circle::Tensor *x, const circle::Tensor *y, const circle::Tensor *output,
                   BaseRuntimeGraph *runtime_graph)
{
  auto x_data = kernels::getTensorData<uint8_t>(runtime_graph->getDataByTensor(x));
  if (x_data == nullptr)
    x_data = kernels::getTensorData<uint8_t>(runtime_graph->getConstDataByTensor(x));

  assert(x_data != nullptr);

  auto y_data = kernels::getTensorData<uint8_t>(runtime_graph->getDataByTensor(y));
  if (y_data == nullptr)
    y_data = kernels::getTensorData<uint8_t>(runtime_graph->getConstDataByTensor(y));

  assert(y_data != nullptr);

  auto output_data = kernels::getTensorData<bool>(runtime_graph->getDataByTensor(output));

  int32_t x_multiplier;
  int x_shift;

  int32_t y_multiplier;
  int y_shift;

  kernels::quantizeMultiplierSmallerThanOneExp(Tensor::scale(x), &x_multiplier, &x_shift);
  kernels::quantizeMultiplierSmallerThanOneExp(Tensor::scale(y), &y_multiplier, &y_shift);

  luci_interpreter_pal::ComparisonParams op_params;
  op_params.left_shift = 8;
  op_params.input1_offset = -Tensor::zero_point(x); // Note the '-'
  op_params.input1_shift = x_shift;
  op_params.input1_multiplier = x_multiplier;
  op_params.input2_offset = -Tensor::zero_point(y); // Note the '-'
  op_params.input2_shift = y_shift;
  op_params.input2_multiplier = y_multiplier;
  op_params.is_broadcast = Tensor::num_elements(x) != Tensor::num_elements(y);

  if (op_params.is_broadcast)
  {
    luci_interpreter_pal::BroadcastComparison4DSlowWithScaling<uint8_t>(
      op_params, kernels::getTensorShape(x), x_data, kernels::getTensorShape(y), y_data,
      kernels::getTensorShape(output), output_data, luci_interpreter_pal::LessFn);
  }
  else
  {
    const int64_t flat_size = kernels::getTensorShape(x).flatSize();
    luci_interpreter_pal::ComparisonWithScaling<uint8_t>(op_params, flat_size, x_data, y_data,
                                                         output_data, luci_interpreter_pal::LessFn);
  }
}
#endif // DIS_QUANT

} // namespace

void configure_kernel_CircleLess(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.input2()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.output()) == DataType::BOOL);
}

void execute_kernel_CircleLess(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  switch (Tensor::element_type(kernel.input1()))
  {
    case DataType::S64:
      kernels::evalComparisonGeneric<int64_t>(kernel.input1(), kernel.input2(), kernel.output(),
                                              runtime_graph, luci_interpreter_pal::LessFn);
      break;
    case DataType::S32:
      kernels::evalComparisonGeneric<int32_t>(kernel.input1(), kernel.input2(), kernel.output(),
                                              runtime_graph, luci_interpreter_pal::LessFn);
      break;
#ifndef DIS_QUANT
    case DataType::U8:
      evalQuantized(kernel.input1(), kernel.input2(), kernel.output(), runtime_graph);
      break;
#endif // DIS_QUANT
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      kernels::evalComparisonGeneric<float>(kernel.input1(), kernel.input2(), kernel.output(),
                                            runtime_graph, luci_interpreter_pal::LessFn);
      break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
