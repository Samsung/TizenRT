/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "kernels/ReverseV2.h"
#include "kernels/Utils.h"
#include <tensorflow/lite/kernels/internal/reference/reference_ops.h>

namespace luci_interpreter
{

namespace kernels
{

ReverseV2::ReverseV2(const Tensor *input, const Tensor *axes, Tensor *output)
  : Kernel({input, axes}, {output})
{
}

void ReverseV2::configure()
{
  assert(axes()->shape().num_dims() == 1);
  assert(input()->shape().num_dims() >= axes()->shape().num_elements());
  if (input()->element_type() != DataType::S32 && input()->element_type() != DataType::FLOAT32 &&
      input()->element_type() != DataType::U8 && input()->element_type() != DataType::S16 &&
      input()->element_type() != DataType::S64)
  {
    assert(false && "Unsupported input type.");
  }
  if (axes()->element_type() != DataType::S32)
  {
    assert(false && "Unsupported axes type.");
  }
  if (axes()->shape().num_elements() > 1)
  {
    assert(false && "Current implementation does not support more than 1 axis.");
  }
  int axis_value = getTensorData<int32_t>(axes())[0];
  if (axis_value < 0 || axis_value >= input()->shape().num_dims())
  {
    assert(false && "Invalid axes value");
  }
  assert(input()->element_type() == output()->element_type());

  // TODO: enable it only if kernel with dynamic shapes
  output()->resize(input()->shape());
}

void ReverseV2::execute() const
{
  int axis_value = getTensorData<int32_t>(axes())[0];
  switch (output()->element_type())
  {
    case DataType::FLOAT32:
      tflite::reference_ops::Reverse<float>(axis_value, getTensorShape(input()),
                                            getTensorData<float>(input()), getTensorShape(output()),
                                            getTensorData<float>(output()));
      break;
    case DataType::U8:
      tflite::reference_ops::Reverse<uint8_t>(
        axis_value, getTensorShape(input()), getTensorData<uint8_t>(input()),
        getTensorShape(output()), getTensorData<uint8_t>(output()));
      break;
    default:
      assert(false && "Unsupported output type");
  }
}

} // namespace kernels
} // namespace luci_interpreter
