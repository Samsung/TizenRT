//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <vector>

// void pop_back();

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"


TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        std::vector<int> c;
        c.push_back(1);
        TC_ASSERT_EXPR(c.size() == 1);
        c.pop_back();
        TC_ASSERT_EXPR(c.size() == 0);

    }
#if TEST_STD_VER >= 11
    {
        std::vector<int, min_allocator<int>> c;
        c.push_back(1);
        TC_ASSERT_EXPR(c.size() == 1);
        c.pop_back();
        TC_ASSERT_EXPR(c.size() == 0);
    }
#endif

    { // LWG 526
        int arr[] = {0, 1, 2, 3, 4};
        int sz = 5;
        std::vector<int> c(arr, arr+sz);
        while (c.size() < c.capacity())
            c.push_back(sz++);
        c.push_back(c.front());
        TC_ASSERT_EXPR(c.back() == 0);
        for (int i = 0; i < sz; ++i)
            TC_ASSERT_EXPR(c[i] == i);
    }

    return true;
}

int tc_containers_sequences_vector_vector_modifiers_pop_back(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
