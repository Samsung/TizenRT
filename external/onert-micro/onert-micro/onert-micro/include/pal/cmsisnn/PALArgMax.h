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

#ifndef ONERT_MICRO_EXECUTE_PAL_ARG_MAX_H
#define ONERT_MICRO_EXECUTE_PAL_ARG_MAX_H

#include "PALArgMinMaxCommon.h"
namespace onert_micro
{
namespace execute
{
namespace pal
{

template <typename T1, typename T2, typename T3>
OMStatus ArgMax(const core::OMRuntimeShape &input1_shape, const T1 *input1_data,
                const T3 *input2_data, const core::OMRuntimeShape &output_shape, T2 *output_data)
{
  return ArgMinMax(input1_shape, input1_data, input2_data, output_shape, output_data,
                   std::greater<T1>());
}
} // namespace pal
} // namespace execute
} // namespace onert_micro
#endif // ONERT_MICRO_EXECUTE_PAL_ARG_MAX_H
