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

#include "core/OMDataType.h"
#include "core/memory/OMMemoryManager.h"
#include "core/train/OMCheckpointLoader.h"

using namespace onert_micro::core::train;
using namespace onert_micro::core;
using namespace onert_micro::train;
using namespace onert_micro;

namespace
{

constexpr uint16_t MAGIC_NUMBER = 429;
constexpr uint8_t SCHEMA_VERSION = 1;

enum WOFFieldsOffsets
{
  MAGIC_NUMBER_OFFSET = 0,
  SCHEMA_VERSION_OFFSET = 2,
  M_OFFSET = 4,
  V_OFFSET = 8,
  OTHER_PARAMS_OFFSET = 12,
  NUM_BUFFERS_OFFSET = 16,
  WEIGHT_OFFSET = 20,
};

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

} // namespace

OMStatus OMCheckpointLoader::validateCheckpointData(core::OMRuntimeContext &context,
                                                    const char *data, OMConfig &config)
{
  OMStatus status = Ok;

  // Validate magic number
  uint16_t mag_num = 0;
  std::memcpy(&mag_num, &data[MAGIC_NUMBER_OFFSET], sizeof(mag_num));
  assert(mag_num == MAGIC_NUMBER && "False MAGIC NUMBER, check correctness of checkpoint file");
  if (mag_num != MAGIC_NUMBER)
    return FailReadCheckpointFile;

  // Validate schema version
  uint8_t version = 0;
  std::memcpy(&version, &data[SCHEMA_VERSION_OFFSET], sizeof(version));
  assert(version == SCHEMA_VERSION &&
         "False SCHEMA_VERSION NUMBER, check correctness of checkpoint file");
  if (version != SCHEMA_VERSION)
    return FailReadCheckpointFile;

  // Validate count of tensors
  uint32_t num_tensors = context.getCircleTensors()->size();
  uint32_t num_tensors_in_file = 0;
  std::memcpy(&num_tensors_in_file, &data[NUM_BUFFERS_OFFSET], sizeof(num_tensors_in_file));
  assert(num_tensors == num_tensors_in_file &&
         "Number of tensors in circle and in checkpoint file should be the same");
  if (num_tensors != num_tensors_in_file)
    return FailReadCheckpointFile;

  // Validate m, v and other parameters offset
  uint32_t m_offset;
  uint32_t v_offset;
  uint32_t other_params_offset;
  std::memcpy(&m_offset, &data[M_OFFSET], sizeof(m_offset));
  std::memcpy(&v_offset, &data[V_OFFSET], sizeof(v_offset));
  std::memcpy(&other_params_offset, &data[OTHER_PARAMS_OFFSET], sizeof(other_params_offset));

  assert(other_params_offset > 0);
  if (other_params_offset == 0)
    return FailReadCheckpointFile;

  if (config.training_context.optimizer == SGD)
  {
    assert(m_offset == 0 and v_offset == 0);
    if (m_offset != 0 or v_offset != 0)
      return FailReadCheckpointFile;
  }

  return Ok;
}

OMStatus OMCheckpointLoader::loadBuffers(core::OMRuntimeContext &context,
                                         OMTrainingStorage &train_storage, const char *data,
                                         OMConfig &config)
{
  OMStatus status = Ok;

  // Read v, m and other params offsets
  uint32_t m_offset;
  uint32_t v_offset;
  uint32_t other_params_offset;
  std::memcpy(&m_offset, &data[M_OFFSET], sizeof(m_offset));
  std::memcpy(&v_offset, &data[V_OFFSET], sizeof(v_offset));
  std::memcpy(&other_params_offset, &data[OTHER_PARAMS_OFFSET], sizeof(other_params_offset));

  uint32_t weight_offset_pos = WEIGHT_OFFSET;

  // If optimizer is Adam then reset its state
  optimizers::Adam *adam_opt = nullptr;
  if (config.training_context.optimizer == ADAM)
  {
    adam_opt = train_storage.getAdam();
    assert(adam_opt != nullptr);

    adam_opt->fullReset();
  }

  auto tensors = context.getCircleTensors();
  auto tensors_size = tensors->size();
  const auto *operators = context.getCircleOperators();
  const auto num_kernels = operators->size();
  uint32_t num_train_layers = config.training_context.num_of_train_layers == 0
                                ? num_kernels
                                : config.training_context.num_of_train_layers;
  uint32_t last_node_pos = std::min(num_kernels, num_train_layers);
  // Goes among trainable ops
  const auto *op_codes = context.getCircleOpcodes();
  for (uint32_t index = 0; index < last_node_pos; ++index)
  {
    uint32_t cur_op_index = num_kernels - index - 1;
    auto *cur_op = operators->operator[](cur_op_index);

    auto input_tensors = cur_op->inputs();

    for (uint32_t i = 0; i < input_tensors->size(); ++i)
    {
      const auto input_tensor_index = input_tensors->operator[](i);
      // Check is const
      if (not context.isConstTensor(input_tensor_index))
        continue;

      uint32_t cur_opcode_index = cur_op->opcode_index();

      assert(cur_opcode_index < op_codes->size());

      const auto opcode = op_codes->operator[](cur_opcode_index);

      // Check it is trainable const
      if (not isTrainableWeights(opcode))
        continue;

      // Get current weight file pos
      uint32_t cur_weight_offset_pos = weight_offset_pos + input_tensor_index * 4;

      // Read current tensor offset
      uint32_t cur_tensor_offset;
      std::memcpy(&cur_tensor_offset, &data[cur_weight_offset_pos], sizeof(cur_tensor_offset));

      // Check is it save data or not
      // Note: zero means there are no data - it is error
      if (cur_tensor_offset == 0)
        return FailReadCheckpointFile;

      // Read weight data and save it
      const auto tensor = tensors->operator[](input_tensor_index);
      assert(tensor != nullptr);

      OMRuntimeShape shape(tensor);
      auto type_size = sizeof(OMDataType(tensor->type()));

      size_t buffer_size = shape.flatSize() * type_size;
      // Get pointer to the data in model
      uint8_t *weight_data_in_model_ptr;

      status = context.getConstDataByTensorIndex(&weight_data_in_model_ptr, input_tensor_index);
      assert(status == Ok);
      assert(weight_data_in_model_ptr != nullptr);
      if (status != Ok or weight_data_in_model_ptr == nullptr)
        return status;

      std::memcpy(weight_data_in_model_ptr, &data[cur_tensor_offset], buffer_size);

      if (config.training_context.optimizer == SGD)
        continue;

      // For Adam read m and v buffer
      assert(config.training_context.optimizer == ADAM);

      // If no saved Adam state then continue
      if (m_offset == 0 or v_offset == 0)
      {
        assert(v_offset == 0);
        assert(m_offset == 0);
        continue;
      }

      // Get current v and m file pos
      uint32_t cur_m_offset_pos = m_offset + input_tensor_index * 4;
      uint32_t cur_v_offset_pos = v_offset + input_tensor_index * 4;

      // Read current v and m offset
      uint32_t cur_m_offset;
      uint32_t cur_v_offset;
      std::memcpy(&cur_m_offset, &data[cur_m_offset_pos], sizeof(cur_m_offset));
      std::memcpy(&cur_v_offset, &data[cur_v_offset_pos], sizeof(cur_v_offset));

      // Cannot be zero due to weight already not zero
      assert(cur_m_offset != 0 and cur_v_offset != 0);
      if (cur_m_offset == 0 or cur_v_offset == 0)
        return FailReadCheckpointFile;

      assert(adam_opt != nullptr);

      // Allocate memory for m and v current buffer
      uint8_t *m_buffer = nullptr;
      uint8_t *v_buffer = nullptr;
      status = memory::OMMemoryManager::allocateMemory(buffer_size, &m_buffer);
      assert(status == Ok);
      assert(m_buffer != nullptr);
      status = memory::OMMemoryManager::allocateMemory(buffer_size, &v_buffer);
      assert(status == Ok);
      assert(v_buffer != nullptr);

      // Read m and v buffer
      std::memcpy(m_buffer, &data[cur_m_offset], buffer_size);
      std::memcpy(v_buffer, &data[cur_v_offset], buffer_size);

      // Save m and v buffer in adam optimizer state
      adam_opt->setExponentAvgDataByTensorIndex(input_tensor_index, m_buffer);
      adam_opt->setExponentAvgSquaresDataByTensorIndex(input_tensor_index, v_buffer);
    }
  }

  // Read other parameters: cur step num and cur epoch num
  uint32_t cur_step_offset = other_params_offset;
  uint32_t cur_epoch_offset = other_params_offset + 4;

  uint32_t cur_step;
  std::memcpy(&cur_step, &data[cur_step_offset], sizeof(cur_step));

  uint32_t cur_epoch;
  std::memcpy(&cur_epoch, &data[cur_epoch_offset], sizeof(cur_epoch));

  // Save it in config
  config.training_context.num_step = cur_step;
  config.training_context.num_epoch = cur_epoch;

  return status;
}

// To check checkpoint file format please see https://github.com/Samsung/ONE/discussions/13037
OMStatus OMCheckpointLoader::loadCheckpointData(core::OMRuntimeContext &context,
                                                OMTrainingStorage &train_storage, const char *data,
                                                OMConfig &config)
{

  // Validate current checkpoint file data
  OMStatus status = validateCheckpointData(context, data, config);
  assert(status == Ok);
  if (status != Ok)
    return status;

  // Read and save buffer
  status = loadBuffers(context, train_storage, data, config);
  assert(status == Ok);

  return status;
}
