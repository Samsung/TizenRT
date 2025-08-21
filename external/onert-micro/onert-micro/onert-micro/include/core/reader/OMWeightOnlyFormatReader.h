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

#ifndef ONERT_MICRO_CORE_READER_WEIGHT_ONLY_FORMAT_READER_H
#define ONERT_MICRO_CORE_READER_WEIGHT_ONLY_FORMAT_READER_H

#include "OMStatus.h"
#include "OMCircleReader.h"

namespace onert_micro
{
namespace core
{
namespace reader
{
namespace
{

enum WOFFieldsOffsets
{
  MAGIC_NUMBER = 0,
  SCHEMA_VERSION = 2,
  NUM_BUFFERS = 4,
  FIRST_OFFSET = 8
};

} // namespace

constexpr uint16_t wof_magic_number = 429;
constexpr uint8_t schema_version = 1;

/**
 * @brief Loads Weight Only Format files and provides helpers functions
 */
class OMWeightOnlyFormatReader
{
public:
  OMWeightOnlyFormatReader() = default;
  OMWeightOnlyFormatReader(const OMWeightOnlyFormatReader &) = delete;
  OMWeightOnlyFormatReader(OMWeightOnlyFormatReader &&) = default;
  OMWeightOnlyFormatReader &operator=(const OMWeightOnlyFormatReader &) = delete;
  OMWeightOnlyFormatReader &&operator=(const OMWeightOnlyFormatReader &&) = delete;
  ~OMWeightOnlyFormatReader() = default;

public: // direct API
  // To validate _wof_ptr and compare with circle model saved in reader.
  OMStatus validate(OMCircleReader *reader);

  void parse(char *ptr) { _wof_ptr = ptr; }

  // Find pointer to buffer in wof_ptr file for tensor with tensor_index.
  // If tensor_index is not const, then return nullptr.
  char *buffer(const uint32_t tensor_index);

private:
  char *_wof_ptr = nullptr;
};

} // namespace reader
} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_READER_WEIGHT_ONLY_FORMAT_READER_H
