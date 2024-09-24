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

#include "kernels/Utils.h"

#include <cassert>
#include <cmath>
#include <limits>

namespace luci_interpreter
{
namespace kernels
{

luci_interpreter::RuntimeShape getTensorRuntimeShape(const circle::Tensor *circle_tensor,
                                                     BaseRuntimeGraph *runtime_graph)
{
  luci_interpreter::RuntimeShape input_shape = getTensorShape(circle_tensor);

#ifndef DIS_DYN_SHAPES
  auto *dynamic_shape_vector = runtime_graph->getDynamicShapeTensor(circle_tensor);
  if (dynamic_shape_vector != nullptr)
  {
    input_shape.resize(dynamic_shape_vector->dimensionsCount());

    for (int n = 0; n < dynamic_shape_vector->dimensionsCount(); ++n)
    {
      input_shape.setDim(n, dynamic_shape_vector->dims(n));
    }
  }
#endif // DIS_DYN_SHAPES
  return input_shape;
}

template <typename T>
void calculateActivationRange(Activation activation, T *activation_min, T *activation_max)
{
  switch (activation)
  {
    case Activation::NONE:
      *activation_min = std::numeric_limits<T>::lowest();
      *activation_max = std::numeric_limits<T>::max();
      break;
    case Activation::RELU:
      *activation_min = 0;
      *activation_max = std::numeric_limits<T>::max();
      break;
    case Activation::RELU_N1_TO_1:
      *activation_min = -1;
      *activation_max = 1;
      break;
    case Activation::RELU6:
      *activation_min = 0;
      *activation_max = 6;
      break;
    default:
      assert(false && "Unsupported activation.");
  }
}

void matrixScalarMultiplyAccumulate(const int8_t *matrix, int32_t scalar, int32_t n_row,
                                    int32_t n_col, int32_t *output)
{
  for (int i = 0; i < n_row; ++i)
  {
    int32_t row_sum = 0;
    for (int j = 0; j < n_col; ++j)
    {
      row_sum += *matrix++;
    }
    output[i] += row_sum * scalar;
  }
}

bool areShapesEqual(const luci_interpreter::RuntimeShape &input_shape1,
                    const luci_interpreter::RuntimeShape &input_shape2)
{
  if (input_shape1.dimensionsCount() == input_shape2.dimensionsCount())
  {
    int N = input_shape1.dimensionsCount();
    for (int i = 0; i < N; ++i)
    {
      if (input_shape1.dims(i) != input_shape2.dims(i))
        return false;
    }
    return true;
  }
  return false;
}

template void calculateActivationRange(Activation activation, float *activation_min,
                                       float *activation_max);
template void calculateActivationRange(Activation activation, int32_t *activation_min,
                                       int32_t *activation_max);
template void calculateActivationRange(Activation activation, int64_t *activation_min,
                                       int64_t *activation_max);

#ifndef DIS_QUANT
bool checkedLog2(const float x, int *log2_result)
{
  const float x_log2 = std::log(x) * (1.0f / std::log(2.0f));
  const float x_log2_rounded = std::round(x_log2);
  const float x_log2_fracpart = x_log2 - x_log2_rounded;

  *log2_result = static_cast<int>(x_log2_rounded);
  return std::abs(x_log2_fracpart) < 1e-3f;
}

int calculateInputRadius(int input_integer_bits, int input_left_shift, int total_signed_bits)
{
  const double max_input_rescaled = 1.0 * ((1 << input_integer_bits) - 1) *
                                    (1LL << (total_signed_bits - input_integer_bits)) /
                                    (1LL << input_left_shift);
  // Tighten bound using floor.  Suppose that we could use the exact value.
  // After scaling the difference, the result would be at the maximum.  Thus we
  // must ensure that our value has lower magnitude.
  return static_cast<int>(std::floor(max_input_rescaled));
}

static void calculateActivationRangeQuantizedImpl(Activation activation, int32_t qmin, int32_t qmax,
                                                  int32_t zero_point, float scale,
                                                  int32_t *activation_min, int32_t *activation_max)
{
  auto quantize = [scale, zero_point](float x) {
    return zero_point + static_cast<int32_t>(std::round(x / scale));
  };

  switch (activation)
  {
    case Activation::NONE:
    case Activation::TANH:
      *activation_min = qmin;
      *activation_max = qmax;
      break;
    case Activation::RELU:
      *activation_min = std::max(qmin, quantize(0.0f));
      *activation_max = qmax;
      break;
    case Activation::RELU_N1_TO_1:
      *activation_min = std::max(qmin, quantize(-1.0f));
      *activation_max = std::min(qmax, quantize(1.0f));
      break;
    case Activation::RELU6:
      *activation_min = std::max(qmin, quantize(0.0f));
      *activation_max = std::min(qmax, quantize(6.0f));
      break;
    default:
      assert(false && "Unsupported activation.");
  }
}

static void calculateActivationRangeQuantizedImpl(Activation activation, int32_t qmin, int32_t qmax,
                                                  const circle::Tensor *output,
                                                  int32_t *activation_min, int32_t *activation_max)
{
  const float scale = Tensor::scale(output);
  const int32_t zero_point = Tensor::zero_point(output);

  calculateActivationRangeQuantizedImpl(activation, qmin, qmax, zero_point, zero_point,
                                        activation_min, activation_max);
}

void calculateActivationRangeQuantized(Activation activation, int32_t output_zero_point,
                                       float output_scale, DataType data_type,
                                       int32_t *activation_min, int32_t *activation_max)
{
  int32_t qmin{};
  int32_t qmax{};
  switch (data_type)
  {
    case DataType::U8:
      qmin = std::numeric_limits<uint8_t>::min();
      qmax = std::numeric_limits<uint8_t>::max();
      break;
    case DataType::S8:
      qmin = std::numeric_limits<int8_t>::min();
      qmax = std::numeric_limits<int8_t>::max();
      break;
    case DataType::S16:
      // For now, assume that signed int16 type implies signed symmetric quantization.
      assert(output_zero_point == 0);
      qmin = std::numeric_limits<int16_t>::min();
      qmax = std::numeric_limits<int16_t>::max();
      break;
    default:
      assert(false && "Unsupported type.");
  }

  calculateActivationRangeQuantizedImpl(activation, qmin, qmax, output_zero_point, output_scale,
                                        activation_min, activation_max);
}

void calculateActivationRangeQuantized(Activation activation, const circle::Tensor *output,
                                       int32_t *activation_min, int32_t *activation_max)
{
  assert(Tensor::zero_points(output).size() == 1);
  const float scale = Tensor::scale(output);
  const int32_t zero_point = Tensor::zero_point(output);
  calculateActivationRangeQuantized(activation, zero_point, scale, Tensor::element_type(output),
                                    activation_min, activation_max);
}

void quantizeMultiplier(double double_multiplier, int32_t *quantized_multiplier, int *shift)
{
  if (double_multiplier == 0.0)
  {
    *quantized_multiplier = 0;
    *shift = 0;
    return;
  }

  const double q = std::frexp(double_multiplier, shift);
  auto q_fixed = static_cast<int64_t>(std::round(q * (int64_t(1) << 31)));

  if (q_fixed == (int64_t(1) << 31))
  {
    q_fixed /= 2;
    ++*shift;
  }
  assert(q_fixed <= std::numeric_limits<int32_t>::max());
  // A shift amount smaller than -31 would cause all bits to be shifted out
  // and thus all results would be zero. We implement that instead with
  // q_fixed==0, so as to avoid hitting issues with right-shift
  // operations with shift amounts greater than 31. Note that this happens
  // roughly when abs(double_multiplier) < 2^-31 and the present handling means
  // that we're effectively flushing tiny double_multiplier's to zero.
  // We could conceivably handle values in the range (roughly) [32, 63]
  // as 'denormals' i.e. (shift==0, q_fixed < 2^30). In that point of view
  // the present handling is just doing 'flush denormals to zero'. We could
  // reconsider and actually generate nonzero denormals if a need arises.
  if (*shift < -31)
  {
    *shift = 0;
    q_fixed = 0;
  }
  *quantized_multiplier = static_cast<int32_t>(q_fixed);
}

void quantizeMultiplierSmallerThanOneExp(double double_multiplier, int32_t *quantized_multiplier,
                                         int *left_shift)
{
  assert(double_multiplier < 1.0);
  assert(double_multiplier > 0.0);
  int shift;
  quantizeMultiplier(double_multiplier, quantized_multiplier, &shift);
  assert(shift <= 0);
  *left_shift = shift;
}
#endif

luci_interpreter::RuntimeShape calculateShapeForBroadcast(const circle::Tensor *input1,
                                                          const circle::Tensor *input2)
{
  const int num_input1_dims = Tensor::num_dims(input1);
  const int num_input2_dims = Tensor::num_dims(input2);
  const int num_out_dims = std::max(num_input1_dims, num_input2_dims);
  luci_interpreter::RuntimeShape output_shape(num_out_dims);

  for (int i = 0; i < num_out_dims; ++i)
  {
    const int32_t input1_dim =
      i < num_input1_dims ? Tensor::dim(input1, num_input1_dims - i - 1) : 1;
    const int32_t input2_dim =
      i < num_input2_dims ? Tensor::dim(input2, num_input2_dims - i - 1) : 1;

    bool need_broadcast = input1_dim != input2_dim;
    bool can_broadcast = input1_dim == 1 || input2_dim == 1;
    LUCI_INTERPRETER_CHECK(!need_broadcast || can_broadcast);

    output_shape.setDim(num_out_dims - i - 1, std::max(input1_dim, input2_dim));
  }

  return output_shape;
}

} // namespace kernels
} // namespace luci_interpreter
