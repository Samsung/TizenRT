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

#include "Builders.h"
#include "kernels/Utils.h"
#include "MISOKernel.h"

#include <cassert>

namespace luci_interpreter
{

namespace
{
const int max_dim = 5;

struct SliceParams
{
  int8_t begin_count;
  int32_t begin[5];
  int8_t size_count;
  int32_t size[5];
};

template <typename T>
inline void slice(const luci_interpreter::SliceParams &op_params,
                  const luci_interpreter::RuntimeShape &input_shape, const T *input_data,
                  const luci_interpreter::RuntimeShape &output_shape, T *output_data)
{
  const luci_interpreter::RuntimeShape ext_shape =
    luci_interpreter::RuntimeShape::extendedShape(5, input_shape);
  const int begin_count = op_params.begin_count;
  const int size_count = op_params.size_count;
  // We front-pad the begin and size vectors.
  int start[5];
  int stop[5];
  for (int i = 0; i < 5; ++i)
  {
    int padded_i = 5 - i;
    start[i] = begin_count < padded_i ? 0 : op_params.begin[begin_count - padded_i];
    stop[i] = (size_count < padded_i || op_params.size[size_count - padded_i] == -1)
                ? ext_shape.dims(i)
                : start[i] + op_params.size[size_count - padded_i];
  }

  for (int i0 = start[0]; i0 < stop[0]; ++i0)
  {
    for (int i1 = start[1]; i1 < stop[1]; ++i1)
    {
      for (int i2 = start[2]; i2 < stop[2]; ++i2)
      {
        for (int i3 = start[3]; i3 < stop[3]; ++i3)
        {
          for (int i4 = start[4]; i4 < stop[4]; ++i4)
          {
            auto position =
              (((i0 * ext_shape.dims(1) + i1) * ext_shape.dims(2) + i2) * ext_shape.dims(3) + i3) *
                ext_shape.dims(4) +
              i4;
            *output_data++ = input_data[position];
          }
        }
      }
    }
  }
}

template <typename T>
void getBeginAndSizeVectors(int dimensions, const uint8_t *begin_data, const uint8_t *size_data,
                            int32_t *begins, int32_t *sizes)
{
  int offset = max_dim - dimensions;
  for (int idx = 0; idx < dimensions; ++idx)
  {
    begins[offset + idx] = kernels::getTensorData<T>(begin_data)[idx];
    sizes[offset + idx] = kernels::getTensorData<T>(size_data)[idx];
  }
}
} // namespace

void configure_kernel_CircleSlice(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::MISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input1()) ==
                         Tensor::element_type(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input2()) == DataType::S32 ||
                         Tensor::element_type(kernel.input2()) == DataType::S64);
  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input3()) == DataType::S32 ||
                         Tensor::element_type(kernel.input3()) == DataType::S64);
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input2()) == 1);
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input3()) == 1);
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input1()) <= max_dim);
}

void execute_kernel_CircleSlice(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::MISOKernel kernel(cur_op, runtime_graph);

  bool is_dynamic_shapes = false;

  const circle::Tensor *input = kernel.input1();
  const circle::Tensor *begin = kernel.input2();
  const circle::Tensor *size_tensor = kernel.input3();
  const circle::Tensor *output = kernel.output();

  const auto *input_data = runtime_graph->getDataByTensor(input);
  if (input_data == nullptr)
    input_data = runtime_graph->getConstDataByTensor(input);
  assert(input_data);

  const auto *begin_data = runtime_graph->getDataByTensor(begin);
  if (begin_data == nullptr)
  {
    begin_data = runtime_graph->getConstDataByTensor(begin);
    is_dynamic_shapes = true;
  }
  assert(begin_data);

  const auto *size_data = runtime_graph->getDataByTensor(size_tensor);
  if (size_data == nullptr)
  {
    size_data = runtime_graph->getConstDataByTensor(size_tensor);
    is_dynamic_shapes = true;
  }
  assert(size_data);

  auto *output_data = runtime_graph->getDataByTensor(output);
  assert(output_data);

  SliceParams op_params{};
  op_params.begin_count = max_dim;
  op_params.size_count = max_dim;
  for (int i = 0; i < max_dim; i++)
  {
    op_params.begin[i] = 0;
    op_params.size[i] = 1;
  }
  auto num_dim = Tensor::num_dims(input);

  if (Tensor::element_type(begin) == DataType::S32)
  {
    getBeginAndSizeVectors<int32_t>(num_dim, begin_data, size_data, op_params.begin,
                                    op_params.size);
  }
  else if (Tensor::element_type(begin) == DataType::S64)
  {
    getBeginAndSizeVectors<int64_t>(num_dim, begin_data, size_data, op_params.begin,
                                    op_params.size);
  }
  else
  {
    assert(false && "Unsupported type");
  }

#ifndef DIS_DYN_SHAPES
  if (is_dynamic_shapes)
  {
    int32_t data_size = 1;
    luci_interpreter::RuntimeShape dynamic_shapes(max_dim - num_dim + 1);
    int offset = max_dim - Tensor::num_dims(input);
    for (int i = 0; i <= max_dim - num_dim; ++i)
    {
      if (i + offset > 4)
        return;
      auto cur_size = op_params.size[i + offset] != -1
                        ? op_params.size[i + offset]
                        : Tensor::dim(input, i) - op_params.begin[i + offset];
      data_size *= cur_size;

      dynamic_shapes.setDim(i, cur_size);
    }
    data_size *= size(Tensor::element_type(output));

    runtime_graph->addDynamicShapeTensor(output, std::move(dynamic_shapes));

    if (data_size == 0)
    {
      runtime_graph->resetTensorData(nullptr, output);
      return;
    }

    auto new_output_data = new uint8_t[data_size];
    output_data = new_output_data;
    runtime_graph->resetTensorData(new_output_data, output);
  }
#else
  assert(is_dynamic_shapes == false);
#endif // DIS_DYN_SHAPES

  switch (Tensor::element_type(input))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      slice<float>(op_params, kernels::getTensorShape(input),
                   kernels::getTensorData<float>(input_data), kernels::getTensorShape(output),
                   kernels::getTensorData<float>(output_data));
      break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case DataType::U8:
      slice<uint8_t>(op_params, kernels::getTensorShape(input),
                     kernels::getTensorData<uint8_t>(input_data), kernels::getTensorShape(output),
                     kernels::getTensorData<uint8_t>(output_data));
      break;
    case DataType::S8:
      slice<int8_t>(op_params, kernels::getTensorShape(input),
                    kernels::getTensorData<int8_t>(input_data), kernels::getTensorShape(output),
                    kernels::getTensorData<int8_t>(output_data));
      break;
    case DataType::S16:
      slice<int16_t>(op_params, kernels::getTensorShape(input),
                     kernels::getTensorData<int16_t>(input_data), kernels::getTensorShape(output),
                     kernels::getTensorData<int16_t>(output_data));
      break;
#endif // DIS_QUANT
    default:
      assert(false && "Unsupported input type.");
  }
}

} // namespace luci_interpreter
