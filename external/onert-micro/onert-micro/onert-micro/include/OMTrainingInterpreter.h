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

#ifndef ONERT_MICRO_TRAINING_INTERPRETER_H
#define ONERT_MICRO_TRAINING_INTERPRETER_H

#include "OMStatus.h"
#include "OMConfig.h"

#include "core/OMTrainingRuntimeModule.h"

namespace onert_micro
{

class OMTrainingInterpreter
{
private:
  core::OMTrainingRuntimeModule _training_runtime_module;

public:
  OMTrainingInterpreter() = default;
  OMTrainingInterpreter(const OMTrainingInterpreter &) = delete;
  OMTrainingInterpreter(OMTrainingInterpreter &&) = delete;
  OMTrainingInterpreter &operator=(const OMTrainingInterpreter &) = delete;
  OMTrainingInterpreter &&operator=(const OMTrainingInterpreter &&) = delete;
  ~OMTrainingInterpreter() = default;

  // Import train model with current config settings
  // Note: model ptr should be non-const to save result
  OMStatus importTrainModel(char *model_ptr, const OMConfig &config);

  // Set input data for input with input_index
  // Note: number of the samples in data should be equal to the batch_size in config structure
  void setInput(uint8_t *data, uint32_t input_index)
  {
    _training_runtime_module.setInputData(data, input_index);
  }
  // Set target data for output with target_index
  // Note: number of the samples in data should be equal to the batch_size in config structure
  void setTarget(uint8_t *data, uint32_t target_index)
  {
    _training_runtime_module.setTargetData(data, target_index);
  }

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
  OMStatus evaluateMetric(const OMConfig &config, OMMetrics metric, void *metric_val,
                          uint32_t test_size)
  {
    return _training_runtime_module.evaluateMetric(config, metric, metric_val, test_size);
  }

  // To get input and output flat size
  uint32_t getInputSizeAt(uint32_t position);
  uint32_t getOutputSizeAt(uint32_t position);

  // Save current model
  OMStatus saveModel(const OMConfig &config, const char *save_path);

  // Save current status as checkpoint
  OMStatus saveCheckpoint(const OMConfig &config, const char *save_path);

  // Load current status from checkpoint and save it in current model and in current config
  OMStatus loadCheckpoint(OMConfig &config, const char *load_path);

  OMStatus run(const OMConfig &config) { return _training_runtime_module.run(config); }
  OMStatus allocateInputs() { return _training_runtime_module.allocateInputs(); }

  void *getInputData(uint32_t position);
  void *getInputDataAt(uint32_t position);
  void *getOutputDataAt(uint32_t position);

#ifdef OM_MEMORY_ESTIMATE
  size_t getPeakFootprintMemory() { return _training_runtime_module.getPeakFootprintMemory(); }
  size_t getCurrentFootprintMemory() { return _training_runtime_module.getPeakFootprintMemory(); }
#endif // OM_MEMORY_ESTIMATE
};

} // namespace onert_micro

#endif // ONERT_MICRO_TRAINING_INTERPRETER_H
