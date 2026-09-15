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

#ifndef ONERT_MICRO_CORE_READER_TRAINING_CONFIG_FILE_READER_H
#define ONERT_MICRO_CORE_READER_TRAINING_CONFIG_FILE_READER_H

#include "OMStatus.h"
#include "OMCircleReader.h"

#include <unordered_map>

namespace onert_micro
{
namespace core
{
namespace reader
{
namespace
{

enum TrainConfigFileFieldsOffsets
{
  MAGIC_NUMBER_FIELD = 0,
  SCHEMA_VERSION_FIELD = 2,
  NUM_LAYERS_FIELD = 4,
  FIRST_LAYER_INDEX_FIELD = 8
};

} // namespace

constexpr uint16_t train_config_file_magic_number = 29;
constexpr uint8_t train_config_file_schema_version = 1;

/**
 * @brief Loads Training Config files and provides helpers functions
 */
class OMTrainingConfigReader
{
public:
  OMTrainingConfigReader() = default;
  OMTrainingConfigReader(const OMTrainingConfigReader &) = delete;
  OMTrainingConfigReader(OMTrainingConfigReader &&) = default;
  OMTrainingConfigReader &operator=(const OMTrainingConfigReader &) = delete;
  OMTrainingConfigReader &&operator=(const OMTrainingConfigReader &&) = delete;
  ~OMTrainingConfigReader() = default;

public:
  // To validate _train_config_ptr and compare with circle model saved in reader.
  OMStatus validate(OMCircleReader *reader);

  // Save pointer
  void parse(char *ptr) { _train_config_ptr = ptr; }

  // Read and return indexes of trainable layers from config file
  // first it is op index in graph, second is rank of the training (see OpTrainableRank)
  std::unordered_map<uint16_t, uint8_t> getTrainableOpsIndexes();

private:
  char *_train_config_ptr;
};

} // namespace reader
} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_READER_TRAINING_CONFIG_FILE_READER_H
