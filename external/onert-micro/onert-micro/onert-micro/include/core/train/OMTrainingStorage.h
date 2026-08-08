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

#ifndef ONERT_MICRO_CORE_TRAIN_TRAINING_STORAGE_H
#define ONERT_MICRO_CORE_TRAIN_TRAINING_STORAGE_H

#include "OMStatus.h"
#include "OMConfig.h"
#include "train/train_optimizers/SGD.h"
#include "train/train_optimizers/Adam.h"

#include <unordered_map>
#include <memory>

namespace onert_micro
{
namespace core
{
namespace train
{

/*
 * Class to store training specific information
 */
class OMTrainingStorage
{
  // Store mapping between input tensor (its input_index) and its current input data.
  // The input data must have a number of samples equal to batch_size
  std::unordered_map<uint32_t, uint8_t *> _input_index_to_input_data;
  // Store mapping between output tensor (its output_index) and its current target data.
  // The target data must have a number of samples equal to batch_size
  std::unordered_map<uint32_t, uint8_t *> _target_index_to_target_data;

  // Store SGD optimizer with its own internal states
  // Note: initial its null
  std::unique_ptr<onert_micro::train::optimizers::SGD> _sgd_optimizer = nullptr;
  // Store Adam optimizer with its own internal states
  // Note: initial its null
  std::unique_ptr<onert_micro::train::optimizers::Adam> _adam_optimizer = nullptr;

  // Store rank types
  std::unordered_map<uint16_t, core::OpTrainableRankType> _tensor_index_to_train_rank;

public:
  OMTrainingStorage() = default;
  OMTrainingStorage(const OMTrainingStorage &) = delete;
  OMTrainingStorage(OMTrainingStorage &&) = delete;
  OMTrainingStorage &operator=(const OMTrainingStorage &) = delete;
  OMTrainingStorage &&operator=(const OMTrainingStorage &&) = delete;
  ~OMTrainingStorage() = default;

  // Set input data for current input tensor
  void setInputData(uint8_t *data, uint32_t input_index)
  {
    _input_index_to_input_data[input_index] = data;
  }
  // Set target data for current output tensor
  void setTargetData(uint8_t *data, uint32_t target_index)
  {
    _target_index_to_target_data[target_index] = data;
  }

  void addTrainRank(uint16_t tensor_index, core::OpTrainableRankType train_rank)
  {
    _tensor_index_to_train_rank[tensor_index] = train_rank;
  }

  std::unordered_map<uint16_t, core::OpTrainableRankType> &getTensorIndexToRankTypeTable()
  {
    return _tensor_index_to_train_rank;
  }

  // Choose and set optimizer defined in config
  OMStatus setOptimizer(const OMConfig &config);

  // Get pointer to SGD optimizer
  // Note: can return nullptr
  onert_micro::train::optimizers::SGD *getSGD() { return _sgd_optimizer.get(); }
  // Get pointer to Adam optimizer
  // Note: can return nullptr
  onert_micro::train::optimizers::Adam *getAdam() { return _adam_optimizer.get(); }

  // Get pointer to saved input data for current input tensor
  uint8_t *getInputData(uint32_t input_index) { return _input_index_to_input_data[input_index]; }
  // Get pointer to saved target data for current output tensor
  uint8_t *getTargetData(uint32_t target_index)
  {
    return _target_index_to_target_data[target_index];
  }

#ifdef OM_MEMORY_ESTIMATE

  // Reset and deallocate all states
  void reset(core::OMRuntimeContext &context, core::OMRuntimeStorage &storage);

#endif // OM_MEMORY_ESTIMATE

  // Reset and deallocate all states
  void reset();
};

} // namespace train
} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_TRAIN_TRAINING_STORAGE_H
