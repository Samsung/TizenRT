/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_PAL_EXP_H
#define LUCI_INTERPRETER_PAL_EXP_H

#include "PALUtils.h"
#include <cmath>

namespace luci_interpreter_pal
{

inline void Log(const int flat_size, const float *input_data, float *output_data)
{
  for (int i = 0; i < flat_size; i++)
  {
    const float val = input_data[i];
    const float result = std::log(val);
    output_data[i] = result;
  }
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_EXP_H
