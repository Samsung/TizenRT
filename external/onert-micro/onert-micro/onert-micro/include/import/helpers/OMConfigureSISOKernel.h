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

#ifndef ONERT_MICRO_IMPORT_HELPERS_CONFIGURE_SISO_KERNEL_H
#define ONERT_MICRO_IMPORT_HELPERS_CONFIGURE_SISO_KERNEL_H

#include "import/OMKernelConfigureBuilder.h"
#include "core/OMUtils.h"
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"

namespace onert_micro
{
namespace import
{
namespace helpers
{
OMStatus configure_SISO_kernel(const OMConfigureArgs &config_args);

} // namespace helpers
} // namespace import
} // namespace onert_micro

#endif // ONERT_MICRO_IMPORT_HELPERS_CONFIGURE_SISO_KERNEL_H
