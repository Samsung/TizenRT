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

#ifndef ONERT_MICRO_EXECUTE_KERNELS_RESHAPE_COMMON_H
#define ONERT_MICRO_EXECUTE_KERNELS_RESHAPE_COMMON_H

#include "OMStatus.h"
#include "core/OMUtils.h"

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMRuntimeKernel.h"

namespace onert_micro
{
namespace execute
{

OMStatus execute_relu_common(const OMExecuteArgs &execute_args, bool is_relu_6);

} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_KERNELS_RESHAPE_COMMON_H
