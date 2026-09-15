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

#ifndef ONERT_MICRO_TRAIN_LOSSES_FUNCTIONS_MSE_H
#define ONERT_MICRO_TRAIN_LOSSES_FUNCTIONS_MSE_H

#include "OMStatus.h"

#include <cstdint>

namespace onert_micro
{
namespace train
{
namespace losses_functions
{

// MSE- Mean Square Error
struct MSE
{
  // Calculate mean square error backpropagation between calculated and target data
  static void calculateErrorBackpropagation(const uint32_t flat_size, const float *calculated_data,
                                            const float *target_data, float *output_grad);
};

} // namespace losses_functions
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_TRAIN_LOSSES_FUNCTIONS_MSE_H
