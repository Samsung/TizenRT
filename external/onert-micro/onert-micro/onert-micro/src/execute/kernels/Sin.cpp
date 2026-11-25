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

#include "execute/kernels/MathCommon.h"
#include "PALSin.h"

using namespace onert_micro;
using namespace onert_micro::execute;

// NOTE: doesnt currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleSin(const OMExecuteArgs &execute_args)
{
  auto sin_float_lambda = [](const core::OMRuntimeShape &input_shape, const float *input_data,
                             const core::OMRuntimeShape &output_shape, float *output_data) {
    return pal::Sin(input_shape, input_data, output_shape, output_data);
  };

  return execute_math_common(execute_args, sin_float_lambda);
}

} // namespace execute
} // namespace onert_micro
