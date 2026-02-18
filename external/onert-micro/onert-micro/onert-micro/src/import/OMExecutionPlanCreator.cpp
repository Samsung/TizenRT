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

#include "import/OMExecutionPlanCreator.h"

#include <map>

using namespace onert_micro::core;
using namespace onert_micro::import;
using namespace onert_micro;

namespace
{

// Layers with trainable weights
// Note: needed not to store some layers with const intputs but it is not trainable (for example
// Reshape)
bool isTrainableWeights(const circle::OperatorCode *opcode)
{
  switch (opcode->builtin_code())
  {
    case circle::BuiltinOperator_FULLY_CONNECTED:
    case circle::BuiltinOperator_CONV_2D:
    case circle::BuiltinOperator_GRU:
      return true;
    default:
      return false;
  }
}

bool isOpNeedSaveOutputData(const circle::OperatorCode *opcode, const circle::Operator *cur_op)
{
  switch (opcode->builtin_code())
  {
    case circle::BuiltinOperator_FULLY_CONNECTED:
    {
      if (cur_op->builtin_options_as_FullyConnectedOptions() != nullptr)
        return true;
    }
    break;
    case circle::BuiltinOperator_CONV_2D:
    {
      if (cur_op->builtin_options_as_Conv2DOptions() != nullptr)
        return true;
    }
    break;
    default:
      return false;
  }
  return false;
}

bool isOpNeedSaveInputData(const circle::OperatorCode *opcode)
{
  switch (opcode->builtin_code())
  {
    case circle::BuiltinOperator_MAX_POOL_2D:
      return true;
    default:
      return false;
  }
}

} // namespace

/*
 * Create execution plan for graph for non trainable mode
 * TODO: describe creation execution plan logic
 */
OMStatus OMExecutionPlanCreator::createExecutionPlan(core::OMRuntimeStorage &runtime_storage,
                                                     core::OMRuntimeContext &runtime_context,
                                                     core::memory::OMRuntimeAllocator &allocator,
                                                     const OMConfig &configs)
{
  // Check is non trainable mode
  assert(configs.train_mode != true);
  if (configs.train_mode == true)
    return UnknownError;

  bool keep_input = configs.keep_input;

  std::vector<std::vector<uint16_t>> &alloc_plan = allocator.getAllocPlan();
  std::vector<std::vector<uint16_t>> &dealloc_plan = allocator.getDeallocPlan();

  // First remove prev plan (if it was created)
  alloc_plan.clear();
  dealloc_plan.clear();

  using Lifetime = std::pair<int32_t, int32_t>;

  std::map<uint16_t, Lifetime> lifetimes;

  const reader::CircleOperators *operators = runtime_context.getCircleOperators();

  const size_t num_kernels = operators->size();

  if (not keep_input)
  {
    auto graph_inputs = runtime_context.getCircleInputs();
    for (const auto input_ind : *graph_inputs)
    {
      assert(lifetimes.count(input_ind) == 0);
      lifetimes[input_ind] = Lifetime(-1, 0);
    }
  }

  for (int32_t index = 0; index < num_kernels; ++index)
  {
    auto *cur_op = operators->operator[](index);

    const auto *op_inputs = cur_op->inputs();
    const auto *op_outputs = cur_op->outputs();
    auto kernel_type = runtime_storage.getKernelType(index);
    for (int32_t j = 0; j < op_inputs->size(); ++j)
    {
      const auto input_index = op_inputs->operator[](j);

      if (input_index == -1)
        continue;

      // Pass constant tensors
      if (runtime_context.isConstTensor(input_index))
        continue;

      if (lifetimes.count(input_index) > 0)
      {
        if (kernel_type == Inplace)
          lifetimes.at(input_index).second = -1;
        else
          lifetimes.at(input_index).second = index;
      }
    }

    for (int32_t j = 0; j < op_outputs->size(); ++j)
    {
      const auto output_index = op_outputs->operator[](j);

      if (kernel_type == Inplace)
        lifetimes[output_index] = Lifetime(-1, index);
      else
        lifetimes[output_index] = Lifetime(index, index);
    }
  }
  auto graph_outputs = runtime_context.getCircleOutputs();
  for (const auto output_ind : *graph_outputs)
  {
    if (lifetimes.count(output_ind) > 0)
      lifetimes.at(output_ind).second = static_cast<int32_t>(num_kernels);
  }

  alloc_plan.assign(num_kernels, std::vector<uint16_t>());
  dealloc_plan.assign(num_kernels + 1, std::vector<uint16_t>());

  for (const auto &item : lifetimes)
  {
    if (item.second.first != -1)
      alloc_plan[item.second.first].push_back(item.first);
    if (item.second.second != -1)
      dealloc_plan[item.second.second].push_back(item.first);
  }

  return Ok;
}

/*
 * Create execution plan for graph for non trainable mode
 * TODO: describe creation execution plan logic
 */
OMStatus OMExecutionPlanCreator::createForwardExecutionPlan(
  core::OMRuntimeStorage &runtime_storage, core::OMRuntimeContext &runtime_context,
  core::memory::OMRuntimeAllocator &allocator, const OMConfig &configs)
{
  // Check is trainable mode
  assert(configs.train_mode == true);
  if (configs.train_mode != true)
    return UnknownError;

  bool keep_input = configs.keep_input;
  std::vector<std::vector<uint16_t>> &alloc_plan = allocator.getAllocPlan();
  std::vector<std::vector<uint16_t>> &dealloc_plan = allocator.getDeallocPlan();

  // First remove prev plan (if it was created)
  alloc_plan.clear();
  dealloc_plan.clear();

  using Lifetime = std::pair<int32_t, int32_t>;

  std::map<uint16_t, Lifetime> lifetimes;

  const reader::CircleOperators *operators = runtime_context.getCircleOperators();

  const size_t num_kernels = operators->size();

  uint32_t num_train_layers = configs.training_context.num_of_train_layers;
  if (num_train_layers == 0)
    num_train_layers = num_kernels;

  std::unordered_map<uint16_t, uint8_t> trainable_ops_config =
    runtime_context.getTrainableOpsIndexes();

  // If context has config file defined trainable operations
  // than ignore configs.training_context.num_of_train_layers value
  // and use max value from trainable_ops_indexes to define last train op
  uint16_t last_train_op_indx = num_kernels - num_train_layers;
  if (!trainable_ops_config.empty())
  {
    last_train_op_indx = std::numeric_limits<uint16_t>::max();
    // Find op trainable index with min value
    for (auto &p : trainable_ops_config)
    {
      last_train_op_indx = std::min(p.first, last_train_op_indx);
    }
    num_train_layers = (num_kernels - last_train_op_indx);
  }

  if (not keep_input)
  {
    auto graph_inputs = runtime_context.getCircleInputs();
    for (const auto input_ind : *graph_inputs)
    {
      assert(lifetimes.count(input_ind) == 0);
      lifetimes[input_ind] = Lifetime(-1, 0);
    }
  }

  const auto *op_codes = runtime_context.getCircleOpcodes();

  for (int32_t index = 0; index < num_kernels; ++index)
  {
    auto *cur_op = operators->operator[](index);

    const auto *op_inputs = cur_op->inputs();
    const auto *op_outputs = cur_op->outputs();
    auto kernel_type = runtime_storage.getKernelType(index);

    uint32_t cur_opcode_index = cur_op->opcode_index();

    assert(cur_opcode_index < op_codes->size());

    const auto opcode = op_codes->operator[](cur_opcode_index);

    // Flag to determine is current operation needed to save input data (is this op in training part
    // of the graph)
    bool need_to_save_input_data =
      (index >= last_train_op_indx) and
      ((trainable_ops_config.empty() or
        trainable_ops_config.find(index) != trainable_ops_config.end() and
          trainable_ops_config[index] != ONLY_BIAS) or
       isOpNeedSaveInputData(opcode));

    // Flag to determine is current operation needed to save output data (is this op in training
    // part of the graph)
    bool need_to_save_output_data =
      (index >= last_train_op_indx) and
      ((trainable_ops_config.find(index) != trainable_ops_config.end() and
        trainable_ops_config[index] != ONLY_BIAS) or
       isOpNeedSaveOutputData(opcode, cur_op));

    for (int32_t j = 0; j < op_inputs->size(); ++j)
    {
      const auto input_index = op_inputs->operator[](j);

      if (input_index == -1)
        continue;

      // Pass constant tensors
      if (runtime_context.isConstTensor(input_index))
        continue;

      if (lifetimes.count(input_index) > 0)
      {
        // lifetimes.at(input_index).second == -2 - Means need to save data for input_index tensor
        if (kernel_type == Inplace or need_to_save_input_data or
            (lifetimes.at(input_index).second == -2))
          lifetimes.at(input_index).second = -1;
        else
          lifetimes.at(input_index).second = index;
      }
    }

    for (int32_t j = 0; j < op_outputs->size(); ++j)
    {
      const auto output_index = op_outputs->operator[](j);

      if (kernel_type == Inplace)
        lifetimes[output_index] = Lifetime(-1, index);
      else if (need_to_save_output_data)
        lifetimes[output_index] = Lifetime(index, -2);
      else
        lifetimes[output_index] = Lifetime(index, index);
    }
  }
  auto graph_outputs = runtime_context.getCircleOutputs();
  for (const auto output_ind : *graph_outputs)
  {
    if (lifetimes.count(output_ind) > 0)
      lifetimes.at(output_ind).second = static_cast<int32_t>(num_kernels);
  }

  alloc_plan.assign(num_kernels, std::vector<uint16_t>());
  dealloc_plan.assign(num_kernels + 1, std::vector<uint16_t>());

  for (const auto &item : lifetimes)
  {
    if (item.second.first >= 0)
      alloc_plan[item.second.first].push_back(item.first);
    if (item.second.second >= 0)
      dealloc_plan[item.second.second].push_back(item.first);
  }

  return Ok;
}

/*
 * Create execution plan for backward graph:
 * - Allocate memory for inputs for current op using the following rules:
 *   1) Don't allocate data for non const input tensor if it is last op for training (don't need to
 * backpropagate result) 2) Don't allocate data for const input tensor if is non trainable const 3)
 * Allocate data otherwise
 * - Deallocate memory for outputs for current op using the following rules:
 *   1) Deallocate all outputs tensors.
 */
OMStatus OMExecutionPlanCreator::createBackwardExecutionPlan(
  core::OMRuntimeStorage &runtime_storage, core::OMRuntimeContext &runtime_context,
  core::memory::OMRuntimeAllocator &allocator, const OMConfig &configs)
{
  bool keep_input = configs.keep_input;
  bool train_mode = configs.train_mode;
  assert(train_mode);
  if (train_mode == false)
    return UnknownError;

  std::vector<std::vector<uint16_t>> &alloc_plan = allocator.getAllocPlan();
  std::vector<std::vector<uint16_t>> &dealloc_plan = allocator.getDeallocPlan();

  // First remove prev plan (if it was created)
  alloc_plan.clear();
  dealloc_plan.clear();

  using Lifetime = std::pair<int32_t, int32_t>;
  std::map<uint16_t, Lifetime> lifetimes;

  const reader::CircleOperators *operators = runtime_context.getCircleOperators();
  const uint32_t num_kernels = operators->size();

  uint32_t num_train_layers =
    configs.training_context.num_of_train_layers == 0
      ? num_kernels
      : std::min(num_kernels, configs.training_context.num_of_train_layers);
  auto graph_outputs = runtime_context.getCircleOutputs();

  for (const auto output_ind : *graph_outputs)
  {
    assert(lifetimes.count(output_ind) == 0);
    lifetimes[output_ind] = Lifetime(-1, 0);
  }

  std::unordered_map<uint16_t, uint8_t> trainable_ops_config =
    runtime_context.getTrainableOpsIndexes();

  // If context has config file defined trainable operations
  // than ignore configs.training_context.num_of_train_layers value
  // and use max value from trainable_ops_indexes to define last train op
  uint16_t last_train_op_indx = num_kernels - num_train_layers;
  if (!trainable_ops_config.empty())
  {
    last_train_op_indx = std::numeric_limits<uint16_t>::max();
    // Find op trainable index with min value
    for (auto &p : trainable_ops_config)
    {
      last_train_op_indx = std::min(p.first, last_train_op_indx);
    }
    num_train_layers = (num_kernels - last_train_op_indx);
  }

  const auto *op_codes = runtime_context.getCircleOpcodes();
  for (int32_t index = 0; index < num_train_layers; ++index)
  {
    uint32_t cur_op_index = num_kernels - index - 1;
    auto *cur_op = operators->operator[](cur_op_index);

    uint32_t cur_opcode_index = cur_op->opcode_index();

    assert(cur_opcode_index < op_codes->size());

    const auto opcode = op_codes->operator[](cur_opcode_index);

    const auto *op_inputs = cur_op->inputs();
    const auto *op_outputs = cur_op->outputs();

    bool is_trainable_ops =
      trainable_ops_config.empty() == true
        ? isTrainableWeights(opcode)
        : trainable_ops_config.find(cur_op_index) != trainable_ops_config.end();

    // Warning: this is right for Conv2D and for FullyConnected kernels
    const int32_t bias_index = 2;

    for (int32_t j = 0; j < op_inputs->size(); ++j)
    {
      const auto input_index = op_inputs->operator[](j);
      const auto is_const = runtime_context.isConstTensor(input_index);
      // Note: we dont need to allocate for last node and for empty tensor
      if (input_index == -1 or (is_const and not is_trainable_ops))
      {
        continue;
      }

      if ((index == num_train_layers - 1) and !is_const)
      {
        lifetimes[input_index] = {-1, index};
      }
      else if (is_const and
               trainable_ops_config.find(cur_op_index) != trainable_ops_config.end() and
               trainable_ops_config[cur_op_index] == ONLY_BIAS and j != bias_index)
      {
        // Do nothing, due to update only bias
        continue;
      }
      else
      {
        lifetimes[input_index] = {index, -1};
      }
    }

    for (int32_t j = 0; j < op_outputs->size(); ++j)
    {
      const auto output_index = op_outputs->operator[](j);
      if (lifetimes.count(output_index) > 0)
        lifetimes.at(output_index).second = index;
    }
  }

  alloc_plan.assign(num_train_layers, std::vector<uint16_t>());
  dealloc_plan.assign(num_train_layers, std::vector<uint16_t>());

  for (const auto &item : lifetimes)
  {
    if (item.second.first != -1)
      alloc_plan[item.second.first].push_back(item.first);
    if (item.second.second != -1)
      dealloc_plan[item.second.second].push_back(item.first);
  }

  return Ok;
}
