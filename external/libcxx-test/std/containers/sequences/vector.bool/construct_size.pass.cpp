//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>
// vector<bool>

// explicit vector(size_type n);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"

template <class C>
static int
test2(typename C::size_type n,
      typename C::allocator_type const& a = typename C::allocator_type ())
{
#if TEST_STD_VER >= 14
    C c(n, a);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.size() == n);
    TC_ASSERT_EXPR(c.get_allocator() == a);
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
        TC_ASSERT_EXPR(*i == typename C::value_type());
#else
  ((void)n);
  ((void)a);
#endif
    return 0;
}

template <class C>
static int
test1(typename C::size_type n)
{
    C c(n);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.size() == n);
    TC_ASSERT_EXPR(c.get_allocator() == typename C::allocator_type());
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
        TC_ASSERT_EXPR(*i == typename C::value_type());
    return 0;
}

template <class C>
static int
test(typename C::size_type n)
{
    TC_ASSERT_FUNC((test1<C> ( n )));
    TC_ASSERT_FUNC((test2<C> ( n )));
    return 0;
}

int tc_libcxx_containers_vector_bool_construct_size(void)
{
    TC_ASSERT_FUNC((test<std::vector<bool> >(50)));
#if TEST_STD_VER >= 11
    TC_ASSERT_FUNC((test2<std::vector<bool, test_allocator<bool>> >( 100, test_allocator<bool>(23))));
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
