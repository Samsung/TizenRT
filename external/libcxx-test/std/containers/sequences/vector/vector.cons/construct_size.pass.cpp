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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>

// explicit vector(size_type n);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "DefaultOnly.h"
#include "test_allocator.h"
#include "asan_testing.h"

template <class C>
static int
test2(typename C::size_type n, typename C::allocator_type const& a = typename C::allocator_type ())
{
#if TEST_STD_VER >= 14
    C c(n, a);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.size() == n);
    TC_ASSERT_EXPR(c.get_allocator() == a);
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
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
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
#if TEST_STD_VER >= 11
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
        TC_ASSERT_EXPR(*i == typename C::value_type());
#endif
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

int tc_libcxx_containers_vector_cons_construct_size(void)
{
    TC_ASSERT_FUNC((test<std::vector<int> >(50)));
    TC_ASSERT_FUNC((test<std::vector<DefaultOnly> >(500)));
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
    TC_SUCCESS_RESULT();
    return 0;
}
