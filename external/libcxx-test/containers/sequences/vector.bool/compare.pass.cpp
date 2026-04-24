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
