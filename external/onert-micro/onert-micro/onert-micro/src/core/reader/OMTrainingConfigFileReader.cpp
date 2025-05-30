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

#include "core/reader/OMTrainingConfigFileReader.h"

using namespace onert_micro::core::reader;
using namespace onert_micro;

/*
 * Validate method for Weight Only Format file to check its correctness
 */
OMStatus OMTrainingConfigReader::validate(OMCircleReader *reader)
{
  OMStatus status = Ok;

  // Validate magic number
  uint16_t mag_num = 0;
  std::memcpy(&mag_num, &_train_config_ptr[MAGIC_NUMBER_FIELD], sizeof(mag_num));
  assert(mag_num == train_config_file_magic_number &&
         "False MAGIC NUMBER, check correctness of wof file");
  if (mag_num != train_config_file_magic_number)
    return FailReadWOFFile;

  // Validate schema version
  uint8_t version = 0;
  std::memcpy(&version, &_train_config_ptr[SCHEMA_VERSION_FIELD], sizeof(version));
  assert(version == train_config_file_schema_version &&
         "False MAGIC NUMBER, check correctness of wof file");
  if (version != train_config_file_schema_version)
    return FailReadWOFFile;

  // Validate count of ops is not greater than current model has
  assert(reader != nullptr && "Reader should exist");
  if (reader == nullptr)
    return ModelNotImport;
  uint32_t num_ops = reader->operators()->size();
  uint32_t num_ops_in_file = 0;
  std::memcpy(&num_ops_in_file, &_train_config_ptr[NUM_LAYERS_FIELD], sizeof(num_ops_in_file));
  assert(num_ops_in_file > 0 and num_ops >= num_ops_in_file &&
         "Number of operators in circle should be greater than train config file has");
  if (num_ops_in_file > 0 and num_ops < num_ops_in_file)
    return FailReadWOFFile;

  return status;
}
/*
 * Read and return indexes of trainable layers from config file
 */
std::unordered_map<uint16_t, uint8_t> OMTrainingConfigReader::getTrainableOpsIndexes()
{
  std::unordered_map<uint16_t, uint8_t> result;

  // If reader is not parsed then return empty vector
  if (_train_config_ptr == nullptr)
    return result;

  // Read number of ops
  uint32_t num_ops_in_file = 0;
  std::memcpy(&num_ops_in_file, &_train_config_ptr[NUM_LAYERS_FIELD], sizeof(num_ops_in_file));

  assert(num_ops_in_file > 0);
  // Obtain pointer to the first layer index position in the file
  char *cur_op_index_ptr = &_train_config_ptr[FIRST_LAYER_INDEX_FIELD];
  char *cur_op_train_rank_ptr =
    &_train_config_ptr[FIRST_LAYER_INDEX_FIELD + sizeof(uint16_t) * num_ops_in_file];
  // Fill result set with indexes and its rank
  for (uint32_t i = 0; i < num_ops_in_file; ++i)
  {
    // Read op index
    uint16_t cur_op_index;
    std::memcpy(&cur_op_index, cur_op_index_ptr, sizeof(cur_op_index));
    cur_op_index_ptr += sizeof(cur_op_index);

    // Read op train rank
    uint8_t cur_op_train_rank;
    std::memcpy(&cur_op_train_rank, cur_op_train_rank_ptr, sizeof(cur_op_train_rank));
    cur_op_train_rank_ptr += sizeof(cur_op_train_rank);

    // Insert op index and op rank
    result[cur_op_index] = cur_op_train_rank;
  }

  return result;
}
