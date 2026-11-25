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
#include "core/train/OMCheckpointSaver.h"

using namespace onert_micro::core::train;
using namespace onert_micro::core;
using namespace onert_micro::train;
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

constexpr uint16_t MAGIC_NUMBER = 429;
constexpr uint8_t SCHEMA_VERSION = 1;

} // namespace

/**
 * Calculate result buffer size
 **/
size_t OMCheckpointSaver::calculateFileSize(core::OMRuntimeContext &context,
                                            OMTrainingStorage &train_storage,
                                            const OMConfig &config)
{
  size_t result = 0;

  // 2 bytes for Magic Number
  result += 2;

  // 1 byte for Schema version
  result += 1;

  // 1 byte for Reserved field
  result += 1;

  // 4 bytes for Adam's state m buffers offset value
  result += 4;

  // 4 bytes for Adam's state v buffers offset value
  result += 4;

  // 4 bytes for others parameters offset value
  result += 4;

  // 4 bytes for number of tensors
  result += 4;

  auto tensors = context.getCircleTensors();

  auto tensors_size = tensors->size();

  // tensors_size * 4 bytes for buffers offsets
  result += tensors_size * 4;

  const auto *operators = context.getCircleOperators();
  const auto num_kernels = operators->size();
  uint32_t num_train_layers = config.training_context.num_of_train_layers == 0
                                ? num_kernels
                                : config.training_context.num_of_train_layers;
  uint32_t last_node_pos = std::min(num_kernels, num_train_layers);
  // Goes among trainable ops
  size_t buffer_size = 0;
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

      const auto tensor = context.getTensorByIndex(input_tensor_index);
      OMRuntimeShape shape(tensor);

      auto type_size = sizeof(OMDataType(tensor->type()));

      buffer_size += type_size * shape.flatSize();
    }
  }

  // If we use Adam optimizer then need to add Adam specific buffers
  assert(config.training_context.optimizer == SGD or
         config.training_context.optimizer == ADAM && "Unsupported type");
  if (config.training_context.optimizer == ADAM)
  {
    // Check is it save any state
    if (not train_storage.getAdam()->isReset())
    {
      // If yes - then just buffer_size = buffer_size * 3 (original weights and two buffers from
      // Adam state)
      buffer_size *= 3;

      // Add offsets for m
      result += tensors_size * 4;
      // Add offsets for v
      result += tensors_size * 4;
    }
  }

  // Add buffer size
  result += buffer_size;

  // 4 bytes to save information about current step
  result += 4;

  // 4 bytes to save information about current epoch
  result += 4;

  return result;
}

OMStatus OMCheckpointSaver::writeOffsetsAndBuffers(core::OMRuntimeContext &context,
                                                   OMTrainingStorage &train_storage,
                                                   const OMConfig &config, std::vector<char> &data)
{
  // Point to start of the buffer
  char *cur_ptr = data.data();

  // Set to the n_buffers field
  cur_ptr += 16;

  auto tensors = context.getCircleTensors();
  auto tensors_size = tensors->size();

  // Write number of buffers
  std::memcpy(cur_ptr, &tensors_size, sizeof(tensors_size));

  // Calculate buffers offsets, set all to zeros
  // Zero value means that there is no buffer for this tensor
  std::vector<uint32_t> offsets(tensors_size, 0);
  // Start offset for buffers
  uint32_t cur_offset = 20 + tensors_size * 4;

  // To calculate sum of saved buffers sizes
  uint32_t acc_buffer_size = 0;

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

      // Found trainable weight tensor, lets calculate its size and save offset for current buffer
      const auto tensor = context.getTensorByIndex(input_tensor_index);
      OMRuntimeShape shape(tensor);

      auto type_size = sizeof(OMDataType(tensor->type()));

      size_t buffer_size = type_size * shape.flatSize();
      // Save for current tensor index its offset
      offsets[input_tensor_index] = cur_offset;
      // Get buffer data
      uint8_t *tensor_data;
      OMStatus status = context.getConstDataByTensorIndex(&tensor_data, input_tensor_index);
      assert(status == Ok);
      if (status != Ok)
        return status;
      assert(tensor_data != nullptr);

      // Write buffer data into vector
      cur_ptr = data.data() + cur_offset;
      std::memcpy(cur_ptr, tensor_data, buffer_size);
      // Move offset
      cur_offset += buffer_size;
      // Save buffers size
      acc_buffer_size += buffer_size;
    }
  }

  // Now cur_offset points to last position after adding all buffers
  // Let's handle with Adam buffers offsets
  assert(config.training_context.optimizer == ADAM or
         config.training_context.optimizer == SGD && "Unsupported type");
  if (config.training_context.optimizer == ADAM and not train_storage.getAdam()->isReset())
  {
    // Move pointer to m offset field
    cur_ptr = data.data() + 4;
    // Save offset for the m offset field
    uint32_t m_offset = cur_offset;
    std::memcpy(cur_ptr, &m_offset, sizeof(m_offset));
    // Move pointer to the v offset field
    cur_ptr += 4;
    // Calculate offset for the v offset field
    uint32_t v_offset = m_offset + acc_buffer_size + tensors_size * 4;
    // Save it
    std::memcpy(cur_ptr, &v_offset, sizeof(v_offset));

    // Let's write offsets and buffers
    const auto adam_opt = train_storage.getAdam();
    assert(adam_opt != nullptr);

    // Move m and v to buffers offsets
    uint32_t m_buffer_offset = m_offset + tensors_size * 4;
    uint32_t v_buffer_offset = v_offset + tensors_size * 4;

    // Adam buffers
    std::vector<uint32_t> m_offsets(tensors_size, 0);
    std::vector<uint32_t> v_offsets(tensors_size, 0);

    // Goes among trainable ops
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

        // Found trainable weight tensor, lets calculate its size and save offset for current buffer
        const auto tensor = context.getTensorByIndex(input_tensor_index);
        OMRuntimeShape shape(tensor);

        auto type_size = sizeof(OMDataType(tensor->type()));

        size_t buffer_size = type_size * shape.flatSize();
        // Save for current tensor index its offset
        m_offsets[input_tensor_index] = m_buffer_offset;
        v_offsets[input_tensor_index] = v_buffer_offset;

        // Obtain m and v data from train storage
        uint8_t *m_data = adam_opt->getExponentAvgDataByTensorIndex(input_tensor_index);
        assert(m_data != nullptr);
        uint8_t *v_data = adam_opt->getExponentAvgSquaresDataByTensorIndex(input_tensor_index);
        assert(v_data != nullptr);

        // Write m data
        cur_ptr = data.data() + m_buffer_offset;
        std::memcpy(cur_ptr, m_data, buffer_size);
        // Write v data
        cur_ptr = data.data() + v_buffer_offset;
        std::memcpy(cur_ptr, v_data, buffer_size);

        // Move m and v buffers offsets
        m_buffer_offset += buffer_size;
        v_buffer_offset += buffer_size;
      }
    }

    // Set pointer to the m offset field start
    char *m_offset_start = data.data() + m_offset;
    // Set pointer to the v offset field start
    char *v_offset_start = data.data() + v_offset;
    // Write m and v offsets
    for (uint32_t i = 0; i < tensors_size; ++i)
    {
      // Save m buffer offset
      uint32_t cur_m_offset = m_offsets[i];
      std::memcpy(m_offset_start, &cur_m_offset, sizeof(cur_m_offset));
      m_offset_start += 4;

      // Save v buffer offset
      uint32_t cur_v_offset = v_offsets[i];
      std::memcpy(v_offset_start, &cur_v_offset, sizeof(cur_v_offset));
      v_offset_start += 4;
    }
  }
  else
  {
    // Note: offset = 0 - means there are no such buffers
    // Move pointer to m offset field
    cur_ptr = data.data() + 4;
    // Save offset for the m offset field
    uint32_t m_offset = 0;
    std::memcpy(cur_ptr, &m_offset, sizeof(m_offset));

    // Move pointer to the v offset field
    cur_ptr += 4;
    // Save offset for the v offset field
    uint32_t v_offset = 0;
    std::memcpy(cur_ptr, &v_offset, sizeof(v_offset));
  }

  // Move cur_ptr to the start of the offsets field
  cur_ptr = data.data() + 20;
  // Write offsets
  for (uint32_t i = 0; i < tensors_size; ++i)
  {
    uint32_t offset = offsets[i];
    std::memcpy(cur_ptr, &offset, sizeof(offset));
    cur_ptr += 4;
  }

  // Save other parameters offset: 20 initial bytes + tensors_size * 4 bytes for buffer offsets +
  // buffer size
  uint32_t other_parameters_offset = 20 + tensors_size * 4 + acc_buffer_size;
  // Adam case need add two more acc_buffer_size
  if (config.training_context.optimizer == ADAM and not train_storage.getAdam()->isReset())
  {
    other_parameters_offset += acc_buffer_size * 2;
    other_parameters_offset += tensors_size * 4 * 2;
  }

  // Write this offset
  cur_ptr = data.data() + 12;
  std::memcpy(cur_ptr, &other_parameters_offset, sizeof(other_parameters_offset));

  // Move pointer to other parameters offset
  cur_ptr = data.data() + other_parameters_offset;

  // Write current step
  std::memcpy(cur_ptr, &config.training_context.num_step, sizeof(config.training_context.num_step));

  cur_ptr += 4;
  // Write current epoch
  std::memcpy(cur_ptr, &config.training_context.num_epoch,
              sizeof(config.training_context.num_epoch));

  return Ok;
}

// To check checkpoint file format please see https://github.com/Samsung/ONE/discussions/13037
OMStatus OMCheckpointSaver::createCheckpointData(core::OMRuntimeContext &context,
                                                 OMTrainingStorage &train_storage,
                                                 std::vector<char> &data, const OMConfig &config)
{
  // Clear data
  data.clear();

  // Obtain file size and resize vector
  const size_t data_size = calculateFileSize(context, train_storage, config);
  data.resize(data_size);

  // Point to start of the buffer
  char *cur_ptr = data.data();

  // Write MAGIC_NUMBER
  std::memcpy(cur_ptr, &MAGIC_NUMBER, sizeof(MAGIC_NUMBER));
  cur_ptr += 2;

  // Write SCHEMA_VERSION
  std::memcpy(cur_ptr, &SCHEMA_VERSION, sizeof(SCHEMA_VERSION));
  cur_ptr += 1;

  // Miss RESERVED field
  cur_ptr += 1;

  // Writes buffers and offsets
  OMStatus status = writeOffsetsAndBuffers(context, train_storage, config, data);

  assert(status == Ok);

  return Ok;
}
