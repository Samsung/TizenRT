//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <vector>

// class vector

// size_type size() const noexcept;

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
    ASSERT_NOEXCEPT(c.size());
    TC_ASSERT_EXPR(c.size() == 0);
    c.push_back(false);
    TC_ASSERT_EXPR(c.size() == 1);
    c.push_back(true);
    TC_ASSERT_EXPR(c.size() == 2);
    c.push_back(false);
    TC_ASSERT_EXPR(c.size() == 3);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 2);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 1);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 0);
    }
#if TEST_STD_VER >= 11
    {
    typedef std::vector<bool, min_allocator<bool>> C;
    C c;
    ASSERT_NOEXCEPT(c.size());
    TC_ASSERT_EXPR(c.size() == 0);
    c.push_back(false);
    TC_ASSERT_EXPR(c.size() == 1);
    c.push_back(true);
    TC_ASSERT_EXPR(c.size() == 2);
    c.push_back(false);
    TC_ASSERT_EXPR(c.size() == 3);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 2);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 1);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 0);
    }
#endif

    return true;
}

int tc_containers_sequences_vector_bool_size(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
