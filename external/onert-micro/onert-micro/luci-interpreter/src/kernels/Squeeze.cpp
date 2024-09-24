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
#include "SISOKernel.h"

#include <cassert>

namespace luci_interpreter
{

void configure_kernel_CircleSqueeze(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  assert(cur_op->inputs()->size() == 1);

  const circle::Tensor *input = kernel.input();
  const circle::Tensor *output = kernel.output();

  assert(Tensor::num_elements(input) == Tensor::num_elements(output));

  const uint8_t *input_data = runtime_graph->getDataByTensor(input);
  uint8_t *output_data = runtime_graph->getDataByTensor(output);

  int input_num_dims = kernels::getTensorShape(input).dimensionsCount();

  // Get parameters
  const circle::SqueezeOptions *op_params = cur_op->builtin_options_as_SqueezeOptions();

  // Verification of the Squeeze parameters
  int num_squeeze_dims = op_params->squeeze_dims()->size();
  assert(input_num_dims <= 8);
  bool should_squeeze[8] = {false};
  int num_squeezed_dims = 0;
  if (num_squeeze_dims == 0)
  {
    for (int idx = 0; idx < input_num_dims; ++idx)
    {

      if (kernels::getTensorShape(input).dims(idx) == 1)
      {
        should_squeeze[idx] = true;
        ++num_squeezed_dims;
      }
    }
  }
  else
  {
    for (int idx = 0; idx < num_squeeze_dims; ++idx)
    {
      int current = (*op_params->squeeze_dims())[idx] < 0
                      ? (*op_params->squeeze_dims())[idx] + input_num_dims
                      : (*op_params->squeeze_dims())[idx];
      assert(current >= 0 && current < input_num_dims &&
             kernels::getTensorShape(input).dims(current) == 1);
      if (!should_squeeze[current])
        ++num_squeezed_dims;
      should_squeeze[current] = true;
    }
  }
}

void execute_kernel_CircleSqueeze(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  assert(cur_op->inputs()->size() == 1);

  const circle::Tensor *input = kernel.input();
  const circle::Tensor *output = kernel.output();

  const uint8_t *input_data = runtime_graph->getDataByTensor(input);
  uint8_t *output_data = runtime_graph->getDataByTensor(output);

  assert(input_data != nullptr);
  assert(output_data != nullptr);

  assert(Tensor::num_elements(input) == Tensor::num_elements(output));

  std::memcpy(output_data, input_data,
              getDataTypeSize(Tensor::element_type(input)) * Tensor::num_elements(input));
}

} // namespace luci_interpreter
