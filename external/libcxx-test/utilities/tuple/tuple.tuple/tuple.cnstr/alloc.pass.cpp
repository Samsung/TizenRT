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
// UNSUPPORTED: c++03

// <tuple>

// template <class... Types> class tuple;

// template <class Alloc>
//   explicit(see-below) tuple(allocator_arg_t, const Alloc& a);

#include <tuple>
#include <cassert>

#include "test_macros.h"
#include "DefaultOnly.h"
#include "allocators.h"
#include "../alloc_first.h"
#include "../alloc_last.h"
#include "libcxx_tc_common.h"

template <class T = void>
struct NonDefaultConstructible {
  constexpr NonDefaultConstructible() {
      static_assert(!std::is_same<T, T>::value, "Default Ctor instantiated");
  }

  explicit constexpr NonDefaultConstructible(int) {}
};


struct DerivedFromAllocArgT : std::allocator_arg_t {};

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_alloc(void) {
    {
        std::tuple<> t(std::allocator_arg, A1<int>());
    }
    {
        std::tuple<int> t(std::allocator_arg, A1<int>());
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
    }
    {
        std::tuple<DefaultOnly> t(std::allocator_arg, A1<int>());
        TC_ASSERT_EXPR(std::get<0>(t) == DefaultOnly());
    }
    {
        TC_ASSERT_EXPR(!alloc_first::allocator_constructed);
        std::tuple<alloc_first> t(std::allocator_arg, A1<int>(5));
        TC_ASSERT_EXPR(alloc_first::allocator_constructed);
        TC_ASSERT_EXPR(std::get<0>(t) == alloc_first());
    }
    {
        TC_ASSERT_EXPR(!alloc_last::allocator_constructed);
        std::tuple<alloc_last> t(std::allocator_arg, A1<int>(5));
        TC_ASSERT_EXPR(alloc_last::allocator_constructed);
        TC_ASSERT_EXPR(std::get<0>(t) == alloc_last());
    }
    {
        alloc_first::allocator_constructed = false;
        std::tuple<DefaultOnly, alloc_first> t(std::allocator_arg, A1<int>(5));
        TC_ASSERT_EXPR(std::get<0>(t) == DefaultOnly());
        TC_ASSERT_EXPR(alloc_first::allocator_constructed);
        TC_ASSERT_EXPR(std::get<1>(t) == alloc_first());
    }
    {
        alloc_first::allocator_constructed = false;
        alloc_last::allocator_constructed = false;
        std::tuple<DefaultOnly, alloc_first, alloc_last> t(std::allocator_arg,
                                                           A1<int>(5));
        TC_ASSERT_EXPR(std::get<0>(t) == DefaultOnly());
        TC_ASSERT_EXPR(alloc_first::allocator_constructed);
        TC_ASSERT_EXPR(std::get<1>(t) == alloc_first());
        TC_ASSERT_EXPR(alloc_last::allocator_constructed);
        TC_ASSERT_EXPR(std::get<2>(t) == alloc_last());
    }
    {
        alloc_first::allocator_constructed = false;
        alloc_last::allocator_constructed = false;
        std::tuple<DefaultOnly, alloc_first, alloc_last> t(std::allocator_arg,
                                                           A2<int>(5));
        TC_ASSERT_EXPR(std::get<0>(t) == DefaultOnly());
        TC_ASSERT_EXPR(!alloc_first::allocator_constructed);
        TC_ASSERT_EXPR(std::get<1>(t) == alloc_first());
        TC_ASSERT_EXPR(!alloc_last::allocator_constructed);
        TC_ASSERT_EXPR(std::get<2>(t) == alloc_last());
    }
    {
        // Test that we can use a tag derived from allocator_arg_t
        struct DerivedFromAllocatorArgT : std::allocator_arg_t { };
        DerivedFromAllocatorArgT derived;
        std::tuple<> t1(derived, A1<int>());
        std::tuple<int> t2(derived, A1<int>());
        std::tuple<int, int> t3(derived, A1<int>());
    }
    {
        // Test that the uses-allocator default constructor does not evaluate
        // its SFINAE when it otherwise shouldn't be selected. Do this by
        // using 'NonDefaultConstructible' which will cause a compile error
        // if std::is_default_constructible is evaluated on it.
        using T = NonDefaultConstructible<>;
        T v(42);
        std::tuple<T, T> t(v, v);
        (void)t;
        std::tuple<T, T> t2(42, 42);
        (void)t2;
    }

  return 0;
}
