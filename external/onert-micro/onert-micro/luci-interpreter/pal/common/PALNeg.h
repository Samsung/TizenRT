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

#ifndef LUCI_INTERPRETER_PAL_NEG_H
#define LUCI_INTERPRETER_PAL_NEG_H

namespace luci_interpreter_pal
{
template <typename T>
inline void Negate(const luci_interpreter::RuntimeShape &input_shape, const T *input_data,
                   const luci_interpreter::RuntimeShape &output_shape, T *output_data)

{
  // check that input and output dimensions are equal
  int N = input_shape.dimensionsCount();
  assert(N == output_shape.dimensionsCount());

  // check that sizes of all dimensions are equal
  for (int i = 0; i < N; ++i)
  {
    assert(input_shape.dims(i) == output_shape.dims(i));
  }

  const int flat_size = input_shape.flatSize();

  for (int i = 0; i < flat_size; ++i)
  {
    output_data[i] = -input_data[i];
  }
}
} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_NEG_H
