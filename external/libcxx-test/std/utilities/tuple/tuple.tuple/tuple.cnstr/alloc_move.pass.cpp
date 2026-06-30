//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <tuple>

// template <class... Types> class tuple;

// template <class Alloc>
//   tuple(allocator_arg_t, const Alloc& a, tuple&&);

// UNSUPPORTED: c++03

#include <tuple>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "allocators.h"
#include "../alloc_first.h"
#include "../alloc_last.h"
#include "libcxx_tc_common.h"

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_alloc_move(void) {
    {
        typedef std::tuple<> T;
        T t0;
        T t(std::allocator_arg, A1<int>(), std::move(t0));
    }
    {
        typedef std::tuple<MoveOnly> T;
        T t0(MoveOnly(0));
        T t(std::allocator_arg, A1<int>(), std::move(t0));
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
    }
    {
        typedef std::tuple<alloc_first> T;
        T t0(1);
        alloc_first::allocator_constructed = false;
        T t(std::allocator_arg, A1<int>(5), std::move(t0));
        TC_ASSERT_EXPR(alloc_first::allocator_constructed);
        TC_ASSERT_EXPR(std::get<0>(t) == 1);
    }
    {
        typedef std::tuple<alloc_last> T;
        T t0(1);
        alloc_last::allocator_constructed = false;
        T t(std::allocator_arg, A1<int>(5), std::move(t0));
        TC_ASSERT_EXPR(alloc_last::allocator_constructed);
        TC_ASSERT_EXPR(std::get<0>(t) == 1);
    }
    {
        typedef std::tuple<MoveOnly, alloc_first> T;
        T t0(0 ,1);
        alloc_first::allocator_constructed = false;
        T t(std::allocator_arg, A1<int>(5), std::move(t0));
        TC_ASSERT_EXPR(alloc_first::allocator_constructed);
        TC_ASSERT_EXPR(std::get<0>(t) == 0);
        TC_ASSERT_EXPR(std::get<1>(t) == 1);
    }
    {
        typedef std::tuple<MoveOnly, alloc_first, alloc_last> T;
        T t0(1, 2, 3);
        alloc_first::allocator_constructed = false;
        alloc_last::allocator_constructed = false;
        T t(std::allocator_arg, A1<int>(5), std::move(t0));
        TC_ASSERT_EXPR(alloc_first::allocator_constructed);
        TC_ASSERT_EXPR(alloc_last::allocator_constructed);
        TC_ASSERT_EXPR(std::get<0>(t) == 1);
        TC_ASSERT_EXPR(std::get<1>(t) == 2);
        TC_ASSERT_EXPR(std::get<2>(t) == 3);
    }
    {
        // Test that we can use a tag derived from allocator_arg_t
        struct DerivedFromAllocatorArgT : std::allocator_arg_t { };
        DerivedFromAllocatorArgT derived;
        std::tuple<int> from(3);
        std::tuple<int> t0(derived, A1<int>(), std::move(from));
    }

    return 0;
}
