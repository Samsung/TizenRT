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

#ifndef ONERT_MICRO_CORE_TRAINING_RUNTIME_MODULE_H
#define ONERT_MICRO_CORE_TRAINING_RUNTIME_MODULE_H

#include "OMStatus.h"
#include "OMConfig.h"
#include "core/OMRuntimeModule.h"
#include "core/train/OMTrainingHandler.h"

#include <vector>

namespace onert_micro
{
namespace core
{
/*
 * Class to handle all training process
 */
class OMTrainingRuntimeModule : public OMRuntimeModule
{
private:
  // Store backwards graphs
  std::vector<OMRuntimeGraph> _backward_graphs;
  // Store training handler
  train::OMTrainingHandler _training_handler;

public:
  OMTrainingRuntimeModule() = default;
  OMTrainingRuntimeModule(const OMTrainingRuntimeModule &) = delete;
  OMTrainingRuntimeModule(OMTrainingRuntimeModule &&) = delete;
  OMTrainingRuntimeModule &operator=(const OMTrainingRuntimeModule &) = delete;
  OMTrainingRuntimeModule &&operator=(const OMTrainingRuntimeModule &&) = delete;
  ~OMTrainingRuntimeModule() = default;

  // Import train model with current config settings
  // Note: model ptr should be non-const to save result
  OMStatus importTrainModel(char *model_ptr, const OMConfig &config);

  // Train single step: run forward graph (with data which was set in SetInput) ->
  // -> calculate error (with target data which was set in SetTarget) ->
  // -> run backward graph -> update optimizer state -> after batch_size steps update weights
  // Warning: before using trainSingleStep call: 1) importTrainModel; 2) setInput; 3) setTarget
  OMStatus trainSingleStep(OMConfig &config);

  // Reset all states and data saved into OMTrainingInterpreter (trained weights will not be reset)
  OMStatus reset();

  // Calculate and save metric into metric_val: run forward graph -> calculate metric
  // Note: calculation will be done on test_size number of test samples
  // Warning: before using evaluateMetric call: 1) importTrainModel; 2) setInput; 3) setTarget
  // Note: number of the samples in data should be equal to the test_size
  // Warning: 1) assume that all metric_val for all OMMetrics types actually are float values.
  //          2) metric_val should be initialized with some value before calling this method due to
  //             after calculation for current batch_num (the sequence number of the current sample)
  //             this value is added to metric_val
  OMStatus evaluateMetric(const OMConfig &config, OMMetrics metric, void *metric_val,
                          uint32_t test_size);

  // Set input data for input with input_index
  // Note: number of the samples in data should be equal to the batch_size in config structure
  void setInputData(uint8_t *data, uint32_t input_index)
  {
    _training_handler.setInputData(data, input_index);
  }
  // Set target data for output with target_index
  // Note: number of the samples in data should be equal to the batch_size in config structure
  void setTargetData(uint8_t *data, uint32_t target_index)
  {
    _training_handler.setTargetData(data, target_index);
  }

  // Create and save checkpoint data into data_buffer vector
  // To check checkpoint file format please see https://github.com/Samsung/ONE/discussions/13037
  OMStatus createCheckpointFile(const OMConfig &config, std::vector<char> &data_buffer);

  // Load checkpoints data and save it in model data and in config
  // To check checkpoint file format please see https://github.com/Samsung/ONE/discussions/13037
  OMStatus loadCheckpointData(OMConfig &config, const char *data);

  void *getInputData(int32_t index);

#ifdef OM_MEMORY_ESTIMATE
  size_t getPeakFootprintMemory();
  size_t getCurrentFootprintMemory();
#endif // OM_MEMORY_ESTIMATE
};

} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_TRAINING_RUNTIME_MODULE_H
