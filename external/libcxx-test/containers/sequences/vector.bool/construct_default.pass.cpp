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
// vector<bool>

// vector();
// vector(const Alloc&);

// This tests a conforming extension
// For vector<>, this was added to the standard by N4258,
//   but vector<bool> was not changed.


#include <vector>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
TEST_CONSTEXPR_CXX20 void test0()
{
#if TEST_STD_VER > 14
    LIBCPP_STATIC_ASSERT((noexcept(C{})), "" );
#elif TEST_STD_VER >= 11
    LIBCPP_STATIC_ASSERT((noexcept(C()) == noexcept(typename C::allocator_type())), "" );
#endif
    C c;
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.empty());
    TC_ASSERT_EXPR(c.get_allocator() == typename C::allocator_type());
#if TEST_STD_VER >= 11
    C c1 = {};
    LIBCPP_ASSERT(c1.__invariants());
    TC_ASSERT_EXPR(c1.empty());
    TC_ASSERT_EXPR(c1.get_allocator() == typename C::allocator_type());
#endif
}

template <class C>
TEST_CONSTEXPR_CXX20 void test1(const typename C::allocator_type& a)
{
#if TEST_STD_VER > 14
    LIBCPP_STATIC_ASSERT((noexcept(C{typename C::allocator_type{}})), "" );
#elif TEST_STD_VER >= 11
    LIBCPP_STATIC_ASSERT((noexcept(C(typename C::allocator_type())) == std::is_nothrow_copy_constructible<typename C::allocator_type>::value), "" );
#endif
    C c(a);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.empty());
    TC_ASSERT_EXPR(c.get_allocator() == a);
}

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
    test0<std::vector<bool> >();
    test1<std::vector<bool, test_allocator<bool> > >(test_allocator<bool>(3));
    }
#if TEST_STD_VER >= 11
    {
    test0<std::vector<bool, min_allocator<bool>> >();
    test1<std::vector<bool, min_allocator<bool> > >(min_allocator<bool>());
    }
    {
    test0<std::vector<bool, explicit_allocator<bool>> >();
    test1<std::vector<bool, explicit_allocator<bool> > >(explicit_allocator<bool>());
    }
#endif

    return true;
}

int tc_containers_sequences_vector_bool_construct_default(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
