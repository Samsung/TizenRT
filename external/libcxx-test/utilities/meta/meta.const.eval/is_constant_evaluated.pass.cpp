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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <type_traits>

// constexpr bool is_constant_evaluated() noexcept; // C++20

#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#ifndef __cpp_lib_is_constant_evaluated
#if TEST_HAS_BUILTIN(__builtin_is_constant_evaluated)
# error __cpp_lib_is_constant_evaluated should be defined
#endif
#endif

// Disable the tautological constant evaluation warnings for this test,
// because it's explicitly testing those cases.
TEST_CLANG_DIAGNOSTIC_IGNORED("-Wconstant-evaluated")
TEST_MSVC_DIAGNOSTIC_IGNORED(5063)

template <bool> struct InTemplate {};

int tc_utilities_meta_meta_const_eval_is_constant_evaluated(void) {
  // Test the signature
  {
    ASSERT_SAME_TYPE(decltype(std::is_constant_evaluated()), bool);
    ASSERT_NOEXCEPT(std::is_constant_evaluated());
    constexpr bool p = std::is_constant_evaluated();
    TC_ASSERT_EXPR(p);
  }
  // Test the return value of the builtin for basic sanity only. It's the
  // compiler's job to test the builtin for correctness.
  {
    static_assert(std::is_constant_evaluated(), "");
    bool p = std::is_constant_evaluated();
    TC_ASSERT_EXPR(!p);
    ASSERT_SAME_TYPE(InTemplate<std::is_constant_evaluated()>, InTemplate<true>);
    static int local_static = std::is_constant_evaluated() ? 42 : -1;
    TC_ASSERT_EXPR(local_static == 42);
  }
  return 0;
}
