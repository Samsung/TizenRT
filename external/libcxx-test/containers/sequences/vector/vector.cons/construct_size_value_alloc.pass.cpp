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

// vector(size_type n, const value_type& x, const allocator_type& a);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

template <class C>
TEST_CONSTEXPR_CXX20 void
test(typename C::size_type n, const typename C::value_type& x,
     const typename C::allocator_type& a)
{
    C c(n, x, a);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(a == c.get_allocator());
    TC_ASSERT_EXPR(c.size() == n);
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
        TC_ASSERT_EXPR(*i == x);
}

TEST_CONSTEXPR_CXX20 bool tests() {
    test<std::vector<int> >(0, 3, std::allocator<int>());
    test<std::vector<int> >(50, 3, std::allocator<int>());
#if TEST_STD_VER >= 11
    test<std::vector<int, min_allocator<int>> >(0, 3, min_allocator<int>());
    test<std::vector<int, min_allocator<int>> >(50, 3, min_allocator<int>());
    test<std::vector<int, safe_allocator<int>> >(0, 3, safe_allocator<int>());
    test<std::vector<int, safe_allocator<int>> >(50, 3, safe_allocator<int>());
#endif

    return true;
}

int tc_containers_sequences_vector_vector_cons_construct_size_value_alloc(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
