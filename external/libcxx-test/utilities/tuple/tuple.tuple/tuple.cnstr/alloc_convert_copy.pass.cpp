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
// <tuple>

// template <class... Types> class tuple;

// template <class Alloc, class... UTypes>
// constexpr                                        // since c++20
//   tuple(allocator_arg_t, const Alloc& a, const tuple<UTypes...>&);

// UNSUPPORTED: c++03

#include <tuple>
#include <memory>
#include <cassert>

#include "test_macros.h"
#include "allocators.h"
#include "test_allocator.h"
#include "../alloc_first.h"
#include "../alloc_last.h"
#include "libcxx_tc_common.h"

struct Explicit {
  int value;
  explicit Explicit(int x) : value(x) {}
};

struct Implicit {
  int value;
  Implicit(int x) : value(x) {}
};

#if TEST_STD_VER > 17
constexpr bool alloc_copy_constructor_is_constexpr() {
  const std::tuple<int> t1 = 1;
  std::tuple<int> t2 = {std::allocator_arg, test_allocator<int>{}, t1};
  TC_ASSERT_EXPR(std::get<0>(t2) == 1);
  return true;
}
#endif

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_alloc_convert_copy(void) {
    {
        typedef std::tuple<long> T0;
        typedef std::tuple<long long> T1;
        T0 t0(2);
        T1 t1(std::allocator_arg, A1<int>(), t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
    }
    {
        typedef std::tuple<int> T0;
        typedef std::tuple<alloc_first> T1;
        T0 t0(2);
        alloc_first::allocator_constructed = false;
        T1 t1(std::allocator_arg, A1<int>(5), t0);
        TC_ASSERT_EXPR(alloc_first::allocator_constructed);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
    }
    {
        typedef std::tuple<int, int> T0;
        typedef std::tuple<alloc_first, alloc_last> T1;
        T0 t0(2, 3);
        alloc_first::allocator_constructed = false;
        alloc_last::allocator_constructed = false;
        T1 t1(std::allocator_arg, A1<int>(5), t0);
        TC_ASSERT_EXPR(alloc_first::allocator_constructed);
        TC_ASSERT_EXPR(alloc_last::allocator_constructed);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == 3);
    }
    {
        typedef std::tuple<long, int, int> T0;
        typedef std::tuple<long long, alloc_first, alloc_last> T1;
        T0 t0(1, 2, 3);
        alloc_first::allocator_constructed = false;
        alloc_last::allocator_constructed = false;
        T1 t1(std::allocator_arg, A1<int>(5), t0);
        TC_ASSERT_EXPR(alloc_first::allocator_constructed);
        TC_ASSERT_EXPR(alloc_last::allocator_constructed);
        TC_ASSERT_EXPR(std::get<0>(t1) == 1);
        TC_ASSERT_EXPR(std::get<1>(t1) == 2);
        TC_ASSERT_EXPR(std::get<2>(t1) == 3);
    }
    {
        const std::tuple<int> t1(42);
        std::tuple<Explicit> t2{std::allocator_arg, std::allocator<int>{},  t1};
        TC_ASSERT_EXPR(std::get<0>(t2).value == 42);
    }
    {
        const std::tuple<int> t1(42);
        std::tuple<Implicit> t2 = {std::allocator_arg, std::allocator<int>{}, t1};
        TC_ASSERT_EXPR(std::get<0>(t2).value == 42);
    }
    {
        // Test that we can use a tag derived from allocator_arg_t
        struct DerivedFromAllocatorArgT : std::allocator_arg_t { };
        DerivedFromAllocatorArgT derived;
        std::tuple<long> from(3l);
        std::tuple<long long> t0(derived, A1<int>(), from);
    }

#if TEST_STD_VER > 17
    static_assert(alloc_copy_constructor_is_constexpr());
#endif
    return 0;
}
