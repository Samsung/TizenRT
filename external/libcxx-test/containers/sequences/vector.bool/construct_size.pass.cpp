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
// <vector>
// vector<bool>

// explicit vector(size_type n);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
TEST_CONSTEXPR_CXX20 void test2(typename C::size_type n,
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
}

template <class C>
TEST_CONSTEXPR_CXX20 void test1(typename C::size_type n)
{
    C c(n);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.size() == n);
    TC_ASSERT_EXPR(c.get_allocator() == typename C::allocator_type());
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
        TC_ASSERT_EXPR(*i == typename C::value_type());
}

template <class C>
TEST_CONSTEXPR_CXX20 void test(typename C::size_type n)
{
    test1<C> ( n );
    test2<C> ( n );
}

TEST_CONSTEXPR_CXX20 bool tests()
{
    test<std::vector<bool> >(50);
#if TEST_STD_VER >= 11
    test<std::vector<bool, min_allocator<bool>> >(50);
    test2<std::vector<bool, test_allocator<bool>> >( 100, test_allocator<bool>(23));
#endif

    return true;
}

int tc_containers_sequences_vector_bool_construct_size(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
