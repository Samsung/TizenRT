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

#include "import/OMKernelConfigureBuilder.h"
#include "core/OMUtils.h"
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr int inputCond = 0;
constexpr int inputX = 1;
constexpr int inputY = 2;
constexpr int outputIndex = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleSelectV2(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  onert_micro::execute::OMRuntimeKernel runtime_kernel;

  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  const circle::Tensor *input_cond = runtime_kernel.inputs[inputCond];
  const circle::Tensor *input_x = runtime_kernel.inputs[inputX];
  const circle::Tensor *input_y = runtime_kernel.inputs[inputY];
  const circle::Tensor *output = runtime_kernel.outputs[outputIndex];

  assert(input_cond != nullptr);
  assert(input_x != nullptr);
  assert(input_y != nullptr);
  assert(output != nullptr);

  if (input_cond->type() != circle::TensorType_BOOL)
    return status;

  if (input_x->type() != input_y->type())
    return status;

  return status;
}

} // namespace import
} // namespace onert_micro
