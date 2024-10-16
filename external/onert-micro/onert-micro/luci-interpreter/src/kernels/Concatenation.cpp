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

#include "PALConcatenation.h"

namespace luci_interpreter
{

namespace
{

template <typename T>
void evalGeneric(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  const auto output_index = cur_op->outputs()->operator[](0);

  assert(output_index != -1);

  auto output = runtime_graph->getCircleTensorByIndex(output_index);

  const auto *options = cur_op->builtin_options_as_ConcatenationOptions();

  int axis = options->axis();
  if (axis < 0)
    axis += Tensor::num_dims(output);

  const auto input_sizes = cur_op->inputs()->size();

  std::vector<const T *> all_input_data;
  std::vector<luci_interpreter::RuntimeShape> all_shape;
  std::vector<luci_interpreter::RuntimeShape *> all_shape_ptr;

  for (int32_t i = 0; i < input_sizes; ++i)
  {
    auto input_index = cur_op->inputs()->operator[](i);
    const auto *tensor = runtime_graph->getCircleTensorByIndex(input_index);

    const auto *tensor_data = runtime_graph->getDataByTensor(tensor);
    if (tensor_data == nullptr)
      tensor_data = runtime_graph->getConstDataByTensor(tensor);

    auto *data = reinterpret_cast<const T *>(tensor_data);

    auto runtime_shape = kernels::getTensorRuntimeShape(tensor, runtime_graph);

    all_input_data.push_back(data);
    all_shape.push_back(runtime_shape);
  }

  for (luci_interpreter::RuntimeShape &shape : all_shape)
  {
    all_shape_ptr.push_back(&shape);
  }

  auto *output_data = reinterpret_cast<T *>(runtime_graph->getDataByTensor(output));

  luci_interpreter_pal::ConcatenationParams params{};
  params.axis = axis;
  params.inputs_count = all_shape.size();
  luci_interpreter_pal::Concatenation(params, all_shape_ptr.data(), all_input_data.data(),
                                      kernels::getTensorShape(output), output_data);
}

} // namespace

void configure_kernel_CircleConcatenation(const circle::Operator *cur_op,
                                          BaseRuntimeGraph *runtime_graph)
{
  const int num_inputs = cur_op->inputs()->size();
  LUCI_INTERPRETER_CHECK(num_inputs > 0);

  auto input_index = cur_op->inputs()->operator[](0);
  auto output_index = cur_op->outputs()->operator[](0);

  assert(input_index != -1);
  assert(output_index != -1);

  const auto *t0 = runtime_graph->getCircleTensorByIndex(input_index);
  const auto *output = runtime_graph->getCircleTensorByIndex(output_index);

  const auto *params = cur_op->builtin_options_as_ConcatenationOptions();

  // TODO: Support concat with fused activation function
  LUCI_INTERPRETER_CHECK(luci_actfunc(params->fused_activation_function()) == FusedActFunc::NONE);

  int axis = params->axis();
  if (axis < 0)
    axis += Tensor::num_dims(t0);
  LUCI_INTERPRETER_CHECK(axis >= 0 && axis < Tensor::num_dims(t0));

  for (int i = 1; i < num_inputs; ++i)
  {
    input_index = cur_op->inputs()->operator[](i);
    const auto *tensor = runtime_graph->getCircleTensorByIndex(input_index);
    LUCI_INTERPRETER_CHECK(Tensor::element_type(tensor) == Tensor::element_type(t0));
    LUCI_INTERPRETER_CHECK(Tensor::num_dims(tensor) == Tensor::num_dims(t0));
  }

#ifndef DIS_QUANT
  // If input tensors are INT8 type then quantization parameters of all input tensors and the output
  // should be the same
  for (int i = 1; i < num_inputs; ++i)
  {
    input_index = cur_op->inputs()->operator[](i);
    const auto *tensor = runtime_graph->getCircleTensorByIndex(input_index);
    if (Tensor::element_type(tensor) == DataType::S8)
    {
      LUCI_INTERPRETER_CHECK(Tensor::quantized_dimension(tensor) ==
                             Tensor::quantized_dimension(output));

      LUCI_INTERPRETER_CHECK(Tensor::zero_points(tensor).size() == Tensor::scales(tensor).size());
      LUCI_INTERPRETER_CHECK(Tensor::zero_points(tensor) == Tensor::zero_points(output));
      LUCI_INTERPRETER_CHECK(Tensor::scales(tensor) == Tensor::scales(output));
    }
  }
#endif // DIS_QUANT
}

void execute_kernel_CircleConcatenation(const circle::Operator *cur_op,
                                        BaseRuntimeGraph *runtime_graph)
{
  int num_inputs = cur_op->inputs()->size();
  LUCI_INTERPRETER_CHECK(num_inputs > 0);

  const auto input_index = cur_op->inputs()->operator[](0);
  assert(input_index != -1);
  const auto *t0 = runtime_graph->getCircleTensorByIndex(input_index);

  switch (Tensor::element_type(t0))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      evalGeneric<float>(cur_op, runtime_graph);
      break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case DataType::S8:
      evalGeneric<int8_t>(cur_op, runtime_graph);
      break;
#endif // DIS_QUANT
    case DataType::S32:
      evalGeneric<int32_t>(cur_op, runtime_graph);
      break;
    case DataType::S64:
      evalGeneric<int64_t>(cur_op, runtime_graph);
      break;
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
