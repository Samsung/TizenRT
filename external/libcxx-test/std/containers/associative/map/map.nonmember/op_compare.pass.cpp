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
// <map>

// template<class Key, class T, class Compare, class Alloc>
// bool operator==(const std::map<Key, T, Compare, Alloc>& lhs,
//                 const std::map<Key, T, Compare, Alloc>& rhs);
//
// template<class Key, class T, class Compare, class Alloc>
// bool operator!=(const std::map<Key, T, Compare, Alloc>& lhs,
//                 const std::map<Key, T, Compare, Alloc>& rhs);
//
// template<class Key, class T, class Compare, class Alloc>
// bool operator<(const std::map<Key, T, Compare, Alloc>& lhs,
//                const std::map<Key, T, Compare, Alloc>& rhs);
//
// template<class Key, class T, class Compare, class Alloc>
// bool operator>(const std::map<Key, T, Compare, Alloc>& lhs,
//                const std::map<Key, T, Compare, Alloc>& rhs);
//
// template<class Key, class T, class Compare, class Alloc>
// bool operator<=(const std::map<Key, T, Compare, Alloc>& lhs,
//                 const std::map<Key, T, Compare, Alloc>& rhs);
//
// template<class Key, class T, class Compare, class Alloc>
// bool operator>=(const std::map<Key, T, Compare, Alloc>& lhs,
//                 const std::map<Key, T, Compare, Alloc>& rhs);

#include <map>
#include <cassert>
#include <string>

#include "test_comparisons.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_map_map_nonmember_op_compare(void) {
    typedef std::map<int, std::string> map_type;
    typedef map_type::value_type value_type;
    {
        map_type m1, m2;
        m1.insert(value_type(1, "abc"));
        m2.insert(value_type(2, "abc"));
        const map_type& cm1 = m1, cm2 = m2;
        TC_ASSERT_EXPR(testComparisons(cm1, cm2, false, true));
    }
    {
        map_type m1, m2;
        m1.insert(value_type(1, "abc"));
        m2.insert(value_type(1, "abc"));
        const map_type& cm1 = m1, cm2 = m2;
        TC_ASSERT_EXPR(testComparisons(cm1, cm2, true, false));
    }
    {
        map_type m1, m2;
        m1.insert(value_type(1, "ab"));
        m2.insert(value_type(1, "abc"));
        const map_type& cm1 = m1, cm2 = m2;
        TC_ASSERT_EXPR(testComparisons(cm1, cm2, false, true));
    }
    {
        map_type m1, m2;
        m1.insert(value_type(1, "abc"));
        m2.insert(value_type(1, "bcd"));
        const map_type& cm1 = m1, cm2 = m2;
        TC_ASSERT_EXPR(testComparisons(cm1, cm2, false, true));
    }
    {
        map_type m1, m2;
        m1.insert(value_type(1, "abc"));
        m2.insert(value_type(1, "abc"));
        m2.insert(value_type(2, "abc"));
        const map_type& cm1 = m1, cm2 = m2;
        TC_ASSERT_EXPR(testComparisons(cm1, cm2, false, true));
    }
    return 0;
}
