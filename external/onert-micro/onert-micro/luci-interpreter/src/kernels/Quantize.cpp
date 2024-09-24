/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "PALQuantize.h"

namespace luci_interpreter
{

void configure_kernel_CircleQuantize(const circle::Operator *cur_op,
                                     BaseRuntimeGraph *runtime_graph)
{
#ifndef DIS_QUANT
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::num_elements(kernel.input()) ==
                         Tensor::num_elements(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input()) == Tensor::num_dims(kernel.output()));
  LUCI_INTERPRETER_CHECK(!Tensor::scales(kernel.output()).empty());
  LUCI_INTERPRETER_CHECK(!Tensor::zero_points(kernel.output()).empty());
#else
  assert(false && "Remove DIS_QUANT flag");
#endif // DIS_QUANT
}

void execute_kernel_CircleQuantize(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
#ifndef DIS_QUANT
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  const auto *input_data = runtime_graph->getDataByTensor(kernel.input());
  assert(input_data);

  auto *output_data = runtime_graph->getDataByTensor(kernel.output());
  assert(output_data);

  const int flat_size = kernels::getTensorRuntimeShape(kernel.input(), runtime_graph).flatSize();

  switch (Tensor::element_type(kernel.input()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      luci_interpreter_pal::QuantizationParams params{};
      params.zero_point = Tensor::zero_point(kernel.output());
      params.scale = Tensor::scale(kernel.output());
      switch (Tensor::element_type(kernel.output()))
      {
        case DataType::S8:
          luci_interpreter_pal::Quantize(params, flat_size,
                                         kernels::getTensorData<float>(input_data),
                                         kernels::getTensorData<int8_t>(output_data));
          break;
        case DataType::U8:
          luci_interpreter_pal::Quantize(params, flat_size,
                                         kernels::getTensorData<float>(input_data),
                                         kernels::getTensorData<uint8_t>(output_data));
          break;
        case DataType::S16:
          luci_interpreter_pal::Quantize(params, flat_size,
                                         kernels::getTensorData<float>(input_data),
                                         kernels::getTensorData<int16_t>(output_data));
          break;
        default:
          assert(false && "Unsupported type");
      }
      break;
    }
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type");
  }
#else
  assert(false && "Remove DIS_QUANT flag");
#endif // DIS_QUANT
}

} // namespace luci_interpreter
