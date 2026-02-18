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

#ifndef ONERT_MICRO_TRAIN_BACKPROP_ARGS_H
#define ONERT_MICRO_TRAIN_BACKPROP_ARGS_H

#include "OMStatus.h"
#include "core/OMRuntimeContext.h"
#include "core/OMRuntimeStorage.h"
#include "core/OMRuntimeModule.h"
#include "core/train/OMTrainingStorage.h"

namespace onert_micro
{
namespace train
{

/*
 * Args to execute backpropagation graph
 */
struct OMBackpropExecuteArgs
{
  core::OMRuntimeStorage &forward_storage;
  core::OMRuntimeStorage &backward_storage;
  core::OMRuntimeContext &backward_context;
  bool is_last_layer;
  bool is_trainable_layer;
  uint16_t kernel_index;
  core::OpTrainableRankType train_rank_type;
};

} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_TRAIN_BACKPROP_ARGS_H
