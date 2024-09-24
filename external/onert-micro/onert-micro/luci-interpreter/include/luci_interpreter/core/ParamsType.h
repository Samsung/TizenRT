/*
 * Copyright (c) 2022 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef __LUCI_MICRO_INTERPRETER_PARAMS_TYPE_H__
#define __LUCI_MICRO_INTERPRETER_PARAMS_TYPE_H__

#include <vector>
#include <cstdint>
#include <utility>

namespace luci_interpreter
{
// TODO check can we remove it
enum class FusedActFunc
{
  UNDEFINED, // This is not defined by TFLite or Circle. This was added to
  // prevent programming error.
  NONE,
  RELU,
  RELU_N1_TO_1,
  RELU6,
  TANH,
  SIGN_BIT
};

enum class Padding
{
  UNDEFINED, // This is not defined by TFLite. This was added to prevent programming error.

  SAME,
  VALID,
};

enum class MirrorPadMode
{
  UNDEFINED, // This is not defined by Circle. This was added to prevent programming error.

  REFLECT,
  SYMMETRIC,
};

} // namespace luci_interpreter

#endif // __LUCI_MICRO_INTERPRETER_PARAMS_TYPE_H__
