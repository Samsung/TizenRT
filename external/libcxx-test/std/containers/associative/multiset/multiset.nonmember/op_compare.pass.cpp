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
// <set>

// template<class Key, class Compare, class Alloc>
// bool operator==(const std::multiset<Key, Compare, Alloc>& lhs,
//                 const std::multiset<Key, Compare, Alloc>& rhs);
//
// template<class Key, class Compare, class Alloc>
// bool operator!=(const std::multiset<Key, Compare, Alloc>& lhs,
//                 const std::multiset<Key, Compare, Alloc>& rhs);
//
// template<class Key, class Compare, class Alloc>
// bool operator<(const std::multiset<Key, Compare, Alloc>& lhs,
//                const std::multiset<Key, Compare, Alloc>& rhs);
//
// template<class Key, class Compare, class Alloc>
// bool operator>(const std::multiset<Key, Compare, Alloc>& lhs,
//                const std::multiset<Key, Compare, Alloc>& rhs);
//
// template<class Key, class Compare, class Alloc>
// bool operator<=(const std::multiset<Key, Compare, Alloc>& lhs,
//                 const std::multiset<Key, Compare, Alloc>& rhs);
//
// template<class Key, class Compare, class Alloc>
// bool operator>=(const std::multiset<Key, Compare, Alloc>& lhs,
//                 const std::multiset<Key, Compare, Alloc>& rhs);

#include <set>
#include <cassert>
#include <string>

#include "test_comparisons.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_nonmember_op_compare(void) {
    {
        std::multiset<int> s1, s2;
        s1.insert(1);
        s2.insert(2);
        const std::multiset<int>& cs1 = s1, cs2 = s2;
        TC_ASSERT_EXPR(testComparisons(cs1, cs2, false, true));
    }
    {
        std::multiset<int> s1, s2;
        s1.insert(1);
        s2.insert(1);
        const std::multiset<int>& cs1 = s1, cs2 = s2;
        TC_ASSERT_EXPR(testComparisons(cs1, cs2, true, false));
    }
    {
        std::multiset<int> s1, s2;
        s1.insert(1);
        s2.insert(1);
        s2.insert(2);
        const std::multiset<int>& cs1 = s1, cs2 = s2;
        TC_ASSERT_EXPR(testComparisons(cs1, cs2, false, true));
    }
    {
        std::multiset<int> s1, s2;
        s1.insert(1);
        s2.insert(1);
        s2.insert(1);
        s2.insert(1);
        const std::multiset<int>& cs1 = s1, cs2 = s2;
        TC_ASSERT_EXPR(testComparisons(cs1, cs2, false, true));
    }
    return 0;
}
