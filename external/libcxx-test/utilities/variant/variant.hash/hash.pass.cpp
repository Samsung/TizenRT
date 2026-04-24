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

// template <class... Types> struct hash<variant<Types...>>;
// template <> struct hash<monostate>;

#include <cassert>
#include <type_traits>
#include <variant>

#include "test_macros.h"
#include "variant_test_helpers.h"
#include "poisoned_hash_helper.h"
#include "libcxx_tc_common.h"

#ifndef TEST_HAS_NO_EXCEPTIONS
namespace std {
template <> struct hash<::MakeEmptyT> {
  std::size_t operator()(const ::MakeEmptyT &) const {
    TC_ASSERT_EXPR(false);
    return 0;
  }
};
}
#endif

void test_hash_variant() {
  {
    using V = std::variant<int, long, int>;
    using H = std::hash<V>;
    const V v(std::in_place_index<0>, 42);
    const V v_copy = v;
    V v2(std::in_place_index<0>, 100);
    const H h{};
    TC_ASSERT_EXPR(h(v) == h(v));
    TC_ASSERT_EXPR(h(v) != h(v2));
    TC_ASSERT_EXPR(h(v) == h(v_copy));
    {
      ASSERT_SAME_TYPE(decltype(h(v)), std::size_t);
      static_assert(std::is_copy_constructible<H>::value, "");
    }
  }
  {
    using V = std::variant<std::monostate, int, long, const char *>;
    using H = std::hash<V>;
    const char *str = "hello";
    const V v0;
    const V v0_other;
    const V v1(42);
    const V v1_other(100);
    V v2(100l);
    V v2_other(999l);
    V v3(str);
    V v3_other("not hello");
    const H h{};
    TC_ASSERT_EXPR(h(v0) == h(v0));
    TC_ASSERT_EXPR(h(v0) == h(v0_other));
    TC_ASSERT_EXPR(h(v1) == h(v1));
    TC_ASSERT_EXPR(h(v1) != h(v1_other));
    TC_ASSERT_EXPR(h(v2) == h(v2));
    TC_ASSERT_EXPR(h(v2) != h(v2_other));
    TC_ASSERT_EXPR(h(v3) == h(v3));
    TC_ASSERT_EXPR(h(v3) != h(v3_other));
    TC_ASSERT_EXPR(h(v0) != h(v1));
    TC_ASSERT_EXPR(h(v0) != h(v2));
    TC_ASSERT_EXPR(h(v0) != h(v3));
    TC_ASSERT_EXPR(h(v1) != h(v2));
    TC_ASSERT_EXPR(h(v1) != h(v3));
    TC_ASSERT_EXPR(h(v2) != h(v3));
  }
#ifndef TEST_HAS_NO_EXCEPTIONS
  {
    using V = std::variant<int, MakeEmptyT>;
    using H = std::hash<V>;
    V v;
    makeEmpty(v);
    V v2;
    makeEmpty(v2);
    const H h{};
    TC_ASSERT_EXPR(h(v) == h(v2));
  }
#endif
}

void test_hash_monostate() {
  using H = std::hash<std::monostate>;
  const H h{};
  std::monostate m1{};
  const std::monostate m2{};
  TC_ASSERT_EXPR(h(m1) == h(m1));
  TC_ASSERT_EXPR(h(m2) == h(m2));
  TC_ASSERT_EXPR(h(m1) == h(m2));
  {
    ASSERT_SAME_TYPE(decltype(h(m1)), std::size_t);
    ASSERT_NOEXCEPT(h(m1));
    static_assert(std::is_copy_constructible<H>::value, "");
  }
  {
    test_hash_enabled_for_type<std::monostate>();
  }
}

void test_hash_variant_duplicate_elements() {
    // Test that the index of the alternative participates in the hash value.
    using V = std::variant<std::monostate, std::monostate>;
    using H = std::hash<V>;
    H h{};
    const V v1(std::in_place_index<0>);
    const V v2(std::in_place_index<1>);
    TC_ASSERT_EXPR(h(v1) == h(v1));
    TC_ASSERT_EXPR(h(v2) == h(v2));
    LIBCPP_ASSERT(h(v1) != h(v2));
}

struct A {};
struct B {};

namespace std {

template <>
struct hash<B> {
  std::size_t operator()(B const&) const {
    return 0;
  }
};

}

void test_hash_variant_enabled() {
  {
    test_hash_enabled_for_type<std::variant<int> >();
    test_hash_enabled_for_type<std::variant<int*, long, double, const int> >();
  }
  {
    test_hash_disabled_for_type<std::variant<int, A>>();
    test_hash_disabled_for_type<std::variant<const A, void*>>();
  }
  {
    test_hash_enabled_for_type<std::variant<int, B>>();
    test_hash_enabled_for_type<std::variant<const B, int>>();
  }
}

int tc_utilities_variant_variant_hash_hash(void) {
  test_hash_variant();
  test_hash_variant_duplicate_elements();
  test_hash_monostate();
  test_hash_variant_enabled();

  return 0;
}
