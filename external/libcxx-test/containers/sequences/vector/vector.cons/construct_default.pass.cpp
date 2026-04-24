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

// vector();
// vector(const Alloc&);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "../../../NotConstructible.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

template <class C>
TEST_CONSTEXPR_CXX20 void test0()
{
#if TEST_STD_VER > 14
    static_assert((noexcept(C{})), "" );
#elif TEST_STD_VER >= 11
    static_assert((noexcept(C()) == noexcept(typename C::allocator_type())), "" );
#endif
    C c;
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.empty());
    TC_ASSERT_EXPR(c.get_allocator() == typename C::allocator_type());
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
#if TEST_STD_VER >= 11
    C c1 = {};
    LIBCPP_ASSERT(c1.__invariants());
    TC_ASSERT_EXPR(c1.empty());
    TC_ASSERT_EXPR(c1.get_allocator() == typename C::allocator_type());
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c1));
#endif
}

template <class C>
TEST_CONSTEXPR_CXX20 void test1(const typename C::allocator_type& a)
{
#if TEST_STD_VER > 14
    static_assert((noexcept(C{typename C::allocator_type{}})), "" );
#elif TEST_STD_VER >= 11
    static_assert((noexcept(C(typename C::allocator_type())) == std::is_nothrow_copy_constructible<typename C::allocator_type>::value), "" );
#endif
    C c(a);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.empty());
    TC_ASSERT_EXPR(c.get_allocator() == a);
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
}

TEST_CONSTEXPR_CXX20 bool tests() {
    {
    test0<std::vector<int> >();
    test0<std::vector<NotConstructible> >();
    test1<std::vector<int, test_allocator<int> > >(test_allocator<int>(3));
    test1<std::vector<NotConstructible, test_allocator<NotConstructible> > >
        (test_allocator<NotConstructible>(5));
    }
    {
        std::vector<int, limited_allocator<int, 10> > v;
        TC_ASSERT_EXPR(v.empty());
    }
#if TEST_STD_VER >= 11
    {
    test0<std::vector<int, min_allocator<int>> >();
    test0<std::vector<NotConstructible, min_allocator<NotConstructible>> >();
    test1<std::vector<int, min_allocator<int> > >(min_allocator<int>{});
    test1<std::vector<NotConstructible, min_allocator<NotConstructible> > >
        (min_allocator<NotConstructible>{});
    }
    {
        std::vector<int, min_allocator<int> > v;
        TC_ASSERT_EXPR(v.empty());
    }

    {
    test0<std::vector<int, explicit_allocator<int>> >();
    test0<std::vector<NotConstructible, explicit_allocator<NotConstructible>> >();
    test1<std::vector<int, explicit_allocator<int> > >(explicit_allocator<int>{});
    test1<std::vector<NotConstructible, explicit_allocator<NotConstructible> > >
        (explicit_allocator<NotConstructible>{});
    }
    {
        std::vector<int, explicit_allocator<int> > v;
        TC_ASSERT_EXPR(v.empty());
    }
#endif

    return true;
}

int tc_containers_sequences_vector_vector_cons_construct_default(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
