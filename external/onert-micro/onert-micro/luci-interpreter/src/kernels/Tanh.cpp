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

#include "PALTanh.h"

namespace luci_interpreter
{

#ifndef DIS_QUANT

namespace
{
void calculateArithmeticData(const circle::Tensor *input, const circle::Tensor *output,
                             int32_t &input_zero_point, int32_t &input_range_radius,
                             int32_t &input_multiplier, int &input_left_shift)
{
  const auto input_dtype = Tensor::element_type(input);
  switch (input_dtype)
  {
    case DataType::S8:
    {
      static constexpr int input_integer_bits = 4;
      const double input_real_multiplier = static_cast<double>(Tensor::scale(input)) *
                                           static_cast<double>(1 << (31 - input_integer_bits));

      const double q = std::frexp(input_real_multiplier, &input_left_shift);
      input_multiplier = static_cast<int32_t>(std::round(q * (1ll << 31)));
      input_range_radius = kernels::calculateInputRadius(input_integer_bits, input_left_shift, 31);
    }
    break;
    case DataType::S16:
    {
      static constexpr int input_integer_bits = 3;
      static constexpr int output_fractional_bits = 15;

      // These operators are implemented in fixed-point arithmetic,
      // which intrinsically wants symmetric ranges (zero_point==0)
      // and power-of-two scales (power-of-two is abbreviated below as POT).
      // While more general support would be possible by means of rescaling,
      // that would add some overhead and some loss of accuracy and wouldn't
      // be used at the moment as current quantized LSTM applications are
      // happy with symmetric, power-of-two-scales quantization. So we just
      // implement that narrow case only for now.

      int input_scale_log2_rounded;
      bool param_scale_pot = kernels::checkedLog2(Tensor::scale(input), &input_scale_log2_rounded);

      input_left_shift = (15 - input_integer_bits) + input_scale_log2_rounded;
      param_scale_pot &= (input_left_shift == 0 || input_left_shift == 1);

      if (param_scale_pot)
      {
        input_multiplier = 0;
      }
      else
      {
        // Calculate multiplier to change input scale to 1/(3*4096)
        // as required by the table lookup.
        // The number 3.0 in the multiplier comes from here,
        // because the interval is [-10.7, 10.7] instead of [-8, 8].
        // So, in this scaling +/-2^17 represents +/-10.7.

        double multiplier = static_cast<double>(Tensor::scale(input)) * 4096.0 * 3.0;
        input_left_shift = 0;

        while (multiplier <= 32767.0 / 2.0 && input_left_shift <= 30)
        {
          input_left_shift++;
          multiplier = multiplier * 2.0;
        }

        input_multiplier = static_cast<int32_t>(multiplier);
      }

      int output_scale_log2_rounded;
      kernels::checkedLog2(Tensor::scale(output), &output_scale_log2_rounded);
      assert(output_scale_log2_rounded == -output_fractional_bits);
    }
    break;
    default:
      assert(false && "Unsupported type");
  }
}

} // namespace

void evalInteger(const circle::Tensor *input, const circle::Tensor *output,
                 BaseRuntimeGraph *runtime_graph)
{
  int32_t input_zero_point = 0;
  int32_t input_range_radius = 0;
  int32_t input_multiplier = 0;
  int input_left_shift = 0;

  calculateArithmeticData(input, output, input_zero_point, input_range_radius, input_multiplier,
                          input_left_shift);

  const auto *input_data = runtime_graph->getDataByTensor(input);
  assert(input_data);

  auto *output_data = runtime_graph->getDataByTensor(output);
  assert(output_data);

  const int flat_size = kernels::getTensorRuntimeShape(input, runtime_graph).flatSize();

  const auto input_dtype = Tensor::element_type(input);
  switch (input_dtype)
  {
    case DataType::S8:
      luci_interpreter_pal::Tanh(
        Tensor::zero_point(input), Tensor::scale(input), Tensor::zero_point(output), Tensor::scale(output),
        flat_size, kernels::getTensorData<int8_t>(input_data), kernels::getTensorData<int8_t>(output_data));
      break;
    case DataType::S16:
      luci_interpreter_pal::Tanh(input_multiplier, input_left_shift, flat_size,
                                 kernels::getTensorData<int16_t>(input_data),
                                 kernels::getTensorData<int16_t>(output_data));
      break;
    default:
      assert(false && "Not support yet");
  }
}
#endif // DIS_QUANT

void configure_kernel_CircleTanh(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  LUCI_INTERPRETER_CHECK(Tensor::element_type(kernel.input()) ==
                         Tensor::element_type(kernel.output()));
}

void execute_kernel_CircleTanh(const circle::Operator *cur_op, BaseRuntimeGraph *runtime_graph)
{
  kernels::SISOKernel kernel(cur_op, runtime_graph);

  const auto *input_data = runtime_graph->getDataByTensor(kernel.input());
  assert(input_data);

  auto *output_data = runtime_graph->getDataByTensor(kernel.output());

  bool is_inplace = runtime_graph->is_inplace_op(cur_op);

  switch (Tensor::element_type(kernel.input()))
  {
#ifndef DIS_FLOAT
    case DataType::FLOAT32:
    {
      const float *input_data_float = kernels::getTensorData<float>(input_data);
      float *output_data_float = kernels::getTensorData<float>(output_data);
      if (is_inplace)
      {
        output_data_float = const_cast<float *>(input_data_float);
      }

      assert(output_data_float);

      const int flat_size =
        kernels::getTensorRuntimeShape(kernel.input(), runtime_graph).flatSize();

      luci_interpreter_pal::Tanh(flat_size, input_data_float, output_data_float);
      break;
    }
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case DataType::S16:
      // TODO: enable it
    case DataType::S8:
      evalInteger(kernel.input(), kernel.output(), runtime_graph);
      break;
#endif // DIS_QUANT
    default:
      assert(false && "Unsupported type");
  }

  if (is_inplace)
    runtime_graph->makeInplaceOperation(kernel.input(), kernel.output());
}
} // namespace luci_interpreter
