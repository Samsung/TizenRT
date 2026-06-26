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

#include "optimize/OMOptimizePassesBuilder.h"
#include "OMStatus.h"
#include "OMConfig.h"
#include "core/OMRuntimeStorage.h"
#include "core/OMRuntimeStorage.h"
#include "core/OMKernelType.h"
#include "core/reader/OMCircleReader.h"

using namespace onert_micro;

namespace
{
OMStatus isInplaceOperation(const circle::Operator *op, core::OMRuntimeContext &runtime_context,
                            bool &is_inplace)
{
  OMStatus status = Ok;

  const auto op_codes = runtime_context.getCircleOpcodes();
  uint32_t index = op->opcode_index();

  assert(index < op_codes->size());

  const auto opcode = op_codes->operator[](index);

  switch (opcode->builtin_code())
  {
    case circle::BuiltinOperator_ABS:
    case circle::BuiltinOperator_CEIL:
    case circle::BuiltinOperator_LOGISTIC:
    case circle::BuiltinOperator_RESHAPE:
    case circle::BuiltinOperator_ELU:
    case circle::BuiltinOperator_EXPAND_DIMS:
    case circle::BuiltinOperator_EXP:
    case circle::BuiltinOperator_TANH:
    case circle::BuiltinOperator_LEAKY_RELU:
    case circle::BuiltinOperator_LOG:
    case circle::BuiltinOperator_RELU:
    case circle::BuiltinOperator_RELU6:
    case circle::BuiltinOperator_ROUND:
    case circle::BuiltinOperator_ADD:
    case circle::BuiltinOperator_MUL:
    case circle::BuiltinOperator_SUB:
    case circle::BuiltinOperator_ZEROS_LIKE:
    {
      is_inplace = true;
      break;
    }
#if 0 // FIXME: Enable after custom operation is introduced
    case circle::BuiltinOperator_CUSTOM:
    {
      core::OMBuilderCustomID custom_id;

      core::OMBuilderID builderID = core::OMBuilderID::Size;
      status = core::getCustomOperatorBuilderId(opcode->custom_code(), builderID);

      if (status != Ok)
        return status;

      status = core::getCustomOperatorByBuilderId(builderID, custom_id);

      if (status != Ok)
        return status;

      switch (custom_id)
      {
        case onert_micro::core::CUSTOM_custom_gru:
          is_inplace = true;
          break;
        default:
          is_inplace = false;
          break;
      }
    }
#endif
    default:
      is_inplace = false;
      break;
  }
  return status;
}

bool isSingleUsageOfTensor(core::OMRuntimeContext &context, const int32_t tensor_index)
{
  uint32_t usage_count = 0;

  const auto operators = context.getCircleOperators();
  for (uint32_t i = 0; i < operators->size(); ++i)
  {
    const auto *op = operators->operator[](i);
    assert(op != nullptr);

    const auto *op_inputs = op->inputs();
    for (int32_t j = 0; j < op_inputs->size(); ++j)
    {
      const auto input_index = op_inputs->operator[](j);
      if (input_index == tensor_index)
      {
        if (++usage_count > 1)
          return false;
      }
    }
  }

  // Let's check that it is not graph output
  if (usage_count == 1)
  {
    const auto &outputs_indexes = context.getCircleOutputs();
    bool is_graph_output = (std::find(outputs_indexes->begin(), outputs_indexes->end(),
                                      tensor_index) != outputs_indexes->end());
    if (is_graph_output)
      return false;
  }

  return true;
}

OMStatus checkInplaceOp(core::OMRuntimeContext &context, const circle::Operator *cur_op,
                        bool &is_inplace)
{
  const auto graph_outputs = context.getCircleOutputs();
  const auto *op_inputs = cur_op->inputs();
  const auto *op_outputs = cur_op->outputs();

  auto non_const_input_it = op_inputs->begin();
  while (non_const_input_it != op_inputs->end())
  {
    auto dist = std::distance(op_inputs->begin(), non_const_input_it);

    const auto non_const_input_idx = *non_const_input_it;

    if (context.isConstTensor(non_const_input_idx))
    {
      non_const_input_it++;
      continue;
    }

    // Check single usage of input tensor
    if (not isSingleUsageOfTensor(context, non_const_input_idx))
    {
      is_inplace = false;
      break;
    }

    // Let's check single usage of output tensor
    if (dist >= op_outputs->size() and op_outputs->size() == 1)
      dist = 0;

    assert(dist < op_outputs->size());
    if (dist >= op_outputs->size())
      return UnknownError;

    const auto output_index = op_outputs->operator[](dist);
    if (not isSingleUsageOfTensor(context, output_index))
    {
      is_inplace = false;
      break;
    }

    // Check that num elements are equal
    {
      const auto *input_non_const_tensor = context.getTensorByIndex(non_const_input_idx);
      const auto *output_tensor = context.getTensorByIndex(output_index);
      if (input_non_const_tensor->shape()->size() != output_tensor->shape()->size())
      {
        is_inplace = false;
        break;
      }
    }

    // Let's check that output is not a graph output tensor
    // TODO: check this statement
    {
      if (std::find(graph_outputs->begin(), graph_outputs->end(), output_index) !=
          graph_outputs->end())
      {
        is_inplace = false;
        break;
      }
    }

    non_const_input_it++;
  }

  return Ok;
}

OMStatus findInplaceOp(core::OMRuntimeStorage &storage, core::OMRuntimeContext &context,
                       const OMConfig &configs, bool &is_changed)
{
  OMStatus status = Ok;

  const core::reader::CircleOperators *operators = context.getCircleOperators();

  for (uint32_t i = 0; i < operators->size(); ++i)
  {
    auto kernel_type = storage.getKernelType(i);
    if (kernel_type == onert_micro::core::Inplace)
      continue;

    auto cur_op = operators->operator[](i);

    bool is_inplace = false;
    isInplaceOperation(cur_op, context, is_inplace);

    if (is_inplace == false)
      continue;

    is_inplace = true;

    status = checkInplaceOp(context, cur_op, is_inplace);

    if (status != Ok)
      return status;

    if (not is_inplace)
      continue;

    is_changed = true;
    storage.setKernelType(i, onert_micro::core::Inplace);
  }

  return status;
}

} // namespace

optimize::OMGraphStatus optimize::onert_micro_FindInplaceOpPass(core::OMRuntimeStorage &storage,
                                                                core::OMRuntimeContext &context,
                                                                const OMConfig &configs)
{
  bool changed = false;

  OMGraphStatus graph_status = {Unchanged, Ok};

  // If it is train mode, skip inplace optimization
  // We need all tensors
  if (configs.train_mode)
    return graph_status;

  do
  {
    changed = false;
    graph_status.main_status = findInplaceOp(storage, context, configs, changed);

    if (graph_status.main_status != Ok)
      return graph_status;

    if (changed)
      graph_status.graph_status = Changed;

  } while (changed);

  return graph_status;
}
