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

#include "core/train/OMTrainingStorage.h"

#include <cassert>

using namespace onert_micro::core::train;
using namespace onert_micro::train;
using namespace onert_micro;

OMStatus OMTrainingStorage::setOptimizer(const OMConfig &config)
{
  switch (config.training_context.optimizer)
  {
    case SGD:
    {
      if (_sgd_optimizer != nullptr)
      {
        return UnknownError;
      }
      _sgd_optimizer = std::make_unique<optimizers::SGD>();
      break;
    }
    case ADAM:
    {
      if (_adam_optimizer != nullptr)
      {
        return UnknownError;
      }
      _adam_optimizer = std::make_unique<optimizers::Adam>();
      break;
    }
    default:
      assert(false && "Unsupported Optimizer type");
  }
  return Ok;
}

void OMTrainingStorage::reset()
{
  if (_sgd_optimizer)
    _sgd_optimizer->reset();

  if (_adam_optimizer)
    _adam_optimizer->fullReset();

  _target_index_to_target_data.clear();
  _input_index_to_input_data.clear();
}

#ifdef OM_MEMORY_ESTIMATE
void OMTrainingStorage::reset(core::OMRuntimeContext &context, core::OMRuntimeStorage &storage)
{
  if (_sgd_optimizer)
    _sgd_optimizer->reset(context, storage);

  if (_adam_optimizer)
    _adam_optimizer->fullReset(context, storage);

  _target_index_to_target_data.clear();
  _input_index_to_input_data.clear();
}
#endif // OM_MEMORY_ESTIMATE
