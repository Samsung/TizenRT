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

#include "OMStatus.h"

#include "core/OMUtils.h"
#include "core/OMKernelData.h"

#include "import/OMKernelConfigureBuilder.h"

#include "execute/OMRuntimeKernel.h"
#include "execute/OMUtils.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{
constexpr int kInputTensorIdx = 0;
constexpr int kPermTensorIdx = 1;
constexpr int kOutputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleTranspose(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  const circle::Tensor *input = runtime_kernel.inputs[kInputTensorIdx];
  const circle::Tensor *perm = runtime_kernel.inputs[kPermTensorIdx];
  const circle::Tensor *output = runtime_kernel.outputs[kOutputTensorIdx];

  assert(input != nullptr);
  assert(perm != nullptr);
  assert(output != nullptr);

  OMStatus status = Ok;

  status = utils::checkCondition(perm->type() == circle::TensorType_INT32);
  if (status != Ok)
    return status;

  core::OMRuntimeShape input_shape(input);
  core::OMRuntimeShape perm_shape(perm);

  status = utils::checkCondition(perm_shape.dimensionsCount() == 1);
  if (status != Ok)
    return status;

  status = utils::checkCondition(perm_shape.dims(0) == input_shape.dimensionsCount());
  if (status != Ok)
    return status;

  return status;
}

} // namespace import
} // namespace onert_micro
