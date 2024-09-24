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

#include "kernels/LocalResponseNormalization.h"

#include "kernels/Utils.h"

#include "PALLocalResponseNormalization.h"

namespace luci_interpreter
{

namespace kernels
{

LocalResponseNormalization::LocalResponseNormalization(
  const Tensor *input, Tensor *output, const LocalResponseNormalizationParams &params)
  : KernelWithParams<LocalResponseNormalizationParams>({input}, {output}, params)
{
}

void LocalResponseNormalization::configure()
{
  LUCI_INTERPRETER_CHECK(input()->shape().num_dims() == 4);
  LUCI_INTERPRETER_CHECK(output()->element_type() == DataType::FLOAT32);
  LUCI_INTERPRETER_CHECK(input()->element_type() == output()->element_type());
  // TODO: enable it only if kernel with dynamic shapes
  output()->resize(input()->shape());
}

void LocalResponseNormalization::execute() const
{
  switch (output()->element_type())
  {
    case DataType::FLOAT32:
      tflite::LocalResponseNormalizationParams op_params;
      op_params.range = params().radius;
      op_params.bias = params().bias;
      op_params.alpha = params().alpha;
      op_params.beta = params().beta;
      luci_interpreter_pal::LocalResponseNormalization(
        op_params, getTensorShape(input()), getTensorData<float>(input()), getTensorShape(output()),
        getTensorData<float>(output()));
      break;
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace kernels
} // namespace luci_interpreter
