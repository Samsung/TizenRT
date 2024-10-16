/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "loader/GraphLoader.h"

namespace luci_interpreter
{
namespace
{

bool isInplaceOperation(const circle::BuiltinOperator &op)
{
  switch (op)
  {
    case circle::BuiltinOperator_ABS:
    case circle::BuiltinOperator_CEIL:
    case circle::BuiltinOperator_LOGISTIC:
    case circle::BuiltinOperator_RESHAPE:
    case circle::BuiltinOperator_ELU:
    case circle::BuiltinOperator_EXPAND_DIMS:
    case circle::BuiltinOperator_EXP:
    case circle::BuiltinOperator_LEAKY_RELU:
    case circle::BuiltinOperator_LOG:
    case circle::BuiltinOperator_RELU:
    case circle::BuiltinOperator_RELU6:
    case circle::BuiltinOperator_ROUND:
    case circle::BuiltinOperator_ADD:
    case circle::BuiltinOperator_MUL:
    case circle::BuiltinOperator_SUB:
    case circle::BuiltinOperator_WHILE:
    case circle::BuiltinOperator_ZEROS_LIKE:
      return true;
    default:
      return false;
  }
}

bool isSingleUsageOfTensor(CircleReader *reader, const int32_t tensor_index)
{
  uint32_t usage_count = 0;

  const auto operators = reader->operators();
  for (uint32_t i = 0; i < operators.size(); ++i)
  {
    const auto *op = operators.at(i);
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
    const auto &outputs_indexes = reader->outputs();
    bool is_graph_output = (std::find(outputs_indexes.begin(), outputs_indexes.end(),
                                      tensor_index) != outputs_indexes.end());
    if (is_graph_output)
      return false;
  }

  return true;
}

} // namespace

void GraphLoader::checkInplaceOps(CircleReader *reader, RuntimeGraph *runtime_graph)
{
  const auto operators = reader->operators();
  const auto graph_outputs = reader->outputs();
  for (uint32_t i = 0; i < operators.size(); ++i)
  {
    const auto *op = operators.at(i);
    assert(op != nullptr);

    // Check inplace optimization for operation with single input and single output
    if (isInplaceOperation(reader->builtin_code(op)))
    {
      const auto *op_inputs = op->inputs();
      const auto *op_outputs = op->outputs();

      bool is_inplace = true;
      auto non_const_input_it = op_inputs->begin();
      while (true)
      {
        non_const_input_it =
          std::find_if(non_const_input_it, op_inputs->end(), [&reader](const auto input_idx) {
            if (input_idx == -1)
              return false;
            auto tensor = reader->tensors()[input_idx];
            if ( tensor == nullptr )
              return false;
            return not Tensor::is_constant_tensor(reader, tensor);
          });

        if (non_const_input_it == op_inputs->end())
          break;

        auto dist = std::distance(op_inputs->begin(), non_const_input_it);

        const auto non_const_input_idx = *non_const_input_it;

        // Check single usage of input tensor
        if (not isSingleUsageOfTensor(reader, non_const_input_idx))
        {
          is_inplace = false;
          break;
        }

        // Let's check single usage of output tensor
        if (dist >= op_outputs->size() and op_outputs->size() == 1)
          dist = 0;
        assert(dist < op_outputs->size());
        const auto output_index = op_outputs->operator[](dist);
        if (not isSingleUsageOfTensor(reader, output_index))
        {
          is_inplace = false;
          break;
        }

        // Check that num elements are equal
        {
          const auto *input_non_const_tensor = reader->tensors().at(non_const_input_idx);
          const auto *output_tensor = reader->tensors().at(output_index);
          if (Tensor::num_elements(input_non_const_tensor) != Tensor::num_elements(output_tensor))
          {
            is_inplace = false;
            break;
          }
        }

        // Let's check that output is not a graph output tensor
        // TODO: check this statement
        {
          if (std::find(graph_outputs.begin(), graph_outputs.end(), output_index) !=
              graph_outputs.end())
          {
            is_inplace = false;
            break;
          }
        }

        non_const_input_it++;
      }

      if (is_inplace)
        runtime_graph->addInplaceOpIndex(op);
    }
  }
}

} // namespace luci_interpreter
