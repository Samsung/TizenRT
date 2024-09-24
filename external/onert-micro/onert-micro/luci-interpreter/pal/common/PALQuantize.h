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

#ifndef LUCI_INTERPRETER_PAL_QUANTIZE_H
#define LUCI_INTERPRETER_PAL_QUANTIZE_H

#include "Params.h"
#include <cmath>

namespace luci_interpreter_pal
{
template <typename InputT, typename OutputT>
inline void Quantize(const QuantizationParams &op_params, const int flat_size,
                     const InputT *input_data, OutputT *output_data)
{
  const int32_t zero_point = op_params.zero_point;
  const double scale = op_params.scale;
  static constexpr int32_t min_val = std::numeric_limits<OutputT>::min();
  static constexpr int32_t max_val = std::numeric_limits<OutputT>::max();

  for (int i = 0; i < flat_size; i++)
  {
    const InputT val = input_data[i];
    int32_t unclamped =
      static_cast<int32_t>(std::round(val / static_cast<float>(scale))) + zero_point;
    int32_t clamped = std::min(std::max(unclamped, min_val), max_val);
    output_data[i] = clamped;
  }
}
} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_QUANTIZE_H
