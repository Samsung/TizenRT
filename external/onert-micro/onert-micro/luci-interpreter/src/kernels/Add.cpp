/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2019 The TensorFlow Authors. All Rights Reserved.
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

#include "Builders.h"
#include "kernels/Utils.h"

#include "kernels/BinaryOpCommon.h"

#include "PALAdd.h"

namespace luci_interpreter
{

namespace
{

#ifndef DIS_QUANT
void evalQuantized(const circle::Tensor *input1, const circle::Tensor *input2,
                   const circle::Tensor *output, const circle::AddOptions *options,
                   BaseRuntimeGraph *runtime_graph, DataType type, bool is_inplace)
{
  assert(type == DataType::S16 or type == DataType::S8 && "Wrong Type");

  luci_interpreter_pal::ArithmeticParams params{};
  luci_interpreter::RuntimeShape input_shape1 =
    kernels::getTensorRuntimeShape(input1, runtime_graph);
  luci_interpreter::RuntimeShape input_shape2 =
    kernels::getTensorRuntimeShape(input2, runtime_graph);

  const bool need_broadcast =
    luci_interpreter_pal::ProcessBroadcastShapes(input_shape1, input_shape2, &params);

  assert(need_broadcast == false && "Broadcast for INT8 and INT16 not supported now");

  params.input1_offset = -Tensor::zero_point(input1);
  params.input2_offset = -Tensor::zero_point(input2);
  params.output_offset = Tensor::zero_point(output);
  params.left_shift = (type == DataType::S16) ? 15 : 20;

  const auto input1_scale = Tensor::scale(input1);
  const auto input2_scale = Tensor::scale(input2);
  const auto output_scale = Tensor::scale(output);

  const double twice_max_input_scale =
    2 * static_cast<double>(std::max(input1_scale, input2_scale));
  const double real_input1_multiplier = static_cast<double>(input1_scale / twice_max_input_scale);
  const double real_input2_multiplier = static_cast<double>(input2_scale / twice_max_input_scale);
  const double real_output_multiplier =
    twice_max_input_scale / ((1 << params.left_shift) * static_cast<double>(output_scale));

  kernels::quantizeMultiplierSmallerThanOneExp(real_input1_multiplier, &params.input1_multiplier,
                                               &params.input1_shift);
  kernels::quantizeMultiplierSmallerThanOneExp(real_input2_multiplier, &params.input2_multiplier,
                                               &params.input2_shift);
  kernels::quantizeMultiplierSmallerThanOneExp(real_output_multiplier, &params.output_multiplier,
                                               &params.output_shift);

  kernels::calculateActivationRangeQuantized(luci_actfunc(options->fused_activation_function()),
                                             output, &params.quantized_activation_min,
                                             &params.quantized_activation_max);
  uint8_t *inplace_data_ptr = nullptr;

  circle::Tensor *input_inplace_tensor = nullptr;

  auto *input1_data = runtime_graph->getDataByTensor(input1);
  if (input1_data != nullptr)
  {
    inplace_data_ptr = const_cast<uint8_t *>(input1_data);
    input_inplace_tensor = const_cast<circle::Tensor *>(input1);
  }
  if (input1_data == nullptr)
    input1_data = runtime_graph->getConstDataByTensor(input1);

  assert(input1_data);

  auto *input2_data = runtime_graph->getDataByTensor(input2);
  if (inplace_data_ptr == nullptr)
  {
    assert(input2_data != nullptr);
    inplace_data_ptr = const_cast<uint8_t *>(input2_data);
    input_inplace_tensor = const_cast<circle::Tensor *>(input2);
  }
  if (input2_data == nullptr)
    input2_data = runtime_graph->getConstDataByTensor(input2);
  assert(input2_data);

  auto output_data = runtime_graph->getDataByTensor(output);
  if (is_inplace)
  {
    assert(output_data == nullptr);
    output_data = inplace_data_ptr;
    assert(output_data);
  }

  if (type == DataType::S8)
  {
    luci_interpreter_pal::Add(
      params, input_shape1.flatSize(), reinterpret_cast<const int8_t *>(input1_data),
      reinterpret_cast<const int8_t *>(input2_data), reinterpret_cast<int8_t *>(output_data));
  }
  else
  {
    luci_interpreter_pal::Add(
      params, input_shape1.flatSize(), reinterpret_cast<const int16_t *>(input1_data),
      reinterpret_cast<const int16_t *>(input2_data), reinterpret_cast<int16_t *>(output_data));
  }

  if (is_inplace)
  {
    runtime_graph->makeInplaceOperation(input_inplace_tensor, output);
    if (input_inplace_tensor == input1)
    {
      runtime_graph->makeInplaceOperation(input2, nullptr);
    }
    else
    {
      runtime_graph->makeInplaceOperation(input1, nullptr);
    }
  }
}
#endif // DIS_QUANT

} // namespace

void configure_kernel_CircleAdd(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.input2()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.input2()));

#ifndef DIS_QUANT
  if (Tensor::element_type(kernel.input1()) == DataType::S16)
  {
    LUCI_INTERPRETER_CHECK(Tensor::zero_points(kernel.input1()).size() == 1 &&
                           Tensor::zero_points(kernel.input2()).size() == 1);
    LUCI_INTERPRETER_CHECK(Tensor::zero_point(kernel.input1()) == 0 &&
                           Tensor::zero_point(kernel.input2()) == 0 &&
                           Tensor::zero_point(kernel.output()) == 0);
  }
#endif // DIS_QUANT
}

void execute_kernel_CircleAdd(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::TISOKernel kernel(cur_op, runtime_graph);

  const auto *options = cur_op->builtin_options_as_AddOptions();

  luci_interpreter::RuntimeShape input_shape1 =
    kernels::getTensorRuntimeShape(kernel.input1(), runtime_graph);
  luci_interpreter::RuntimeShape input_shape2 =
    kernels::getTensorRuntimeShape(kernel.input2(), runtime_graph);

  bool is_inplace = runtime_graph->is_inplace_op(cur_op);

  // TODO remove code duplication, introduce func
#ifndef DIS_DYN_SHAPES
  luci_interpreter::RuntimeShape output_shape =
    kernels::getTensorRuntimeShape(kernel.output(), runtime_graph);
  // Dynamic shape case
  if (not is_inplace and not(input_shape1 == output_shape) and not(input_shape2 == output_shape))
  {
    int32_t num_dims;

    if (input_shape1.flatSize() > input_shape2.flatSize())
    {
      output_shape = input_shape1;
      num_dims = input_shape1.dimensionsCount();
    }
    else
    {
      output_shape = input_shape2;
      num_dims = input_shape2.dimensionsCount();
    }

    luci_interpreter::RuntimeShape dynamic_shape(num_dims);
    int32_t data_size = 1;
    for (int i = 0; i < num_dims; ++i)
    {
      dynamic_shape.setDim(i, output_shape.dims(i));
      data_size *= output_shape.dims(i);
    }
    data_size *= size(Tensor::element_type(kernel.output()));

    runtime_graph->addDynamicShapeTensor(kernel.output(), std::move(dynamic_shape));

    if (data_size == 0)
    {
      runtime_graph->resetTensorData(nullptr, kernel.output());
      return;
    }
    auto new_output_data = new uint8_t[data_size];
    runtime_graph->resetTensorData(new_output_data, kernel.output());
  }
#endif // DIS_DYN_SHAPES

  const auto type = Tensor::element_type(kernel.input1());
  switch (type)
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      auto tiso_func = luci_interpreter_pal::Add<float>;
      auto broadcast_tiso_func = luci_interpreter_pal::BroadcastAdd4DSlow<float>;
      if (is_inplace)
      {
        kernels::evalTISOInplaceKernel<float>(tiso_func, broadcast_tiso_func, &kernel, options,
                                              std::move(input_shape1), std::move(input_shape2),
                                              std::move(output_shape));
      }
      else
      {
        kernels::TISOData kernel_data = kernel.readData();
        kernels::evalTISOKernel<float>(tiso_func, broadcast_tiso_func, &kernel, &kernel_data,
                                       options, std::move(input_shape1), std::move(input_shape2),
                                       std::move(output_shape));
      }
    }
    break;
#endif // DIS_FLOAT
    case DataType::S64:
    {
      auto tiso_func = luci_interpreter_pal::Add<int64_t>;
      auto broadcast_tiso_func = luci_interpreter_pal::BroadcastAdd4DSlow<int64_t>;
      if (is_inplace)
      {
        kernels::evalTISOInplaceKernel<int64_t>(tiso_func, broadcast_tiso_func, &kernel, options,
                                                std::move(input_shape1), std::move(input_shape2),
                                                std::move(output_shape));
      }
      else
      {
        kernels::TISOData kernel_data = kernel.readData();
        kernels::evalTISOKernel<int64_t>(tiso_func, broadcast_tiso_func, &kernel, &kernel_data,
                                         options, std::move(input_shape1), std::move(input_shape2),
                                         std::move(output_shape));
      }
    }
    break;
    case DataType::S32:
    {
      auto tiso_func = luci_interpreter_pal::Add<int32_t>;
      auto broadcast_tiso_func = luci_interpreter_pal::BroadcastAdd4DSlow<int32_t>;
      if (is_inplace)
      {
        kernels::evalTISOInplaceKernel<int32_t>(tiso_func, broadcast_tiso_func, &kernel, options,
                                                std::move(input_shape1), std::move(input_shape2),
                                                std::move(output_shape));
      }
      else
      {
        kernels::TISOData kernel_data = kernel.readData();
        kernels::evalTISOKernel<int32_t>(tiso_func, broadcast_tiso_func, &kernel, &kernel_data,
                                         options, std::move(input_shape1), std::move(input_shape2),
                                         std::move(output_shape));
      }
    }
    break;
#ifndef DIS_QUANT
    case DataType::S8:
    case DataType::S16:
    {
      evalQuantized(kernel.input1(), kernel.input2(), kernel.output(), options, runtime_graph,
                    type, is_inplace);
    }
    break;
#endif
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
