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

// vector(const vector& v, const allocator_type& a);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

template <class C>
TEST_CONSTEXPR_CXX20 void
test(const C& x, const typename C::allocator_type& a)
{
    typename C::size_type s = x.size();
    C c(x, a);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.size() == s);
    TC_ASSERT_EXPR(c == x);
    LIBCPP_ASSERT(is_contiguous_container_asan_correct(c));
}

TEST_CONSTEXPR_CXX20 bool tests() {
    {
        int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 1, 0};
        int* an = a + sizeof(a)/sizeof(a[0]);
        test(std::vector<int>(a, an), std::allocator<int>());
    }
    {
        std::vector<int, test_allocator<int> > l(3, 2, test_allocator<int>(5));
        std::vector<int, test_allocator<int> > l2(l, test_allocator<int>(3));
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == test_allocator<int>(3));
    }
    {
        std::vector<int, other_allocator<int> > l(3, 2, other_allocator<int>(5));
        std::vector<int, other_allocator<int> > l2(l, other_allocator<int>(3));
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == other_allocator<int>(3));
    }
    {
        // Test copy ctor with allocator and empty source
        std::vector<int, other_allocator<int> > l(other_allocator<int>(5));
        std::vector<int, other_allocator<int> > l2(l, other_allocator<int>(3));
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == other_allocator<int>(3));
        TC_ASSERT_EXPR(l2.empty());
    }
#if TEST_STD_VER >= 11
    {
        int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 1, 0};
        int* an = a + sizeof(a)/sizeof(a[0]);
        test(std::vector<int, min_allocator<int>>(a, an), min_allocator<int>());
        test(std::vector<int, safe_allocator<int>>(a, an), safe_allocator<int>());
    }
    {
        std::vector<int, min_allocator<int> > l(3, 2, min_allocator<int>());
        std::vector<int, min_allocator<int> > l2(l, min_allocator<int>());
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == min_allocator<int>());
    }
    {
      std::vector<int, safe_allocator<int> > l(3, 2, safe_allocator<int>());
      std::vector<int, safe_allocator<int> > l2(l, safe_allocator<int>());
      TC_ASSERT_EXPR(l2 == l);
      TC_ASSERT_EXPR(l2.get_allocator() == safe_allocator<int>());
    }
#endif

    return true;
}

int tc_containers_sequences_vector_vector_cons_copy_alloc(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
