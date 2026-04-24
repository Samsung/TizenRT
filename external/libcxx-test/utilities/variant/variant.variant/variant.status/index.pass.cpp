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

// constexpr size_t index() const noexcept;

#include <cassert>
#include <string>
#include <type_traits>
#include <variant>

#include "archetypes.h"
#include "test_macros.h"
#include "variant_test_helpers.h"
#include "libcxx_tc_common.h"


int tc_utilities_variant_variant_variant_variant_status_index(void) {
  {
    using V = std::variant<int, long>;
    constexpr V v;
    static_assert(v.index() == 0, "");
  }
  {
    using V = std::variant<int, long>;
    V v;
    TC_ASSERT_EXPR(v.index() == 0);
  }
  {
    using V = std::variant<int, long>;
    constexpr V v(std::in_place_index<1>);
    static_assert(v.index() == 1, "");
  }
  {
    using V = std::variant<int, std::string>;
    V v("abc");
    TC_ASSERT_EXPR(v.index() == 1);
    v = 42;
    TC_ASSERT_EXPR(v.index() == 0);
  }
#ifndef TEST_HAS_NO_EXCEPTIONS
  {
    using V = std::variant<int, MakeEmptyT>;
    V v;
    TC_ASSERT_EXPR(v.index() == 0);
    makeEmpty(v);
    TC_ASSERT_EXPR(v.index() == std::variant_npos);
  }
#endif

  return 0;
}
