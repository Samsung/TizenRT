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

#ifndef ONERT_MICRO_IMPORT_CONFIGURE_ARGS_H
#define ONERT_MICRO_IMPORT_CONFIGURE_ARGS_H

#include "OMStatus.h"
#include "OMConfig.h"
#include "core/OMRuntimeStorage.h"
#include "core/OMRuntimeContext.h"
#include "core/OMRuntimeModule.h"

namespace onert_micro
{
namespace import
{

struct OMConfigureArgs
{
  core::OMRuntimeStorage &runtime_storage;
  core::OMRuntimeContext &runtime_context;
  uint16_t kernel_index;
  const OMConfig &configs;
  core::OMRuntimeModule &runtime_module;
};

} // namespace import
} // namespace onert_micro

#endif // ONERT_MICRO_IMPORT_CONFIGURE_ARGS_H
