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
#include "SISOKernel.h"

#include "PALSoftmax.h"

namespace luci_interpreter
{

namespace
{

#ifndef DIS_FLOAT
void evalFloat(const circle::Tensor *input, const circle::Tensor *output,
               const circle::SoftmaxOptions *options, BaseRuntimeGraph *runtime_graph)
{
  const auto *input_data = runtime_graph->getDataByTensor(input);
  auto *output_data = runtime_graph->getDataByTensor(output);

  const float beta = options->beta();

  const auto trailing_dim = Tensor::num_dims(input) - 1;

  int flat_size = 1;
  for (int i = 0; i < Tensor::num_dims(input); ++i)
  {
    flat_size *= (i == trailing_dim) ? 1 : Tensor::dim(input, i);
  }

  luci_interpreter_pal::SoftmaxParams params;
  params.beta = beta;
  params.num_rows = flat_size;
  params.row_size = std::min(Tensor::dim(input, trailing_dim), Tensor::dim(output, trailing_dim));

  luci_interpreter_pal::Softmax(params, kernels::getTensorData<float>(input_data),
                                kernels::getTensorData<float>(output_data));
}
#endif // DIS_FLOAT

#ifndef DIS_QUANT
void preprocessSoftmaxScaling(double beta, double input_scale, int input_integer_bits,
                              int32_t *quantized_multiplier, int *left_shift)
{
  const double max_real_multiplier = (1LL << 31) - 1.0;
  const double input_beta_real_multiplier =
    std::min<double>(beta * input_scale * (1 << (31 - input_integer_bits)), max_real_multiplier);

  kernels::quantizeMultiplier(input_beta_real_multiplier, quantized_multiplier, left_shift);
}

void evalQuantize(const circle::Tensor *input, const circle::Tensor *output,
                  const circle::SoftmaxOptions *options, BaseRuntimeGraph *runtime_graph)
{
  static const int kScaledDiffIntegerBits = 5;

  const float beta = options->beta();

  const auto trailing_dim = Tensor::num_dims(input) - 1;

  int flat_size = 1;
  for (int i = 0; i < Tensor::num_dims(input); ++i)
  {
    flat_size *= (i == trailing_dim) ? 1 : Tensor::dim(input, i);
  }

  luci_interpreter_pal::SoftmaxParams params;
  params.beta = beta;
  params.num_rows = flat_size;
  params.row_size = std::min(Tensor::dim(input, trailing_dim), Tensor::dim(output, trailing_dim));
  params.input_scale = Tensor::scale(input);
  params.output_scale = Tensor::scale(output);
  params.input_zp = Tensor::zero_point(input);
  params.output_zp = Tensor::zero_point(output);

  if (Tensor::element_type(input) == DataType::S16)
  {
    int left_shift = 0;
    double input_scale_beta_rescale =
      static_cast<double>(Tensor::scale(input)) * static_cast<double>(beta) /
      (10.0 / 65535.0); // scale the input_diff such that [-65535, 0]
    // correspond to [-10.0, 0.0]
    kernels::quantizeMultiplier(input_scale_beta_rescale, &params.input_multiplier, &left_shift);
    params.input_left_shift = left_shift;
    luci_interpreter_pal::Softmax(
      params, kernels::getTensorData<int16_t>(runtime_graph->getDataByTensor(input)),
      kernels::getTensorData<int16_t>(runtime_graph->getDataByTensor(output)));
  }
  else
  {
    int left_shift = 0;
    preprocessSoftmaxScaling(static_cast<double>(params.beta),
                             static_cast<double>(Tensor::scale(input)), kScaledDiffIntegerBits,
                             &params.input_multiplier, &left_shift);
    params.input_left_shift = left_shift;
    params.diff_min =
      -1.0 * kernels::calculateInputRadius(kScaledDiffIntegerBits, params.input_left_shift, 31);
    if (Tensor::element_type(output) == DataType::S8)
      luci_interpreter_pal::Softmax(
        params, kernels::getTensorData<int8_t>(runtime_graph->getDataByTensor(input)),
        kernels::getTensorData<int8_t>(runtime_graph->getDataByTensor(output)));
    else if (Tensor::element_type(output) == DataType::S16)
      luci_interpreter_pal::Softmax(
        params, kernels::getTensorData<int8_t>(runtime_graph->getDataByTensor(input)),
        kernels::getTensorData<int16_t>(runtime_graph->getDataByTensor(output)));
  }
}
#endif // DIS_QUANT

} // namespace

void configure_kernel_CircleSoftmax(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input()) ==
                         Tensor::element_type(kernel.output()));
  LUCI_INTERPRETER_CHECK(Tensor::num_dims(kernel.input()) >= 1);

#ifndef DIS_QUANT
  if (Tensor::element_type(kernel.input()) == DataType::U8 ||
      Tensor::element_type(kernel.input()) == DataType::S8)
  {
    LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input()) == DataType::S8 ||
                           Tensor::zero_point(kernel.output()) == 0);
    LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input()) == DataType::U8 ||
                           Tensor::zero_point(kernel.output()) ==
                             std::numeric_limits<int8_t>::min());
  }
#endif
}

void execute_kernel_CircleSoftmax(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  const auto *options = cur_op->builtin_options_as_SoftmaxOptions();
  const auto input_type = Tensor::element_type(kernel.input());
  switch (input_type)
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
      evalFloat(kernel.input(), kernel.output(), options, runtime_graph);
      break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case DataType::S8:
    case DataType::S16:
      evalQuantize(kernel.input(), kernel.output(), options, runtime_graph);
      break;
#endif
    default:
      assert(false && "Unsupported type.");
  }
}

} // namespace luci_interpreter
