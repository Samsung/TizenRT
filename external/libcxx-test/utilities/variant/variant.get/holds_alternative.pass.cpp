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

// template <class T, class... Types>
// constexpr bool holds_alternative(const variant<Types...>& v) noexcept;

#include "test_macros.h"
#include <variant>
#include "libcxx_tc_common.h"

int tc_utilities_variant_variant_get_holds_alternative(void) {
  {
    using V = std::variant<int>;
    constexpr V v;
    static_assert(std::holds_alternative<int>(v), "");
  }
  {
    using V = std::variant<int, long>;
    constexpr V v;
    static_assert(std::holds_alternative<int>(v), "");
    static_assert(!std::holds_alternative<long>(v), "");
  }
  { // noexcept test
    using V = std::variant<int>;
    const V v;
    ASSERT_NOEXCEPT(std::holds_alternative<int>(v));
  }

  return 0;
}
