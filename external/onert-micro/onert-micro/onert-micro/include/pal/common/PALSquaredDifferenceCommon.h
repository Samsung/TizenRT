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

#ifndef ONERT_MICRO_EXECUTE_PAL_SQUARED_DIFFERENCE_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_SQUARED_DIFFERENCE_COMMON_H

#include "PALArithmeticOpCommon.h"

namespace onert_micro
{
namespace execute
{
namespace pal
{
template <typename T>
OMStatus SquaredDifference(const core::BinaryArithmeticBroadcastParams &params, const int flat_size,
                           const T *input1_data, const T *input2_data, T *output_data)
{
  ArithmeticOp<T, SquaredDifferenceFn<T>>(params, flat_size, input1_data, input2_data, output_data);
  return Ok;
}

template <typename T>
OMStatus
BroadcastSquaredDifference4DSlow(const core::BinaryArithmeticBroadcastParams &params,
                                 const core::OMRuntimeShape &input1_shape, const T *input1_data,
                                 const core::OMRuntimeShape &input2_shape, const T *input2_data,
                                 const core::OMRuntimeShape &output_shape, T *output_data)
{
  BroadcastArithmeticOp4DSlow<T, SquaredDifferenceFn<T>>(
    params, input1_shape, input1_data, input2_shape, input2_data, output_shape, output_data);
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_SQUARED_DIFFERENCE_COMMON_H
