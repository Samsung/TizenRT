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
// UNSUPPORTED: c++03, c++11, c++14

// <variant>

// template <class ...Types> class variant;

// Make sure that the implicitly-generated CTAD works.

// We make sure that it is not ill-formed, however we still produce a warning for
// this one because explicit construction from a variant using CTAD is ambiguous
// (in the sense that the programer intent is not clear).
// ADDITIONAL_COMPILE_FLAGS: -Wno-ctad-maybe-unsupported

#include <variant>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_variant_variant_variant_implicit_ctad(void) {
  // This is the motivating example from P0739R0
  {
    std::variant<int, double> v1(3);
    std::variant v2 = v1;
    ASSERT_SAME_TYPE(decltype(v2), std::variant<int, double>);
  }

  {
    std::variant<int, double> v1(3);
    std::variant v2 = std::variant(v1); // Technically valid, but intent is ambiguous!
    ASSERT_SAME_TYPE(decltype(v2), std::variant<int, double>);
  }

  return 0;
}
