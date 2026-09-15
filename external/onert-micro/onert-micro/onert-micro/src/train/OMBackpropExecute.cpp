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

#include "train/OMBackpropExecute.h"
#include "train/OMBackpropExecutionBuilder.h"

using namespace onert_micro::train;
using namespace onert_micro;

/*
 * Run backward graph to calculate gradients
 */
OMStatus OMBackpropExecute::runBackward(const OMConfig &config, OMBackpropExecuteArgs &args,
                                        core::memory::OMRuntimeAllocator &allocator)
{
  OMStatus status = Ok;

  core::OMRuntimeContext &context = args.backward_context;
  core::OMRuntimeStorage &forward_storage = args.forward_storage;
  core::OMRuntimeStorage &backward_storage = args.backward_storage;

  const core::reader::CircleOperators *operators = context.getCircleOperators();

  const auto num_operators = operators->size();
  const auto *op_codes = context.getCircleOpcodes();

  uint32_t num_train_layers =
    config.training_context.num_of_train_layers == 0
      ? num_operators
      : std::min(num_operators, config.training_context.num_of_train_layers);
  std::unordered_map<uint16_t, uint8_t> trainable_ops_config = context.getTrainableOpsIndexes();

  // If context has config file defined trainable operations
  // than ignore configs.training_context.num_of_train_layers value
  // and use max value from trainable_ops_indexes to define last train op
  uint16_t last_train_op_indx = num_operators - num_train_layers;
  if (!trainable_ops_config.empty())
  {
    last_train_op_indx = std::numeric_limits<uint16_t>::max();
    // Find op trainable index with min value
    for (auto &p : trainable_ops_config)
    {
      last_train_op_indx = std::min(p.first, last_train_op_indx);
    }
    num_train_layers = (num_operators - last_train_op_indx);
  }

  for (int32_t i = 0; i < num_train_layers; ++i)
  {
    uint32_t cur_op_index = num_operators - i - 1;
    auto *cur_op = operators->operator[](cur_op_index);

    status = allocator.allocate(i, &context, &backward_storage);

    if (status != Ok)
      return status;

    core::OMBuilderID builder_id = core::OMBuilderID::Size;
    const circle::Operator *op = operators->operator[](cur_op_index);
    uint32_t index = op->opcode_index();

    assert(index < op_codes->size());

    const auto opcode = op_codes->operator[](index);

    status = core::getBuilderId(opcode, builder_id);

    assert(status == Ok);
    if (status != Ok)
      return status;

    args.kernel_index = cur_op_index;

    if (i == num_train_layers - 1)
    {
      args.is_last_layer = true;
    }
    else
    {
      args.is_last_layer = false;
    }

    if (trainable_ops_config.empty())
    {
      args.is_trainable_layer = true;
      args.train_rank_type = core::OpTrainableRankType::ALL;
    }
    else if (trainable_ops_config.find(cur_op_index) != trainable_ops_config.end())
    {
      args.is_trainable_layer = true;
      args.train_rank_type = core::OpTrainableRankType(trainable_ops_config[cur_op_index]);
    }
    else
    {
      args.is_trainable_layer = false;
    }

    // Calculate gradients
    KernelTrainFunc *train_func = nullptr;
    if (size_t(builder_id) < size_t(core::OMBuilderID::BuiltinOperatorsSize))
    {
      // Builtin operator
      status = kernel_builtin_train.getKernelTrainFunc(builder_id, &train_func);
    }
    else
    {
      assert(false && "Unsupported kernel type for training");
      return UnsupportedOp;
    }

    assert(train_func != nullptr);

    if (status != Ok)
      return status;

    status = train_func(args);

    assert(status == Ok);

    if (status != Ok)
      return status;

      // Deallocate tensors data in backward storage
#ifdef OM_MEMORY_ESTIMATE
    status = allocator.deallocate(i, &backward_storage, &context);
    if (status != Ok)
      return status;

    // Deallocate tensors data in forward storage
    status = allocator.deallocate(i, &forward_storage, &context);
#else
    status = allocator.deallocate(i, &backward_storage);
    if (status != Ok)
      return status;

    // Deallocate tensors data in forward storage
    status = allocator.deallocate(i, &forward_storage);
#endif
  }

  return status;
}
