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

#include "train/metrics/MSE.h"

#include <cmath>
#include <cassert>

using namespace onert_micro;
using namespace onert_micro::train;
using namespace onert_micro::train::metrics;

/*
 * E(Y, Y_t) = (SUM(Y_i - Y_t_i)^2) / N
 * SUM - sum among i = (0 ... N - 1)
 */
float MSE::calculateValue(const uint32_t flat_size, const float *calculated_data,
                          const float *target_data)
{
  float result_value = 0.f;

  for (uint32_t i = 0; i < flat_size; ++i)
  {
    const auto cur_val = calculated_data[i] - target_data[i];
    result_value += cur_val * cur_val;
  }

  assert(flat_size != 0);

  return result_value / static_cast<float>(flat_size);
}
