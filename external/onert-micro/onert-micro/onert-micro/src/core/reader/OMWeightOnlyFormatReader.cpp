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

#include "core/reader/OMWeightOnlyFormatReader.h"

using namespace onert_micro::core::reader;
using namespace onert_micro;

/*
 * Validate method for Weight Only Format file to check its correctness
 */
OMStatus OMWeightOnlyFormatReader::validate(OMCircleReader *reader)
{
  OMStatus status = Ok;

  // Validate magic number
  uint16_t mag_num = 0;
  std::memcpy(&mag_num, &_wof_ptr[MAGIC_NUMBER], sizeof(mag_num));
  assert(mag_num == wof_magic_number && "False MAGIC NUMBER, check correctness of wof file");
  if (mag_num != wof_magic_number)
    return FailReadWOFFile;

  // Validate schema version
  uint8_t version = 0;
  std::memcpy(&version, &_wof_ptr[SCHEMA_VERSION], sizeof(version));
  assert(version == schema_version && "False MAGIC NUMBER, check correctness of wof file");
  if (version != schema_version)
    return FailReadWOFFile;

  // Validate count of tensors
  assert(reader != nullptr && "Reader should exist");
  if (reader == nullptr)
    return ModelNotImport;
  uint32_t num_tensors = reader->tensors()->size();
  uint32_t num_tensors_in_wof_file = 0;
  std::memcpy(&num_tensors_in_wof_file, &_wof_ptr[NUM_BUFFERS], sizeof(num_tensors_in_wof_file));
  assert(num_tensors == num_tensors_in_wof_file &&
         "Number of tensors in circle and in wof file should be the same");
  if (num_tensors != num_tensors_in_wof_file)
    return FailReadWOFFile;

  return status;
}

/*
 * Get pointer to saved data (weights data) for tensor with tensor index.
 * If tensor index not save in wof file return nullptr
 */
char *OMWeightOnlyFormatReader::buffer(const uint32_t tensor_index)
{
  if (_wof_ptr == nullptr)
    return nullptr;

  uint32_t buffer_offset = 0;
  uint32_t offset_offset = FIRST_OFFSET + tensor_index * sizeof(uint32_t);

  std::memcpy(&buffer_offset, &_wof_ptr[offset_offset], sizeof(buffer_offset));

  if (buffer_offset == 0)
    return nullptr;

  return _wof_ptr + buffer_offset;
}
