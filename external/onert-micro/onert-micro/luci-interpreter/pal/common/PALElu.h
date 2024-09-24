/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2020 The TensorFlow Authors. All Rights Reserved.
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

#ifndef LUCI_INTERPRETER_PAL_ELU_H
#define LUCI_INTERPRETER_PAL_ELU_H

#include "PALUtils.h"
#include <cmath>

namespace luci_interpreter_pal
{

inline void Elu(const int flat_size, const float *input_data, float *output_data)
{
  for (int i = 0; i < flat_size; i++)
  {
    float val = input_data[i];
    float result = val < 0.0f ? std::expm1(val) : val;
    output_data[i] = result;
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_ELU_H
