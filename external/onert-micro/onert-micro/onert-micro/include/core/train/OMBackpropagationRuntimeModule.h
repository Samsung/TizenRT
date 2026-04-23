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

#ifndef ONERT_MICRO_CORE_TRAIN_BACKPROPAGATION_RUNTIME_MODULE_H
#define ONERT_MICRO_CORE_TRAIN_BACKPROPAGATION_RUNTIME_MODULE_H

#include "OMStatus.h"
#include "OMConfig.h"

#include "core/OMRuntimeGraph.h"
#include "core/reader/OMCircleReader.h"
#include "core/train/OMTrainingStorage.h"

#include <vector>

namespace onert_micro
{
namespace core
{
namespace train
{

class OMBackpropagationRuntimeModule
{
private:
  std::vector<OMRuntimeGraph> _main_runtime_graphs;
  std::vector<OMRuntimeGraph> _backpropagation_runtime_graphs;
  train::OMTrainingStorage _training_storage;
  bool _is_train_mode = false;

public:
  OMTrainingRuntimeModule() = default;
  OMTrainingRuntimeModule(const OMTrainingRuntimeModule &) = delete;
  OMTrainingRuntimeModule(OMTrainingRuntimeModule &&) = delete;
  OMTrainingRuntimeModule &operator=(const OMTrainingRuntimeModule &) = delete;
  OMTrainingRuntimeModule &&operator=(const OMTrainingRuntimeModule &&) = delete;
  ~OMTrainingRuntimeModule() = default;

  void set_train_mode(bool is_train_mode) { _is_train_mode = is_train_mode; }

  // Import circle forward and backward model
  OMStatus import(const char *model_ptr, const char *backpropagation_model_ptr, const OMConfig &config);

  // Do forward training phase:
  // Calculate outputs for current input
  OMStatus forward();

  // Do backward training phase:
  // Calculate and update gradients
  // If batch = 1, then also update weights
  OMStatus backward();

  // Update all trainable weights
  // Note: if batch = 1, then this method do nothing,
  // all changes do internal in backward
  OMStatus updateWeights();

  // Reset(delete all internal values) for all interpreter
  OMStatus reset();

  // NUmber of inputs/outputs/targets nodes
  uint32_t getNumberOfInputs();
  uint32_t getNumberOfOutputs();
  uint32_t getNumberOfTargets();

  // Allocate memory for all inputs/targets nodes
  OMStatus allocateInputs();
  OMStatus allocateTargets();

  // Size of the inputs/outputs/targets nodes (count of elements)
  uint32_t getInputSizeAt(uint32_t position);
  uint32_t getOutputSizeAt(uint32_t position);
  uint32_t getTargetSizeAt(uint32_t position);

  // Return ptr to the data input/output/target node in position = [0, get*SizeAt)
  void *getInputDataAt(uint32_t position);
  void *getOutputDataAt(uint32_t position);
  void *getTargetDataAt(uint32_t position);

private:
  OMStatus importMainModel(const char *model_ptr, const OMConfig &config);

  OMStatus importBackpropagationModel(const char *backpropagation_model_ptr, const OMConfig &config);

  // Method to update with current optimization strategy
  void updateSGDGradients(uint8_t *dest, uint8_t *src, size_t size);
};

} // namespace train
} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_TRAIN_BACKPROPAGATION_RUNTIME_MODULE_H
