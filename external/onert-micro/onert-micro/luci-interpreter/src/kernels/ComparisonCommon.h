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

#ifndef LUCI_INTERPRETER_KERNELS_COMPARISONCOMMON_H
#define LUCI_INTERPRETER_KERNELS_COMPARISONCOMMON_H

#include "Builders.h"

#include "kernels/Utils.h"
#include "PALComparisons.h"

namespace luci_interpreter
{
namespace kernels
{

template <typename T>
void evalComparisonGeneric(const circle::Tensor *x, const circle::Tensor *y,
                           const circle::Tensor *output, BaseRuntimeGraph *runtime_graph,
                           bool F(T, T))
{
  auto x_data = kernels::getTensorData<T>(runtime_graph->getDataByTensor(x));
  if (x_data == nullptr)
    x_data = kernels::getTensorData<T>(runtime_graph->getConstDataByTensor(x));

  assert(x_data != nullptr);

  auto y_data = kernels::getTensorData<T>(runtime_graph->getDataByTensor(y));
  if (y_data == nullptr)
    y_data = kernels::getTensorData<T>(runtime_graph->getConstDataByTensor(y));

  assert(y_data != nullptr);

  auto output_data = kernels::getTensorData<bool>(runtime_graph->getDataByTensor(output));

  luci_interpreter_pal::ComparisonParams op_params;
  op_params.is_broadcast = Tensor::num_elements(x) != Tensor::num_elements(y);

  if (op_params.is_broadcast)
  {
    luci_interpreter_pal::BroadcastComparison4DSlowNoScaling<T>(
      op_params, kernels::getTensorShape(x), x_data, kernels::getTensorShape(y), y_data,
      kernels::getTensorShape(output), output_data, F);
  }
  else
  {
    const int64_t flat_size = kernels::getTensorShape(x).flatSize();
    luci_interpreter_pal::ComparisonNoScaling<T>(flat_size, x_data, y_data, output_data, F);
  }
}

} // namespace kernels
} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNELS_COMPARISONCOMMON_H
