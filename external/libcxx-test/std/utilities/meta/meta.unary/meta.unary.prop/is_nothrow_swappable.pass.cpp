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

// is_swappable

#include <type_traits>
#include <vector>
#include "test_macros.h"
#include "libcxx_tc_common.h"

namespace MyNS {

// Make the test types non-copyable so that generic std::swap is not valid.
struct A {
  A(A const&) = delete;
  A& operator=(A const&) = delete;
};

struct B {
  B(B const&) = delete;
  B& operator=(B const&) = delete;
};

void swap(A&, A&) noexcept {}
void swap(B&, B&) {}

struct M {
  M(M const&) = delete;
  M& operator=(M const&) = delete;
};

void swap(M&&, M&&) noexcept {}

struct ThrowingMove {
    ThrowingMove(ThrowingMove&&) {}
    ThrowingMove& operator=(ThrowingMove&&) { return *this; }
};

} // namespace MyNS

int tc_utilities_meta_meta_unary_meta_unary_prop_is_nothrow_swappable(void) {
    using namespace MyNS;
    {
        // Test that is_swappable applies an lvalue reference to the type.
        static_assert(std::is_nothrow_swappable<int>::value, "");
        static_assert(std::is_nothrow_swappable<int&>::value, "");
        static_assert(!std::is_nothrow_swappable<M>::value, "");
        static_assert(!std::is_nothrow_swappable<M&&>::value, "");
    }
    {
        // Test that it correctly deduces the noexcept of swap.
        static_assert(std::is_nothrow_swappable<A>::value, "");
        static_assert(!std::is_nothrow_swappable<B>::value
                      && std::is_swappable<B>::value, "");
        static_assert(!std::is_nothrow_swappable<ThrowingMove>::value
                      && std::is_swappable<ThrowingMove>::value, "");
    }
    {
        // Test that it doesn't drop the qualifiers
        static_assert(!std::is_nothrow_swappable<const A>::value, "");
    }
    {
        // test non-referenceable types
        static_assert(!std::is_nothrow_swappable<void>::value, "");
        static_assert(!std::is_nothrow_swappable<int() const>::value, "");
        static_assert(!std::is_nothrow_swappable<int(int, ...) const &>::value, "");
    }
    {
        // test for presence of is_nothrow_swappable_v
        static_assert(std::is_nothrow_swappable_v<int>, "");
        static_assert(!std::is_nothrow_swappable_v<void>, "");
    }

  return 0;
}
