/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2020 The TensorFlow Authors. All Rights Reserved.
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

#ifndef LUCI_INTERPRETER_PAL_RELU_COMMON_H
#define LUCI_INTERPRETER_PAL_RELU_COMMON_H

#include "PALUtils.h"
#include "Params.h"
#include "ProcessBroadcastShapes.h"

namespace luci_interpreter_pal
{

void BroadcastPrelu4DSlowFloat(const luci_interpreter::RuntimeShape &unextended_input1_shape,
                               const float *input1_data,
                               const luci_interpreter::RuntimeShape &unextended_input2_shape,
                               const float *input2_data,
                               const luci_interpreter::RuntimeShape &unextended_output_shape,
                               float *output_data)
{
  const luci_interpreter::RuntimeShape output_shape =
    luci_interpreter::RuntimeShape::extendedShape(4, unextended_output_shape);

  NdArrayDesc<4> desc1;
  NdArrayDesc<4> desc2;
  NdArrayDescsForElementwiseBroadcast(unextended_input1_shape, unextended_input2_shape, &desc1,
                                      &desc2);

  for (int b = 0; b < output_shape.dims(0); ++b)
  {
    for (int y = 0; y < output_shape.dims(1); ++y)
    {
      for (int x = 0; x < output_shape.dims(2); ++x)
      {
        for (int c = 0; c < output_shape.dims(3); ++c)
        {
          auto out_idx = offset(output_shape.dimsData(), b, y, x, c);
          auto in1_idx = subscriptToIndex(desc1, b, y, x, c);
          auto in2_idx = subscriptToIndex(desc2, b, y, x, c);
          auto in1_val = input1_data[in1_idx];
          auto in2_val = input2_data[in2_idx];
          output_data[out_idx] = in1_val >= 0.0f ? in1_val : in1_val * in2_val;
        }
      }
    }
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_RELU_COMMON_H
