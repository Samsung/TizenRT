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

#ifndef LUCI_INTERPRETER_KERNELS_POOL2DCOMMON_H
#define LUCI_INTERPRETER_KERNELS_POOL2DCOMMON_H

#include "Builders.h"
#include "SISOKernel.h"
#include "Utils.h"

namespace luci_interpreter
{

inline void configure_kernel_CirclePool2DCommon(const circle::Operator *cur_op,
                                                BaseRuntimeGraph *runtime_graph)
{
  const kernels::SISOKernel siso_kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(siso_kernel.input()) ==
                         Tensor::element_type(siso_kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(siso_kernel.input()) == 4);
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(siso_kernel.input()) ==
                         Tensor::num_dims(siso_kernel.output()));

#ifndef DIS_QUANT
  if (Tensor::element_type(siso_kernel.input()) == DataType::U8)
  {
    LUCI_INTERPRETER_CHECK(
      std::abs(Tensor::scale(siso_kernel.output()) - Tensor::scale(siso_kernel.input())) <= 1.0e-6);
    LUCI_INTERPRETER_CHECK(Tensor::zero_point(siso_kernel.output()) ==
                           Tensor::zero_point(siso_kernel.input()));
  }
  else if (Tensor::element_type(siso_kernel.input()) == DataType::S16)
  {
    LUCI_INTERPRETER_CHECK(
      std::abs(Tensor::scale(siso_kernel.output()) - Tensor::scale(siso_kernel.input())) <= 1.0e-6);
    LUCI_INTERPRETER_CHECK(Tensor::zero_point(siso_kernel.input()) == 0 &&
                           Tensor::zero_point(siso_kernel.output()) == 0);
  }
#endif // DIS_QUANT
}

inline luci_interpreter_pal::PoolParams createPoolParams(const circle::Operator *cur_op,
                                                         const kernels::SISOKernel &siso_kernel)
{
  const auto input = siso_kernel.input();
  const auto output = siso_kernel.output();

  const auto *options = cur_op->builtin_options_as_Pool2DOptions();

  const int32_t input_height = Tensor::dim(input, 1);
  const int32_t input_width = Tensor::dim(input, 2);

  const int32_t output_height = kernels::computeOutputSize(
    luci_padding(options->padding()), input_height, options->filter_height(), options->stride_h());
  const int32_t output_width = kernels::computeOutputSize(
    luci_padding(options->padding()), input_width, options->filter_width(), options->stride_w());

  const auto padding_height = kernels::computePadding(options->stride_h(), 1, input_height,
                                                      options->filter_height(), output_height);
  const auto padding_width = kernels::computePadding(options->stride_w(), 1, input_width,
                                                     options->filter_width(), output_width);

  const DataType input_type = Tensor::element_type(input);

  float activation_min{};
  float activation_max{};

  int32_t quantized_activation_min{};
  int32_t quantized_activation_max{};

  if (input_type == DataType::S8 or input_type == DataType::S16)
  {
#ifndef DIS_QUANT
    kernels::calculateActivationRangeQuantized(luci_actfunc(options->fused_activation_function()),
                                               output, &quantized_activation_min,
                                               &quantized_activation_max);
#endif // DIS_QUANT
  }
  else if (input_type == DataType::FLOAT32)
  {
#ifndef DIS_FLOAT
    kernels::calculateActivationRange(luci_actfunc(options->fused_activation_function()),
                                      &activation_min, &activation_max);
#endif // DIS_FLOAT
  }
  else
  {
    assert(false && "Not supported type");
  }

  luci_interpreter_pal::PoolParams params{};
  params.padding_values.height = padding_height;
  params.padding_values.width = padding_width;
  params.stride_height = options->stride_h();
  params.stride_width = options->stride_w();
  params.filter_height = options->filter_height();
  params.filter_width = options->filter_width();
  params.float_activation_min = activation_min;
  params.float_activation_max = activation_max;
  params.quantized_activation_max = quantized_activation_max;
  params.quantized_activation_min = quantized_activation_min;

  return params;
}

} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNELS_POOL2DCOMMON_H
