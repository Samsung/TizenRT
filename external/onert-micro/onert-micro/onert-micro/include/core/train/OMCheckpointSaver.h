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

#ifndef ONERT_MICRO_CORE_TRAIN_CHECKPOINT_SAVER_H
#define ONERT_MICRO_CORE_TRAIN_CHECKPOINT_SAVER_H

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
 * Class to save checkpoints
 * Note: class is stateless
 */
class OMCheckpointSaver
{
public:
  OMCheckpointSaver() = default;
  OMCheckpointSaver(const OMCheckpointSaver &) = delete;
  OMCheckpointSaver(OMCheckpointSaver &&) = delete;
  OMCheckpointSaver &operator=(const OMCheckpointSaver &) = delete;
  OMCheckpointSaver &&operator=(const OMCheckpointSaver &&) = delete;
  ~OMCheckpointSaver() = default;

  // Create checkpoint data for current state
  // To check checkpoint file format please see https://github.com/Samsung/ONE/discussions/13037
  static OMStatus createCheckpointData(core::OMRuntimeContext &context,
                                       OMTrainingStorage &train_storage, std::vector<char> &data,
                                       const OMConfig &config);

private:
  static size_t calculateFileSize(core::OMRuntimeContext &context, OMTrainingStorage &train_storage,
                                  const OMConfig &config);

  static OMStatus writeOffsetsAndBuffers(core::OMRuntimeContext &context,
                                         OMTrainingStorage &train_storage, const OMConfig &config,
                                         std::vector<char> &data);
};

} // namespace train
} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_TRAIN_CHECKPOINT_SAVER_H
