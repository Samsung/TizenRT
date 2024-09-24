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

#include "ConvolutionCommon.h"

#include "Utils.h"

namespace luci_interpreter
{

namespace
{

bool isSupportedQuantized(const circle::Tensor *input, const circle::Tensor *weights)
{
  return Tensor::element_type(input) == DataType::U8 and Tensor::scales(weights).size() == 1;
}

bool isSupportedQuantizedPerChannel(const circle::Tensor *input, const circle::Tensor *weights)
{
  return (Tensor::element_type(input) == DataType::U8 or
          Tensor::element_type(input) == DataType::S8) and
         Tensor::scales(weights).size() > 1;
}

} // namespace

int32_t computeConvPadding(const circle::Tensor *input, const circle::Tensor *filter,
                           circle::Padding padding_type, int32_t stride, int32_t dilation, int axis)
{
  const int32_t input_dim = Tensor::dim(input, axis);
  const int32_t filter_dim = Tensor::dim(filter, axis);
  const int32_t output_dim =
    kernels::computeOutputSize(luci_padding(padding_type), input_dim, filter_dim, stride, dilation);

  const auto padding = kernels::computePadding(stride, dilation, input_dim, filter_dim, output_dim);

  return padding;
}

luci_interpreter_pal::ConvParams createConv2DParams(const circle::Tensor *input,
                                                    const circle::Tensor *filter,
                                                    const circle::Tensor *output,
                                                    const circle::Conv2DOptions *options)
{
  luci_interpreter_pal::ConvParams params{};

  if (isSupportedQuantized(input, filter))
  {
#ifndef DIS_QUANT
    const auto input_scale = static_cast<double>(Tensor::scale(input));
    const auto filter_scale = static_cast<double>(Tensor::scale(filter));
    const auto output_scale = static_cast<double>(Tensor::scale(output));

    const double real_multiplier = input_scale * filter_scale / output_scale;
    int32_t output_multiplier{};
    int output_shift{};
    kernels::quantizeMultiplier(real_multiplier, &output_multiplier, &output_shift);

    params.output_multiplier = output_multiplier;
    params.output_shift = output_shift;

    int32_t activation_min{};
    int32_t activation_max{};
    kernels::calculateActivationRangeQuantized(luci_actfunc(options->fused_activation_function()),
                                               output, &activation_min, &activation_max);

    // The kernel expects input and filter zero points to be negated.
    params.input_offset = -Tensor::zero_point(input);    // Note the '-'.
    params.weights_offset = -Tensor::zero_point(filter); // Note the '-'.
    params.output_offset = Tensor::zero_point(output);
    params.quantized_activation_min = activation_min;
    params.quantized_activation_max = activation_max;
#endif // DIS_QUANT
  }
  else if (isSupportedQuantizedPerChannel(input, filter))
  {
#ifndef DIS_QUANT
    int32_t activation_min{};
    int32_t activation_max{};
    kernels::calculateActivationRangeQuantized(luci_actfunc(options->fused_activation_function()),
                                               output, &activation_min, &activation_max);

    const std::vector<double> effective_output_scale = kernels::getQuantizedConvolutionMultiplers(
      Tensor::scale(input), Tensor::scales(filter), Tensor::scale(output));

    size_t n = effective_output_scale.size();
    params.per_channel_output_shift.resize(n);
    params.per_channel_output_multiplier.resize(n);
    for (size_t i = 0; i < n; ++i)
    {
      kernels::quantizeMultiplier(effective_output_scale[i],
                                  &params.per_channel_output_multiplier[i],
                                  &params.per_channel_output_shift[i]);

      // The kernel expects input and filter zero points to be negated.
      params.input_offset = -Tensor::zero_point(input);    // Note the '-'.
      params.weights_offset = -Tensor::zero_point(filter); // Note the '-'.
      params.output_offset = Tensor::zero_point(output);
      params.quantized_activation_min = activation_min;
      params.quantized_activation_max = activation_max;
    }
#endif // DIS_QUANT
  }
  else if (Tensor::element_type(input) == DataType::FLOAT32 and
           Tensor::element_type(filter) == DataType::FLOAT32)
  {
#ifndef DIS_FLOAT
    float activation_min{};
    float activation_max{};
    kernels::calculateActivationRange(luci_actfunc(options->fused_activation_function()),
                                      &activation_min, &activation_max);

    params.float_activation_min = activation_min;
    params.float_activation_max = activation_max;
#endif // DIS_FLOAT
  }
  else
  {
    assert(false && "Not supported type");
  }

  const auto padding = options->padding();
  const auto stride_height = options->stride_h();
  const auto stride_width = options->stride_w();
  const auto dilation_height_factor = options->dilation_h_factor();
  const auto dilation_width_factor = options->dilation_h_factor();

  params.padding_values.height =
    computeConvPadding(input, filter, padding, stride_height, dilation_height_factor, 1);
  params.padding_values.width =
    computeConvPadding(input, filter, padding, stride_width, dilation_width_factor, 2);
  params.stride_height = stride_height;
  params.stride_width = stride_width;
  params.dilation_height_factor = dilation_height_factor;
  params.dilation_width_factor = dilation_width_factor;

  return params;
}

} // namespace luci_interpreter
