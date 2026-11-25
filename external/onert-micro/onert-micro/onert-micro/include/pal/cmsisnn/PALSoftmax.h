/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2017 The TensorFlow Authors. All Rights Reserved.
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

#ifndef ONERT_MICRO_EXECUTE_PAL_SOFTMAX_H
#define ONERT_MICRO_EXECUTE_PAL_SOFTMAX_H

#include "PALSoftmaxCommon.h"

#include <arm_nnfunctions.h>

namespace onert_micro
{
namespace execute
{
namespace pal
{

template <>
OMStatus Softmax<int8_t, int8_t>(const core::SoftmaxParams &params, const int8_t *input_data,
                                 int8_t *output_data)
{
  arm_softmax_s8(input_data, params.num_rows, params.row_size, params.input_multiplier,
                 params.input_left_shift, params.diff_min, output_data);

  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_SOFTMAX_H
