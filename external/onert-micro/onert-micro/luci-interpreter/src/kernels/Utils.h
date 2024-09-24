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

#ifndef LUCI_INTERPRETER_KERNELS_UTILS_H
#define LUCI_INTERPRETER_KERNELS_UTILS_H

#include "luci_interpreter/core/Tensor.h"
#include "Builders.h"
#include "Params.h"
#include <cassert>
#include <cstdint>

#include <cmath>

namespace luci_interpreter
{
namespace kernels
{

using Activation = luci_interpreter::FusedActFunc;

#define LUCI_INTERPRETER_CHECK(cond)                 \
  if (!(cond))                                       \
  {                                                  \
    assert(false && "LUCI_INTERPRETER_CHECK fails"); \
  }

inline int32_t computePadding(int32_t stride, int32_t dilation_rate, int32_t in_size,
                              int32_t filter_size, int32_t out_size)
{
  const int32_t effective_filter_size = (filter_size - 1) * dilation_rate + 1;
  const int32_t padding = ((out_size - 1) * stride + effective_filter_size - in_size) / 2;
  return padding > 0 ? padding : 0;
}

inline int32_t computePaddingWithOffset(int32_t stride, int32_t dilation_rate, int32_t in_size,
                                        int32_t filter_size, int32_t out_size, int32_t *offset)
{
  int32_t effective_filter_size = (filter_size - 1) * dilation_rate + 1;
  int32_t total_padding = ((out_size - 1) * stride + effective_filter_size - in_size);
  total_padding = total_padding > 0 ? total_padding : 0;
  *offset = total_padding % 2;
  return total_padding / 2;
}

inline int32_t computeOutputSize(Padding padding, int32_t image_size, int32_t filter_size,
                                 int32_t stride, int32_t dilation_rate = 1)
{
  const int32_t effective_filter_size = (filter_size - 1) * dilation_rate + 1;
  switch (padding)
  {
    case Padding::SAME:
      assert(stride != 0);
      return (image_size + stride - 1) / stride;
    case Padding::VALID:
      assert(stride != 0);
      return (image_size + stride - effective_filter_size) / stride;
    default:
      assert(false);
      return 0;
  }
}

inline int32_t calcOffset(const circle::Tensor *tensor, int32_t d0, int32_t d1, int32_t d2,
                          int32_t d3)
{

  return ((d0 * Tensor::dim(tensor, 1) + d1) * Tensor::dim(tensor, 2) + d2) *
           Tensor::dim(tensor, 3) +
         d3;
}

template <typename T>
void calculateActivationRange(Activation activation, T *activation_min, T *activation_max);

luci_interpreter::RuntimeShape calculateShapeForBroadcast(const circle::Tensor *input1,
                                                          const circle::Tensor *input2);

// Helper wrapper to hide broadcast logic
template <typename T> class BroadcastableWrapper
{
public:
  BroadcastableWrapper(const std::vector<T> &v) : _v(v), _stride(v.size() == 1 ? 0 : 1) {}

  T operator[](int idx) { return _v[idx * _stride]; }

private:
  const std::vector<T> &_v;
  int _stride;
};

inline luci_interpreter::RuntimeShape getTensorShape(const circle::Tensor *tensor)
{
  if (tensor == nullptr)
    return luci_interpreter::RuntimeShape();

  auto const tensor_shape = Tensor::tensor_shape(tensor);

  luci_interpreter::RuntimeShape runtime_shape(tensor_shape.size());
  for (int i = 0; i < tensor_shape.size(); ++i)
  {
    runtime_shape.setDim(i, tensor_shape[i]);
  }
  return runtime_shape;
}

inline void getTensorDims(const circle::Tensor *tensor, BaseRuntimeGraph *runtime_graph,
                          int32_t *dims)
{
  if (tensor == nullptr)
  {
    dims = nullptr;
    return;
  }

#ifndef DIS_DYN_SHAPES
  auto *dynamic_shape_vector = runtime_graph->getDynamicShapeTensor(tensor);
  if (dynamic_shape_vector != nullptr)
  {
    for (int n = 0; n < dynamic_shape_vector->dimensionsCount(); ++n)
    {
      dims[n] = dynamic_shape_vector->dims(n);
    }
  }
  else
  {
    auto const tensor_shape = Tensor::tensor_shape(tensor);
    assert(tensor_shape.size() <= kMaxSmallSize);
    for (int i = 0; i < tensor_shape.size(); ++i)
    {
      dims[i] = tensor_shape[i];
    }
  }
#else
  auto const tensor_shape = Tensor::tensor_shape(tensor);
  assert(tensor_shape.size() <= kMaxSmallSize);
  for (int i = 0; i < tensor_shape.size(); ++i)
  {
    dims[i] = tensor_shape[i];
  }
#endif // DIS_DYN_SHAPES
}

template <typename T> const T *getTensorData(const uint8_t *tensor_data)
{
  return tensor_data != nullptr ? reinterpret_cast<const T *>(tensor_data) : nullptr;
}

template <typename T> inline T *getTensorData(uint8_t *tensor_data)
{
  return tensor_data != nullptr ? reinterpret_cast<T *>(tensor_data) : nullptr;
}

luci_interpreter::RuntimeShape getTensorRuntimeShape(const circle::Tensor *circle_tensor,
                                                     BaseRuntimeGraph *runtime_graph);

// A list of tensors in a format that can be used by kernels like split and
// concatenation.
template <typename T, bool is_const> class VectorOfTensors
{
public:
  using ElementT = typename std::conditional<is_const, const T, T>::type;
  using TensorT = typename std::conditional<is_const, const Tensor, Tensor>::type;

  // Build with the tensors in 'tensor_list'.
  explicit VectorOfTensors(const std::vector<TensorT *> &tensor_list)
  {
    const int num_tensors = tensor_list.size();

    all_data_.reserve(num_tensors);
    all_shape_.reserve(num_tensors);
    all_shape_ptr_.reserve(num_tensors);

    for (TensorT *tensor : tensor_list)
    {
      all_data_.push_back(getTensorData<T>(tensor));
      all_shape_.push_back(getTensorShape(tensor));
    }

    // Taking the pointer from inside a std::vector is only OK if the vector is
    // never modified, so we populate all_shape in the previous loop and then we
    // are free to grab iterators here.
    for (luci_interpreter::RuntimeShape &shape : all_shape_)
    {
      all_shape_ptr_.push_back(&shape);
    }
  }
  // Return a pointer to the data pointers of all tensors in the list. For
  // example:
  //   float* const* f = v.data();
  //   f[0][1] is the second element of the first tensor.
  ElementT *const *data() const { return all_data_.data(); }

  // Return a pointer the shape pointers of all tensors in the list. For
  // example:
  //   const RuntimeShape* const* d = v.dims();
  //   dims[1] are the dimensions of the second tensor in the list.
  const luci_interpreter::RuntimeShape *const *shapes() const { return all_shape_ptr_.data(); }

private:
  std::vector<ElementT *> all_data_;
  std::vector<luci_interpreter::RuntimeShape> all_shape_;
  std::vector<luci_interpreter::RuntimeShape *> all_shape_ptr_;
};

template <typename T> constexpr bool one_of_types() { return false; }

// Checks if T is equal to one of {U,Other} types
template <typename T, typename U, typename... Other> constexpr bool one_of_types()
{
  return std::is_same<T, U>::value || one_of_types<T, Other...>();
}

void matrixScalarMultiplyAccumulate(const int8_t *matrix, int32_t scalar, int32_t n_row,
                                    int32_t n_col, int32_t *output);

// Checks if input and output dimensions are equal
bool areShapesEqual(const luci_interpreter::RuntimeShape &input_shape1,
                    const luci_interpreter::RuntimeShape &input_shape2);

#ifndef DIS_QUANT
bool checkedLog2(const float x, int *log2_result);

int calculateInputRadius(int input_integer_bits, int input_left_shift, int total_signed_bits);

void calculateActivationRangeQuantized(Activation activation, const circle::Tensor *output,
                                       int32_t *activation_min, int32_t *activation_max);

void calculateActivationRangeQuantized(Activation activation, int32_t output_zero_point,
                                       float output_scale, DataType data_type,
                                       int32_t *activation_min, int32_t *activation_max);

// Decompose a double multiplier into a Q0.31 int32 representation of its
// significand, and shift representation of its exponent.
//
// Handles an arbitrary positive multiplier. The 'shift' output-value is
// basically the 'floating-point exponent' of the multiplier:
// Negative for a right-shift (when the multiplier is <1), positive for a
// left-shift (when the multiplier is >1)
void quantizeMultiplier(double double_multiplier, int32_t *quantized_multiplier, int *shift);

// Decompose a double multiplier into a Q0.31 int32 representation of its
// significand, and shift representation of NEGATIVE its exponent ---
// this is intended as a RIGHT-shift.
//
// Restricted to the case where the multiplier < 1 (and non-negative).
void quantizeMultiplierSmallerThanOneExp(double double_multiplier, int32_t *quantized_multiplier,
                                         int *left_shift);

inline double getQuantizedConvolutionMultipler(float input_scale, float filter_scale,
                                               float output_scale)
{
  const double input_product_scale = static_cast<double>(input_scale * filter_scale);
  LUCI_INTERPRETER_CHECK(input_product_scale >= 0);
  return input_product_scale / static_cast<double>(output_scale);
}

// TODO rename getQuantizedConvolutionMultiplers to something more general
// it is used for non conv operators too
inline std::vector<double> getQuantizedConvolutionMultiplers(float input_scale,
                                                             const std::vector<float> &filter_scale,
                                                             float output_scale)
{
  std::vector<double> effective_output_scales;
  size_t n = filter_scale.size();
  effective_output_scales.reserve(n);
  for (size_t i = 0; i < n; ++i)
  {
    effective_output_scales.push_back(
      getQuantizedConvolutionMultipler(input_scale, filter_scale[i], output_scale));
  }
  return effective_output_scales;
}

struct ChannelQuantMultipliers
{
  int shift;
  int32_t multiplier;
  ChannelQuantMultipliers() = default;
};

inline std::vector<ChannelQuantMultipliers>
quantizeMultipliers(const std::vector<double> &effective_scale)
{
  size_t n = effective_scale.size();
  std::vector<ChannelQuantMultipliers> params(n);
  for (size_t i = 0; i < n; ++i)
  {
    quantizeMultiplier(effective_scale[i], &params[i].multiplier, &params[i].shift);
  }
  return params;
}

// A list of quantized tensors in a format that can be used by kernels like
// split and concatenation.
template <bool is_const> class VectorOfQuantizedTensors : public VectorOfTensors<uint8_t, is_const>
{
public:
  using typename VectorOfTensors<uint8_t, is_const>::TensorT;

  // Build with the tensors in 'tensor_list'.
  explicit VectorOfQuantizedTensors(const std::vector<TensorT *> &tensor_list)
    : VectorOfTensors<uint8_t, is_const>(tensor_list)
  {
    for (TensorT *tensor : tensor_list)
    {
      zero_point_.push_back(tensor->zero_point());
      scale_.push_back(tensor->scale());
    }
  }

  const float *scale() const { return scale_.data(); }
  const int32_t *zero_point() const { return zero_point_.data(); }

private:
  std::vector<int32_t> zero_point_;
  std::vector<float> scale_;
};
#endif // DIS_QUANT

} // namespace kernels
} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNELS_UTILS_H
