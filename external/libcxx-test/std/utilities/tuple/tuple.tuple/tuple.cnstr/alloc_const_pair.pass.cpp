//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <tuple>

// template <class... Types> class tuple;

// template <class Alloc, class U1, class U2>
//   tuple(allocator_arg_t, const Alloc& a, const pair<U1, U2>&);

// UNSUPPORTED: c++03

#include <tuple>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "allocators.h"
#include "../alloc_first.h"
#include "../alloc_last.h"
#include "libcxx_tc_common.h"

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_alloc_const_pair(void) {
    {
        typedef std::pair<long, int> T0;
        typedef std::tuple<long long, double> T1;
        T0 t0(2, 3);
        T1 t1(std::allocator_arg, A1<int>(5), t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == 3);
    }
    {
        typedef std::pair<int, int> T0;
        typedef std::tuple<alloc_first, double> T1;
        T0 t0(2, 3);
        alloc_first::allocator_constructed = false;
        T1 t1(std::allocator_arg, A1<int>(5), t0);
        TC_ASSERT_EXPR(alloc_first::allocator_constructed);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == 3);
    }
    {
        typedef std::pair<int, int> T0;
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
        // Test that we can use a tag derived from allocator_arg_t
        struct DerivedFromAllocatorArgT : std::allocator_arg_t { };
        DerivedFromAllocatorArgT derived;
        std::pair<int, int> p(1, 2);
        std::tuple<int, int> t(derived, A1<int>(), p);
    }

    return 0;
}
