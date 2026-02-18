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

#include "import/OMUtils.h"
#include <cmath>

using namespace onert_micro::execute;
using namespace onert_micro;

OMStatus onert_micro::import::SISOHeader(const OMConfigureArgs &execute_args,
                                         const circle::Tensor **input,
                                         const circle::Tensor **output)
{
  OMStatus status;

  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  OMRuntimeKernel runtime_kernel;
  status = runtime_kernel.readKernel(op_index, runtime_context);

  *input = runtime_kernel.inputs[0];
  *output = runtime_kernel.outputs[0];

  assert(*input != nullptr);
  assert(*output != nullptr);

  return status;
}

OMStatus onert_micro::import::TISOHeader(const OMConfigureArgs &execute_args,
                                         const circle::Tensor **input1,
                                         const circle::Tensor **input2,
                                         const circle::Tensor **output)
{
  OMStatus status;

  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  OMRuntimeKernel runtime_kernel;
  status = runtime_kernel.readKernel(op_index, runtime_context);

  *input2 = runtime_kernel.inputs[0];
  *input1 = runtime_kernel.inputs[1];
  *output = runtime_kernel.outputs[0];

  assert(*input1 != nullptr);
  assert(*input2 != nullptr);
  assert(*output != nullptr);

  return status;
}
