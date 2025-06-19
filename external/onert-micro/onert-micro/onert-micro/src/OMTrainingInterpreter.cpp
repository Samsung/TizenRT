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

#include "OMTrainingInterpreter.h"

#ifndef DIS_STREAM
#include <fstream>
#endif // DIS_STREAM

using namespace onert_micro;

OMStatus OMTrainingInterpreter::importTrainModel(char *model_ptr, const OMConfig &config)
{
  assert(model_ptr != nullptr && "Model ptr shouldn't be nullptr");
  if (model_ptr == nullptr)
    return UnknownError;

  return _training_runtime_module.importTrainModel(model_ptr, config);
}

OMStatus OMTrainingInterpreter::trainSingleStep(OMConfig &config)
{
  return _training_runtime_module.trainSingleStep(config);
}

OMStatus OMTrainingInterpreter::reset() { return _training_runtime_module.reset(); }

uint32_t OMTrainingInterpreter::getInputSizeAt(uint32_t position)
{
  return _training_runtime_module.getInputSizeAt(position);
}

uint32_t OMTrainingInterpreter::getOutputSizeAt(uint32_t position)
{
  return _training_runtime_module.getOutputSizeAt(position);
}

OMStatus OMTrainingInterpreter::saveModel(const OMConfig &config, const char *save_path)
{
  if (save_path == nullptr or config.model_size == 0 or config.model_ptr == nullptr)
    return UnknownError;

#ifndef DIS_STREAM
  // Open or create file
  // Note: if the file existed, it will be overwritten
  std::ofstream out_file(save_path, std::ios::binary | std::ios::trunc);
  if (not out_file.is_open())
    return UnknownError;

  // Write data
  out_file.write(config.model_ptr, config.model_size);

  // Close file
  out_file.close();
#else
  assert(false && "Not supported");
  return UnknownError;
#endif // DIS_STREAM

  // Saving done
  return Ok;
}

OMStatus OMTrainingInterpreter::loadCheckpoint(OMConfig &config, const char *load_path)
{
  // Not imported or path is empty
  if (load_path == nullptr or config.model_ptr == nullptr or config.model_size == 0)
    return UnknownError;

  // Get DataBuffer (vector of chars) of checkpoints
  std::vector<char> checkpoint_data;

  // Read data
#ifndef DIS_STREAM
  std::ifstream file(load_path, std::ios::binary | std::ios::in);
  if (!file.good())
  {
    assert(false && "Fail to open");
    return UnknownError;
  }

  file.seekg(0, std::ios::end);
  auto fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  // reserve capacity
  checkpoint_data.resize(fileSize);

  // read the data
  file.read(checkpoint_data.data(), fileSize);
  if (file.fail())
  {
    assert(false && "Fail to read");
    return UnknownError;
  }
#else
  assert(false && "Not supported");
  return UnknownError;
#endif // DIS_STREAM

  // Load data
  OMStatus status = _training_runtime_module.loadCheckpointData(config, checkpoint_data.data());

  return status;
}

OMStatus OMTrainingInterpreter::saveCheckpoint(const OMConfig &config, const char *save_path)
{
  // Not imported or path is empty
  if (save_path == nullptr or config.model_ptr == nullptr or config.model_size == 0)
    return UnknownError;

  // Get DataBuffer (vector of chars) of checkpoints
  std::vector<char> checkpoint_data;

  OMStatus status = _training_runtime_module.createCheckpointFile(config, checkpoint_data);

  assert(status == Ok);
  if (status != Ok)
    return status;

    // Save it into save_path
#ifndef DIS_STREAM
  // Open or create file
  // Note: if the file existed, it will be overwritten
  std::ofstream out_file(save_path, std::ios::binary | std::ios::trunc);
  if (not out_file.is_open())
    return UnknownError;

  // Write data
  out_file.write(checkpoint_data.data(), checkpoint_data.size());

  // Close file
  out_file.close();
#else
  assert(false && "Not supported");
  return UnknownError;
#endif // DIS_STREAM

  return Ok;
}

void *OMTrainingInterpreter::getInputDataAt(uint32_t position)
{
  return _training_runtime_module.getInputDataAt(position);
}

void *OMTrainingInterpreter::getOutputDataAt(uint32_t position)
{
  return _training_runtime_module.getOutputDataAt(position);
}

void *OMTrainingInterpreter::getInputData(uint32_t position)
{
  return _training_runtime_module.getInputData(position);
}
