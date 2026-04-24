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

// type_traits

// is_nothrow_swappable_with

#include <type_traits>
#include <vector>
#include "test_macros.h"
#include "libcxx_tc_common.h"

namespace MyNS {

struct A {
  A(A const&) = delete;
  A& operator=(A const&) = delete;
};

struct B {
  B(B const&) = delete;
  B& operator=(B const&) = delete;
};

struct C {};
struct D {};

void swap(A&, A&) {}

void swap(A&, B&) noexcept {}
void swap(B&, A&) noexcept {}

void swap(A&, C&) noexcept {}
void swap(C&, A&) {}

struct M {};

void swap(M&&, M&&) noexcept {}

} // namespace MyNS

int tc_utilities_meta_meta_unary_meta_unary_prop_is_nothrow_swappable_with(void) {
    using namespace MyNS;
    {
        // Test that is_swappable_with doesn't apply an lvalue reference
        // to the type. Instead it is up to the user.
        static_assert(!std::is_nothrow_swappable_with<int, int>::value, "");
        static_assert(std::is_nothrow_swappable_with<int&, int&>::value, "");
        static_assert(std::is_nothrow_swappable_with<M, M>::value, "");
        static_assert(std::is_swappable_with<A&, A&>::value &&
                      !std::is_nothrow_swappable_with<A&, A&>::value, "");
    }
    {
        // test that heterogeneous swap is allowed only if both 'swap(A, B)' and
        // 'swap(B, A)' are valid.
        static_assert(std::is_nothrow_swappable_with<A&, B&>::value, "");
        static_assert(!std::is_nothrow_swappable_with<A&, C&>::value &&
                      std::is_swappable_with<A&, C&>::value, "");
        static_assert(!std::is_nothrow_swappable_with<D&, C&>::value, "");
    }
    {
        // test we guard against cv void inputs as required.
        static_assert(!std::is_nothrow_swappable_with_v<void, int>, "");
        static_assert(!std::is_nothrow_swappable_with_v<int, void>, "");
        static_assert(!std::is_nothrow_swappable_with_v<const void, const volatile void>, "");

    }
    {
        // test for presence of is_nothrow_swappable_with_v
        static_assert(std::is_nothrow_swappable_with_v<int&, int&>, "");
        static_assert(!std::is_nothrow_swappable_with_v<int&&, int&&>, "");
    }

  return 0;
}
