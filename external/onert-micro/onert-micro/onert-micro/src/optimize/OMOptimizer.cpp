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
#include "optimize/OMGraphStatus.h"
#include "OMStatus.h"
#include "optimize/OMOptimizePassesBuilder.h"
#include "optimize/OMOptimizer.h"

using namespace onert_micro;
using namespace onert_micro::optimize;

OMStatus OMOptimizer::optimize(core::OMRuntimeStorage &storage, core::OMRuntimeContext &context,
                               const OMConfig &configs)
{
  OMGraphStatus graph_status = {Unchanged, Ok};

  const uint32_t number_of_passes = optimize_passes.getPassNumber();

  do
  {
    for (uint32_t i = 0; i < number_of_passes; ++i)
    {
      OptimizePassFunc *cur_pass_func;
      OMStatus status = optimize_passes.getOptimizeFunc(i, &cur_pass_func);
      assert(status == Ok && "Unsupported pass");
      if (status != Ok)
        return status;

      graph_status = cur_pass_func(storage, context, configs);
      assert(graph_status.main_status == Ok);
      if (graph_status.main_status != Ok)
        return graph_status.main_status;
    }
  } while (graph_status.graph_status == Changed);

  return Ok;
}
