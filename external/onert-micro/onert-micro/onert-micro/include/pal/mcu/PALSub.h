/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2019 The TensorFlow Authors. All Rights Reserved.
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

#ifndef ONERT_MICRO_EXECUTE_PAL_SUB_H
#define ONERT_MICRO_EXECUTE_PAL_SUB_H

#include "PALSubCommon.h"
#include "PALUtils.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{

OMStatus Sub(const core::ArithmeticQuantParams &params, const uint32_t flat_size,
             const int8_t *input1_data, const int8_t *input2_data, int8_t *output_data)
{
  ElementWise(flat_size, params, input1_data, input2_data, output_data, SubFunc);
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_MUL_H
