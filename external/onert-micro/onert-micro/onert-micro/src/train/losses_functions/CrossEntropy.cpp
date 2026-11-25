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

#include "train/losses_functions/CrossEntropy.h"

using namespace onert_micro;
using namespace onert_micro::train;
using namespace onert_micro::train::losses_functions;

/*
 * dE/dY = Y_t / Y
 * where Y - vector of calculated outputs,
 *       Y_t - vector of target outputs
 * Note: size of Y and Y_t should be the same and equal to flat_size
 */
void CrossEntropy::calculateErrorBackpropagation(const uint32_t flat_size,
                                                 const float *calculated_data,
                                                 const float *target_data, float *output_grad)
{
  for (uint32_t i = 0; i < flat_size; ++i)
  {
    output_grad[i] = -1.f * target_data[i] / (calculated_data[i] + float(10.0e-32));
  }
}
