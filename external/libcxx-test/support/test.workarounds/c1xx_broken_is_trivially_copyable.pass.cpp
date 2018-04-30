/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// This workaround option is specific to MSVC's C1XX, so we don't care that
// it isn't set for older GCC versions.
// XFAIL: gcc-4.9

// Verify TEST_WORKAROUND_C1XX_BROKEN_IS_TRIVIALLY_COPYABLE.

#include <type_traits>

#include "test_workarounds.h"

struct S {
  S(S const&) = default;
  S(S&&) = default;
  S& operator=(S const&) = delete;
  S& operator=(S&&) = delete;
};

int main() {
#if defined(TEST_WORKAROUND_C1XX_BROKEN_IS_TRIVIALLY_COPYABLE)
  static_assert(!std::is_trivially_copyable<S>::value, "");
#else
  static_assert(std::is_trivially_copyable<S>::value, "");
#endif
}
