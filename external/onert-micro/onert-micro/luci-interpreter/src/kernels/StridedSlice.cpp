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

#include "Builders.h"
#include "kernels/Utils.h"
#include "MISOKernel.h"

#include "PALStridedSlice.h"

namespace luci_interpreter
{
namespace
{

luci_interpreter_pal::StridedSliceParams
buildStridedSliceParams(int32_t dims, const int32_t *begin, const int32_t *end,
                        const int32_t *strides, const circle::StridedSliceOptions *options)
{
  luci_interpreter_pal::StridedSliceParams op_params;
  op_params.start_indices_count = dims;
  op_params.stop_indices_count = dims;
  op_params.strides_count = dims;

  for (int i = 0; i < dims; ++i)
  {
    op_params.start_indices[i] = begin[i];
    op_params.stop_indices[i] = end[i];
    op_params.strides[i] = strides[i];
  }

  op_params.begin_mask = options->begin_mask();
  op_params.ellipsis_mask = 0;
  op_params.end_mask = options->end_mask();
  op_params.new_axis_mask = 0;
  op_params.shrink_axis_mask = options->shrink_axis_mask();
  return op_params;
}

} // namespace

void configure_kernel_CircleStridedSlice(const circle::Operator *cur_op,
                                         BaseRuntimeGraph *runtime_graph)
{
  kernels::MISOKernel miso_kernel(cur_op, runtime_graph);

  const circle::Tensor *input = miso_kernel.input1();
  const circle::Tensor *begin = miso_kernel.input2();
  const circle::Tensor *end = miso_kernel.input3();
  const circle::Tensor *strides = miso_kernel.input4();

  LUCI_INTERPRETER_CHECK(strides != nullptr);

  const circle::Tensor *output = miso_kernel.output();

  LUCI_INTERPRETER_CHECK(Tensor::element_type(begin) == DataType::S32);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(end) == DataType::S32);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(strides) == DataType::S32);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(input) == Tensor::element_type(output));
}

void execute_kernel_CircleStridedSlice(const circle::Operator *cur_op,
                                       BaseRuntimeGraph *runtime_graph)
{
  kernels::MISOKernel miso_kernel(cur_op, runtime_graph);

  const circle::Tensor *input = miso_kernel.input1();
  const circle::Tensor *begin = miso_kernel.input2();
  const circle::Tensor *end = miso_kernel.input3();
  const circle::Tensor *strides = miso_kernel.input4();
  const circle::Tensor *output = miso_kernel.output();

  const int32_t dims = Tensor::num_dims(input);

  const uint8_t *input_data = runtime_graph->getDataByTensor(input);
  const int32_t *begin_data =
    kernels::getTensorData<int32_t>(runtime_graph->getConstDataByTensor(begin));
  const int32_t *end_data =
    kernels::getTensorData<int32_t>(runtime_graph->getConstDataByTensor(end));
  const int32_t *strides_data =
    kernels::getTensorData<int32_t>(runtime_graph->getConstDataByTensor(strides));
  uint8_t *output_data = runtime_graph->getDataByTensor(output);

  LUCI_INTERPRETER_CHECK(input_data != nullptr);
  LUCI_INTERPRETER_CHECK(begin_data != nullptr);
  LUCI_INTERPRETER_CHECK(end_data != nullptr);
  LUCI_INTERPRETER_CHECK(strides_data != nullptr);
  LUCI_INTERPRETER_CHECK(output_data != nullptr);

  const auto *options = cur_op->builtin_options_as_StridedSliceOptions();

  auto op_params = buildStridedSliceParams(dims, begin_data, end_data, strides_data, options);

  switch (Tensor::element_type(input))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      luci_interpreter_pal::StridedSlice(op_params, kernels::getTensorShape(input),
                                         kernels::getTensorData<float>(input_data),
                                         kernels::getTensorData<float>(output_data));
      break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case DataType::U8:
      luci_interpreter_pal::StridedSlice(op_params, kernels::getTensorShape(input), input_data,
                                         output_data);
      break;
    case DataType::S8:
      luci_interpreter_pal::StridedSlice(op_params, kernels::getTensorShape(input), input_data,
                                         output_data);
      break;
#endif
    case DataType::S32:
      luci_interpreter_pal::StridedSlice(op_params, kernels::getTensorShape(input),
                                         kernels::getTensorData<int32_t>(input_data),
                                         kernels::getTensorData<int32_t>(output_data));
      break;
    default:
      assert(false && "Unsupported type");
  }
}

} // namespace luci_interpreter
