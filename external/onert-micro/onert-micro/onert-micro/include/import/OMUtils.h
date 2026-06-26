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

#ifndef ONERT_MICRO_IMPORT_UTILS_H
#define ONERT_MICRO_IMPORT_UTILS_H

#include <cmath>
#include "OMStatus.h"
#include "core/reader/OMCircleReader.h"
#include "core/OMRuntimeShape.h"
#include "core/OMKernelData.h"

#include "import/OMKernelConfigureBuilder.h"

// FIXME: this dependancy
#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMRuntimeKernel.h"

namespace onert_micro
{
namespace import
{

OMStatus SISOHeader(const OMConfigureArgs &execute_args, const circle::Tensor **input,
                    const circle::Tensor **output);

OMStatus TISOHeader(const OMConfigureArgs &execute_args, const circle::Tensor **input1,
                    const circle::Tensor **input2, const circle::Tensor **output);

} // namespace import
} // namespace onert_micro

#endif // ONERT_MICRO_IMPORT_UTILS_H
