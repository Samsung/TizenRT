/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef ONERT_MICRO_EXECUTE_PAL_SOFTMAX_GRAD_H
#define ONERT_MICRO_EXECUTE_PAL_SOFTMAX_GRAD_H

#include "Params.h"
#include "core/OMKernelData.h"

#include <cmath>

namespace onert_micro
{
namespace execute
{
namespace pal
{



OMStatus SoftmaxGrad(const float *input_data, float *output_data, const onert_micro::core::OMRuntimeShape &output_shape)
{
  int dim_value = output_shape.dims(0);

  for (int i = 0; i < dim_value; ++i)
  {
    for (int j = 0; j < dim_value; ++j)
    {
      output_data[i * dim_value + j] = (-input_data[i]) * input_data[j];
    }
  }

  for (int i = 0; i < dim_value; ++i)
  {
    output_data[i * dim_value + i] += input_data[i];
  }

  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_SOFTMAX_GRAD_H
