/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_PAL_MIRROR_PAD_COMMON_H
#define LUCI_INTERPRETER_PAL_MIRROR_PAD_COMMON_H

#include "PALUtils.h"
#include "Params.h"

namespace luci_interpreter_pal
{
namespace
{

// Helper method that fills the left and right pads.
template <typename T>
inline void getPadding(const T *data, int offset, int64_t *left_pad, int64_t *right_pad)
{
  *left_pad = static_cast<int64_t>(*(data + offset * 2));
  *right_pad = static_cast<int64_t>(*(data + offset * 2 + 1));
}

// Given dimension index and the left/right padding.
// Returns the corresponding dimension in the input array.
inline int getInputDimension(int padded_dimension, int left_pad, int right_pad, int input_dim_size,
                             int offset)
{
  if (padded_dimension < left_pad)
  {
    const int original_ind = left_pad + offset - 1;
    return original_ind - (std::min(padded_dimension, original_ind - offset));
  }
  padded_dimension -= left_pad;
  if (padded_dimension >= input_dim_size)
  {
    padded_dimension -= input_dim_size;
    const int original_ind = input_dim_size - (1 + offset);
    return original_ind - std::min(padded_dimension, original_ind);
  }
  return padded_dimension;
}

// Given and index in output array, returns the index of the value
// in input array.
int getFlatIndex(int index, int num_dims, const luci_interpreter::DataType padding_matrix_type,
                 const uint8_t *padding_matrix_data, const int32_t *input_dims,
                 int *output_dims_num_elements, int *input_dims_num_elements, const int offset)
{
  int flat_index = 0;
  int64_t left_pad = 0, right_pad = 0, dimension_index, index_in_input;

  for (int i = 0; i < num_dims; ++i)
  {
    switch (padding_matrix_type)
    {
      case luci_interpreter::DataType::S32:
        getPadding(luci_interpreter::kernels::getTensorData<int32_t>(padding_matrix_data), i,
                   &left_pad, &right_pad);
        break;
      case luci_interpreter::DataType::S64:
        getPadding(luci_interpreter::kernels::getTensorData<int64_t>(padding_matrix_data), i,
                   &left_pad, &right_pad);
        break;
      default:
        break;
    }
    dimension_index = index / output_dims_num_elements[i];

    index_in_input = getInputDimension(dimension_index, left_pad, right_pad, input_dims[i], offset);

    flat_index += index_in_input * (input_dims_num_elements)[i];
    index %= output_dims_num_elements[i];
  }

  return flat_index;
}

} // namespace

template <typename T>
void MirrorPad(const luci_interpreter::DataType padding_matrix_type,
               const uint8_t *padding_matrix_data, const int32_t *input_dims,
               int *output_dims_num_elements, int *input_dims_num_elements, const T *input_data,
               T *output_data, const int offset, const int num_dims, const int output_size)
{
  for (int i = 0; i < output_size; ++i)
  {
    output_data[i] =
      input_data[getFlatIndex(i, num_dims, padding_matrix_type, padding_matrix_data, input_dims,
                              output_dims_num_elements, input_dims_num_elements, offset)];
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_MIRROR_PAD_COMMON_H
