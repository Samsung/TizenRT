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

#include "kernels/Pow.h"
#include "kernels/Utils.h"

#include <tensorflow/lite/kernels/internal/reference/reference_ops.h>

namespace luci_interpreter
{
namespace kernels
{

Pow::Pow(const Tensor *input1, const Tensor *input2, Tensor *output)
  : Kernel({input1, input2}, {output})
{
}

void Pow::configure()
{
  LUCI_INTERPRETER_CHECK(input1()->element_type() == input2()->element_type());
  LUCI_INTERPRETER_CHECK(input1()->element_type() == output()->element_type());
  // TODO: enable it only if kernel with dynamic shapes
  output()->resize(calculateShapeForBroadcast(input1()->shape(), input2()->shape()));
}

void Pow::execute() const
{
  switch (input1()->element_type())
  {
    case DataType::FLOAT32:
      eval<float>();
      break;
    case DataType::S32:
      eval<int32_t>();
      break;
    default:
      assert(false && "Unsupported type.");
  }
}

template <typename T> void Pow::eval() const
{
  tflite::ArithmeticParams params{};

  const bool need_broadcast = tflite::reference_ops::ProcessBroadcastShapes(
    getTensorShape(input1()), getTensorShape(input2()), &params);

  if (need_broadcast)
  {
    tflite::reference_ops::BroadcastPow4DSlow(getTensorShape(input1()), getTensorData<T>(input1()),
                                              getTensorShape(input2()), getTensorData<T>(input2()),
                                              getTensorShape(output()), getTensorData<T>(output()));
  }
  else
  {
    tflite::reference_ops::Pow(getTensorShape(input1()), getTensorData<T>(input1()),
                               getTensorShape(input2()), getTensorData<T>(input2()),
                               getTensorShape(output()), getTensorData<T>(output()));
  }
}

} // namespace kernels
} // namespace luci_interpreter
