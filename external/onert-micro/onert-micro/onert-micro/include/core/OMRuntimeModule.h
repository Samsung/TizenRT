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

#ifndef ONERT_MICRO_CORE_RUNTIME_MODULE_H
#define ONERT_MICRO_CORE_RUNTIME_MODULE_H

#include "OMStatus.h"
#include "OMConfig.h"
#include "OMRuntimeGraph.h"
#include "core/reader/OMCircleReader.h"

#include <vector>

namespace onert_micro
{
namespace core
{

class OMRuntimeModule
{
protected:
  std::vector<OMRuntimeGraph> _graphs;

public:
  OMRuntimeModule() = default;
  OMRuntimeModule(const OMRuntimeModule &) = delete;
  OMRuntimeModule(OMRuntimeModule &&) = delete;
  OMRuntimeModule &operator=(const OMRuntimeModule &) = delete;
  OMRuntimeModule &&operator=(const OMRuntimeModule &&) = delete;
  ~OMRuntimeModule() = default;

  OMStatus importModel(const char *model_ptr, const OMConfig &config);
  OMStatus run(const OMConfig &config);
  OMStatus reset();

  uint32_t getNumberOfInputs();
  uint32_t getNumberOfOutputs();

  uint32_t getInputSizeAt(uint32_t position);
  uint32_t getOutputSizeAt(uint32_t position);

  void *getInputDataAt(uint32_t position);
  void *getOutputDataAt(uint32_t position);

  OMStatus getRuntimeGraphAt(uint32_t pos, OMRuntimeGraph **runtime_graph);

  OMStatus allocateInputs();
};

} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_RUNTIME_MODULE_H
