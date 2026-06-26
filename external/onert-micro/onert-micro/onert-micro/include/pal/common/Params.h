/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef ONERT_MICRO_EXECUTE_PAL_PARAMS_H
#define ONERT_MICRO_EXECUTE_PAL_PARAMS_H

namespace onert_micro
{
namespace execute
{
namespace pal
{
//
// struct MeanParams
//{
//  int8_t axis_count;
//  int16_t axis[4];
//};
//
// struct ResizeNearestNeighborParams
//{
//  bool align_corners;
//  bool half_pixel_centers;
//};
//
// struct PadParams
//{
//  int8_t left_padding_count;
//  int32_t left_padding[5];
//  int8_t right_padding_count;
//  int32_t right_padding[5];
//};
//
// enum class PaddingType : uint8_t
//{
//  None,
//  Same,
//  Valid
//};
//
// struct PaddingValues
//{
//  int16_t width;
//  int16_t height;
//  // offset is used for calculating "remaining" padding, for example, `width`
//  // is 1 and `width_offset` is 1, so padding_left is 1 while padding_right is
//  // 1 + 1 = 2.
//  int16_t width_offset;
//  // Same as width_offset except it's over the height dimension.
//  int16_t height_offset;
//};
//
// struct ConvParams
//{
//  PaddingType padding_type;
//  PaddingValues padding_values;
//  int16_t stride_width;
//  int16_t stride_height;
//  int16_t dilation_width_factor;
//  int16_t dilation_height_factor;
//  // uint8_t inference params.
//  int32_t input_offset;
//  int32_t weights_offset;
//  int32_t output_offset;
//  int32_t output_multiplier;
//  int output_shift;
//  // uint8_t, etc, activation params.
//  int32_t quantized_activation_min;
//  int32_t quantized_activation_max;
//  // float activation params.
//  float float_activation_min;
//  float float_activation_max;
//  int32_t depth_multiplier;
//  std::vector<int32_t> per_channel_output_multiplier;
//  std::vector<int> per_channel_output_shift;
//};
//
// enum class BroadcastableOpCategory : uint8_t
//{
//  kNone,
//  kNonBroadcast,              // Matching input shapes.
//  kFirstInputBroadcastsFast,  // Fivefold nested loops.
//  kSecondInputBroadcastsFast, // Fivefold nested loops.
//  kGenericBroadcast,          // Fall-back.
//  kScalarFirstBroadcast,      // Scalar
//  kScalarSecondBroadcast,     // Scalar
//};
//
////struct ConcatenationParams
////{
////  int8_t axis;
////  const int32_t *input_zeropoint;
////  const float *input_scale;
////  uint16_t inputs_count;
////  int32_t output_zeropoint;
////  float output_scale;
////};
//
// struct TransposeParams
//{
//  int8_t perm_count;
//  int32_t perm[5];
//};
//
// struct PreluParams
//{
//  int32_t input_offset;
//  int32_t alpha_offset;
//  int32_t output_offset;
//  int32_t output_multiplier_1;
//  int output_shift_1;
//  int32_t output_multiplier_2;
//  int output_shift_2;
//};
//
// struct ComparisonParams
//{
//  // uint8_t inference params.
//  int left_shift;
//  int32_t input1_offset;
//  int32_t input1_multiplier;
//  int input1_shift;
//  int32_t input2_offset;
//  int32_t input2_multiplier;
//  int input2_shift;
//  // Shape dependent / common to inference types.
//  bool is_broadcast;
//};
//
// struct StridedSliceParams
//{
//  int8_t start_indices_count;
//  int32_t start_indices[5];
//  int8_t stop_indices_count;
//  int32_t stop_indices[5];
//  int8_t strides_count;
//  int32_t strides[5];
//
//  int16_t begin_mask;
//  int16_t ellipsis_mask;
//  int16_t end_mask;
//  int16_t new_axis_mask;
//  int16_t shrink_axis_mask;
//};
//
//// For Add, Sub, Mul ops.
////struct ArithmeticParams
////{
////  // Shape dependent / common to data / op types.
////  BroadcastableOpCategory broadcast_category;
////  // uint8_t inference params.
////  int32_t input1_offset;
////  int32_t input2_offset;
////  int32_t output_offset;
////  int32_t output_multiplier;
////  int output_shift;
////  // Add / Sub, not Mul, uint8_t inference params.
////  int left_shift;
////  int32_t input1_multiplier;
////  int input1_shift;
////  int32_t input2_multiplier;
////  int input2_shift;
////  // uint8_t, etc, activation params.
////  int32_t quantized_activation_min;
////  int32_t quantized_activation_max;
////  // float activation params.
////  float float_activation_min;
////  float float_activation_max;
////  // int64_t activation params.
////  int64_t int64_activation_min;
////  int64_t int64_activation_max;
////
////  // Processed output dimensions.
////  // Let input "a" be the one that broadcasts in the faster-changing dimension.
////  // Then, after coalescing, for shapes {a0, a1, a2, a3, a4} and
////  // {b0, b1, b2, b3, b4},
////  // broadcast_shape[4] = b0 = a0.
////  // broadcast_shape[3] = b1; a1 = 1.
////  // broadcast_shape[2] = b2 = a2.
////  // broadcast_shape[1] = a3; b3 = 1.
////  // broadcast_shape[0] = b4 = a4.
////  int broadcast_shape[5];
////};
//
// enum class FusedActivationFunctionType : uint8_t
//{
//  kNone,
//  kRelu6,
//  kRelu1,
//  kRelu
//};
//
// struct PoolParams
//{
//  FusedActivationFunctionType activation;
//  PaddingType padding_type;
//  PaddingValues padding_values;
//  int stride_height;
//  int stride_width;
//  int filter_height;
//  int filter_width;
//  // uint8_t, etc, activation params.
//  int32_t quantized_activation_min;
//  int32_t quantized_activation_max;
//  // float activation params.
//  float float_activation_min;
//  float float_activation_max;
//};
//
// struct SoftmaxParams
//{
//  float beta;
//  int32_t input_multiplier;
//  int32_t input_left_shift;
//  int diff_min;
//  int num_rows;
//  int row_size;
//};
//
// struct QuantizationParams
//{
//  float scale;
//  int zero_point;
//};

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_PARAMS_H
