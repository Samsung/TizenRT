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

using namespace onert_micro;
using namespace onert_micro::core;

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleWhile(const onert_micro::import::OMConfigureArgs &config_args)
{
  OMRuntimeModule &runtime_module = config_args.runtime_module;
  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  onert_micro::execute::OMRuntimeKernel runtime_kernel;

  OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
  if (status != Ok)
    return status;

  auto options = runtime_kernel.first_operator->builtin_options_as_WhileOptions();
  status = utils::checkCondition(options != nullptr);
  if (status != Ok)
    return status;

  // Num of inputs equals to num of outputs
  status = utils::checkCondition(runtime_kernel.inputs_num == runtime_kernel.outputs_num);
  if (status != Ok)
    return status;

  // Obtain conditional and body runtime subgraphs
  const auto body_subgraph_index = options->body_subgraph_index();
  const auto cond_subgraph_index = options->cond_subgraph_index();
  OMRuntimeGraph *cond_runtime_graph = nullptr;
  OMRuntimeGraph *body_runtime_graph = nullptr;
  status = runtime_module.getRuntimeGraphAt(cond_subgraph_index, &cond_runtime_graph);
  if (status != Ok)
    return status;
  status = runtime_module.getRuntimeGraphAt(body_subgraph_index, &body_runtime_graph);
  if (status != Ok)
    return status;

  OMRuntimeContext &cond_runtime_context = cond_runtime_graph->getRuntimeContext();
  OMRuntimeContext &body_runtime_context = body_runtime_graph->getRuntimeContext();

  // Check cond runtime graph
  status =
    utils::checkCondition(cond_runtime_graph->getNumberOfInputs() == runtime_kernel.inputs_num and
                          cond_runtime_graph->getNumberOfOutputs() == 1);
  if (status != Ok)
    return status;

  const auto cond_output_index = cond_runtime_context.getGraphOutputTensorIndex(0);
  const auto cond_output_tensor =
    cond_runtime_context.getTensorByIndex(static_cast<int32_t>(cond_output_index));
  status = utils::checkCondition(cond_output_tensor->type() == circle::TensorType_BOOL);

  // Check body runtime graph
  status =
    utils::checkCondition(body_runtime_graph->getNumberOfInputs() == runtime_kernel.inputs_num and
                          body_runtime_graph->getNumberOfOutputs() == runtime_kernel.outputs_num);
  return status;
}

} // namespace import
} // namespace onert_micro
