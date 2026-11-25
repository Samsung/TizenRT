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

#ifndef ONERT_MICRO_OPTIMIZE_PASSES_BUILDER_H
#define ONERT_MICRO_OPTIMIZE_PASSES_BUILDER_H

#include "OMConfig.h"
#include "core/reader/OMCircleReader.h"
#include "core/OMRuntimeStorage.h"
#include "core/OMRuntimeContext.h"
#include "optimize/OMGraphStatus.h"

namespace onert_micro
{
namespace optimize
{

#define REGISTER_PASS(name) PASS_##name,
enum class OMPassBuilderID
{
#include "BuildPass.lst"
  Size
};
#undef REGISTER_PASS

using OptimizePassFunc = OMGraphStatus(core::OMRuntimeStorage &storage,
                                       core::OMRuntimeContext &context, const OMConfig &configs);

#define REGISTER_PASS(name)                                         \
  OMGraphStatus onert_micro_##name(core::OMRuntimeStorage &storage, \
                                   core::OMRuntimeContext &context, const OMConfig &configs);
#include "BuildPass.lst"
#undef REGISTER_PASS

class OptimizePassRegistry
{
public:
  constexpr OptimizePassRegistry() : _optimize_passes()
  {
#define REGISTER_PASS(name) \
  registerOptimizePasses(optimize::OMPassBuilderID::PASS_##name, onert_micro_##name);

#include "BuildPass.lst"

#undef REGISTER_PASS
  }

  OMStatus getOptimizeFunc(uint32_t idx, OptimizePassFunc **optimize_func) const
  {
    assert(idx < size_t(optimize::OMPassBuilderID::Size));
    if (idx >= size_t(optimize::OMPassBuilderID::Size))
    {
      *optimize_func = nullptr;
      return UnknownError;
    }
    *optimize_func = _optimize_passes[idx];
    return Ok;
  }

  constexpr uint32_t getPassNumber() const { return size_t(optimize::OMPassBuilderID::Size); }

private:
  constexpr void registerOptimizePasses(optimize::OMPassBuilderID id, OptimizePassFunc *func)
  {
    assert(size_t(id) < size_t(optimize::OMPassBuilderID::Size));
    _optimize_passes[size_t(id)] = func;
  }

private:
  OptimizePassFunc *_optimize_passes[size_t(optimize::OMPassBuilderID::Size)];
};

// Global constexpr kernel builtin and custom configure
constexpr OptimizePassRegistry optimize_passes;

} // namespace optimize
} // namespace onert_micro

#endif // ONERT_MICRO_OPTIMIZE_PASSES_BUILDER_H
