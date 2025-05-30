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

#ifndef ONERT_MICRO_TRAIN_METRICS_CROSS_ENTROPY_H
#define ONERT_MICRO_TRAIN_METRICS_CROSS_ENTROPY_H

#include "OMStatus.h"

#include <cstdint>

namespace onert_micro
{
namespace train
{
namespace metrics
{

struct CrossEntropy
{
  // Calculate cross entropy metric between calculated and target data
  static float calculateValue(const uint32_t flat_size, const float *calculated_data,
                              const float *target_data);
};

} // namespace metrics
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_TRAIN_METRICS_CROSS_ENTROPY_H
