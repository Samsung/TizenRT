//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11
// <vector>
//  vector<bool>

// template <class... Args> iterator emplace(const_iterator pos, Args&&... args);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        typedef std::vector<bool> C;
        C c;

        C::iterator i = c.emplace(c.cbegin());
        TC_ASSERT_EXPR(i == c.begin());
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front() == false);

        i = c.emplace(c.cend(), true);
        TC_ASSERT_EXPR(i == c.end()-1);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c.back() == true);

        i = c.emplace(c.cbegin()+1, true);
        TC_ASSERT_EXPR(i == c.begin()+1);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c[1] == true);
        TC_ASSERT_EXPR(c.back() == true);
    }
    {
        typedef std::vector<bool, min_allocator<bool>> C;
        C c;

        C::iterator i = c.emplace(c.cbegin());
        TC_ASSERT_EXPR(i == c.begin());
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front() == false);

        i = c.emplace(c.cend(), true);
        TC_ASSERT_EXPR(i == c.end()-1);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c.back() == true);

        i = c.emplace(c.cbegin()+1, true);
        TC_ASSERT_EXPR(i == c.begin()+1);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.front() == false);
        TC_ASSERT_EXPR(c[1] == true);
        TC_ASSERT_EXPR(c.back() == true);
    }

    return true;
}

int tc_containers_sequences_vector_bool_emplace(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
