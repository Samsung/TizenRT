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

#include "Builders.h"
#include "kernels/Utils.h"
#include "TISOKernel.h"

#include "PALMean.h"

#include <cassert>

namespace luci_interpreter
{
namespace
{
const int kMaxNumberOfAxis = 5;
const int kMaxNumberOfReducedAxis = 2;

void ResolveAxis(const int *axis_data, int axis_count, luci_interpreter_pal::MeanParams *op_params)
{
  int i = 0;
  for (; i < axis_count; ++i)
  {
    op_params->axis[i] = static_cast<int16_t>(axis_data[i]);
  }
  for (; i < 4; ++i)
  {
    op_params->axis[i] = 1;
  }
  op_params->axis_count = axis_count;
}

} // namespace

void configure_kernel_CircleMean(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input2()) == DataType::S32);

  const int32_t axis_value =
    kernels::getTensorData<int>(runtime_graph->getConstDataByTensor(kernel.input2()))[0];
  LUCI_INTERPRETER_CHECK(axis_value >= 0);
}

void execute_kernel_CircleMean(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);
  kernels::TISOData tiso_data = kernel.readData();

  const auto *input = kernel.input1();
  const auto *axis = kernel.input2();
  const auto *output = kernel.output();

  const auto *options = cur_op->builtin_options_as_ReducerOptions();

  int num_axis = static_cast<int>(Tensor::num_elements(axis));
  int temp_index[kMaxNumberOfAxis];
  int resolved_axis[kMaxNumberOfReducedAxis];

  switch (Tensor::element_type(kernel.input1()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      luci_interpreter_pal::MeanParams op_params;
      ResolveAxis(kernels::getTensorData<int>(tiso_data.input2_data), num_axis, &op_params);

      // Special case mean implementation exists for 4D mean across axes 1
      // and 2.
      bool special_case_4d_axes_1_and_2 = Tensor::num_dims(input) == 4 &&
                                          op_params.axis_count == 2 &&
                                          ((op_params.axis[0] == 1 && op_params.axis[1] == 2) ||
                                           (op_params.axis[0] == 2 && op_params.axis[1] == 1));

      // Defer to specialized implementation for 4D Mean across axes 1 & 2.
      if (options->keep_dims() && special_case_4d_axes_1_and_2)
      {
        luci_interpreter_pal::Mean(op_params, kernels::getTensorShape(input),
                                   kernels::getTensorData<float>(tiso_data.input1_data),
                                   kernels::getTensorShape(output),
                                   kernels::getTensorData<float>(tiso_data.output_data));
      }
      else
      {
        luci_interpreter_pal::Mean(
          kernels::getTensorData<float>(tiso_data.input1_data),
          reinterpret_cast<const int *>(wrap(input->shape()).data()), Tensor::num_dims(input),
          kernels::getTensorData<float>(tiso_data.output_data),
          reinterpret_cast<const int *>(wrap(output->shape()).data()), Tensor::num_dims(output),
          kernels::getTensorData<int>(tiso_data.input2_data), num_axis, options->keep_dims(),
          temp_index, resolved_axis, kernels::getTensorData<float>(tiso_data.output_data));
      }
    }
    break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type");
  }
}

} // namespace luci_interpreter
