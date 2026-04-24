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

// explicit vector(size_type n);
// explicit vector(size_type n, const Allocator& alloc = Allocator());

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "DefaultOnly.h"
#include "min_allocator.h"
#include "test_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

template <class C>
TEST_CONSTEXPR_CXX20
void test(typename C::size_type n,
          typename C::allocator_type const& a = typename C::allocator_type())
{
    (void)a;
    // Test without a custom allocator
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
    }

    // Test with a custom allocator
#if TEST_STD_VER >= 14
    {
        C c(n, a);
        LIBCPP_ASSERT(c.__invariants());
        TC_ASSERT_EXPR(c.size() == n);
        TC_ASSERT_EXPR(c.get_allocator() == a);
        LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
        for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
            TC_ASSERT_EXPR(*i == typename C::value_type());
    }
#endif
}

TEST_CONSTEXPR_CXX20 bool tests() {
    test<std::vector<int> >(0);
    test<std::vector<int> >(50);
#if TEST_STD_VER >= 11
    test<std::vector<int, min_allocator<int>>>(0);
    test<std::vector<int, min_allocator<int>>>(50);
    test<std::vector<int, safe_allocator<int>>>(0);
    test<std::vector<int, safe_allocator<int>>>(50);
#endif

    return true;
}

int tc_containers_sequences_vector_vector_cons_construct_size(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    test<std::vector<DefaultOnly> >(0);
    test<std::vector<DefaultOnly> >(500);
    TC_ASSERT_EXPR(DefaultOnly::count == 0);

#if TEST_STD_VER >= 11
    test<std::vector<DefaultOnly, min_allocator<DefaultOnly>>>(0);
    test<std::vector<DefaultOnly, min_allocator<DefaultOnly>>>(500);
    test<std::vector<DefaultOnly, safe_allocator<DefaultOnly>>>(0);
    test<std::vector<DefaultOnly, safe_allocator<DefaultOnly>>>(500);
    test<std::vector<DefaultOnly, test_allocator<DefaultOnly>>>(0, test_allocator<DefaultOnly>(23));
    test<std::vector<DefaultOnly, test_allocator<DefaultOnly>>>(100, test_allocator<DefaultOnly>(23));
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
#endif

    return 0;
}
