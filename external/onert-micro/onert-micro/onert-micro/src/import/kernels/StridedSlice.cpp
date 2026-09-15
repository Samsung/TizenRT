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

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t beginTensorIdx = 1;
constexpr uint32_t endTensorIdx = 2;
constexpr uint32_t stridesTensorIdx = 3;

constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleStridedSlice(const OMConfigureArgs &config_args)
{
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  execute::OMRuntimeKernel runtime_kernel;
  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  const circle::Tensor *input = runtime_kernel.inputs[inputTensorIdx];
  const circle::Tensor *begin = runtime_kernel.inputs[beginTensorIdx];
  const circle::Tensor *end = runtime_kernel.inputs[endTensorIdx];
  const circle::Tensor *strides = runtime_kernel.inputs[stridesTensorIdx];

  const circle::Tensor *output = runtime_kernel.outputs[outputTensorIdx];

  assert(input != nullptr);
  assert(begin != nullptr);
  assert(end != nullptr);
  assert(strides != nullptr);
  assert(output != nullptr);

  status = utils::checkCondition(input->type() == output->type());
  if (status != Ok)
    return status;

  status = utils::checkCondition(begin->type() == circle::TensorType_INT32 and
                                 end->type() == circle::TensorType_INT32 and
                                 strides->type() == circle::TensorType_INT32);
  if (status != Ok)
    return status;

  return status;
}

} // namespace import
} // namespace onert_micro
