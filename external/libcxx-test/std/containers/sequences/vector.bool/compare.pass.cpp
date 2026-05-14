//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <vector>

// bool operator==( const vector& lhs, const vector& rhs );
// bool operator!=( const vector& lhs, const vector& rhs );
// bool operator< ( const vector& lhs, const vector& rhs );
// bool operator<=( const vector& lhs, const vector& rhs );
// bool operator> ( const vector& lhs, const vector& rhs );
// bool operator>=( const vector& lhs, const vector& rhs );

#include <vector>
#include <cassert>

#include "test_comparisons.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool test() {
    typedef std::vector<bool> VB;
    {
        const VB v1, v2;
        TC_ASSERT_EXPR(testComparisons(v1, v2, true, false));
    }
    {
        const VB v1(1, true);
        const VB v2(1, true);
        TC_ASSERT_EXPR(testComparisons(v1, v2, true, false));
    }
    {
        const VB v1(1, false);
        const VB v2(1, true);
        TC_ASSERT_EXPR(testComparisons(v1, v2, false, true));
    }
    {
        const VB v1, v2(1, true);
        TC_ASSERT_EXPR(testComparisons(v1, v2, false, true));
    }
    {
        bool items1[3] = {false, true, false};
        bool items2[3] = {false, true, true};
        const VB v1(items1, items1 + 3);
        const VB v2(items2, items2 + 3);
        TC_ASSERT_EXPR(testComparisons(v1, v2, false, true));
    }
    {
        bool items1[3] = {false, false, false};
        bool items2[3] = {false, true, false};
        const VB v1(items1, items1 + 3);
        const VB v2(items2, items2 + 3);
        TC_ASSERT_EXPR(testComparisons(v1, v2, false, true));
    }
    {
        bool items1[2] = {false, true};
        bool items2[3] = {false, true, false};
        const VB v1(items1, items1 + 2);
        const VB v2(items2, items2 + 3);
        TC_ASSERT_EXPR(testComparisons(v1, v2, false, true));
    }
    {
        bool items[3] = {false, true, false};
        const VB v1(items, items + 3);
        const VB v2(1, true);
        TC_ASSERT_EXPR(testComparisons(v1, v2, false, true));
    }
    {
        TC_ASSERT_EXPR( (std::vector<bool>() == std::vector<bool>()));
        TC_ASSERT_EXPR(!(std::vector<bool>() != std::vector<bool>()));
        TC_ASSERT_EXPR(!(std::vector<bool>() < std::vector<bool>()));
        TC_ASSERT_EXPR( (std::vector<bool>() <= std::vector<bool>()));
        TC_ASSERT_EXPR(!(std::vector<bool>() > std::vector<bool>()));
        TC_ASSERT_EXPR( (std::vector<bool>() >= std::vector<bool>()));
    }

    return true;
}

int tc_containers_sequences_vector_bool_compare(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

    return 0;
}
