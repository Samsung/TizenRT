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

// constexpr bool valueless_by_exception() const noexcept;

#include <cassert>
#include <string>
#include <type_traits>
#include <variant>

#include "archetypes.h"
#include "test_macros.h"
#include "variant_test_helpers.h"
#include "libcxx_tc_common.h"


int tc_utilities_variant_variant_variant_variant_status_valueless_by_exception(void) {
  {
    using V = std::variant<int, long>;
    constexpr V v;
    static_assert(!v.valueless_by_exception(), "");
  }
  {
    using V = std::variant<int, long>;
    V v;
    TC_ASSERT_EXPR(!v.valueless_by_exception());
  }
  {
    using V = std::variant<int, long, std::string>;
    const V v("abc");
    TC_ASSERT_EXPR(!v.valueless_by_exception());
  }
#ifndef TEST_HAS_NO_EXCEPTIONS
  {
    using V = std::variant<int, MakeEmptyT>;
    V v;
    TC_ASSERT_EXPR(!v.valueless_by_exception());
    makeEmpty(v);
    TC_ASSERT_EXPR(v.valueless_by_exception());
  }
#endif

  return 0;
}
