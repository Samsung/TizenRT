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

//  template <class T, class... Types>
//  constexpr add_pointer_t<T> get_if(variant<Types...>* v) noexcept;
// template <class T, class... Types>
//  constexpr add_pointer_t<const T> get_if(const variant<Types...>* v)
//  noexcept;

#include "test_macros.h"
#include "variant_test_helpers.h"
#include <cassert>
#include <variant>
#include "libcxx_tc_common.h"

void test_const_get_if() {
  {
    using V = std::variant<int>;
    constexpr const V *v = nullptr;
    static_assert(std::get_if<int>(v) == nullptr, "");
  }
  {
    using V = std::variant<int, const long>;
    constexpr V v(42);
    ASSERT_NOEXCEPT(std::get_if<int>(&v));
    ASSERT_SAME_TYPE(decltype(std::get_if<int>(&v)), const int *);
    static_assert(*std::get_if<int>(&v) == 42, "");
    static_assert(std::get_if<const long>(&v) == nullptr, "");
  }
  {
    using V = std::variant<int, const long>;
    constexpr V v(42l);
    ASSERT_SAME_TYPE(decltype(std::get_if<const long>(&v)), const long *);
    static_assert(*std::get_if<const long>(&v) == 42, "");
    static_assert(std::get_if<int>(&v) == nullptr, "");
  }
// FIXME: Remove these once reference support is reinstated
#if !defined(TEST_VARIANT_HAS_NO_REFERENCES)
  {
    using V = std::variant<int &>;
    int x = 42;
    const V v(x);
    ASSERT_SAME_TYPE(decltype(std::get_if<int &>(&v)), int *);
    TC_ASSERT_EXPR(std::get_if<int &>(&v) == &x);
  }
  {
    using V = std::variant<int &&>;
    int x = 42;
    const V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(std::get_if<int &&>(&v)), int *);
    TC_ASSERT_EXPR(std::get_if<int &&>(&v) == &x);
  }
  {
    using V = std::variant<const int &&>;
    int x = 42;
    const V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(std::get_if<const int &&>(&v)), const int *);
    TC_ASSERT_EXPR(std::get_if<const int &&>(&v) == &x);
  }
#endif
}

void test_get_if() {
  {
    using V = std::variant<int>;
    V *v = nullptr;
    TC_ASSERT_EXPR(std::get_if<int>(v) == nullptr);
  }
  {
    using V = std::variant<int, const long>;
    V v(42);
    ASSERT_NOEXCEPT(std::get_if<int>(&v));
    ASSERT_SAME_TYPE(decltype(std::get_if<int>(&v)), int *);
    TC_ASSERT_EXPR(*std::get_if<int>(&v) == 42);
    TC_ASSERT_EXPR(std::get_if<const long>(&v) == nullptr);
  }
  {
    using V = std::variant<int, const long>;
    V v(42l);
    ASSERT_SAME_TYPE(decltype(std::get_if<const long>(&v)), const long *);
    TC_ASSERT_EXPR(*std::get_if<const long>(&v) == 42);
    TC_ASSERT_EXPR(std::get_if<int>(&v) == nullptr);
  }
// FIXME: Remove these once reference support is reinstated
#if !defined(TEST_VARIANT_HAS_NO_REFERENCES)
  {
    using V = std::variant<int &>;
    int x = 42;
    V v(x);
    ASSERT_SAME_TYPE(decltype(std::get_if<int &>(&v)), int *);
    TC_ASSERT_EXPR(std::get_if<int &>(&v) == &x);
  }
  {
    using V = std::variant<const int &>;
    int x = 42;
    V v(x);
    ASSERT_SAME_TYPE(decltype(std::get_if<const int &>(&v)), const int *);
    TC_ASSERT_EXPR(std::get_if<const int &>(&v) == &x);
  }
  {
    using V = std::variant<int &&>;
    int x = 42;
    V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(std::get_if<int &&>(&v)), int *);
    TC_ASSERT_EXPR(std::get_if<int &&>(&v) == &x);
  }
  {
    using V = std::variant<const int &&>;
    int x = 42;
    V v(std::move(x));
    ASSERT_SAME_TYPE(decltype(std::get_if<const int &&>(&v)), const int *);
    TC_ASSERT_EXPR(std::get_if<const int &&>(&v) == &x);
  }
#endif
}

int tc_utilities_variant_variant_get_get_if_type(void) {
  test_const_get_if();
  test_get_if();

  return 0;
}
