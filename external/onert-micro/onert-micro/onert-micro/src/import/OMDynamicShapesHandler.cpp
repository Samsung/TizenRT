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

#include "import/OMDynamicShapesHandler.h"
#include "core/OMKernelType.h"

using namespace onert_micro::core;
using namespace onert_micro::import;
using namespace onert_micro;

namespace
{

// Function to obtain index for current operation which has weight tensor, which will be sparse
// updated If operation don't support such behaviour - return -1
int32_t getWeightTensorIndexForOperatorWithOpcode(const circle::OperatorCode *opcode)
{
  switch (opcode->builtin_code())
  {
    // For Conv2D and for FullyConnected return index for weight tensor - is 1
    case circle::BuiltinOperator_CONV_2D:
    case circle::BuiltinOperator_FULLY_CONNECTED:
      return 1;
    default:
      break;
  }
  return -1;
}

OMRuntimeShape createDynamicRuntimeShapeForOperator(OMRuntimeShape shape,
                                                    const circle::OperatorCode *opcode,
                                                    const float partition_size)
{
  assert(partition_size > 1.0);
  switch (opcode->builtin_code())
  {
    // For Conv2D and for FullyConnected return index for weight tensor - is 1
    case circle::BuiltinOperator_CONV_2D:
    case circle::BuiltinOperator_FULLY_CONNECTED:
    {
      auto first_dim_val = shape.dims(0);
      assert(partition_size <= static_cast<float>(first_dim_val));
      assert(partition_size > 0);
      if (partition_size == 0)
        return shape;
      first_dim_val = static_cast<int32_t>(static_cast<float>(first_dim_val) / partition_size);
      assert(first_dim_val > 0);
      shape.setDim(0, first_dim_val);
    }
    break;
    default:
      break;
  }

  return shape;
}

} // namespace

/*
 * Import dynamic shapes from train config file data:
 * Some tensors can have sparse tensor backpropagation scheme (train rank)
 */
OMStatus OMDynamicShapesHandler::importDynamicShapesFromTrainConfig(
  core::OMRuntimeStorage &storage, core::OMRuntimeContext &context,
  core::train::OMTrainingStorage &train_storage)
{
  std::unordered_map<uint16_t, uint8_t> train_op_indexes_to_train_rank =
    context.getTrainableOpsIndexes();
  const auto opcodes = context.getCircleOpcodes();

  // Goes over pairs of op index and train rank value
  for (auto &p : train_op_indexes_to_train_rank)
  {
    const uint16_t op_index = p.first;
    const auto train_rank = static_cast<OpTrainableRankType>(p.second);

    switch (train_rank)
    {
      case core::LOWER_1_2_PART:
      case core::UP_1_2_PART:
      {
        const auto cur_op = context.getCircleOperatorAt(op_index);
        const auto opcode = opcodes->operator[](cur_op->opcode_index());

        int32_t res_index = getWeightTensorIndexForOperatorWithOpcode(opcode);
        // The operation doesn't support such behaviour
        if (res_index == -1)
          continue;

        auto tensor_local_index = static_cast<uint16_t>(res_index);
        auto tensor_index = cur_op->inputs()->operator[](tensor_local_index);
        auto tensor = context.getTensorByIndex(tensor_index);
        OMRuntimeShape old_shape(tensor);
        const float partition_size = 2.f;
        OMRuntimeShape new_shape =
          createDynamicRuntimeShapeForOperator(old_shape, opcode, partition_size);
        storage.setDynamicRuntimeShape(tensor_index, new_shape);

        train_storage.addTrainRank(tensor_index, train_rank);

        break;
      }
      default:
        continue;
    }
  }

  return Ok;
}
