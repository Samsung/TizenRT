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

#include "kernels/InstanceNorm.h"

#include "kernels/Utils.h"

#include <tensorflow/lite/kernels/internal/common.h>
#include <cmath>

namespace luci_interpreter
{
namespace kernels
{

InstanceNorm::InstanceNorm(const Tensor *input, const Tensor *gamma, const Tensor *beta,
                           Tensor *output, const InstanceNormParams &params)
  : KernelWithParams<InstanceNormParams>({input, gamma, beta}, {output}, params)
{
}

void InstanceNorm::configure()
{
  LUCI_INTERPRETER_CHECK(input()->shape().num_dims() == 4);
  LUCI_INTERPRETER_CHECK(input()->element_type() == output()->element_type());
  LUCI_INTERPRETER_CHECK(gamma()->element_type() == input()->element_type());
  LUCI_INTERPRETER_CHECK(gamma()->shape().num_dims() == 1);
  LUCI_INTERPRETER_CHECK(gamma()->shape().dim(0) == input()->shape().dim(3) ||
                         gamma()->shape().dim(0) == 1);
  LUCI_INTERPRETER_CHECK(beta()->element_type() == input()->element_type());
  LUCI_INTERPRETER_CHECK(beta()->shape().num_dims() == 1);
  LUCI_INTERPRETER_CHECK(beta()->shape().dim(0) == input()->shape().dim(3) ||
                         beta()->shape().dim(0) == 1);
  // TODO: enable it only if kernel with dynamic shapes
  output()->resize(input()->shape());
}

void InstanceNorm::execute() const
{
  switch (input()->element_type())
  {
    case DataType::FLOAT32:
      evalFloat();
      break;
    default:
      assert(false && "Unsupported type.");
  }
}

void InstanceNorm::evalFloat() const
{
  float activation_min, activation_max;
  calculateActivationRange(params().activation, &activation_min, &activation_max);
  auto input_shape = getTensorShape(input());
  auto output_shape = getTensorShape(output());
  const int32_t batches = tflite::MatchingDim(input_shape, 0, output_shape, 0);
  const int32_t heights = tflite::MatchingDim(input_shape, 1, output_shape, 1);
  const int32_t widths = tflite::MatchingDim(input_shape, 2, output_shape, 2);
  const int32_t channels = tflite::MatchingDim(input_shape, 3, output_shape, 3);
  const float *input_data = getTensorData<float>(input());
  const float *gamma_data = getTensorData<float>(gamma());
  auto gamma_shape = getTensorShape(gamma());
  bool single_gamma = gamma_shape.DimensionsCount() == 1 && gamma_shape.Dims(0) == 1;
  const float *beta_data = getTensorData<float>(beta());
  auto beta_shape = getTensorShape(beta());
  bool single_beta = beta_shape.DimensionsCount() == 1 && beta_shape.Dims(0) == 1;
  float *output_data = getTensorData<float>(output());
  for (int32_t batch = 0; batch < batches; batch++)
  {
    for (int32_t channel = 0; channel < channels; channel++)
    {
      double sum = 0.0f;
      double square_sum = 0.0f;
      int32_t size = heights * widths;
      for (int32_t height = 0; height < heights; height++)
      {
        for (int32_t width = 0; width < widths; width++)
        {
          double input_val = input_data[tflite::Offset(input_shape, batch, height, width, channel)];
          sum += input_val;
          square_sum += (input_val * input_val);
        }
      }
      double mean = sum / size;
      double var = square_sum / size - mean * mean;

      double gamma = single_gamma ? gamma_data[0] : gamma_data[channel];
      double beta = single_beta ? beta_data[0] : beta_data[channel];
      double a = gamma / (std::sqrt(var + params().epsilon));
      double b = -mean * a + beta;

      for (int32_t height = 0; height < heights; height++)
      {
        for (int32_t width = 0; width < widths; width++)
        {
          double input_value =
            input_data[tflite::Offset(output_shape, batch, height, width, channel)];
          double output_value = input_value * a + b;
          output_data[tflite::Offset(output_shape, batch, height, width, channel)] =
            tflite::ActivationFunctionWithMinMax((float)output_value, activation_min,
                                                 activation_max);
        }
      }
    }
  }
}

} // namespace kernels
} // namespace luci_interpreter
