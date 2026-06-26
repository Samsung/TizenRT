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

#ifndef ONERT_MICRO_CORE_TRAIN_CHECKPOINT_LOADER_H
#define ONERT_MICRO_CORE_TRAIN_CHECKPOINT_LOADER_H

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
 * Class to load checkpoints files
 * Note: class is stateless
 */
class OMCheckpointLoader
{
public:
  OMCheckpointLoader() = default;
  OMCheckpointLoader(const OMCheckpointLoader &) = delete;
  OMCheckpointLoader(OMCheckpointLoader &&) = delete;
  OMCheckpointLoader &operator=(const OMCheckpointLoader &) = delete;
  OMCheckpointLoader &&operator=(const OMCheckpointLoader &&) = delete;
  ~OMCheckpointLoader() = default;

  // Load and save states from checkpoint data in model and in config
  // To check checkpoint file format please see https://github.com/Samsung/ONE/discussions/13037
  static OMStatus loadCheckpointData(core::OMRuntimeContext &context,
                                     OMTrainingStorage &train_storage, const char *data,
                                     OMConfig &config);

private:
  static OMStatus validateCheckpointData(core::OMRuntimeContext &context, const char *data,
                                         OMConfig &config);

  static OMStatus loadBuffers(core::OMRuntimeContext &context, OMTrainingStorage &train_storage,
                              const char *data, OMConfig &config);
};

} // namespace train
} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_TRAIN_CHECKPOINT_LOADER_H
