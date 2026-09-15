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

#include "train/losses_functions/SparseCrossEntropy.h"
#include <cmath>

using namespace onert_micro;
using namespace onert_micro::train;
using namespace onert_micro::train::losses_functions;

/*
 * dE/dZi = (dE/dy) * (dy / dZi)
 * where Z - vector of logits,
 *       y - probaility of target.
 *
 * Since dE/dy = -(1/y),
 *      (true label)   if  i == y : dE/dZi = py - 1 = y - 1
 *      (wrong label)  else :       dE/dZi = pj
 */
void SparseCrossEntropy::calculateErrorBackpropagation(const uint32_t flat_size,
                                                       const float *calculated_data,
                                                       const float *target_data, float *output_grad)
{
  uint32_t label = static_cast<uint32_t>(target_data[0]);

  for (uint32_t i = 0; i < flat_size; ++i)
  {
    if (i == label)
    {
      output_grad[i] = -1.0 / (calculated_data[i] + float(10.0e-20));
    }
    else
    {
      output_grad[i] = 0.0;
    }
  }
}
