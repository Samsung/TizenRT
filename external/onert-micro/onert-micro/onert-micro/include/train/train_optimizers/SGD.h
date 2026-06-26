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

#ifndef ONERT_MICRO_TRAIN_TRAIN_OPTIMIZERS_SGD_H
#define ONERT_MICRO_TRAIN_TRAIN_OPTIMIZERS_SGD_H

#include "OMStatus.h"
#include "core/OMRuntimeStorage.h"
#include "core/OMRuntimeContext.h"

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
class SGD
{
private:
  // Save mapping between tensor index and internal state data with calculated gradients
  std::unordered_map<uint16_t, uint8_t *> _tensor_index_to_gradient;

public:
  SGD() = default;
  SGD(const SGD &) = delete;
  SGD(SGD &&) = delete;
  SGD &operator=(const SGD &) = delete;
  SGD &&operator=(const SGD &&) = delete;
  ~SGD() { reset(); }

#ifdef OM_MEMORY_ESTIMATE
  // Reset and deallocate all internal states
  void reset(core::OMRuntimeContext &context, core::OMRuntimeStorage &storage);
#endif // OM_MEMORY_ESTIMATE

  // Reset and deallocate all internal states
  void reset();

  // Update internal states according to SGD theory
  OMStatus handle(core::OMRuntimeStorage &backward_storage, core::OMRuntimeContext &context,
                  core::OMRuntimeStorage &storage);

  // Update weights according to SGD theory
  OMStatus updateWeights(
    const OMTrainingContext &training_config, core::OMRuntimeContext &context,
    core::OMRuntimeStorage &storage,
    std::unordered_map<uint16_t, core::OpTrainableRankType> &tensor_index_to_rank_type_map);
};

} // namespace optimizers
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_TRAIN_TRAIN_OPTIMIZERS_SGD_H
