/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_KERNELS_BINARYOPUTILS_H
#define LUCI_INTERPRETER_KERNELS_BINARYOPUTILS_H

#include "TISOKernel.h"
#include "PALComparisons.h"
#include "Params.h"
#include "ProcessBroadcastShapes.h"
#include "Utils.h"

namespace luci_interpreter
{
namespace kernels
{

namespace
{

/**
 * Fills activation min and max parameters depending on given data type and activation
 *
 * T is a template parameter, so after optimization this code left with only required if case
 *
 * @tparam T data type of arithmetic operation output tensor
 * @param params tflite params to fill
 * @param activation luci_interpreter::Activation of arithmetic operation
 */
template <typename T>
void fillArithmeticActivationRange(luci_interpreter_pal::ArithmeticParams &p, Activation act)
{
  static_assert(one_of_types<T, float, int32_t, int64_t>(), "Unsupported dtype");

  if (std::is_same<T, float>::value)
    calculateActivationRange(act, &p.float_activation_min, &p.float_activation_max);
  if (std::is_same<T, int32_t>::value)
    calculateActivationRange(act, &p.quantized_activation_min, &p.quantized_activation_max);
  else
    calculateActivationRange(act, &p.int64_activation_min, &p.int64_activation_max);
}

} // namespace

template <typename T, typename TISOFunc = nullptr_t, typename TISOBroadcastFunc = nullptr_t,
          typename Options = nullptr_t>
void evalTISOKernel(TISOFunc tiso_func, TISOBroadcastFunc tiso_broadcast_func,
                    kernels::TISOKernel *kernel, kernels::TISOData *kernel_data,
                    const Options *options, RuntimeShape &&input_shape_1,
                    RuntimeShape &&input_shape_2, RuntimeShape &&output_shape)
{
  luci_interpreter_pal::ArithmeticParams params{};
  fillArithmeticActivationRange<T>(params, luci_actfunc(options->fused_activation_function()));

  const bool need_broadcast =
    luci_interpreter_pal::ProcessBroadcastShapes(input_shape_1, input_shape_2, &params);

  if (need_broadcast)
  {
    tiso_broadcast_func(params, input_shape_1, kernels::getTensorData<T>(kernel_data->input1_data),
                        input_shape_2, kernels::getTensorData<T>(kernel_data->input2_data),
                        output_shape, kernels::getTensorData<T>(kernel_data->output_data));
  }
  else
  {
    const int flat_size = input_shape_1.flatSize();
    tiso_func(params, flat_size, kernels::getTensorData<T>(kernel_data->input1_data),
              kernels::getTensorData<T>(kernel_data->input2_data),
              kernels::getTensorData<T>(kernel_data->output_data));
  }
}

template <typename T, typename TISOFunc = nullptr_t, typename TISOBroadcastFunc = nullptr_t,
          typename Options = nullptr_t>
void evalTISOInplaceKernel(TISOFunc tiso_func, TISOBroadcastFunc tiso_broadcast_func,
                           kernels::TISOKernel *kernel, const Options *options,
                           RuntimeShape &&input_shape_1, RuntimeShape &&input_shape_2,
                           RuntimeShape &&output_shape)
{
  uint8_t *inplace_data_ptr = nullptr;
  circle::Tensor *input_inplace_tensor = nullptr;

  kernels::TISOData kernel_data = kernel->readInplaceData(inplace_data_ptr, input_inplace_tensor);

  evalTISOKernel<T, TISOFunc, TISOBroadcastFunc, Options>(
    tiso_func, tiso_broadcast_func, kernel, &kernel_data, options, std::move(input_shape_1),
    std::move(input_shape_2), std::move(output_shape));

  BaseRuntimeGraph *runtime_graph = kernel->runtime_graph();

  runtime_graph->makeInplaceOperation(input_inplace_tensor, kernel->output());
  if (input_inplace_tensor == kernel->input1())
  {
    runtime_graph->makeInplaceOperation(kernel->input2(), nullptr);
  }
  else
  {
    runtime_graph->makeInplaceOperation(kernel->input1(), nullptr);
  }
}

inline void CheckBinaryOpDataTypesEqual(const kernels::TISOKernel &kernel)
{
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.input2()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.output()));
}

#ifndef DIS_QUANT
template <typename T, typename TISOFunc = nullptr_t, typename TISOBroadcastFunc = nullptr_t,
          typename Options = nullptr_t>
void evalTISOQuantizedKernel(TISOFunc tiso_func, TISOBroadcastFunc tiso_broadcast_func,
                             kernels::TISOKernel *kernel, kernels::TISOData *kernel_data,
                             const Options *options)
{
  const auto *input1 = kernel->input1();
  const auto *input2 = kernel->input2();
  const auto *output = kernel->output();

  const auto input1_scale = static_cast<double>(Tensor::scale(input1));
  const auto input2_scale = static_cast<double>(Tensor::scale(input2));
  const auto output_scale = static_cast<double>(Tensor::scale(output));

  const int left_shift = 20;
  const double twice_max_input_scale = 2 * std::max(input1_scale, input2_scale);
  const double real_input1_multiplier = input1_scale / twice_max_input_scale;
  const double real_input2_multiplier = input2_scale / twice_max_input_scale;
  const double real_output_multiplier = twice_max_input_scale / ((1 << left_shift) * output_scale);

  int32_t input1_multiplier{}, input2_multiplier{}, output_multiplier{};
  int input1_shift{}, input2_shift{}, output_shift{};
  kernels::quantizeMultiplierSmallerThanOneExp(real_input1_multiplier, &input1_multiplier,
                                               &input1_shift);
  kernels::quantizeMultiplierSmallerThanOneExp(real_input2_multiplier, &input2_multiplier,
                                               &input2_shift);
  kernels::quantizeMultiplierSmallerThanOneExp(real_output_multiplier, &output_multiplier,
                                               &output_shift);

  int32_t activation_min{};
  int32_t activation_max{};
  kernels::calculateActivationRangeQuantized(luci_actfunc(options->fused_activation_function()),
                                             output, &activation_min, &activation_max);

  luci_interpreter_pal::ArithmeticParams params{};
  params.left_shift = left_shift;
  // The kernel expects inputs' zero points to be negated.
  params.input1_offset = -Tensor::zero_point(input1); // Note the '-'.
  params.input1_multiplier = input1_multiplier;
  params.input1_shift = input1_shift;
  params.input2_offset = -Tensor::zero_point(input2); // Note the '-'.
  params.input2_multiplier = input2_multiplier;
  params.input2_shift = input2_shift;
  params.output_offset = Tensor::zero_point(output);
  params.output_multiplier = output_multiplier;
  params.output_shift = output_shift;
  params.quantized_activation_min = activation_min;
  params.quantized_activation_max = activation_max;

  const bool need_broadcast = luci_interpreter_pal::ProcessBroadcastShapes(
    kernels::getTensorShape(input1), kernels::getTensorShape(input2), &params);

  if (need_broadcast)
  {
    tiso_broadcast_func(
      params, kernels::getTensorShape(input1), kernels::getTensorData<T>(kernel_data->input1_data),
      kernels::getTensorShape(input2), kernels::getTensorData<T>(kernel_data->input2_data),
      kernels::getTensorShape(output), kernels::getTensorData<T>(kernel_data->output_data));
  }
  else
  {
    tiso_func(params, kernels::getTensorShape(input1),
              kernels::getTensorData<uint8_t>(kernel_data->input1_data),
              kernels::getTensorShape(input2), kernels::getTensorData<T>(kernel_data->input2_data),
              kernels::getTensorShape(output), kernels::getTensorData<T>(kernel_data->output_data));
  }
}

template <typename T, typename TISOFunc = nullptr_t, typename TISOBroadcastFunc = nullptr_t,
          typename Options = nullptr_t>
void evalTISOInplaceQuantizedKernel(TISOFunc tiso_func, TISOBroadcastFunc tiso_broadcast_func,
                                    kernels::TISOKernel *kernel, const Options *options)
{
  uint8_t *inplace_data_ptr = nullptr;
  circle::Tensor *input_inplace_tensor = nullptr;

  kernels::TISOData kernel_data = kernel->readInplaceData(inplace_data_ptr, input_inplace_tensor);

  evalTISOQuantizedKernel<T, TISOFunc, TISOBroadcastFunc, Options>(tiso_func, tiso_broadcast_func,
                                                                   kernel, &kernel_data, options);

  kernel->runtime_graph()->makeInplaceOperation(input_inplace_tensor, kernel->output());
  if (input_inplace_tensor == kernel->input1())
  {
    kernel->runtime_graph()->makeInplaceOperation(kernel->input2(), nullptr);
  }
  else
  {
    kernel->runtime_graph()->makeInplaceOperation(kernel->input1(), nullptr);
  }
}

#endif // DIS_QUANT

} // namespace kernels
} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNELS_BINARYOPUTILS_H
