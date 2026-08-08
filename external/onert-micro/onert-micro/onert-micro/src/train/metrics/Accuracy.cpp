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

#include "train/metrics/Accuracy.h"

using namespace onert_micro;
using namespace onert_micro::train;
using namespace onert_micro::train::metrics;

/*
 * return 1.0 if predicted class equals to target
 * return 0.0 otherwise
 */
float Accuracy::calculateValue(const uint32_t flat_size, const float *calculated_data,
                               const float *target_data)
{
  // Find target class
  float target_class = 0.f;
  float target_max_val = target_data[0];
  for (uint32_t i = 0; i < flat_size; ++i)
  {
    if (target_max_val < target_data[i])
    {
      target_max_val = target_data[i];
      target_class = static_cast<float>(i);
    }
  }
  // Find predicted class
  float pred_class = 0.f;
  float pred_max_val = calculated_data[0];
  for (uint32_t i = 0; i < flat_size; ++i)
  {
    if (pred_max_val < calculated_data[i])
    {
      pred_max_val = calculated_data[i];
      pred_class = static_cast<float>(i);
    }
  }

  return pred_class == target_class ? 1.0f : 0.0f;
}
