
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

#include "import/OMUtils.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr uint32_t input1TensorIdx = 0;
constexpr uint32_t input2TensorIdx = 1;
constexpr uint32_t outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleNotEqual(const OMConfigureArgs &config_args)
{
  const circle::Tensor *input1;
  const circle::Tensor *input2;
  const circle::Tensor *output;

  TISOHeader(config_args, &input1, &input2, &output);

  OMStatus status;

  status = utils::checkCondition(input1->type() == input2->type());
  if (status != Ok)
    return status;

  status = utils::checkCondition(output->type() == circle::TensorType_BOOL);
  if (status != Ok)
    return status;

  return status;
}

} // namespace import
} // namespace onert_micro
