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

#ifndef ONERT_MICRO_CORE_TRAIN_TRAINING_HANDLER_H
#define ONERT_MICRO_CORE_TRAIN_TRAINING_HANDLER_H

#include "OMStatus.h"
#include "OMConfig.h"
#include "core/OMRuntimeContext.h"
#include "core/OMRuntimeStorage.h"
#include "core/train/OMTrainingStorage.h"

namespace onert_micro
{
namespace core
{
namespace train
{

/*
 * Class to handle training process
 */
class OMTrainingHandler
{
  OMTrainingStorage _training_storage;

public:
  OMTrainingHandler() = default;
  OMTrainingHandler(const OMTrainingHandler &) = delete;
  OMTrainingHandler(OMTrainingHandler &&) = delete;
  OMTrainingHandler &operator=(const OMTrainingHandler &) = delete;
  OMTrainingHandler &&operator=(const OMTrainingHandler &&) = delete;
  ~OMTrainingHandler() = default;

  // Save input and target data in OMTrainingStorage
  void setInputData(uint8_t *data, uint32_t input_index)
  {
    _training_storage.setInputData(data, input_index);
  }
  void setTargetData(uint8_t *data, uint32_t target_index)
  {
    _training_storage.setTargetData(data, target_index);
  }

  // Get input and target data from OMTrainingStorage
  uint8_t *getInputData(uint32_t input_index)
  {
    return _training_storage.getInputData(input_index);
  }
  uint8_t *getTargetData(uint32_t target_index)
  {
    return _training_storage.getTargetData(target_index);
  }

  // Handle with current error function (defined in config).
  // Calculate backpropagation error between target and calculated data.
  // Batch_num - number of current sample in current batch (needed to calculate offset to get
  // current target sample)
  OMStatus handleError(const OMConfig &config, OMRuntimeStorage &forward_storage,
                       OMRuntimeStorage &backward_storage, OMRuntimeContext &context,
                       uint32_t batch_num);
  // Handle with updating optimizer state
  OMStatus updateOptimizerState(const OMConfig &config, OMRuntimeStorage &backward_storage,
                                OMRuntimeContext &context);

  // Handle with updating weights with current optimizer
  OMStatus updateWeights(const OMConfig &config, OMRuntimeContext &context,
                         OMRuntimeStorage &storage);

  // Evaluate metric and save result in metric_val
  // Warning: 1) assume that all metric_val for all OMMetrics types actually are float values.
  //          2) metric_val should be initialized with some value before calling this method due to
  //             after calculation for current batch_num (the sequence number of the current sample)
  //             this value is added to metric_val
  OMStatus evaluateMetric(OMMetrics metric, void *metric_val, OMRuntimeStorage &storage,
                          OMRuntimeContext &context, uint32_t batch_num);

  // Save optimizer in OMTrainingStorage
  OMStatus setOptimizer(const OMConfig &config) { return _training_storage.setOptimizer(config); }

  // Get training storage
  OMTrainingStorage &getTrainingStorage() { return _training_storage; }

  // Reset and deallocate all internal states
  void reset();

#ifdef OM_MEMORY_ESTIMATE

  // Reset and deallocate all states
  void reset(core::OMRuntimeContext &context, core::OMRuntimeStorage &storage);

#endif // OM_MEMORY_ESTIMATE
};

} // namespace train
} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_TRAIN_TRAINING_HANDLER_H
