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

#ifndef ONERT_MICRO_TRAIN_TRAIN_OPTIMIZERS_ADAM_H
#define ONERT_MICRO_TRAIN_TRAIN_OPTIMIZERS_ADAM_H

#include "OMStatus.h"
#include "core/OMRuntimeStorage.h"
#include "core/OMRuntimeContext.h"
#include "core/OMRuntimeStorage.h"

#include <cstdint>
#include <unordered_map>

namespace onert_micro
{
namespace train
{
namespace optimizers
{

/*
 * Class to handle Adam optimizer
 */
class Adam
{
private:
  // Save mapping between tensor index and internal state data with calculated exponent average
  // squares
  std::unordered_map<uint16_t, uint8_t *> _tensor_to_exponent_avg_squares;
  // Save mapping between tensor index and internal state data with calculated exponent average
  std::unordered_map<uint16_t, uint8_t *> _tensor_to_exponent_avg;
  // Save mapping between tensor index and internal state data with calculated gradients
  std::unordered_map<uint16_t, uint8_t *> _tensor_index_to_gradient;

public:
  Adam() = default;
  Adam(const Adam &) = delete;
  Adam(Adam &&) = delete;
  Adam &operator=(const Adam &) = delete;
  Adam &&operator=(const Adam &&) = delete;
  ~Adam() { fullReset(); }

#ifdef OM_MEMORY_ESTIMATE
  // Reset and deallocate all internal states
  void fullReset(core::OMRuntimeContext &context, core::OMRuntimeStorage &storage);

  // Reset only gradients
  void reset(core::OMRuntimeContext &context, core::OMRuntimeStorage &storage);
#endif // OM_MEMORY_ESTIMATE

  // Reset and deallocate all internal states
  void fullReset();

  // Reset only gradients
  void reset();

  // Check is contains current state or not
  // Needed for saving checkpoints
  bool isReset()
  {
    return _tensor_to_exponent_avg_squares.empty() or _tensor_to_exponent_avg.empty();
  }

  // Get exponent and exponent squares data be tensor index
  uint8_t *getExponentAvgDataByTensorIndex(uint16_t tensor_index);
  uint8_t *getExponentAvgSquaresDataByTensorIndex(uint16_t tensor_index);
  // Set exponent and exponent squares data be tensor index
  void setExponentAvgDataByTensorIndex(uint16_t tensor_index, uint8_t *data);
  void setExponentAvgSquaresDataByTensorIndex(uint16_t tensor_index, uint8_t *data);

  // Update internal states according to Adam theory
  OMStatus handle(core::OMRuntimeStorage &backward_storage, core::OMRuntimeContext &context,
                  core::OMRuntimeStorage &storage);

  // Update weights according to Adam theory
  OMStatus updateWeights(const OMTrainingContext &training_config, core::OMRuntimeContext &context,
                         core::OMRuntimeStorage &storage,
                         std::unordered_map<uint16_t, core::OpTrainableRankType> &);
};

} // namespace optimizers
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_TRAIN_TRAIN_OPTIMIZERS_ADAM_H
